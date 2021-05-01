#include "Engine.h"
#include "Auth.h"

#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#pragma warning(disable:4244)


namespace Engine
{
	bool Initialize()
	{
		Auth::Auth();
		!CSX::Utils::IsModuleLoad( CLIENT_DLL , 45000 );
		!CSX::Utils::IsModuleLoad( ENGINE_DLL );
		!CSX::Utils::IsModuleLoad( VGUI2_DLL );
		!CSX::Utils::IsModuleLoad( VGUIMAT_DLL );
		!CSX::Utils::IsModuleLoad( VSTDLIB_DLL);
		!Interfaces::GetConVar();
		!Interfaces::SteamUser();
		!Interfaces::ClientStringTableContainer();
		!Interfaces::VPanel();
		!Interfaces::Physprops();
		!Interfaces::Effects();
	    !Interfaces::Engine();
		!Interfaces::Client();
	    !Interfaces::EntityList();
		!Interfaces::GlobalVars();
		!Interfaces::EngineTrace();
	    !Interfaces::ClientMode();
	    !Interfaces::ModelInfo() ;
	    !Interfaces::Sound();
	    !Interfaces::ModelRender();
    	!Interfaces::RenderView();
    	!Interfaces::MaterialSystem();
    	!Interfaces::Surface();
    	!Interfaces::GameEvent();
    	!Interfaces::InputSystem();
    	!Interfaces::Input();
    	!Interfaces::g_pViewRenderBeams();
    	!Interfaces::DebugOverlay();
		!g_NetVar.Init(Interfaces::Client()->GetAllClasses());
		  return true;
	}

	
	void Shutdown()
	{
		Hook::Shutdown();
		
	}

	WEAPON_TYPE GetWeaponType(int iItemDefinitionIndex)
	{
		switch (iItemDefinitionIndex)
		{
		case WEAPON_DEAGLE:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_ELITE:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_FIVESEVEN:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_GLOCK:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_AK47:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_AUG:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_AWP:
			return WEAPON_TYPE_SNIPER;
		case WEAPON_FAMAS:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_G3SG1:
			return WEAPON_TYPE_SNIPER;
		case WEAPON_GALILAR:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_M249:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_M4A4:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_MAC10:
			return WEAPON_TYPE_SMG;
		case WEAPON_P90:
			return WEAPON_TYPE_SMG;
		case WEAPON_MP5:
			return WEAPON_TYPE_SMG;
		case WEAPON_UMP45:
			return WEAPON_TYPE_SMG;
		case WEAPON_XM1014:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_BIZON:
			return WEAPON_TYPE_SMG;
		case WEAPON_MAG7:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_NEGEV:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_SAWEDOFF:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_TEC9:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_TASER:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_HKP2000:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_MP7:
			return WEAPON_TYPE_SMG;
		case WEAPON_MP9:
			return WEAPON_TYPE_SMG;
		case WEAPON_NOVA:
			return WEAPON_TYPE_SHOTGUN;
		case WEAPON_P250:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_SCAR20:
			return WEAPON_TYPE_SNIPER;
		case WEAPON_SG553:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_SSG08:
			return WEAPON_TYPE_SNIPER;
		case WEAPON_KNIFE:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_FLASHBANG:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_HEGRENADE:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_SMOKEGRENADE:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_MOLOTOV:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_DECOY:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_INCGRENADE:
			return WEAPON_TYPE_GRENADE;
		case WEAPON_C4:
			return WEAPON_TYPE_C4;
		case WEAPON_KNIFE_T:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_M4A1_SILENCER:
			return WEAPON_TYPE_RIFLE;
		case WEAPON_USP_SILENCER:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_CZ75A:
			return WEAPON_TYPE_PISTOL;
		case WEAPON_REVOLVER:
			return WEAPON_TYPE_PISTOL;
		
		case WEAPON_KNIFE_FLIP:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_GUT:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_KARAMBIT:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_M9_BAYONET:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_TACTICAL:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_FALCHION:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_BUTTERFLY:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_PUSH:
			return WEAPON_TYPE_KNIFE;
		
		case WEAPON_KNIFE_STILETTO:
			return WEAPON_TYPE_KNIFE;
		case WEAPON_KNIFE_URSUS:
			return WEAPON_TYPE_KNIFE;
	
		default:
			return WEAPON_TYPE_UNKNOWN;
		}

		return WEAPON_TYPE_UNKNOWN;
	}

