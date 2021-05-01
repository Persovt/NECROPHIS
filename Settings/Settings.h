#pragma once

#include "../Main/Main.h"

#define FIX_MIN_FOV_HEAD	30
#define FOV_BASE_DISTANCE	30

#define HACK_NAME "Necrophis"
#define RAGEBOT_TEXT "!"
#define AIMBOT_TEXT "'"
#define CONFIG_TEXT "*"
#define VISUAL_TEXT "$"
#define COLOR_TEXT "&"
#define RADAR_TEXT "Radar"
#define SKIN_TEXT "#"
#define SKIN_TEXT_SEED "Seed"
#define MISC_TEXT "%"



void LoadSettings(string szIniFile);
void SaveSettings(string szIniFile);

struct k_weapon_data {
	int itemDefinitionIndex;
	int paintKit;
	int rarity;
	int seed;
	float wear;
	char* name = "";
};

class CSettings
{
public:
	/*RageBot*/
	struct 
	{
		bool freestand;
		int Pitch_stand;
		int Pitch_move;
		int Pitch_air;
		int Yaw_stand;
		int Yaw_move;
		int Yaw_air;
		int desync;
		bool antiaim_enabled;
		bool aimbot_enabled = false;
		 bool rage_hitscan_hitboxes[6];
		bool prefer_bodyaim[6] = { false, false, false, false, false };
		int bodyaim_mode = 0;
		int bodyaim_health = 0;
		int bodyaim_shots = 0;
		bool aimbot_norecoil = false;
		bool aimbot_silentaim = false;
		bool aimbot_ignorelimbs = false;
		int aimbot_headpointscale = 0;
		int aimbot_bodypointscale = 0;
		int aimbot_fov = 0;
		bool aimbot_resolver;
		int aimbot_hitbox;

		bool autorevolver = false;
		bool stop_inbetween_shots = false;
		int autostop_mode = 0;
		bool auto_scope = false;
		bool auto_crouch = false;
		bool accuracy_boost = false;
		bool fakelag_prediction = false;

		int auto_mindamage = 1;
		int auto_hitchance = 0;

		int scout_mindamage = 1;
		int scout_hitchance = 0;

		int awp_mindamage = 0;
		int awp_hitchance = 0;

		int heavy_pistol_mindamage = 1;
		int heavy_pistol_hitchance = 0;

		int other_mindamage = 1;
		int other_hitchance = 0;

		bool prefer_bodyaim_if_low_velocity = false;
		bool smart_bodyaim = false;

		int antiaim_freestanding_mode = 0;
		bool extrapolated = false;
		
		
		int on_shot_aa_type = 0;
		int fakelag = 0;
		int fakeduck_test = 0;
		int variance = 0;
		bool choke_shot = false;
		int walkspeed = 0;
		int fakewalk;

	}RageBot;
	/*RageBot*/

	/*Visual*/
	struct 
	{
		bool fakeangle;
		 bool b_IsThirdPerson;
		 bool disable_tp_on_nade;
		 float thirdperson_dist = 50.f;
		 int thirdperson;
		 bool thirdperson_tp;
		 int RenderTypeBox;
		 bool Watermark;
		 bool esp_AAIndicator = false;
		 bool DamageIndicator;
		
		 int esp_Size;
		 bool esp_Line;
		
	      bool tp_bool;
		
		  struct CHVisual
		  {
			  int esp_Style;
			  int esp_Health;
			  int esp_Armor;
			  bool esp_Scoped;
			  bool esp_Flashed;
			  bool esp_Name;
			  bool esp_Skeleton;
			 

			  int esp_Chams;
		
		  };
	
		 bool esp_Rank;
		
		 int esp_Chams_Light;
		 bool esp_Weapon;
		 int esp_WeaponIcon;
		 bool esp_Ammo;
		 bool esp_Distance;
		 int thirdperson_int;
		 float esp_Dlight[3];
		 bool esp_NightMode;
		 bool esp_Infoz;
		 bool esp_Defusing;
		
		 bool esp_GrenadePrediction;
		 
		 int esp_BulletTrace;
		 bool esp_Team;
		 bool esp_Enemy;

		 bool esp_Visible;

