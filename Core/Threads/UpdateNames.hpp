#include <Includes/Includes.hpp>
#include <Includes/Utils.hpp>
#include <Core/Offsets.hpp>
#include <Core/Core.hpp>


#include <fstream>
#include <string>
#include <regex>
#include <iostream>

#define CURL_STATIC_LIB
#include <Security/Api/curl/curl.h>
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "Normaliz.lib" )
#pragma comment( lib, "Crypt32.lib" )
#pragma comment( lib, "Wldap32.lib" )
#pragma comment( lib, "libcurl.lib" )


using json = nlohmann::json;

namespace Core
{
	namespace Threads
	{
		class cUpdateNames
		{
		private:
			std::string ServerIp;
			std::string ServerToken;
			std::string DirFiveM;
			std::string RedirectUrl;
		public:
			std::unordered_map<int, Core::SDK::Game::NetworkInfo> NetworkMap;
		private:
			static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp)
			{
				((std::string*)userp)->append((char*)contents, size * nmemb);
				return size * nmemb;
			}

			std::string ExtractIp(const std::string& line)
			{
				std::string ip;

				size_t fist_serv = line.rfind(xorstr("last_server_url"));
				if (fist_serv != std::string::npos)
				{
					size_t start = line.find(xorstr("last_server"), fist_serv + 16 - 1);
					if (start != std::string::npos)
					{
						size_t last_serv = line.find(xorstr(":"), start);
						if (last_serv != std::string::npos)
						{
							size_t ip_start = start + 12 - 1;
							size_t ip_end = line.find(xorstr(":"), ip_start) + 6;
							if (ip_end != std::string::npos && ip_end > ip_start)
							{
								ip = line.substr(ip_start, ip_end - ip_start);
							}
						}
					}
				}

				return ip;
			}

			std::string GetServerToken() {

				if (DirFiveM.empty())
				{
					char value[255];
					DWORD BufferSize = 8192;

					auto GetDirFiveM = RegGetValue(HKEY_CURRENT_USER, xorstr("Software\\CitizenFX\\FiveM"), xorstr("Last Run Location"), RRF_RT_REG_SZ, NULL, (PVOID)&value, &BufferSize);

					if (GetDirFiveM != ERROR_SUCCESS)
						return xorstr("");

					DirFiveM = (std::string)value;
				}


				std::string CrashoMetryDir = DirFiveM + xorstr("data\\cache\\crashometry");
				std::ifstream File(CrashoMetryDir, std::ios::binary);

				if (!File)
					return xorstr("");

				std::string line;
				while (std::getline(File, line)) {
					size_t LastServer = line.find(xorstr("last_server"));

					if (LastServer != std::string::npos)
					{
						ServerIp = this->ExtractIp(line);
						break;
					}
				}

				File.close();

				if (ServerIp.empty()) {
					return xorstr("");
				}

				g_Variables.ServerIp = ServerIp;

				std::string ResponseStr;

				std::string ReqUrl = xorstr("http://") + ServerIp;

				CURL* hnd;
				CURLcode res;
				hnd = curl_easy_init();
				if (hnd) {
					curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, xorstr("GET"));
					curl_easy_setopt(hnd, CURLOPT_URL, ReqUrl.c_str());

					struct curl_slist* headers = NULL;
					curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

					curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallBack);
					curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &ResponseStr);

					curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
					curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 10L);

					res = curl_easy_perform(hnd);

					if (res == CURLE_OK)
					{
						char* FinalUrl;
						curl_easy_getinfo(hnd, CURLINFO_EFFECTIVE_URL, &FinalUrl);

						if (FinalUrl) {
							RedirectUrl = std::string(FinalUrl);
						}
					}

					curl_easy_cleanup(hnd);
				}

				if (ResponseStr.empty()) {
					return xorstr("");
				}

				auto pos = RedirectUrl.find_last_of('/');
				if (pos == std::string::npos)
					return xorstr("");

				std::string Token = RedirectUrl.substr(pos + 1);

				return Token;
			}
		public:

			nlohmann::json GetPlayerData() {
				ServerToken = GetServerToken();

				if (ServerToken.empty())
					return NULL;

				std::string ApiUrl = xorstr("https://servers-frontend.fivem.net/api/servers/single/") + ServerToken;

				std::string ResponseStr;
				CURL* hnd;
				CURLcode res;
				hnd = curl_easy_init();
				if (hnd) {
					curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, xorstr("GET"));
					curl_easy_setopt(hnd, CURLOPT_URL, ApiUrl.c_str());

					struct curl_slist* headers = NULL;
					headers = curl_slist_append(headers, xorstr("User-Agent: Gozio"));
					curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

					curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallBack);
					curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &ResponseStr);

					res = curl_easy_perform(hnd);

					curl_easy_cleanup(hnd);
				}

				if (ResponseStr.empty())
					return NULL;

				nlohmann::json ResponseJson = json::parse(ResponseStr);
				nlohmann::json ServerData = ResponseJson[xorstr("Data")];
				nlohmann::json PlayersArray = ServerData[xorstr("players")];

				return PlayersArray;
			}

			void GetPlayerNames()
			{
				nlohmann::json PlayersArr = GetPlayerData();

				if (PlayersArr == NULL)
					return;

				for (const auto& PlayerJson : PlayersArr)
				{
					nlohmann::json Player = json::parse(PlayerJson.dump());
					int PlayerId = Player[xorstr("id")].get<int>();
					std::string PlayerName = Player[xorstr("name")].get<std::string>();
					nlohmann::json Identifiers = Player[xorstr("identifiers")];

					std::string Discord, SteamId;

					if (Identifiers != NULL)
					{
						for (const auto& Identifier : Identifiers)
						{

							if (!Identifier.is_string())
								continue;

							std::string IdentifierVal = Identifier;

							if (Identifier.find("discord:") != Identifier.end())
								Discord = IdentifierVal.substr(8);
							if (Identifier.find("steam:") != Identifier.end())
								SteamId = IdentifierVal.substr(6);

						}
					}

					NetworkMap[PlayerId] =
					{
						PlayerName, Discord, SteamId
					};

				}
			}

			static std::string ReadMsvcString(uintptr_t strAddr)
			{
				if (!strAddr) return "";
				uint64_t capacity = Mem.Read<uint64_t>(strAddr + 0x18);
				if (capacity >= 16 && capacity < 4096)
				{
					uintptr_t strPtr = Mem.Read<uintptr_t>(strAddr);
					if (strPtr)
					{
						std::string s = Mem.ReadString(strPtr);
						if (!s.empty()) return s;
					}
				}
				return Mem.ReadString(strAddr);
			}

			void UpdateNamesFromMemory()
			{
				if (!g_Variables.ProcIdFiveM || !Mem.ProcHandle)
					return;

				// Dynamically resolve citizen-playernames-five.dll if not resolved during startup
				if (!g_Offsets.m_NetIdToNamesEntry)
				{
					uintptr_t citizenModSize = 0;
					g_Offsets.m_CitizenNamesModBase = Mem.GetModuleBaseAddr(g_Variables.ProcIdFiveM, "citizen-playernames-five.dll", &citizenModSize);
					if (g_Offsets.m_CitizenNamesModBase)
					{
						uintptr_t player_names = Mem.FindSignature(
							{ 0x48, 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x48, 0xC1, 0xE1 },
							g_Offsets.m_CitizenNamesModBase,
							citizenModSize
						);
						if (player_names)
						{
							g_Offsets.m_NetIdToNamesEntry = Mem.ResolveRelativeAddress(player_names, 3, 7);
						}
					}
				}

				if (!g_Offsets.m_NetIdToNamesEntry)
					return;

				// 1. Dereferenced bucket table traversal (standard FiveM NetIdToNamesPtr)
				uintptr_t tablePtr = Mem.Read<uintptr_t>(g_Offsets.m_NetIdToNamesEntry);
				if (tablePtr)
				{
					for (int i = 0; i < 512; i++)
					{
						uintptr_t node = Mem.Read<uintptr_t>(tablePtr + (i * 0x8));
						if (!node) continue;
						int depth = 0;
						while (node && depth < 30)
						{
							int netId = Mem.Read<int>(node + 0x10);
							if (netId >= 0 && netId < 65536)
							{
								std::string name = ReadMsvcString(node + 0x18);
								if (!name.empty() && name != xorstr("NPC") && name != xorstr("** Invalid **"))
								{
									NetworkMap[netId].UserName = name;
								}
							}
							node = Mem.Read<uintptr_t>(node + 0x00);
							depth++;
						}
					}
				}

				// 2. Direct entry table traversal
				for (int i = 0; i < 512; i++)
				{
					uintptr_t node = Mem.Read<uintptr_t>(g_Offsets.m_NetIdToNamesEntry + (i * 0x8));
					if (!node) continue;
					int depth = 0;
					while (node && depth < 30)
					{
						int netId = Mem.Read<int>(node + 0x10);
						if (netId >= 0 && netId < 65536)
						{
							std::string name = ReadMsvcString(node + 0x18);
							if (!name.empty() && name != xorstr("NPC") && name != xorstr("** Invalid **"))
							{
								NetworkMap[netId].UserName = name;
							}
						}
						node = Mem.Read<uintptr_t>(node + 0x00);
						depth++;
					}
				}

				// 3. std::unordered_map linked list traversal via _Myhead
				uintptr_t head = Mem.Read<uintptr_t>(g_Offsets.m_NetIdToNamesEntry);
				if (head)
				{
					uintptr_t curr = Mem.Read<uintptr_t>(head + 0x00);
					int visited = 0;
					while (curr && curr != head && visited < 1000)
					{
						int netId = Mem.Read<int>(curr + 0x10);
						if (netId >= 0 && netId < 65536)
						{
							std::string name = ReadMsvcString(curr + 0x18);
							if (!name.empty() && name != xorstr("NPC") && name != xorstr("** Invalid **"))
							{
								NetworkMap[netId].UserName = name;
							}
						}
						curr = Mem.Read<uintptr_t>(curr + 0x00);
						visited++;
					}
				}
			}

			std::string GetNameByNetId(int id)
			{
				auto it = NetworkMap.find(id);
				if (it != NetworkMap.end() && !it->second.UserName.empty() && it->second.UserName != xorstr("NPC"))
				{
					return it->second.UserName;
				}

				if (!g_Offsets.m_NetIdToNamesEntry) return "";

				uintptr_t tablePtr = Mem.Read<uintptr_t>(g_Offsets.m_NetIdToNamesEntry);
				if (tablePtr)
				{
					for (int i = 0; i < 256; i++)
					{
						uintptr_t node = Mem.Read<uintptr_t>(tablePtr + (i * 0x8));
						if (!node) continue;
						int depth = 0;
						while (node && depth < 20)
						{
							int netId = Mem.Read<int>(node + 0x10);
							if (netId == id)
							{
								std::string name = ReadMsvcString(node + 0x18);
								if (!name.empty())
								{
									NetworkMap[id].UserName = name;
									return name;
								}
							}
							node = Mem.Read<uintptr_t>(node + 0x00);
							depth++;
						}
					}
				}

				for (int i = 0; i < 256; i++)
				{
					uintptr_t node = Mem.Read<uintptr_t>(g_Offsets.m_NetIdToNamesEntry + (i * 0x8));
					if (!node) continue;
					int depth = 0;
					while (node && depth < 20)
					{
						int netId = Mem.Read<int>(node + 0x10);
						if (netId == id)
						{
							std::string name = ReadMsvcString(node + 0x18);
							if (!name.empty())
							{
								NetworkMap[id].UserName = name;
								return name;
							}
						}
						node = Mem.Read<uintptr_t>(node + 0x00);
						depth++;
					}
				}

				return "";
			}

			void Update()
			{
				int httpTick = 0;
				while (true)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					try {
						UpdateNamesFromMemory();

						httpTick++;
						if (httpTick >= 6)
						{
							httpTick = 0;
							GetPlayerNames();
						}
					}
					catch (const std::exception& e) {
						Sleep(1000);
						continue;
					}
					catch (...) {
						Sleep(1000);
						continue;
					}

				}
			}
		};

		inline cUpdateNames g_UpdateNames;
	}

}
