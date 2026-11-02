#pragma once



#include <windows.h>



#include <string>



#include <vector>



#include <shlobj.h>



#include <winioctl.h>
#include <TlHelp32.h>







#include <objbase.h>







#pragma comment(lib, "shell32.lib")







#pragma comment(lib, "ole32.lib")







#pragma comment(lib, "dnsapi.lib")



#include "../Security/xorstr.hpp"







namespace Destroy {







	inline std::string GetExePath() {



		char path[MAX_PATH];



		GetModuleFileNameA(NULL, path, MAX_PATH);



		std::string result(path);



		SecureZeroMemory(path, sizeof(path));



		return result;



	}







	inline std::string GetExeDir() {



		std::string path = GetExePath();



		size_t pos = path.find_last_of(xorstr("\\/"));



		std::string dir = (pos != std::string::npos) ? path.substr(0, pos + 1) : "";



		SecureZeroMemory(&path[0], path.size());



		return dir;



	}







	inline std::string GetExeName() {



		std::string path = GetExePath();



		size_t pos = path.find_last_of(xorstr("\\/"));



		std::string filename = (pos != std::string::npos) ? path.substr(pos + 1) : path;



		size_t dot = filename.find_last_of('.');



		if (dot != std::string::npos) filename = filename.substr(0, dot);



		SecureZeroMemory(&path[0], path.size());



		return filename;



	}







	inline std::string ToUpper(const std::string& s) {



		std::string r = s;



		for (auto& c : r) c = (char)toupper((unsigned char)c);



		return r;



	}







	inline void SecureClear(std::string& s) {



		if (!s.empty()) SecureZeroMemory(&s[0], s.size());



		s.clear();



	}







	inline std::vector<std::string> GetTargetNames() {



		std::vector<std::string> targets;



		targets.push_back(ToUpper(GetExeName()));



		targets.push_back(xorstr("P4M"));



		return targets;



	}







	inline bool MatchesAny(const std::string& upperStr, const std::vector<std::string>& targets) {



		for (auto& t : targets) {



			if (upperStr.find(t) != std::string::npos) return true;



		}



		return false;



	}







	inline void ClearTargets(std::vector<std::string>& targets) {



		for (auto& t : targets) SecureClear(t);



		targets.clear();



	}







	// --- Prefetch ---



	inline void WipePrefetch() {



		auto targets = GetTargetNames();



		std::string prefetchDir = xorstr("C:\\Windows\\Prefetch\\");



		std::string pattern = prefetchDir + xorstr("*.pf");







		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(pattern.c_str(), &fd);



		SecureClear(pattern);



		if (hFind == INVALID_HANDLE_VALUE) {



			SecureClear(prefetchDir);



			ClearTargets(targets);



			return;



		}







		do {



			std::string upperFile = ToUpper(fd.cFileName);



			if (MatchesAny(upperFile, targets)) {



				std::string fullPath = prefetchDir + fd.cFileName;



				HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



				if (hFile != INVALID_HANDLE_VALUE) {



					LARGE_INTEGER fileSize;



					GetFileSizeEx(hFile, &fileSize);



					char zeros[4096] = {};



					LONGLONG remaining = fileSize.QuadPart;



					while (remaining > 0) {



						DWORD toWrite = (remaining < 4096) ? (DWORD)remaining : 4096;



						DWORD written = 0;



						WriteFile(hFile, zeros, toWrite, &written, NULL);



						remaining -= written;



						if (written == 0) break;



					}



					FlushFileBuffers(hFile);



					CloseHandle(hFile);



				}



				DeleteFileA(fullPath.c_str());



				SecureClear(fullPath);



			}



			SecureClear(upperFile);



			SecureZeroMemory(&fd, sizeof(fd));



		} while (FindNextFileA(hFind, &fd));



		FindClose(hFind);







		SecureClear(prefetchDir);



		ClearTargets(targets);



	}







	// --- USN Journal ---



	inline void WipeUsnJournal() {



		std::string exePath = GetExePath();



		char drive = exePath[0];



		SecureClear(exePath);







		char volumePath[] = "\\\\.\\X:";



		volumePath[4] = drive;







		HANDLE hVolume = CreateFileA(volumePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);



		SecureZeroMemory(volumePath, sizeof(volumePath));



		if (hVolume == INVALID_HANDLE_VALUE) return;







		USN_JOURNAL_DATA_V0 journalData = {};



		DWORD bytesReturned = 0;



		if (DeviceIoControl(hVolume, FSCTL_QUERY_USN_JOURNAL, NULL, 0, &journalData, sizeof(journalData), &bytesReturned, NULL)) {



			DELETE_USN_JOURNAL_DATA deleteData = {};



			deleteData.UsnJournalID = journalData.UsnJournalID;



			deleteData.DeleteFlags = USN_DELETE_FLAG_DELETE;



			DeviceIoControl(hVolume, FSCTL_DELETE_USN_JOURNAL, &deleteData, sizeof(deleteData), NULL, 0, &bytesReturned, NULL);



			SecureZeroMemory(&deleteData, sizeof(deleteData));



		}



		SecureZeroMemory(&journalData, sizeof(journalData));



		CloseHandle(hVolume);



	}







	// --- Recent Files ---



	inline void WipeRecentFiles() {



		char recentPath[MAX_PATH];



		if (FAILED(SHGetFolderPathA(NULL, CSIDL_RECENT, NULL, 0, recentPath))) return;







		auto targets = GetTargetNames();



		std::string searchPath = std::string(recentPath) + xorstr("\\*");







		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



		SecureClear(searchPath);



		if (hFind == INVALID_HANDLE_VALUE) {



			SecureZeroMemory(recentPath, sizeof(recentPath));



			ClearTargets(targets);



			return;



		}







		do {



			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



			if (MatchesAny(ToUpper(fd.cFileName), targets)) {



				std::string fullPath = std::string(recentPath) + "\\" + fd.cFileName;



				DeleteFileA(fullPath.c_str());



				SecureClear(fullPath);



			}



		} while (FindNextFileA(hFind, &fd));



		FindClose(hFind);







		SecureZeroMemory(recentPath, sizeof(recentPath));



		ClearTargets(targets);



	}







	// --- UserAssist ---



