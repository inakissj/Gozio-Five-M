#pragma once
#include <vector>
#include <algorithm>
#include <cmath>

namespace Custom
{
	class PedModel3D
	{
	public:
		struct Vtx { float x, y, z; };
		struct Tri { int a, b, c; ImU32 col; };

		bool Initialized = false;
		float RotY = 0.f;
		float RotX = 0.f;
		bool Dragging = false;
		ImVec2 LastMouse = { 0, 0 };

		std::vector<Vtx> Verts;
		std::vector<Tri> Tris;

		void Init()
		{
			if (Initialized) return;
			Verts.clear();
			Tris.clear();
			BuildPed();
			Initialized = true;
		}

		void HandleInput(ImVec2 areaPos, ImVec2 areaSize)
		{
			ImVec2 mp = ImGui::GetMousePos();
			bool hover = mp.x >= areaPos.x && mp.x <= areaPos.x + areaSize.x &&
				mp.y >= areaPos.y && mp.y <= areaPos.y + areaSize.y;

			if (hover && ImGui::IsMouseClicked(0) && !ImGui::GetDragDropPayload())
			{
				Dragging = true;
				LastMouse = mp;
			}
			if (!ImGui::IsMouseDown(0)) Dragging = false;

			if (Dragging)
			{
				float dx = mp.x - LastMouse.x;
				float dy = mp.y - LastMouse.y;
				RotY += dx * 0.01f;
				RotX += dy * 0.006f;
				if (RotX > 0.5f) RotX = 0.5f;
				if (RotX < -0.5f) RotX = -0.5f;
				LastMouse = mp;
			}
		}

		void Draw(ImDrawList* dl, ImVec2 center, float scale)
		{
			Init();

			float guiAlpha = ImGui::GetStyle().Alpha;
			if (guiAlpha <= 0.001f) return;

			float cosY = cosf(RotY), sinY = sinf(RotY);
			float cosX = cosf(RotX), sinX = sinf(RotX);

			struct ProjTri { ImVec2 a, b, c; float z; ImU32 col; };
			std::vector<ProjTri> sorted;
			sorted.reserve(Tris.size());

			for (auto& t : Tris)
			{
				auto pa = Project(Verts[t.a], cosY, sinY, cosX, sinX, center, scale);
				auto pb = Project(Verts[t.b], cosY, sinY, cosX, sinX, center, scale);
				auto pc = Project(Verts[t.c], cosY, sinY, cosX, sinX, center, scale);

				float za = DepthOf(Verts[t.a], cosY, sinY, cosX, sinX);
				float zb = DepthOf(Verts[t.b], cosY, sinY, cosX, sinX);
				float zc = DepthOf(Verts[t.c], cosY, sinY, cosX, sinX);
				float avgZ = (za + zb + zc) / 3.f;

				float nx = (pb.y - pa.y) * (pc.x - pa.x) - (pb.x - pa.x) * (pc.y - pa.y);
				if (nx > 0) continue;

				float light = 0.55f + 0.45f * ImClamp(-avgZ * 0.3f + 0.5f, 0.f, 1.f);

				ImU32 baseCol = t.col;
				int r = (int)(((baseCol >> 0) & 0xFF) * light);
				int g = (int)(((baseCol >> 8) & 0xFF) * light);
				int b = (int)(((baseCol >> 16) & 0xFF) * light);
				int a = (int)((float)((baseCol >> 24) & 0xFF) * guiAlpha); if (a <= 0) continue; ImU32 litCol = IM_COL32(r, g, b, a);

				sorted.push_back({ pa, pb, pc, avgZ, litCol });
			}

			std::sort(sorted.begin(), sorted.end(), [](const ProjTri& a, const ProjTri& b) {
				return a.z > b.z;
			});

			for (auto& t : sorted)
			{
				dl->AddTriangleFilled(t.a, t.b, t.c, t.col);
			}
		}

	private:
		ImVec2 Project(const Vtx& v, float cosY, float sinY, float cosX, float sinX, ImVec2 center, float scale)
		{
			float x1 = v.x * cosY + v.z * sinY;
			float z1 = -v.x * sinY + v.z * cosY;
			float y1 = v.y * cosX - z1 * sinX;
			float z2 = v.y * sinX + z1 * cosX;

			float d = 3.5f;
			float p = d / (d - z2);
			return ImVec2(center.x + x1 * scale * p, center.y - y1 * scale * p);
		}

