#pragma once
#include <Includes/Includes.hpp>
#include <Core/Offsets.hpp>
#include <Core/SDK/SDK.hpp>

extern const char* keys[];

namespace Core
{
	namespace Features
	{
		inline ImColor GetAdminMultiColor(float timeOffset = 0.0f, float alpha = 1.0f)
		{
			float t = fmodf((float)ImGui::GetTime() * 1.8f + timeOffset, 1.0f);
			if (t < 0.0f) t += 1.0f;
			float r, g, b;
			ImGui::ColorConvertHSVtoRGB(t, 0.95f, 1.0f, r, g, b);
			return ImColor(r, g, b, alpha);
		}

		class cEsp
		{
		public:
			void Draw();
			void DrawVehicle();
		};

		inline cEsp g_Esp;

	}
}