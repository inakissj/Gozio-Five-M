#pragma once
#include <Includes/Includes.hpp>
#include <Core/Offsets.hpp>
#include <Core/SDK/SDK.hpp>

// Threads
#include "Threads/ScreenResolution.hpp"
#include "Threads/UpdatePointers.hpp"
#include "Threads/UpdateNames.hpp"
#include "Threads/VehicleList.hpp"
#include "Threads/EntityList.hpp"


// Features
#include "Features/MagicBullets.hpp"
#include "Features/Triggerbot.hpp"
#include "Features/SilentAim.hpp"
#include "Features/Aimbot.hpp"
#include "Features/ESP.hpp"
#include "Features/AutoPeek.hpp"
//Exploits
#include "Features/Exploits/ResourceList.hpp"
#include "Web/WebPage.hpp"
#include <includes/lazyimporter.hpp>

#include <regex>

namespace Core {

	inline static bool ThreadsStarted = false;

	inline void StartThreads() {
		ThreadsStarted = false;

		//Info
		std::thread(&Threads::cScreenResolution::Update, Threads::g_ScreenResolution).detach();
		std::thread(&Threads::cUpdatePtrs::Update, Threads::g_UpdatePtrs).detach();
		std::thread(&Threads::cEntityList::Update, &Threads::g_EntityList).detach();
		std::thread(&Threads::cVehicleList::Update, &Threads::g_VehicleList).detach();
		std::thread(&Threads::cUpdateNames::Update, &Threads::g_UpdateNames).detach();

		//Exploits
		std::thread(&Features::Exploits::cResourceList::List, Features::Exploits::g_ResourceList).detach();

		//Features
		std::thread(&Features::cAimbot::Start, Features::g_Aimbot).detach();
		std::thread(&Features::cSilentAim::HookSilent, Features::g_SilentAim).detach();
		std::thread(&Features::cTriggerbot::Start, Features::g_Triggerbot).detach();
		std::thread(&Features::cMagicBullets::Start, Features::g_MagicBullets).detach();

		//Web Map Radar Server
		if (g_Config.Web->Enabled && !Web::g_WebServer.IsRunning()) {
			Web::g_WebServer.Start(g_Config.Web->Port);
		}

		ThreadsStarted = true;
	}


