#pragma once
#include "SkinChanger.h"

CUtlVector<SndInfo_t> sndList;


int GetGloveSkinByMenu(int gloveIndex, int skinIndex)
{
	if (gloveIndex == 0) // bloudhound
	{
		switch (skinIndex)
		{
		case 0:
			return 10006;
		case 1:
			return 10007;
		case 2:
			return 10008;
		case 3:
			return 10039;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 1) // Sport
	{
		switch (skinIndex)
		{
		case 0:
			return 10038;
		case 1:
			return 10037;
		case 2:
			return 10018;
		case 3:
			return 10019;//
		case 4:
			return 10048;
		case 5:
			return 10047;
		case 6:
			return 10045;
		case 7:
			return 10046;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 2) // Driver
	{
		switch (skinIndex)
		{
		case 0:
			return 10013;
		case 1:
			return 10015;
		case 2:
			return 10016;
		case 3:
			return 10040;//
		case 4:
			return 10043;
		case 5:
			return 10044;
		case 6:
			return 10041;
		case 7:
			return 10042;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 3) // Wraps
	{
		switch (skinIndex)
		{
		case 0:
			return 10009;
		case 1:
			return 10010;
		case 2:
			return 10021;
		case 3:
			return 10036;//
		case 4:
			return 10053;
		case 5:
			return 10054;
		case 6:
			return 10055;
		case 7:
			return 10056;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 4) // Moto
	{
		switch (skinIndex)
		{
		case 0:
			return 10024;
		case 1:
			return 10026;
		case 2:
			return 10027;
		case 3:
			return 10028;//
		case 4:
			return 10050;
		case 5:
			return 10051;
		case 6:
			return 10052;
		case 7:
			return 10049;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 5) // Specialist
	{
		switch (skinIndex)
		{
		case 0:
			return 10030;
		case 1:
			return 10033;
		case 2:
			return 10034;
		case 3:
			return 10035;//
		case 4:
			return 10061;
		case 5:
			return 10062;
		case 6:
			return 10063;
		case 7:
			return 10064;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 6) // Hydra
	{
		switch (skinIndex)
		{
		case 0:
			return 10057;
		case 1:
			return 10058;
		case 2:
			return 10059;
		case 3:
			return 10060;//
		default:
			return 0;
		}
	}
	else
	{
		return 0;
	}
}



unique_ptr<RecvPropHook> SequenceHook;
void _cdecl SequenceProxy(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;
	if (pViewModel) {
		CBaseEntity* pOwner = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(*(HMODULE*)((DWORD)pViewModel + 0x29BC/*mhOwner*/));

		if (pOwner && pOwner->GetIndex() == Interfaces::Engine()->GetLocalPlayer()) {
			const model_t* pModel = Interfaces::ModelInfo()->GetModel(pViewModel->GetModelIndex());
			const char* szModel = Interfaces::ModelInfo()->GetModelName(pModel);

			int m_nSequence = pData->m_Value.m_Int;

			if (!strcmp(szModel, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = Math::RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Math::RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = Math::RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Math::RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = Math::RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = Math::RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_ursus.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = Math::RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Math::RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_stiletto.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Math::RandomInt(12, 13);
					break;

				default:break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_knife_widowmaker.mdl"))
			{
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Math::RandomInt(14, 15);
					break;
				default:break;
				}
			}
			else if (!strcmp(szModel, "models/weapons/v_pist_deagle.mdl"))
			{
				switch (m_nSequence)
				{
				case 7:
					m_nSequence = Math::RandomInt(7, 8);  //7 = default //8 = new
					break;
				default:break;
				}
			}

			pData->m_Value.m_Int = m_nSequence;
		}
	}

	SequenceHook->GetOriginalFunction()(pData, pStruct, pOut);

}

void ApplyCustomGloves(CBaseEntity* mPlayer, int xuidlow)
{

	CBaseEntity* pLocal = Players->GetLocal()->m_pEntity;
	UINT* pWareables = pLocal->GetWearables();
	CBaseAttributableItem* pGlove = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]);
	

	if (!(CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle((HANDLE)pWareables[0])) {
		static ClientClass* pClass;

		if (!pClass)
			pClass = Interfaces::Client()->GetAllClasses();
		while (pClass)
		{
			if (pClass->m_ClassID == (int)CLIENT_CLASS_ID::CEconWearable)
				break;
			pClass = pClass->m_pNext;
		}

		int iEntry, iSerial;

		pClass->m_pCreateFn(iEntry = (Interfaces::EntityList()->GetHighestEntityIndex() + 1),
			iSerial = (Math::RandomInt(0x0, 0xFFF)));

		pWareables[0] = iEntry | (iSerial << 16);


		

		if (pWareables) {
			static int ModelIndex = 0;
			static int ItemDefinitionIndex = 0;

			if (settings.SkinChanger.GloveType == 0)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl");
				ItemDefinitionIndex = 5027;
			}
			else if (settings.SkinChanger.GloveType == 1)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl");
				ItemDefinitionIndex = 5030;
			}
			else if (settings.SkinChanger.GloveType == 2)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl");
				ItemDefinitionIndex = 5031;
			}
			else if (settings.SkinChanger.GloveType == 3)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl");
				ItemDefinitionIndex = 5032;
			}
			else if (settings.SkinChanger.GloveType == 4)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl");
				ItemDefinitionIndex = 5033;
			}
			else if (settings.SkinChanger.GloveType == 5)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl");
				ItemDefinitionIndex = 5034;
			}
			else if (settings.SkinChanger.GloveType == 6)
			{
				ModelIndex = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl");
				ItemDefinitionIndex = 5035;
			}
			else
			{
				ModelIndex = 0;
				ItemDefinitionIndex = 0;
			}

			*(int*)((DWORD)pWareables + Offset::Entity::m_iItemDefinitionIndex) = ItemDefinitionIndex;
			*(int*)((DWORD)pWareables + Offset::Entity::m_iItemIDHigh) = -1;
			*(int*)((DWORD)pWareables + Offset::Entity::m_iEntityQuality) = 4;
			*(int*)((DWORD)pWareables + Offset::Entity::m_iAccountID) = xuidlow;
			*(float*)((DWORD)pWareables + Offset::Entity::m_flFallbackWear) = 0.0005f;
			*(int*)((DWORD)pWareables + Offset::Entity::m_nFallbackSeed) = 0;
			*(int*)((DWORD)pWareables + Offset::Entity::m_nFallbackStatTrak) = -1;
			*(int*)((DWORD)pWareables + Offset::Entity::m_nFallbackPaintKit) = GetGloveSkinByMenu(settings.SkinChanger.GloveType, settings.SkinChanger.gloves_skin);

			pGlove->GetViewModel()->SetModelIndex(ModelIndex);
			pGlove->GetClientNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		}

	}


}