		float DepthOf(const Vtx& v, float cosY, float sinY, float cosX, float sinX)
		{
			float z1 = -v.x * sinY + v.z * cosY;
			return v.y * sinX + z1 * cosX;
		}

		void AddCylinder(float x0, float y0, float z0, float x1, float y1, float z1,
			float rTop, float rBot, int seg, ImU32 col)
		{
			int base = (int)Verts.size();
			float dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;
			float len = sqrtf(dx * dx + dy * dy + dz * dz);
			if (len < 0.0001f) return;

			float ax = dx / len, ay = dy / len, az = dz / len;

			float px, py, pz;
			if (fabsf(ay) < 0.9f) { px = 0; py = 1; pz = 0; }
			else { px = 1; py = 0; pz = 0; }

			float bx = ay * pz - az * py;
			float by = az * px - ax * pz;
			float bz = ax * py - ay * px;
			float bl = sqrtf(bx * bx + by * by + bz * bz);
			bx /= bl; by /= bl; bz /= bl;

			float cx = ay * bz - az * by;
			float cy = az * bx - ax * bz;
			float cz = ax * by - ay * bx;

			for (int i = 0; i < seg; i++)
			{
				float a = (float)i / seg * 6.28318f;
				float cs = cosf(a), sn = sinf(a);

				Verts.push_back({ x0 + (bx * cs + cx * sn) * rTop,
								  y0 + (by * cs + cy * sn) * rTop,
								  z0 + (bz * cs + cz * sn) * rTop });

				Verts.push_back({ x1 + (bx * cs + cx * sn) * rBot,
								  y1 + (by * cs + cy * sn) * rBot,
								  z1 + (bz * cs + cz * sn) * rBot });
			}

			for (int i = 0; i < seg; i++)
			{
				int n = (i + 1) % seg;
				int t0 = base + i * 2, t1 = base + i * 2 + 1;
				int n0 = base + n * 2, n1 = base + n * 2 + 1;
				Tris.push_back({ t0, n0, t1, col });
				Tris.push_back({ t1, n0, n1, col });
			}

			int topC = (int)Verts.size();
			Verts.push_back({ x0, y0, z0 });
			for (int i = 0; i < seg; i++)
			{
				int n = (i + 1) % seg;
				Tris.push_back({ topC, base + n * 2, base + i * 2, col });
			}

			int botC = (int)Verts.size();
			Verts.push_back({ x1, y1, z1 });
			for (int i = 0; i < seg; i++)
			{
				int n = (i + 1) % seg;
				Tris.push_back({ botC, base + i * 2 + 1, base + n * 2 + 1, col });
			}
		}

		void AddSphere(float cx, float cy, float cz, float r, int rings, int seg, ImU32 col,
			float sx = 1.f, float sy = 1.f, float sz = 1.f)
		{
			int base = (int)Verts.size();

			for (int j = 0; j <= rings; j++)
			{
				float phi = 3.14159f * j / rings;
				float sp = sinf(phi), cp = cosf(phi);
				for (int i = 0; i <= seg; i++)
				{
					float th = 6.28318f * i / seg;
					float st = sinf(th), ct = cosf(th);
					Verts.push_back({
						cx + r * sp * ct * sx,
						cy + r * cp * sy,
						cz + r * sp * st * sz
					});
				}
			}

			for (int j = 0; j < rings; j++)
			{
				for (int i = 0; i < seg; i++)
				{
					int a = base + j * (seg + 1) + i;
					int b = a + seg + 1;
					Tris.push_back({ a, b, a + 1, col });
					Tris.push_back({ a + 1, b, b + 1, col });
				}
			}
		}

		void AddBox(float w, float h, float d, float px, float py, float pz, ImU32 col)
		{
			int base = (int)Verts.size();
			float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;

			Verts.push_back({ px - hw, py - hh, pz - hd }); // 0
			Verts.push_back({ px + hw, py - hh, pz - hd }); // 1
			Verts.push_back({ px + hw, py + hh, pz - hd }); // 2
			Verts.push_back({ px - hw, py + hh, pz - hd }); // 3
			Verts.push_back({ px - hw, py - hh, pz + hd }); // 4
			Verts.push_back({ px + hw, py - hh, pz + hd }); // 5
			Verts.push_back({ px + hw, py + hh, pz + hd }); // 6
			Verts.push_back({ px - hw, py + hh, pz + hd }); // 7

			int f[][4] = { {0,1,2,3},{5,4,7,6},{4,0,3,7},{1,5,6,2},{4,5,1,0},{3,2,6,7} };
			for (auto& q : f)
			{
				Tris.push_back({ base + q[0], base + q[1], base + q[2], col });
				Tris.push_back({ base + q[0], base + q[2], base + q[3], col });
			}
		}

