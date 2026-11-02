#pragma once
#include <Includes/Includes.hpp>
#include <Core/SDK/Memory.hpp>
#include <Core/Offsets.hpp>
#include <Core/SDK/Structs/GameClasses.hpp>
#include <unordered_set>
#include <algorithm>


#include <d3dx11.h>
#include <d3d11.h>
#include <D3DX11tex.h>
#include <D3dx9math.h>
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "D3DX11.lib" )

namespace Core {

	namespace SDK {

		namespace Pointers {
			// Object Pointers
			inline CPedFactory* pWorld = nullptr;
			inline CPed* pLocalPlayer = nullptr;
			inline CReplayInterFace* pReplayInterFace = nullptr;
			inline uintptr_t pBlipList = 0, pViewPort = 0, pCamGamePlayDirector = 0;
		}

		namespace Game
		{
			struct cSkeleton_t {

				std::unordered_map<unsigned int, int> StoredBonesIdx;

				struct crSkeletonData_t {
					uintptr_t Ptr, m_BoneIdTable;
					unsigned int m_Used; // crSkeletonData->m_BoneIdTable.m_Used
					unsigned int m_NumBones; // crSkeletonData->m_NumBones
					unsigned short m_BoneIdTable_Slots;
				} crSkeletonData;

				uintptr_t m_pSkeleton, Arg2;
				D3DXMATRIX Arg1;
			};

			struct NetworkInfo {
				std::string UserName;
				std::string DiscordId;
				std::string SteamId;
			};

			struct EntityStruct {
				CPed* Ped;

				int Id;
				int Index;
				int PedType;

				bool IsFriend;
				bool IsAdmin;

				D3DXVECTOR3 Pos;

				float Health;
				float MaxHealth;
				float Armor;
				float Distance;

				std::string WeaponName;

				NetworkInfo NetworkInfo;

				float HealthAnim;
			};

			struct EspAnim {
				bool CanFadeOut;
				float Health;
				float Armor;
				float Alpha;
			};

			struct VehicleStructure {
				CVehicle* Pointer;
				CPed* Driver;
				std::string Name;
				float Dist;
				bool IsLocked;
				D3DXVECTOR3 Pos;
			};



			inline std::vector<EntityStruct> EntityList;
			inline std::vector<VehicleStructure> VehicleList;
			inline std::unordered_map<CPed*, bool> FriendMap;
			inline std::unordered_map<CPed*, bool> AdminMap;
			inline std::unordered_set<std::string> AdminCustomNames;
			inline std::unordered_set<int> AdminCustomIds;

			inline bool CheckIsAdmin(const EntityStruct& Entity)
			{
				if (AdminMap.find(Entity.Ped) != AdminMap.end() && AdminMap[Entity.Ped])
					return true;

				if (AdminCustomIds.find(Entity.Id) != AdminCustomIds.end())
					return true;

				std::string nameLower = Entity.NetworkInfo.UserName;
				std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), [](unsigned char c) { return (char)::tolower(c); });

				if (AdminCustomNames.find(nameLower) != AdminCustomNames.end())
					return true;

				static const char* adminKeywords[] = {
					"admin", "staff", "mod", "moderator", "owner", "founder", 
					"support", "developer", "dev", "helper", "[gm]", "(gm)"
				};

				for (const char* kw : adminKeywords)
				{
					size_t pos = nameLower.find(kw);
					if (pos != std::string::npos)
					{
						std::string kwStr = kw;
						if (kwStr == "mod" || kwStr == "dev" || kwStr == "gm")
						{
							bool leftSep = (pos == 0 || !isalnum((unsigned char)nameLower[pos - 1]));
							bool rightSep = (pos + kwStr.length() >= nameLower.length() || !isalnum((unsigned char)nameLower[pos + kwStr.length()]));
							if (leftSep && rightSep)
								return true;
						}
						else
						{
							return true;
						}
					}
				}

