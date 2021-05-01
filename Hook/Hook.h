#pragma once

#include "../Engine/Engine.h"
//#include "Render2.h"
#include "../VMT/Vmthook.h"
typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);                  // PaintTraverse hook
using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);                 // CreateMove hook
using EmitsoundXD = void(__stdcall*)(IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags, int iPitch,
const Vector *pOrigin, const Vector *pDirection, CUtlVector< Vector >* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int test);
using PlaySound = void(__thiscall *)(ISurface*, const char*);
using CreateMove = void(__thiscall *)(IClientMode*, float flInputSampleTime, CUserCmd* pCmd);
typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
using OverrideView = void(__thiscall*)(IClientMode*, CViewSetup*);
namespace Engine
{
	namespace index
	{
		constexpr auto DrawModelExecute = 21;
		constexpr auto FrameStageNotify = 37;
		constexpr auto OverrideView = 18;
		constexpr auto Reset = 16;
		constexpr auto CreateMove = 24;
		constexpr auto PaintTraverse = 41;
		constexpr auto GetViewmodelFOV = 35;
		constexpr auto Hook_FireEventClientSideThink = 9;
		constexpr auto SvCheatsGetBool = 13;
	}
	namespace Hook
	{
		
		extern vfunc_hook client;
		bool Initialize();
		void Shutdown();
	}
}
