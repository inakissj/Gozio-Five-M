#pragma once
#include <Includes/includes.hpp>
#include <windows.h>
#include <iostream>
#include <thread>
#include <Includes/CustomWidgets/Preview.hpp>

#include <Core/Core.hpp>

using namespace std;

namespace Visuals {

    static int iSubTabCount = 0;
    static float SubTabAlpha = 0.f;
    static int iSubTab = 0;

    void Render() {

        Custom::SetCenteredSubTabs({ xorstr("Players"), xorstr("Vehicles"), xorstr("Others") });
        if (Custom::SubTab(xorstr("Players"), 0 == iSubTabCount)) {
            iSubTabCount = 0;
        }
        ImGui::SameLine();
        if (Custom::SubTab(xorstr("Vehicles"), 1 == iSubTabCount)) {
            iSubTabCount = 1;
        }
        ImGui::SameLine();
        if (Custom::SubTab(xorstr("Others"), 2 == iSubTabCount)) {
            iSubTabCount = 2;
        }


        SubTabAlpha = ImClamp(SubTabAlpha + (5.f * ImGui::GetIO().DeltaTime * (iSubTabCount == iSubTab ? 1.f : -1.f)), 0.f, 1.f);

        if (SubTabAlpha == 0.f) iSubTab = iSubTabCount;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, SubTabAlpha * ImGui::GetStyle().Alpha);

