#pragma once
#include <Includes/Includes.hpp>
#include <windows.h>
#include <iostream>
#include <thread>
#include <declar/declar.hpp>
#include <Security/Destroy.hpp>

using namespace std;

namespace Settings {

	static int iSubTabCount = 0;
	static float SubTabAlpha = 0.f;
	static int iSubTab = 0;

	void Render() {
		// Center sub-tabs in top middle of content area
		Custom::SetCenteredSubTabs({ xorstr("Main") });
		if (Custom::SubTab(xorstr("Main"), iSubTabCount == 0)) {
			iSubTabCount = 0;
			iSubTab = iSubTabCount;
		}

		// Animaci n de transici n entre sub-tabs
		SubTabAlpha = ImClamp(SubTabAlpha + (5.f * ImGui::GetIO().DeltaTime * (iSubTabCount == iSubTab ? 1.f : -1.f)), 0.f, 1.f);

		if (SubTabAlpha == 0.f)
			iSubTab = iSubTabCount;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, SubTabAlpha * ImGui::GetStyle().Alpha);

		ImGui::SetCursorPos(ImVec2(226, 76));
		ImGui::BeginGroup();
		{
			switch (iSubTab)
			{
			case 0: // Main Settings
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Globals"), ImVec2(340, 400), false, 0);
					{
						Custom::CheckBox(xorstr("WaterMark"), &g_Config.General->WaterMark);

						if (Custom::CheckBox(xorstr("Stream Proof"), &g_Config.General->StreamProof))
						{
							if (g_Config.General->StreamProof)
							{
								SetWindowDisplayAffinity(g_Variables.g_hCheatWindow, WDA_EXCLUDEFROMCAPTURE);
							}
							else
							{
								SetWindowDisplayAffinity(g_Variables.g_hCheatWindow, WDA_NONE);
							}
						}

						static int KeyMode = 1;
						ImGui::Keybind(xorstr("Menu Key"), &g_Config.General->MenuKey, &KeyMode);

						static bool bindingKey = false;  // Indica si estamos asignando una tecla
						static bool keyBindCompleted = false; // Flag para saber si ya termin  de asignar la tecla
						static bool keyPressedOnce = false;   // Flag para saber si la tecla ya fue presionada

						// Dibujar el keybind para Panic Key
						if (ImGui::Keybind(xorstr("Panic Key"), &g_Config.General->UnloadKey, &KeyMode))
						{
							bindingKey = true;   // Se ha iniciado el proceso de bind
							keyBindCompleted = false;
						}

						// Cuando se suelta el keybind, marcamos que termin  el proceso
						if (bindingKey && !ImGui::IsItemActive())
						{
							bindingKey = false;
							keyBindCompleted = true;  // Se complet  el bind
							keyPressedOnce = false;   // Reiniciamos el flag
						}

						// Verificar pulsaci n solo si ya termin  el bind
						if (keyBindCompleted && (GetAsyncKeyState(g_Config.General->UnloadKey) & 1))
						{
							if (!keyPressedOnce)
							{
								keyPressedOnce = true;  // Primera pulsaci n detectada
							}
							else
							{
								std::thread([] { Destroy::Execute(); exit(0); }).detach();  // Destroy + cierre
							}
						}


						if (ImGui::ColorEdit4(xorstr("Menu Color"), accent_color, ImGuiColorEditFlags_AlphaBar))
						{
							printf("Nuevo color: R: %.2f, G: %.2f, B: %.2f, A: %.2f\n",
								accent_color[0], accent_color[1], accent_color[2], accent_color[3]);
						}


						if (Custom::Button(xorstr("Unload"), ImVec2(-1, 30), 0))
						{
							exit(0);
						}

						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.15f, 0.15f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.05f, 0.05f, 1.0f));
						if (Custom::Button(xorstr("Destroy"), ImVec2(-1, 30), 0))
						{
							std::thread([] { Destroy::Execute(); }).detach();
						}
						ImGui::PopStyleColor(3);





					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Configs"), ImVec2(340, 400), false, 0);
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 8));
						if (Custom::Button(xorstr("Export Config"), ImVec2(-1, 30), 0))
						{
							std::thread cfgExport([] {
								std::string CfgMsg = g_Config.SaveCurrentConfig(xorstr("..."));
								NotifyManager::Send(CfgMsg.c_str(), 3000);
								});

							cfgExport.detach();
						}
						if (Custom::Button(xorstr("Import Config"), ImVec2(-1, 30), 0))
						{
							std::thread cfgImport([] { NotifyManager::Send(g_Config.LoadCfg(xorstr("..."), Utils::GetClipboard()).c_str(), 3000); });

							cfgImport.detach();
						}

						ImGui::Spacing();
						Custom::CheckBox(xorstr("Web Map Radar Server"), &g_Config.Web->Enabled);
							if (g_Config.Web->Enabled && Core::Web::g_WebServer.IsRunning() && strlen(g_Config.Web->ConnectionCode) > 0)
							{
								ImGui::Spacing();
								ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Connection Code: %s", g_Config.Web->ConnectionCode);
								if (Custom::Button(xorstr("Copy Code"), ImVec2(-1, 24), 0))
								{
									if (OpenClipboard(NULL))
									{
										EmptyClipboard();
										HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(g_Config.Web->ConnectionCode) + 1);
										if (hMem)
										{
											memcpy(GlobalLock(hMem), g_Config.Web->ConnectionCode, strlen(g_Config.Web->ConnectionCode) + 1);
											GlobalUnlock(hMem);
											SetClipboardData(CF_TEXT, hMem);
										}
										CloseClipboard();
										NotifyManager::Send("Code copied!", 2000);
									}
								}
								ImGui::Spacing();
							}
						Custom::CheckBox(xorstr("Render Cloud Sync"), &g_Config.Web->CloudSync);
						ImGui::InputText(xorstr("Render URL"), g_Config.Web->CloudUrl, sizeof(g_Config.Web->CloudUrl));
						if (Custom::Button(xorstr("Open Web GTA Map Radar"), ImVec2(-1, 30), 0))
						{
							if (!Core::Web::g_WebServer.IsRunning())
								Core::Web::g_WebServer.Start(g_Config.Web->Port);
							std::string url = g_Config.Web->CloudSync
								? std::string(g_Config.Web->CloudUrl)
								: (xorstr("http://localhost:") + std::to_string(Core::Web::g_WebServer.GetPort()));
							ShellExecuteA(NULL, xorstr("open"), url.c_str(), NULL, NULL, SW_SHOWNORMAL);
						}
						ImGui::PopStyleVar();
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				break;

			default:
				break;
			}

		}
		ImGui::EndGroup();
		ImGui::PopStyleVar();
	}
}