		 bool esp_ChamsVisible;

		
		 bool esp_Bomb;
		 bool esp_BombPlanted;
		 int esp_BombTimer = 40;
		 bool esp_WorldWeapons;
		 bool esp_WorldGrenade;
		 float WeaponEnemy_Color[3] = { 0.f,0.0f,0.f };
		 bool Visuals_WorldWeapons;
		 bool DrawZeusDist;
		 float WorldWeapon_Color[3] = { 0.f,0.0f,0.f };
		 float Visuals_Line_Color[3] = { 0.f,0.0f,0.f };
		 float Name_Color[3] = { 0.f,0.0f,0.f };
		 float DamagerColor[3];
		 bool esp_me;

		 float esp_Color_Team[3];
		 float esp_Color_Enemy[3];
		 float esp_Color_VTeam[3];
		 float esp_Color_VEnemy[3];
		 bool noscope;
		 bool helper;

		 float blend = 1.f;
		 bool blendonscope;
		 float chams_Color_Team[3] = { 0.f,0.0f,0.f };
		 float chams_Color_Enemy[3] = { 0.f,0.0f,0.f };
		 float chams_Color_VTeam[3] = { 0.f,0.0f,0.f };
		 float chams_Color_VEnemy[3] = { 0.f,0.0f,0.f };
		 float flTracer_Beam[3] = { 200.f,0.0f,255.f };
		 bool esp_beams_tracer;
		 float flTracersDuration;
		 float flTracersWidth;

		 bool desyncangle;
		 bool fakecangle;

		 bool esp_hitevent;

		 CHVisual CHvisual[3];
	}Visual;
	/*Visual*/
	
	/*LegitBot*/
	struct
	{
		bool Humanize;
		int BuletRcs = 1;
		 bool automaticscope;
		 bool aim_DrawFov;
		 bool aim_Backtrack;
		 bool aim_DrawBacktrack;
		 int aim_Backtracktickrate;
		 bool onlyiszoom;
		 int aim_Delay;

		struct weapon_aim_s
		{
			bool aim_Active = false;
			int aim_Smooth = 1;
			int aim_Fov = 1;
			int aim_FovType = 0;
			int aim_RCSType = 0;
			int aim_BestHit;
			int aim_Spot = 0;
			int aim_Rcs = 0;
			int aim_RcsTypes = 0;
			int aim_RcsTypes2 = 0;
			int aim_RcsAfterXShot;
			int aim_Rcsx;
			int aim_Rcsy;
			int aim_RcsFov;
			int aim_RcsSmooth;
			int aim_RcsClampType;		
		};

		 bool aim_AutoPistol;
		 bool aim_Active;
		 bool aim_Deathmatch;
		 bool aim_WallAttack;
		 bool aim_CheckSmoke;
		 bool aim_CheckFlash;
		 bool aim_AntiJump;
		 int aim_RcsType;
		 bool aim_DrawSpot;
		 bool aim_KillDelay;
		 int aim_KillDelayTime;
		 bool aim_OnKey;
		 int aim_Key;
		 float aim_FovColor[3];

		 weapon_aim_s weapon_aim_settings[5];

	}LegitBot;
	/*LegitBot*/

	/*Color*/
	struct
	{
		float Skelet[3] = { 0.f,0.0f,0.f };
		float Fraggrenade_Color[3] = { 0.f,0.0f,0.f };
		float Flashbang_Color[3] = { 0.f,0.0f,0.f };
		float Smokegrenade_Color[3] = { 0.f,0.0f,0.f };
		float SoundVisuals_Color[3] = { 0.f,0.0f,0.f };

		float Visuals_Line_Color[3] = { 0.f,0.0f,0.f };
		float Name_Color[3] = { 0.f,0.0f,0.f };
		float Health_Color[3] = { 0.f,0.0f,0.f };
		float Armor_Color[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_CT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_TT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_VCT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_VTT[3] = { 0.f,0.0f,0.f };
		float chams_Color_CT[3] = { 0.f,0.0f,0.f };
		float chams_Color_TT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VCT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VTT[3] = { 0.f,0.0f,0.f };
	}Color;
	/*Color*/

	/*Misc*/
	struct
	{

		int theme;
		QAngle qLastTickAngle;
		bool misc_ThirdPerson = false;
		float misc_ThirdPersonRange = 90.f;
		bool custommodelson = false;
		int clantage;
		int chatspam;

