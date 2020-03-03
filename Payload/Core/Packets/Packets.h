//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cPackets
	{
	public:

		void WritePacket(sUserCmd* currentcmd);
		void PredictPlayerState(sUserCmd* oldcmd, sUserCmd* newcmd);
	} extern _packets;
}

//=====================================================================================