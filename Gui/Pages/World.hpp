#pragma once
#include <Includes/includes.hpp>
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;

namespace World {

	static int iSubTabCount = 0;
	static float SubTabAlpha = 0.f;
	static int iSubTab = 0;

	void Render()
	{

		// Center sub-tabs in top middle of content area
		Custom::SetCenteredSubTabs({ xorstr("Players"), xorstr("Vehicles") });
		if (Custom::SubTab(xorstr("Players"), 0 == iSubTabCount)) {
			iSubTabCount = 0;
		}
		ImGui::SameLine();
		if (Custom::SubTab(xorstr("Vehicles"), 1 == iSubTabCount)) {
			iSubTabCount = 1;
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
			case 0:
			{


				static int SeletedIndex = -1;
				bool IsSelected;


				ImGui::BeginGroup();
				{

					ImGui::BeginChild(xorstr("List"), ImVec2(340, 400), false, 0);
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 8));
						for (int i = 0; i < Core::SDK::Game::EntityList.size(); i++)
						{
							if (Core::SDK::Game::EntityList[i].Ped == Core::SDK::Pointers::pLocalPlayer)
								continue;

							if (Core::SDK::Game::EntityList[i].PedType != 2)
								continue;

							ImGui::PushID(i);
							IsSelected = SeletedIndex == i;
							std::string displayLabel = Core::SDK::Game::EntityList[i].IsAdmin
								? (xorstr("[Admin] ") + Core::SDK::Game::EntityList[i].NetworkInfo.UserName)
								: Core::SDK::Game::EntityList[i].NetworkInfo.UserName;
							if (ImGui::ListSelectable(displayLabel.c_str(), &IsSelected)) SeletedIndex = i;
							ImGui::PopID();
						}
						ImGui::PopStyleVar();
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Info"), ImVec2(340, 180), false, 0);
					{
						if (SeletedIndex == -1 || SeletedIndex >= (int)Core::SDK::Game::EntityList.size()) {
							ImGui::TextColored(ImColor(g_Col.SecundaryText), xorstr("Select Player"));
						}
						else
						{
							auto SelectedPed = Core::SDK::Game::EntityList[SeletedIndex];
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 4));

							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Name:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), SelectedPed.NetworkInfo.UserName.c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Id:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), std::to_string(SelectedPed.Id).c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Distance:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), std::to_string(SelectedPed.Distance).c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Weapon:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), SelectedPed.WeaponName.c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Friend:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), SelectedPed.IsFriend ? xorstr("True") : xorstr("False"));
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Admin:")); ImGui::SameLine();
							ImGui::TextColored(SelectedPed.IsAdmin ? ImColor(255, 65, 85, 255) : ImColor(g_Col.SecundaryText), SelectedPed.IsAdmin ? xorstr("Admin") : xorstr("False"));

							ImGui::PopStyleVar();
						}
					}
					ImGui::EndChild();
					ImGui::BeginChild(xorstr("Actions"), ImVec2(340, 210), false, 0);
					{
						if (SeletedIndex == -1 || SeletedIndex >= (int)Core::SDK::Game::EntityList.size()) {
							ImGui::TextColored(ImColor(g_Col.SecundaryText), xorstr("Select Player"));
						}
						else
						{
							auto& SelectedPed = Core::SDK::Game::EntityList[SeletedIndex];

							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 8));
							bool Friend = SelectedPed.IsFriend;
							if (Custom::CheckBox(xorstr("Friend"), &Friend))
							{
								Core::SDK::Game::FriendMap[SelectedPed.Ped] = Friend;
							}

							bool AdminFlag = SelectedPed.IsAdmin;
							if (Custom::CheckBox(xorstr("Mark as Admin"), &AdminFlag))
							{
								Core::SDK::Game::AdminMap[SelectedPed.Ped] = AdminFlag;
								SelectedPed.IsAdmin = AdminFlag;
							}

							ImGui::Spacing();

							if (Custom::Button(xorstr("Teleport"), ImVec2(-1, 30), 0))
							{
								Core::SDK::Pointers::pLocalPlayer->SetPos(SelectedPed.Pos);
							}


							/*if (Custom::Button(xorstr("Copy Discord"), ImVec2(-1, 30), 0))
							{
								std::string PlayerDiscord = SelectedPed.NetworkInfo.DiscordId;
								if ( PlayerDiscord.empty( ) )
								{
									std::thread( [ ] ( ) { NotifyManager::Send( xorstr( "DiscordId not found." ), 4000 ); } ).detach( );
								}
								else
								{
									std::thread( [ ] ( ) { NotifyManager::Send( xorstr( "DiscordId copied to clipboard!" ), 4000 ); } ).detach( );
									Utils::PasteClipboard( PlayerDiscord.c_str( ) );
								}
							}
							if ( Custom::Button( xorstr( "Copy Steam" ), ImVec2( -1, 30 ), 0 ) )
							{
								std::string PlayerSteam = SelectedPed.NetworkInfo.SteamId;
								if ( PlayerSteam.empty( ) )
								{
									std::thread( [ ] ( ) { NotifyManager::Send( xorstr( "SteamId not found." ), 4000 ); } ).detach( );
								}
								else
								{
									std::thread( [ ] ( ) { NotifyManager::Send( xorstr( "SteamId copied to clipboard!" ), 4000 ); } ).detach( );
									Utils::PasteClipboard( PlayerSteam.c_str( ) );
								}
							}
							*/
							ImGui::PopStyleVar();
						}
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
			}
			break;
			case 1: //Vehicles
			{

				static int SeletedIndex = -1;
				bool IsSelected;


				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("List"), ImVec2(340, 400), false, 0);
					{
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 8));


						{
							std::lock_guard<std::mutex> lock(Core::Threads::g_VehicleList.vehicleListMutex); // Protege a lista durante a renderiza  o

							for (int i = 0; i < Core::SDK::Game::VehicleList.size(); i++)
							{
								IsSelected = SeletedIndex == i;
								std::string Name = Core::SDK::Game::VehicleList[i].Name + xorstr(" (") + std::to_string((int)Core::SDK::Game::VehicleList[i].Dist) + xorstr("m)");
								if (ImGui::ListSelectable(Name.c_str(), &IsSelected)) SeletedIndex = i;
							}
						}

						ImGui::PopStyleVar();
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
				{
					ImGui::BeginChild(xorstr("Info"), ImVec2(340, 140), false, 0);
					{
						if (SeletedIndex == -1) {
							ImGui::TextColored(ImColor(g_Col.SecundaryText), xorstr("Select Vehicle"));
						}
						else
						{
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 4));

							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Name:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), Core::SDK::Game::VehicleList[SeletedIndex].Name.c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Distance:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), (std::to_string((int)Core::SDK::Game::VehicleList[SeletedIndex].Dist) + xorstr("m")).c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Driver:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), Core::SDK::Game::VehicleList[SeletedIndex].Driver == 0 ? xorstr("Unknown") : Core::SDK::Game::VehicleList[SeletedIndex].Driver->GetPedType() != 2 ? xorstr("NPC") : Core::SDK::Game::GetPedName(Core::SDK::Game::VehicleList[SeletedIndex].Driver).c_str());
							ImGui::TextColored(ImColor(g_Col.FeaturesText), xorstr("Doors Locked:")); ImGui::SameLine();
							ImGui::TextColored(ImColor(g_Col.SecundaryText), Core::SDK::Game::VehicleList[SeletedIndex].IsLocked ? xorstr("Yes") : xorstr("No"));

							ImGui::PopStyleVar();
						}
					}
					ImGui::EndChild();
					ImGui::BeginChild(xorstr("Actions"), ImVec2(340, 250), false, 0);
					{
						if (SeletedIndex == -1) {
							ImGui::TextColored(ImColor(g_Col.SecundaryText), xorstr("Select Vehicle"));
						}
						else
						{
							auto Veh = Core::SDK::Game::VehicleList[SeletedIndex].Pointer;
							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 8));

							if (Core::SDK::Game::VehicleList[SeletedIndex].Pointer->IsLocked())
							{
								if (Custom::Button(xorstr("Unlock"), ImVec2(-1, 30), 0)) {
									Veh->DoorState(true);
								}
							}
							else {
								if (Custom::Button(xorstr("Lock"), ImVec2(-1, 30), 0)) {
									Veh->DoorState(false);
								}
							}

							if (Custom::Button(xorstr("Teleport"), ImVec2(-1, 30), 0)) {
								Core::SDK::Pointers::pLocalPlayer->SetPos(Veh->GetPos());
							}

							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.15f, 0.1f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.15f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.05f, 1.0f));
							if (Custom::Button(xorstr("Explode"), ImVec2(-1, 30), 0)) {
								std::thread([Veh]() {
									Core::Features::Exploits::ExplodeVehicle(Veh, true);
								}).detach();
								std::thread([]() { NotifyManager::Send(xorstr("Vehicle exploded!"), 2000); }).detach();
							}
							ImGui::PopStyleColor(3);

							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.3f, 0.1f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.4f, 0.15f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.35f, 0.05f, 1.0f));
							if (Custom::Button(xorstr("Break Wheels"), ImVec2(-1, 30), 0)) {
								std::thread([Veh]() {
									Core::Features::Exploits::BreakVehicleWheels(Veh);
								}).detach();
								std::thread([]() { NotifyManager::Send(xorstr("Wheels broken!"), 2000); }).detach();
							}
							ImGui::PopStyleColor(3);

							ImGui::PopStyleVar();
						}
					}
					ImGui::EndChild();

				}
				ImGui::EndGroup();
			}
			break;
			default:
				break;
			}

		}
		ImGui::EndGroup();
		ImGui::PopStyleVar();

	}
}