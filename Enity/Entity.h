#pragma once

#include "../Engine/Engine.h"


namespace Engine
{
	class CCSGOAnimState
	{
	public:
		char pad[4];
		char bUnknown; //0x4
		char pad2[91];
		void* pBaseEntity; //0x60
		void* pActiveWeapon; //0x64
		void* pLastActiveWeapon; //0x68
		float m_flLastClientSideAnimationUpdateTime; //0x6C
		int m_iLastClientSideAnimationUpdateFramecount; //0x70
		float m_flEyePitch; //0x74
		float m_flEyeYaw; //0x78
		float m_flPitch; //0x7C
		float m_flGoalFeetYaw; //0x80
		float m_flCurrentFeetYaw; //0x84
		float m_flCurrentTorsoYaw; //0x88
		float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
		float m_flLeanAmount; //0x90
		char pad4[4]; //NaN
		float m_flFeetCycle; //0x98 0 to 1
		float m_flFeetYawRate; //0x9C 0 to 1
		float m_fUnknown2;
		float m_fDuckAmount; //0xA4
		float m_fLandingDuckAdditiveSomething; //0xA8
		float m_fUnknown3; //0xAC
		Vector m_vOrigin; //0xB0, 0xB4, 0xB8
		Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
		float m_vVelocityX; //0xC8
		float m_vVelocityY; //0xCC
		char pad5[4];
		float m_flUnknownFloat1; //0xD4 Affected by movement and direction
		char pad6[8];
		float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
		float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
		float m_unknown; //0xE8
		float speed_2d; //0xEC
		float flUpVelocity; //0xF0
		float m_flSpeedNormalized; //0xF4 //from 0 to 1
		float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
		float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
		float m_flTimeSinceStartedMoving; //0x100
		float m_flTimeSinceStoppedMoving; //0x104
		unsigned char m_bOnGround; //0x108
		unsigned char m_bInHitGroundAnimation; //0x109
		char pad7[10];
		float m_flLastOriginZ; //0x114
		float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
		float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
		char pad8[4]; //NaN
		float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
		char pad9[4]; //NaN
		float m_flUnknown3;
		char pad10[528];
	};

	struct CAnimState {

		char pad_0x0000[0x18]; //0x0000
		float anim_update_timer; //0x0018 
		char pad_0x001C[0xC]; //0x001C
		float started_moving_time; //0x0028 
		float last_move_time; //0x002C 
		char pad_0x0030[0x10]; //0x0030
		float last_lby_time; //0x0040 
		char pad_0x0044[0x8]; //0x0044
		float run_amount; //0x004C 
		char pad_0x0050[0x10]; //0x0050
		void *entity; //0x0060 
		__int32 active_weapon; //0x0064 
		__int32 last_active_weapon; //0x0068 
		float last_client_side_animation_update_time; //0x006C 
		__int32 last_client_side_animation_update_framecount; //0x0070 
		float eye_timer; //0x0074 
		float eye_angles_y; //0x0078 
		float eye_angles_x; //0x007C 
		float goal_feet_yaw; //0x0080 
		float current_feet_yaw; //0x0084 
		float torso_yaw; //0x0088 
		float last_move_yaw; //0x008C 
		float lean_amount; //0x0090 
		char pad_0x0094[0x4]; //0x0094
		float feet_cycle; //0x0098 
		float feet_yaw_rate; //0x009C 
		char pad_0x00A0[0x4]; //0x00A0
		float duck_amount; //0x00A4 
		float landing_duck_amount; //0x00A8 
		char pad_0x00AC[0x4]; //0x00AC
		Vector current_origin;
		Vector last_origin;
		float velocity_x; //0x00C8 
		float velocity_y; //0x00CC 
		char pad_0x00D0[0x10]; //0x00D0
		float move_direction_1; //0x00E0 
		float move_direction_2; //0x00E4 
		char pad_0x00E8[0x4]; //0x00E8
		float m_velocity; //0x00EC 
		float jump_fall_velocity; //0x00F0 
		float clamped_velocity; //0x00F4 
		float feet_speed_forwards_or_sideways; //0x00F8 
		float feet_speed_unknown_forwards_or_sideways; //0x00FC 
		float last_time_started_moving; //0x0100 
		float last_time_stopped_moving; //0x0104 
		bool on_ground; //0x0108 
		bool hit_in_ground_animation; //0x010C 
		char pad_0x0110[0x4]; //0x0110
		float last_origin_z; //0x0114 
		float head_from_ground_distance_standing; //0x0118 
		float stop_to_full_running_fraction; //0x011C 
		char pad_0x0120[0x14]; //0x0120
		__int32 is_not_moving; //0x0134 
		char pad_0x0138[0x20]; //0x0138
		float last_anim_update_time; //0x0158 
		float moving_direction_x; //0x015C 
		float moving_direction_y; //0x0160 
		float moving_direction_z; //0x0164 
		char pad_0x0168[0x44]; //0x0168
		__int32 started_moving; //0x01AC 
		char pad_0x01B0[0x8]; //0x01B0
		float lean_yaw; //0x01B8 
		char pad_0x01BC[0x8]; //0x01BC
		float poses_speed; //0x01C4 
		char pad_0x01C8[0x8]; //0x01C8
		float ladder_speed; //0x01D0 
		char pad_0x01D4[0x8]; //0x01D4
		float ladder_yaw; //0x01DC 
		char pad_0x01E0[0x8]; //0x01E0
		float some_pose; //0x01E8 
		char pad_0x01EC[0x14]; //0x01EC
		float body_yaw; //0x0200 
		char pad_0x0204[0x8]; //0x0204
		float body_pitch; //0x020C 
		char pad_0x0210[0x8]; //0x0210
		float death_yaw; //0x0218 
		char pad_0x021C[0x8]; //0x021C
		float stand; //0x0224 
		char pad_0x0228[0x8]; //0x0228
		float jump_fall; //0x0230 
		char pad_0x0234[0x8]; //0x0234
		float aim_blend_stand_idle; //0x023C 
		char pad_0x0240[0x8]; //0x0240
		float aim_blend_crouch_idle; //0x0248 
		char pad_0x024C[0x8]; //0x024C
		float strafe_yaw; //0x0254 
		char pad_0x0258[0x8]; //0x0258
		float aim_blend_stand_walk; //0x0260 
		char pad_0x0264[0x8]; //0x0264
		float aim_blend_stand_run; //0x026C 
		char pad_0x0270[0x8]; //0x0270
		float aim_blend_crouch_walk; //0x0278 
		char pad_0x027C[0x8]; //0x027C
		float move_blend_walk; //0x0284 
		char pad_0x0288[0x8]; //0x0288
		float move_blend_run; //0x0290 
		char pad_0x0294[0x8]; //0x0294
		float move_blend_crouch; //0x029C 
		char pad_0x02A0[0x4]; //0x02A0
		float speed; //0x02A4 
		__int32 moving_in_any_direction; //0x02A8 
		float acceleration; //0x02AC 
		char pad_0x02B0[0x74]; //0x02B0
		float crouch_height; //0x0324 
		__int32 is_full_crouched; //0x0328 
		char pad_0x032C[0x4]; //0x032C
		float velocity_subtract_x; //0x0330 
		float velocity_subtract_y; //0x0334 
		float velocity_subtract_z; //0x0338 
		float standing_head_height; //0x033C 
		char pad_0x0340[0x4]; //0x0340
	}; //Size=0x0344

