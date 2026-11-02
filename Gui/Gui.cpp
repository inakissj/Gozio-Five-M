#include "Gui.hpp"

#include <Includes/CustomWidgets/Custom.hpp>
#include <Includes/CustomWidgets/WaterMarks.hpp>
#include <Includes/CustomWidgets/Notify.hpp>

#include <Core/Features/AutoPeek.hpp>
#include <Gui/Pages/Combat.hpp>
#include <Gui/Pages/Visuals.hpp>
#include <Gui/Pages/Local.hpp>
#include <Gui/Pages/Exploits.hpp>
#include <Core/Features/Exploits/VehicleExploits.hpp>
#include <Gui/Pages/World.hpp>
#include <Gui/Pages/Settings.hpp>
#include <Gui/Pages/Login.hpp>
#include <Core/Features/Exploits/Exploits.hpp>
#include <Includes/CustomWidgets/Notify.hpp>
#include <Core/Features/Esp.hpp>
#include <Security/Api/Auth/api/api.hh>
#include <declar/declar.hpp>

int PauseLoop;
inline std::mutex DrawMtx;


#pragma region Animation
struct Particle
{
	ImVec2 position;
	ImVec2 velocity;
	ImVec4 color;
	float radius;
};

class ParticleSystem
{
public:
	ParticleSystem(int numParticles)
	{
		setupParticles(numParticles);
	}

	void update(float deltaTime, const ImVec2& windowPos)
	{
		this->windowPos = windowPos;

		if (!g_MenuInfo.particles) return;

		for (auto& particle : particles)
		{
			particle.position.x += particle.velocity.x * deltaTime;
			particle.position.y += particle.velocity.y * deltaTime;

			// Check if the particle is out of bounds, reset its position
			if (particle.position.x < 0 || particle.position.x > ImGui::GetWindowWidth() ||
				particle.position.y < 0 || particle.position.y > ImGui::GetWindowHeight())
			{
				resetParticle(particle);
			}
		}
	}

	void draw()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImDrawList* drawList = window->DrawList;

		if (!g_MenuInfo.particles) return;

		// Draw particles
		for (const auto& particle : particles)
		{
			ImVec4 particleColor = ImVec4(accent_color[0], accent_color[1], accent_color[2], accent_color[3]);
			drawList->AddCircleFilled(
				ImVec2(particle.position.x + windowPos.x, particle.position.y + windowPos.y),
				particle.radius,
				ImGui::GetColorU32(particleColor));
		}

		// Draw lines connecting particles within a certain distance
		float maxDistance = 150.0f; // Maximum distance to draw a line
		for (size_t i = 0; i < particles.size(); ++i)
		{
			for (size_t j = i + 1; j < particles.size(); ++j)
			{
				float distance = std::sqrt(
					std::pow(particles[i].position.x - particles[j].position.x, 2) +
					std::pow(particles[i].position.y - particles[j].position.y, 2));

				if (distance < maxDistance)
				{
					// Fade line opacity based on distance
					float alpha = 0.5f - (distance / maxDistance);
					ImU32 lineColor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, alpha));
					drawList->AddLine(
						ImVec2(particles[i].position.x + windowPos.x, particles[i].position.y + windowPos.y),
						ImVec2(particles[j].position.x + windowPos.x, particles[j].position.y + windowPos.y),
						lineColor, 1.0f);
				}
			}
		}
	}

private:
	ImVec2 windowPos;
	std::vector<Particle> particles;

	void setupParticles(int numParticles)
	{
		particles.clear();

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> disPos(0, 10);
		std::uniform_real_distribution<> disVel(-50, 50);
		std::uniform_real_distribution<> disColor(0, 1);
		std::uniform_real_distribution<> disRadius(1, 3);

		for (int i = 0; i < numParticles; ++i)
		{
			Particle particle;
			particle.position = ImVec2(disPos(gen) * ImGui::GetWindowWidth(), disPos(gen) * ImGui::GetWindowHeight());
			particle.velocity = ImVec2(disVel(gen), disVel(gen));
			particle.color = ImVec4(disColor(gen), disColor(gen), disColor(gen), 0.4f);
			particle.radius = disRadius(gen);

			particles.push_back(particle);
		}
	}

	void resetParticle(Particle& particle)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> disPos(0, 1);
		std::uniform_real_distribution<> disVel(-50, 50);

		particle.position = ImVec2(disPos(gen) * ImGui::GetWindowWidth(), disPos(gen) * ImGui::GetWindowHeight());
		particle.velocity = ImVec2(disVel(gen), disVel(gen));
	}
};


