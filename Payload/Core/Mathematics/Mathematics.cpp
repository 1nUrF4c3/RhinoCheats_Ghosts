//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cMathematics _mathematics;

	float cMathematics::CalculateFOV(Vector3 position)
	{
		Vector3 vViewOrigin, vDirection, vAngles, vAimAngles;

		GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);
		VectorSubtract(position, WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg : vViewOrigin, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		MakeVector(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, vAimAngles);
		MakeVector(vAngles, vAngles);

		float flMag = sqrtf(DotProduct(vAimAngles, vAimAngles)),
			flDot = DotProduct(vAimAngles, vAngles),
			flReturn = RadiansToDegrees(acosf(flDot / powf(flMag, 2.0f)));

		if (isnan(flReturn))
			flReturn = 0.0f;

		return flReturn;
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateDistance(Vector3 start, Vector3 end)
	{
		Vector3 vDirection;

		VectorSubtract(start, end, vDirection);

		return sqrtf(DotProduct(vDirection, vDirection));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorAngles(Vector3 direction, Vector3 angles)
	{
		float flTemp, flYaw, flPitch;

		if (direction[0] == 0.0f && direction[1] == 0.0f)
		{
			flYaw = 0.0f;

			if (direction[2] > 0.0f)
				flPitch = 90.0f;

			else
				flPitch = 270.0f;
		}

		else
		{
			flYaw = RadiansToDegrees(atan2f(direction[1], direction[0]));

			if (flYaw < 0.0f)
				flYaw += 360.0f;

			flTemp = sqrtf(direction[0] * direction[0] + direction[1] * direction[1]);
			flPitch = RadiansToDegrees(atan2f(direction[2], flTemp));

			if (flPitch < 0.0f)
				flPitch += 360.0f;
		}

		angles[0] = -flPitch;
		angles[1] = flYaw;
		angles[2] = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::AngleVectors(Vector3 angles, Vector3 forward, Vector3 right, Vector3 up)
	{
		float flAngle, flSinRoll, flSinPitch, flSinYaw, flCosRoll, flCosPitch, flCosYaw;

		flAngle = DegreesToRadians(angles[1]);
		flSinYaw = sinf(flAngle);
		flCosYaw = cosf(flAngle);
		
		flAngle = DegreesToRadians(angles[0]);
		flSinPitch = sinf(flAngle);
		flCosPitch = cosf(flAngle);
		
		flAngle = DegreesToRadians(angles[2]);
		flSinRoll = sinf(flAngle);
		flCosRoll = cosf(flAngle);

		if (forward)
		{
			forward[0] = flCosPitch * flCosYaw;
			forward[1] = flCosPitch * flSinYaw;
			forward[2] = -flSinPitch;
		}

		if (right)
		{
			right[0] = (-1.0f * flSinRoll * flSinPitch * flCosYaw + -1.0f * flCosRoll * -flSinYaw);
			right[1] = (-1.0f * flSinRoll * flSinPitch * flSinYaw + -1.0f * flCosRoll * flCosYaw);
			right[2] = -1.0f * flSinRoll * flCosPitch;
		}

		if (up)
		{
			up[0] = (flCosRoll * flSinPitch * flCosYaw + -flSinRoll * -flSinYaw);
			up[1] = (flCosRoll * flSinPitch * flSinYaw + -flSinRoll * flCosYaw);
			up[2] = flCosRoll * flCosPitch;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorNormalize(Vector3 direction)
	{
		float flLen = sqrtf(DotProduct(direction, direction));

		if (flLen == 0.0f)
		{
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 1.0f;
		}

		else
		{
			flLen = 1.0f / flLen;

			direction[0] *= flLen;
			direction[1] *= flLen;
			direction[2] *= flLen;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::ClampAngles(Vector3 angles)
	{
		while (angles[0] < -180.0f) 
			angles[0] += 360.0f;

		while (angles[0] > 180.0f) 
			angles[0] -= 360.0f;

		while (angles[1] < -180.0f) 
			angles[1] += 360.0f;

		while (angles[1] > 180.0f) 
			angles[1] -= 360.0f;

		if (angles[2] != 0.0f) 
			angles[2] = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::ClampMove(char value[])
	{
		while (value[0] < -128)
			value[0] = -128;

		while (value[0] > 127)
			value[0] = 127;

		while (value[1] < -128)
			value[1] = -128;

		while (value[1] > 127)
			value[1] = 127;

		if (value[2] != 0)
			value[2] = 0;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::CalculateAimAngles(Vector3 start, Vector3 end, Vector3 angles)
	{
		Vector3 vDirection;

		VectorSubtract(start, end, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);
		
		ClampAngles(angles);

		angles[0] -= WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[0] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[0] : CG->vWeaponAngles[0];
		angles[1] -= WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[1] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[1] : CG->vWeaponAngles[1];

		ClampAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::CalculateAntiAimAngles(Vector3 start, Vector3 end, Vector3 angles)
	{
		Vector3 vDirection;
		VectorSubtract(start, end, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);

		ClampAngles(angles);

		angles[0] = -40.0f - angles[0];
		angles[1] = angles[1] - 180.0f;

		angles[0] -= CG->PlayerState.vDeltaAngles[0];
		angles[1] -= CG->PlayerState.vDeltaAngles[1];

		ClampAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MakeVector(Vector3 angles, Vector3 out)
	{
		float flPitch = DegreesToRadians(angles[0]), 
			flYaw = DegreesToRadians(angles[1]);

		out[0] = -cosf(flPitch) * -cosf(flYaw);
		out[1] = sinf(flYaw) * cosf(flPitch);
		out[2] = -sinf(flPitch);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MovementFix(sUserCmd* usercmd, float yaw, float oldyaw, float forward, float right)
	{
		float flDelta = DegreesToRadians(yaw - oldyaw);

		usercmd->szMove[0] = (char)(cosf(flDelta) * forward - sinf(flDelta) * right);
		usercmd->szMove[1] = (char)(sinf(flDelta) * forward + cosf(flDelta) * right);

		ClampMove(usercmd->szMove);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToCompass(Vector3 world, ImVec2 compasspos, float compasssize, ImVec2& screen)
	{
		float flAngle;

		Vector3 vViewOrigin, vDirection, vAngles;

		GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);
		VectorSubtract(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg : vViewOrigin, world, vDirection);
		
		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		VectorSubtract(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, vAngles, vAngles);
		ClampAngles(vAngles);

		flAngle = ((vAngles[1] + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE;

		screen.x = compasspos.x + (cosf(flAngle) * (compasssize / 2.0f));
		screen.y = compasspos.y + (sinf(flAngle) * (compasssize / 2.0f));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToRadar(Vector3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen)
	{
		Vector3 vViewOrigin;

		GetPlayerViewOrigin(&CG->PlayerState, vViewOrigin);

		float flCosYaw = cosf(DegreesToRadians(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[1] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[1] : CG->vWeaponAngles[1])),
			flSinYaw = sinf(DegreesToRadians(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[1] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[1] : CG->vWeaponAngles[1])),
			flDeltaX = world[0] - (WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg[0] : vViewOrigin[0]),
			flDeltaY = world[1] - (WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? RefDef->vViewOrg[1] : vViewOrigin[1]),
			flLocationX = (flDeltaY * flCosYaw - flDeltaX * flSinYaw) / scale,
			flLocationY = (flDeltaX * flCosYaw + flDeltaY * flSinYaw) / scale;

		if (flLocationX < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationX = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (flLocationX > (radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationX = (radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		if (flLocationY < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationY = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (flLocationY > (radarsize / 2.0f - blipsize / 2.0f))
			flLocationY = (radarsize / 2.0f - blipsize / 2.0f);

		screen.x = -flLocationX + radarpos.x;
		screen.y = -flLocationY + radarpos.y;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::RotatePoint(Vector3 point, Vector3 center, float yaw, Vector3 out)
	{
		float flAngleCos = cosf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE),
			flAngleSin = sinf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE),
			flDifferenceX = point[0] - center[0],
			flDifferenceY = point[1] - center[1];

		out[0] = center[0] + flAngleSin * flDifferenceX - flAngleCos * flDifferenceY;
		out[1] = center[1] + flAngleCos * flDifferenceX + flAngleSin * flDifferenceY;
		out[2] = point[2];
	}
}