#include "Client.h"
#include <locale.h>
#include <fstream>
#include <urlmon.h>
#include <windows.h>
#include <io.h>
#include <shellapi.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include "image.h"
#include "user.h"



IDirect3DTexture9 *tImage = nullptr;

IDirect3DTexture9 *rus1 = nullptr;
IDirect3DTexture9 *eng1 = nullptr;
IDirect3DTexture9 *Users = nullptr;
IDirect3DTexture9 *black = nullptr;
IDirect3DTexture9 *bac = nullptr;
CRender*	Render = nullptr;


	

	string BaseDir = "";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";

	vector<string> ConfigList;


	static char clantag[128] = { 0 };
	bool		bC4Timer = false;
	int			iC4Timer = 40;
	
	int         WeaponType;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = 0;
	
	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = "C:\\SQUIRT\\*";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	
	
	bool CClient::Initialize(IDirect3DDevice9* pDevice)
	{
		Render = new CRender(pDevice);
	//	g_pRender = new CRender(pDevice);
		CreateDirectoryW(L"C:\\SQUIRT", NULL);
	//	GuiFile = "C:\\TRB-PROJECT\\gui";
		IniFile = "C:\\SQUIRT\\settings";

		if (tImage == nullptr)D3DXCreateTextureFromFileInMemory(pDevice, &NameArry, sizeof(NameArry), &tImage);
		if (black == nullptr)D3DXCreateTextureFromFileInMemory(pDevice, &black_av, sizeof(black_av), &black);
		if (rus1 == nullptr)D3DXCreateTextureFromFileInMemory(pDevice, &rus, sizeof(rus), &rus1);
		if (eng1 == nullptr)D3DXCreateTextureFromFileInMemory(pDevice, &eng, sizeof(eng), &eng1);
		if (Users == nullptr)D3DXCreateTextureFromFileInMemory(pDevice, &User, sizeof(User), &Users);

		Gui->GUI_Init(pDevice);

	//	if(Skin)//
	//	Skin->InitalizeSkins();
		

		//Settings::LoadSettings(IniFile);

	//	iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

	
		RefreshConfigs();
		

		return true;
	}
	
	

	

	void CClient::OnRender()
	{
		if (!Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			Render->BeginRender();

			if (Gui)
				Gui->GUI_Draw_Elements();

			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			CenterScreens.x = iScreenWidth / 2.f;
			CenterScreens.y = iScreenHeight / 2.f;

			Render->EndRender();
		}
	}
	const char* weapons[34] =
	{
		"deagle",
		"elite",
		"fiveseven",
		"glock18",
		"p2000",
		"p250",
		"usp_s",
		"cz75",
		"revolver",
		"tec9",
		"ak47",
		"aug",
		"famas",
		"galilar",
		"m249",
		"m4a4",
		"m4a1_s",
		"mac10",
		"p90",
		"mp5-sd",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"mp7",
		"mp9",
		"nova",
		"sg553",
		"scar20",
		"g3sg1",
		"awp",
		"ssg08"
	};
   
	int GetWeaponSettingsSelectID()
	{
		for (size_t i = 0; i <= 34; i++)
		{
			if (pWeaponItemIndexData[i] == Players->GetLocal()->WeaponIndex)
				return i;
		}

		return -1;
	}

	void CClient::OnCreateMove(CUserCmd* pCmd)
	{
		if (!bIsGuiVisible)
		{
			int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

			if (iWeaponSettingsSelectID >= 0)
				iWeaponID = iWeaponSettingsSelectID;
		}
		//QAngle view;
		//Interfaces::Engine()->GetViewAngles(view);
		
	}

	

	void CClient::OnRenderGUI()
	{
		

		bool bIs = false;

		ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel /*ImGuiColorEditFlags_PickerHueBar*/);
	
		static int tabSelected = 0;
		
		
		ImVec2 Pos;
		ImGui::SetNextWindowSize(ImVec2(890.f, 630.f));
		
		if (ImGui::Begin("", &bIs, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))	
		{
			Render->DrawFillBox(0, 0, iScreenWidth, iScreenHeight, Color(0, 0, 0, 170));

			Pos = ImGui::GetWindowPos();
			Gui->MainFont1();
				
			


			const char* gloves_listbox_items[49] =
			{
				"default",
				"Sport Gloves | Superconductor",
				"Sport Gloves | Pandora Box",
				"Sport Gloves | Hedge Maze",
				"Sport Gloves | Arid",
				"Sport Gloves | Vice",
				"Sport Gloves | Omega",
				"Sport Gloves | Bronze Morph",
				"Sport Gloves | Amphibious",
				"Moto Gloves | Eclipse",
				"Moto Gloves | Spearmint",
				"Moto Gloves | Boom",
				"Moto Gloves | Cool Mint",
				"Moto Gloves | Polygon",
				"Moto Gloves | Transport",
				"Moto Gloves | Turtle",
				"Moto Gloves | Pow",
				"Specialist Gloves | Crimson Kimono",
				"Specialist Gloves | Emerald Web",
				"Specialist Gloves | Foundation",
				"Specialist Gloves | Forest DDPAT",
				"Specialist Gloves | Mogul",
				"Specialist Gloves | Fade",
				"Specialist Gloves | Buckshot",
				"Specialist Gloves | Crimson Web",
				"Driver Gloves | Lunar Weave",
				"Driver Gloves | Convoy",
				"Driver Gloves | Crimson Weave",
				"Driver Gloves | Diamondback",
				"Driver Gloves | Racing Green",
				"Driver Gloves | Overtake",
				"Driver Gloves | Imperial Plad",
				"Driver Gloves | King Snake",
				"Hand Wraps | Leather",
				"Hand Wraps | Spruce DDPAT",
				"Hand Wraps | Badlands",
				"Hand Wraps | Slaughter",
				"Hand Wraps | Aboreal",
				"Hand Wraps | Duct Tape",
				"Hand Wraps | Overprint",
				"Hand Wraps | Cobalt Skulls",
				"Bloodhound Gloves | Charred",
				"Bloodhound Gloves | Snakebite",
				"Bloodhound Gloves | Bronzed",
				"Bloodhound Gloves | Guerrilla",
				"Hydra Gloves | Case Hardened",
				"Hydra Gloves | Rattler",
				"Hydra Gloves | Mangrove",
				"Hydra Gloves | Emerald",
			};

			

		

			

			if (tabSelected == 0) // Legit
			{
				

				ImGui::BeginChild("##Main", ImVec2(230, -1), true);
				{
				//	ImGui::Text("Main");ImGui::Separator();ImGui::Spacing();
				ImGui::Checkbox("Enable LegitBot", &settings.LegitBot.aim_Active); 				
					ImGui::Checkbox("Cheack Jump", &settings.LegitBot.aim_AntiJump);
					ImGui::Checkbox("Check Smoke", &settings.LegitBot.aim_CheckSmoke);
					ImGui::Checkbox("Check Flash", &settings.LegitBot.aim_CheckFlash);
					ImGui::Checkbox("FriendlyFire", &settings.LegitBot.aim_Deathmatch);
					ImGui::Checkbox("Draw Fov", &settings.LegitBot.aim_DrawFov);
					ImGui::Checkbox("Autopistol", &settings.LegitBot.aim_AutoPistol);
					ImGui::Checkbox("Cheack zoom", &settings.LegitBot.onlyiszoom);
					ImGui::Checkbox("Huminize", &settings.LegitBot.Humanize);
					ImGui::Spacing();
					ImGui::PushItemWidth(210.f);
					ImGui::Text("First shot delay:");
					ImGui::SliderInt("##Firstshotdelay", &settings.LegitBot.aim_Delay, 0, 200);
					ImGui::PopItemWidth();
				}ImGui::EndChild();	ImGui::SameLine();

				
				
				ImGui::BeginChild("##Aimbot", ImVec2(460, -1), true);
				{

					const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
					const char* RCSType[] = { "Default", "RCS Horizontal and Vertical" };
					const char* RCSTypes[] = { "Type 1", "Type 2", "Type 3" };
					const char* BestHit[] = { "HitBox" , "Nearest"};					
					const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
					const char* FovType[] = { "Static", "Dynamic" };
					
						ImGui::PushItemWidth(280.f);
						

							ImGui::SliderInt("Fov", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Fov, 1, 89);
							ImGui::SliderInt("Smooth", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Smooth, 1, 30);
							ImGui::Combo("HitPoints", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit, BestHit, IM_ARRAYSIZE(BestHit));
							if(settings.LegitBot.weapon_aim_settings[iWeaponID].aim_BestHit == 0)
							 ImGui::Combo("Hitbox", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Spot, Aimspot, IM_ARRAYSIZE(Aimspot));
							ImGui::Combo("Fov type", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_FovType, FovType, IM_ARRAYSIZE(FovType));

					        ImGui::Combo("RCS Clamp", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsClampType, ClampType, IM_ARRAYSIZE(ClampType));
							ImGui::Combo("RCS Type", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RCSType, RCSType, IM_ARRAYSIZE(RCSType));

							if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RCSType == 0)
							{
								ImGui::Combo("RCS Typical", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes, RCSTypes, IM_ARRAYSIZE(RCSTypes));
								ImGui::SliderInt("RCS", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcs, 0, 100);
							}

							if (settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RCSType == 1)
							{
								ImGui::Combo("RCS Typical", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_RcsTypes2, RCSTypes, IM_ARRAYSIZE(RCSTypes));
								ImGui::SliderInt("RCS X", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsx, 0, 100);
								ImGui::SliderInt("RCS Y", &settings.LegitBot.weapon_aim_settings[iWeaponID].aim_Rcsy, 0, 100);
							}
						
					
					ImGui::PopItemWidth();
				}ImGui::EndChild();	ImGui::SameLine();
				ImGui::BeginChild("##AimbotWeapon", ImVec2(200, -1), false);
				{
					ImGui::ListBox("##Select", &iWeaponID, weapons, IM_ARRAYSIZE(weapons), 27);
				}ImGui::EndChild(); 
			}

			else if (tabSelected == 1) // rage
			{
				ImGui::BeginChild("##MainRage", ImVec2(300, -1),true);
				{
					const char* BodyAimModeOptions[2] = { "Prefer", "Force" };
					const char* Hitboxes[6] = { "Head", "Upper Chest", "Lower Chest", "Pelvis", "Arms", "Legs" };
					const char* BodyAimOptions[5] = { "In Air", "Slow Walk", "High Inaccuracy", "Vulnerable", "Lethal" };
					const char* AutoStopOptions[] = { "Disabled", "Minimum Speed", "Full Stop" };
					ImGui::Checkbox("Enable", &settings.RageBot.aimbot_enabled);
					ImGui::PushItemWidth(130.f);
					ImGui::Combo("Auto stop", &settings.RageBot.autostop_mode, AutoStopOptions, IM_ARRAYSIZE(AutoStopOptions));
					if (settings.RageBot.autostop_mode != 0)
						ImGui::Checkbox("Stop Between Shots", &settings.RageBot.stop_inbetween_shots);
					ImGui::Checkbox("Auto Scope", &settings.RageBot.auto_scope);
					ImGui::Checkbox("Resolver", &settings.RageBot.aimbot_resolver);
					ImGui::Combo("Body-Aim Mode", &settings.RageBot.bodyaim_mode, BodyAimModeOptions, IM_ARRAYSIZE(BodyAimModeOptions));
					ImGui::SliderInt("Baim after shot", &settings.RageBot.bodyaim_shots, 1, 10);
					ImGui::SliderInt("Baim health", &settings.RageBot.bodyaim_health, 1, 101);

					static string value = "";
					if (ImGui::BeginCombo("HitScan", value.c_str()))
					{
						value = "";
						vector<string> vec;
						for (size_t i = 0; i < IM_ARRAYSIZE(Hitboxes); i++)
						{
							ImGui::Selectable(Hitboxes[i], &settings.RageBot.rage_hitscan_hitboxes[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							if (settings.RageBot.rage_hitscan_hitboxes[i])
								vec.push_back(Hitboxes[i]);
						}
						for (size_t i = 0; i < vec.size(); i++)
						{
							if (vec.size() == 1)
								value += vec.at(i);
							else if (!(i == vec.size() - 1))
								value += vec.at(i) + ",";
							else
								value += vec.at(i);

						}
						ImGui::EndCombo();
					}

					static string value1 = "";
					if (ImGui::BeginCombo("Body-Aim", value1.c_str()))
					{
						value1 = "";
						vector<string> vec;
						for (size_t i = 0; i < IM_ARRAYSIZE(BodyAimOptions); i++)
						{
							ImGui::Selectable(BodyAimOptions[i], &settings.RageBot.prefer_bodyaim[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							if (settings.RageBot.prefer_bodyaim[i])
								vec.push_back(BodyAimOptions[i]);
						}
						for (size_t i = 0; i < vec.size(); i++)
						{
							if (vec.size() == 1)
								value1 += vec.at(i);
							else if (!(i == vec.size() - 1))
								value1 += vec.at(i) + ",";
							else
								value1 += vec.at(i);

						}
						ImGui::EndCombo();
					}
					ImGui::PopItemWidth();
				}ImGui::EndChild(); ImGui::SameLine();
				ImGui::BeginChild("##AntiAim", ImVec2(270, -1),true);
				{
					static int antiaim_page = 0;
					ImGui::Checkbox("AntiAim Enable", &settings.RageBot.antiaim_enabled);
					ImGui::Checkbox("FreeStand", &settings.RageBot.freestand);
					if (ImGui::Button("Stand",  ImVec2(50, 60)))	antiaim_page = 0; ImGui::SameLine();
					if (ImGui::Button("Move", ImVec2(50, 60)))antiaim_page = 1; ImGui::SameLine();
					if (ImGui::Button("Air",  ImVec2(50, 60))) antiaim_page = 2;
					const char* DeSyncList[] = { "None", "Static", "Double", "Test" };
					ImGui::Combo("DeSync", &settings.RageBot.desync, DeSyncList, IM_ARRAYSIZE(DeSyncList));
					const char* Yaw[] = { "None",  "Back", "Left", "Right", "Foward", "Slow Spin","Jitter" };
					const char* Pitch[] = { "None" , "Zero" ,"Down" , "Up" };
					if (antiaim_page == 0)
					{

						ImGui::Combo("Pitch", &settings.RageBot.Pitch_stand, Pitch, IM_ARRAYSIZE(Pitch));
						ImGui::Combo("Real", &settings.RageBot.Yaw_stand, Yaw, IM_ARRAYSIZE(Yaw));

					}
					else if (antiaim_page == 1)
					{
						ImGui::Combo("Pitch", &settings.RageBot.Pitch_move, Pitch, IM_ARRAYSIZE(Pitch));
						ImGui::Combo("Real", &settings.RageBot.Yaw_move, Yaw, IM_ARRAYSIZE(Yaw));
					}
					else if (antiaim_page == 2)
					{
						ImGui::Combo("Pitch", &settings.RageBot.Pitch_air, Pitch, IM_ARRAYSIZE(Pitch));
						ImGui::Combo("Real", &settings.RageBot.Yaw_air, Yaw, IM_ARRAYSIZE(Yaw));
					}
				}ImGui::EndChild(); ImGui::SameLine();
				ImGui::BeginChild("##BOXING", ImVec2(300, -1),true);
				{
					ImGui::Text("Scale");
					ImGui::Text("Head");
					ImGui::SliderInt("##Head", &settings.RageBot.aimbot_headpointscale, 1, 100);
					ImGui::Text("Body");
					ImGui::SliderInt("##Body", &settings.RageBot.aimbot_bodypointscale, 1, 100);
					ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
					ImGui::Checkbox("Ignore Limbs if walking", &settings.RageBot.aimbot_ignorelimbs);

					ImGui::Text("HitChance");
					ImGui::Text("Auto");
					ImGui::SliderInt("##AutoHitchance", &settings.RageBot.auto_hitchance, 1, 100);
					ImGui::Text("Scout");
					ImGui::SliderInt("##ScoutHitchance", &settings.RageBot.scout_hitchance, 1, 100);
					ImGui::Text("AWP");
					ImGui::SliderInt("##AWPHitchance", &settings.RageBot.awp_hitchance, 1, 100);
					ImGui::Text("HeavyPistol");
					ImGui::SliderInt("##HeavyPistolHitchance", &settings.RageBot.heavy_pistol_hitchance, 1, 100);
					ImGui::Text("Other");
					ImGui::SliderInt("##OtherHitchance", &settings.RageBot.other_hitchance, 1, 100);
					ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

					ImGui::Text("MinDamage");
					ImGui::Text("Auto");
					ImGui::SliderInt("##AutoMinDamage", &settings.RageBot.auto_mindamage, 1, 100);					
					ImGui::Text("Scout");
					ImGui::SliderInt("##ScoutMinDamage", &settings.RageBot.scout_mindamage, 1, 100);				
					ImGui::Text("AWP");
					ImGui::SliderInt("##AWPMinDamage", &settings.RageBot.awp_mindamage, 1, 100);		
					ImGui::Text("HeavyPistol");
					ImGui::SliderInt("##HeavyPistolMinDamage", &settings.RageBot.heavy_pistol_mindamage, 1, 100);			
					ImGui::Text("Other");
					ImGui::SliderInt("##OtherMinDamage", &settings.RageBot.other_mindamage, 1, 100);
				}ImGui::EndChild();
			}
			else if (tabSelected == 2) // Visuals
			{

				ImGui::BeginChild("##Esp", ImVec2(620, -1), true);
				{
					static int visualpage = 0;

					if (ImGui::Button("Main", ImVec2(120.0f, 30.0f)))					
						visualpage = 0;
					
					ImGui::SameLine();
					if (ImGui::Button("Team", ImVec2(120.0f, 30.0f)))				
						visualpage = 1;
					
					ImGui::SameLine();
					if (ImGui::Button("Enemy", ImVec2(120.0f, 30.0f)))		
						visualpage = 2;
					
					ImGui::SameLine();
					if (ImGui::Button("Me", ImVec2(120.0f, 30.0f)))
						visualpage = 3;

					ImGui::SameLine();
					if (ImGui::Button("Radar", ImVec2(120.0f, 30.0f)))					
						visualpage = 4;
					

					if (visualpage == 0)
					{
						ImGui::Text("Main");
						ImGui::Separator();



						ImGui::Checkbox("Esp Wall", &settings.Visual.esp_Visible);
						ImGui::Checkbox("NoFlash", &settings.Misc.misc_NoFlash);
						ImGui::Checkbox("NoScope", &settings.Visual.noscope);
						ImGui::Checkbox("NoSmoke", &settings.Misc.misc_NoSmoke);
						ImGui::Checkbox("Damage Indicator", &settings.Visual.DamageIndicator);
						ImGui::Checkbox("Grenade Prediction", &settings.Visual.esp_GrenadePrediction);
						ImGui::Checkbox("Bomb Player", &settings.Visual.esp_Bomb);
						ImGui::Checkbox("Esp Defusing", &settings.Visual.esp_Defusing);
						ImGui::Checkbox("Bomb Planted", &settings.Visual.esp_BombPlanted);
				
						ImGui::Text("Third Person key:"); ImGui::SameLine();
						ImGui::Hotkey("##Thirdperson", &settings.Visual.thirdperson, ImVec2(55, 20));

						ImGui::PushItemWidth(130.f);
						ImGui::SliderFloat("Third distance", &settings.Visual.thirdperson_dist, 1, 150);
						ImGui::Checkbox("FOV Changer", &settings.Misc.misc_FovChanger);
						if (settings.Misc.misc_FovChanger)
						{
							ImGui::Text("View fov:");
							ImGui::SliderInt("", &settings.Misc.misc_FovView, 1, 170);
							ImGui::Text("Model fov:");
							ImGui::SliderInt(" ", &settings.Misc.misc_FovModelView, 1, 190);

						}
						ImGui::PopItemWidth();
					}
					else if (visualpage == 1)
					{

						ImGui::Checkbox("Team visible", &settings.Visual.esp_Team);

						const char* items1[] = { "None" , "Box" , "Frame" , "Genuine" };
						const char* items3[] = { "None" , "Number" , "Bottom" , "Left" , "Edgy" };
						const char* items4[] = { "None" , "Number" ,"Bottom" , "Right" , "Edgy" };
						const char* items5[] = { "None" , "Flat" , "Warframe" };

						ImGui::PushItemWidth(130.f);
						ImGui::Combo("Health", &settings.Visual.CHvisual[1].esp_Health, items3, IM_ARRAYSIZE(items3));
						ImGui::Combo("Armor", &settings.Visual.CHvisual[1].esp_Armor, items4, IM_ARRAYSIZE(items4));
						ImGui::Combo("Box Type", &settings.Visual.CHvisual[1].esp_Style, items1, IM_ARRAYSIZE(items1));
						ImGui::Combo("Chams", &settings.Visual.CHvisual[1].esp_Chams, items5, IM_ARRAYSIZE(items5));
						ImGui::PopItemWidth();

						ImGui::Checkbox("Name", &settings.Visual.CHvisual[1].esp_Name);
						ImGui::Checkbox("Skeleton", &settings.Visual.CHvisual[1].esp_Skeleton);
						ImGui::Checkbox("Esp Scoped", &settings.Visual.CHvisual[1].esp_Scoped);
						ImGui::Checkbox("Esp Flashed", &settings.Visual.CHvisual[1].esp_Flashed);
					}
					else if (visualpage == 2)
					{
						changvis = 2;
						ImGui::Checkbox("Enemy visible", &settings.Visual.esp_Enemy);

						const char* items1[] = { "None" , "Box" , "Frame" , "Genuine" };
						const char* items3[] = { "None" , "Number" , "Bottom" , "Left" , "Edgy" };
						const char* items4[] = { "None" , "Number" ,"Bottom" , "Right" , "Edgy" };
						const char* items5[] = { "None" , "Flat" , "Warframe" };

						ImGui::PushItemWidth(130.f);

						ImGui::Combo("Health", &settings.Visual.CHvisual[2].esp_Health, items3, IM_ARRAYSIZE(items3));
						ImGui::Combo("Armor", &settings.Visual.CHvisual[2].esp_Armor, items4, IM_ARRAYSIZE(items4));
						ImGui::Combo("Box Type", &settings.Visual.CHvisual[2].esp_Style, items1, IM_ARRAYSIZE(items1));
						ImGui::Combo("Chams", &settings.Visual.CHvisual[2].esp_Chams, items5, IM_ARRAYSIZE(items5));
						ImGui::PopItemWidth();

						ImGui::Checkbox("Name", &settings.Visual.CHvisual[2].esp_Name);
						ImGui::Checkbox("Skeleton", &settings.Visual.CHvisual[2].esp_Skeleton);
						ImGui::Checkbox("Esp Scoped", &settings.Visual.CHvisual[2].esp_Scoped);
						ImGui::Checkbox("Esp Flashed", &settings.Visual.CHvisual[2].esp_Flashed);
					}
					else if (visualpage == 3)
					{
						ImGui::SliderFloat("Blend person", &settings.Visual.blend, 0.f, 1.f);
						ImGui::Checkbox("Blend person on scope", &settings.Visual.blendonscope);
					}

					else if (visualpage == 4)
					{
						ImGui::Text("Radar");

						ImGui::Checkbox("Active", &settings.Radar.rad_Active);

						ImGui::Checkbox("InGame", &settings.Radar.rad_InGame);
						if (settings.Radar.rad_InGame)
						{

							ImGui::Hotkey("InGame Key", &settings.Radar.rad_InGameKey, ImVec2(50, 20));
						}
						ImGui::Checkbox("Show Team", &settings.Radar.rad_Team);

						ImGui::Checkbox("Show Enemy", &settings.Radar.rad_Enemy);
						ImGui::PushItemWidth(150.f);
						ImGui::Text("Range:");
						ImGui::SliderInt("", &settings.Radar.rad_Range, 1, 5000);
						ImGui::Text("Alpha:");
						ImGui::SliderInt(" ", &settings.Radar.rad_Alpha, 1, 255);
						ImGui::PopItemWidth();

					}

				}ImGui::EndChild(); ImGui::SameLine();

			}
			
			else if (tabSelected == 3) // Skins
			{
			
			
				ImGui::BeginChild("##knife&glove", ImVec2(435, -1), true);
				{				
					static int otherpages22 = 0;
					static int otherpages2 = 0;
					if (ImGui::Button("Knife",  ImVec2(60.0f, 40.0f)))
						otherpages2 = 0;

					ImGui::SameLine();

					if (ImGui::Button("Glove",  ImVec2(60.0f, 40.0f)))					
						otherpages2 = 1;
					
					
					

					if (otherpages2 == 1)
					{
						//ImGui::Text("Glove changer");
						//if(ImGui::ListBox("", &settings.SkinChanger.gloves_skin, gloves_listbox_items, IM_ARRAYSIZE(gloves_listbox_items), 24))
						//	weapon->ForceFullUpdate();
					}
					else if (otherpages2 == 0)
					{
						//ImGui::Combo(("##0"), &settings.SkinChanger.knife, "Bayonet\0Flip\0Gut\0Karambit\0M9Bayonet\0Falchion\0Bowie\0Butterfly\0Tactical\0Push\0Jack\0Ursus\0Stiletto\0Widowmaker\0\0", -1);
						//if(ImGui::Button("Update"))
						//	weapon->ForceFullUpdate();
					}
					
					
				}ImGui::EndChild();	ImGui::SameLine();
				ImGui::BeginChild("##weapon", ImVec2(435, -1), true);
				{


					

				}ImGui::EndChild();
			}

			else if (tabSelected == 4) // Misc
			{
				ImGui::BeginChild("##Misc", ImVec2(431, -1), true);
				{
					const char* crosshair[] = { "None" , "Punch" , "Recoil", "Sniper Dot" };
					ImGui::Text("MISC");
					ImGui::Separator;
					//ImGui::Checkbox("Night Mode", &settings.Visual.esp_NightMode);
					ImGui::Checkbox("Rank", &settings.Visual.esp_Rank);
					ImGui::Hotkey("FakeWalk", &settings.RageBot.fakewalk, ImVec2(50,20));
					ImGui::Text("FakeWalk Speed:");
					ImGui::SliderInt("##FakeWalkSpeed", &settings.RageBot.walkspeed,0,100);					
					ImGui::Checkbox("Infinity duck", &settings.Misc.misc_Duck);
					ImGui::Checkbox("Bhop", &settings.Misc.misc_Bhop);

					if (settings.Misc.misc_Bhop)	
						ImGui::Checkbox("AutoStrafe", &settings.Misc.misc_AutoStrafe);

					ImGui::Checkbox("Spectators", &settings.Misc.misc_Spectators);
					ImGui::Checkbox("Anti-AFK", &settings.Misc.misc_antiafk);
					ImGui::PushItemWidth(130.f);
					ImGui::Combo("Crosshairs", &settings.Misc.misc_Punch, crosshair, IM_ARRAYSIZE(crosshair));
				}ImGui::EndChild();
			}
			else if (tabSelected == 5) // color
			{
					ImGui::BeginChild("##Color", ImVec2(800, -1), true);
					{
						const char* espcolornames[] = { "Dot Color","Color CT", "Color visible CT","Color TT", "Color visible TT",
							"Esp Color Team","Esp Vis Team","Chams Team","Chams Vis Team", "Esp Color Enemy","Esp Vis Enemy", "Chams Enemy", "Chams Vis Enemy", "Weapon Enemy", "Bullet tracer", "DamagerColor"};

						static int selected = 0;

						ImGui::Columns(2, nullptr, true);
						ImGui::PushItemWidth(-1);
						ImGui::ListBox("", &selected, espcolornames, IM_ARRAYSIZE(espcolornames), 20);
						ImGui::NextColumn();
						static bool done = false;
						if (!done) {
							ImGui::SetColumnOffset(1, 250);
							done = true;
						}
						switch (selected) {
						case 0:
							ImGui::ColorPicker3("Dot Color", settings.Misc.misc_AwpAimColor);
							break;
						case 1:
							ImGui::ColorPicker3("Color CT", settings.Radar.rad_Color_CT);
							break;
						case 2:
							ImGui::ColorPicker3("Color visible CT", settings.Radar.rad_Color_VCT);
							break;
						case 3:
							ImGui::ColorPicker3("Color TT", settings.Radar.rad_Color_TT);
							break;
						case 4:
							ImGui::ColorPicker3("Color visible TT", settings.Radar.rad_Color_VTT);
							break;
						case 5:
							ImGui::ColorPicker3("Esp Color Team", settings.Visual.esp_Color_Team);
							break;
						case 6:
							ImGui::ColorPicker3("Esp Vis Team", settings.Visual.esp_Color_VTeam);
							break;
						case 7:
							ImGui::ColorPicker3("Chams Team", settings.Visual.chams_Color_Team);
							break;
						case 8:
							ImGui::ColorPicker3("Chams Vis Team", settings.Visual.chams_Color_VTeam);
							break;
						case 9:
							ImGui::ColorPicker3("Esp Color Enemy", settings.Visual.esp_Color_Enemy);
							break;
						case 10:
							ImGui::ColorPicker3("Esp Vis Enemy", settings.Visual.esp_Color_VEnemy);
							break;
						case 11:
							ImGui::ColorPicker3("Chams Enemy", settings.Visual.chams_Color_Enemy);
							break;
						case 12:
							ImGui::ColorPicker3("Chams Vis Enemy", settings.Visual.chams_Color_VEnemy);
							break;
						case 13:
							ImGui::ColorPicker3("Weapon Enemy", settings.Visual.WeaponEnemy_Color);
							break;
						case 14:
							ImGui::ColorPicker3("Bullet tracer", settings.Visual.flTracer_Beam);
							break;
						
						case 15:
							ImGui::ColorPicker3("DamagerColor", settings.Visual.DamagerColor);
							break;
								
								
						}
						ImGui::Columns(1, nullptr, false);


					
					}ImGui::EndChild(); ImGui::SameLine();
					
				
			}
			else if (tabSelected == 6) // config
			{
				ImGui::BeginChild("##Config", ImVec2(431, 400), true);
				{
					ImGui::Text("CONFIG");
					ImGui::Separator;
					ImGui::PushItemWidth(386);

					static int iConfigSelect = 0;
				
					static char ConfigName[64] = { 0 };

					
					//ImGui::ComboBoxArray("Config", &iConfigSelect, ConfigList);


					ImGui::ListBoxConfigArray("##Select", &iConfigSelect, ConfigList);
					ImGui::InputText("##Config Name", ConfigName, 64);

					if (iConfigSelect > 0)
					{
						if (ImGui::Button("Load", ImVec2(125, 0)))
						{
							CBaseWeapon* weapon;
							LoadSettings("C:\\SQUIRT\\" + ConfigList[iConfigSelect]);//
							weapon->ForceFullUpdate();
						}ImGui::SameLine();
						if (ImGui::Button("Save", ImVec2(125, 0)))
						{
							if (iConfigSelect >= 0)
								SaveSettings("C:\\SQUIRT\\" + ConfigList[iConfigSelect]);
							else
								SaveSettings("C:\\SQUIRT\\clearcfg");
						}
						ImGui::SameLine();
						if (ImGui::Button("Delete", ImVec2(125, 0)))
						{
							remove(string("C:\\SQUIRT\\" + ConfigList[iConfigSelect]).c_str());

						}
					}

					if (ImGui::Button("Create", ImVec2(375, 0)))
					{
						string ConfigFileName = ConfigName;

						if (ConfigFileName.size() < 1)
						{
							ConfigFileName = "default";
						}

						SaveSettings("C:\\SQUIRT\\" + ConfigFileName);

					}



					RefreshConfigs();

				}ImGui::EndChild(); ImGui::SameLine();

			}	ImGui::End();

			
			
			//ImGui::SetNextWindowFocus();		
			ImGui::SetNextWindowPos(ImVec2(Pos.x - 185.f, Pos.y));
			ImGui::SetNextWindowSize(ImVec2(150.f, 580.f));
			if (ImGui::Begin("##LeftBar", &bIs, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
			{
			

				static int pzd[6];

				Gui->TitileFont();

				 ImGui::Text("SQUIRT");

			  Gui->MainFontBig();
 
				ImGui::Spacing();

				/*------------------------------------------------------------------------*/
				if (tabSelected == 0 || pzd[0] == 1)
				{
					if (ImGui::Button("Legit", ImVec2(140.f, 50.f)))
						tabSelected = 0;
				}
		
				else if (ImGui::Button("L", ImVec2(70.f, 50.f)))
					tabSelected = 0; if (ImGui::IsItemHovered()) pzd[0] = 1; else pzd[0] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
		
				if (tabSelected == 1 || pzd[1] == 1)
				{
					if (ImGui::Button("Rage", ImVec2(140.f, 50.f)))
						tabSelected = 1;
				}
		
				else if (ImGui::Button("R",  ImVec2(70.f, 50.f)))
					tabSelected = 1; if (ImGui::IsItemHovered()) pzd[1] = 1; else pzd[1] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				if (tabSelected == 2 || pzd[2] == 1)
				{
					if (ImGui::Button("Visual",  ImVec2(140.f, 50.f)))
						tabSelected = 2;
				}
		
				else if (ImGui::Button("V",  ImVec2(70.f, 50.f)))
					tabSelected = 2; if (ImGui::IsItemHovered()) pzd[2] = 1; else pzd[2] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				if (tabSelected == 3 || pzd[3] == 1)
				{
					if (ImGui::Button("Skin",  ImVec2(140.f, 50.f)))
						tabSelected = 3;
				}
		
				else if (ImGui::Button("S",  ImVec2(70.f, 50.f)))
					tabSelected = 3; if (ImGui::IsItemHovered()) pzd[3] = 1; else pzd[3] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				if (tabSelected == 4 || pzd[4] == 1)
				{
					if (ImGui::Button("Misc",  ImVec2(140.f, 50.f)))
						tabSelected = 4;
				}
		
				else if (ImGui::Button("M",  ImVec2(70.f, 50.f)))
					tabSelected = 4; if (ImGui::IsItemHovered()) pzd[4] = 1; else pzd[4] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				if (tabSelected == 5 || pzd[5] == 1)
				{
					if (ImGui::Button("Color",  ImVec2(140.f, 50.f)))
						tabSelected = 5;
				}
		
				else if (ImGui::Button("C",  ImVec2(70.f, 50.f)))
					tabSelected = 5; if (ImGui::IsItemHovered()) pzd[5] = 1; else pzd[5] = 0;
		
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				/*
				if (tabSelected == 6 || pzd[6] == 1)
				{
					if (ImGui::Button("Settings",  ImVec2(140.f, 50.f)))
						tabSelected = 6;
				}
		
				else if (ImGui::Button("S", ImVec2(70.f, 50.f)))
					tabSelected = 6; if (ImGui::IsItemHovered()) pzd[6] = 1; else pzd[6] = 0;
		*/
				ImGui::Spacing();
				/*------------------------------------------------------------------------*/
				
				
				ImGui::Separator();
				ImGui::Spacing();
				Gui->MainFont1();
				ImGui::PushItemWidth(100);
				static int iConfigSelect = 0;
				static char ConfigName[64] = { 0 };
				ImGui::Text("Config:");
				ImGui::ComboBoxArray("##Config", &iConfigSelect, ConfigList);
				ImGui::InputText("##Config Name", ConfigName, 64);

			
					if (ImGui::Button("Load", ImVec2(120, 0)))
					{
						CBaseWeapon* weapon;
						LoadSettings("C:\\SQUIRT\\" + ConfigList[iConfigSelect]);//
						weapon->ForceFullUpdate();
					}
					if (ImGui::Button("Save", ImVec2(120, 0)))
					{
						if (iConfigSelect >= 0)
							SaveSettings("C:\\SQUIRT\\" + ConfigList[iConfigSelect]);
						else
							SaveSettings("C:\\SQUIRT\\clearcfg");
					}
					
					if (ImGui::Button("Delete", ImVec2(120, 0)))
						remove(string("C:\\SQUIRT\\" + ConfigList[iConfigSelect]).c_str());

				if (ImGui::Button("Create", ImVec2(120, 0)))
				{
					string ConfigFileName = ConfigName;
					if (ConfigFileName.size() < 1)
						ConfigFileName = "default";
					SaveSettings("C:\\SQUIRT\\" + ConfigFileName);
				}
				RefreshConfigs();

			}ImGui::End(); 
		}

		
	}
	CClient* Client = new CClient;
	