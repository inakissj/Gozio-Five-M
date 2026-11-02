#pragma once
#include <Includes/Includes.hpp>
#include <Core/SDK/SDK.hpp>
#include <Core/Config.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib")

namespace Core
{
	namespace Web
	{
		class cWebServer
		{
		private:
			std::atomic<bool> m_Running{ false };
			std::thread m_ServerThread;
			SOCKET m_ListenSocket = INVALID_SOCKET;
			int m_Port = 8080;

			std::string GetMimeType(const std::string& path)
			{
				if (path.find(".html") != std::string::npos || path == "/") return "text/html; charset=utf-8";
				if (path.find(".json") != std::string::npos) return "application/json";
				if (path.find(".js") != std::string::npos) return "application/javascript";
				if (path.find(".css") != std::string::npos) return "text/css";
				return "text/plain";
			}

			std::string UrlDecode(const std::string& str)
			{
				std::string result;
				for (size_t i = 0; i < str.length(); ++i)
				{
					if (str[i] == '+')
					{
						result += ' ';
					}
					else if (str[i] == '%' && i + 2 < str.length())
					{
						int val = 0;
						std::istringstream is(str.substr(i + 1, 2));
						if (is >> std::hex >> val)
						{
							result += (char)val;
							i += 2;
						}
						else
						{
							result += str[i];
						}
					}
					else
					{
						result += str[i];
					}
				}
				return result;
			}

			std::unordered_map<std::string, std::string> ParseQueryParams(const std::string& query)
			{
				std::unordered_map<std::string, std::string> params;
				std::stringstream ss(query);
				std::string item;
				while (std::getline(ss, item, '&'))
				{
					size_t eq = item.find('=');
					if (eq != std::string::npos)
					{
						std::string key = UrlDecode(item.substr(0, eq));
						std::string val = UrlDecode(item.substr(eq + 1));
						params[key] = val;
					}
				}
				return params;
			}

			std::string BuildStatusJson()
			{
				std::ostringstream ss;
				ss << std::fixed << std::setprecision(2);
				ss << "{\n";
				ss << "  \"connected\": " << (g_Variables.ProcIdFiveM ? "true" : "false") << ",\n";
				ss << "  \"pid\": " << g_Variables.ProcIdFiveM << ",\n";
				ss << "  \"build\": " << g_Offsets.CurrentBuild << ",\n";

				if (Core::SDK::Pointers::pLocalPlayer)
				{
					D3DXVECTOR3 pos = Core::SDK::Pointers::pLocalPlayer->GetPos();
					float health = Core::SDK::Pointers::pLocalPlayer->GetHealth();
					float maxHealth = Core::SDK::Pointers::pLocalPlayer->GetMaxHealth();
					float armor = Core::SDK::Pointers::pLocalPlayer->GetArmor();

					ss << "  \"local\": {\n";
					ss << "    \"x\": " << pos.x << ",\n";
					ss << "    \"y\": " << pos.y << ",\n";
					ss << "    \"z\": " << pos.z << ",\n";
					ss << "    \"health\": " << health << ",\n";
					ss << "    \"max_health\": " << maxHealth << ",\n";
					ss << "    \"armor\": " << armor << ",\n";
					ss << "    \"godmode\": " << (g_Config.Player->EnableGodMode ? "true" : "false") << ",\n";
					ss << "    \"noclip\": " << (g_Config.Player->NoClipEnabled ? "true" : "false") << "\n";
					ss << "  }\n";
				}
				else
				{
					ss << "  \"local\": null\n";
				}
				ss << "}";
				return ss.str();
			}