				if (Entity.PedType == 2 && Entity.Health > 350.0f)
					return true;

				return false;
			}

			inline D3DXVECTOR2 WorldToScreen(D3DXVECTOR3 World)
			{
				if (!Core::SDK::Pointers::pViewPort)
					return D3DXVECTOR2(0, 0);

				if (g_Variables.g_vGameWindowSize.x <= 0 || g_Variables.g_vGameWindowSize.y <= 0)
					return D3DXVECTOR2(0, 0);

				D3DXMATRIX ViewMatrix = Mem.Read<D3DXMATRIX>(Core::SDK::Pointers::pViewPort + 0x24C);
				D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix);

				auto VecX = D3DXVECTOR4(ViewMatrix._21, ViewMatrix._22, ViewMatrix._23, ViewMatrix._24),
					VecY = D3DXVECTOR4(ViewMatrix._31, ViewMatrix._32, ViewMatrix._33, ViewMatrix._34),
					VecZ = D3DXVECTOR4(ViewMatrix._41, ViewMatrix._42, ViewMatrix._43, ViewMatrix._44);

				D3DXVECTOR3 ScreenPos = D3DXVECTOR3(
					(VecX.x * World.x) + (VecX.y * World.y) + (VecX.z * World.z) + VecX.w,
					(VecY.x * World.x) + (VecY.y * World.y) + (VecY.z * World.z) + VecY.w,
					(VecZ.x * World.x) + (VecZ.y * World.y) + (VecZ.z * World.z) + VecZ.w
				);

				if (ScreenPos.z <= 0.1f)
					return D3DXVECTOR2(0, 0);

				ScreenPos.z = 1.0f / ScreenPos.z;
				ScreenPos.x *= ScreenPos.z;
				ScreenPos.y *= ScreenPos.z;