		void BuildPed()
		{
			ImU32 skin = IM_COL32(212, 165, 116, 255);
			ImU32 skinD = IM_COL32(196, 148, 100, 255);
			ImU32 shirt = IM_COL32(42, 42, 42, 255);
			ImU32 pants = IM_COL32(30, 48, 80, 255);
			ImU32 pantsD = IM_COL32(22, 36, 64, 255);
			ImU32 hair = IM_COL32(26, 16, 8, 255);
			ImU32 shoe = IM_COL32(26, 26, 26, 255);
			ImU32 sole = IM_COL32(51, 51, 51, 255);
			ImU32 belt = IM_COL32(42, 32, 21, 255);
			ImU32 buckle = IM_COL32(136, 128, 112, 255);
			ImU32 eyeW = IM_COL32(248, 248, 248, 255);
			ImU32 eyeI = IM_COL32(58, 37, 16, 255);
			ImU32 lip = IM_COL32(184, 112, 96, 255);

			int S = 8;


			// Head
			AddSphere(0, 0.69f, 0, 0.088f, 8, 10, skin, 1.f, 1.15f, 0.92f);
			// Hair cap
			AddSphere(0, 0.74f, -0.01f, 0.092f, 6, 10, hair, 1.05f, 0.65f, 0.95f);
			// Ears
			AddSphere(-0.088f, 0.695f, 0, 0.016f, 4, 6, skin, 0.5f, 0.9f, 0.7f);
			AddSphere(0.088f, 0.695f, 0, 0.016f, 4, 6, skin, 0.5f, 0.9f, 0.7f);
			// Nose
			AddSphere(0, 0.672f, 0.085f, 0.014f, 4, 6, skin, 0.8f, 0.9f, 0.7f);
			// Eyes
			AddSphere(-0.032f, 0.705f, 0.074f, 0.012f, 4, 6, eyeW, 1.f, 0.75f, 0.5f);
			AddSphere(0.032f, 0.705f, 0.074f, 0.012f, 4, 6, eyeW, 1.f, 0.75f, 0.5f);
			AddSphere(-0.032f, 0.705f, 0.082f, 0.006f, 3, 5, eyeI);
			AddSphere(0.032f, 0.705f, 0.082f, 0.006f, 3, 5, eyeI);
			// Eyebrows
			AddBox(0.03f, 0.006f, 0.01f, -0.032f, 0.722f, 0.078f, hair);
			AddBox(0.03f, 0.006f, 0.01f, 0.032f, 0.722f, 0.078f, hair);
			// Mouth
			AddBox(0.028f, 0.005f, 0.008f, 0, 0.66f, 0.082f, lip);

			// Neck
			AddCylinder(0, 0.635f, 0, 0, 0.59f, 0, 0.034f, 0.04f, S, skin);

			// Torso
			AddCylinder(0, 0.59f, 0, 0, 0.35f, 0, 0.14f, 0.12f, S, shirt);
			AddCylinder(0, 0.35f, 0, 0, 0.06f, 0, 0.12f, 0.115f, S, pants);
			// Collar
			AddCylinder(0, 0.59f, 0, 0, 0.575f, 0, 0.055f, 0.065f, S, skin);

			// Belt
			AddCylinder(0, 0.06f, 0, 0, 0.04f, 0, 0.115f, 0.113f, S, belt);
			AddBox(0.025f, 0.018f, 0.012f, 0, 0.05f, 0.115f, buckle);

			// Shoulders
			AddSphere(-0.175f, 0.56f, 0, 0.045f, 5, S, shirt, 1.f, 0.85f, 0.9f);
			AddSphere(0.175f, 0.56f, 0, 0.045f, 5, S, shirt, 1.f, 0.85f, 0.9f);

			// Upper arms (shirt)
			AddCylinder(-0.19f, 0.54f, 0, -0.28f, 0.33f, 0, 0.04f, 0.035f, S, shirt);
			AddCylinder(0.19f, 0.54f, 0, 0.28f, 0.33f, 0, 0.04f, 0.035f, S, shirt);

			// Elbows
			AddSphere(-0.29f, 0.31f, 0, 0.033f, 4, S, skin);
			AddSphere(0.29f, 0.31f, 0, 0.033f, 4, S, skin);

			// Forearms (skin)
			AddCylinder(-0.29f, 0.31f, 0, -0.26f, 0.08f, 0, 0.032f, 0.024f, S, skin);
			AddCylinder(0.29f, 0.31f, 0, 0.26f, 0.08f, 0, 0.032f, 0.024f, S, skin);

			// Wrists
			AddSphere(-0.26f, 0.06f, 0, 0.023f, 4, 6, skin);
			AddSphere(0.26f, 0.06f, 0, 0.023f, 4, 6, skin);

			// Hands
			AddBox(0.038f, 0.05f, 0.02f, -0.255f, 0.03f, 0, skin);
			AddBox(0.038f, 0.05f, 0.02f, 0.255f, 0.03f, 0, skin);
			// Fingers
			float fi[] = { -0.013f, -0.004f, 0.004f, 0.013f };
			for (int i = 0; i < 4; i++)
			{
				AddBox(0.007f, 0.025f, 0.01f, -0.255f + fi[i], 0.002f, 0, skinD);
				AddBox(0.007f, 0.025f, 0.01f, 0.255f - fi[i], 0.002f, 0, skinD);
			}
			// Thumbs
			AddBox(0.01f, 0.022f, 0.012f, -0.277f, 0.035f, 0.005f, skinD);
			AddBox(0.01f, 0.022f, 0.012f, 0.277f, 0.035f, 0.005f, skinD);

			// Hips
			AddSphere(-0.065f, 0.02f, 0, 0.055f, 5, S, pants, 1.f, 0.7f, 0.85f);
			AddSphere(0.065f, 0.02f, 0, 0.055f, 5, S, pants, 1.f, 0.7f, 0.85f);

			// Upper legs
			AddCylinder(-0.08f, 0.0f, 0, -0.09f, -0.34f, 0, 0.058f, 0.046f, S, pants);
			AddCylinder(0.08f, 0.0f, 0, 0.09f, -0.34f, 0, 0.058f, 0.046f, S, pants);

			// Knees
			AddSphere(-0.09f, -0.34f, 0, 0.046f, 4, S, pants);
			AddSphere(0.09f, -0.34f, 0, 0.046f, 4, S, pants);

			// Lower legs
			AddCylinder(-0.09f, -0.34f, 0, -0.09f, -0.65f, 0, 0.044f, 0.035f, S, pants);
			AddCylinder(0.09f, -0.34f, 0, 0.09f, -0.65f, 0, 0.044f, 0.035f, S, pants);
			// Cuffs
			AddCylinder(-0.09f, -0.64f, 0, -0.09f, -0.67f, 0, 0.037f, 0.036f, S, pantsD);
			AddCylinder(0.09f, -0.64f, 0, 0.09f, -0.67f, 0, 0.037f, 0.036f, S, pantsD);

			// Ankles
			AddSphere(-0.09f, -0.68f, 0, 0.034f, 4, S, shoe);
			AddSphere(0.09f, -0.68f, 0, 0.034f, 4, S, shoe);

			// Shoes
			AddBox(0.065f, 0.04f, 0.13f, -0.09f, -0.72f, 0.015f, shoe);
			AddBox(0.065f, 0.04f, 0.13f, 0.09f, -0.72f, 0.015f, shoe);
			// Soles
			AddBox(0.068f, 0.012f, 0.138f, -0.09f, -0.742f, 0.018f, sole);
			AddBox(0.068f, 0.012f, 0.138f, 0.09f, -0.742f, 0.018f, sole);
			// Toe caps
			AddSphere(-0.09f, -0.73f, 0.075f, 0.032f, 4, 6, shoe, 1.f, 0.5f, 0.6f);
			AddSphere(0.09f, -0.73f, 0.075f, 0.032f, 4, 6, shoe, 1.f, 0.5f, 0.6f);
		}
	};

	inline PedModel3D g_PedModel;
}