        ImGui::SetCursorPos(ImVec2(226, 76));
        ImGui::BeginGroup();
        {
            switch (iSubTab)
            {
            case 0: // Players
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Globals#ESP"), ImVec2(340, 400), false, 0);
                    {
                        Custom::CheckBox(xorstr("Enable"), &g_Config.ESP->Enabled);
                        Custom::CheckBox(xorstr("Admin ESP"), &g_Config.ESP->AdminESP);
                        Custom::CheckBox(xorstr("Admin Multi-Color"), &g_Config.ESP->AdminRainbow);
                        if (Custom::CheckBox(xorstr("Box"), &g_Config.ESP->Box)) {}

                        if (Custom::CheckBox(xorstr("Skeleton"), &g_Config.ESP->Skeleton))
                        {
                            Core::SDK::Pointers::pLocalPlayer->RemoveKinematics();
                        }

                        Custom::CheckBox(xorstr("SnapLines"), &g_Config.ESP->SnapLines);
                        Custom::CheckBox(xorstr("Head Circle"), &g_Config.ESP->HeadCircle);
                        Custom::CheckBox(xorstr("Weapon Names"), &g_Config.ESP->WeaponName);
                        Custom::CheckBox(xorstr("Player Names"), &g_Config.ESP->UserNames);
                        Custom::CheckBox(xorstr("Distance"), &g_Config.ESP->DistanceFromMe);
                        Custom::CheckBox(xorstr("Health Bar"), &g_Config.ESP->HealthBar);
                        Custom::CheckBox(xorstr("Armor Bar"), &g_Config.ESP->ArmorBar);
                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Preview"), ImVec2(340, 400), false, ImGuiWindowFlags_NoScrollbar);
                    {
                        Custom::g_EspPreview.DragDropHandler();
                        Custom::g_EspPreview.Draw();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                break;

            case 1: // Vehicles
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Globals"), ImVec2(340, 400), false, 0);
                    {
                        Custom::CheckBox(xorstr("Toggle"), &g_Config.VehicleESP->Enabled);
                        Custom::CheckBox(xorstr("Vehicle Names"), &g_Config.VehicleESP->VehName);
                        Custom::CheckBox(xorstr("Locked/Unlocked"), &g_Config.VehicleESP->ShowLockUnlock);
                        Custom::CheckBox(xorstr("SnapLines"), &g_Config.VehicleESP->SnapLines);
                        Custom::CheckBox(xorstr("Distance"), &g_Config.VehicleESP->DistanceFromMe);
                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Settings"), ImVec2(340, 400), false, 0);
                    {
                        ImGui::SliderInt(xorstr("Max Distance"), &g_Config.VehicleESP->MaxDistance, 0, 1000, xorstr("%dm"), 0);
                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                break;

            case 2: // Settings
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Visual Settings"), ImVec2(340, 400), false, 0);
                    {

                        Custom::CheckBox(xorstr("Show LocalPlayer"), &g_Config.ESP->ShowLocalPlayer);
                        Custom::CheckBox(xorstr("Ignore Dead"), &g_Config.ESP->IgnoreDead);
                        Custom::CheckBox(xorstr("Ignore NPCs"), &g_Config.ESP->IgnoreNPCs);

                        Custom::CheckBoxCfg(xorstr("Friend Marker"), &g_Config.ESP->FriendsMarker, []
                            {
                                static int KeyMode = 1;
                                ImGui::Keybind(xorstr("Bind"), &g_Config.ESP->FriendsMarkerBind, &KeyMode);
                            });

                        ImGui::SliderInt(xorstr("Max Distance"), &g_Config.ESP->MaxDistance, 0, 1000, xorstr("%dm"), 0);

                        Custom::CheckBox(xorstr("Web Map Radar"), &g_Config.Web->Enabled);
                        if (Custom::Button(xorstr("Open GTA Map Radar (Web)"), ImVec2(-1, 30), 0))
                        {
                            if (!Core::Web::g_WebServer.IsRunning())
                                Core::Web::g_WebServer.Start(g_Config.Web->Port);
                            std::string url = xorstr("http://localhost:") + std::to_string(Core::Web::g_WebServer.GetPort());
                            ShellExecuteA(NULL, xorstr("open"), url.c_str(), NULL, NULL, SW_SHOWNORMAL);
                        }

                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild(xorstr("Colors"), ImVec2(340, 400), false, 0);
                    {
                        static float admin_col[4] = { g_Config.ESP->AdminColor.Value.x, g_Config.ESP->AdminColor.Value.y, g_Config.ESP->AdminColor.Value.z, g_Config.ESP->AdminColor.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Admin Label"), admin_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->AdminColor.Value.x = admin_col[0];
                            g_Config.ESP->AdminColor.Value.y = admin_col[1];
                            g_Config.ESP->AdminColor.Value.z = admin_col[2];
                            g_Config.ESP->AdminColor.Value.w = admin_col[3];
                        }

                        static float admin_box_col[4] = { g_Config.ESP->AdminBoxCol.Value.x, g_Config.ESP->AdminBoxCol.Value.y, g_Config.ESP->AdminBoxCol.Value.z, g_Config.ESP->AdminBoxCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Admin Box"), admin_box_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->AdminBoxCol.Value.x = admin_box_col[0];
                            g_Config.ESP->AdminBoxCol.Value.y = admin_box_col[1];
                            g_Config.ESP->AdminBoxCol.Value.z = admin_box_col[2];
                            g_Config.ESP->AdminBoxCol.Value.w = admin_box_col[3];
                        }

                        static float admin_skel_col[4] = { g_Config.ESP->AdminSkeletonCol.Value.x, g_Config.ESP->AdminSkeletonCol.Value.y, g_Config.ESP->AdminSkeletonCol.Value.z, g_Config.ESP->AdminSkeletonCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Admin Skeleton"), admin_skel_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->AdminSkeletonCol.Value.x = admin_skel_col[0];
                            g_Config.ESP->AdminSkeletonCol.Value.y = admin_skel_col[1];
                            g_Config.ESP->AdminSkeletonCol.Value.z = admin_skel_col[2];
                            g_Config.ESP->AdminSkeletonCol.Value.w = admin_skel_col[3];
                        }
                        static float box_col[4] = { g_Config.ESP->BoxCol.Value.x, g_Config.ESP->BoxCol.Value.y, g_Config.ESP->BoxCol.Value.z,g_Config.ESP->BoxCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Box"), box_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->BoxCol.Value.x = box_col[0];
                            g_Config.ESP->BoxCol.Value.y = box_col[1];
                            g_Config.ESP->BoxCol.Value.z = box_col[2];
                            g_Config.ESP->BoxCol.Value.w = box_col[3];
                        }

                        static float skeleton_col[4] = { g_Config.ESP->SkeletonCol.Value.x, g_Config.ESP->SkeletonCol.Value.y, g_Config.ESP->SkeletonCol.Value.z,g_Config.ESP->SkeletonCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Skeleton"), skeleton_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->SkeletonCol.Value.x = skeleton_col[0];
                            g_Config.ESP->SkeletonCol.Value.y = skeleton_col[1];
                            g_Config.ESP->SkeletonCol.Value.z = skeleton_col[2];
                            g_Config.ESP->SkeletonCol.Value.w = skeleton_col[3];
                        }

                        static float lines_col[4] = { g_Config.ESP->SnapLinesCol.Value.x, g_Config.ESP->SnapLinesCol.Value.y, g_Config.ESP->SnapLinesCol.Value.z,g_Config.ESP->SnapLinesCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("SnapLines"), lines_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->SnapLinesCol.Value.x = lines_col[0];
                            g_Config.ESP->SnapLinesCol.Value.y = lines_col[1];
                            g_Config.ESP->SnapLinesCol.Value.z = lines_col[2];
                            g_Config.ESP->SnapLinesCol.Value.w = lines_col[3];
                        }

                        static float names_col[4] = { g_Config.ESP->UserNamesCol.Value.x, g_Config.ESP->UserNamesCol.Value.y, g_Config.ESP->UserNamesCol.Value.z,g_Config.ESP->UserNamesCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Names"), names_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->UserNamesCol.Value.x = names_col[0];
                            g_Config.ESP->UserNamesCol.Value.y = names_col[1];
                            g_Config.ESP->UserNamesCol.Value.z = names_col[2];
                            g_Config.ESP->UserNamesCol.Value.w = names_col[3];
                        }

                        static float weapon_names_col[4] = { g_Config.ESP->WeaponNameCol.Value.x, g_Config.ESP->WeaponNameCol.Value.y, g_Config.ESP->WeaponNameCol.Value.z,g_Config.ESP->WeaponNameCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Weapon Names"), weapon_names_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->WeaponNameCol.Value.x = weapon_names_col[0];
                            g_Config.ESP->WeaponNameCol.Value.y = weapon_names_col[1];
                            g_Config.ESP->WeaponNameCol.Value.z = weapon_names_col[2];
                            g_Config.ESP->WeaponNameCol.Value.w = weapon_names_col[3];
                        }

                        static float dist_col[4] = { g_Config.ESP->DistanceCol.Value.x, g_Config.ESP->DistanceCol.Value.y, g_Config.ESP->DistanceCol.Value.z,g_Config.ESP->DistanceCol.Value.w };
                        if (ImGui::ColorEdit4(xorstr("Distance"), dist_col, ImGuiColorEditFlags_AlphaBar))
                        {
                            g_Config.ESP->DistanceCol.Value.x = dist_col[0];
                            g_Config.ESP->DistanceCol.Value.y = dist_col[1];
                            g_Config.ESP->DistanceCol.Value.z = dist_col[2];
                            g_Config.ESP->DistanceCol.Value.w = dist_col[3];
                        }

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