	class CBaseWeapon;
	class CBaseViewModel;

	class CBaseEntity : public IClientEntity
	{
	public:
		//[swap_lines]
		char*	GetPlayerName();
		Vector        GetEyePos();
		bool	IsPlayer();
		void UpdateClientAnimation();
		bool	IsValid();
	//	bool	IsValid();
		void ClientAnimations(bool value);
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
		void SetLowerBodyYaw(float value);
		float   GetSimTime();
		float   GetFlashDuration();
		int		GetShotsFired();
		int		GetIsScoped();
			void FixSetupBones(matrix3x4_t *Matrix);
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
		CAnimState* GetAnimState2();
		Vector	GetAimPunchAngle();
		Vector	GetViewPunchAngle();
		int GetBoneByName(const char * boneName);
		float get_max_desync_delta(CBaseEntity * local);
		float & m_flAbsRotation();
		float getmaxdesync();
		model_t * GetModel2();
		int GetHitboxSet_();
		Vector * GetEyeAnglesPtr();
		void SetAngle2(Vector wantedang);
		int DrawModel2(int flags, uint8_t alpha);
		//CBaseWeapon * GetWeapon();
		Vector	GetVelocity();
		void SetEyeAngles(Vector angles);
		Vector	GetViewOffset();
		int GetSequenceActivity(int sequence);
		Vector	GetEyePosition();
		bool SetupBones2(matrix3x4_t * pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
		QAngle	GetEyeAngles();
		CAnimationLayer  *GetAnimOverlay();
		
		void SetAbsOrigin(Vector ArgOrigin);
		void SetAbsAngles(Vector angles);
		int* m_hMyWeapons();
		Vector GetBonePosition(int bone);
		Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius);
		matrix3x4_t GetBoneMatrix(int BoneID);
		studiohdr_t* GetStudioModel();
		float FireRate();
		mstudiobone_t* GetBone(int nBone);
		mstudiobbox_t* GetHitBox(int nHitbox);
		float GetLastShotTime();
		Vector	GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix);
		Vector GetHitboxPosition(int nHitbox);
		mstudiohitboxset_t* GetHitBoxSet();
		

	};
	

	class CBaseViewModel : public IClientEntity
	{
	public:
		int		GetModelIndex();
		void	SetModelIndex(int nModelIndex);
		void	SetWeaponModel(const char* Filename, IClientEntity* Weapon);

		DWORD	GetOwner();
		DWORD	GetWeapon();
		
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
		CTEPlayerAnimEven,
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