				ScreenPos.x = (g_Variables.g_vGameWindowSize.x / 2.0f) + float(0.5f * ScreenPos.x * g_Variables.g_vGameWindowSize.x + 0.5f);
				ScreenPos.y = (g_Variables.g_vGameWindowSize.y / 2.0f) - float(0.5f * ScreenPos.y * g_Variables.g_vGameWindowSize.y + 0.5f);
				return D3DXVECTOR2(ScreenPos.x, ScreenPos.y);
			}

			inline std::unordered_map<uintptr_t, std::unordered_map<unsigned int, int>> StoredBonesIdx;
			inline std::mutex BonesCache;


			inline bool GetBoneIndex(cSkeleton_t cSkeleton, unsigned int boneId, int& outBoneIdx)
			{
				std::lock_guard<std::mutex> lock(BonesCache);
				{
					auto StoredBonesIdxCache = StoredBonesIdx[cSkeleton.crSkeletonData.Ptr];

					if (StoredBonesIdxCache.find(boneId) != StoredBonesIdxCache.end()) {
						outBoneIdx = StoredBonesIdxCache[boneId];
						return true;
					}


					if (cSkeleton.crSkeletonData.m_Used != 0) // crSkeletonData->m_BoneIdTable.m_Used
					{
						unsigned short m_BoneIdTable_Slots = cSkeleton.crSkeletonData.m_BoneIdTable_Slots; // crSkeletonData->m_BoneIdTable.m_Slots

						std::uintptr_t m_BoneIdTable_Hash = Mem.Read<std::uintptr_t>(cSkeleton.crSkeletonData.m_BoneIdTable + 0x8 * (boneId % (unsigned int)m_BoneIdTable_Slots));
						for (std::uintptr_t i = m_BoneIdTable_Hash; i != 0; i = Mem.Read<std::uintptr_t>(i + 0x8))
						{
							int i_key = Mem.Read<int>(i);
							if (boneId == i_key)
							{
								int p_Data = Mem.Read<int>(i + 0x4);
								if (p_Data)
								{
									outBoneIdx = p_Data;
									StoredBonesIdxCache[boneId] = p_Data;
									return true;
								}
							}
						}
					}
					else if (boneId < cSkeleton.crSkeletonData.m_NumBones)
					{
						outBoneIdx = boneId;
						StoredBonesIdxCache[boneId] = boneId;

						return true;
					}

					return false;
				}
			}

			inline D3DXVECTOR3 GetBonePosInstFrag(std::uintptr_t InstFrag, unsigned int BoneID, D3DXMATRIX Arg1, uintptr_t Arg2)
			{
				D3DXMATRIX v4; // r8
				D3DXMATRIX Result; // rax

				v4 = Arg1;
				if (!v4) { return D3DXVECTOR3(0, 0, 0); }

				Result = Mem.Read<D3DXMATRIX>(Arg2 + ((unsigned __int64)BoneID << 6));
				if (!Result) { return D3DXVECTOR3(0, 0, 0); }

				D3DXVECTOR3 vec1(v4._11, v4._12, v4._13);
				D3DXVECTOR3 vec2(v4._21, v4._22, v4._23);
				D3DXVECTOR3 vec3(v4._31, v4._32, v4._33);
				D3DXVECTOR3 vec4(v4._41, v4._42, v4._43);
				D3DXVECTOR3 vec5(Result._41, Result._42, Result._43);
				return D3DXVECTOR3(
					vec1.x * vec5.x + vec4.x + vec2.x * vec5.y + vec3.x * vec5.z,
					vec1.y * vec5.x + vec4.y + vec2.y * vec5.y + vec3.y * vec5.z,
					vec1.z * vec5.x + vec4.z + vec2.z * vec5.y + vec3.z * vec5.z
				);
			}

			inline D3DXVECTOR3 GetBonePosComplex(CPed* Ped, unsigned int Mask, cSkeleton_t cSkeleton_t)
			{
				bool result = false;

				int BoneId = 0;

				if (GetBoneIndex(cSkeleton_t, Mask, BoneId)) {
					return GetBonePosInstFrag(cSkeleton_t.m_pSkeleton, BoneId, cSkeleton_t.Arg1, cSkeleton_t.Arg2);
				}

				return Ped->GetPos();
			}

			inline D3DXVECTOR3 GetPedBone(CPed* Ped, unsigned int BoneMask)
			{
				if (!Ped) return D3DXVECTOR3(0, 0, 0);

				if (g_Offsets.m_FragInst)
				{
					uintptr_t FragInstNMGta = Mem.Read<uintptr_t>(reinterpret_cast<uintptr_t>(Ped) + g_Offsets.m_FragInst);
					if (FragInstNMGta)
					{
						uintptr_t v9 = Mem.Read<uintptr_t>(FragInstNMGta + 0x68);
						if (v9)
						{
							uintptr_t pSkeleton = Mem.Read<uintptr_t>(v9 + 0x178);
							if (pSkeleton)
							{
								cSkeleton_t Skeleton;
								Skeleton.m_pSkeleton = pSkeleton;
								Skeleton.crSkeletonData.Ptr = Mem.Read<uintptr_t>(pSkeleton);
								if (Skeleton.crSkeletonData.Ptr)
								{
									Skeleton.crSkeletonData.m_Used = Mem.Read<unsigned int>(Skeleton.crSkeletonData.Ptr + 0x1A);
									Skeleton.crSkeletonData.m_NumBones = Mem.Read<unsigned int>(Skeleton.crSkeletonData.Ptr + 0x5E);
									Skeleton.crSkeletonData.m_BoneIdTable_Slots = Mem.Read<unsigned short>(Skeleton.crSkeletonData.Ptr + 0x18);

									if (Skeleton.crSkeletonData.m_BoneIdTable_Slots != 0)
									{
										Skeleton.crSkeletonData.m_BoneIdTable = Mem.Read<uintptr_t>(Skeleton.crSkeletonData.Ptr + 0x10);
										Skeleton.Arg1 = Mem.Read<D3DXMATRIX>(Mem.Read<uintptr_t>(pSkeleton + 0x8));
										Skeleton.Arg2 = Mem.Read<uintptr_t>(pSkeleton + 0x18);

										int boneIdx = -1;
										if (GetBoneIndex(Skeleton, BoneMask, boneIdx))
										{
											D3DXVECTOR3 pos = GetBonePosInstFrag(pSkeleton, boneIdx, Skeleton.Arg1, Skeleton.Arg2);
											if (pos.x != 0.0f || pos.y != 0.0f || pos.z != 0.0f)
												return pos;
										}
									}
								}
							}
						}
					}
				}

				D3DXVECTOR3 basePos = Ped->GetPos();
				if (basePos.x == 0.0f && basePos.y == 0.0f && basePos.z == 0.0f)
					return D3DXVECTOR3(0, 0, 0);

				if (BoneMask == SKEL_Head)
					return D3DXVECTOR3(basePos.x, basePos.y, basePos.z + 0.85f);
				if (BoneMask == SKEL_Neck_1)
					return D3DXVECTOR3(basePos.x, basePos.y, basePos.z + 0.70f);
				if (BoneMask == SKEL_Pelvis)
					return D3DXVECTOR3(basePos.x, basePos.y, basePos.z);
				if (BoneMask == SKEL_L_Foot || BoneMask == SKEL_R_Foot)
					return D3DXVECTOR3(basePos.x, basePos.y, basePos.z - 0.95f);

				return basePos;
			}

			inline std::string GetPedName(CPed* Ped)
			{
				if (!Ped) return "";

				for (auto& Ent : EntityList)
				{
					if (Ent.Ped != Ped)
						continue;

					if (!Ent.NetworkInfo.UserName.empty() && Ent.NetworkInfo.UserName != xorstr("NPC"))
						return Ent.NetworkInfo.UserName;
				}

				int id = Ped->GetID();
				if (Ped->GetPedType() == 2)
				{
					return xorstr("Player ") + std::to_string(id);
				}

				return xorstr("NPC");
			}

			inline CPed* GetClosestPed(int MaxDistance, bool IgnoreNpc, bool VisibleCheck)
			{
				CPed* ClosestPed = nullptr;
				float ClosestDist = FLT_MAX;

				for (auto& Entity : Core::SDK::Game::EntityList)
				{
					CPed* Ped = Entity.Ped;

					if (Ped == Core::SDK::Pointers::pLocalPlayer)
						continue;

					if (std::abs(Entity.Health) <= 101.f)
						continue;

					if (IgnoreNpc && Entity.PedType != 2)
						continue;

					if (Entity.IsFriend)
						continue;

					if (Entity.Distance > MaxDistance)
						continue;

					D3DXVECTOR2 PedPos = Core::SDK::Game::WorldToScreen(Entity.Pos);

					float x = PedPos.x - g_Variables.g_vGameWindowCenter.x;
					float y = PedPos.y - g_Variables.g_vGameWindowCenter.y;
					float AimDist = std::sqrt((x * x) + (y * y));

					if (AimDist < ClosestDist)
					{
						ClosestDist = AimDist;
						ClosestPed = Ped;
					}
				}

				return ClosestPed;
			}

			inline bool IsOnScreen(D3DXVECTOR2 Pos)
			{
				if (Pos == D3DXVECTOR2(0, 0)) return false;
				return true;
			}

		}
	}

}

inline D3DXVECTOR3 CPed::GetBonePos(const unsigned int BoneMask)
{
	return Core::SDK::Game::GetPedBone(this, BoneMask);
}

inline D3DXVECTOR3 CPed::GetBonePosDefault(const int Bone)
{
	if (Bone == 0)
		return Core::SDK::Game::GetPedBone(this, SKEL_Head);
	if (Bone == 8)
		return Core::SDK::Game::GetPedBone(this, SKEL_Pelvis);
	return Core::SDK::Game::GetPedBone(this, static_cast<unsigned int>(Bone));
}
