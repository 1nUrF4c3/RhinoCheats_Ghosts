//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cAimbot
	{
	public:

		struct sAimState
		{
			bool bAkimbo, bTargetAcquired, bLockonTarget, bIsAutoAiming, bIsAutoFiring;
			int iFireTMR, iCurrentAimTime, iCurrentAimDelay, iCurrentZoomDelay, iCurrentFireDelay, iDeltaTMR, iTargetNum, iLastTarget;
			Vector3 vAimPosition, vAimAngles, vAntiAimAngles;
		} AimState;

		void StandardAim();
		void SilentAim(sUserCmd* usercmd);
		void AutoFire(sUserCmd* usercmd);
		void SetAimState();
	} extern _aimBot;
}

//=====================================================================================
