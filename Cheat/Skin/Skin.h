#pragma once

#include <unordered_map>

#include "../../Engine/Engine.h"

#include "DynSkin.h"


#include <xlocbuf>
#include <deque>
#include <codecvt>
#include <algorithm>
namespace Engine
{
	class CBaseAttributableItem;
	class CBaseEntity;
}

using namespace Engine;

struct EconomyItemCfg
{
	bool LiveStatTrack = false;
	int iItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int iEntityQuality = 4;
	int nFallbackStatTrak = -1;
	float flFallbackWear = 0.00000001f;
	int nFallbackSeed = 0;
};

struct WeaponSkins_s
{
	vector<int> SkinPaintKit;
	vector<string> SkinNames;
};

struct KnifeSkins_s
{
	vector<int> SkinPaintKit;
	vector<string> SkinNames;
};

struct GlovesSkins_s
{
	int ItemIndex;
	int PaintKit;
	const char* szModel;
};
struct Gloves_s
{
	vector<int> PaintKit;
	vector<string> Names;
};

extern WeaponSkins_s WeaponSkins[34];
extern KnifeSkins_s KnifeSkins[14];
extern GlovesSkins_s GlovesSkin_Array[49];
extern Gloves_s GloveSkin[10];
extern unordered_map<int, EconomyItemCfg> g_SkinChangerCfg;
extern unordered_map<int, const char*> g_ViewModelCfg;
extern unordered_map<const char*, const char*> g_KillIconCfg;

extern char* pWeaponData[34];
extern int pWeaponItemIndexData[34];
extern char* pKnifeData[14];

#define WEAPON_DATA_SIZE ( sizeof( pWeaponData ) / sizeof( *pWeaponData ) )
#define KNIFE_DATA_SIZE ( sizeof( pKnifeData ) / sizeof( *pKnifeData ) )

int GetWeaponSkinIndexFromPaintKit(int iPaintKit);
int GetKnifeSkinIndexFromPaintKit(int iPaintKit, bool tt);

struct GloveInfo
{
	int GloveType;
	const char* Name;
	const char* szModel;
};

const std::deque<GloveInfo> k_glove_names =
{
	{ 0, "Default" },
{ 5027, "Bloodhound", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" },
{ 5028, "Default (Terrorists)", "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" },
{ 5029, "Default (Counter-Terrorists)", "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" },
{ 5030, "Sporty", "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" },
{ 5031, "Slick", "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" },
{ 5032, "Handwrap", "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" },
{ 5033, "Motorcycle", "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" },
{ 5034, "Specialist", "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" },
{ 5035, "Hydra", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl" }
};

class CSkin
{
public:
	
	void OnEvents(IGameEvent* pEvent);

	void SetSkinConfig();
	void SetModelConfig();
	void SetKillIconCfg();

	void InitalizeSkins();

	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool ApplyCustomModel(CBaseEntity* pLocal, CBaseAttributableItem* pWeapon);
	bool ApplyCustomKillIcon(IGameEvent* pEvent);
	
};

void Skin_OnFrameStageNotify(ClientFrameStage_t Stage);
void Gloves_OnFrameStageNotify(ClientFrameStage_t Stage);
void Model_OnFrameStageNotify(ClientFrameStage_t Stage);

void Hook_SetViewModelSequence(const CRecvProxyData *pDataConst, void *pStruct, void *pOut);

// œ»—“ŒÀ≈“€

#define WEAPON_DEAGLE_STR		"deagle"		// 0
#define WEAPON_ELITE_STR		"elite"			// 1
#define WEAPON_FIVESEVEN_STR	"fiveseven"		// 2
#define WEAPON_GLOCK_STR		"glock"			// 3
#define WEAPON_TASER_STR		"taser"
#define WEAPON_HKP2000_STR		"hkp2000"		// 4
#define WEAPON_P250_STR			"p250"			// 5
#define WEAPON_USP_S_STR		"usp_silencer"	// 6
#define WEAPON_CZ75A_STR		"cz75a"			// 7
#define WEAPON_REVOLVER_STR		"revolver"		// 8
#define WEAPON_TEC9_STR			"tec9"			// 9

// ƒ–Œ¡Œ¬» » , ¿¬“ŒÃ¿“€

#define WEAPON_AK47_STR        "ak47"                // 10
#define WEAPON_AUG_STR        "aug"                  // 11
#define WEAPON_FAMAS_STR    "famas"                  // 12
#define WEAPON_GALILAR_STR    "galilar"              // 13
#define WEAPON_M249_STR        "m249"                // 14
#define WEAPON_M4A4_STR        "m4a1"                // 15
#define WEAPON_M4A1_S_STR    "m4a1_silencer"         // 16
#define WEAPON_MAC10_STR    "mac10"                  // 17
#define WEAPON_P90_STR        "p90"                  // 18
#define WEAPON_MP5_STR        "mp5sd"                // 19
#define WEAPON_UMP45_STR    "ump45"                  // 20
#define WEAPON_XM1014_STR    "xm1014"                // 21
#define WEAPON_BIZON_STR    "bizon"                  // 22
#define WEAPON_MAG7_STR        "mag7"                // 23
#define WEAPON_NEGEV_STR    "negev"                  // 24
#define WEAPON_SAWEDOFF_STR    "sawedoff"            // 25
#define WEAPON_MP7_STR        "mp7"                  // 26
#define WEAPON_MP9_STR        "mp9"                  // 27
#define WEAPON_NOVA_STR        "nova"                // 28
#define WEAPON_SG556_STR    "sg556"                  // 29
#define WEAPON_G3SG1_STR    "g3sg1"                  // 30
#define WEAPON_SCAR20_STR    "scar20"                // 31

// —Õ¿…œ≈– »

#define WEAPON_AWP_STR		"awp"				// 32
#define WEAPON_SSG08_STR	"ssg08"				// 33

extern CSkin* Skin;