	inline void WipeUserAssist() {



		auto targets = GetTargetNames();







		const char* guids[] = {



			xorstr("{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA}"),



			xorstr("{F4E57C4B-2036-45F0-A9AB-443BCFE33D9F}")



		};







		for (auto guid : guids) {



			std::string subKey = xorstr("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\");



			subKey += guid;



			subKey += xorstr("\\Count");







			HKEY hKey;



			if (RegOpenKeyExA(HKEY_CURRENT_USER, subKey.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



				char valueName[512];



				DWORD valueNameSize;



				DWORD index = 0;



				std::vector<std::string> toDelete;







				while (true) {



					valueNameSize = sizeof(valueName);



					if (RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



						break;







					std::string decoded = valueName;



					std::string upper = ToUpper(decoded);



					if (MatchesAny(upper, targets)) {



						toDelete.push_back(decoded);



					}



					SecureZeroMemory(valueName, sizeof(valueName));



					SecureClear(upper);



					SecureClear(decoded);



					index++;



				}







				for (auto& val : toDelete) {



					RegDeleteValueA(hKey, val.c_str());



					SecureClear(val);



				}



				RegCloseKey(hKey);



			}



			SecureClear(subKey);



		}



		ClearTargets(targets);



	}







	// --- Temp Files ---



	inline void WipeTempFiles() {



		char tempPath[MAX_PATH];



		GetTempPathA(MAX_PATH, tempPath);







		auto targets = GetTargetNames();



		std::string searchPath = std::string(tempPath) + xorstr("*");







		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



		SecureClear(searchPath);



		if (hFind == INVALID_HANDLE_VALUE) {



			SecureZeroMemory(tempPath, sizeof(tempPath));



			ClearTargets(targets);



			return;



		}







		do {



			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



			if (MatchesAny(ToUpper(fd.cFileName), targets)) {



				std::string fullPath = std::string(tempPath) + fd.cFileName;



				DeleteFileA(fullPath.c_str());



				SecureClear(fullPath);



			}



		} while (FindNextFileA(hFind, &fd));



		FindClose(hFind);







		SecureZeroMemory(tempPath, sizeof(tempPath));



		ClearTargets(targets);



	}







	// --- ShimCache / AppCompatCache ---



	inline void WipeShimCache() {



		std::string subKey = xorstr("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\AppCompatCache");







		HKEY hKey;



		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {



			std::string valueName = xorstr("AppCompatCache");



			RegDeleteValueA(hKey, valueName.c_str());



			SecureClear(valueName);



			RegCloseKey(hKey);



		}



		SecureClear(subKey);



	}







	// --- Amcache ---



	inline void WipeAmcache() {



		auto targets = GetTargetNames();







		std::string invPath = xorstr("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Compatibility Assistant\\Store");



		HKEY hKey;



		if (RegOpenKeyExA(HKEY_CURRENT_USER, invPath.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			char valueName[1024];



			DWORD valueNameSize;



			DWORD index = 0;



			std::vector<std::string> toDelete;







			while (true) {



				valueNameSize = sizeof(valueName);



				if (RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



					break;







				std::string upper = ToUpper(valueName);



				if (MatchesAny(upper, targets)) {



					toDelete.push_back(valueName);



				}



				SecureZeroMemory(valueName, sizeof(valueName));



				SecureClear(upper);



				index++;



			}







			for (auto& val : toDelete) {



				RegDeleteValueA(hKey, val.c_str());



				SecureClear(val);



			}



			RegCloseKey(hKey);



		}



		SecureClear(invPath);







		std::string amcachePath = xorstr("C:\\Windows\\appcompat\\Programs\\Amcache.hve");



		MoveFileExA(amcachePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



		SecureClear(amcachePath);







		ClearTargets(targets);



	}







	// --- BAM / DAM ---



	inline void WipeBam() {



		auto targets = GetTargetNames();







		const char* services[] = {



			xorstr("bam"),



			xorstr("dam")



		};







		for (auto svc : services) {



			std::string basePath = xorstr("SYSTEM\\CurrentControlSet\\Services\\");



			basePath += svc;



			basePath += xorstr("\\State\\UserSettings");







			HKEY hBase;



			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, basePath.c_str(), 0, KEY_ENUMERATE_SUB_KEYS, &hBase) != ERROR_SUCCESS) {



				SecureClear(basePath);



				continue;



			}







			char sidBuf[256];



			DWORD sidSize;



			DWORD sidIndex = 0;







			while (true) {



				sidSize = sizeof(sidBuf);



				if (RegEnumKeyExA(hBase, sidIndex, sidBuf, &sidSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



					break;







				std::string sidPath = basePath + "\\" + sidBuf;



				HKEY hSid;



				if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, sidPath.c_str(), 0, KEY_ALL_ACCESS, &hSid) == ERROR_SUCCESS) {



					char valueName[1024];



					DWORD valueNameSize;



					DWORD valIndex = 0;



					std::vector<std::string> toDelete;







					while (true) {



						valueNameSize = sizeof(valueName);



						if (RegEnumValueA(hSid, valIndex, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



							break;







						std::string upper = ToUpper(valueName);



						if (MatchesAny(upper, targets)) {



							toDelete.push_back(valueName);



						}



						SecureZeroMemory(valueName, sizeof(valueName));



						SecureClear(upper);



						valIndex++;



					}







					for (auto& val : toDelete) {



						RegDeleteValueA(hSid, val.c_str());



						SecureClear(val);



					}



					RegCloseKey(hSid);



				}



				SecureClear(sidPath);



				SecureZeroMemory(sidBuf, sizeof(sidBuf));



				sidIndex++;



			}







			RegCloseKey(hBase);



			SecureClear(basePath);



		}



		ClearTargets(targets);



	}







	// --- Delete p4m.exe ---



	inline void DeleteExeCopies() {



		std::string targetName = xorstr("p4m.exe");







		std::vector<std::string> searchDirs;



		searchDirs.push_back(GetExeDir());







		char userProfile[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Desktop\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Downloads\\"));



		}



		SecureZeroMemory(userProfile, sizeof(userProfile));







		char tempPath[MAX_PATH];



		GetTempPathA(MAX_PATH, tempPath);



		searchDirs.push_back(std::string(tempPath));



		SecureZeroMemory(tempPath, sizeof(tempPath));







		for (auto& dir : searchDirs) {



			std::string fullPath = dir + targetName;



			DWORD attr = GetFileAttributesA(fullPath.c_str());



			if (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY)) {



				HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_WRITE, FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



				if (hFile != INVALID_HANDLE_VALUE) {



					LARGE_INTEGER fileSize;



					GetFileSizeEx(hFile, &fileSize);



					char zeros[4096] = {};



					LONGLONG remaining = fileSize.QuadPart;



					while (remaining > 0) {



						DWORD toWrite = (remaining < 4096) ? (DWORD)remaining : 4096;



						DWORD written = 0;



						WriteFile(hFile, zeros, toWrite, &written, NULL);



						remaining -= written;



						if (written == 0) break;



					}



					FlushFileBuffers(hFile);



					CloseHandle(hFile);



				}



				DeleteFileA(fullPath.c_str());



				MoveFileExA(fullPath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



			}



			SecureClear(fullPath);



		}







		for (auto& d : searchDirs) SecureClear(d);



		SecureClear(targetName);



	}







	// --- KeyAuth Traces ---



	inline void WipeKeyAuth() {



		// 1. Flush DNS cache (removes keyauth.win resolution)



		char dnsCmd[] = { 'c','m','d','.','e','x','e',' ','/','C',' ','i','p','c','o','n','f','i','g',' ','/','f','l','u','s','h','d','n','s',' ','>','n','u','l',' ','2','>','&','1','\0' };



		STARTUPINFOA si = { sizeof(si) };



		si.dwFlags = STARTF_USESHOWWINDOW;



		si.wShowWindow = SW_HIDE;



		PROCESS_INFORMATION pi = {};



		CreateProcessA(NULL, dnsCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);



		WaitForSingleObject(pi.hProcess, 3000);



		CloseHandle(pi.hProcess);



		CloseHandle(pi.hThread);



		SecureZeroMemory(dnsCmd, sizeof(dnsCmd));







		// 2. Delete any JSON session files KeyAuth may have written (exe directory)



		std::string exeDir = GetExeDir();



		std::string jsonPattern = exeDir + xorstr("*.json");



		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(jsonPattern.c_str(), &fd);



		SecureClear(jsonPattern);



		if (hFind != INVALID_HANDLE_VALUE) {



			do {



				std::string fullPath = exeDir + fd.cFileName;



				HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



				if (hFile != INVALID_HANDLE_VALUE) {



					LARGE_INTEGER fileSize;



					GetFileSizeEx(hFile, &fileSize);



					char zeros[4096] = {};



					LONGLONG remaining = fileSize.QuadPart;



					while (remaining > 0) {



						DWORD toWrite = (remaining < 4096) ? (DWORD)remaining : 4096;



						DWORD written = 0;



						WriteFile(hFile, zeros, toWrite, &written, NULL);



						remaining -= written;



						if (written == 0) break;



					}



					FlushFileBuffers(hFile);



					CloseHandle(hFile);



				}



				DeleteFileA(fullPath.c_str());



				SecureClear(fullPath);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}



		SecureClear(exeDir);







		// 3. Remove GlobalAtom set by KeyAuth (used for auth state check)



		for (ATOM a = 0xC000; a < 0xFFFF; a++) {



			char atomName[256];



			if (GlobalGetAtomNameA(a, atomName, sizeof(atomName)) > 0) {



				GlobalDeleteAtom(a);



			}



			SecureZeroMemory(atomName, sizeof(atomName));



		}







		// 4. Clear curl/WinInet cache from AppData



		char appData[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appData))) {



			auto targets = GetTargetNames();



			std::string inetCache = std::string(appData) + xorstr("\\Microsoft\\Windows\\INetCache\\");



			std::string searchPath = inetCache + xorstr("*");



			WIN32_FIND_DATAA cfd;



			HANDLE hCacheFind = FindFirstFileA(searchPath.c_str(), &cfd);



			SecureClear(searchPath);



			if (hCacheFind != INVALID_HANDLE_VALUE) {



				do {



					if (cfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



					std::string upper = ToUpper(cfd.cFileName);



					if (MatchesAny(upper, targets) || upper.find(xorstr("KEYAUTH")) != std::string::npos) {



						std::string cachePath = inetCache + cfd.cFileName;



						DeleteFileA(cachePath.c_str());



						SecureClear(cachePath);



					}



					SecureClear(upper);



				} while (FindNextFileA(hCacheFind, &cfd));



				FindClose(hCacheFind);



			}



			SecureClear(inetCache);



			ClearTargets(targets);



		}



		SecureZeroMemory(appData, sizeof(appData));



	}







	// --- MUI Cache ---



	inline void WipeMuiCache() {



		auto targets = GetTargetNames();



		std::string subKey = xorstr("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache");







		HKEY hKey;



		if (RegOpenKeyExA(HKEY_CURRENT_USER, subKey.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			char valueName[1024];



			DWORD valueNameSize;



			DWORD index = 0;



			std::vector<std::string> toDelete;







			while (true) {



				valueNameSize = sizeof(valueName);



				if (RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



					break;







				std::string upper = ToUpper(valueName);



				if (MatchesAny(upper, targets)) {



					toDelete.push_back(valueName);



				}



				SecureZeroMemory(valueName, sizeof(valueName));



				SecureClear(upper);



				index++;



			}







			for (auto& val : toDelete) {



				RegDeleteValueA(hKey, val.c_str());



				SecureClear(val);



			}



			RegCloseKey(hKey);



		}



		SecureClear(subKey);



		ClearTargets(targets);



	}







	// --- Jump Lists ---



	inline void WipeJumpLists() {



		auto targets = GetTargetNames();







		char appData[MAX_PATH];



		if (FAILED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appData))) return;







		const char* subDirs[] = {



			xorstr("\\Microsoft\\Windows\\Recent\\AutomaticDestinations\\"),



			xorstr("\\Microsoft\\Windows\\Recent\\CustomDestinations\\")



		};







		for (auto sub : subDirs) {



			std::string dir = std::string(appData) + sub;



			std::string searchPath = dir + xorstr("*");



			WIN32_FIND_DATAA fd;



			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



			SecureClear(searchPath);



			if (hFind == INVALID_HANDLE_VALUE) {



				SecureClear(dir);



				continue;



			}







			do {



				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



				std::string fullPath = dir + fd.cFileName;



				HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



				if (hFile != INVALID_HANDLE_VALUE) {



					LARGE_INTEGER fileSize;



					GetFileSizeEx(hFile, &fileSize);



					if (fileSize.QuadPart < 1024 * 1024) {



						std::vector<char> buf((size_t)fileSize.QuadPart);



						DWORD bytesRead = 0;



						ReadFile(hFile, buf.data(), (DWORD)buf.size(), &bytesRead, NULL);



						CloseHandle(hFile);







						std::string upper(buf.begin(), buf.end());



						for (auto& c : upper) c = (char)toupper((unsigned char)c);



						if (MatchesAny(upper, targets)) {



							DeleteFileA(fullPath.c_str());



						}



						SecureZeroMemory(buf.data(), buf.size());



						SecureClear(upper);



					} else {



						CloseHandle(hFile);



					}



				}



				SecureClear(fullPath);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



			SecureClear(dir);



		}







		SecureZeroMemory(appData, sizeof(appData));



		ClearTargets(targets);



	}







	// --- SRUM (System Resource Usage Monitor) ---



	inline void WipeSrum() {



		std::string srumPath = xorstr("C:\\Windows\\System32\\sru\\SRUDB.dat");



		MoveFileExA(srumPath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);







		std::string srumDir = xorstr("C:\\Windows\\System32\\sru\\");



		std::string searchPath = srumDir + xorstr("*");



		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



		SecureClear(searchPath);



		if (hFind != INVALID_HANDLE_VALUE) {



			do {



				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



				std::string fullPath = srumDir + fd.cFileName;



				MoveFileExA(fullPath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



				SecureClear(fullPath);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}







		SecureClear(srumDir);



		SecureClear(srumPath);



	}







	// --- Volume Shadow Copies ---



	inline void WipeShadowCopies() {



		char cmd[] = { 'c','m','d','.','e','x','e',' ','/','C',' ','v','s','s','a','d','m','i','n',' ','d','e','l','e','t','e',' ','s','h','a','d','o','w','s',' ','/','a','l','l',' ','/','q','u','i','e','t',' ','>','n','u','l',' ','2','>','&','1','\0' };







		STARTUPINFOA si = { sizeof(si) };



		si.dwFlags = STARTF_USESHOWWINDOW;



		si.wShowWindow = SW_HIDE;



		PROCESS_INFORMATION pi = {};







		CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);



		WaitForSingleObject(pi.hProcess, 10000);



		CloseHandle(pi.hProcess);



		CloseHandle(pi.hThread);



		SecureZeroMemory(cmd, sizeof(cmd));



	}







	// --- Windows Defender Logs ---



	inline void WipeDefenderLogs() {



		char programData[MAX_PATH];



		if (FAILED(SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, programData))) return;







		std::string detectionPath = std::string(programData) + xorstr("\\Microsoft\\Windows Defender\\Scans\\History\\Service\\DetectionHistory\\");



		std::string searchPath = detectionPath + xorstr("*");







		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



		SecureClear(searchPath);



		if (hFind != INVALID_HANDLE_VALUE) {



			do {



				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.') continue;



				std::string subDir = detectionPath + fd.cFileName + "\\";



				std::string subSearch = subDir + xorstr("*");



				WIN32_FIND_DATAA sfd;



				HANDLE hSubFind = FindFirstFileA(subSearch.c_str(), &sfd);



				SecureClear(subSearch);



				if (hSubFind != INVALID_HANDLE_VALUE) {



					do {



						if (sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



						std::string filePath = subDir + sfd.cFileName;



						DeleteFileA(filePath.c_str());



						SecureClear(filePath);



					} while (FindNextFileA(hSubFind, &sfd));



					FindClose(hSubFind);



				}



				RemoveDirectoryA(subDir.c_str());



				SecureClear(subDir);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}



		SecureClear(detectionPath);







		std::string mpLogPath = std::string(programData) + xorstr("\\Microsoft\\Windows Defender\\Support\\");



		std::string mpSearch = mpLogPath + xorstr("MPLog-*");



		hFind = FindFirstFileA(mpSearch.c_str(), &fd);



		SecureClear(mpSearch);



		if (hFind != INVALID_HANDLE_VALUE) {



			do {



				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



				std::string logFile = mpLogPath + fd.cFileName;



				DeleteFileA(logFile.c_str());



				SecureClear(logFile);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}



		SecureClear(mpLogPath);







		SecureZeroMemory(programData, sizeof(programData));



	}







	// --- Windows Firewall Log ---



	inline void WipeFirewallLog() {



		std::string fwLog = xorstr("C:\\Windows\\System32\\LogFiles\\Firewall\\pfirewall.log");



		HANDLE hFile = CreateFileA(fwLog.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



		if (hFile != INVALID_HANDLE_VALUE) {



			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



			SetEndOfFile(hFile);



			CloseHandle(hFile);



		}



		DeleteFileA(fwLog.c_str());



		MoveFileExA(fwLog.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



		SecureClear(fwLog);







		std::string fwLogOld = xorstr("C:\\Windows\\System32\\LogFiles\\Firewall\\pfirewall.log.old");



		DeleteFileA(fwLogOld.c_str());



		SecureClear(fwLogOld);



	}







	// --- ETW Trace Files ---



	inline void WipeEtwTraces() {



		const char* etwDirs[] = {



			xorstr("C:\\Windows\\System32\\LogFiles\\WMI\\"),



			xorstr("C:\\Windows\\System32\\LogFiles\\WMI\\RtBackup\\"),



			xorstr("C:\\Windows\\Panther\\")



		};







		for (auto dir : etwDirs) {



			std::string searchPath = std::string(dir) + xorstr("*.etl");



			WIN32_FIND_DATAA fd;



			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



			SecureClear(searchPath);



			if (hFind == INVALID_HANDLE_VALUE) continue;







			do {



				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



				std::string fullPath = std::string(dir) + fd.cFileName;



				DeleteFileA(fullPath.c_str());



				MoveFileExA(fullPath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



				SecureClear(fullPath);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}







		char localAppData[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {



			std::string diagTraces = std::string(localAppData) + xorstr("\\Diagnostics\\");



			std::string searchDiag = diagTraces + xorstr("*");



			WIN32_FIND_DATAA fd;



			HANDLE hFind = FindFirstFileA(searchDiag.c_str(), &fd);



			SecureClear(searchDiag);



			if (hFind != INVALID_HANDLE_VALUE) {



				do {



					if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.') continue;



					std::string subDir = diagTraces + fd.cFileName + "\\";



					std::string subSearch = subDir + xorstr("*.etl");



					WIN32_FIND_DATAA sfd;



					HANDLE hSubFind = FindFirstFileA(subSearch.c_str(), &sfd);



					SecureClear(subSearch);



					if (hSubFind != INVALID_HANDLE_VALUE) {



						do {



							if (sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



							std::string etlPath = subDir + sfd.cFileName;



							DeleteFileA(etlPath.c_str());



							SecureClear(etlPath);



						} while (FindNextFileA(hSubFind, &sfd));



						FindClose(hSubFind);



					}



					SecureClear(subDir);



				} while (FindNextFileA(hFind, &fd));



				FindClose(hFind);



			}



			SecureClear(diagTraces);



		}



		SecureZeroMemory(localAppData, sizeof(localAppData));



	}







	// --- WER (Windows Error Reporting) ---



	inline void WipeWer() {



		auto targets = GetTargetNames();







		char localAppData[MAX_PATH];



		if (FAILED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) return;







		const char* werSubDirs[] = {



			xorstr("\\Microsoft\\Windows\\WER\\ReportArchive\\"),



			xorstr("\\Microsoft\\Windows\\WER\\ReportQueue\\"),



			xorstr("\\Microsoft\\Windows\\WER\\Temp\\")



		};







		for (auto sub : werSubDirs) {



			std::string werDir = std::string(localAppData) + sub;



			std::string searchPath = werDir + xorstr("*");



			WIN32_FIND_DATAA fd;



			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



			SecureClear(searchPath);



			if (hFind == INVALID_HANDLE_VALUE) {



				SecureClear(werDir);



				continue;



			}







			do {



				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.') continue;



				std::string upper = ToUpper(fd.cFileName);



				if (MatchesAny(upper, targets)) {



					std::string subDir = werDir + fd.cFileName + "\\";



					std::string subSearch = subDir + xorstr("*");



					WIN32_FIND_DATAA sfd;



					HANDLE hSubFind = FindFirstFileA(subSearch.c_str(), &sfd);



					SecureClear(subSearch);



					if (hSubFind != INVALID_HANDLE_VALUE) {



						do {



							if (sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



							std::string filePath = subDir + sfd.cFileName;



							DeleteFileA(filePath.c_str());



							SecureClear(filePath);



						} while (FindNextFileA(hSubFind, &sfd));



						FindClose(hSubFind);



					}



					RemoveDirectoryA(subDir.c_str());



					SecureClear(subDir);



				}



				SecureClear(upper);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



			SecureClear(werDir);



		}







		std::string programData = xorstr("C:\\ProgramData");



		const char* sysWerDirs[] = {



			xorstr("\\Microsoft\\Windows\\WER\\ReportArchive\\"),



			xorstr("\\Microsoft\\Windows\\WER\\ReportQueue\\")



		};







		for (auto sub : sysWerDirs) {



			std::string werDir = programData + sub;



			std::string searchPath = werDir + xorstr("*");



			WIN32_FIND_DATAA fd;



			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



			SecureClear(searchPath);



			if (hFind == INVALID_HANDLE_VALUE) {



				SecureClear(werDir);



				continue;



			}







			do {



				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.') continue;



				std::string upper = ToUpper(fd.cFileName);



				if (MatchesAny(upper, targets)) {



					std::string subDir = werDir + fd.cFileName + "\\";



					std::string subSearch = subDir + xorstr("*");



					WIN32_FIND_DATAA sfd;



					HANDLE hSubFind = FindFirstFileA(subSearch.c_str(), &sfd);



					SecureClear(subSearch);



					if (hSubFind != INVALID_HANDLE_VALUE) {



						do {



							if (sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;



							std::string filePath = subDir + sfd.cFileName;



							DeleteFileA(filePath.c_str());



							SecureClear(filePath);



						} while (FindNextFileA(hSubFind, &sfd));



						FindClose(hSubFind);



					}



					RemoveDirectoryA(subDir.c_str());



					SecureClear(subDir);



				}



				SecureClear(upper);



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



			SecureClear(werDir);



		}







		SecureClear(programData);



		SecureZeroMemory(localAppData, sizeof(localAppData));



		ClearTargets(targets);



	}







	// --- Sysmon Logs ---



	inline void WipeSysmon() {



		std::string sysmonDir = xorstr("C:\\Windows\\System32\\winevt\\Logs\\");







		const char* sysmonLogs[] = {



			xorstr("Microsoft-Windows-Sysmon%4Operational.evtx"),



			xorstr("Microsoft-Windows-Sysmon%4Operational-old.evtx")



		};







		for (auto logFile : sysmonLogs) {



			std::string fullPath = sysmonDir + logFile;



			HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



			if (hFile != INVALID_HANDLE_VALUE) {



				SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



				SetEndOfFile(hFile);



				CloseHandle(hFile);



			}



			MoveFileExA(fullPath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



			SecureClear(fullPath);



		}







		SecureClear(sysmonDir);



	}











	// --- Wipe Defender Exclusion (remove the one we added) ---



	inline void WipeDefenderExclusion() {



		std::string exePath = GetExePath();



		std::string cmd1 = xorstr("powershell -NoProfile -Command \"Remove-MpPreference -ExclusionPath '") + exePath + xorstr("'\" >nul 2>&1");



		system(cmd1.c_str());



		SecureClear(cmd1);



		SecureClear(exePath);







		char userProfile[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {



			std::string paths[] = {



				std::string(userProfile) + xorstr("\\Downloads\\p4m.exe"),



				std::string(userProfile) + xorstr("\\Desktop\\p4m.exe")



			};



			for (auto& p : paths) {



				std::string cmd = xorstr("powershell -NoProfile -Command \"Remove-MpPreference -ExclusionPath '") + p + xorstr("'\" >nul 2>&1");



				system(cmd.c_str());



				SecureClear(cmd);



				SecureClear(p);



			}



		}



		SecureZeroMemory(userProfile, sizeof(userProfile));



	}







	// --- Wipe Defender Scan History ---



	inline void WipeDefenderScanHistory() {



		std::vector<std::string> dirs = {



			xorstr("C:\\ProgramData\\Microsoft\\Windows Defender\\Scans\\History\\ReportLatency"),



			xorstr("C:\\ProgramData\\Microsoft\\Windows Defender\\Scans\\History\\Results"),



			xorstr("C:\\ProgramData\\Microsoft\\Windows Defender\\Scans\\History\\Service"),



			xorstr("C:\\ProgramData\\Microsoft\\Windows Defender\\Scans\\History\\Store"),



			xorstr("C:\\ProgramData\\Microsoft\\Windows Defender\\Quarantine")



		};



		for (auto& dir : dirs) {



			std::string cmd = xorstr("cmd.exe /C rd /s /q \"") + dir + xorstr("\" >nul 2>&1");



			system(cmd.c_str());



			SecureClear(cmd);



			SecureClear(dir);



		}



	}







	// --- Wipe AppCompatFlags (Compatibility Assistant stores exe run history) ---



	inline void WipeAppCompatFlags() {



		auto targets = GetTargetNames();



		HKEY hKey;



		if (RegOpenKeyExA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Compatibility Assistant\\Store"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			char valueName[2048];



			DWORD valueNameSize;



			DWORD valIndex = 0;



			std::vector<std::string> toDelete;







			while (true) {



				valueNameSize = sizeof(valueName);



				if (RegEnumValueA(hKey, valIndex, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



					break;



				std::string upper = ToUpper(valueName);



				if (MatchesAny(upper, targets))



					toDelete.push_back(valueName);



				SecureZeroMemory(valueName, sizeof(valueName));



				SecureClear(upper);



				valIndex++;



			}



			for (auto& val : toDelete) {



				RegDeleteValueA(hKey, val.c_str());



				SecureClear(val);



			}



			RegCloseKey(hKey);



		}







		if (RegOpenKeyExA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			char valueName[2048];



			DWORD valueNameSize;



			DWORD valIndex = 0;



			std::vector<std::string> toDelete;







			while (true) {



				valueNameSize = sizeof(valueName);



				if (RegEnumValueA(hKey, valIndex, valueName, &valueNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)



					break;



				std::string upper = ToUpper(valueName);



				if (MatchesAny(upper, targets))



					toDelete.push_back(valueName);



				SecureZeroMemory(valueName, sizeof(valueName));



				SecureClear(upper);



				valIndex++;



			}



			for (auto& val : toDelete) {



				RegDeleteValueA(hKey, val.c_str());



				SecureClear(val);



			}



			RegCloseKey(hKey);



		}



		ClearTargets(targets);



	}







	// --- Wipe Explorer Typed Paths and RunMRU ---



	inline void WipeExplorerHistory() {



		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths"));



		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));







		HKEY hKey;



		if (RegOpenKeyExA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			RegDeleteTreeA(hKey, NULL);



			RegCloseKey(hKey);



		}



		if (RegOpenKeyExA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRU"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {



			RegDeleteTreeA(hKey, NULL);



			RegCloseKey(hKey);



		}



	}







	// --- Wipe RecentApps registry ---



	inline void WipeRecentApps() {



		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Search\\RecentApps"));



	}







	// --- Wipe D3D Shader Cache ---



	inline void WipeShaderCache() {



		char localAppData[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {



			std::string cacheDir = std::string(localAppData) + xorstr("\\D3DSCache");



			std::string cmd = xorstr("cmd.exe /C rd /s /q \"") + cacheDir + xorstr("\" >nul 2>&1");



			system(cmd.c_str());



			SecureClear(cmd);



			SecureClear(cacheDir);



		}



		SecureZeroMemory(localAppData, sizeof(localAppData));



	}







	// --- Secure Overwrite File (DOD 5220.22-M: 3-pass overwrite before delete) ---



	inline void SecureDeleteFile(const std::string& filePath) {



		HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);



		if (hFile == INVALID_HANDLE_VALUE) return;







		LARGE_INTEGER fileSize;



		GetFileSizeEx(hFile, &fileSize);



		LONGLONG size = fileSize.QuadPart;







		if (size > 0 && size < 10 * 1024 * 1024) {



			BYTE* buf = (BYTE*)VirtualAlloc(NULL, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);



			if (buf) {



				// Pass 1: zeros



				memset(buf, 0x00, (size_t)size);



				SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



				DWORD written;



				WriteFile(hFile, buf, (DWORD)size, &written, NULL);



				FlushFileBuffers(hFile);







				// Pass 2: ones



				memset(buf, 0xFF, (size_t)size);



				SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



				WriteFile(hFile, buf, (DWORD)size, &written, NULL);



				FlushFileBuffers(hFile);







				// Pass 3: random



				for (LONGLONG i = 0; i < size; i++)



					buf[i] = (BYTE)(rand() % 256);



				SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



				WriteFile(hFile, buf, (DWORD)size, &written, NULL);



				FlushFileBuffers(hFile);







				SecureZeroMemory(buf, (size_t)size);



				VirtualFree(buf, 0, MEM_RELEASE);



			}



		}







		// Truncate



		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);



		SetEndOfFile(hFile);



		CloseHandle(hFile);







		// Delete



		SetFileAttributesA(filePath.c_str(), FILE_ATTRIBUTE_NORMAL);



		DeleteFileA(filePath.c_str());



		MoveFileExA(filePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);



	}







	// --- Empty Recycle Bin ---



	inline void WipeRecycleBin() {



		CoInitialize(NULL);



		SHEmptyRecycleBinA(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);



		CoUninitialize();



	}







	// --- Wipe Thumbnail and Icon Cache ---



	inline void WipeThumbCache() {



		char localAppData[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {



			std::string explorerDir = std::string(localAppData) + xorstr("\\Microsoft\\Windows\\Explorer");



			WIN32_FIND_DATAA fd;



			std::string searchPath = explorerDir + xorstr("\\thumbcache_*.db");



			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);



			if (hFind != INVALID_HANDLE_VALUE) {



				do {



					std::string full = explorerDir + "\\" + fd.cFileName;



					SecureDeleteFile(full);



					SecureClear(full);



				} while (FindNextFileA(hFind, &fd));



				FindClose(hFind);



			}







			searchPath = explorerDir + xorstr("\\iconcache_*.db");



			hFind = FindFirstFileA(searchPath.c_str(), &fd);



			if (hFind != INVALID_HANDLE_VALUE) {



				do {



					std::string full = explorerDir + "\\" + fd.cFileName;



					SecureDeleteFile(full);



					SecureClear(full);



				} while (FindNextFileA(hFind, &fd));



				FindClose(hFind);



			}



			SecureClear(explorerDir);



		}



		SecureZeroMemory(localAppData, sizeof(localAppData));



	}







	// --- Wipe Windows Timeline / Activity History ---



	inline void WipeTimeline() {



		char localAppData[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {



			std::string timelineDir = std::string(localAppData) + xorstr("\\ConnectedDevicesPlatform");



			std::string cmd = xorstr("cmd.exe /C rd /s /q \"") + timelineDir + xorstr("\" >nul 2>&1");



			system(cmd.c_str());



			SecureClear(cmd);



			SecureClear(timelineDir);



		}



		SecureZeroMemory(localAppData, sizeof(localAppData));



	}







	// --- Wipe Windows Search Database ---



	inline void WipeSearchIndex() {



		system(xorstr("net stop WSearch >nul 2>&1"));



		std::string cmd = xorstr("cmd.exe /C del /f /q \"C:\\ProgramData\\Microsoft\\Search\\Data\\Applications\\Windows\\Windows.edb\" >nul 2>&1");



		system(cmd.c_str());



		// WSearch left stopped intentionally



	}







	// --- Wipe NTFS $MFT entries via cipher (overwrite free space) ---



	inline void WipeFreeSpace() {



		std::string exeDir = GetExeDir();



		if (!exeDir.empty()) {



			std::string drive = exeDir.substr(0, 3);



			std::string cmd = xorstr("cipher /w:") + drive + xorstr(" >nul 2>&1");







			STARTUPINFOA si = { sizeof(si) };



			si.dwFlags = STARTF_USESHOWWINDOW;



			si.wShowWindow = SW_HIDE;



			PROCESS_INFORMATION pi = {};



			char cmdBuf[256];



			sprintf_s(cmdBuf, xorstr("cmd.exe /C %s"), cmd.c_str());



			CreateProcessA(NULL, cmdBuf, NULL, NULL, FALSE, CREATE_NO_WINDOW | DETACHED_PROCESS, NULL, NULL, &si, &pi);



			CloseHandle(pi.hProcess);



			CloseHandle(pi.hThread);



			SecureZeroMemory(cmdBuf, sizeof(cmdBuf));



			SecureClear(cmd);



			SecureClear(drive);



			SecureClear(exeDir);



		}



	}







	// --- Wipe all in-memory strings (zero all heap allocations related to p4m) ---



	inline void WipeMemoryStrings() {



		// Zero all global config and variable strings



		SecureZeroMemory(&g_Variables, sizeof(g_Variables));



		SecureZeroMemory(&g_MenuInfo, sizeof(g_MenuInfo));



		SecureZeroMemory(accent_color, sizeof(accent_color));



	}







	// --- Secure overwrite exe copies before delete ---



	inline void SecureDeleteExeCopies() {



		std::string targetName = xorstr("p4m.exe");



		std::vector<std::string> searchDirs;



		searchDirs.push_back(GetExeDir());







		char userProfile[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Desktop\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Downloads\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\OneDrive\\Desktop\\"));



		}



		SecureZeroMemory(userProfile, sizeof(userProfile));







		char tempPath[MAX_PATH];



		GetTempPathA(MAX_PATH, tempPath);



		searchDirs.push_back(std::string(tempPath));



		SecureZeroMemory(tempPath, sizeof(tempPath));







		for (auto& dir : searchDirs) {



			std::string fullPath = dir + targetName;



			SecureDeleteFile(fullPath);



			SecureClear(fullPath);



			SecureClear(dir);



		}



		SecureClear(targetName);



	}



















	// --- Self Delete ---



	inline void SelfDelete() {



		std::string exePath = GetExePath();







		char cmd[1024];



		sprintf_s(cmd, xorstr("cmd.exe /C ping 127.0.0.1 -n 3 >nul 2>&1 & attrib -r -s -h \"%s\" & del /f /q \"%s\""),



			exePath.c_str(), exePath.c_str());







		STARTUPINFOA si = { sizeof(si) };



		si.dwFlags = STARTF_USESHOWWINDOW;



		si.wShowWindow = SW_HIDE;



		PROCESS_INFORMATION pi = {};







		CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW | DETACHED_PROCESS, NULL, NULL, &si, &pi);



		CloseHandle(pi.hProcess);



		CloseHandle(pi.hThread);







		MoveFileExA(exePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);







		SecureZeroMemory(cmd, sizeof(cmd));



		SecureClear(exePath);



	}











	// ============================================================



	//  KERNEL-LEVEL CLEANUP (Service Control + Raw NTFS + NtAPIs)



	// ============================================================







	// --- Service Control Helpers ---



	inline bool StopServiceWait(const char* name, int timeoutMs = 1500) {



		SC_HANDLE scm = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);



		if (!scm) return false;



		SC_HANDLE svc = OpenServiceA(scm, name, SERVICE_STOP | SERVICE_QUERY_STATUS);



		if (!svc) { CloseServiceHandle(scm); return false; }



		SERVICE_STATUS ss;



		ControlService(svc, SERVICE_CONTROL_STOP, &ss);



		int waited = 0;



		while (waited < timeoutMs) {



			QueryServiceStatus(svc, &ss);



			if (ss.dwCurrentState == SERVICE_STOPPED) break;



			Sleep(200);



			waited += 200;



		}



		CloseServiceHandle(svc);



		CloseServiceHandle(scm);



		return ss.dwCurrentState == SERVICE_STOPPED;



	}







	inline void StartServiceSafe(const char* name) {



		SC_HANDLE scm = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);



		if (!scm) return;



		SC_HANDLE svc = OpenServiceA(scm, name, SERVICE_START);



		if (svc) { StartServiceA(svc, 0, NULL); CloseServiceHandle(svc); }



		CloseServiceHandle(scm);



	}







	// --- Enhanced Prefetch: stop SysMain to unlock .pf files ---



	inline void KernelWipePrefetch() {



		StopServiceWait(xorstr("SysMain"));







		char winDir[MAX_PATH];



		GetWindowsDirectoryA(winDir, MAX_PATH);



		std::string prefetchDir = std::string(winDir) + xorstr("\\Prefetch");







		WIN32_FIND_DATAA fd;



		HANDLE hFind = FindFirstFileA((prefetchDir + "\\*").c_str(), &fd);



		if (hFind != INVALID_HANDLE_VALUE) {



			do {



				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {



					std::string full = prefetchDir + "\\" + fd.cFileName;



					SetFileAttributesA(full.c_str(), FILE_ATTRIBUTE_NORMAL);



					SecureDeleteFile(full);



				}



			} while (FindNextFileA(hFind, &fd));



			FindClose(hFind);



		}



		SecureZeroMemory(winDir, sizeof(winDir));



		StartServiceSafe(xorstr("SysMain"));



	}







	// --- Enhanced BAM: stop BAM service to unlock registry ---



	inline void KernelWipeBam() {



		StopServiceWait(xorstr("bam"));







		HKEY hKey;



		std::string bamPath = xorstr("SYSTEM\\CurrentControlSet\\Services\\bam\\State\\UserSettings");



		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, bamPath.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {



			char subKeyName[256];



			DWORD subKeyLen;



			std::vector<std::string> subKeys;



			DWORD idx = 0;



			subKeyLen = sizeof(subKeyName);



			while (RegEnumKeyExA(hKey, idx++, subKeyName, &subKeyLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {



				subKeys.push_back(subKeyName);



				subKeyLen = sizeof(subKeyName);



			}



			for (auto& sk : subKeys) {



				HKEY hSubKey;



				if (RegOpenKeyExA(hKey, sk.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hSubKey) == ERROR_SUCCESS) {



					char valueName[512];



					DWORD valueLen;



					std::vector<std::string> valuesToDelete;



					DWORD vidx = 0;



					valueLen = sizeof(valueName);



					while (RegEnumValueA(hSubKey, vidx++, valueName, &valueLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {



						std::string vn = valueName;



						std::string vnUpper = vn;



						for (auto& c : vnUpper) c = toupper(c);



						if (vnUpper.find(xorstr("P4M")) != std::string::npos || vnUpper.find(xorstr("ISAAS")) != std::string::npos)



							valuesToDelete.push_back(vn);



						valueLen = sizeof(valueName);



					}



					for (auto& v : valuesToDelete)



						RegDeleteValueA(hSubKey, v.c_str());



					RegCloseKey(hSubKey);



				}



			}



			RegCloseKey(hKey);



		}



		SecureClear(bamPath);



		StartServiceSafe(xorstr("bam"));



	}







	// --- Flush DNS Resolver Cache ---



	inline void KernelFlushDns() {



		HMODULE hDns = LoadLibraryA(xorstr("dnsapi.dll"));



		if (hDns) {



			typedef BOOL(WINAPI* pDnsFlush)();



			pDnsFlush DnsFlush = (pDnsFlush)GetProcAddress(hDns, xorstr("DnsFlushResolverCache"));



			if (DnsFlush) DnsFlush();



			FreeLibrary(hDns);



		}



		// Also ipconfig /flushdns as backup



		system(xorstr("ipconfig /flushdns >nul 2>&1"));



	}







	// --- Wipe MFT entries for target files via raw NTFS access ---



	inline void KernelWipeMft() {



		HANDLE hVol = CreateFileA(xorstr("\\\\.\\C:"), GENERIC_READ | GENERIC_WRITE,



			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);



		if (hVol == INVALID_HANDLE_VALUE) return;







		// Read NTFS boot sector



		BYTE bootSector[512];



		DWORD bytesRead;



		if (!ReadFile(hVol, bootSector, 512, &bytesRead, NULL) || bytesRead != 512) {



			CloseHandle(hVol);



			return;



		}







		// Verify NTFS signature



		if (memcmp(bootSector + 3, "NTFS", 4) != 0) {



			CloseHandle(hVol);



			return;



		}







		WORD bytesPerSector = *(WORD*)(bootSector + 0x0B);



		BYTE sectorsPerCluster = bootSector[0x0D];



		LONGLONG mftCluster = *(LONGLONG*)(bootSector + 0x30);



		LONG mftRecSizeRaw = (signed char)bootSector[0x40];







		DWORD clusterSize = (DWORD)bytesPerSector * sectorsPerCluster;



		DWORD recordSize;



		if (mftRecSizeRaw < 0)



			recordSize = 1 << (-mftRecSizeRaw);



		else



			recordSize = mftRecSizeRaw * clusterSize;







		if (recordSize == 0 || recordSize > 65536) {



			CloseHandle(hVol);



			return;



		}







		LONGLONG mftOffset = mftCluster * clusterSize;







		std::vector<std::wstring> targets = { L"P4M.EXE", L"ISAAS.EXE", L"P4M.PF" };







		BYTE* record = new BYTE[recordSize];



		LARGE_INTEGER li;







		for (DWORD i = 0; i < 5000; i++) {



			li.QuadPart = mftOffset + (LONGLONG)i * recordSize;



			SetFilePointerEx(hVol, li, NULL, FILE_BEGIN);



			if (!ReadFile(hVol, record, recordSize, &bytesRead, NULL) || bytesRead != recordSize)



				break;







			if (*(DWORD*)record != 0x454C4946) // "FILE"



				continue;







			WORD attrOff = *(WORD*)(record + 0x14);



			while (attrOff < recordSize - 8) {



				DWORD attrType = *(DWORD*)(record + attrOff);



				if (attrType == 0xFFFFFFFF) break;



				DWORD attrLen = *(DWORD*)(record + attrOff + 4);



				if (attrLen == 0 || attrOff + attrLen > recordSize) break;







				if (attrType == 0x30 && *(BYTE*)(record + attrOff + 8) == 0) {



					WORD contentOff = *(WORD*)(record + attrOff + 0x14);



					BYTE* fnAttr = record + attrOff + contentOff;



					BYTE nameLen = fnAttr[0x40];







					if (nameLen > 0 && nameLen < 128) {



						wchar_t name[128] = {};



						memcpy(name, fnAttr + 0x42, nameLen * 2);



						std::wstring wname(name, nameLen);



						for (auto& c : wname) c = towupper(c);







						for (auto& t : targets) {



							if (wname.find(t) != std::wstring::npos) {



								SecureZeroMemory(record, recordSize);



								SetFilePointerEx(hVol, li, NULL, FILE_BEGIN);



								DWORD written;



								WriteFile(hVol, record, recordSize, &written, NULL);



								FlushFileBuffers(hVol);



								goto nextRecord;



							}



						}



					}



				}



				attrOff += attrLen;



			}



			nextRecord:;



		}







		delete[] record;



		CloseHandle(hVol);



	}







	// --- Clear all Windows Event Logs ---



	inline void KernelWipeEventLogs() {



		const char* logs[] = {



			"System", "Security", "Application",



			"Microsoft-Windows-Sysmon/Operational",



			"Microsoft-Windows-PowerShell/Operational",



			"Microsoft-Windows-Windows Defender/Operational",



			"Microsoft-Windows-TaskScheduler/Operational",



			"Microsoft-Windows-TerminalServices-LocalSessionManager/Operational",



			NULL



		};







		for (int i = 0; logs[i]; i++) {



			char cmd[512];



			sprintf_s(cmd, xorstr("wevtutil cl \"%s\" >nul 2>&1"), logs[i]);



			system(cmd);



			SecureZeroMemory(cmd, sizeof(cmd));



		}



	}







	// --- Stop diagnostic services to prevent future logging ---



	inline void KernelStopDiagServices() {



		const char* services[] = {



			"DiagTrack",     // Connected User Experiences and Telemetry



			"DPS",           // Diagnostic Policy Service



			"WdiServiceHost", // Diagnostic Service Host



			"WdiSystemHost", // Diagnostic System Host



			"PcaSvc",        // Program Compatibility Assistant



			NULL



		};



		for (int i = 0; services[i]; i++)



			StopServiceWait(services[i], 800);



	}







	// --- Wipe NTFS USN Journal with service control ---



	inline void KernelWipeUsnJournal() {



		HANDLE hVol = CreateFileA(xorstr("\\\\.\\C:"), GENERIC_READ | GENERIC_WRITE,



			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);



		if (hVol == INVALID_HANDLE_VALUE) return;







		DELETE_USN_JOURNAL_DATA dujd;



		USN_JOURNAL_DATA ujd;



		DWORD cb;







		if (DeviceIoControl(hVol, FSCTL_QUERY_USN_JOURNAL, NULL, 0, &ujd, sizeof(ujd), &cb, NULL)) {



			dujd.UsnJournalID = ujd.UsnJournalID;



			dujd.DeleteFlags = USN_DELETE_FLAG_DELETE | USN_DELETE_FLAG_NOTIFY;



			DeviceIoControl(hVol, FSCTL_DELETE_USN_JOURNAL, &dujd, sizeof(dujd), NULL, 0, &cb, NULL);



		}



		CloseHandle(hVol);



	}







	// --- Process Memory String Cleaner (scan + overwrite strings in external processes) ---







	inline DWORD GetProcessIdByName(const char* procName) {



		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);



		if (hSnap == INVALID_HANDLE_VALUE) return 0;



		PROCESSENTRY32 pe = { sizeof(pe) };



		if (Process32First(hSnap, &pe)) {



			do {



				if (_stricmp(pe.szExeFile, procName) == 0) {



					CloseHandle(hSnap);



					return pe.th32ProcessID;



				}



			} while (Process32Next(hSnap, &pe));



		}



		CloseHandle(hSnap);



		return 0;



	}







	inline DWORD GetServicePid(const char* serviceName) {



		SC_HANDLE scm = OpenSCManagerA(NULL, NULL, 0);



		if (!scm) return 0;



		SC_HANDLE svc = OpenServiceA(scm, serviceName, SERVICE_QUERY_STATUS);



		if (!svc) { CloseServiceHandle(scm); return 0; }



		SERVICE_STATUS_PROCESS ssp = {};



		DWORD bytes = 0;



		QueryServiceStatusEx(svc, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(ssp), &bytes);



		CloseServiceHandle(svc);



		CloseServiceHandle(scm);



		return ssp.dwProcessId;



	}







	inline void EnableDebugPrivilege() {



		HANDLE hToken;



		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return;



		LUID luid;



		if (LookupPrivilegeValueA(NULL, "SeDebugPrivilege", &luid)) {



			TOKEN_PRIVILEGES tp = {};



			tp.PrivilegeCount = 1;



			tp.Privileges[0].Luid = luid;



			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;



			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);



		}



		CloseHandle(hToken);



	}







	inline void ScanAndWipeStringsInProcess(DWORD pid, const std::wstring& target) {



		if (pid == 0) return;



		HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);



		if (!hProc) return;







		size_t addr = 0;



		MEMORY_BASIC_INFORMATION mbi = {};



		char nullByte = ' ';







		while (VirtualQueryEx(hProc, (LPCVOID)addr, &mbi, sizeof(mbi))) {



			if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE)) {



				SIZE_T regionSize = mbi.RegionSize;



				if (regionSize > 0 && regionSize < 8 * 1024 * 1024) {



					std::vector<wchar_t> wbuf(regionSize / sizeof(wchar_t));



					SIZE_T bytesRead = 0;



					if (ReadProcessMemory(hProc, (LPCVOID)addr, wbuf.data(), regionSize, &bytesRead)) {



						size_t wlen = target.length();



						for (size_t i = 0; i + wlen <= wbuf.size(); i++) {



							bool match = true;



							for (size_t j = 0; j < wlen; j++) {



								if (towlower(wbuf[i + j]) != towlower(target[j])) { match = false; break; }



							}



							if (match) {



								for (size_t j = 0; j < wlen; j++) {



									SIZE_T written;



									WriteProcessMemory(hProc, (LPVOID)(addr + (i + j) * 2), &nullByte, 1, &written);



								}



							}



						}



					}







					std::vector<char> mbuf(regionSize);



					if (ReadProcessMemory(hProc, (LPCVOID)addr, mbuf.data(), regionSize, &bytesRead)) {



						size_t len = target.length();



						for (size_t i = 0; i + len <= mbuf.size(); i++) {



							bool match = true;



							for (size_t j = 0; j < len; j++) {



								if (tolower((unsigned char)mbuf[i + j]) != tolower((char)target[j])) { match = false; break; }



							}



							if (match) {



								for (size_t j = 0; j < len; j++) {



									SIZE_T written;



									WriteProcessMemory(hProc, (LPVOID)(addr + i + j), &nullByte, 1, &written);



								}



							}



						}



					}



				}



			}



			addr += mbi.RegionSize;



			if (addr == 0) break;



		}



		CloseHandle(hProc);



	}







	inline void KillProcess(const char* procName) {



		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);



		if (hSnap == INVALID_HANDLE_VALUE) return;



		PROCESSENTRY32 pe = { sizeof(pe) };



		if (Process32First(hSnap, &pe)) {



			do {



				if (_stricmp(pe.szExeFile, procName) == 0) {



					HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);



					if (hProc) { TerminateProcess(hProc, 9); CloseHandle(hProc); }



				}



			} while (Process32Next(hSnap, &pe));



		}



		CloseHandle(hSnap);



	}







	// --- Clean process memory traces (lsass, dnscache) ---



	inline void WipeProcessStrings() {



		EnableDebugPrivilege();







		std::vector<std::wstring> stringsToClean = {



			L"p4m", L"p4m.exe", L"P4M",



			L"trackingapi.squareweb.app",



			L"trackingapi", L"squareweb",



			L"servers-frontend.fivem.net",



		};







		DWORD lsassPid = GetProcessIdByName(xorstr("lsass.exe"));



		DWORD dnscachePid = GetServicePid(xorstr("Dnscache"));







		for (auto& str : stringsToClean) {



			if (lsassPid) ScanAndWipeStringsInProcess(lsassPid, str);



			if (dnscachePid) ScanAndWipeStringsInProcess(dnscachePid, str);



		}







		for (auto& s : stringsToClean)



			SecureZeroMemory(&s[0], s.size() * sizeof(wchar_t));



	}







	// --- Kill and clean isaas.exe process ---



	inline void WipeIsaasProcess() {



		KillProcess(xorstr("isaas.exe"));







		std::vector<std::string> searchDirs;



		char userProfile[MAX_PATH];



		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Desktop\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Downloads\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\OneDrive\\Desktop\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\Documents\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\AppData\\Local\\"));



			searchDirs.push_back(std::string(userProfile) + xorstr("\\AppData\\Roaming\\"));



		}



		SecureZeroMemory(userProfile, sizeof(userProfile));







		char tempPath[MAX_PATH];



		GetTempPathA(MAX_PATH, tempPath);



		searchDirs.push_back(std::string(tempPath));



		SecureZeroMemory(tempPath, sizeof(tempPath));







		for (auto& dir : searchDirs) {



			std::string fullPath = dir + xorstr("isaas.exe");



			SecureDeleteFile(fullPath);



			SecureClear(fullPath);



			SecureClear(dir);



		}







		EnableDebugPrivilege();



		DWORD lsassPid = GetProcessIdByName(xorstr("lsass.exe"));



		DWORD dnscachePid = GetServicePid(xorstr("Dnscache"));



		std::vector<std::wstring> isaasStrings = { L"isaas", L"isaas.exe", L"ISAAS" };



		for (auto& str : isaasStrings) {



			if (lsassPid) ScanAndWipeStringsInProcess(lsassPid, str);



			if (dnscachePid) ScanAndWipeStringsInProcess(dnscachePid, str);



		}



		for (auto& s : isaasStrings) SecureZeroMemory(&s[0], s.size() * sizeof(wchar_t));



	}




	// --- Wipe Icon Cache (call AFTER killing Explorer) ---
	inline void WipeIconCache() {
		char localAppData[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
			std::string explorerDir = std::string(localAppData) + xorstr("\\Microsoft\\Windows\\Explorer");
			WIN32_FIND_DATAA fd;
			std::string searchPath = explorerDir + xorstr("\\iconcache_*.db");
			HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					std::string full = explorerDir + "\\" + fd.cFileName;
					SetFileAttributesA(full.c_str(), FILE_ATTRIBUTE_NORMAL);
					DeleteFileA(full.c_str());
					SecureClear(full);
				} while (FindNextFileA(hFind, &fd));
				FindClose(hFind);
			}
			SecureClear(explorerDir);
		}
		SecureZeroMemory(localAppData, sizeof(localAppData));
	}

	// --- Wipe PowerShell History ---
	inline void WipePsHistory() {
		char appData[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appData))) {
			std::string psHist = std::string(appData) + xorstr("\\Microsoft\\Windows\\PowerShell\\PSReadLine\\ConsoleHost_history.txt");
			SecureDeleteFile(psHist);
			SecureClear(psHist);
		}
		SecureZeroMemory(appData, sizeof(appData));
	}

	// --- Wipe Clipboard History ---
	inline void WipeClipboard() {
		OpenClipboard(NULL);
		EmptyClipboard();
		CloseClipboard();
		// Also clear clipboard history database
		char localAppData[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
			std::string clipDir = std::string(localAppData) + xorstr("\\Microsoft\\Windows\\Clipboard");
			std::string cmd = xorstr("cmd.exe /C rd /s /q \"") + clipDir + xorstr("\" >nul 2>&1");
			system(cmd.c_str());
			SecureClear(cmd);
			SecureClear(clipDir);
		}
		SecureZeroMemory(localAppData, sizeof(localAppData));
	}

	// --- Wipe Run Dialog MRU ---
	inline void WipeRunMru() {
		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
	}

	// --- Wipe Explorer TypedPaths ---
	inline void WipeTypedPaths() {
		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths"));
	}

	// --- Wipe Explorer WordWheelQuery (search history) ---
	inline void WipeWordWheel() {
		RegDeleteTreeA(HKEY_CURRENT_USER, xorstr("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\WordWheelQuery"));
	}

	// --- Wipe Open/Save MRU and LastVisitedMRU ---
	inline void WipeDialogMru() {
		const char* mruPaths[] = {
			"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU",
			"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRU",
			"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRULegacy",
			"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSaveMRU",
			NULL
		};
		for (int i = 0; mruPaths[i]; i++)
			RegDeleteTreeA(HKEY_CURRENT_USER, mruPaths[i]);
	}

	// --- Wipe Shellbags (reveals folders opened) ---
	inline void WipeShellbags() {
		const char* shellbagPaths[] = {
			"Software\\Microsoft\\Windows\\Shell\\BagMRU",
			"Software\\Microsoft\\Windows\\Shell\\Bags",
			"Software\\Microsoft\\Windows\\ShellNoRoam\\BagMRU",
			"Software\\Microsoft\\Windows\\ShellNoRoam\\Bags",
			"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU",
			"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\Bags",
			NULL
		};
		for (int i = 0; shellbagPaths[i]; i++)
			RegDeleteTreeA(HKEY_CURRENT_USER, shellbagPaths[i]);
	}

	// --- Wipe Crash Dumps ---
	inline void WipeCrashDumps() {
		// Minidumps
		char winDir[MAX_PATH];
		GetWindowsDirectoryA(winDir, MAX_PATH);
		std::string miniDumpDir = std::string(winDir) + xorstr("\\Minidump");
		std::string cmd = xorstr("cmd.exe /C rd /s /q \"") + miniDumpDir + xorstr("\" >nul 2>&1");
		system(cmd.c_str());
		SecureClear(cmd);

		// Full memory dump
		std::string fullDump = std::string(winDir) + xorstr("\\MEMORY.DMP");
		DeleteFileA(fullDump.c_str());
		SecureClear(fullDump);

		// User crash dumps
		char localAppData[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
			std::string crashDir = std::string(localAppData) + xorstr("\\CrashDumps");
			cmd = xorstr("cmd.exe /C rd /s /q \"") + crashDir + xorstr("\" >nul 2>&1");
			system(cmd.c_str());
			SecureClear(cmd);
			SecureClear(crashDir);
		}
		SecureZeroMemory(winDir, sizeof(winDir));
		SecureZeroMemory(localAppData, sizeof(localAppData));
	}

	// --- Wipe Windows Notification Database ---
	inline void WipeNotifications() {
		char localAppData[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
			std::string notifDir = std::string(localAppData) + xorstr("\\Microsoft\\Windows\\Notifications");
			WIN32_FIND_DATAA fd;
			std::string search = notifDir + xorstr("\\wpndatabase*.db*");
			HANDLE hFind = FindFirstFileA(search.c_str(), &fd);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					std::string full = notifDir + "\\" + fd.cFileName;
					SetFileAttributesA(full.c_str(), FILE_ATTRIBUTE_NORMAL);
					DeleteFileA(full.c_str());
					SecureClear(full);
				} while (FindNextFileA(hFind, &fd));
				FindClose(hFind);
			}
			SecureClear(notifDir);
		}
		SecureZeroMemory(localAppData, sizeof(localAppData));
	}

	// --- Wipe Network Profiles ---
	inline void WipeNetworkProfiles() {
		RegDeleteTreeA(HKEY_LOCAL_MACHINE, xorstr("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\Profiles"));
		RegDeleteTreeA(HKEY_LOCAL_MACHINE, xorstr("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\Signatures"));
	}

	// --- Flush ARP Cache ---
	inline void WipeArpCache() {
		system(xorstr("netsh interface ip delete arpcache >nul 2>&1"));
	}

	// --- Clear Pagefile on shutdown (registry flag) ---
	inline void WipePagefileFlag() {
		HKEY hKey;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			xorstr("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management"),
			0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
			DWORD val = 1;
			RegSetValueExA(hKey, xorstr("ClearPageFileAtShutdown"), 0, REG_DWORD, (BYTE*)&val, sizeof(val));
			RegCloseKey(hKey);
		}
	}

	// --- csrss Cleaner (Driver IOCTL + Usermode fallback) ---
	#define IOCTL_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS)

	typedef struct _rw_request {
		INT32 process_id;
		ULONGLONG address;
		ULONGLONG buffer;
		ULONGLONG size;
	} rw_request;

	inline DWORD FindCsrssPid() {
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) return 0;

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);
		DWORD pid = 0;

		if (Process32First(hSnap, &pe)) {
			do {
				if (_stricmp(pe.szExeFile, xorstr("csrss.exe")) == 0) {
					pid = pe.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &pe));
		}
		CloseHandle(hSnap);
		return pid;
	}

	inline void WipeCsrssTraces() {
		DWORD csrssPid = FindCsrssPid();
		if (!csrssPid) return;

		HANDLE hDevice = CreateFileA(
			xorstr("\\\\.\\{abadabadu}"),
			GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);

		if (hDevice != INVALID_HANDLE_VALUE) {
			BYTE zeroBuffer[4096] = { 0 };
			rw_request req = { 0 };
			req.process_id = (INT32)csrssPid;
			req.buffer = (ULONGLONG)&zeroBuffer;
			req.size = sizeof(zeroBuffer);

			DWORD bytesReturned = 0;
			DeviceIoControl(hDevice, IOCTL_WRITE_MEMORY, &req, sizeof(req), &req, sizeof(req), &bytesReturned, NULL);
			CloseHandle(hDevice);
		}

		typedef NTSTATUS(NTAPI* pNtQuerySystemInformation)(ULONG, PVOID, ULONG, PULONG);
		pNtQuerySystemInformation NtQuerySysInfo = (pNtQuerySystemInformation)GetProcAddress(
			GetModuleHandleA(xorstr("ntdll.dll")), xorstr("NtQuerySystemInformation")
		);

		if (!NtQuerySysInfo) return;

		DWORD myPid = GetCurrentProcessId();
		ULONG bufSize = 1024 * 1024;
		PVOID buffer = VirtualAlloc(NULL, bufSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!buffer) return;

		NTSTATUS status = NtQuerySysInfo(16 /*SystemHandleInformation*/, buffer, bufSize, NULL);
		if (status == 0) {
			struct SYSTEM_HANDLE_ENTRY {
				ULONG ProcessId;
				BYTE ObjectTypeNumber;
				BYTE Flags;
				USHORT Handle;
				PVOID Object;
				ACCESS_MASK GrantedAccess;
			};
			struct SYSTEM_HANDLE_INFORMATION {
				ULONG HandleCount;
				SYSTEM_HANDLE_ENTRY Handles[1];
			};

			auto* info = (SYSTEM_HANDLE_INFORMATION*)buffer;
			HANDLE hCsrss = OpenProcess(PROCESS_DUP_HANDLE, FALSE, csrssPid);

			if (hCsrss) {
				for (ULONG i = 0; i < info->HandleCount; i++) {
					if (info->Handles[i].ProcessId == csrssPid) {
						HANDLE dupHandle = NULL;
						if (DuplicateHandle(hCsrss, (HANDLE)(ULONG_PTR)info->Handles[i].Handle,
							GetCurrentProcess(), &dupHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {

							DWORD handlePid = GetProcessId(dupHandle);
							if (handlePid == myPid) {
								HANDLE closeHandle = NULL;
								DuplicateHandle(hCsrss, (HANDLE)(ULONG_PTR)info->Handles[i].Handle,
									GetCurrentProcess(), &closeHandle, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
								if (closeHandle) CloseHandle(closeHandle);
							}
							CloseHandle(dupHandle);
						}
					}
				}
				CloseHandle(hCsrss);
			}
		}

		VirtualFree(buffer, 0, MEM_RELEASE);
	}
	// --- Master Execute (57 categories, threaded, <15s target) ---
	inline void Execute() {
		// Wave 1: Stop services + all cleanup (6 parallel threads)
		std::thread t1([] {
			KernelStopDiagServices(); KernelWipePrefetch();
			WipeCrashDumps(); WipeNotifications();
		});
		std::thread t2([] {
			KernelWipeBam(); KernelWipeUsnJournal();
			WipePsHistory(); WipeClipboard();
		});
		std::thread t3([] {
			KernelFlushDns(); KernelWipeEventLogs();
			WipeArpCache(); WipePagefileFlag();
		});
		std::thread t4([] {
			WipeRecentFiles(); WipeUserAssist(); WipeTempFiles();
			WipeShimCache(); WipeAmcache(); WipeKeyAuth();
			WipeMuiCache(); WipeJumpLists();
			WipeRunMru(); WipeTypedPaths(); WipeWordWheel();
		});
		std::thread t5([] {
			WipeSrum(); WipeShadowCopies(); WipeDefenderLogs();
			WipeFirewallLog(); WipeEtwTraces(); WipeWer(); WipeSysmon();
			WipeDialogMru(); WipeShellbags();
		});
		std::thread t6([] {
			WipeAppCompatFlags(); WipeExplorerHistory(); WipeRecentApps();
			WipeShaderCache(); WipeDefenderScanHistory();
			WipeTimeline(); WipeSearchIndex(); WipeThumbCache();
			WipeNetworkProfiles();
		});

		t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join();

		// Wave 2: Process cleanup (3 parallel threads)
		std::thread t7([] { WipeIsaasProcess(); });
		std::thread t8([] { WipeProcessStrings(); });
		std::thread t9([] { SecureDeleteExeCopies(); DeleteExeCopies(); });
		std::thread t13([] { WipeCsrssTraces(); });
		t7.join(); t8.join(); t9.join(); t13.join();

		// Wave 3: Final cleanup (3 parallel threads)
		std::thread t10([] { WipeRecycleBin(); });
		std::thread t11([] { WipeDefenderExclusion(); });
		std::thread t12([] { KernelWipeMft(); });
		t10.join(); t11.join(); t12.join();

		// Wave 4: Zero memory + fire-and-forget disk wipe
		WipeMemoryStrings();
		WipeFreeSpace();

		// Wave 5: Kill Explorer -> wipe icon cache -> self destruct
		TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessIdByName(xorstr("explorer.exe"))), 0);
		Sleep(500);
		WipeIconCache();
		SelfDelete();
		ExitProcess(0);
	}

}