			std::string BuildBlipsJson()
			{
				std::ostringstream ss;
				ss << std::fixed << std::setprecision(2);
				ss << "{\n";

				// Local player
				if (Core::SDK::Pointers::pLocalPlayer)
				{
					D3DXVECTOR3 pos = Core::SDK::Pointers::pLocalPlayer->GetPos();
					ss << "  \"local\": {\"x\": " << pos.x << ", \"y\": " << pos.y << ", \"z\": " << pos.z
					   << ", \"health\": " << Core::SDK::Pointers::pLocalPlayer->GetHealth()
					   << ", \"armor\": " << Core::SDK::Pointers::pLocalPlayer->GetArmor() << "},\n";
				}
				else
				{
					ss << "  \"local\": null,\n";
				}

				// Players
				ss << "  \"players\": [\n";
				size_t pCount = Core::SDK::Game::EntityList.size();
				int pWritten = 0;
				for (size_t i = 0; i < pCount; i++)
				{
					const auto& ent = Core::SDK::Game::EntityList[i];
					if (ent.Ped == Core::SDK::Pointers::pLocalPlayer) continue;
					if (ent.PedType != 2) continue; // players only

					if (pWritten > 0) ss << ",\n";

					// Escape JSON string for player name
					std::string cleanName = ent.NetworkInfo.UserName;
					std::string escapedName;
					for (char c : cleanName)
					{
						if (c == '"' || c == '\\') escapedName += '\\';
						escapedName += c;
					}

					ss << "    {\"id\": " << ent.Id
					   << ", \"name\": \"" << escapedName << "\""
					   << ", \"x\": " << ent.Pos.x
					   << ", \"y\": " << ent.Pos.y
					   << ", \"z\": " << ent.Pos.z
					   << ", \"dist\": " << ent.Distance
					   << ", \"health\": " << ent.Health
					   << ", \"armor\": " << ent.Armor
					   << ", \"is_admin\": " << (ent.IsAdmin ? "true" : "false")
					   << ", \"is_friend\": " << (ent.IsFriend ? "true" : "false")
					   << ", \"weapon\": \"" << ent.WeaponName << "\"}";
					pWritten++;
				}
				ss << "\n  ],\n";

				// Vehicles
				ss << "  \"vehicles\": [\n";
				size_t vCount = Core::SDK::Game::VehicleList.size();
				int vWritten = 0;
				for (size_t i = 0; i < vCount; i++)
				{
					const auto& veh = Core::SDK::Game::VehicleList[i];
					if (vWritten > 0) ss << ",\n";

					std::string cleanVehName = veh.Name;
					std::string escapedVeh;
					for (char c : cleanVehName)
					{
						if (c == '"' || c == '\\') escapedVeh += '\\';
						escapedVeh += c;
					}

					ss << "    {\"name\": \"" << escapedVeh << "\""
					   << ", \"x\": " << veh.Pos.x
					   << ", \"y\": " << veh.Pos.y
					   << ", \"z\": " << veh.Pos.z
					   << ", \"dist\": " << veh.Dist
					   << ", \"has_driver\": " << (veh.Driver != nullptr ? "true" : "false") << "}";
					vWritten++;
				}
				ss << "\n  ],\n";

				// Native GTA Map Blips (from g_Offsets.m_BlipList)
				ss << "  \"map_blips\": [\n";
				int bWritten = 0;
				if (g_Offsets.m_BlipList && g_Variables.ProcIdFiveM)
				{
					for (int i = 0; i < 1500 && bWritten < 64; i++)
					{
						uintptr_t blipPtr = Mem.Read<uintptr_t>(g_Offsets.m_BlipList + (i * 8));
						if (!blipPtr) continue;

						int icon = Mem.Read<int>(blipPtr + 0x40);
						if (icon <= 0 || icon > 900) continue;

						D3DXVECTOR3 bPos = Mem.Read<D3DXVECTOR3>(blipPtr + 0x10);
						if (bPos.x < -5000.f || bPos.x > 6000.f || bPos.y < -5000.f || bPos.y > 9000.f)
							continue;
						if (bPos.x == 0.f && bPos.y == 0.f)
							continue;

						uint8_t color = Mem.Read<uint8_t>(blipPtr + 0x48);
						if (bWritten > 0) ss << ",\n";
						ss << "    {\"sprite\": " << icon
						   << ", \"color\": " << (int)color
						   << ", \"x\": " << bPos.x
						   << ", \"y\": " << bPos.y
						   << ", \"z\": " << bPos.z << "}";
						bWritten++;
					}
				}
				ss << "\n  ]\n";
				ss << "}";
				return ss.str();
			}

