#pragma once

#include "../Engine/Engine.h"
#define MAX_PLAYERS_SIZE 64

namespace Engine
{
	enum PLAYER_TEAM;
	enum WEAPON_TYPE;

	class CBaseEntity;

	class CMe
	{
	public:

		bool			bAlive;
		bool			bInReload;

		int				iIndex;
		int				iFov;
		int				iFlags;
		int				iShotsFired;

		Vector			vAimPunch;
		Vector			vPunch;

		Vector			vEyeOrigin;

		string			WeaponName;
		string			WeaponPrintName;
		WEAPON_TYPE		WeaponType;
		
		int				WeaponIndex;
		int				WeaponAmmo;

		PLAYER_TEAM		Team;

		CBaseEntity*	m_pEntity;
		CBaseWeapon*	m_pWeaponEntity;

	};

	class CPlayer
	{
	public:

		Vector			vHitboxHead;
		Vector			vHitboxHeadScreen;

		Vector			vOrigin;
		Vector			vOriginScreen;

		Vector			vHitboxSkeletonArray[18][2];
		Vector			vBulletTraceArray[2];

		string			Name;
		string			WeaponName;
		string			WeaponPrintName;

		PLAYER_TEAM		Team;

		int				iHealth;
		int				iArmor;
		int				iWAmmo;
		int				iDistance;
	

		float			fDistanceHead;
		float			fDistanceChest;

		bool			bUpdate;
		bool			bVisible;

		CBaseEntity*	m_pEntity;

	};

	class CPlayers
	{
	public:
		CPlayers();
		~CPlayers();

		CMe*		GetLocal();
		CPlayer*	GetPlayer( BYTE Index );
		BYTE		GetSize();

		bool		UpdateLocal();
		void		Update();
		void		Clear();

		void		GetHitBoxSkeleton( int nHitBoxOne , int nHitBoxTwo , CBaseEntity* pEntity , Vector* vOut );

	private:
		CMe*		m_pMe;
		CPlayer*	m_pPlayers;
	};
	extern CPlayers* Players;
}