	inline std::pair<DWORD, std::wstring> GetProcessPidByName(const std::wstring& ProcessNamePattern) {
		HANDLE hSnapshot = SafeCall(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0);
		if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH)) {
			return { 0, L"" };
		}

		DWORD Pid = 0;
		std::wstring processName;
		PROCESSENTRY32 ProcessEntry;
		ProcessEntry.dwSize = sizeof(ProcessEntry);

		std::wregex pattern(ProcessNamePattern);
		if (SafeCall(Process32First)(hSnapshot, &ProcessEntry)) {
			do {
				wchar_t wExeFile[260];
				mbstowcs(wExeFile, ProcessEntry.szExeFile, sizeof(wExeFile) / sizeof(wchar_t));

				std::wstring exeFile(wExeFile);
				if (std::regex_match(exeFile, pattern)) {
					Pid = ProcessEntry.th32ProcessID;
					processName = exeFile;
					break;
				}
			} while (SafeCall(Process32Next)(hSnapshot, &ProcessEntry));
		}

		SafeCall(CloseHandle)(hSnapshot);
		return { Pid, processName };
	}

	inline std::string trim(const std::string& str) {
		size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos) return "";
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, last - first + 1);
	}

	inline int GetBuild() {
		if (!Mem.ProcName.empty()) {
			std::wsmatch match;
			if (std::regex_search(Mem.ProcName, match, std::wregex(L"b(\\d{4})"))) {
				return std::stoi(match[1]);
			}
		}

		std::wstring appDataPath = _wgetenv(L"LOCALAPPDATA");
		std::wstring iniFilePath = appDataPath + L"\\FiveM\\FiveM.app\\CitizenFX.ini";

		std::ifstream iniFile(iniFilePath);
		if (iniFile.is_open()) {
			std::string line;
			while (std::getline(iniFile, line)) {
				line = trim(line);
				if (line == "ReplaceExecutable=0") {
					return 3258;
				}
			}

			iniFile.clear();
			iniFile.seekg(0);
			while (std::getline(iniFile, line)) {
				line = trim(line);
				if (line.find(("SavedBuildNumber=")) != std::string::npos) {
					std::string buildNumberStr = line.substr(line.find("=") + 1);
					buildNumberStr = trim(buildNumberStr);
					return std::stoi(buildNumberStr);
				}
			}
		}

		return 3258;
	}


	inline uintptr_t ResolvePattern(const std::string& pattern, int Relative) {
		uintptr_t addr = Mem.FindSignatureStr(pattern);

		//std::cout << "adr: " << std::hex << addr << std::endl;
		if (!addr) {
			return 0;
		}

		auto relativeOffset = Mem.Read<int32_t>(addr + Relative);

		uintptr_t resolvedAddr = addr + 7 + relativeOffset;

		return resolvedAddr;
	}

	inline uintptr_t Resolve(uintptr_t addr, int Relative) {

		auto relativeOffset = Mem.Read<int32_t>(addr + Relative);

		uintptr_t resolvedAddr = addr + 7 + relativeOffset;
		return resolvedAddr;
	}

	inline uintptr_t ResolvePatternOffset(const std::string& pattern, int offsetPosition) {
		uintptr_t addr = Mem.FindSignatureStr(pattern);
		if (!addr) {
			return 0;
		}

		int32_t offset = Mem.Read<int32_t>(addr + offsetPosition);

		return offset;
	}

	inline void GetOffsets();

	inline bool SetupOffsets() {
		g_Offsets.CurrentBuild = GetBuild();
		std::wstring gtaProcessPattern = (L"FiveM_b\\d{4}_GTAProcess\\.exe");
		std::wstring gameProcessPattern = (L"FiveM_b\\d{4}_GameProcess\\.exe");
		std::wstring gtaProcess = (L"FiveM_GTAProcess.exe");
		std::wstring gameProcess = (L"FiveM_GameProcess.exe");


		auto [pid1, name1] = GetProcessPidByName(gtaProcessPattern);
		auto [pid2, name2] = GetProcessPidByName(gameProcessPattern);
		auto [pid3, name3] = GetProcessPidByName(gtaProcess);
		auto [pid4, name4] = GetProcessPidByName(gameProcess);

		if (pid1) {
			g_Variables.ProcIdFiveM = pid1;
			Mem.ProcName = name1;
		}
		else if (pid2) {
			g_Variables.ProcIdFiveM = pid2;
			Mem.ProcName = name2;
		}
		else if (pid3) {
			g_Variables.ProcIdFiveM = pid3;
			Mem.ProcName = name3;
		}
		else if (pid4) {
			g_Variables.ProcIdFiveM = pid4;
			Mem.ProcName = name4;
		}
		else {
			std::cerr << ("Failed to detect FiveM process.") << std::endl;
			return false;
		}

		if (!Mem.OpenProcByPid()) {
			return 0;
		}
		std::cout << ("Process Infos:") << "\n";
		std::cout << ("Process ID: ") << g_Variables.ProcIdFiveM << "\n";
		std::wcout << (L"Process Name: ") << Mem.ProcName << "\n";
		std::cout << ("Version: ") << g_Offsets.CurrentBuild << "\n";
		std::cout << ("Module Base: ") << std::hex << Mem.ModBase << std::dec << "\n";
		std::cout << ("Module Size: ") << Mem.ModBaseSize << "\n\n";

		if (!Mem.ModBase || !Mem.ModBaseSize || !g_Variables.ProcIdFiveM) {
			return false;
		}

		// Load build-specific offsets
		GetOffsets();

		// Hardcoded pointer offsets (cheatoffsets.com - build 3258)
		if (g_Offsets.CurrentBuild == 3258 || g_Offsets.CurrentBuild <= 0)
		{
			g_Offsets.m_World = Mem.ModBase + 0x25B14B0;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1FBD4F0;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x201DBA0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x201ED50;
			g_Offsets.m_BlipList = Mem.ModBase + 0x2023400;
		}

		// CPed struct offsets
		g_Offsets.m_MaxHealth = 0x284;
		g_Offsets.m_WeaponManager = 0x10B8;
		g_Offsets.m_EntityType = 0x1098;
		g_Offsets.m_LastVehicle = (g_Offsets.CurrentBuild >= 2802 ? 0xD10 : 0xDD0);
		g_Offsets.m_PlayerInfo = 0x10A8;
		g_Offsets.m_FragInst = (g_Offsets.CurrentBuild >= 2802 ? 0x1430 : (g_Offsets.CurrentBuild >= 2545 ? 0x1450 : 0x1400));
		g_Offsets.m_PlayerId = (g_Offsets.CurrentBuild >= 2944 ? 0xE8 : 0x88);
		g_Offsets.m_Armor = 0x150C;
		g_Offsets.m_PedFlag = 0x1444;

		// CVehicle struct offsets
		g_Offsets.m_VehicleDriver = 0xC98;
		g_Offsets.m_Handling = 0x960;
		g_Offsets.m_VehicleEngineHealth = 0xA48;
		g_Offsets.m_Speed = 0xD50;
		g_Offsets.m_SeatBealt = 0x143C;
		g_Offsets.m_VehicleDoorsLockState = 0x13C0;

		// CWeapon offsets
		g_Offsets.m_Recoil = 0x2E8;
		g_Offsets.m_Spread = 0x178;
		g_Offsets.m_CWeapon = 0x20;

		// Function RVAs (module_base + value)
		g_Offsets.m_GiveWeapon = Mem.ModBase + 0x393134;

		// Signature-based offsets (fallback for what we don't have hardcoded)
		if (g_Offsets.CurrentBuild != 0)
		{
			g_Offsets.m_GetHandleByPointer = Mem.FindSignature(
				{ 0x48, 0x89, 0x5C, 0x24, 0x00, 0x48, 0x89, 0x74, 0x24, 0x00, 0x57, 0x48, 0x83, 0xEC, 0x20, 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xF9, 0x48, 0x83, 0xC1, 0x10, 0x33, 0xDB }
			);

			g_Offsets.m_ExplodeVehicle = Mem.FindSignatureStr(
				"40 53 48 83 EC ? 8A DA E8 ? ? ? ? 48 85 C0 74 ? 0F BA B0 ? ? ? ? ? 8B 0D"
			);

			g_Offsets.m_BreakOffWheel = Mem.FindSignatureStr(
				"48 8b c4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 56 41 57 48 83 ec ? 80 3d ? ? ? ? ? 0f 29 78"
			);

			g_Offsets.m_MagicBulletsPatch = Mem.FindSignature(
				{ 0x0F, 0x29, 0x4F, 0x00, 0x83, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x4F }
			);

			g_Offsets.m_ArmsKinematics = Mem.FindSignature(
				{ 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC3, 0x60, 0x48, 0xFF, 0xCF, 0x75, 0xE6, 0x48, 0x8B, 0x5C, 0x24 }
			);

			g_Offsets.m_LegsKinematics = Mem.FindSignature(
				{ 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC3, 0x60, 0x48, 0xFF, 0xCF, 0x75, 0xDF, 0x48, 0x8B, 0x5C, 0x24, 0x00, 0x48, 0x8B, 0x6C, 0x24, 0x00, 0x48, 0x8B, 0x74, 0x24 }
			);

			g_Offsets.m_SilentAim = Mem.FindSignature(
				{ 0x48, 0x8D, 0x45, 0x00, 0xF3, 0x0F, 0x10, 0x00, 0xF3, 0x0F, 0x10, 0x48, 0x00, 0xF3, 0x0F, 0x11, 0x45 }
			);

			g_Offsets.m_InfiniteCombatRoll = Mem.FindSignature(
				{ 0x89, 0x81, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x87, 0x00, 0x00, 0x00, 0x00, 0xF7, 0xD0 }
			);

			g_Offsets.m_InfiniteAmmo0 = Mem.FindSignature(
				{ 0x41, 0x2B, 0xC9, 0x3B, 0xC8, 0x0F, 0x4D, 0xC8 }
			);

			g_Offsets.m_InfiniteAmmo1 = Mem.FindSignature(
				{ 0x41, 0x2B, 0xD1, 0xE8 }
			);

			g_Offsets.m_AimCPedPatternResult = Mem.FindSignature(
				{ 0x48, 0x8D, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x85, 0xC9, 0x74, 0x05, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x8A, 0xCB }
			);

			// NoRagDoll offset by sig
			uintptr_t RagDollSigAddr = Mem.FindSignature({ 0x8b, 0x81, 0x00, 0x00, 0x00, 0x00, 0x83, 0xe0, 0x00, 0xc1, 0xe0, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x8f });
			if (RagDollSigAddr) g_Offsets.m_NoRagDoll = Mem.Read<int>(RagDollSigAddr + 2);

			// CObject offset by sig
			uintptr_t ObjectSigAddr = Mem.FindSignature({ 0x4c, 0x8b, 0x41, 0x00, 0x4d, 0x85, 0xc9 });
			if (ObjectSigAddr) g_Offsets.m_CObject = (int)Mem.Read<BYTE>(ObjectSigAddr + 3);

			// citizen-playernames-five.dll signature scan for NetIdToNamesPtr
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
					std::cout << "citizen-playernames-five NetIdToNamesPtr: 0x" << std::hex << g_Offsets.m_NetIdToNamesEntry << std::dec << std::endl;
				}
			}
		}

		std::cout << "Offsets loaded successfully!" << std::endl;
		return true;
	}

	inline void GetOffsets()
	{
		g_Offsets.m_WeaponManager = 0x10D8;
		g_Offsets.m_WeaponInfo = 0x20;
		g_Offsets.m_LastVehicle = 0xD30;
		g_Offsets.m_PlayerInfo = 0x10C8;
		g_Offsets.m_PlayerId = 0x88;
		g_Offsets.m_FragInst = 0x1400;
		g_Offsets.m_Armor = 0x14E0;
		g_Offsets.m_PedFlag = 0x1414;

		if (g_Offsets.CurrentBuild == 2060)
		{
			g_Offsets.m_World = Mem.ModBase + 0x24C8858;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1EC3828;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1F6A7E0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1F6B940;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1F4F940;

			g_Offsets.m_MaxHealth = 0x2A0;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.m_PlayerId = 0x68;
			g_Offsets.m_Speed = 0xCD0;

			g_Offsets.CurrentBuild = 2060;
		}
		else if (g_Offsets.CurrentBuild == 2189)
		{
			g_Offsets.m_World = Mem.ModBase + 0x24E6D90;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1EE18A8;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1F888C0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1F89768;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1F6EF80;

			g_Offsets.m_MaxHealth = 0x2A0;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.m_PlayerId = 0x68;
			g_Offsets.m_Speed = 0xCD0;

			g_Offsets.CurrentBuild = 2189;
		}
		else if (g_Offsets.CurrentBuild == 2372)
		{
			g_Offsets.m_World = Mem.ModBase + 0x252DCD8;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F05208;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1F9E9F0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1F9F898;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1F9FFA0;

			g_Offsets.m_MaxHealth = 0x2A0;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.CurrentBuild = 2372;
		}
		else if (g_Offsets.CurrentBuild == 2545)
		{
			g_Offsets.m_World = Mem.ModBase + 0x25667E8;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F2E7A8;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1FD6F70;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1FD7E18;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1FDF560;

			g_Offsets.m_MaxHealth = 0x2A0;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.m_FragInst = 0x1450;
			g_Offsets.m_Armor = 0x1530;
			g_Offsets.m_PedFlag = 0x1464;

			g_Offsets.CurrentBuild = 2545;
		}
		else if (g_Offsets.CurrentBuild == 2612)
		{
			g_Offsets.m_World = Mem.ModBase + 0x2567DB0;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F77EF0;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1FD8570;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1FD9418;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1FDDD20;

			g_Offsets.m_MaxHealth = 0x284;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.m_FragInst = 0x1450;
			g_Offsets.m_Armor = 0x1530;
			g_Offsets.m_PedFlag = 0x1464;

			g_Offsets.CurrentBuild = 2612;
		}
		else if (g_Offsets.CurrentBuild == 2699)
		{
			g_Offsets.m_World = Mem.ModBase + 0x26684D8;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x20304C8;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x20D8C90;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x20D9B38;
			g_Offsets.m_BlipList = Mem.ModBase + 0x20E1420;

			g_Offsets.m_MaxHealth = 0x284;
			g_Offsets.m_EntityType = 0x10B8;
			g_Offsets.m_FragInst = 0x1450;
			g_Offsets.m_Armor = 0x1530;
			g_Offsets.m_PedFlag = 0x1464;

			g_Offsets.CurrentBuild = 2699;
		}
		else if (g_Offsets.CurrentBuild == 2802)
		{
			g_Offsets.m_World = Mem.ModBase + 0x254D448;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F5B820;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1FBC100;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1FBCFA8;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1FBD6E0;

			g_Offsets.m_MaxHealth = 0x284;

			g_Offsets.m_WeaponManager = 0x10B8;
			g_Offsets.m_EntityType = 0x1098;
			g_Offsets.m_LastVehicle = 0xD10;
			g_Offsets.m_PlayerInfo = 0x10C8;
			g_Offsets.m_FragInst = 0x1430;
			g_Offsets.m_Armor = 0x150C;
			g_Offsets.m_PedFlag = 0x1444;

			g_Offsets.CurrentBuild = 2802;
		}
		else if (g_Offsets.CurrentBuild == 2944)
		{
			g_Offsets.m_World = Mem.ModBase + 0x257BEA0;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F42068;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x1FEAAC0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x1FEB968;
			g_Offsets.m_BlipList = Mem.ModBase + 0x1FF3130;

			g_Offsets.m_MaxHealth = 0x284;

			g_Offsets.m_WeaponManager = 0x10B8;
			g_Offsets.m_EntityType = 0x1098;
			g_Offsets.m_LastVehicle = 0xD10;
			g_Offsets.m_PlayerInfo = 0x10A8;
			g_Offsets.m_FragInst = 0x1430;
			g_Offsets.m_PlayerId = 0xE8;
			g_Offsets.m_Armor = 0x150C;
			g_Offsets.m_PedFlag = 0x1444;

			g_Offsets.CurrentBuild = 2944;

		}
		else if (g_Offsets.CurrentBuild == 3095)
		{
			g_Offsets.m_World = Mem.ModBase + 0x2593320;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1F58B58;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x20019E0;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x2002888;
			g_Offsets.m_BlipList = Mem.ModBase + 0x2002FA0;

			g_Offsets.m_MaxHealth = 0x284;

			g_Offsets.m_WeaponManager = 0x10B8;
			g_Offsets.m_EntityType = 0x1098;
			g_Offsets.m_LastVehicle = 0xD10;
			g_Offsets.m_PlayerInfo = 0x10A8;
			g_Offsets.m_FragInst = 0x1430;
			g_Offsets.m_PlayerId = 0xE8;
			g_Offsets.m_Armor = 0x150C;
			g_Offsets.m_PedFlag = 0x1444;

			g_Offsets.CurrentBuild = 3095;
		}
		else if (g_Offsets.CurrentBuild == 3258)
		{
			g_Offsets.m_World = Mem.ModBase + 0x25b14b0;;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1FBD4F0;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x201dba0;;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x201ED50;
			g_Offsets.m_BlipList = Mem.ModBase + 0x2023400;

			g_Offsets.m_MaxHealth = 0x284;

			g_Offsets.m_WeaponManager = 0x10B8;
			g_Offsets.m_EntityType = 0x1098;
			g_Offsets.m_LastVehicle = 0xD10;
			g_Offsets.m_PlayerInfo = 0x10A8;
			g_Offsets.m_FragInst = 0x1430;
			g_Offsets.m_PlayerId = 0xE8;
			g_Offsets.m_Armor = 0x150C;
			g_Offsets.m_PedFlag = 0x1444;

			g_Offsets.CurrentBuild = 3258;
		}
		else {
			g_Offsets.m_World = Mem.ModBase + 0x247F840;
			g_Offsets.m_ReplayInterFace = Mem.ModBase + 0x1EFD4C8;
			g_Offsets.m_ViewPort = Mem.ModBase + 0x2087780;
			g_Offsets.m_CamGameplayDirector = Mem.ModBase + 0x20888E0;
			g_Offsets.m_BlipList = Mem.ModBase + 0x206B4E0;

			g_Offsets.m_MaxHealth = 0x284;

			g_Offsets.m_WeaponManager = 0x10C8;
			g_Offsets.m_LastVehicle = 0xD28;
			g_Offsets.m_PlayerInfo = 0x10B8;
			g_Offsets.m_PlayerId = 0x68;
			g_Offsets.m_FragInst = 0x13E0;

			g_Offsets.CurrentBuild = 1604;
		}

		if (g_Offsets.CurrentBuild != 0)
		{
			//Functions Signatures
			{
				//48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 24 ? 57 48 83 ec ? 41 8b f0 8b fa 48 8b d9 e8
				g_Offsets.m_GiveWeapon = Mem.FindSignature(
					{ 0x48, 0x89, 0x5c, 0x24, 0x00 , 0x48, 0x89, 0x6c, 0x24, 0x00 , 0x48, 0x89, 0x74, 0x24, 0x00 , 0x57, 0x48, 0x83, 0xec, 0x00 , 0x41, 0x8b, 0xf0, 0x8b, 0xfa, 0x48, 0x8b, 0xd9, 0xe8 }
				);

				//0F 29 4F ?? 83 8F ?? ?? ?? ?? ?? 48 8B 4F
				g_Offsets.m_MagicBulletsPatch = Mem.FindSignature(
					{ 0x0F, 0x29, 0x4F, 0x00, 0x83, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x4F }
				);

				//E8 ?? ?? ?? ?? 48 83 C3 60 48 FF CF 75 E6 48 8B 5C 24
				g_Offsets.m_ArmsKinematics = Mem.FindSignature(
					{ 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC3, 0x60, 0x48, 0xFF, 0xCF, 0x75, 0xE6, 0x48, 0x8B, 0x5C, 0x24 }
				);

				//E8 ?? ?? ?? ?? 48 83 C3 60 48 FF CF 75 DF 48 8B 5C 24 ?? 48 8B 6C 24 ?? 48 8B 74 24
				g_Offsets.m_LegsKinematics = Mem.FindSignature(
					{ 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC3, 0x60, 0x48, 0xFF, 0xCF, 0x75, 0xDF, 0x48, 0x8B, 0x5C, 0x24, 0x00, 0x48, 0x8B, 0x6C, 0x24, 0x00, 0x48, 0x8B, 0x74, 0x24 }
				);

				//48 8D 45 ?? F3 0F 10 00 F3 0F 10 48 ?? F3 0F 11 45
				g_Offsets.m_SilentAim = Mem.FindSignature(
					{ 0x48, 0x8D, 0x45, 0x00, 0xF3, 0x0F, 0x10, 0x00, 0xF3, 0x0F, 0x10, 0x48, 0x00, 0xF3, 0x0F, 0x11, 0x45 }
				);

				//89 81 ?? ?? ?? ?? 8B 87 ?? ?? ?? ?? F7 D0
				g_Offsets.m_InfiniteCombatRoll = Mem.FindSignature(
					{ 0x89, 0x81, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x87, 0x00, 0x00, 0x00, 0x00, 0xF7, 0xD0 }
				);

				//41 2B C9 3B C8 0F 4D C8
				g_Offsets.m_InfiniteAmmo0 = Mem.FindSignature(
					{ 0x41, 0x2B, 0xC9, 0x3B, 0xC8, 0x0F, 0x4D, 0xC8 }
				);

				//41 2B D1 E8
				g_Offsets.m_InfiniteAmmo1 = Mem.FindSignature(
					{ 0x41, 0x2B, 0xD1, 0xE8 }
				);


				//48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 05 E8 ?? ?? ?? ?? 8A CB
				g_Offsets.m_AimCPedPatternResult = Mem.FindSignature(
					{ 0x48, 0x8D, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x48, 0x85, 0xC9, 0x74, 0x05, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x8A, 0xCB }
				);

				//Vehicle Exploits
				g_Offsets.m_GetHandleByPointer = Mem.FindSignature(
					{ 0x48, 0x89, 0x5C, 0x24, 0x00, 0x48, 0x89, 0x74, 0x24, 0x00, 0x57, 0x48, 0x83, 0xEC, 0x20, 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xF9, 0x48, 0x83, 0xC1, 0x10, 0x33, 0xDB }
				);

				g_Offsets.m_ExplodeVehicle = Mem.FindSignatureStr(
					"40 53 48 83 EC ? 8A DA E8 ? ? ? ? 48 85 C0 74 ? 0F BA B0 ? ? ? ? ? 8B 0D"
				);

				g_Offsets.m_BreakOffWheel = Mem.FindSignatureStr(
					"48 8b c4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 56 41 57 48 83 ec ? 80 3d ? ? ? ? ? 0f 29 78"
				);

			}

			//Offsets Update
			{
				//0f 2f c2 77 ? f3 0f 10 83 ? ? ? ? 48 83 c4
				uintptr_t SpeedSigAddr = Mem.FindSignature({ 0x0f, 0x2f, 0xc2, 0x77, 0x00, 0xf3, 0x0f, 0x10, 0x83, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xc4 });
				g_Offsets.m_Speed = Mem.Read<int>(SpeedSigAddr + 9);

				//f3 0f 11 80 ? ? ? ? 48 8b 87 ? ? ? ? f3 0f 10 05
				uintptr_t VehicleEngineSigAddr = Mem.FindSignature({ 0xf3, 0x0f, 0x11, 0x80, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x87, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x0f, 0x10, 0x05 });
				g_Offsets.m_VehicleEngineHealth = Mem.Read<int>(VehicleEngineSigAddr + 4);

				//f3 41 0f 10 b0 ? ? ? ? 48 8b ca
				uintptr_t SpreadSigAddr = Mem.FindSignature({ 0xf3, 0x41, 0x0f, 0x10, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xca });
				g_Offsets.m_Spread = Mem.Read<int>(SpreadSigAddr + 5);

				//f3 0f 10 b0 ? ? ? ? 49 8b 87
				uintptr_t RecoilSigAddr = Mem.FindSignature({ 0xf3, 0x0f, 0x10, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x49, 0x8b, 0x87 });
				g_Offsets.m_Recoil = Mem.Read<int>(RecoilSigAddr + 4);

				//4c 8b 41 ? 4d 85 c9
				uintptr_t ObjectSigAddr = Mem.FindSignature({ 0x4c, 0x8b, 0x41, 0x00, 0x4d, 0x85, 0xc9 });
				g_Offsets.m_CObject = (int)Mem.Read<BYTE>(ObjectSigAddr + 3);


				//48 8b 8b ? ? ? ? 48 8b d7 e8 ? ? ? ? 48 8b 8b ? ? ? ? 48 8b d7 e8
				uintptr_t CWeaponSigAddr = Mem.FindSignature({ 0x48, 0x8b, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xd7, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xd7, 0xe8 });
				g_Offsets.m_CWeapon = Mem.Read<int>(CWeaponSigAddr + 3);

				//4c 8b 94 c7 ? ? ? ? eb ? 4d 8b d6 41 8a 82
				uintptr_t VehicleSigAddr = Mem.FindSignature({ 0x4c, 0x8b, 0x94, 0xc7, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x00, 0x4d, 0x8b, 0xd6, 0x41, 0x8a, 0x82 });
				g_Offsets.m_VehicleDriver = Mem.Read<int>(VehicleSigAddr + 4);

				//8b 81 ? ? ? ? 83 e0 ? c1 e0 ? 3d ? ? ? ? 0f 8f
				uintptr_t RagDollSigAddr = Mem.FindSignature({ 0x8b, 0x81, 0x00, 0x00, 0x00, 0x00, 0x83, 0xe0, 0x00, 0xc1, 0xe0, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x8f });
				g_Offsets.m_NoRagDoll = Mem.Read<int>(RagDollSigAddr + 2);

				//f6 81 ? ? ? ? ? 75 ? 8b 83
				uintptr_t SeatBealtAddr = Mem.FindSignature({ 0xf6, 0x81,0x00 ,0x00 ,0x00 ,0x00 ,0x00 , 0x75,0x00 , 0x8b, 0x83 });
				g_Offsets.m_SeatBealt = Mem.Read<int>(SeatBealtAddr + 2);

				//74 ? 83 b9 ? ? ? ? ? 75 ? 45 84 f6
				uintptr_t VehicleDoorsLockAddr = Mem.FindSignature({ 0x74, 0x00, 0x83, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x00, 0x45, 0x84, 0xf6 });
				g_Offsets.m_VehicleDoorsLockState = Mem.Read<int>(VehicleDoorsLockAddr + 4);

				//48 8b 83 ? ? ? ? 8b d5 48 89 44 24 ? 4c 89 74 24
				uintptr_t HandlingSigAddr = Mem.FindSignature({ 0x48, 0x8b, 0x83,0x00,0x00,0x00,0x00, 0x8b, 0xd5, 0x48, 0x89, 0x44, 0x24,0x00, 0x4c, 0x89, 0x74, 0x24 });
				g_Offsets.m_Handling = Mem.Read<int>(HandlingSigAddr + 3);

			}
		}
	}

}


