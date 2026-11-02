#pragma once
#include <Gui/Overlay/Overlay.hpp>
#include <Includes/Includes.hpp>
#include <Core/SDK/Memory.hpp>
#include <Includes/Utils.hpp>
#include <Core/SDK/SDK.hpp>
#include <Core/Core.hpp>
#include <Gui/gui.hpp>
#include <winternl.h>
#include <windows.h>
#include <dwmapi.h>
#include <tchar.h>
#include <vector>
#include <regex>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <string>

#include <Security/AntiCrack.hpp>
#include <Security/Api/Auth/api/api.hh>
#include <Includes/CustomWidgets/Custom.hpp>
#include <Includes/ImGui/Fonts.hpp>
#include <../Security/api.hpp>
#include <../Globals.hpp>
#include "../Includes/keyauth/auth.hpp"
#include "../Security/xorstr.hpp"
using namespace Core;
using namespace KeyAuth;

std::string name = xorstr("appname");
std::string ownerid = xorstr("ownerid");
std::string version = xorstr("1.0");
std::string url = xorstr("https://keyauth.win/api/1.2/");
std::string path = xorstr("");

api KeyAuthApp(name, ownerid, version, url, path);





RECT rc;

void style() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowRounding = 10.0f;
	style->WindowPadding = ImVec2(0.f, 0.f);
	style->WindowBorderSize = 1.f;
	style->Alpha = 1.1f;

	style->Colors[ImGuiCol_WindowBg] = ImVec4(ImColor(17,17,21));
	style->Colors[ImGuiCol_ChildBg] = ImVec4(ImColor(255, 255, 255, 255));
	style->Colors[ImGuiCol_Border] = ImVec4(22, 22, 22, 0);
	//style->WindowBorderSize = 0.1f;
}

inline std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}


ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

D3DX11_IMAGE_LOAD_INFO info;
ID3DX11ThreadPump* pump{ nullptr };
HWND hwnd;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ImVec2 window_size = ImVec2(440, 320);

