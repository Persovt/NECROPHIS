#pragma once

#include "../Engine/Engine.h"


namespace Engine
{
	class CCSGOAnimState
	{
	public:

		float m_flSpeedFraction; //0x00F8
		float m_flSpeedFactor; //0x00FC
		float m_flLandingRatio; //0x011C   fd
		float m_fDuckAmount;
		float m_flMaxBodyYawDegrees; //0x0334
		char pad_0x0000[0x14]; //0x0000
		float m_flHowLongIsEntityWalking; //0x0014 counts up if full walk speed is achieved
		char pad_0x0018[0xC]; //0x0018
		float m_flIsWalking; //0x0024 Gets set up to 1.f if the entity is fully walking
		float m_flTimeSinceWalkingWithMovementSpeedGreaterThenWalking; //0x0028 if speed is greater then walking it starts counting up
		float m_flTimeSinceMovementWasGreaterThenWalking; //0x002C Only resets if speed is greater then walking
		char pad_0x0030[0xC]; //0x0030
		float m_flTimeSinceStandingStill; //0x003C Not that agressive only counts up if speed is higher then 50.f +
		float m_flTimeSinceAllMovement; //0x0040 every slight movement will reset it
		char pad_0x0044[0x8]; //0x0044
		float m_flIsRunning; //0x004C Gets set up to 1.f if entity is fully running
		char pad_0x0050[0x14]; //0x0050
		void* pActiveWeapon; //0x0064
		void* pLastActiveWeapon; //0x0068
		float m_flLastClientSideAnimationUpdateTime; //0x006C
		int m_iLastClientSideAnimationUpdateFramecount; //0x0070
		float m_flEyePitch; //0x0074
		float m_flEyeYaw; //0x0078 weird eyeyaw
		float m_flPitch; //0x007C
		float m_flGoalFeetYaw; //0x0080
		float m_flCurrentFeetYaw; //0x0084
		float m_flCurrentTorsoYaw; //0x0088 torso yaw and unknown velocity lean have the same values for some reason
		float m_flUnknownVelocityLean; //0x008C
		float m_flLeanAmmount; //0x0090
		float m_flUnknown1; //0x0094 never gets set dunno
		float m_flFeetCycle; //0x0098
		float m_flYawRate; //0x009C
		float m_flUnknown2; //0x00A0 never gets set dont fucking know what that is
		float m_flDuckAmount; //0x00A4 this also shows the duck amount when youve landed
		float m_flLandingAmount; //0x00A8 the original name was retarded this fits more
		float m_flUnknown3; //0x00AC
		Vector m_vOrigin; //0x00B0
		Vector m_vLastOrigin; //0x00BC
		float m_flVelocityX; //0x00C8
		float m_flVelocityY; //0x00CC
		float m_flUnknown4; //0x00D0
		float m_flUnknownVelocityFloat1; //0x00D4 affected by movement & direction
		float m_flUnknownVelocityFloat2; //0x00D8 affected by movement & direction
		float m_flUnknown5; //0x00DC
		float m_flUnknownVelocityFloat3; //0x00E0 from -1 to 1 when moving and affected by direction
		float m_flUnknownVelocityFloat4; //0x00E4 from -1 to 1 when moving and affected by direction
		float m_flUnknown6; //0x00E8
		float m_flCurrentVelocity; //0x00EC normal speed2d = entity moving speed/velocity
		float m_flFallingAndJumpingVelocity; //0x00F0 falling down and jumping up velocity
		float m_flSpeedNormalized; //0x00F4  0 to 1
		float m_flFeetSpeedForwardsOrSideWays; //0x00F8 from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
		float m_flFeetSpeedUnknownForwardOrSideways; //0x00FC from 0 to 3
		float m_flTimeSinceStartedMoving; //0x0100 valve something similar exists at 0x28 get some structure in here
		float m_flTimeSinceStoppedMoving; //0x0104 same thing as above 0x40 does nearly the same shit ffs
		unsigned char m_bOnGround; //0x0108 instead of making a bool valve decided to make it 1 byte and assign 0 - 1 and sometimes fucking 257 to it
		unsigned char m_bInHitGroundAnimation; //0x0109 dunno why they dont make simple bools but decide to make them 1 byte
		char pad_0x010A[0x6]; //0x010A
		float m_flInAirTime; //0x0110 could be neat to have
		float m_flLastOriginZ; //0x0114
		float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x0118 pretty weird value ngl
		float m_flStopToFullRunningFraction; //0x011C from 0 to 1, doesnt change when walking or crouching, only running
		char pad_0x0120[0x4]; //0x0120
		float m_flUnknownFraction; //0x0124 affected while jumping and running, or when just jumping, 0 to 1
		char pad_0x0128[0xC]; //0x0128
		int m_bIsRunning; //0x0134 similar already exists at 0x4C ..
		char pad_0x0138[0x1F8]; //0x0138
		float m_flMinDesyncValue; //0x0330 I assume those are the values?
		float m_flMaxDesyncValue; //0x0334 I assume those are the values?
		float m_flMinYawUnknown; //0x0338
		float m_flMaxYawUnknown; //0x033C
		char pad_0x0340[0x100]; //0x0340
	};
	class CBaseWeapon;
	class CBaseViewModel;