	bool IsLocalAlive()
	{
		if (Players &&
			Players->GetLocal() &&
			Players->GetLocal()->bAlive &&
			Players->GetLocal()->m_pEntity &&
			Players->GetLocal()->m_pWeaponEntity)
			return true;

		return false;
	}
	
	bool WorldToScreen(const Vector& vOrigin, Vector& vScreen)
	{
		static float* ViewMatrixOld = nullptr;
		float* ViewMatrix = nullptr;

		if (!ViewMatrixOld)
		{
			ViewMatrixOld = Offset::FindW2Matrix();
		}
		else
		{
			ViewMatrix = (float*)(*(PDWORD)(ViewMatrixOld)+0x3DC);
		}

		if (ViewMatrix && *ViewMatrix)
		{
			vScreen.x = ViewMatrix[0] * vOrigin.x + ViewMatrix[1] * vOrigin.y + ViewMatrix[2] * vOrigin.z + ViewMatrix[3];
			vScreen.y = ViewMatrix[4] * vOrigin.x + ViewMatrix[5] * vOrigin.y + ViewMatrix[6] * vOrigin.z + ViewMatrix[7];
			float w = ViewMatrix[12] * vOrigin.x + ViewMatrix[13] * vOrigin.y + ViewMatrix[14] * vOrigin.z + ViewMatrix[15];

			if (w < 0.01f)
				return false;

			float invw = 1.0f / w;
			vScreen.x *= invw;
			vScreen.y *= invw;

			float x = (float)iScreenWidth / 2.f;
			float y = (float)iScreenHeight / 2.f;

			x += 0.5f * vScreen.x * iScreenWidth + 0.5f;
			y -= 0.5f * vScreen.y * iScreenHeight + 0.5f;

			vScreen.x = x;
			vScreen.y = y;

			return true;
		}

		return false;
	}



	bool LineGoesThroughSmoke( Vector vStartPos , Vector vEndPos )
	{
		typedef bool( __cdecl* _LineGoesThroughSmoke ) ( Vector , Vector );

		static _LineGoesThroughSmoke LineGoesThroughSmokeFn = 0;
		static bool SearchFunction = false;

		if ( !SearchFunction )
		{
			DWORD dwFunctionAddress = CSX::Memory::FindPattern( CLIENT_DLL , SMOK_PATTERN , "xxxxxxxx????xxx" , 0 );

			if ( dwFunctionAddress )
			{
				LineGoesThroughSmokeFn = (_LineGoesThroughSmoke)dwFunctionAddress;
				SearchFunction = true;
			}
		}

		if ( LineGoesThroughSmokeFn && SearchFunction )
		{
			return LineGoesThroughSmokeFn( vStartPos , vEndPos );
		}

		return false;
	}

	bool CheckPlayerFov( Vector vScreen , int iFov )
	{
		int	iFovX = (int)CenterScreens.x - (int)vScreen.x;
		int	iFovY = (int)CenterScreens.y - (int)vScreen.y;

		if ( iFovX < iFov && iFovX > -iFov && iFovY < iFov && iFovY > -iFov )
			return true;

		return false;
	}


