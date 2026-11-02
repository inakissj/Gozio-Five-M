#pragma once
#include <Includes/Includes.hpp>
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;

namespace Combat {

	static int iSubTabCount = 0;
	static float SubTabAlpha = 0.f;
	static int iSubTab = 0;

	void Render() {


		// Center sub-tabs in top middle of content area
		Custom::SetCenteredSubTabs({ xorstr("Aimbot"), xorstr("SilentAim"), xorstr("TriggerBot"), xorstr("Auto Peek") });
		if (Custom::SubTab(xorstr("Aimbot"), 0 == iSubTabCount)) {
			iSubTabCount = 0;
		}
		ImGui::SameLine();
		if (Custom::SubTab(xorstr("SilentAim"), 1 == iSubTabCount)) {
			iSubTabCount = 1;
		}
		ImGui::SameLine();
		if (Custom::SubTab(xorstr("TriggerBot"), 2 == iSubTabCount)) {
			iSubTabCount = 2;
		}
		ImGui::SameLine();
		if (Custom::SubTab(xorstr("Auto Peek"), 3 == iSubTabCount)) {
			iSubTabCount = 3;
		}

		SubTabAlpha = ImClamp(SubTabAlpha + (5.f * ImGui::GetIO().DeltaTime * (iSubTabCount == iSubTab ? 1.f : -1.f)), 0.f, 1.f);

		if (SubTabAlpha == 0.f)
			iSubTab = iSubTabCount;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, SubTabAlpha * ImGui::GetStyle().Alpha);