	class CBaseEntity : public IClientEntity
	{
	public:
		//[swap_lines]
		char*	GetPlayerName();
		Vector        GetEyePos();
		bool	IsPlayer();
		bool	IsValid();
	//	bool	IsValid();
		
		bool	IsDead();
		Vector GetOrigin();
		Vector * GetVAngles();
		float GetNextAttack();
		bool	IsVisible(CBaseEntity* pLocalEntity);
		int		GetMoveType();
		int GetIndex();
		bool	HasHelmet();
		bool	HasDefuser();
		int    IsDefusing();
		int		IsFlashed();
		bool*	IsSpotted();

		int		GetFovStart();
		int		GetFlags();
		int		GetHealth();
		int		GetArmor();
		int		GetTeam();
		float   GetLowerBodyYaw();
		float   GetSimTime();
		float   GetFlashDuration();
		int		GetShotsFired();
		int		GetIsScoped();
		int		GetTickBase();
		float m_hGroundEntity();
		int movetype();
		float m_nWaterLevel();
		ObserverMode_t GetObserverMode();
		PVOID GetObserverTarget();
		int GetActiveWeaponIndex();
		PVOID			GetActiveWeapon();
		CBaseWeapon*	GetBaseWeapon();
		UINT*			GetWeapons();
		UINT*			GetWearables();
		CBaseViewModel* GetViewModel();
		CCSGOAnimState* GetAnimState();
		Vector	GetAimPunchAngle();
		Vector	GetViewPunchAngle();
		int GetBoneByName(const char * boneName);
		float get_max_desync_delta(CBaseEntity * local);
		float & m_flAbsRotation();
		float getmaxdesync();
		model_t * GetModel2();
		int GetHitboxSet_();
		matrix3x4_t GetBoneMatrix(int BoneID);
		Vector * GetEyeAnglesPtr();
		void SetAngle2(Vector wantedang);
		int DrawModel2(int flags, uint8_t alpha);
		CBaseWeapon * GetWeapon();
		Vector	GetVelocity();
		void SetEyeAngles(Vector angles);
		Vector	GetViewOffset();
		int GetSequenceActivity(int sequence);
		Vector	GetEyePosition();
		bool SetupBones2(matrix3x4_t * pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
		QAngle	GetEyeAngles();
		CAnimationLayer & GetAnimOverlay(int Index);
		void SetAbsOrigin(Vector ArgOrigin);
		Vector	GetBonePosition(int nBone);
		Vector	GetHitboxPosition(int nHitbox);

	

		mstudiobone_t* GetBone(int nBone);
		mstudiobbox_t* GetHitBox(int nHitbox);

		mstudiohitboxset_t* GetHitBoxSet();
		//[/swap_lines]

	};
	

	class CBaseViewModel : public IClientEntity
	{
	public:
		//[swap_lines]
		int		GetModelIndex();
		void	SetModelIndex(int nModelIndex);
		void	SetWeaponModel(const char* Filename, IClientEntity* Weapon);

