


#include "Hook.h"
#include <intrin.h>

namespace Engine
{
	namespace Hook
	{
		vfunc_hook direct3d_hook;
		vfunc_hook mdlrender_hook;
		vfunc_hook clientmode_hook;
		vfunc_hook panel;
		vfunc_hook client;
		vfunc_hook sv_cheats;
		vfunc_hook GameEventTable;

		IDirect3DDevice9* g_pDevice = nullptr;

		typedef HRESULT(WINAPI* Present_t)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
		Present_t Present_o;


		//--------------------------------------------------------HOOK RESETS------------------------------------------------------------------
		HRESULT WINAPI Hook_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
		{
			if (Render)
			{
				if (Client)
					Client->OnRender();

				if (Radar)
					Radar->OnRender();

				if (Misc)
					Misc->OnRender();//

				if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
				{
					if (Aimbot)
						Aimbot->OnRender();//

					if (Esp)
						Esp->OnRender();//
				}
			}
			return Present_o(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}


		long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
		{
			static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(index::Reset);

			if (Render)
				Render->OnLostDevice();

			if (Gui)
				ImGui_ImplDX9_InvalidateDeviceObjects();
			
			auto hr = oReset(device, pPresentationParameters);


			if (Render)
				Render->OnResetDevice();

			if (Gui)
				ImGui_ImplDX9_CreateDeviceObjects();

			return hr;
		}

		//------------------------------------------------------CREATEMOVE--------------------------------------------------------------------------------------
		
		
		bool __fastcall Hook_CreateMove(void* _this, void* edx, float flInputSampleTime, CUserCmd* cmd)
		{
			static auto ofunc = clientmode_hook.get_original<decltype(&Hook_CreateMove)>(24);

			if (!cmd || !cmd->command_number)
				return ofunc;

			uintptr_t* FPointer; __asm { MOV FPointer, EBP }
			byte* SendPacket = (byte*)(*FPointer - 0x1C);

			if (!SendPacket) return false;
			bSendPacket = *SendPacket;
			
			if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame())
			{
				
				mLocal = Players->GetLocal();
				LocalPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

				Players->Update();

				if (settings.RageBot.aimbot_resolver)
					Resolver->OnCreateMove();
				
				if (GetAsyncKeyState(settings.Visual.thirdperson) & 1)
					settings.Visual.thirdperson_tp = !settings.Visual.thirdperson_tp;

				if(Ragebot)
				 Ragebot->OnCreateMove(cmd);

			    if(Aimbot)
				 Aimbot->OnCreateMove(cmd);

				if(AntiAim)
				 AntiAim->OnCreateMove(cmd);

				if (Client)
					Client->OnCreateMove(cmd);
					//FakeLag->Fakelag(cmd);

				if(Esp)
				 Esp->OnCreateMove(cmd);

				if(Misc)
				 Misc->OnCreateMove(cmd);
				
				RealAngles = cmd->viewangles;

				if (settings.RageBot.aimbot_enabled || settings.RageBot.antiaim_enabled)
					return false;

			}
			*SendPacket = bSendPacket;
			return ofunc(Interfaces::ClientMode(), edx, flInputSampleTime, cmd);			
		}
		
		//------------------------------------------------------------------OVERRIDEVIEW---------------------------------------------------------
		
