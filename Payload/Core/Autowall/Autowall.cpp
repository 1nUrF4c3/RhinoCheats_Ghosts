//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cAutowall _autoWall;

	float cAutowall::C_Autowall(sCEntity* entity, Vector3 start, Vector3 end, short hitloc)
	{
		int iClientNum = CG->PredictedPlayerState.iClientNum;
		sCEntity* pCEntity = &CEntity[iClientNum];
		BYTE iWeapon = pCEntity->NextEntityState.iWeapon;
		int iInAltWeaponMode = pCEntity->NextEntityState.iInAltWeaponMode;
		sWeaponDef* pWeaponDef = Weapons->WeaponDef[iWeapon];
		ePenetrateType iPenetrateType = pWeaponDef->iPenetrateType;
		eWeaponType iWeaponType = pWeaponDef->iWeaponType;

		sBulletFireParams FP_Enter, FP_Exit;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&FP_Exit, sizeof(sBulletFireParams));

		sBulletTraceResults TR_Enter, TR_Exit;

		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));
		ZeroMemory(&TR_Exit, sizeof(sBulletTraceResults));

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (pWeaponDef->bRifleBullet != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		VectorSubtract(end, start, FP_Enter.vDir);
		float flLength = VectorLength(FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		bool bEnterHit = C_BulletTrace(&FP_Enter, pCEntity, &TR_Enter, TRACE_HITTYPE_NONE);

		if (bEnterHit)
		{
			if (iPenetrateType <= PENETRATE_TYPE_NONE)
				return 0.0f;

			if (GetTraceHitType(&TR_Enter) == entity->NextEntityState.iEntityNum)
				return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);

			float flEnterDepth = 0.0f, flExitDepth = 0.0f, flSurfaceDepth = 0.0f;

			Vector3 vHitPos = { 0.0f }, vTemp = { 0.0f };

			for (int iSurfaceCount = 0; bEnterHit && iSurfaceCount < 5; ++iSurfaceCount)
			{
				flEnterDepth = GetSurfacePenetrationDepth(iWeapon, iInAltWeaponMode, TR_Enter.iDepthSurfaceType);

				if (HasPerk(iClientNum, PERK_EXTRABP))
					flEnterDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

				if (flEnterDepth <= 0.0f)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);
				VectorSubtract(vHitPos, FP_Enter.vStart, vTemp);

				if (VectorLength(vTemp) >= flLength)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);

				if (!AdvanceTrace(&FP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = C_BulletTrace(&FP_Enter, pCEntity, &TR_Enter, TR_Enter.iDepthSurfaceType);

				CopyMemory(&FP_Exit, &FP_Enter, sizeof(sBulletFireParams));
				VectorScale(FP_Enter.vDir, -1.0f, FP_Exit.vDir);

				VectorCopy(FP_Enter.vEnd, FP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, FP_Exit.vDir, FP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(sBulletTraceResults));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&FP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = C_BulletTrace(&FP_Exit, pCEntity, &TR_Exit, TR_Exit.iDepthSurfaceType);
				bool bStaticModel = (bExitHit && TR_Exit.Trace.bAllSolid) || (TR_Enter.Trace.bStartSolid && TR_Exit.Trace.bStartSolid);

				if (bExitHit || bStaticModel)
				{
					if (bStaticModel)
						flSurfaceDepth = _mathematics.CalculateDistance(FP_Exit.vEnd, FP_Exit.vStart);
					else
						flSurfaceDepth = _mathematics.CalculateDistance(vHitPos, TR_Exit.vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit)
					{
						flExitDepth = GetSurfacePenetrationDepth(iWeapon, iInAltWeaponMode, TR_Exit.iDepthSurfaceType);

						if (HasPerk(iClientNum, PERK_EXTRABP))
							flExitDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

						flEnterDepth = min(flEnterDepth, flExitDepth);

						if (flEnterDepth <= 0.0f)
							return 0.0f;
					}

					FP_Enter.flPower -= flSurfaceDepth / flEnterDepth;

					if (FP_Enter.flPower <= 0.0f)
						return 0.0f;

					if (!bStaticModel && iWeaponType == WEAPTYPE_BULLET)
					{
						Vector3 vLength;

						VectorSubtract(TR_Exit.vHitPos, TR_Enter.vHitPos, vLength);

						float flLength = DotProduct(vLength, vLength);

						if (flLength > 900.0f)
						{
							if (!bEnterHit)
								return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);
						}

						if (GetTraceHitType(&TR_Exit) == entity->NextEntityState.iEntityNum)
							return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);
					}
				}

				else if (!bEnterHit)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);

				if (GetTraceHitType(&TR_Enter) == entity->NextEntityState.iEntityNum)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);
			}

			return 0.0f;
		}

		return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::C_TraceBullet(sCEntity* entity, Vector3 start, Vector3 end, short hitloc)
	{
		int iClientNum = CG->PredictedPlayerState.iClientNum;
		sCEntity* pCEntity = &CEntity[iClientNum];
		BYTE iWeapon = pCEntity->NextEntityState.iWeapon;
		int iInAltWeaponMode = pCEntity->NextEntityState.iInAltWeaponMode;
		sWeaponDef* pWeaponDef = Weapons->WeaponDef[iWeapon];

		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (pWeaponDef->bRifleBullet != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		C_BulletTrace(&FP_Enter, pCEntity, &TR_Enter, TRACE_HITTYPE_NONE);

		if (GetTraceHitType(&TR_Enter) == entity->NextEntityState.iEntityNum || TR_Enter.Trace.flFraction == 1.0f)
			return GetRemainingDamage(&FP_Enter, &TR_Enter, hitloc, iWeapon, iInAltWeaponMode);

		return 0.0f;
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::G_Autowall(sGEntity* entity, Vector3 start, Vector3 end)
	{
		int iClientNum = CG->PredictedPlayerState.iClientNum;
		sGEntity* pGEntity = &GEntity[iClientNum];
		BYTE iWeapon = pGEntity->EntityState.iWeapon;
		int iInAltWeaponMode = pGEntity->EntityState.iInAltWeaponMode;
		sWeaponDef* pWeaponDef = Weapons->WeaponDef[iWeapon];
		ePenetrateType iPenetrateType = pWeaponDef->iPenetrateType;
		eWeaponType iWeaponType = pWeaponDef->iWeaponType;

		sBulletFireParams FP_Enter, FP_Exit;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&FP_Exit, sizeof(sBulletFireParams));

		sBulletTraceResults TR_Enter, TR_Exit;

		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));
		ZeroMemory(&TR_Exit, sizeof(sBulletTraceResults));

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (pWeaponDef->bRifleBullet != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		VectorSubtract(end, start, FP_Enter.vDir);
		float flLength = VectorLength(FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		bool bEnterHit = G_BulletTrace(&FP_Enter, iWeapon, iInAltWeaponMode, pGEntity, &TR_Enter, TRACE_HITTYPE_NONE);

		if (TR_Enter.Trace.wPartGroup == 19)
			return 0.0f;

		if (bEnterHit)
		{
			if (iPenetrateType <= PENETRATE_TYPE_NONE)
				return 0.0f;

			if (GetTraceHitType(&TR_Enter) == entity->EntityState.iEntityNum)
				return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);

			float flEnterDepth = 0.0f, flExitDepth = 0.0f, flSurfaceDepth = 0.0f;

			Vector3 vHitPos = { 0.0f }, vTemp = { 0.0f };

			for (int iSurfaceCount = 0; bEnterHit && iSurfaceCount < 5; ++iSurfaceCount)
			{
				flEnterDepth = GetSurfacePenetrationDepth(iWeapon, iInAltWeaponMode, TR_Enter.iDepthSurfaceType);

				if (HasPerk(iClientNum, PERK_EXTRABP))
					flEnterDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

				if (flEnterDepth <= 0.0f)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);
				VectorSubtract(vHitPos, FP_Enter.vStart, vTemp);

				if (VectorLength(vTemp) >= flLength)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);

				if (!AdvanceTrace(&FP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = G_BulletTrace(&FP_Enter, iWeapon, iInAltWeaponMode, pGEntity, &TR_Enter, TR_Enter.iDepthSurfaceType);

				if (TR_Enter.Trace.wPartGroup == 19)
					return 0.0f;

				CopyMemory(&FP_Exit, &FP_Enter, sizeof(sBulletFireParams));
				VectorScale(FP_Enter.vDir, -1.0f, FP_Exit.vDir);

				VectorCopy(FP_Enter.vEnd, FP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, FP_Exit.vDir, FP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(sBulletTraceResults));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&FP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = G_BulletTrace(&FP_Exit, iWeapon, iInAltWeaponMode, pGEntity, &TR_Exit, TR_Exit.iDepthSurfaceType);
				bool bStaticModel = (bExitHit && TR_Exit.Trace.bAllSolid) || (TR_Enter.Trace.bStartSolid && TR_Exit.Trace.bStartSolid);

				if (TR_Exit.Trace.wPartGroup == 19)
					return 0.0f;

				if (bExitHit || bStaticModel)
				{
					if (bStaticModel)
						flSurfaceDepth = _mathematics.CalculateDistance(FP_Exit.vEnd, FP_Exit.vStart);
					else
						flSurfaceDepth = _mathematics.CalculateDistance(vHitPos, TR_Exit.vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit)
					{
						flExitDepth = GetSurfacePenetrationDepth(iWeapon, iInAltWeaponMode, TR_Exit.iDepthSurfaceType);

						if (HasPerk(iClientNum, PERK_EXTRABP))
							flExitDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

						flEnterDepth = min(flEnterDepth, flExitDepth);

						if (flEnterDepth <= 0.0f)
							return 0.0f;
					}

					FP_Enter.flPower -= flSurfaceDepth / flEnterDepth;

					if (FP_Enter.flPower <= 0.0f)
						return 0.0f;

					if (!bStaticModel && iWeaponType == WEAPTYPE_BULLET)
					{
						Vector3 vLength;

						VectorSubtract(TR_Exit.vHitPos, TR_Enter.vHitPos, vLength);

						float flLength = DotProduct(vLength, vLength);

						if (flLength > 900.0f)
						{
							if (!bEnterHit)
								return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);
						}

						if (GetTraceHitType(&TR_Exit) == entity->EntityState.iEntityNum)
							return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);
					}
				}

				else if (!bEnterHit)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);

				if (GetTraceHitType(&TR_Enter) == entity->EntityState.iEntityNum)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);
			}

			return 0.0f;
		}

		return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::G_TraceBullet(sGEntity* entity, Vector3 start, Vector3 end)
	{
		int iClientNum = CG->PredictedPlayerState.iClientNum;
		sGEntity* pGEntity = &GEntity[iClientNum];
		BYTE iWeapon = pGEntity->EntityState.iWeapon;
		int iInAltWeaponMode = pGEntity->EntityState.iInAltWeaponMode;
		sWeaponDef* pWeaponDef = Weapons->WeaponDef[iWeapon];

		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (pWeaponDef->bRifleBullet != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		G_BulletTrace(&FP_Enter, iWeapon, iInAltWeaponMode, pGEntity, &TR_Enter, TRACE_HITTYPE_NONE);

		if (TR_Enter.Trace.wPartGroup == 19)
			return 0.0f;

		if (GetTraceHitType(&TR_Enter) == entity->EntityState.iEntityNum || TR_Enter.Trace.flFraction == 1.0f)
			return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, iWeapon, iInAltWeaponMode);

		return 0.0f;
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::GetRemainingDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, short partgroup, int weapon, bool alternate)
	{
		float flDamage = 0.0f;
		Vector3 vHitPos, vStart;

		if (fireparams->flPower > 0.0f)
		{
			VectorCopy(traceresults->vHitPos, vHitPos);
			VectorCopy(fireparams->vStart, vStart);

			flDamage = (float)GetBulletDamageForProjectile(&CG->PredictedPlayerState, weapon, alternate, vStart, vHitPos) * fireparams->flPower;
			flDamage = GetWeaponHitLocationMultiplier(partgroup, weapon, alternate) * flDamage;

			if (flDamage <= 0.0f)
				flDamage = 1.0f;
		}

		return flDamage;
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::TraceLine(sCEntity* entity, Vector3 start, Vector3 end)
	{
		Vector3 vStart, vEnd;

		VectorCopy(start, vStart);
		VectorCopy(end, vEnd);

		sTrace Trace;
		ZeroMemory(&Trace, sizeof(sTrace));

		LocationalTrace(&Trace, vStart, vEnd, CG->PredictedPlayerState.iClientNum, 0x803003);

		return (Trace.wHitID == entity->NextEntityState.iEntityNum || Trace.flFraction == 1.0f);
	}
}

//=====================================================================================