		DWORD	GetOwner();
		DWORD	GetWeapon();
		//[/swap_lines]
	};


	enum PLAYER_TEAM
	{
		TEAM_SPEC = 1,
		TEAM_TT,
		TEAM_CT
	};


	enum class CLIENT_CLASS_ID
	{
		CAI_BaseNPC = 0,

		CAK47,

		CBaseAnimating,

		CBaseAnimatingOverlay,

		CBaseAttributableItem,

		CBaseButton,

		CBaseCombatCharacter,

		CBaseCombatWeapon,

		CBaseCSGrenade,

		CBaseCSGrenadeProjectile,

		CBaseDoor,

		CBaseEntity,

		CBaseFlex,

		CBaseGrenade,

		CBaseParticleEntity,

		CBasePlayer,

		CBasePropDoor,

		CBaseTeamObjectiveResource,

		CBaseTempEntity,

		CBaseToggle,

		CBaseTrigger,

		CBaseViewModel,

		CBaseVPhysicsTrigger,

		CBaseWeaponWorldModel,

		CBeam,

		CBeamSpotlight,

		CBoneFollower,

		CBRC4Target,

		CBreachCharge,

		CBreachChargeProjectile,

		CBreakableProp,

		CBreakableSurface,

		CBumpMine,

		CBumpMineProjectile,

		CC4,

		CCascadeLight,

		CChicken,

		CColorCorrection,

		CColorCorrectionVolume,

		CCSGameRulesProxy,

		CCSPlayer,

		CCSPlayerResource,

		CCSRagdoll,

		CCSTeam,

		CDangerZone,

		CDangerZoneController,

		CDEagle,

		CDecoyGrenade,

		CDecoyProjectile,

		CDrone,

		CDronegun,

		CDynamicLight,

		CDynamicProp,

		CEconEntity,

		CEconWearable,

		CEmbers,

		CEntityDissolve,

		CEntityFlame,

		CEntityFreezing,

		CEntityParticleTrail,

		CEnvAmbientLight,

		CEnvDetailController,

		CEnvDOFController,

		CEnvGasCanister,

		CEnvParticleScript,

		CEnvProjectedTexture,

		CEnvQuadraticBeam,

		CEnvScreenEffect,

		CEnvScreenOverlay,

		CEnvTonemapController,

		CEnvWind,

		CFEPlayerDecal,

		CFireCrackerBlast,

		CFireSmoke,

		CFireTrail,

		CFish,

		CFists,

		CFlashbang,

		CFogController,

		CFootstepControl,

		CFunc_Dust,

		CFunc_LOD,

		CFuncAreaPortalWindow,

		CFuncBrush,

		CFuncConveyor,

		CFuncLadder,

		CFuncMonitor,

		CFuncMoveLinear,

		CFuncOccluder,

		CFuncReflectiveGlass,

		CFuncRotating,

		CFuncSmokeVolume,

		CFuncTrackTrain,

		CGameRulesProxy,

		CGrassBurn,

		CHandleTest,

		CHEGrenade,

		CHostage,

		CHostageCarriableProp,

		CIncendiaryGrenade,

		CInferno,

		CInfoLadderDismount,

		CInfoMapRegion,

		CInfoOverlayAccessor,

		CItem_Healthshot,

		CItemCash,

		CItemDogtags,

		CKnife,

		CKnifeGG,

		CLightGlow,

		CMaterialModifyControl,

		CMelee,

		CMolotovGrenade,

		CMolotovProjectile,

		CMovieDisplay,

		CParadropChopper,

		CParticleFire,

		CParticlePerformanceMonitor,

		CParticleSystem,

		CPhysBox,

		CPhysBoxMultiplayer,

		CPhysicsProp,

		CPhysicsPropMultiplayer,

		CPhysMagnet,

		CPhysPropAmmoBox,

		CPhysPropLootCrate,

		CPhysPropRadarJammer,

		CPhysPropWeaponUpgrade,

		CPlantedC4,

		CPlasma,

		CPlayerPing,

		CPlayerResource,

		CPointCamera,

		CPointCommentaryNode,

		CPointWorldText,

		CPoseController,

		CPostProcessController,

		CPrecipitation,