	IMaterial* CreateMaterial( bool bFlat , bool bShouldIgnoreZ )
	{
		static int iCreated = 0;

		static const char szTmp[] =
		{
			"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"VGUI/white_additive\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"1\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"1\"\
		\n\t\"halflambert\" \"1\"\
		\n\t\"$wireframe\" \"%i\"\
		\n}\n"
		};

		char szMaterial[256];
		char szBaseType[18];

		if ( bFlat )
		{
			static const char pszBaseType[] = "UnlitGeneric";
			sprintf_s( szMaterial , sizeof( szMaterial ) , szTmp , pszBaseType , ( bShouldIgnoreZ ) ? 1 : 0 , 0 );
			strcpy_s( szBaseType , pszBaseType );
		}
		else
		{
			static const char pszBaseType[] = "VertexLitGeneric";
			sprintf_s( szMaterial , sizeof( szMaterial ) , szTmp , pszBaseType , ( bShouldIgnoreZ ) ? 1 : 0 , 0 );
			sprintf_s( szBaseType , pszBaseType );
		}

		char szName[25];
		sprintf_s( szName , sizeof( szName ) , "custom_material_%i.vmt" , iCreated );

		++iCreated;

		auto pKeyValues = new KeyValues( szBaseType );
		pKeyValues->LoadFromBuffer( pKeyValues , szName , szMaterial );
		auto pCreatedMaterial = Interfaces::MaterialSystem()->CreateMaterial( szName , pKeyValues );
		pCreatedMaterial->IncrementReferenceCount();
		return pCreatedMaterial;
	}

	void ForceMaterial( Color color , IMaterial* material , bool useColor , bool forceMaterial )
	{
		if ( useColor )
		{
			float blend[3] = { (float)color.r() / 255.f, (float)color.g() / 255.f, (float)color.b() / 255.f };
			float alpha = (float)color.a() / 255.f;

			Interfaces::RenderView()->SetBlend( alpha );
			Interfaces::RenderView()->SetColorModulation( blend );
		}

		if ( forceMaterial )
			Interfaces::ModelRender()->ForcedMaterialOverride( material );
	}

	BOOL SearchFiles( LPCTSTR lpszFileName , LPSEARCHFUNC lpSearchFunc , BOOL bInnerFolders = FALSE )
	{
		LPTSTR part;
		char tmp[MAX_PATH];
		char name[MAX_PATH];

		HANDLE hSearch = NULL;
		WIN32_FIND_DATA wfd;
		memset( &wfd , 0 , sizeof( WIN32_FIND_DATA ) );

		if ( bInnerFolders )
		{
			if ( GetFullPathName( lpszFileName , MAX_PATH , tmp , &part ) == 0 ) return FALSE;
			strcpy( name , part );
			strcpy( part , "*.*" );
			wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
			if ( !( ( hSearch = FindFirstFile( tmp , &wfd ) ) == INVALID_HANDLE_VALUE ) )
				do
				{
					if ( !strncmp( wfd.cFileName , "." , 1 ) || !strncmp( wfd.cFileName , ".." , 2 ) )
						continue;

					if ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						char next[MAX_PATH];
						if ( GetFullPathName( lpszFileName , MAX_PATH , next , &part ) == 0 ) return FALSE;
						strcpy( part , wfd.cFileName );
						strcat( next , "\\" );
						strcat( next , name );

						SearchFiles( next , lpSearchFunc , TRUE );
					}
				} while ( FindNextFile( hSearch , &wfd ) );
				FindClose( hSearch );
		}

		if ( ( hSearch = FindFirstFile( lpszFileName , &wfd ) ) == INVALID_HANDLE_VALUE )
			return TRUE; 
		do
			if ( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				char file[MAX_PATH];
				if ( GetFullPathName( lpszFileName , MAX_PATH , file , &part ) == 0 ) return FALSE;
				strcpy( part , wfd.cFileName );

				lpSearchFunc( wfd.cFileName );
			}
		while ( FindNextFile( hSearch , &wfd ) );
		FindClose( hSearch );
		return TRUE;
	}
}

bool CTimer::delay( DWORD dwMsec )
{
	if ( !bEnable )
		return true;

	if ( !dwTime )
		dwTime = GetTickCount64();

	if ( dwTime + dwMsec < GetTickCount64() )
	{
		//dwTime = 0;
		bEnable = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CTimer::reset()
{
	dwTime = 0;
	bEnable = true;
}

void CTimer::stop()
{
	bEnable = false;
}