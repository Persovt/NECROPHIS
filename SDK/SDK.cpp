#include "SDK.h"





namespace SDK
{
	typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
	typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
	inline CreateInterfaceFn get_module_factory(HMODULE module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
	}
	auto EnginePointer = get_module_factory(GetModuleHandleW(L"engine.dll"));
	
	
	CNetworkStringTableContainer* Interfaces::g_ClientStringTableContainer = nullptr; //custommodels
	IVEngineClient*     Interfaces::g_pEngine = nullptr;
	IBaseClientDLL*     Interfaces::g_pClient = nullptr;
	IClientEntityList*  Interfaces::g_pEntityList = nullptr;
	CGlobalVarsBase*    Interfaces::g_pGlobals = nullptr;
	CInput*             Interfaces::g_pInput = nullptr;
	
	IEngineTrace*       Interfaces::g_pEngineTrace = nullptr;
	IClientMode*        Interfaces::g_pClientMode = nullptr;
	IVModelInfoClient*	Interfaces::g_pModelInfo = nullptr;
	IEngineSound*		Interfaces::g_pSound = nullptr;
	IVModelRender*		Interfaces::g_pModelRender = nullptr;
	IVRenderView*		Interfaces::g_pRenderView = nullptr;
	IMaterialSystem*	Interfaces::g_pMaterialSystem = nullptr;
	ISurface*			Interfaces::g_pSurface = nullptr;
	IGameEventManager2*	Interfaces::g_pGameEventMgr = nullptr;
	IInputSystem*		Interfaces::g_pInputSystem = nullptr;
	ConVar*             Interfaces::g_pConVar = nullptr;
	IViewRenderBeams*   Interfaces::g_ViewRenderBeams = nullptr;
	IDebugOverlay*  Interfaces::g_DebugOverlay = nullptr;
	CEffects*			Interfaces::g_pEffects = nullptr;
	IPhysicsSurfaceProps*  Interfaces::g_Physprops = nullptr;
	ISteamGameCoordinator* Interfaces::g_pSteamGameCoordinator = nullptr;
	ISteamUser* Interfaces::g_pSteamUser = nullptr;
	IVPanel* Interfaces::g_pPanel = nullptr;
	
