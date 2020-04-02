//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cTargetList
	{
	public:

		typedef struct
		{
			int iIndex;
			float flFOV = FLT_MAX, flDistance = FLT_MAX;
		} sTargetInfo;

		typedef struct
		{
			float flDamage;
			eBone iBoneIndex;
		} sDamageInfo;

		struct sEntityList
		{
			bool bIsValid, bW2SSuccess, bAimFeet, bIsVisible;
			eBone iBoneIndex, iLastBone;
			std::string szWeapon;
			ImVec2 vBones2D[BONE_MAX], vCorners2D[8], vCenter2D, vPosition, vDimentions, vLower, vUpper;
			Vector3 vBones3D[BONE_MAX], vCorners3D[8], vCenter3D, vHitLocation;
			ImVec4 cColor;
		} EntityList[MAX_ENTITIES];

		int iRiotShieldTarget;
		Vector3 vRiotShieldTarget;

		std::vector<BOOL> vIsTarget = std::vector<BOOL>(MAX_CLIENTS, TRUE);

		void GetInformation();
		bool EntityIsValid(int index);
		bool EntityIsEnemy(int index);
		bool IsVisibleInternal(sCEntity* entity, Vector3 position, short hitloc, bool autowall, float* damage);
		bool IsVisible(sCEntity* entity, Vector3 bones3d[BONE_MAX], bool bonescan, bool autowall, eBone* index);
		void ApplyPrediction(sCEntity* entity, Vector3 position);
	} extern _targetList;
}

//=====================================================================================
