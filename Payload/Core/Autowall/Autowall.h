//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAutowall
	{
	public:

		float C_Autowall(Vector3 start, Vector3 end, short hitloc);
		float C_TraceBullet(Vector3 start, Vector3 end, short hitloc, int entitynum);
		float G_Autowall(Vector3 start, Vector3 end);
		float G_TraceBullet(Vector3 start, Vector3 end, int entitynum);
		float GetRemainingDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, short partgroup, int weapon, bool alternate);
		bool TraceLine(Vector3 start, Vector3 end, int entitynum);
	} extern _autoWall;
}

//=====================================================================================
