//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cHooks
	{
	public:

		void Refresh(int localnum);
		void WritePacket(int localnum);
		void CreateNewCommands(int localnum);
		void BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
		void CalcEntityLerpPositions(int localnum, sCEntity* entity);
		void Obituary(int localnum, sEntityState* entitystate, int weapon);
		void AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
		void ClientFrame(sGEntity* entity);
	} extern _hooks;
}

//=====================================================================================