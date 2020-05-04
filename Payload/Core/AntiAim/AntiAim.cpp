//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cAntiAim _antiAim;

	void cAntiAim::AntiAim(sUserCmd* usercmd)
	{
		if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_SPINBOT)
		{
			static float flAngle = 0.0f;

			if (flAngle > 360.0f)
				flAngle -= 360.0f;

			vAntiAimAngles[0] = 85.0f - CG->PlayerState.vDeltaAngles[0];
			vAntiAimAngles[1] = flAngle - CG->PlayerState.vDeltaAngles[1];
			vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);

			flAngle += 45.0f;
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_JITTERBOT)
		{
			static int iMode = 1;

			switch (iMode)
			{
			case 1:
				vAntiAimAngles[0] = -85.0f - CG->PlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 0.0f - CG->PlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 2;
				break;

			case 2:
				vAntiAimAngles[0] = 85.0f - CG->PlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 90.0f - CG->PlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 3;
				break;

			case 3:
				vAntiAimAngles[0] = -85.0f - CG->PlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 180.0f - CG->PlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 4;
				break;

			case 4:
				vAntiAimAngles[0] = 85.0f - CG->PlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 270.0f - CG->PlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 1;
				break;
			}
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomPitch(-85.0f, 85.0f), RandomYaw(0.0f, 360.0f);

			vAntiAimAngles[0] = RandomPitch(Device) - CG->PlayerState.vDeltaAngles[0];
			vAntiAimAngles[1] = RandomYaw(Device) - CG->PlayerState.vDeltaAngles[1];
			vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_REVERSED)
		{
			if (_targetList.vIsTarget[_targetList.iRiotShieldTarget] && _targetList.iRiotShieldTarget != CG->PlayerState.iClientNum)
			{
				_mathematics.CalculateAntiAimAngles(CEntity[_targetList.iRiotShieldTarget].vOrigin, CG->PlayerState.vOrigin, _targetList.vRiotShieldAimAngles);

				vAntiAimAngles[0] = _targetList.vRiotShieldAimAngles[0];
				vAntiAimAngles[1] = _targetList.vRiotShieldAimAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
			}

			else
			{
				if (_aimBot.AimState.bIsAutoAiming)
				{
					vAntiAimAngles[0] = _aimBot.AimState.vAntiAimAngles[0];
					vAntiAimAngles[1] = _aimBot.AimState.vAntiAimAngles[1];
					vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				}

				else
				{
					vAntiAimAngles[0] = -40.0f - CG->PlayerState.vDeltaAngles[0];
					vAntiAimAngles[1] = -180.0f - CG->PlayerState.vDeltaAngles[1];
					vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				}
			}
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_INVERTED)
		{
			vAntiAimAngles[0] = ShortToAngle(usercmd->iViewAngles[0]);
			vAntiAimAngles[1] = ShortToAngle(usercmd->iViewAngles[1]);
			vAntiAimAngles[2] = -180 - CG->PlayerState.vDeltaAngles[2];
		}

		if (IsAntiAiming())
		{
			usercmd->iViewAngles[0] = AngleToShort(vAntiAimAngles[0]);
			usercmd->iViewAngles[1] = AngleToShort(vAntiAimAngles[1]);
			usercmd->iViewAngles[2] = AngleToShort(vAntiAimAngles[2]);

			Vector3 vViewOrigin, vDirection, vAngles, vOneVector = { 1.0f, 1.0f, 1.0f };

			GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);
			VectorSubtract(vOneVector, vViewOrigin, vDirection);

			_mathematics.VectorNormalize(vDirection);
			_mathematics.VectorAngles(vDirection, vAngles);
			_mathematics.ClampAngles(vAngles);

			ViewMatrix->vViewAngles[2] = vAngles[2] - CG->PlayerState.vDeltaAngles[2];
		}
	}
	/*
	//=====================================================================================
	*/
	bool cAntiAim::IsAntiAiming()
	{
		return (GetViewmodelWeapon(&CG->PlayerState) && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) &&
			!(CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_PRONE) &&
			!(CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_MANTLE) &&
			(_profiler.gAntiAim->Current.iValue > cProfiler::ANTIAIM_OFF));
	}
}

//=====================================================================================