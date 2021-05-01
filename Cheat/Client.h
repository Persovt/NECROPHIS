#pragma once

#define DELETE_MOD(name) delete name; name = nullptr

#include "../Gui/Gui.h"
#include "../Engine/Engine.h"

#include "Aimbot/Aimbot.h"
#include "Esp/Esp.h"
#include "Radar/Radar.h"
#include "Misc/Misc.h"
//#include "Skin/Skin.h"
#include "RageBot/Resolver.h"
//#include "Fakelag/Fakelag.h"
#include "Antiaim/Antiaim.h"
#include "RageBot/Autowall.h"
#include "RageBot/Ragebot.h"
//#include "Skin/InventoryChanger.h"
#include "cfgmsgpop/cfgmsgpop.h"
#include "Skin/SkinChanger.h"
namespace Engine
{
	//class CRender;
	class CPlayers;
}


using namespace Engine;


	


	extern string BaseDir;
	extern string LogFile;
	extern string GuiFile;
	extern string IniFile;

	extern bool			bC4Timer;
	extern int			iC4Timer;


	extern int			iWeaponSelectSkinIndex;
	extern int			iWeaponSelectIndex;
	

	class CClient
	{
	public:
		bool Initialize(IDirect3DDevice9* pDevice);
		void Shutdown();
		void OnRender();
		void OnCreateMove(CUserCmd* pCmd);
		void OnRenderGUI();
		
	};
	extern CClient* Client;