		void __fastcall hkOverrideView(void* _this, int edx, CViewSetup* vsView)
		{
			if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame())
			{
				static auto ofunc = clientmode_hook.get_original<decltype(&hkOverrideView)>(index::OverrideView);
				if (settings.Misc.misc_FovChanger && !LocalPlayer->GetIsScoped())
				{ 
				int fov = 90;
				fov = settings.Misc.misc_FovView;
				vsView->fov = fov;
				}
				ofunc(Interfaces::ClientMode(), edx, vsView);
			}
		}

			
		float __fastcall GetViewmodelFOV()
		{
			if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame())
			{
				Esp->ThirdPerson();

			    float fov = LocalPlayer->GetFovStart();

				if (settings.Misc.misc_FovChanger)
					fov = (int)settings.Misc.misc_FovModelView;

				 return fov;

			}
		}
		
			
		
		
		//--------------------------------------------------------------FIREEVENT------------------------------------------------------------------------------------

		void __fastcall Hook_FireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent)
		{
			static auto ofunc = GameEventTable.get_original<decltype(&Hook_FireEventClientSideThink)>(9);
			
			if (!strcmp(pEvent->GetName(), "player_connect_full") ||!strcmp(pEvent->GetName(), "round_start") || !strcmp(pEvent->GetName(), "cs_game_disconnected"))
			{
				if (Players)
					Players->Clear();

				if (Esp)
					Esp->OnReset();
			}

			if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				if (Esp)
					Esp->OnEvents(pEvent);		

			//	if (Skin)
				//	Skin->OnEvents(pEvent);
			}

			ofunc(Interfaces::GameEvent(), edx, pEvent);
		}

		//-------------------------------------------------------------FRAMESTAGE------------------------------------
	
		

		void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t Stage)
		{
			static auto ofunc = client.get_original<decltype(&hkFrameStageNotify)>(index::FrameStageNotify);

			if (!LocalPlayer)
				return;			
			
			if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				if (Interfaces::g_pInput->m_fCameraInThirdPerson)
					*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(LocalPlayer) + 0x31D8) = RealAngles;


				if(Esp)
				 Esp->NightMode();

				if (settings.RageBot.aimbot_resolver)
					Resolver->FrameStage(Stage);

				//SkinChanger->fsn_skinchanger(local_player, Stage);

			}
			ofunc(Interfaces::Client(), edx, Stage);
		}
		

		



		//------------------------------------------------------------DRAWMODEL--------------------------------------------------------------------------------------
		void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL)
		{
			static auto ofunc = mdlrender_hook.get_original<decltype(&hkDrawModelExecute)>(index::DrawModelExecute);

			if (Interfaces::ModelRender()->IsForcedMaterialOverride() &&
				!strstr(pInfo.pModel->szName, "arms") &&
				!strstr(pInfo.pModel->szName, "weapons/v_")) {
				return ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				if (Esp)
					Esp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

				if (Misc)
					Misc->OnDrawModelExecute();

				IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);
				if (pBaseEntity || LocalPlayer)
				{
					bool IsMe = LocalPlayer == pBaseEntity;
					if ((LocalPlayer->GetIsScoped() && settings.Visual.blendonscope && settings.Visual.thirdperson_tp && IsMe) || (!settings.Visual.blendonscope && settings.Visual.thirdperson_tp && IsMe))
						Interfaces::RenderView()->SetBlend(settings.Visual.blend);
				}
			}
			ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);

			Interfaces::ModelRender()->ForcedMaterialOverride(nullptr);
		}

		
		void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
		{
				if (settings.Visual.noscope && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected() && LocalPlayer->GetIsScoped())
				{
					if (!strcmp("HudZoom", Interfaces::VPanel()->GetName(vguiPanel))) return;

					IMaterial *xblur_mat = Interfaces::MaterialSystem()->FindMaterial("dev/blurfilterx_nohdr", TEXTURE_GROUP_OTHER, true);
					IMaterial *yblur_mat = Interfaces::MaterialSystem()->FindMaterial("dev/blurfiltery_nohdr", TEXTURE_GROUP_OTHER, true);
					IMaterial *scope = Interfaces::MaterialSystem()->FindMaterial("dev/scope_bluroverlay", TEXTURE_GROUP_OTHER, true);
					xblur_mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
					yblur_mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
					scope->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
			
			string sPanel = ("FocusOverlayPanel");
			const auto panelName = Interfaces::VPanel()->GetName(vguiPanel);
			static auto ofunc = panel.get_original<paint_traverse_t>(41);
	
			ofunc(pPanels, vguiPanel, forceRepaint, allowForce);

			if (!strstr(panelName, sPanel.data()))return;

			Interfaces::VPanel()->SetMouseInputEnabled(vguiPanel, bIsGuiVisible);
		}
		

	
		auto dwCAM_Think = CSX::Memory::FindPattern(CLIENT_DLL,"85 C0 75 30 38 86",0);
		typedef bool(__thiscall *svc_get_bool_t)(PVOID);
		bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
		{
			static auto ofunc = sv_cheats.get_original<svc_get_bool_t>(13);
			if (!ofunc)
				return false;

			if (reinterpret_cast<DWORD>(_ReturnAddress()) == dwCAM_Think)
				return true;
			return ofunc(pConVar);
		}
		
		//-------------------------------------------------------------INITIALIZE---------------------------------------------------
		bool Initialize()
		{
			DWORD d3d9TablePtrPtr = CSX::Memory::FindPattern(SHADERPIDX9_DLL, D3D9_PATTERN, D3D9_MASK, 1);
			DWORD_PTR** dwPresent_o = (DWORD_PTR**)CSX::Memory::FindPattern(GAMEOVERLAYRENDERER_DLL, GMOR_PATTERN, GMOR_MASK, 1);
			

			g_pDevice = (IDirect3DDevice9*)(**(PDWORD*)d3d9TablePtrPtr);


			ConVar* sv_cheats_con = Interfaces::GetConVar()->FindVar("sv_cheats");
			sv_cheats.setup(sv_cheats_con);

					DWORD_PTR* dwAddress = *dwPresent_o;
					Present_o = (Present_t)(*dwAddress);
					*dwAddress = (DWORD_PTR)(&Hook_Present);
					direct3d_hook.setup(g_pDevice);			
					panel.setup(Interfaces::VPanel());
					client.setup(Interfaces::Client());
					GameEventTable.setup(Interfaces::GameEvent());				
					mdlrender_hook.setup(Interfaces::ModelRender());
					clientmode_hook.setup(Interfaces::ClientMode());
					sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);
					direct3d_hook.hook_index(index::Reset, hkReset);			
					clientmode_hook.hook_index(index::OverrideView, hkOverrideView);
					clientmode_hook.hook_index(index::CreateMove, Hook_CreateMove);
					clientmode_hook.hook_index(index::GetViewmodelFOV, GetViewmodelFOV);
					panel.hook_index(index::PaintTraverse, hkPaintTraverse);
					client.hook_index(index::FrameStageNotify, hkFrameStageNotify);
					GameEventTable.hook_index(index::Hook_FireEventClientSideThink, Hook_FireEventClientSideThink);
					mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);			
					Client->Initialize(g_pDevice);
					return true;
		}

		void Shutdown()
		{
			direct3d_hook.unhook_all();
			panel.unhook_all();				
			clientmode_hook.unhook_all();
			GameEventTable.unhook_all();
			mdlrender_hook.unhook_all();
			client.unhook_all();
		}
	}
}