		CPrecipitationBlocker,

		CPredictedViewModel,

		CProp_Hallucination,

		CPropCounter,

		CPropDoorRotating,

		CPropJeep,

		CPropVehicleDriveable,

		CRagdollManager,

		CRagdollProp,

		CRagdollPropAttached,

		CRopeKeyframe,

		CSCAR17,

		CSceneEntity,

		CSensorGrenade,

		CSensorGrenadeProjectile,

		CShadowControl,

		CSlideshowDisplay,

		CSmokeGrenade,

		CSmokeGrenadeProjectile,

		CSmokeStack,

		CSnowball,

		CSnowballPile,

		CSnowballProjectile,

		CSpatialEntity,

		CSpotlightEnd,

		CSprite,

		CSpriteOriented,

		CSpriteTrail,

		CStatueProp,

		CSteamJet,

		CSun,

		CSunlightShadowControl,

		CSurvivalSpawnChopper,

		CTablet,

		CTeam,

		CTeamplayRoundBasedRulesProxy,

		CTEArmorRicochet,

		CTEBaseBeam,

		CTEBeamEntPoint,

		CTEBeamEnts,

		CTEBeamFollow,

		CTEBeamLaser,

		CTEBeamPoints,

		CTEBeamRing,

		CTEBeamRingPoint,

		CTEBeamSpline,

		CTEBloodSprite,

		CTEBloodStream,

		CTEBreakModel,
		CTEBSPDecal,
		CTEBubbles,
		CTEBubbleTrail,
		CTEClientProjectile,
		CTEDecal,
		CTEDust,
		CTEDynamicLight,
		CTEEffectDispatch,
		CTEEnergySplash,
		CTEExplosion,
		CTEFireBullets,
		CTEFizz,
		CTEFootprintDecal,
		CTEFoundryHelpers,
		CTEGaussExplosion,
		CTEGlowSprite,
		CTEImpact,
		CTEKillPlayerAttachments,
       	CTELargeFunnel,
		CTEMetalSparks,
		CTEMuzzleFlash,
		CTEParticleSystem,
		CTEPhysicsProp,
		CTEPlantBomb,
		CTEPlayerAnimEvent,
		CTEPlayerDecal,
		CTEProjectedDecal,
		CTERadioIcon,
		CTEShatterSurface,
		CTEShowLine,
		CTesla,
		CTESmoke,
		CTESparks,
		CTESprite,
		CTESpriteSpray,
		CTest_ProxyToggle_Networkable,
		CTestTraceline,
		CTEWorldDecal,
		CTriggerPlayerMovement,
		CTriggerSoundOperator,
		CVGuiScreen,
		CVoteController,
		CWaterBullet,
		CWaterLODControl,
		CWeaponAug,
		CWeaponAWP,
		CWeaponBaseItem,
		CWeaponBizon,
		CWeaponCSBase,
		CWeaponCSBaseGun,
      	CWeaponCycler,
		CWeaponElite,
		CWeaponFamas,
		CWeaponFiveSeven,
		CWeaponG3SG1,
		CWeaponGalil,
		CWeaponGalilAR,
		CWeaponGlock,
		CWeaponHKP2000,
		CWeaponM249,
		CWeaponM3,
		CWeaponM4A1,
		CWeaponMAC10,
		CWeaponMag7,
		CWeaponMP5Navy,
		CWeaponMP7,
		CWeaponMP9,
		CWeaponNegev,
		CWeaponNOVA,
		CWeaponP228,
		CWeaponP250,
		CWeaponP90,
		CWeaponSawedoff,
		CWeaponSCAR20,
		CWeaponScout,
		CWeaponSG550,
		CWeaponSG552,
		CWeaponSG556,
		CWeaponShield,
		CWeaponSSG08,
		CWeaponTaser,
		CWeaponTec9,
		CWeaponTMP,
		CWeaponUMP45,
		CWeaponUSP,
		CWeaponXM1014,
		CWorld,
		CWorldVguiText,
		DustTrail,
		MovieExplosion,
		ParticleSmokeGrenade,
		RocketTrail,
		SmokeTrail,
		SporeExplosion,
		SporeTrail,
	};
}
