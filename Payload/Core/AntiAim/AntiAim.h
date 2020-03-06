//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cAntiAim
	{
	public:

		void ClientAntiAim(sUserCmd* usercmd);
		void EntityAntiAim(Vector3 angles);
	} extern _antiAim;
}

//=====================================================================================