		ImGui::SetCursorPos(ImVec2(226, 76));
		ImGui::BeginGroup();
		{
			switch (iSubTab)
			{
			case 0: //Aimbot
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Aimbot"), ImVec2(340, 400), false, 0);
					{
						static int KeyMode = 1;

						Custom::CheckBox(xorstr("Enable"), &g_Config.Aimbot->Enabled);
						Custom::CheckBox(xorstr("Only Visible"), &g_Config.Aimbot->OnlyVisible);
						Custom::CheckBox(xorstr("Ignore NPCs"), &g_Config.Aimbot->IgnoreNPCs);
						Custom::CheckBox(xorstr("Show Fov"), &g_Config.Aimbot->ShowFov);

						ImGui::Keybind(xorstr("Bind"), &g_Config.Aimbot->KeyBind, &KeyMode);

					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Settings"), ImVec2(340, 400), false, 0);
					{
						ImGui::SliderInt(xorstr("Field of View"), &g_Config.Aimbot->FOV, 0, 400, xorstr("%d"), 0);
						ImGui::SliderInt(xorstr("Smooth"), &g_Config.Aimbot->AimbotSpeed, 0, 100, xorstr("%d"), 0);
						ImGui::SliderInt(xorstr("Max Distance"), &g_Config.Aimbot->MaxDistance, 0, 1000, xorstr("%dm"), 0);
						static float AimbotFovCol[4] = { g_Config.Aimbot->FovColor.Value.x, g_Config.Aimbot->FovColor.Value.y, g_Config.Aimbot->FovColor.Value.z,g_Config.Aimbot->FovColor.Value.w };
						if (ImGui::ColorEdit4(xorstr("Aimbot Fov"), AimbotFovCol, ImGuiColorEditFlags_AlphaBar))
						{
							g_Config.Aimbot->FovColor.Value.x = AimbotFovCol[0];
							g_Config.Aimbot->FovColor.Value.y = AimbotFovCol[1];
							g_Config.Aimbot->FovColor.Value.z = AimbotFovCol[2];
							g_Config.Aimbot->FovColor.Value.w = AimbotFovCol[3];
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();

				break;
			case 1: //Silent
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Silent"), ImVec2(340, 400), false, 0);
					{
						Custom::CheckBox(xorstr("Enable"), &g_Config.SilentAim->Enabled);
						Custom::CheckBox(xorstr("Magic Bullets"), &g_Config.SilentAim->MagicBullets);
						Custom::CheckBox(xorstr("Only Visible"), &g_Config.SilentAim->OnlyVisible);
						Custom::CheckBox(xorstr("Ignore NPCs"), &g_Config.SilentAim->IgnoreNPCs);
						Custom::CheckBox(xorstr("Show Fov"), &g_Config.SilentAim->ShowFov);
						static int KeyMode = 1;

						ImGui::Keybind(xorstr("Bind"), &g_Config.SilentAim->KeyBind, &KeyMode);

					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Settings"), ImVec2(340, 400), false, 0);
					{
						ImGui::SliderInt(xorstr("Field of View"), &g_Config.SilentAim->FOV, 0, 400, xorstr("%d"), 0);
						ImGui::SliderInt(xorstr("Miss Chance"), &g_Config.SilentAim->MissChance, 0, 100, xorstr("%dx"), 0);
						ImGui::SliderInt(xorstr("Max Distance"), &g_Config.SilentAim->MaxDistance, 0, 1000, xorstr("%dm"), 0);
						static float SilentAimFovCol[4] = { g_Config.SilentAim->FovColor.Value.x, g_Config.SilentAim->FovColor.Value.y, g_Config.SilentAim->FovColor.Value.z,g_Config.SilentAim->FovColor.Value.w };
						if (ImGui::ColorEdit4(xorstr("SilentAim Fov"), SilentAimFovCol, ImGuiColorEditFlags_AlphaBar))
						{
							g_Config.SilentAim->FovColor.Value.x = SilentAimFovCol[0];
							g_Config.SilentAim->FovColor.Value.y = SilentAimFovCol[1];
							g_Config.SilentAim->FovColor.Value.z = SilentAimFovCol[2];
							g_Config.SilentAim->FovColor.Value.w = SilentAimFovCol[3];
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				break;
			case 2: //Trigger
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Trigger"), ImVec2(340, 400), false, 0);
					{
						Custom::CheckBox(xorstr("Enable"), &g_Config.TriggerBot->Enabled);
						Custom::CheckBox(xorstr("Smart Triggerbot"), &g_Config.TriggerBot->SmartTrigger);
						Custom::CheckBox(xorstr("Only Visible"), &g_Config.TriggerBot->OnlyVisible);
						Custom::CheckBox(xorstr("Ignore NPCs"), &g_Config.TriggerBot->IgnoreNPCs);
						if (!g_Config.TriggerBot->SmartTrigger) {
							Custom::CheckBox(xorstr("Show Fov"), &g_Config.TriggerBot->ShowFov);
						}
						else
						{
							g_Config.TriggerBot->ShowFov = false;
						}
						static int KeyMode = 1;
						ImGui::Keybind(xorstr("Bind"), &g_Config.TriggerBot->KeyBind, &KeyMode);
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Settings"), ImVec2(340, 400), false, 0);
					{

						if (!g_Config.TriggerBot->SmartTrigger) {
							ImGui::SliderInt(xorstr("Field of View"), &g_Config.TriggerBot->FOV, 0, 400, xorstr("%d"), 0);
							ImGui::SliderInt(xorstr("Max Distance"), &g_Config.TriggerBot->MaxDistance, 0, 1000, xorstr("%dm"), 0);
						}

						ImGui::SliderInt(xorstr("Delay"), &g_Config.TriggerBot->Delay, 0, 10, xorstr("%d"), 0);


						static float TriggerFovCol[4] = { g_Config.TriggerBot->FovColor.Value.x, g_Config.TriggerBot->FovColor.Value.y, g_Config.TriggerBot->FovColor.Value.z,g_Config.TriggerBot->FovColor.Value.w };
						if (ImGui::ColorEdit4(xorstr("Triggerbot Fov"), TriggerFovCol, ImGuiColorEditFlags_AlphaBar))
						{
							g_Config.TriggerBot->FovColor.Value.x = TriggerFovCol[0];
							g_Config.TriggerBot->FovColor.Value.y = TriggerFovCol[1];
							g_Config.TriggerBot->FovColor.Value.z = TriggerFovCol[2];
							g_Config.TriggerBot->FovColor.Value.w = TriggerFovCol[3];
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				break;
			case 3: //Auto Peek
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Auto Peek"), ImVec2(340, 400), false, 0);
					{
						static int KeyMode = 1;
						Custom::CheckBox(xorstr("Enable"), &g_Config.Player->AutoPeekEnabled);
						ImGui::Keybind(xorstr("Peek Key"), &g_Config.Player->AutoPeekKey, &KeyMode);
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Info"), ImVec2(340, 400), false, 0);
					{
						ImGui::TextColored(ImVec4(0.78f, 0.63f, 1.0f, 1.0f), xorstr("How to use:"));
						ImGui::TextWrapped(xorstr("Press the key once to save your position. Peek around corners, then press again to teleport back."));
						ImGui::Spacing();
						ImGui::TextColored(ImVec4(0.78f, 0.63f, 1.0f, 1.0f), xorstr("Status:"));
						if (Core::State::IsPeeking)
							ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), xorstr("PEEKING - Press key to return"));
						else
							ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), xorstr("Ready"));
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