void move_window() {
	ImGui::SetCursorPos(ImVec2(0, 0));
	if (ImGui::InvisibleButton("Move_detector", ImVec2(window_size.x, window_size.y)));
	if (ImGui::IsItemActive()) {

		GetWindowRect(hwnd, &rc);
		MoveWindow(hwnd, rc.left + ImGui::GetMouseDragDelta().x, rc.top + ImGui::GetMouseDragDelta().y, window_size.x, window_size.y, TRUE);
	}
}


 bool flag = false;
 c_globals globals;
 static enum PANELS
	{
		login_panel,
		register_panel,

	} panel;
 bool render() {
	 ImGui::SetNextWindowSize(ImVec2(window_size));
	 ImGui::SetNextWindowPos(ImVec2(0, 0));

	 ImGui::Begin("##menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);
	 {
		 switch (panel)
		 {
			 			 case login_panel:
			 {
				 ImDrawList* draw = ImGui::GetWindowDrawList();
				 ImVec2 wp = ImGui::GetWindowPos();

				 // --- Top accent bar (thin celeste line at top) ---
				 draw->AddRectFilled(
					 ImVec2(wp.x, wp.y),
					 ImVec2(wp.x + window_size.x, wp.y + 3),
					 IM_COL32(100, 180, 255, 255)
				 );

				 // --- "p4m" title centered ---
				 ImGui::PushFont(globals.open_sans_sm);
				 {
					 const char* title = "p4m";
					 ImVec2 titleSz = ImGui::CalcTextSize(title);
					 float titleX = (window_size.x - titleSz.x) * 0.5f;
					 draw->AddText(globals.open_sans_sm, 21.0f, ImVec2(wp.x + titleX, wp.y + 25), IM_COL32(100, 180, 255, 255), title);
				 }
				 ImGui::PopFont();

				 // --- Subtitle ---
				 ImGui::PushFont(globals.open_sans);
				 {
					 const char* sub = "Sign in to continue";
					 ImVec2 subSz = ImGui::CalcTextSize(sub);
					 float subX = (window_size.x - subSz.x) * 0.5f;
					 draw->AddText(globals.open_sans, 18.0f, ImVec2(wp.x + subX, wp.y + 55), IM_COL32(120, 120, 130, 255), sub);
				 }

				 // --- Separator line ---
				 float sepY = 85.0f;
				 float sepPad = 60.0f;
				 draw->AddLine(
					 ImVec2(wp.x + sepPad, wp.y + sepY),
					 ImVec2(wp.x + window_size.x - sepPad, wp.y + sepY),
					 IM_COL32(100, 180, 255, 60), 1.0f
				 );

				 // --- Input styling ---
				 float inputW = 300.0f;
				 float inputX = (window_size.x - inputW) * 0.5f;
				 ImVec2 inputSz(inputW, 32);

				 ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
				 ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				 ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.10f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.10f, 0.10f, 0.13f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.12f, 0.12f, 0.15f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.18f, 0.22f, 1.0f));
				 ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.87f, 1.0f));

				 // Username label
				 ImGui::SetCursorPos(ImVec2(inputX, 100));
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.55f, 0.60f, 1.0f));
				 ImGui::Text("USERNAME");
				 ImGui::PopStyleColor();

				 // Username input
				 ImGui::SetCursorPos(ImVec2(inputX, 120));
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.90f, 0.92f, 1.0f));
				 ImGui::InputTextEx("##User", "", globals.userLogin, IM_ARRAYSIZE(globals.userLogin), inputSz, 0);
				 ImGui::PopStyleColor();

				 // Password label
				 ImGui::SetCursorPos(ImVec2(inputX, 163));
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.55f, 0.55f, 0.60f, 1.0f));
				 ImGui::Text("PASSWORD");
				 ImGui::PopStyleColor();

				 // Password input
				 ImGui::SetCursorPos(ImVec2(inputX, 183));
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.90f, 0.92f, 1.0f));
				 ImGui::InputTextEx("##Pass", "", globals.passLogin, IM_ARRAYSIZE(globals.passLogin), inputSz, ImGuiInputTextFlags_Password);
				 ImGui::PopStyleColor();

				 ImGui::PopStyleColor(5);
				 ImGui::PopStyleVar(3);

				 // --- Login button ---
				 float btnW = 300.0f;
				 float btnX = (window_size.x - btnW) * 0.5f;

				 ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
				 ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.39f, 0.71f, 1.0f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.75f, 1.0f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.30f, 0.60f, 0.90f, 1.0f));
				 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

				 ImGui::SetCursorPos(ImVec2(btnX, 230));

				 static char statusMsg[256] = "";
				 static bool loginFailed = false;

				 if (ImGui::ButtonEx("LOGIN", ImVec2(btnW, 34))) {
					 if (strcmp(globals.userLogin, xorstr("seko")) == 0 && strcmp(globals.passLogin, xorstr("seko")) == 0) {
						 flag = true;
					 } else {
						 loginFailed = true;
						 sprintf_s(statusMsg, "Invalid credentials");
					 }
				 }

				 ImGui::PopStyleColor(4);
				 ImGui::PopStyleVar();

				 // --- Error message ---
				 if (loginFailed) {
					 ImVec2 errSz = ImGui::CalcTextSize(statusMsg);
					 float errX = (window_size.x - errSz.x) * 0.5f;
					 ImGui::SetCursorPos(ImVec2(errX, 270));
					 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.35f, 0.35f, 1.0f));
					 ImGui::Text("%s", statusMsg);
					 ImGui::PopStyleColor();
				 }

				 // --- Version footer ---
				 {
					 const char* ver = "v1.0";
					 ImVec2 verSz = ImGui::CalcTextSize(ver);
					 float verX = (window_size.x - verSz.x) * 0.5f;
					 draw->AddText(ImVec2(wp.x + verX, wp.y + 298), IM_COL32(60, 60, 65, 255), ver);
				 }

				 ImGui::PopFont();

				 break;
			 }
		 }
		 move_window();
	 }
	 ImGui::End();
	 return flag;
 }