#pragma endregion 


void Gui::Rendering( )
{
	//static ParticleSystem particleSystem(80);

	if ( !g_MenuInfo.IsLogged )
	{
		g_MenuInfo.MenuSize = { 500, 350 };
	}
	ImGui::SetNextWindowSize(g_MenuInfo.MenuSize);

	if (!PauseLoop) { ImGui::SetNextWindowPos(g_Variables.g_vGameWindowSize / 2 - g_MenuInfo.MenuSize / 2); PauseLoop++; }

	ImGui::Begin(" ", nullptr, ImGuiWindowFlags);
	ImGui::PushFont(g_Variables.m_FontNormal);
	{
		Custom::DrawBackground(g_MenuInfo.IsLogged);
		if (g_MenuInfo.IsLogged)
		{

			//particleSystem.update(ImGui::GetIO().DeltaTime, ImGui::GetWindowPos());
			//particleSystem.draw();

			ImGui::SetCursorPos(ImVec2(12, 77/*84*/));

			ImGui::BeginGroup();
			ImGui::PushFont(g_Variables.m_FontSecundary);
			{
				//ImGui::TextColored( ImColor( 80, 80, 80 ), std::to_string( ImGui::GetIO().Framerate ).c_str() );
				if (Custom::Tab(ICON_FA_CROSSHAIRS, xorstr("Combat"), g_MenuInfo.Combat == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.Combat;
				}
				if (Custom::Tab(ICON_FA_EYE_SLASH, xorstr("Visuals"), g_MenuInfo.Visuals == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.Visuals;
				}
				if (Custom::Tab(ICON_FA_USER, xorstr("Player"), g_MenuInfo.Local == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.Local;
				}
				if (Custom::Tab(ICON_FA_GLOBE, xorstr("World"), g_MenuInfo.World == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.World;
				}
				if (Custom::Tab(ICON_FA_LAYER_GROUP, xorstr("Exploits"), g_MenuInfo.Exploits == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.Exploits;
				}
				if (Custom::Tab(ICON_FA_FOLDER, xorstr("Settings"), g_MenuInfo.Settings == g_MenuInfo.iTabCount))
				{
					g_MenuInfo.iTabCount = g_MenuInfo.Settings;
				}

			}
			ImGui::PopFont();
			ImGui::EndGroup();
		}

		g_MenuInfo.TabAlpha = ImClamp(g_MenuInfo.TabAlpha + (5.f * ImGui::GetIO().DeltaTime * (g_MenuInfo.iTabCount == g_MenuInfo.iCurrentPage ? 1.f : -1.f)), 0.f, 1.f);

		if (g_MenuInfo.TabAlpha == 0.f)
			g_MenuInfo.iCurrentPage = g_MenuInfo.iTabCount;

		ImGuiStyle* style = &ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, g_MenuInfo.TabAlpha * style->Alpha);

		if (g_MenuInfo.IsLogged)
		{


			ImGui::SetCursorPos(ImVec2(176, 16));
			ImGui::BeginGroup();



			switch (g_MenuInfo.iCurrentPage)
			{

			case g_MenuInfo.Combat:
				Combat::Render();
				break;
			case g_MenuInfo.Visuals:
				Visuals::Render();
				break;
			case g_MenuInfo.Local:
				Local::Render();
				break;
			case g_MenuInfo.World:
				World::Render();
				break;
			case g_MenuInfo.Exploits:
				Exploits::Render();
				break;
			case g_MenuInfo.Settings:
				Settings::Render();
				break;

			}

			ImGui::EndGroup();


			HWND ActiveWindow = GetForegroundWindow();

			{
				std::lock_guard<std::mutex> Lock(DrawMtx);

				NotifyManager::Render();

				if (g_Variables.ProcIdFiveM && ActiveWindow == g_Variables.g_hGameWindow)
				{
					if (GetAsyncKeyState(g_Config.Player->GodModeKey) & 1)
					{
						g_Config.Player->EnableGodMode = !g_Config.Player->EnableGodMode;

						Core::SDK::Pointers::pLocalPlayer->SetGodMode(g_Config.Player->EnableGodMode);

						std::thread([&]()
							{
								NotifyManager::Send(xorstr("GodMode has been ") + (std::string)(g_Config.Player->EnableGodMode ? xorstr("enabled!") : xorstr("disabled!")), 2000);
							}
						).detach();

					}

					if (GetAsyncKeyState(g_Config.Player->NoClipKey) & 1)
					{
						g_Config.Player->NoClipEnabled = !g_Config.Player->NoClipEnabled;

						Core::SDK::Pointers::pLocalPlayer->FreezePed(g_Config.Player->NoClipEnabled);

						std::thread([&]()
							{
								NotifyManager::Send(xorstr("NoClip has been ") + (std::string)(g_Config.Player->NoClipEnabled ? xorstr("enabled!") : xorstr("disabled!")), 2000);
							}
						).detach();
					}

					if (g_Config.Player->NoClipEnabled)
						Features::Exploits::NoClip();

					Features::g_AutoPeek.CheckAutoPeek();

				}

				if (ActiveWindow == g_Variables.g_hGameWindow || ActiveWindow == g_Variables.g_hCheatWindow || !g_Variables.ProcIdFiveM)
				{
					struct FovFuncs_t {
						bool* Enabled;
						int* FovSize;
						ImVec4 FovColor;
					};

					std::vector<FovFuncs_t> FovDrawList = {
						FovFuncs_t(&g_Config.Aimbot->ShowFov, &g_Config.Aimbot->FOV, g_Config.Aimbot->FovColor),
						FovFuncs_t(&g_Config.SilentAim->ShowFov, &g_Config.SilentAim->FOV, g_Config.SilentAim->FovColor),
						FovFuncs_t(&g_Config.TriggerBot->ShowFov, &g_Config.TriggerBot->FOV, g_Config.TriggerBot->FovColor),
					};

					static std::vector<float> Alphas(FovDrawList.size(), 0.0f);
					static std::vector<float> Sizes(FovDrawList.size(), 0.0f);

					for (int i = 0; i < FovDrawList.size(); ++i)
					{
						auto& Fov = FovDrawList[i];

						Alphas[i] = ImClamp(ImLerp(Alphas[i], *Fov.Enabled ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 10.f), 0.f, 1.f);
						Sizes[i] = ImLerp(Sizes[i], (float)*Fov.FovSize, ImGui::GetIO().DeltaTime * 12.f);

						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Alphas[i]);

						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g_Variables.g_vGameWindowCenter.x, g_Variables.g_vGameWindowCenter.y), Sizes[i], ImGui::GetColorU32(Fov.FovColor), 999);

						ImGui::PopStyleVar();
					}

					if (g_Config.General->WaterMark)
						Custom::WaterMark::Render();

					if (g_Variables.ProcIdFiveM)
					{
						ImGui::PushFont(g_Variables.m_DrawFont);
						Features::g_Esp.Draw();
						Features::g_Esp.DrawVehicle();
						Features::g_AutoPeek.DrawAutoPeekMarker();
						ImGui::PopFont();
					}
				}
				else if (g_Variables.ProcIdFiveM) {

					if (ImGui::GetStyle().Alpha >= 0.9f)
					{
						g_MenuInfo.IsOpen = false;
						SetWindowLong(g_Variables.g_hCheatWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
					}

				}

			}
		}
		ImGui::PopStyleVar();
		ImGui::PopFont();
	}
	ImGui::End();
}