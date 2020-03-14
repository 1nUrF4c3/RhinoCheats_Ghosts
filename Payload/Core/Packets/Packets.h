//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cPackets
	{
	public:

		int iTeaBagTime;
		Vector3 vTeaBagPos;

		void WritePacket(sUserCmd* currentcmd);
		void PredictPlayerState(sUserCmd* oldcmd, sUserCmd* newcmd);
	} extern _packets;
}

//=====================================================================================