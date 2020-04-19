//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cTargetList _targetList;

	void cTargetList::GetInformation()
	{
		sTargetInfo TargetInfo;
		std::vector<sTargetInfo> vTargetInfo;

		_aimBot.AimState.iTargetNum = -1;

		static int iCounter = 0;
		int iBonescanNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			EntityList[i].bIsValid = false;
			EntityList[i].bAimFeet = false;

			if (!EntityIsValid(i))
				continue;

			if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER || CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				LPVOID pDObj = GetEntityDObj(i);

				if (!pDObj)
					continue;

				Vector3 vMinTemp = { FLT_MAX, FLT_MAX, FLT_MAX }, vMaxTemp = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				for (auto& Bone : vBones)
				{
					GetTagPosition(&CEntity[i], pDObj, RegisterTag(szBones[Bone.first].second), EntityList[i].vBones3D[Bone.first]);

					for (int j = 0; j < 3; j++)
					{
						if (EntityList[i].vBones3D[Bone.first][j] < vMinTemp[j])
							vMinTemp[j] = EntityList[i].vBones3D[Bone.first][j];

						if (EntityList[i].vBones3D[Bone.first][j] > vMaxTemp[j])
							vMaxTemp[j] = EntityList[i].vBones3D[Bone.first][j];
					}
				}

				VectorAverage(vMinTemp, vMaxTemp, EntityList[i].vCenter3D);
			}

			char szWeapon[1024] = { NULL };

			GetWeaponDisplayName((BYTE)CEntity[i].NextEntityState.iWeapon, CEntity[i].NextEntityState.iInAltWeaponMode, szWeapon, sizeof(szWeapon));
			EntityList[i].szWeapon = szWeapon;

			EntityList[i].bIsValid = true;

			if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				Vector3 vViewOrigin;
				VectorCopy(CEntity[i].vOrigin, vViewOrigin);
				vViewOrigin[2] += M_METERS;

				EntityList[i].bW2SSuccess = _drawing.Calculate2D(EntityList[i].vBones3D, EntityList[i].vBones2D, EntityList[i].vPosition, EntityList[i].vDimentions) &&
					_drawing.Calculate3D(&CEntity[i], EntityList[i].vCenter3D, EntityList[i].vCorners3D, EntityList[i].vCorners2D) &&
					WorldToScreen(GetScreenMatrix(), EntityList[i].vCenter3D, EntityList[i].vCenter2D) &&
					WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower) &&
					WorldToScreen(GetScreenMatrix(), vViewOrigin, EntityList[i].vUpper);

				_mathematics.WorldToCompass(CEntity[i].vOrigin, _drawing.Compass.vCompassPosition, _drawing.Compass.flCompassSize, _drawing.Compass.vArrowPosition[i]);
				_mathematics.WorldToRadar(CEntity[i].vOrigin, _drawing.Radar.vRadarPosition, _drawing.Radar.flScale, _drawing.Radar.flRadarSize, _drawing.Radar.flBlipSize, _drawing.Radar.vBlipPosition[i]);

				if (!EntityIsEnemy(i))
				{
					EntityList[i].cColor = _profiler.gColorAllies->Current.cValue;
					continue;
				}

				EntityList[i].cColor = _profiler.gColorAxis->Current.cValue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_ITEM)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vCenter2D);
				continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_MISSILE)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), EntityList[i].vBones3D[BONE_HEAD], EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			if (!(CEntity[i].NextEntityState.iEntityType == ET_PLAYER ||
				(_profiler.gTargetMissiles->Current.bValue && CEntity[i].NextEntityState.iEntityType == ET_MISSILE &&
				(CEntity[i].NextEntityState.iWeapon == WEAPON_C4 || CEntity[i].NextEntityState.iWeapon == WEAPON_IED)) ||
					(_profiler.gTargetAgents->Current.bValue && CEntity[i].NextEntityState.iEntityType == ET_AGENT)))
				continue;

			Vector3 vDirection, vAngles, vDelta;

			VectorSubtract(CEntity[i].vOrigin, CG->PlayerState.vOrigin, vDirection);

			VectorNormalize(vDirection);
			VectorAngles(vDirection, vAngles);
			_mathematics.ClampAngles(vAngles);

			VectorSubtract(vAngles, CEntity[i].vViewAngles, vDelta);

			if (((BYTE)CEntity[i].NextEntityState.iWeapon == WEAPON_RIOT_SHIELD && !AngleCompare180(vDelta[1])) ||
				((BYTE)CEntity[i].NextEntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD && AngleCompare180(vDelta[1])))
			{
				if (_profiler.gRiotShield->Current.iValue == cProfiler::RIOTSHIELD_IGNOREPLAYER)
					continue;

				else if (_profiler.gRiotShield->Current.iValue == cProfiler::RIOTSHIELD_TARGETFEET)
					EntityList[i].bAimFeet = true;
			}

			if (EntityList[i].bAimFeet)
			{
				bool bIsLeftAnkleVisible = IsVisibleInternal(&CEntity[i], EntityList[i].vBones3D[BONE_LEFT_ANKLE], vBones[BONE_LEFT_ANKLE].second, _profiler.gAutoWall->Current.bValue, NULL),
					bIsRightAnkleVisible = IsVisibleInternal(&CEntity[i], EntityList[i].vBones3D[BONE_RIGHT_ANKLE], vBones[BONE_RIGHT_ANKLE].second, _profiler.gAutoWall->Current.bValue, NULL);

				if (bIsLeftAnkleVisible && bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = EntityList[i].vBones3D[BONE_LEFT_ANKLE][2] < EntityList[i].vBones3D[BONE_RIGHT_ANKLE][2] ? BONE_LEFT_ANKLE : BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsLeftAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_LEFT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else
					EntityList[i].bIsVisible = false;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				if (_profiler.gBoneScan->Current.iValue == cProfiler::BONESCAN_ONTIMER)
				{
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, iBonescanNum == i, _profiler.gAutoWall->Current.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
				}

				else if (_profiler.gBoneScan->Current.iValue == cProfiler::BONESCAN_IMMEDIATE)
				{
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, true, _profiler.gAutoWall->Current.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
				}

				else
				{
					EntityList[i].iBoneIndex = (eBone)_profiler.gAimBone->Current.iValue;
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, false, _profiler.gAutoWall->Current.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
				}
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].iBoneIndex = BONE_HEAD;
				EntityList[i].bIsVisible = IsVisibleInternal(&CEntity[i], EntityList[i].vBones3D[EntityList[i].iBoneIndex], vBones[EntityList[i].iBoneIndex].second, _profiler.gAutoWall->Current.bValue, NULL);
				VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
			}

			else
			{
				EntityList[i].bIsVisible = IsVisibleInternal(&CEntity[i], CEntity[i].vOrigin, HITLOC_NONE, _profiler.gAutoWall->Current.bValue, NULL);
				VectorCopy(CEntity[i].vOrigin, EntityList[i].vHitLocation);
			}

			if (i < FindVariable("sv_maxclients")->Current.iValue && *(int*)OFF_ISCURRENTHOST)
				if (GEntity[i].iHealth < 1)
					continue;

			if (std::find(vIsTarget.begin(), vIsTarget.end(), TRUE) != vIsTarget.end())
			{
				if (i < FindVariable("sv_maxclients")->Current.iValue)
				{
					if (!vIsTarget[i])
						continue;
				}

				else
				{
					if (!vIsTarget[CEntity[i].NextEntityState.iOtherEntityNum])
						continue;
				}
			}

			if (EntityList[i].bIsVisible && _mathematics.CalculateFOV(EntityList[i].vHitLocation) <= _profiler.gAimAngle->Current.flValue)
			{
				TargetInfo.iIndex = i;

				TargetInfo.flFOV = _mathematics.CalculateFOV(EntityList[i].vHitLocation);
				TargetInfo.flDistance = _mathematics.CalculateDistance(CEntity[i].vOrigin, CG->PlayerState.vOrigin);

				vTargetInfo.push_back(TargetInfo);
			}
		}

		if (!vTargetInfo.empty())
		{
			if (_profiler.gSortMethod->Current.iValue == cProfiler::SORT_METHOD_FOV)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flFOV < b.flFOV; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else if (_profiler.gSortMethod->Current.iValue == cProfiler::SORT_METHOD_DISTANCE)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance < b.flDistance; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			vTargetInfo.clear();
		}

		_aimBot.AimState.bTargetAcquired = (_aimBot.AimState.iTargetNum > -1);
		_aimBot.AimState.bLockonTarget = (_profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_AUTO || (_profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_MANUAL && CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM));
		_aimBot.AimState.bIsAutoAiming = (_aimBot.AimState.bTargetAcquired && _aimBot.AimState.bLockonTarget);
		_aimBot.AimState.bIsAutoFiring = (_profiler.gAutoFire->Current.bValue && _aimBot.AimState.bIsAutoAiming);

		if (_aimBot.AimState.bLockonTarget)
		{
			if (_aimBot.AimState.iCurrentAimDelay == _profiler.gAutoAimDelay->Current.iValue)
				_aimBot.AimState.iCurrentAimTime += clock() - _aimBot.AimState.iDeltaTMR;

			_aimBot.AimState.iCurrentAimDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentZoomDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentFireDelay += clock() - _aimBot.AimState.iDeltaTMR;
		}

		_aimBot.AimState.iDeltaTMR = clock();

		if (_aimBot.AimState.iLastTarget != _aimBot.AimState.iTargetNum)
		{
			_aimBot.AimState.iLastTarget = _aimBot.AimState.iTargetNum;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (EntityList[_aimBot.AimState.iTargetNum].iLastBone != EntityList[_aimBot.AimState.iTargetNum].iBoneIndex)
		{
			EntityList[_aimBot.AimState.iTargetNum].iLastBone = EntityList[_aimBot.AimState.iTargetNum].iBoneIndex;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (!_aimBot.AimState.bTargetAcquired)
			_aimBot.AimState.iCurrentAimDelay = _aimBot.AimState.iCurrentZoomDelay = _aimBot.AimState.iCurrentFireDelay = 0;

		if (_aimBot.AimState.iCurrentAimTime > _profiler.gAutoAimTime->Current.iValue)
			_aimBot.AimState.iCurrentAimTime = _profiler.gAutoAimTime->Current.iValue;

		if (_aimBot.AimState.iCurrentAimDelay > _profiler.gAutoAimDelay->Current.iValue)
			_aimBot.AimState.iCurrentAimDelay = _profiler.gAutoAimDelay->Current.iValue;

		if (_aimBot.AimState.iCurrentZoomDelay > _profiler.gAutoZoomDelay->Current.iValue)
			_aimBot.AimState.iCurrentZoomDelay = _profiler.gAutoZoomDelay->Current.iValue;

		if (_aimBot.AimState.iCurrentFireDelay > _profiler.gAutoFireDelay->Current.iValue)
			_aimBot.AimState.iCurrentFireDelay = _profiler.gAutoFireDelay->Current.iValue;

		if (_aimBot.AimState.bTargetAcquired)
		{
			Vector3 vViewOrigin;
			GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);

			VectorCopy(EntityList[_aimBot.AimState.iTargetNum].vHitLocation, _aimBot.AimState.vAimPosition);

			_mathematics.CalculateAimAngles(_aimBot.AimState.vAimPosition, WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg : vViewOrigin, _aimBot.AimState.vAimAngles);
			_mathematics.CalculateAntiAimAngles(_aimBot.AimState.vAimPosition, WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg : vViewOrigin, _aimBot.AimState.vAntiAimAngles);
		}

		iCounter++;
		_aimBot.AimState.iFireTMR++;

		if (WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)))
		{
			if (!(_aimBot.AimState.iFireTMR % ((BYTE)GetViewmodelWeapon(&CG->PlayerState) == WEAPON_44_MAGNUM ? 12 : 6)))
				_aimBot.AimState.bAkimbo = !_aimBot.AimState.bAkimbo;
		}

		else
			_aimBot.AimState.bAkimbo = false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsValid(int index)
	{
		if (CEntity[index].NextEntityState.iEntityType == ET_PLAYER)
		{
			if (index != CG->PlayerState.iClientNum && CEntity[index].iIsAlive & 1 && CharacterInfo[index].iInfoValid && !(CEntity[index].NextEntityState.LerpEntityState.iEntityFlags & EF_DEAD))
				return true;
		}

		else
		{
			if (index != CG->PlayerState.iClientNum && CEntity[index].iIsAlive & 1 && CEntity[index].wValid)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsEnemy(int index)
	{
		if (CEntity[index].NextEntityState.iEntityType == ET_PLAYER)
		{
			if (CharacterInfo[index].iTeam == TEAM_FREE || CharacterInfo[index].iTeam != CharacterInfo[CG->PlayerState.iClientNum].iTeam)
				return true;
		}

		else
		{
			if (CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam == TEAM_FREE || CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam != CharacterInfo[CG->PlayerState.iClientNum].iTeam)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisibleInternal(sCEntity* entity, Vector3 position, short hitloc, bool autowall, float* damage)
	{
		Vector3 vViewOrigin;

		GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);
		ApplyPrediction(entity, position);

		if (WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))
		{
			bool bTraceHit = _autoWall.TraceLine(RefDef->vViewOrg, position, entity->NextEntityState.iEntityNum);

			if (bTraceHit)
				return true;
		}

		else if (autowall)
		{
			float flDamage = _autoWall.C_Autowall(vViewOrigin, position, hitloc);

			if (damage)
				*damage = flDamage;

			if (flDamage >= 1.0f)
				return true;
		}

		else
		{
			float flDamage = _autoWall.C_TraceBullet(vViewOrigin, position, hitloc, entity->NextEntityState.iEntityNum);

			if (damage)
				*damage = flDamage;

			if (flDamage >= 1.0f)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisible(sCEntity* entity, Vector3 bones3d[BONE_MAX], bool bonescan, bool autowall, eBone* index)
	{
		bool bReturn = false;

		sDamageInfo DamageInfo;
		std::vector<sDamageInfo> vDamageInfo;

		if (bonescan)
		{
			for (auto& Bone : vBones)
			{
				if (IsVisibleInternal(entity, bones3d[Bone.first], vBones[Bone.first].second, autowall, &DamageInfo.flDamage))
				{
					DamageInfo.iBoneIndex = Bone.first;
					vDamageInfo.push_back(DamageInfo);

					bReturn = true;
				}
			}
		}

		else
		{
			return IsVisibleInternal(entity, bones3d[*index], vBones[*index].second, autowall, &DamageInfo.flDamage);
		}

		if (!vDamageInfo.empty())
		{
			std::stable_sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const sDamageInfo& a, const sDamageInfo& b) { return a.flDamage > b.flDamage; });
			*index = vDamageInfo.front().iBoneIndex;
			vDamageInfo.clear();
		}

		return bReturn;
	}
	/*
	//=====================================================================================
	*/
	void cTargetList::ApplyPrediction(sCEntity* entity, Vector3 position)
	{
		Vector3 vOldPosition, vNewPosition, vVelocity;

		EvaluateTrajectory(&entity->CurrentEntityState.PositionTrajectory, CG->PlayerState.OldSnapShot->iServerTime, vOldPosition);
		EvaluateTrajectory(&entity->NextEntityState.LerpEntityState.PositionTrajectory, CG->PlayerState.NewSnapShot->iServerTime, vNewPosition);

		VectorSubtract(vNewPosition, vOldPosition, vVelocity);

		VectorMA(position, *(int*)OFF_FRAMETIME / 1000.0f, vVelocity, position);
		VectorMA(position, *(int*)OFF_PING / 1000.0f, vVelocity, position);
	}
}

//=====================================================================================