int gay()
{
	g_Variables.g_vGameWindowSize = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };
	g_Variables.g_vGameWindowPos = { 0, 0 };
	g_Variables.g_vGameWindowCenter = { g_Variables.g_vGameWindowSize.x / 2, g_Variables.g_vGameWindowSize.y / 2 };

	g_MenuInfo.IsOpen = true;

	Gui::cOverlay.Render();

	return 0;
}
// Verifica se o Brave (brave.exe) está rodando
bool IsFiveMRunning() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	bool found = false;
	if (Process32First(hSnapshot, &pe32)) {
		do {
			std::string exeName = pe32.szExeFile;
			if (exeName.find("FiveM") != std::string::npos) {
				found = true;
				break;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return found;
}

// Callback para verificar se há uma janela com "YouTube" no título
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	wchar_t title[256];
	GetWindowTextW(hwnd, title, sizeof(title) / sizeof(wchar_t));

	std::wstring windowTitle = title;
	if (windowTitle.find(L"YouTube") != std::wstring::npos) {
		*(bool*)lParam = true;
		return FALSE;
	}
	return TRUE;
}

// Verifica se o YouTube está aberto em alguma janela
bool IsYouTubeOpen() {
	bool found = false;
	EnumWindows(EnumWindowsProc, (LPARAM)&found);
	return found;
}



void ShowSplash() {
	WNDCLASSEXW sc = { sizeof(sc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"p4mSplash", NULL };
	RegisterClassExW(&sc);

	int splashW = 420, splashH = 240;
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED, sc.lpszClassName, L"p4m", WS_POPUP,
		(screenW - splashW) / 2, (screenH - splashH) / 2, splashW, splashH, NULL, NULL, sc.hInstance, NULL);

	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	if (!CreateDeviceD3D(hwnd)) { CleanupDeviceD3D(); UnregisterClassW(sc.lpszClassName, sc.hInstance); return; }

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;

	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	globals.open_sans = io.Fonts->AddFontFromMemoryCompressedBase85TTF(open_sans.data(), 18);
	globals.open_sans_sm = io.Fonts->AddFontFromMemoryCompressedBase85TTF(open_sans.data(), 24);

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	bool offsetsDone = false;
	bool offsetsOk = false;
	std::thread([&]() {
		offsetsOk = Core::SetupOffsets();
		if (!offsetsOk) { Sleep(3000); offsetsOk = Core::SetupOffsets(); }
		offsetsDone = true;
	}).detach();

	auto startTime = std::chrono::steady_clock::now();
	bool done = false;
	MSG msg;

	while (!done) {
		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); DispatchMessage(&msg);
			if (msg.message == WM_QUIT) done = true;
		}
		if (done) break;

		float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();
		float progress = offsetsDone ? 1.0f : (std::min)(elapsed / 2.5f, 0.92f);
		const char* status = offsetsDone ? (offsetsOk ? "Ready" : "Offline mode") : (elapsed < 0.8f ? "Initializing..." : elapsed < 1.6f ? "Finding process..." : "Loading offsets...");

		if (offsetsDone && elapsed >= 3.0f) done = true;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)splashW, (float)splashH));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::Begin("##splash", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		{
			ImDrawList* dl = ImGui::GetWindowDrawList();
			ImVec2 wp = ImGui::GetWindowPos();
			float w = (float)splashW, h = (float)splashH;

			dl->AddRectFilled(wp, ImVec2(wp.x + w, wp.y + h), IM_COL32(14, 14, 18, 255), 12.0f);
			dl->AddRect(wp, ImVec2(wp.x + w, wp.y + h), IM_COL32(40, 40, 55, 255), 12.0f);
			dl->AddRectFilled(wp, ImVec2(wp.x + w, wp.y + 3), IM_COL32(100, 180, 255, 255));

			ImGui::PushFont(globals.open_sans_sm);
			const char* title = "p4m";
			ImVec2 ts = ImGui::CalcTextSize(title);
			dl->AddText(ImVec2(wp.x + (w - ts.x) / 2, wp.y + 50), IM_COL32(100, 180, 255, 255), title);
			ImGui::PopFont();

			ImGui::PushFont(globals.open_sans);
			const char* sub = "External Overlay";
			ImVec2 ss = ImGui::CalcTextSize(sub);
			dl->AddText(ImVec2(wp.x + (w - ss.x) / 2, wp.y + 85), IM_COL32(100, 100, 115, 255), sub);

			float barW = 300, barH = 6, barX = (w - barW) / 2, barY = 140;
			dl->AddRectFilled(ImVec2(wp.x + barX, wp.y + barY), ImVec2(wp.x + barX + barW, wp.y + barY + barH), IM_COL32(30, 30, 42, 255), 3.0f);

			float fillW = barW * progress;
			if (fillW > 0) {
				ImU32 c1 = IM_COL32(80, 160, 255, 255), c2 = IM_COL32(140, 200, 255, 255);
				dl->AddRectFilledMultiColor(ImVec2(wp.x + barX, wp.y + barY), ImVec2(wp.x + barX + fillW, wp.y + barY + barH), c1, c2, c2, c1);
			}

			ImVec2 sts = ImGui::CalcTextSize(status);
			dl->AddText(ImVec2(wp.x + (w - sts.x) / 2, wp.y + 160), offsetsDone ? IM_COL32(100, 220, 130, 255) : IM_COL32(160, 160, 175, 255), status);

			char pctBuf[16]; sprintf_s(pctBuf, "%d%%", (int)(progress * 100));
			ImVec2 ps = ImGui::CalcTextSize(pctBuf);
			dl->AddText(ImVec2(wp.x + (w - ps.x) / 2, wp.y + 185), IM_COL32(80, 80, 95, 255), pctBuf);

			const char* ver = "v1.0";
			ImVec2 vs = ImGui::CalcTextSize(ver);
			dl->AddText(ImVec2(wp.x + (w - vs.x) / 2, wp.y + h - 25), IM_COL32(50, 50, 60, 255), ver);
			ImGui::PopFont();
		}
		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::EndFrame();

		const float clear[4] = { 0 };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear);
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClassW(sc.lpszClassName, sc.hInstance);
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (!IsFiveMRunning()) {
		return 0;
	}

	ShowSplash();
	gay();
	return 0;

	// --- Original login window code (bypassed) ---

	/*AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);*/

	WNDCLASSEXW wc;
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = nullptr;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = L" ";
	wc.lpszClassName = L" ";
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassExW(&wc);
	hwnd = CreateWindowExW(WS_EX_TOPMOST, wc.lpszClassName, L" ", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (window_size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (window_size.y / 2), window_size.x,window_size.y, 0, 0, 0, 0);
	//gui::RenderBlur( globals.hwnd );

	//SetWindowDisplayAffinity(globals.hwnd, WDA_EXCLUDEFROMCAPTURE);

	SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margins);

	POINT mouse;
	rc = { 0 };
	GetWindowRect(hwnd, &rc);

	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return 1;
	}


	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(); {
		ImGui::StyleColorsDark();


		style();
		
	}
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImFontConfig cfg;
	cfg.FontBuilderFlags = ImGuiBackendFlags_None; 
	cfg.FontDataOwnedByAtlas = false;
	//io.Fonts->AddFontFromMemoryTTF(InterSemiBold, sizeof(InterSemiBold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	//globals.open_sans = io.Fonts->AddFontFromMemoryTTF(open_sans, sizeof(open_sans), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	globals.open_sans = io.Fonts->AddFontFromMemoryCompressedBase85TTF(open_sans.data(), 18);
	globals.open_sans_sm = io.Fonts->AddFontFromMemoryCompressedBase85TTF(open_sans.data(), 21);

	static const ImWchar icons_ranges[] = { 0xe000, 0xf8ff, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	static const ImWchar FontAwesomeRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	static const ImWchar FontAwesomeRangesBrands[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

	ImFontConfig FontAwesomeConfig;
	FontAwesomeConfig.MergeMode = true;
	FontAwesomeConfig.PixelSnapH = true;
	FontAwesomeConfig.GlyphMinAdvanceX = 17.f;

	//globals.FontAwesomeSolid = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Solid_compressed_data, sizeof(FontAwesome6Solid_compressed_size), 17.f, &FontAwesomeConfig, FontAwesomeRanges);
	//globals.FontAwesomeRegular = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Regular_compressed_data, sizeof(FontAwesome6Regular_compressed_size), 17.f, &FontAwesomeConfig, FontAwesomeRanges);
	//globals.FontAwesomeBrands = io.Fonts->AddFontFromMemoryCompressedTTF(FontAwesome6Brands_compressed_data, sizeof(FontAwesome6Brands_compressed_size), 17.f, &FontAwesomeConfig, FontAwesomeRangesBrands);

	io.IniFilename = nullptr;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static bool done = false;
	while (!done)
	{

		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}

		if (done)
			break;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		static bool hide = true;
		static int opticaly = 255;
		/*if ( GetAsyncKeyState( VK_INSERT ) & 0x1 ) hide = !hide;
		if ( GetAsyncKeyState( VK_DELETE ) & 1 ) { exit( 0 ); }*/
		opticaly = ImLerp(opticaly, opticaly <= 255 && hide ? 300 : 0, ImGui::GetIO().DeltaTime * 8.f);
		if (opticaly > 255) opticaly = 255;

		SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), opticaly, LWA_ALPHA);

		//SetWindowDisplayAffinity(globals.hwnd, WDA_EXCLUDEFROMCAPTURE);

		ShowWindow(hwnd, opticaly > 0 ? SW_SHOW : SW_HIDE);
		if (!flag) SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		ImGui::NewFrame();
		{
			ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
			ImGui::SetNextWindowSize({window_size.x, window_size.y }, ImGuiCond_Once);

			ImGui::Begin(" ", &done, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
			{

				if (!flag) {
					render();
					move_window();
					
				}
				else
				{
					ShowWindow(hwnd, SW_HIDE);
					done = true;
					gay();
				}
			
			}
			ImGui::End();
		}

		ImGui::Render();


		const float clear_color_with_alpha[4] = { 0 };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) {
		g_pSwapChain->Release();
		g_pSwapChain = NULL;
	}

	if (g_pd3dDeviceContext) {
		g_pd3dDeviceContext->Release();
		g_pd3dDeviceContext = NULL;
	}

	if (g_pd3dDevice) {
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) {
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = NULL;
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}







std::string hwid;