		int customodelst;
		int AK47Models;
		int AWPModels;
		int GlockModels;
		int M4A4Models;
		int customodelsct;
		int customweaponmodelknife;
		bool misc_ChamsMaterials = false;
		int misc_ChamsMaterialsList = 0;
		bool misc_ArmMaterials = false;
		int misc_ArmMaterialsList = 0;
		int misc_ArmMaterialsType = 0;
		bool anim2 = false;
		bool ClanTag = false;
		bool Other_AutoAccept = false;
		bool ShowSpread = 0;
		bool lang = false;
		const char* misc_SkyName;
		//bool black = false;
		bool Blackout = true;
		bool misc_antiafk = false;
		int themes = 0;
		//bool white = false;
		int misc_CurrentSky;
		bool misc_NoSky = false;
		bool misc_Snow = false;
		bool misc_Duck = false;

		bool misc_Postprocess = false;
		bool misc_Bhop = false;
		bool misc_EdgeJump = false;
		bool misc_LeftHandKnife = false;
		bool misc_RightHandKnife = false;
		int misc_Punch;
		bool misc_Punch2 = false;
		bool misc_NoFlash = false;
		bool misc_NoSmoke = false;
		bool misc_AwpAim = false;
		bool misc_AutoStrafe = false;
		bool misc_Spectators = false;
		bool misc_FovChanger = false;
		int misc_FovView = 90;
		int misc_FovModelView = 68;
		float misc_AwpAimColor[3] = { 0.f,0.f,0.f };
		float DamagerColor[3] = { 0.f,0.f,0.f };

		float hitmarkerAlpha;

		int KillDelayVar = 0;
		bool KillDelayBool2 = false;
		bool misc_HitMarker = false;

		
		float misc_HitMarkerColor[3] = { 0.f, 0.f, 0.f };
		bool skinids = false;
		bool medalids = false;
		bool weaponids = false;

	}Misc;
	/*Misc*/

	/*SkinChanger*/
	struct
	{
		int knife;
		bool EnabledGlove;
		bool EnabledKnife;
		int GloveType = -1;
		short current_weapon = 0;

		int knf_ct_model = 0;
		int knf_ct_skin = 0;
		int knf_tt_model = 0;
		int knf_tt_skin = 0;
		int gloves_skin = 0;

		class cWeapon_skin
		{
		public:
			int  m_nFallbackPaintKit = 0,
				m_nFallbackSeed = 0,
				m_nFallbackStatTrak = 0;

			float m_flFallbackWear = 0.f;

			const char* custom_name = "SQRT";

			bool enable_changer = false;

		}weapon[524];

	}SkinChanger;
	/*SkinChanger*/
	
	/*Radar*/
	struct
	{
		bool Radar_Enable = false;
		bool rad_Active = false;
		bool rad_Team = false;
		bool rad_Enemy = false;
		bool rad_InGame = false;
		int rad_InGameKey = 0;
		int rad_Range = 3500;
		int rad_Alpha = 255;
		float rad_Color_CT[3] = { 0.f,0.0f,0.f };
		float rad_Color_TT[3] = { 0.f,0.0f,0.f };
		float rad_Color_VCT[3] = { 0.f,0.f,0.f };
		float rad_Color_VTT[3] = { 0.f,0.f,0.f };
	}Radar;
	/*Radar*/

	/*InventoryChanger*/
	struct
	{
		bool enabled = false;
		std::vector<k_weapon_data> weapons = {};
	}InventoryChanger;
	/*InventoryChanger*/

	/*ProfileChanger*/
	struct
	{
		bool enabled = false;
		int rank_id = 0;
		int wins = 0;
		int cmd_friendly = 0;
		int cmd_leader = 0;
		int cmd_teaching = 0;
		int level = 0;
		int xp = 0;
		int ban = 0;
		int time = 0;
	}ProfileChanger;
	/*ProfileChanger*/

	/*MedalChanger*/
	struct
	{
		bool enabled = false;
		
		std::vector<uint32_t> medals = {};
		bool equipped_medal_override = false;
		uint32_t equipped_medal = 0;
	}MedalChanger;
	/*MedalChanger*/

	float hitmarkerAlpha;
	bool fakefix_bool;

	

}; extern CSettings settings;