	template<typename T>
	T* get_interface(CreateInterfaceFn f, char* szInterfaceVersion)
	{
		auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));
		return result;
	}

	CreateInterfaceFn CaptureFactory(char* FactoryModule)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA(FactoryModule), "CreateInterface"));
	}

	IVEngineClient* Interfaces::Engine()
	{	
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pEngine = get_interface<IVEngineClient>(pfnFactory, VENGINE_CLIENT_INTERFACE_VERSION);
		return g_pEngine;
	}

	IBaseClientDLL* Interfaces::Client()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
		g_pClient = get_interface<IBaseClientDLL>(pfnFactory, CLIENT_DLL_INTERFACE_VERSION);
		return g_pClient;
	}


	IClientEntityList* Interfaces::EntityList()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
		g_pEntityList = get_interface<IClientEntityList>(pfnFactory, VCLIENTENTITYLIST_INTERFACE_VERSION);
		return g_pEntityList;
	}

	CGlobalVarsBase* Interfaces::GlobalVars()
	{
		auto pClientVFTable = *(uint32_t**)Client();
		g_pGlobals = **(CGlobalVarsBase***)(pClientVFTable[0] + 0x1B);
		return g_pGlobals;
	}

	IViewRenderBeams* Interfaces::g_pViewRenderBeams()
	{
		g_ViewRenderBeams = *(IViewRenderBeams**)(CSX::Memory::FindPattern(CLIENT_DLL, "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08") + 0x1);
		return g_ViewRenderBeams;
	}

	CInput* Interfaces::Input()
	{
		g_pInput = *(CInput**)(CSX::Memory::FindPattern(CLIENT_DLL, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		return g_pInput;
	}

	IEngineTrace* Interfaces::EngineTrace()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pEngineTrace = get_interface<IEngineTrace>(pfnFactory, INTERFACEVERSION_ENGINETRACE_CLIENT);
		return g_pEngineTrace;
	}

	IClientMode* Interfaces::ClientMode()
	{
		auto pClientVFTable = *(uint32_t**)Client();
		g_pClientMode = **(IClientMode***)(pClientVFTable[10] + 0x5);
		return g_pClientMode;
	}
	ISteamUser* Interfaces::SteamUser()
	{
		SteamGameCoordinator();
		return g_pSteamUser;
	}
	ISteamGameCoordinator* Interfaces::SteamGameCoordinator()
	{
		if (!g_pSteamGameCoordinator) {
			typedef uint32_t SteamPipeHandle;
			typedef uint32_t SteamUserHandle;

			SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
			SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();

			auto SteamClient = ((ISteamClient*(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();

			auto SteamHTTP = SteamClient->GetISteamHTTP(hSteamUser, hSteamPipe, "STEAMHTTP_INTERFACE_VERSION002");
			g_pSteamUser = SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
			auto SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
			auto SteamInventory = SteamClient->GetISteamInventory(hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002");
			g_pSteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");
		}

		return g_pSteamGameCoordinator;
	}
	IVModelInfoClient* Interfaces::ModelInfo()
	{
		if (!g_pModelInfo)
		{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pModelInfo = get_interface<IVModelInfoClient>(pfnFactory, VMODELINFO_CLIENT_INTERACE_VERSION);
		}

		return g_pModelInfo;
	}

	IEngineSound* Interfaces::Sound()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pSound = get_interface<IEngineSound>(pfnFactory, IENGINESOUND_CLIENT_INTERFACE_VERSION);
		return g_pSound;
	}

	IVModelRender* Interfaces::ModelRender()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pModelRender = get_interface<IVModelRender>(pfnFactory, VENGINE_HUDMODEL_INTERFACE_VERSION);
		return g_pModelRender;
	}

	IVRenderView* Interfaces::RenderView()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pRenderView = get_interface<IVRenderView>(pfnFactory, VENGINE_RENDERVIEW_INTERFACE_VERSION);
		return g_pRenderView;
	}

	IMaterialSystem* Interfaces::MaterialSystem()
	{	
		CreateInterfaceFn pfnFactory = CaptureFactory(MATERIAL_DLL);
		g_pMaterialSystem = get_interface<IMaterialSystem>(pfnFactory, MATERIAL_SYSTEM_INTERFACE_VERSION);
		return g_pMaterialSystem;
	}

	ISurface* Interfaces::Surface()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VGUIMT_DLL);
		g_pSurface = get_interface<ISurface>(pfnFactory, VGUI_SURFACE_INTERFACE_VERSION);
		return g_pSurface;
	}

	IGameEventManager2*	Interfaces::GameEvent()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_pGameEventMgr = get_interface<IGameEventManager2>(pfnFactory, GAMEEVENTMANAGER_INTERFACE_VERSION);
		return g_pGameEventMgr;
	}

	IInputSystem* Interfaces::InputSystem()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(INPUTSYSTEM_DLL);
		g_pInputSystem = get_interface<IInputSystem>(pfnFactory, INPUTSYSTEM_INTERFACE_VERSION);
		return g_pInputSystem;
	}

	ConVar* Interfaces::GetConVar()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VSTDLIB_DLL);
		g_pConVar = get_interface<ConVar>(pfnFactory, VENGINEVCAR_INTERFACE_VERSION);
		return g_pConVar;
	}

	CEffects* Interfaces::Effects()
	{
			CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
			g_pEffects = get_interface<CEffects>(pfnFactory, VENGINEVEFFECTS_INTERFACE_VERSION);
			return g_pEffects;
	}

	IDebugOverlay* Interfaces::DebugOverlay()
	{
		g_DebugOverlay = (IDebugOverlay*)EnginePointer("VDebugOverlay004", nullptr);
		return g_DebugOverlay;
	}
	CNetworkStringTableContainer* Interfaces::ClientStringTableContainer()
	{
		g_ClientStringTableContainer = (CNetworkStringTableContainer*)EnginePointer("VEngineClientStringTable001", nullptr);
		return g_ClientStringTableContainer;
	}
	IVPanel* Interfaces::VPanel()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory("vgui2.dll");
		g_pPanel = get_interface<IVPanel>(pfnFactory, "VGUI_Panel009");
		return g_pPanel;
	}
	IPhysicsSurfaceProps* Interfaces::Physprops()
	{
		CreateInterfaceFn pfnFactory = CaptureFactory("vphysics.dll");
		g_Physprops = get_interface<IPhysicsSurfaceProps>(pfnFactory, "VPhysicsSurfaceProps001");
		return g_Physprops;
	}
}


































































































































































































































































































































































