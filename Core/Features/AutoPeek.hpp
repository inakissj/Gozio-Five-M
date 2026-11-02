#pragma once
#include <Includes/Includes.hpp>
#include <Core/Offsets.hpp>
#include <Core/SDK/SDK.hpp>

namespace Core
{
	namespace State
	{
		inline bool IsPeeking = false;
		inline D3DXVECTOR3 SafePosition = { 0, 0, 0 };
	}

	namespace Features
	{
		inline void AddEllipse(ImDrawList* drawList, const ImVec2& center, float radius_x, float radius_y, ImU32 color, bool closed, int num_segments = 32, float thickness = 1.0f)
		{
			ImVec2* points = new ImVec2[num_segments];
			float step = 2.0f * IM_PI / num_segments;
			for (int i = 0; i < num_segments; i++)
			{
				float angle = i * step;
				points[i] = ImVec2(center.x + cos(angle) * radius_x, center.y + sin(angle) * radius_y);
			}
			drawList->AddPolyline(points, num_segments, color, closed, thickness);
			delete[] points;
		}

		class cAutoPeek
		{
		public:
			void CheckAutoPeek()
			{
				if (!g_Config.Player->AutoPeekEnabled || g_Config.Player->AutoPeekKey == 0)
					return;

				CPed* pLocalPlayer = Core::SDK::Pointers::pLocalPlayer;
				if (!pLocalPlayer)
					return;

				static bool wasKeyPressed = false;
				bool isKeyPressed = (GetAsyncKeyState(g_Config.Player->AutoPeekKey) & 0x8000) != 0;

				if (isKeyPressed && !wasKeyPressed)
				{
					if (!Core::State::IsPeeking)
					{
						Core::State::SafePosition = pLocalPlayer->GetPos();
						Core::State::IsPeeking = true;
					}
					else
					{
						pLocalPlayer->FreezePed(true);
						pLocalPlayer->SetPos(Core::State::SafePosition);
						std::thread([]() {
							std::this_thread::sleep_for(std::chrono::milliseconds(50));
							if (Core::SDK::Pointers::pLocalPlayer) {
								Core::SDK::Pointers::pLocalPlayer->FreezePed(false);
							}
						}).detach();
						Core::State::IsPeeking = false;
					}
				}

				wasKeyPressed = isKeyPressed;
			}

			void DrawAutoPeekMarker()
			{
				if (!Core::State::IsPeeking)
					return;

				auto DrawList = ImGui::GetBackgroundDrawList();

				D3DXVECTOR3 feetPos3D = Core::State::SafePosition;
				feetPos3D.z -= 1.0f;

				D3DXVECTOR2 feetPos2D = Core::SDK::Game::WorldToScreen(feetPos3D);

				if (Core::SDK::Game::IsOnScreen(feetPos2D))
				{
					ImVec2 drawPos = ImVec2(feetPos2D.x, feetPos2D.y);

					float time = ImGui::GetTime();
					float pulseSpeed = 0.75f;
					int numWaves = 3;
					float waveInterval = 0.5f;

					float maxRadiusX = 70.0f;
					float maxRadiusY = 35.0f;

					for (int i = 0; i < numWaves; ++i)
					{
						float waveProgress = fmod(time * pulseSpeed + (i * waveInterval), 1.0f);

						float currentRadiusX = waveProgress * maxRadiusX;
						float currentRadiusY = waveProgress * maxRadiusY;

						float currentAlpha = 1.0f - waveProgress;

						ImColor waveColor = ImColor(200, 160, 255, (int)(currentAlpha * 255));

						AddEllipse(DrawList, drawPos, currentRadiusX, currentRadiusY, waveColor, true, 32, 2.0f);
					}
				}
			}
		};

		inline cAutoPeek g_AutoPeek;
	}
}
