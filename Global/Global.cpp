#include "Global.h"
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
#define WEAPON_AWP_STR		"awp"				// 32
#define WEAPON_SSG08_STR	"ssg08"				// 33
char* pWeaponData[34] =
{
	// ????????? - 0 - 9
	WEAPON_DEAGLE_STR,
	WEAPON_ELITE_STR,
	WEAPON_FIVESEVEN_STR,
	WEAPON_GLOCK_STR,
	WEAPON_HKP2000_STR,
	WEAPON_P250_STR,
	WEAPON_USP_S_STR,
	WEAPON_CZ75A_STR,
	WEAPON_REVOLVER_STR,
	WEAPON_TEC9_STR,
	// ???????? - 10 - 32
	WEAPON_AK47_STR,
	WEAPON_AUG_STR,
	WEAPON_FAMAS_STR,
	WEAPON_GALILAR_STR,
	WEAPON_M249_STR,
	WEAPON_M4A4_STR,
	WEAPON_M4A1_S_STR,
	WEAPON_MAC10_STR,
	WEAPON_P90_STR,
	WEAPON_MP5_STR,
	WEAPON_UMP45_STR,
	WEAPON_XM1014_STR,
	WEAPON_BIZON_STR,
	WEAPON_MAG7_STR,
	WEAPON_NEGEV_STR,
	WEAPON_SAWEDOFF_STR,
	WEAPON_MP7_STR,
	WEAPON_MP9_STR,
	WEAPON_NOVA_STR,
	WEAPON_SG556_STR,
	WEAPON_SCAR20_STR,
	WEAPON_G3SG1_STR,
	// ?????????  - 33 - 34
	WEAPON_AWP_STR,
	WEAPON_SSG08_STR
};

int pWeaponItemIndexData[34] =
{
	// ????????? - 0 - 9
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_HKP2000,
	WEAPON_P250,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A,
	WEAPON_REVOLVER,
	WEAPON_TEC9,
	// ???????? - 10 - 32
	WEAPON_AK47,
	WEAPON_AUG,
	WEAPON_FAMAS,
	WEAPON_GALILAR,
	WEAPON_M249,
	WEAPON_M4A4,
	WEAPON_M4A1_SILENCER,
	WEAPON_MAC10,
	WEAPON_P90,
	WEAPON_MP5,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_SG553,
	WEAPON_SCAR20,
	WEAPON_G3SG1,
	// ?????????  - 33 - 34
	WEAPON_AWP,
	WEAPON_SSG08
};;

int            MissedShots[65];
bool           Shot[65];
Vector         AimbotHitbox[65][27];
Vector         EnemyEyeAngs[65];
int            TargetIndex = -1;
bool using_fake_angles[65];
bool backtrack_tick[65];
int shots_fired[65];
bool           Hit[65];
int shots_hit[65];
int shots_missed[65];
float update_time[65];
float lby_to_back[65];
int resolve_type[65];
int changvis = 0;
Vector2D	CenterScreens = Vector2D(0.f, 0.f);
int	iWeaponID=0;
int	iScreenWidth = 0;
int	iScreenHeight = 0;
bool bSendPacket;
Vector RealAngles;
bool auth;
CBaseEntity* LocalPlayer ;
CMe* mLocal;