void CSkinChanger::fsn_skinchanger(CBaseEntity* pLocalPlayer, ClientFrameStage_t stage)
{
	CBaseEntity* pLocal = Players->GetLocal()->m_pEntity;
	UINT* pWareables = pLocal->GetWearables();
	CBaseAttributableItem* pGlove = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]);

	if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		return;
	}

	static int models = -1;

	switch (settings.SkinChanger.knife) {
	case 0:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_bayonet.mdl"); break;
	case 1:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_flip.mdl"); break;
	case 2:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gut.mdl"); break;
	case 3:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_karam.mdl"); break;
	case 4:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl"); break;
	case 5:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl"); break;
	case 6:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl"); break;
	case 7:	models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_butterfly.mdl"); break;
	case 8: models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_tactical.mdl"); break;
	case 9: models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_push.mdl"); break;
	case 10:models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl"); break;
	case 11:models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_ursus.mdl"); break;
	case 12:models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_stiletto.mdl"); break;
	case 13:models = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_widowmaker.mdl"); break;
	}

	PlayerInfo pInfo;


	if (!Interfaces::Engine()->GetPlayerInfo(Interfaces::Engine()->GetLocalPlayer(), &pInfo))
		return;


	if (settings.SkinChanger.EnabledGlove) {
		ApplyCustomGloves(pLocalPlayer, pInfo.m_nXuidLow);
	}

	auto hweapons = pLocalPlayer->m_hMyWeapons();

	if (!hweapons) {
		return;
	}

	for (int nIndex = 0; hweapons[nIndex]; nIndex++) {

		if (!hweapons) {
			continue;
		}


		CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)hweapons[nIndex]);
		

		if (!pWeapon) {
			continue;
		}

		settings.SkinChanger.current_weapon = *pWeapon->GetItemDefinitionIndex();

		auto cw = settings.SkinChanger.current_weapon;

		if (cw < 1)
			continue;

		*pWeapon->GetItemIDHigh() = -1;

		if (pInfo.m_nXuidLow != *pWeapon->GetOriginalOwnerXuidLow()
			|| pInfo.m_nXuidHigh != *pWeapon->GetOriginalOwnerXuidHigh()) {
			continue;
		}
		auto weapon = (CBaseWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
		if (!weapon->IsKnife()) {

			if (!settings.SkinChanger.weapon[cw].enable_changer) {
				continue;
			}

			*pWeapon->GetFallbackPaintKit() = settings.SkinChanger.weapon[cw].m_nFallbackPaintKit;
			*pWeapon->GetFallbackWear() = min(1.f, settings.SkinChanger.weapon[cw].m_flFallbackWear + 0.0000000001f);

			if (settings.SkinChanger.weapon[cw].m_nFallbackStatTrak > 0)
			{
				*pWeapon->GetFallbackStatTrak() = settings.SkinChanger.weapon[cw].m_nFallbackStatTrak;
				*pWeapon->GetEntityQuality() = 1;
				*pWeapon->GetAccountID() = pInfo.m_nXuidLow;
			}
			else {
				*pWeapon->GetEntityQuality() = 0;
			}
		}
		else {

			if (!settings.SkinChanger.EnabledKnife) {
				continue;
			}
			CBaseViewModel* pView = pWeapon->GetViewModel();
			*pWeapon->GetItemDefinitionIndex() = weapon->GetKnifeDefinitionIndex(settings.SkinChanger.knife);
			pView->SetModelIndex(models);
			*pWeapon->GetFallbackPaintKit() = settings.SkinChanger.weapon[weapon->GetKnifeDefinitionIndex(settings.SkinChanger.knife)].m_nFallbackPaintKit;
			*pWeapon->GetEntityQuality() = 3;
			*pWeapon->GetFallbackStatTrak() = -1;
			*pWeapon->GetFallbackWear() = 0.00000001f;

			auto  pwpn = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pLocalPlayer->GetActiveWeapon());
			if (!pwpn) {
				continue;
			}
			if (weapon->IsKnife()) {
				DWORD* hViewModel = (DWORD*)((uintptr_t)pLocalPlayer + 0x32F8);

				if (hViewModel) {
					auto pViewModel = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle((HANDLE)hViewModel[0]);
					if (pViewModel) {
						pView->SetModelIndex(models);
					}
				}
			}
		}
	}
}
CSkinChanger* SkinChanger = new CSkinChanger;