			std::string BuildConfigJson()
			{
				std::ostringstream ss;
				ss << "{\n";
				// ESP
				ss << "  \"esp_enabled\": " << (g_Config.ESP->Enabled ? "true" : "false") << ",\n";
				ss << "  \"esp_box\": " << (g_Config.ESP->Box ? "true" : "false") << ",\n";
				ss << "  \"esp_skeleton\": " << (g_Config.ESP->Skeleton ? "true" : "false") << ",\n";
				ss << "  \"esp_snaplines\": " << (g_Config.ESP->SnapLines ? "true" : "false") << ",\n";
				ss << "  \"esp_health\": " << (g_Config.ESP->HealthBar ? "true" : "false") << ",\n";
				ss << "  \"esp_armor\": " << (g_Config.ESP->ArmorBar ? "true" : "false") << ",\n";
				ss << "  \"esp_names\": " << (g_Config.ESP->UserNames ? "true" : "false") << ",\n";
				ss << "  \"esp_weapon\": " << (g_Config.ESP->WeaponName ? "true" : "false") << ",\n";
				ss << "  \"esp_headcircle\": " << (g_Config.ESP->HeadCircle ? "true" : "false") << ",\n";
				ss << "  \"esp_distance\": " << (g_Config.ESP->DistanceFromMe ? "true" : "false") << ",\n";
				ss << "  \"esp_ignore_npcs\": " << (g_Config.ESP->IgnoreNPCs ? "true" : "false") << ",\n";
				ss << "  \"esp_ignore_dead\": " << (g_Config.ESP->IgnoreDead ? "true" : "false") << ",\n";
				ss << "  \"esp_max_distance\": " << g_Config.ESP->MaxDistance << ",\n";
				ss << "  \"esp_admin\": " << (g_Config.ESP->AdminESP ? "true" : "false") << ",\n";
				ss << "  \"esp_admin_rainbow\": " << (g_Config.ESP->AdminRainbow ? "true" : "false") << ",\n";

				// Vehicle ESP
				ss << "  \"vehesp_enabled\": " << (g_Config.VehicleESP->Enabled ? "true" : "false") << ",\n";
				ss << "  \"vehesp_names\": " << (g_Config.VehicleESP->VehName ? "true" : "false") << ",\n";
				ss << "  \"vehesp_snaplines\": " << (g_Config.VehicleESP->SnapLines ? "true" : "false") << ",\n";
				ss << "  \"vehesp_distance\": " << (g_Config.VehicleESP->DistanceFromMe ? "true" : "false") << ",\n";

				// Combat
				ss << "  \"aimbot_enabled\": " << (g_Config.Aimbot->Enabled ? "true" : "false") << ",\n";
				ss << "  \"aimbot_fov\": " << g_Config.Aimbot->FOV << ",\n";
				ss << "  \"aimbot_speed\": " << g_Config.Aimbot->AimbotSpeed << ",\n";
				ss << "  \"aimbot_distance\": " << g_Config.Aimbot->MaxDistance << ",\n";
				ss << "  \"aimbot_visible\": " << (g_Config.Aimbot->OnlyVisible ? "true" : "false") << ",\n";
				ss << "  \"silentaim_enabled\": " << (g_Config.SilentAim->Enabled ? "true" : "false") << ",\n";
				ss << "  \"silentaim_fov\": " << g_Config.SilentAim->FOV << ",\n";
				ss << "  \"silentaim_distance\": " << g_Config.SilentAim->MaxDistance << ",\n";
				ss << "  \"triggerbot_enabled\": " << (g_Config.TriggerBot->Enabled ? "true" : "false") << ",\n";

				// Exploits
				ss << "  \"godmode\": " << (g_Config.Player->EnableGodMode ? "true" : "false") << ",\n";
				ss << "  \"noclip\": " << (g_Config.Player->NoClipEnabled ? "true" : "false") << ",\n";
				ss << "  \"noclip_speed\": " << g_Config.Player->NoClipSpeed << ",\n";
				ss << "  \"fastrun\": " << (g_Config.Player->FastRun ? "true" : "false") << ",\n";
				ss << "  \"stamina\": " << (g_Config.Player->InfiniteStamina ? "true" : "false") << ",\n";
				ss << "  \"ammo\": " << (g_Config.Player->InfiniteAmmoEnabled ? "true" : "false") << ",\n";
				ss << "  \"norecoil\": " << (g_Config.Player->NoRecoilEnabled ? "true" : "false") << ",\n";
				ss << "  \"nospread\": " << (g_Config.Player->NoSpreadEnabled ? "true" : "false") << "\n";
				ss << "}";
				return ss.str();
			}

