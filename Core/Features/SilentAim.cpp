#include "SilentAim.hpp"

bool Core::Features::cSilentAim::SilentAimInitialized = false;
std::uintptr_t Core::Features::cSilentAim::StartAddy = 0;

void Core::Features::cSilentAim::InitializeSilentAim( )
{
	StartAddy = g_Offsets.m_SilentAim;
	if ( !StartAddy ) return;

	if ( !SilentAimHook )
	{
		SilentAimHook = Mem.CreateCodeCave( 500 );
		if ( !SilentAimHook ) return;
	}

	uint8_t disp = Mem.Read<uint8_t>( StartAddy + 3 );
	if ( disp != 0 )
	{
		SilentAimShell[ 2 ] = disp;
		SilentAimShell[ 9 ] = disp + 4;
		SilentAimShell[ 16 ] = disp + 8;
	}

	uintptr_t BackAddress = ( uintptr_t ) ( StartAddy + 0x26 );
	memcpy( SilentAimShell.data( ) + 27, &BackAddress, sizeof( BackAddress ) );

	Mem.WriteBytes( SilentAimHook, SilentAimShell );

	if ( OriginalFuncTable.empty( ) )
	{
		OriginalFuncTable = Mem.ReadBytes( StartAddy, 38 );
	}

	Mem.HookJMP( StartAddy, SilentAimHook );
	SilentAimInitialized = true;
}

void Core::Features::cSilentAim::RestoreSilent( )
{
	if ( !StartAddy || OriginalFuncTable.empty( ) ) return;

	Mem.WriteBytes( StartAddy, OriginalFuncTable );
	SilentAimInitialized = false;
}

void Core::Features::cSilentAim::HookSilent( )
{
	while ( true )
	{
		if ( g_Config.SilentAim->Enabled && g_Config.SilentAim->KeyBind != 0 && ( GetAsyncKeyState( g_Config.SilentAim->KeyBind ) & 0x8000 ) && GetForegroundWindow( ) != g_Variables.g_hCheatWindow )
		{
			CPed * Ped = Core::SDK::Game::GetClosestPed( g_Config.SilentAim->MaxDistance, g_Config.SilentAim->IgnoreNPCs, g_Config.SilentAim->OnlyVisible );
			if ( Ped )
			{
				D3DXVECTOR3 HeadPos = Ped->GetBonePosDefault( 0 );
				D3DXVECTOR2 HeadToScreen = Core::SDK::Game::WorldToScreen( HeadPos );

				if ( Core::SDK::Game::IsOnScreen( HeadToScreen ) )
				{
					int Fov = std::hypot( HeadToScreen.x - g_Variables.g_vGameWindowCenter.x, HeadToScreen.y - g_Variables.g_vGameWindowCenter.y );
					if ( Fov < g_Config.SilentAim->FOV )
					{
						if ( !SilentAimInitialized && g_Offsets.m_SilentAim )
						{
							InitializeSilentAim( );
						}

						bool Miss = g_Config.SilentAim->MissChance >= Utils::GenRandomInt( 0, 100 );
						auto FinalPos = Miss ? HeadPos + D3DXVECTOR3( 0.0f, 0.4f, 0.0f ) : HeadPos + D3DXVECTOR3( 0.0f, 0.0f, 0.08f );

						if ( SilentAimHook && SilentAimInitialized )
						{
							memcpy( SilentAimShell.data( ) + 3, &FinalPos.x, sizeof( float ) );
							memcpy( SilentAimShell.data( ) + 10, &FinalPos.y, sizeof( float ) );
							memcpy( SilentAimShell.data( ) + 17, &FinalPos.z, sizeof( float ) );

							uintptr_t BackAddress = ( uintptr_t ) ( StartAddy + 0x26 );
							memcpy( SilentAimShell.data( ) + 27, &BackAddress, sizeof( BackAddress ) );

							Mem.WriteBytes( SilentAimHook, SilentAimShell );
						}

						// Weapon muzzle redirect
						if ( Core::SDK::Pointers::pLocalPlayer )
						{
							CWeaponManager * WeaponMgr = Core::SDK::Pointers::pLocalPlayer->GetWeaponManager( );
							if ( WeaponMgr )
							{
								auto CObject = Mem.Read<uintptr_t>( ( uintptr_t ) WeaponMgr + g_Offsets.m_CObject );
								if ( CObject )
								{
									uintptr_t CWeapon = Mem.Read<uintptr_t>( CObject + g_Offsets.m_CWeapon );
									if ( CWeapon )
									{
										Mem.Write( CWeapon + 0x20, FinalPos );
									}
								}
							}
						}
					}
					else if ( SilentAimInitialized )
					{
						RestoreSilent( );
					}
				}
				else if ( SilentAimInitialized )
				{
					RestoreSilent( );
				}
			}
			else if ( SilentAimInitialized )
			{
				RestoreSilent( );
			}
		}
		else if ( SilentAimInitialized )
		{
			RestoreSilent( );
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
}