			std::string HandleApiSet(const std::unordered_map<std::string, std::string>& params)
			{
				for (const auto& [key, val] : params)
				{
					bool bVal = (val == "true" || val == "1");

					// ESP
					if (key == "esp_enabled") g_Config.ESP->Enabled = bVal;
					else if (key == "esp_box") g_Config.ESP->Box = bVal;
					else if (key == "esp_skeleton") {
						g_Config.ESP->Skeleton = bVal;
						if (bVal && Core::SDK::Pointers::pLocalPlayer)
							Core::SDK::Pointers::pLocalPlayer->RemoveKinematics();
					}
					else if (key == "esp_snaplines") g_Config.ESP->SnapLines = bVal;
					else if (key == "esp_health") g_Config.ESP->HealthBar = bVal;
					else if (key == "esp_armor") g_Config.ESP->ArmorBar = bVal;
					else if (key == "esp_names") g_Config.ESP->UserNames = bVal;
					else if (key == "esp_weapon") g_Config.ESP->WeaponName = bVal;
					else if (key == "esp_headcircle") g_Config.ESP->HeadCircle = bVal;
					else if (key == "esp_distance") g_Config.ESP->DistanceFromMe = bVal;
					else if (key == "esp_ignore_npcs") g_Config.ESP->IgnoreNPCs = bVal;
					else if (key == "esp_ignore_dead") g_Config.ESP->IgnoreDead = bVal;
					else if (key == "esp_max_distance") g_Config.ESP->MaxDistance = std::atoi(val.c_str());
					else if (key == "esp_admin") g_Config.ESP->AdminESP = bVal;
					else if (key == "esp_admin_rainbow") g_Config.ESP->AdminRainbow = bVal;

					// Vehicle ESP
					else if (key == "vehesp_enabled") g_Config.VehicleESP->Enabled = bVal;
					else if (key == "vehesp_names") g_Config.VehicleESP->VehName = bVal;
					else if (key == "vehesp_snaplines") g_Config.VehicleESP->SnapLines = bVal;
					else if (key == "vehesp_distance") g_Config.VehicleESP->DistanceFromMe = bVal;

					// Combat
					else if (key == "aimbot_enabled") g_Config.Aimbot->Enabled = bVal;
					else if (key == "aimbot_fov") g_Config.Aimbot->FOV = std::atoi(val.c_str());
					else if (key == "aimbot_speed") g_Config.Aimbot->AimbotSpeed = std::atoi(val.c_str());
					else if (key == "aimbot_distance") g_Config.Aimbot->MaxDistance = std::atoi(val.c_str());
					else if (key == "aimbot_visible") g_Config.Aimbot->OnlyVisible = bVal;
					else if (key == "silentaim_enabled") g_Config.SilentAim->Enabled = bVal;
					else if (key == "silentaim_fov") g_Config.SilentAim->FOV = std::atoi(val.c_str());
					else if (key == "silentaim_distance") g_Config.SilentAim->MaxDistance = std::atoi(val.c_str());
					else if (key == "triggerbot_enabled") g_Config.TriggerBot->Enabled = bVal;

					// Exploits
					else if (key == "godmode") {
						g_Config.Player->EnableGodMode = bVal;
						if (Core::SDK::Pointers::pLocalPlayer)
							Core::SDK::Pointers::pLocalPlayer->SetGodMode(bVal);
					}
					else if (key == "noclip") {
						g_Config.Player->NoClipEnabled = bVal;
						if (Core::SDK::Pointers::pLocalPlayer)
							Core::SDK::Pointers::pLocalPlayer->FreezePed(bVal);
					}
					else if (key == "noclip_speed") g_Config.Player->NoClipSpeed = (float)std::atof(val.c_str());
					else if (key == "fastrun") g_Config.Player->FastRun = bVal;
					else if (key == "stamina") g_Config.Player->InfiniteStamina = bVal;
					else if (key == "ammo") g_Config.Player->InfiniteAmmoEnabled = bVal;
					else if (key == "norecoil") g_Config.Player->NoRecoilEnabled = bVal;
					else if (key == "nospread") g_Config.Player->NoSpreadEnabled = bVal;
				}
				return "{\"status\": \"ok\"}";
			}

			std::string HandleTeleport(const std::unordered_map<std::string, std::string>& params)
			{
				if (!Core::SDK::Pointers::pLocalPlayer)
					return "{\"status\": \"error\", \"message\": \"Local player not available\"}";

				auto itX = params.find("x");
				auto itY = params.find("y");
				if (itX == params.end() || itY == params.end())
					return "{\"status\": \"error\", \"message\": \"Missing coordinates\"}";

				float x = (float)std::atof(itX->second.c_str());
				float y = (float)std::atof(itY->second.c_str());
				float z = 30.0f; // Default elevation

				auto itZ = params.find("z");
				if (itZ != params.end())
					z = (float)std::atof(itZ->second.c_str());
				else
					z = Core::SDK::Pointers::pLocalPlayer->GetPos().z + 1.0f;

				Core::SDK::Pointers::pLocalPlayer->SetPos(D3DXVECTOR3(x, y, z));
				return "{\"status\": \"ok\", \"teleported_to\": {\"x\":" + std::to_string(x) + ",\"y\":" + std::to_string(y) + ",\"z\":" + std::to_string(z) + "}}";
			}

			std::string GetHtmlPage();

			void HandleClient(SOCKET clientSocket)
			{
				char buffer[4096];
				int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
				if (bytesReceived <= 0)
				{
					closesocket(clientSocket);
					return;
				}
				buffer[bytesReceived] = '\0';

				std::string request(buffer);
				std::istringstream reqStream(request);
				std::string method, fullPath, httpVersion;
				reqStream >> method >> fullPath >> httpVersion;

				std::string path = fullPath;
				std::string query;
				size_t qPos = fullPath.find('?');
				if (qPos != std::string::npos)
				{
					path = fullPath.substr(0, qPos);
					query = fullPath.substr(qPos + 1);
				}

				auto params = ParseQueryParams(query);

				std::string responseBody;
				std::string contentType = "application/json";
				int statusCode = 200;

				if (path == "/" || path == "/index.html")
				{
					responseBody = GetHtmlPage();
					contentType = "text/html; charset=utf-8";
				}
				else if (path == "/api/status")
				{
					responseBody = BuildStatusJson();
				}
				else if (path == "/api/blips" || path == "/api/players")
				{
					responseBody = BuildBlipsJson();
				}
				else if (path == "/api/config")
				{
					responseBody = BuildConfigJson();
				}
				else if (path == "/api/set")
				{
					responseBody = HandleApiSet(params);
				}
				else if (path == "/api/teleport")
				{
					responseBody = HandleTeleport(params);
				}
				else if (path == "/api/admin/add")
				{
					auto it = params.find("name");
					if (it != params.end())
					{
						std::string lower = it->second;
						std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return (char)::tolower(c); });
						Core::SDK::Game::AdminCustomNames.insert(lower);
						responseBody = "{\"status\": \"ok\", \"added\": \"" + it->second + "\"}";
					}
					else
					{
						responseBody = "{\"status\": \"error\", \"message\": \"Missing name\"}";
					}
				}
				else if (path == "/api/admin/remove")
				{
					auto it = params.find("name");
					if (it != params.end())
					{
						std::string lower = it->second;
						std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return (char)::tolower(c); });
						Core::SDK::Game::AdminCustomNames.erase(lower);
						responseBody = "{\"status\": \"ok\", \"removed\": \"" + it->second + "\"}";
					}
					else
					{
						responseBody = "{\"status\": \"error\", \"message\": \"Missing name\"}";
					}
				}
				else
				{
					statusCode = 404;
					responseBody = "{\"error\": \"Not Found\"}";
				}

				std::ostringstream response;
				response << "HTTP/1.1 " << statusCode << " OK\r\n";
				response << "Content-Type: " << contentType << "\r\n";
				response << "Content-Length: " << responseBody.length() << "\r\n";
				response << "Access-Control-Allow-Origin: *\r\n";
				response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
				response << "Access-Control-Allow-Headers: Content-Type\r\n";
				response << "Connection: close\r\n\r\n";
				response << responseBody;

				std::string respStr = response.str();
				send(clientSocket, respStr.c_str(), (int)respStr.length(), 0);
				closesocket(clientSocket);
			}

			void RunServer()
			{
				WSADATA wsaData;
				WSAStartup(MAKEWORD(2, 2), &wsaData);

				m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (m_ListenSocket == INVALID_SOCKET)
				{
					m_Running = false;
					return;
				}

				int opt = 1;
				setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

				sockaddr_in serverAddr{};
				serverAddr.sin_family = AF_INET;
				serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
				serverAddr.sin_port = htons(m_Port);

				if (::bind(m_ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
				{
					// Try alternate port 8081 if 8080 occupied
					m_Port = 8081;
					serverAddr.sin_port = htons(m_Port);
					if (::bind(m_ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
					{
						closesocket(m_ListenSocket);
						m_ListenSocket = INVALID_SOCKET;
						m_Running = false;
						return;
					}
				}

				if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
				{
					closesocket(m_ListenSocket);
					m_ListenSocket = INVALID_SOCKET;
					m_Running = false;
					return;
				}

				m_Running = true;
				g_Config.Web->Port = m_Port;
				g_Config.Web->Running = true;

				std::cout << "[WebRadar] Running at http://localhost:" << m_Port << std::endl;

				while (m_Running)
				{
					fd_set readSet;
					FD_ZERO(&readSet);
					FD_SET(m_ListenSocket, &readSet);

					timeval tv{ 1, 0 }; // 1 sec timeout for clean exit check
					int ret = select(0, &readSet, nullptr, nullptr, &tv);
					if (ret > 0 && FD_ISSET(m_ListenSocket, &readSet))
					{
						sockaddr_in clientAddr{};
						int clientLen = sizeof(clientAddr);
						SOCKET client = accept(m_ListenSocket, (sockaddr*)&clientAddr, &clientLen);
						if (client != INVALID_SOCKET)
						{
							std::thread([this, client]() {
								HandleClient(client);
							}).detach();
						}
					}
				}

				closesocket(m_ListenSocket);
				m_ListenSocket = INVALID_SOCKET;
				g_Config.Web->Running = false;
			}

			static size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* out)
			{
				size_t totalSize = size * nmemb;
				if (out) out->append((char*)contents, totalSize);
				return totalSize;
			}

			void RunCloudSync()
			{
				while (m_Running)
				{
					if (g_Config.Web->CloudSync && strlen(g_Config.Web->CloudUrl) > 4)
					{
						CURL* curl = curl_easy_init();
						if (curl)
						{
							std::string baseUrl(g_Config.Web->CloudUrl);
							if (!baseUrl.empty() && baseUrl.back() == '/')
								baseUrl.pop_back();
							std::string syncUrl = baseUrl + "/api/sync";

							std::string payload = "{\"blips\":" + BuildBlipsJson() + ",\"config\":" + BuildConfigJson() + "}";
							std::string responseBody;

							struct curl_slist* headers = nullptr;
							headers = curl_slist_append(headers, "Content-Type: application/json");

							curl_easy_setopt(curl, CURLOPT_URL, syncUrl.c_str());
							curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
							curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
							curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
							curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1500L);
							curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

							CURLcode res = curl_easy_perform(curl);
							if (res == CURLE_OK && !responseBody.empty())
							{
								try
								{
									auto j = nlohmann::json::parse(responseBody);
									if (j.contains("commands") && j["commands"].is_array())
									{
										for (const auto& cmd : j["commands"])
										{
											std::string type = cmd.value("type", "");
											if (type == "set_config" && cmd.contains("params") && cmd["params"].is_object())
											{
												std::unordered_map<std::string, std::string> pmap;
												for (auto it = cmd["params"].begin(); it != cmd["params"].end(); ++it)
												{
													if (it.value().is_string())
														pmap[it.key()] = it.value().get<std::string>();
													else
														pmap[it.key()] = it.value().dump();
												}
												HandleApiSet(pmap);
											}
											else if (type == "teleport")
											{
												std::unordered_map<std::string, std::string> pmap;
												pmap["x"] = std::to_string(cmd.value("x", 0.0f));
												pmap["y"] = std::to_string(cmd.value("y", 0.0f));
												pmap["z"] = std::to_string(cmd.value("z", 35.0f));
												HandleTeleport(pmap);
											}
											else if (type == "admin_add")
											{
												std::string name = cmd.value("name", "");
												if (!name.empty())
												{
													std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return (char)::tolower(c); });
													Core::SDK::Game::AdminCustomNames.insert(name);
												}
											}
											else if (type == "admin_remove")
											{
												std::string name = cmd.value("name", "");
												if (!name.empty())
												{
													std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return (char)::tolower(c); });
													Core::SDK::Game::AdminCustomNames.erase(name);
												}
											}
										}
									}
								}
								catch (...) {}
							}

							curl_slist_free_all(headers);
							curl_easy_cleanup(curl);
						}
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
				}
			}

		public:
			void Start(int port = 8080)
			{
				if (m_Running) return;
				m_Port = port;
				m_ServerThread = std::thread(&cWebServer::RunServer, this);
				m_ServerThread.detach();
				std::thread(&cWebServer::RunCloudSync, this).detach();
			}

			void Stop()
			{
				m_Running = false;
			}

			bool IsRunning() const { return m_Running; }
			int GetPort() const { return m_Port; }
		};

		inline cWebServer g_WebServer;
	}
}
