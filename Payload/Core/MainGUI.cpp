//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cMainGUI _mainGui;

	void cMainGUI::InitInterface()
	{
		hWindow = *(HWND*)OFF_WINDOWHANDLE;

		if (!hWindow || !pDevice || !pDeviceContext)
			return;

		oWindowProcess = (tWindowProcess)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)_thunkWindowProcess.GetThunk());

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		Menu.szIniFileName = acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_INI, " ", "");
		Menu.szLogFileName = acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_LOG, " ", "");

		ImGui::GetIO().IniFilename = Menu.szIniFileName.c_str();
		ImGui::GetIO().LogFilename = Menu.szLogFileName.c_str();

		_profiler.gMenuColor->Current.iValue = GetPrivateProfileInt("MenuStyle", "COLOR", cProfiler::MENU_COLOR_NEUTRAL, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());
		_profiler.gMenuCursor->Current.iValue = GetPrivateProfileInt("MenuStyle", "CURSOR", cProfiler::MENU_CURSOR_BLACK, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());
		_profiler.gMenuFont->Current.iValue = GetPrivateProfileInt("MenuStyle", "FONT", cProfiler::MENU_FONT_LIGHT, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

		RefreshInterface(_profiler.gMenuColor->Current.iValue, _profiler.gMenuCursor->Current.iValue, _profiler.gMenuFont->Current.iValue);

		bInitialized = true;
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuColor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_COLOR_NEUTRAL:
			ImGui::StyleColorsNeutral();
			break;

		case cProfiler::MENU_COLOR_NEUTRAL_NEON:
			ImGui::StyleColorsNeutralNeon();
			break;

		case cProfiler::MENU_COLOR_RED:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_RED_NEON:
			ImGui::StyleColorsRedNeon();
			break;

		case cProfiler::MENU_COLOR_ORANGE:
			ImGui::StyleColorsOrange();
			break;

		case cProfiler::MENU_COLOR_ORANGE_NEON:
			ImGui::StyleColorsOrangeNeon();
			break;

		case cProfiler::MENU_COLOR_YELLOW:
			ImGui::StyleColorsYellow();
			break;

		case cProfiler::MENU_COLOR_YELLOW_NEON:
			ImGui::StyleColorsYellowNeon();
			break;

		case cProfiler::MENU_COLOR_GREEN:
			ImGui::StyleColorsGreen();
			break;

		case cProfiler::MENU_COLOR_GREEN_NEON:
			ImGui::StyleColorsGreenNeon();
			break;

		case cProfiler::MENU_COLOR_BLUE:
			ImGui::StyleColorsBlue();
			break;

		case cProfiler::MENU_COLOR_BLUE_NEON:
			ImGui::StyleColorsBlueNeon();
			break;

		case cProfiler::MENU_COLOR_PURPLE:
			ImGui::StyleColorsPurple();
			break;

		case cProfiler::MENU_COLOR_PURPLE_NEON:
			ImGui::StyleColorsPurpleNeon();
			break;

		case cProfiler::MENU_COLOR_RAINBOW:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_RAINBOW_NEON:
			ImGui::StyleColorsRedNeon();
			break;

		default:
			ImGui::StyleColorsNeutral();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuCursor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_CURSOR_BLACK:
			ImGui::StyleCursorsBlack();
			break;

		case cProfiler::MENU_CURSOR_WHITE:
			ImGui::StyleCursorsWhite();
			break;

		default:
			ImGui::StyleCursorsBlack();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuFont(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_FONT_LIGHT:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;

		case cProfiler::MENU_FONT_MEDIUM:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_MEDIUM];
			break;

		case cProfiler::MENU_FONT_BOLD:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_BOLD];
			break;

		default:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::RefreshInterface(int color, int cursor, int font)
	{
		SetMenuColor(color);

		ImGui::GetIO().Fonts->Clear();

		SetMenuCursor(cursor);

		ImGui::GetIO().Fonts->AddFontLight();
		ImGui::GetIO().Fonts->AddFontMedium();
		ImGui::GetIO().Fonts->AddFontBold();

		Eurostile_Bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_bold_compressed_data_base85, flEurostile_Bold = Window->iHeight / 80.0f);
		Eurostile_Extended = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_extended_compressed_data_base85, flEurostile_Extended = Window->iHeight / 60.0f);
		Eurostile_Regular = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_regular_compressed_data_base85, flEurostile_Regular = Window->iHeight / 80.0f);

		ImGui_ImplDX11_CreateDeviceObjects();

		SetMenuFont(font);
	}
	/*
	//=====================================================================================
	*/
	bool cMainGUI::GetKeyPress(int vkey, bool immediate)
	{
		if (VirtualKeys[vkey].bKey)
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = true;
		}

		else if (!VirtualKeys[vkey].bKey && VirtualKeys[vkey].bDown)
		{
			VirtualKeys[vkey].bUp = true;
			VirtualKeys[vkey].bDown = false;
		}

		else
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = false;
		}

		if (immediate)
			return VirtualKeys[vkey].bDown;

		else
			return VirtualKeys[vkey].bUp;
	}
	/*
	//=====================================================================================
	*/
	LRESULT CALLBACK cMainGUI::WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			VirtualKeys[VK_LBUTTON].bKey = true;
			break;

		case WM_LBUTTONUP:
			VirtualKeys[VK_LBUTTON].bKey = false;
			break;

		case WM_RBUTTONDOWN:
			VirtualKeys[VK_RBUTTON].bKey = true;
			break;

		case WM_RBUTTONUP:
			VirtualKeys[VK_RBUTTON].bKey = false;
			break;

		case WM_KEYDOWN:
			VirtualKeys[wParam].bKey = true;
			break;

		case WM_KEYUP:
			VirtualKeys[wParam].bKey = false;
			break;
		}

		if (GetKeyPress(VK_INSERT, false))
			Menu.bShowWindow = !Menu.bShowWindow;

		if (GetKeyPress(VK_DELETE, false))
			if (!LocalClientIsInGame())
				std::thread(&cHost::StartMatch, &_host).detach();

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		if (GetKeyPress(VK_PRIOR, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin, Menu.HostMenu.vTeleport);

		if (GetKeyPress(VK_NEXT, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(Menu.HostMenu.vTeleport, PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin);

		*(bool*)OFF_MOUSEINPUT = !Menu.bShowWindow;
		FindVariable("cl_bypassMouseInput")->Current.iValue = Menu.bShowWindow;

		if (bInitialized && Menu.bShowWindow && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return TRUE;

		return CallWindowProc(oWindowProcess, hWnd, uMsg, wParam, lParam);
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags)
	{
		if (!bInitialized)
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pDeviceContext);
			InitInterface();
		}

		else
		{
			if (_profiler.gMenuColor->Current.iValue == cProfiler::MENU_COLOR_RAINBOW || _profiler.gMenuColor->Current.iValue == cProfiler::MENU_COLOR_RAINBOW_NEON)
			{
				static float flHue = 0.0f;

				if (flHue > 255.0f)
					flHue = 0.0f;

				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					Vector3 vHSV;

					ImGui::ColorConvertRGBtoHSV(ImGui::GetStyle().Colors[i].x, ImGui::GetStyle().Colors[i].y, ImGui::GetStyle().Colors[i].z, vHSV[0], vHSV[1], vHSV[2]);
					ImGui::ColorConvertHSVtoRGB(ByteToFloat(flHue), vHSV[1], vHSV[2], ImGui::GetStyle().Colors[i].x, ImGui::GetStyle().Colors[i].y, ImGui::GetStyle().Colors[i].z);
				}

				for (int i = 0; i < ImGui::TabLabelStyle::Col_TabLabel_Count; i++)
				{
					Vector3 vHSV;
					ImVec4 vColor = ImGui::ColorConvertU32ToFloat4(ImGui::TabLabelStyle::Get().colors[i]);

					ImGui::ColorConvertRGBtoHSV(vColor.x, vColor.y, vColor.z, vHSV[0], vHSV[1], vHSV[2]);
					ImGui::ColorConvertHSVtoRGB(ByteToFloat(flHue), vHSV[1], vHSV[2], vColor.x, vColor.y, vColor.z);

					ImGui::TabLabelStyle::Get().colors[i] = ImGui::ColorConvertFloat4ToU32(vColor);
				}

				flHue += (60.0f / ImGui::GetIO().Framerate);
			}

			ImGui::GetIO().MouseDrawCursor = Menu.bShowWindow;

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

			ImGui::Begin("INVISIBLE", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
			ImGui::GetWindowDrawList()->PushClipRectFullScreen();

			ImGui::PopStyleColor(2);

			if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
			{
				_drawing.DrawESP();
				_drawing.DrawCompass();
				_drawing.DrawRadar();
				_drawing.DrawCrosshair();
				_drawing.DrawTracers();
			}

			std::string szWatermark(VariadicText("%s - COD GHOSTS by: InUrFace | Frametime: %s, Ping: %s", acut::ToUpper(PROGRAM_NAME).c_str(),
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_FRAMETIME).c_str() : "N/A",
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_PING).c_str() : "N/A"));

			ImVec2 vWatermark(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szWatermark.c_str()));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y + 1.0f, flEurostile_Extended - vWatermark.y + 1.0f),
				0xFF000000, szWatermark.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y, flEurostile_Extended - vWatermark.y),
				0xFFFFFFFF, szWatermark.c_str());

			if (LocalClientIsInGame())
			{
				std::string szConnection(*(int*)OFF_ISCURRENTHOST ? "Hosting" : "Not Hosting");
				ImVec2 vConnection(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szConnection.c_str()));

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f + 1.0f, flEurostile_Extended - vConnection.y + 1.0f),
					0xFF000000, szConnection.c_str());

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f, flEurostile_Extended - vConnection.y),
					*(int*)OFF_ISCURRENTHOST ? 0xFF00FF00 : 0xFF0000FF, szConnection.c_str());
			}

			std::string szFramesPerSecond(VariadicText("%i", (int)ImGui::GetIO().Framerate));
			ImVec2 vFramesPerSecond(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szFramesPerSecond.c_str()));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y + 1.0f, flEurostile_Extended - vFramesPerSecond.y + 1.0f),
				0xFF000000, szFramesPerSecond.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y, flEurostile_Extended - vFramesPerSecond.y),
				0xFF00FFFF, szFramesPerSecond.c_str());
			
			ImGui::End();

			if (Menu.bShowWindow)
			{
				if (Menu.bWriteLog)
				{
					ImGui::LogToFile();
					Menu.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(490.0f, 324.0f));
				ImGui::Begin(acut::ToUpper(PROGRAM_NAME).c_str(), &Menu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);

				if (ImGui::TabLabels(_profiler.gMenuTabs->Domain.iMax, acut::StringVectorToCharPointerArray(_profiler.gMenuTabs->szItems), _profiler.gMenuTabs->Current.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
				{
					Menu.bWriteLog = true;
				}

				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 204.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				switch (_profiler.gMenuTabs->Current.iValue)
				{
				case cProfiler::MENU_TAB_AIMBOT:
				{
					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_OFF].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_OFF))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(148.0f);

					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_MANUAL].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_MANUAL))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_AUTO].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_AUTO))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutoZoom->szName.c_str(), &_profiler.gAutoZoom->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAutoFire->szName.c_str(), &_profiler.gAutoFire->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutoWall->szName.c_str(), &_profiler.gAutoWall->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gSilentAim->szName.c_str(), &_profiler.gSilentAim->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gTargetAgents->szName.c_str(), &_profiler.gTargetAgents->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gTargetMissiles->szName.c_str(), &_profiler.gTargetMissiles->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gAntiAim->szName.c_str(), &_profiler.gAntiAim->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gAntiAim->szItems), _profiler.gAntiAim->Domain.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gBoneScan->szName.c_str(), &_profiler.gBoneScan->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gBoneScan->szItems), _profiler.gBoneScan->Domain.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gSortMethod->szName.c_str(), &_profiler.gSortMethod->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gSortMethod->szItems), _profiler.gSortMethod->Domain.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_WALLHACK:
				{
					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_AXIS].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_AXIS))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(148.0f);

					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALLIES].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_ALLIES))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALL].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_ALL))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBoxes->szName.c_str(), &_profiler.gPlayerBoxes->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBoxes->szItems), _profiler.gPlayerBoxes->Domain.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBones->szName.c_str(), &_profiler.gPlayerBones->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBones->szItems), _profiler.gPlayerBones->Domain.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerSnapLines->szName.c_str(), &_profiler.gPlayerSnapLines->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerSnapLines->szItems), _profiler.gPlayerSnapLines->Domain.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerInformation->szName.c_str(), &_profiler.gPlayerInformation->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerWeapons->szName.c_str(), &_profiler.gPlayerWeapons->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerBulletTracers->szName.c_str(), &_profiler.gPlayerBulletTracers->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAgents->szName.c_str(), &_profiler.gAgents->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gMissiles->szName.c_str(), &_profiler.gMissiles->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gItems->szName.c_str(), &_profiler.gItems->Current.bValue))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_MISCELLANEOUS:
				{
					if (ImGui::Checkbox(_profiler.gKillSpam->szName.c_str(), &_profiler.gKillSpam->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gNameStealer->szName.c_str(), &_profiler.gNameStealer->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gThirdPersonAntiAim->szName.c_str(), &_profiler.gThirdPersonAntiAim->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerCrossHair->szName.c_str(), &_profiler.gPlayerCrossHair->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerCompass->szName.c_str(), &_profiler.gPlayerCompass->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerRadar->szName.c_str(), &_profiler.gPlayerRadar->Current.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Console", &Menu.bShowConsole))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Player List", &Menu.PlayerList.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Host Menu", &Menu.HostMenu.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Memory Editor", &Menu.bShowMemoryEditor))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gRiotShield->szName.c_str(), &_profiler.gRiotShield->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gRiotShield->szItems), _profiler.gRiotShield->Domain.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_TWEAKS:
				{
					if (ImGui::SliderInt(_profiler.gAimBone->szName.c_str(), &_profiler.gAimBone->Current.iValue, _profiler.gAimBone->Domain.iMin, _profiler.gAimBone->Domain.iMax, szBones[_profiler.gAimBone->Current.iValue].first.c_str()))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAimAngle->szName.c_str(), &_profiler.gAimAngle->Current.iValue, _profiler.gAimAngle->Domain.iMin, _profiler.gAimAngle->Domain.iMax, "%d degrees"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAimPower->szName.c_str(), &_profiler.gAimPower->Current.iValue, _profiler.gAimPower->Domain.iMin, _profiler.gAimPower->Domain.iMax, "%d percent"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoAimTime->szName.c_str(), &_profiler.gAutoAimTime->Current.iValue, _profiler.gAutoAimTime->Domain.iMin, _profiler.gAutoAimTime->Domain.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoAimDelay->szName.c_str(), &_profiler.gAutoAimDelay->Current.iValue, _profiler.gAutoAimDelay->Domain.iMin, _profiler.gAutoAimDelay->Domain.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoZoomDelay->szName.c_str(), &_profiler.gAutoZoomDelay->Current.iValue, _profiler.gAutoZoomDelay->Domain.iMin, _profiler.gAutoZoomDelay->Domain.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoFireDelay->szName.c_str(), &_profiler.gAutoFireDelay->Current.iValue, _profiler.gAutoFireDelay->Domain.iMin, _profiler.gAutoFireDelay->Domain.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gRecoilFactor->szName.c_str(), &_profiler.gRecoilFactor->Current.flValue, _profiler.gRecoilFactor->Domain.flMin, _profiler.gRecoilFactor->Domain.flMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gSpreadFactor->szName.c_str(), &_profiler.gSpreadFactor->Current.flValue, _profiler.gSpreadFactor->Domain.flMin, _profiler.gSpreadFactor->Domain.flMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat("Field Of View", &FindVariable("cg_fov")->Current.flValue, FindVariable("cg_fov")->Domain.flMin, FindVariable("cg_fov")->Domain.flMax, "%.0f fov"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt("Frames Per Second", (int*)&FindVariable("com_maxfps")->Current.dwValue, FindVariable("com_maxfps")->Domain.dwMin, FindVariable("com_maxfps")->Domain.dwMax, "%d fps"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gAimBone->Current.iValue = _profiler.gAimBone->Reset.iValue;
						_profiler.gAimAngle->Current.iValue = _profiler.gAimAngle->Reset.iValue;
						_profiler.gAimPower->Current.iValue = _profiler.gAimPower->Reset.iValue;
						_profiler.gAutoAimTime->Current.iValue = _profiler.gAutoAimTime->Reset.iValue;
						_profiler.gAutoAimDelay->Current.iValue = _profiler.gAutoAimDelay->Reset.iValue;
						_profiler.gAutoZoomDelay->Current.iValue = _profiler.gAutoZoomDelay->Reset.iValue;
						_profiler.gAutoFireDelay->Current.iValue = _profiler.gAutoFireDelay->Reset.iValue;
						_profiler.gRecoilFactor->Current.flValue = _profiler.gRecoilFactor->Reset.flValue;
						_profiler.gSpreadFactor->Current.flValue = _profiler.gSpreadFactor->Reset.flValue;

						FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
						FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_STYLES:
				{
					_drawing.ColorPicker(_profiler.gColorAxis->szName, _profiler.gColorAxis->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorAxis->szName.c_str(), _profiler.gColorAxis->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					_drawing.ColorPicker(_profiler.gColorAllies->szName, _profiler.gColorAllies->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorAllies->szName.c_str(), _profiler.gColorAllies->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					_drawing.ColorPicker(_profiler.gColorAccents->szName, _profiler.gColorAccents->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorAccents->szName.c_str(), _profiler.gColorAccents->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					_drawing.ColorPicker(_profiler.gColorCrossHair->szName, _profiler.gColorCrossHair->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorCrossHair->szName.c_str(), _profiler.gColorCrossHair->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					_drawing.ColorPicker(_profiler.gColorText->szName, _profiler.gColorText->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorText->szName.c_str(), _profiler.gColorText->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					_drawing.ColorPicker(_profiler.gColorShadow->szName, _profiler.gColorShadow->Current.cValue);
					ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::ColorEdit4(_profiler.gColorShadow->szName.c_str(), _profiler.gColorShadow->Current.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gMenuColor->szName.c_str(), &_profiler.gMenuColor->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuColor->szItems), _profiler.gMenuColor->Domain.iMax))
					{
						WritePrivateProfileString("MenuStyle", "COLOR", std::to_string(_profiler.gMenuColor->Current.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuCursor->szName.c_str(), &_profiler.gMenuCursor->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuCursor->szItems), _profiler.gMenuCursor->Domain.iMax))
					{
						WritePrivateProfileString("MenuStyle", "CURSOR", std::to_string(_profiler.gMenuCursor->Current.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuFont->szName.c_str(), &_profiler.gMenuFont->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuFont->szItems), _profiler.gMenuFont->Domain.iMax))
					{
						WritePrivateProfileString("MenuStyle", "FONT", std::to_string(_profiler.gMenuFont->Current.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gColorAxis->Current.cValue = _profiler.gColorAxis->Reset.cValue;
						_profiler.gColorAllies->Current.cValue = _profiler.gColorAllies->Reset.cValue;
						_profiler.gColorAccents->Current.cValue = _profiler.gColorAccents->Reset.cValue;
						_profiler.gColorCrossHair->Current.cValue = _profiler.gColorCrossHair->Reset.cValue;
						_profiler.gColorText->Current.cValue = _profiler.gColorText->Reset.cValue;
						_profiler.gColorShadow->Current.cValue = _profiler.gColorShadow->Reset.cValue;

						Menu.bWriteLog = true;
					}
				} break;
				}

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::EndChild();

				if (ImGui::Button("Set as Default", ImVec2(156.0f, 30.0f)))
				{
					_profiler.SaveProfile("");
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);

				static bool bSaveButton;
				if ((bSaveButton = ImGui::Button("Save Profile", ImVec2(156.0f, 30.0f))))
				{
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);
				LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(bSaveButton, Menu.SaveDialog.getLastDirectory(), NULL, ".xml", "Save Profile");

				if (strlen(szSavePath))
				{
					_profiler.SaveProfile(szSavePath);
					strcpy_s(Menu.szProfilePath, szSavePath);
				}

				static bool bLoadButton;
				if ((bLoadButton = ImGui::Button("Load Profile", ImVec2(156.0f, 30.0f))))
				{
					Menu.bWriteLog = true;
				}
				LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(bLoadButton, Menu.LoadDialog.getLastDirectory(), ".xml", "Load Profile");

				if (strlen(szLoadPath))
				{
					_profiler.LoadProfile(szLoadPath);
					strcpy_s(Menu.szProfilePath, szLoadPath);
				}

				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
				ImGui::InputText("", Menu.szProfilePath, IM_ARRAYSIZE(Menu.szProfilePath), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();
				ImGui::End();

				if (Menu.bShowMemoryEditor)
				{
					Menu.MemEdit.DrawWindow("MEMORY EDITOR", &Menu.bShowMemoryEditor, hIw6mp64_ship.lpBaseOfDll, hIw6mp64_ship.SizeOfImage, (size_t)hIw6mp64_ship.lpBaseOfDll);
				}

				if (Menu.bShowConsole)
				{
					_console.Init();
					_console.Draw("CONSOLE", &Menu.bShowConsole);
				}

				if (Menu.PlayerList.bShowWindow && LocalClientIsInGame())
				{
					if (Menu.PlayerList.bWriteLog)
					{
						ImGui::LogToFile();
						Menu.PlayerList.bWriteLog = false;
					}

					ImGui::SetNextWindowSize(ImVec2(400.0f, 480.0f));
					ImGui::Begin("PLAYER LIST", &Menu.PlayerList.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
					ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);

					for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
					{
						if (CharacterInfo[i].iInfoValid)
						{
							GetPlayerAddr(&Menu.PlayerList.NetAddr[i], GetCurrentSession(), ClientInfo[i].iClientNum);

							ImGui::PushID(i);

							if (ImGui::RadioButton("", &_targetList.iRiotShieldTarget, i))
							{
								Menu.PlayerList.bWriteLog = true;
							} ImGui::PopID(); ImGui::SameLine();

							ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue);

							if (ImGui::Checkbox("", (bool*)&_targetList.vIsTarget[i]))
							{
								Menu.PlayerList.bWriteLog = true;
							} ImGui::PopID(); ImGui::SameLine();

							if (*(int*)OFF_ISCURRENTHOST)
							{
								ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue * 2);

								if (ImGui::Button("Crash", ImVec2(50.0f, 0.0f)))
								{
									Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[i], 0, "\x5E\x01\x3D\x3D\xFF");
									Menu.PlayerList.bWriteLog = true;
								} ImGui::PopID(); ImGui::SameLine();
							}

							ImGui::PushItemWidth(150.0f);
							ImGui::InputText(VariadicText("%i: %s", ClientInfo[i].iClientNum, ClientInfo[i].szName).c_str(),
								(LPSTR)VariadicText("%u.%u.%u.%u",
									(BYTE)Menu.PlayerList.NetAddr[i].szIP[0],
									(BYTE)Menu.PlayerList.NetAddr[i].szIP[1],
									(BYTE)Menu.PlayerList.NetAddr[i].szIP[2],
									(BYTE)Menu.PlayerList.NetAddr[i].szIP[3]).c_str(),
								1024, ImGuiInputTextFlags_ReadOnly);
							ImGui::PopItemWidth();
						}
					}

					ImGui::EndChild();
					ImGui::End();
				}

				if (Menu.HostMenu.bShowWindow && LocalClientIsInGame() && *(int*)OFF_ISCURRENTHOST && !*(bool*)OFF_ISALIENSMODE)
				{
					if (Menu.HostMenu.bWriteLog)
					{
						ImGui::LogToFile();
						Menu.HostMenu.bWriteLog = false;
					}

					ImGui::SetNextWindowSize(ImVec2(484.0f, 708.0f));
					ImGui::Begin("HOST MENU", &Menu.HostMenu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

					for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
						if (CharacterInfo[i].iInfoValid)
							Menu.HostMenu.vPlayers.push_back(std::make_pair(ClientInfo[i].iClientNum, ClientInfo[i].szName));

					ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
					if (ImGui::BeginCombo("", ClientInfo[Menu.HostMenu.iPlayer].szName))
					{
						Menu.HostMenu.GetPlayerSelection();
						ImGui::EndCombo();

						Menu.HostMenu.bWriteLog = true;
					} ImGui::PopItemWidth();

					ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);
					ImGui::Separator();
					ImGui::NewLine();

					if (ImGui::Checkbox("God Mode", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bGodMode))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(292.0f);

					if (ImGui::Checkbox("No Clip", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bNoClip))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Infinite Ammo", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bInfiniteAmmo))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(292.0f);

					if (ImGui::Checkbox("Invisibility", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bInvisibility))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Super Speed", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bSuperSpeed))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(292.0f);

					if (ImGui::Checkbox("Freeze Position", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bFreezePosition))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::InputText("Name", PlayerState[Menu.HostMenu.iPlayer].ClientState.szName, 16))
					{
						Menu.HostMenu.bWriteLog = true;
					}

					if (ImGui::Combo("Team", (int*)&PlayerState[Menu.HostMenu.iPlayer].ClientState.iTeam, Menu.HostMenu.szTeam.data(), TEAM_MAX))
					{
						TeamChanged(ClientInfo[Menu.HostMenu.iPlayer].iClientNum);
						Menu.HostMenu.bWriteLog = true;
					}

					if (ImGui::InputInt("Health", &GEntity[Menu.HostMenu.iPlayer].iHealth))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					for (auto& WeaponID : vWeaponIDs)
					{
						char szComplete[1024] = { NULL }, szDisplay[1024] = { NULL };

						GetWeaponNameComplete(WeaponID, false, szComplete, 1024);
						GetWeaponDisplayName(WeaponID, false, szDisplay, 1024);

						Menu.HostMenu.vWeaponCompleteNames.push_back(_strdup(acut::StripColorCodes(szComplete).c_str()));
						Menu.HostMenu.vWeaponDisplayNames.push_back(_strdup(acut::StripColorCodes(szDisplay).c_str()));
					}

					if (ImGui::Combo("Weapon", &Menu.HostMenu.iWeaponID, Menu.HostMenu.vWeaponDisplayNames.data(), (int)vWeaponIDs.size()))
					{
						Menu.HostMenu.bWriteLog = true;
					} Menu.HostMenu.vWeaponCompleteNames.clear(); Menu.HostMenu.vWeaponDisplayNames.clear();

					if (ImGui::Button("Give Weapon", ImVec2(150.0f, 25.0f)))
					{
						TakePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[Menu.HostMenu.iPlayer]));
						GivePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], vWeaponIDs[Menu.HostMenu.iWeaponID], false, false, true);
						GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", vWeaponIDs[Menu.HostMenu.iWeaponID]));
						AddAmmo(&PlayerState[Menu.HostMenu.iPlayer], vWeaponIDs[Menu.HostMenu.iWeaponID], false, 255, true);

						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::Button("Take Weapon", ImVec2(150.0f, 25.0f)))
					{
						TakePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[Menu.HostMenu.iPlayer]));
						GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", 0));

						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo("Perk", &Menu.HostMenu.iPerkID, szPerkIDs.data(), (int)vPerkIDs.size()))
					{
						Menu.HostMenu.bWriteLog = true;
					}

					if (ImGui::Button("Enable Perk", ImVec2(150.0f, 25.0f)))
					{
						EnablePerk(Menu.HostMenu.iPlayer, vPerkIDs[Menu.HostMenu.iPerkID]);

						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::Button("Disable Perk", ImVec2(150.0f, 25.0f)))
					{
						DisablePerk(Menu.HostMenu.iPlayer, vPerkIDs[Menu.HostMenu.iPerkID]);

						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::SliderFloat3("Position", PlayerState[Menu.HostMenu.iPlayer].vOrigin, -8192.0f, 8192.0f, "%.0f"))
					{
						Menu.HostMenu.bWriteLog = true;
					}

					if (ImGui::Button("Teleport To", ImVec2(150.0f, 25.0f)))
					{
						VectorCopy(PlayerState[Menu.HostMenu.iPlayer].vOrigin, PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin);
						Menu.HostMenu.bWriteLog = true;
					} ImGui::SameLine(0.0f, 4.0f);

					if (ImGui::Button("Teleport From", ImVec2(150.0f, 25.0f)))
					{
						VectorCopy(PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin, PlayerState[Menu.HostMenu.iPlayer].vOrigin);
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Kick", ImVec2(304.0f, 25.0f)))
					{
						GameDropClient(ClientInfo[Menu.HostMenu.iPlayer].iClientNum, Menu.HostMenu.szKickReason);
						Menu.HostMenu.bWriteLog = true;
					}

					if (ImGui::InputText("Reason", Menu.HostMenu.szKickReason, 64))
					{
						Menu.HostMenu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::EndChild();

					Menu.HostMenu.vPlayers.clear();

					ImGui::End();
				}
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			
			if (Menu.bStyleChanged)
			{
				RefreshInterface(_profiler.gMenuColor->Current.iValue, _profiler.gMenuCursor->Current.iValue, _profiler.gMenuFont->Current.iValue);
				Menu.bStyleChanged = false;
			}

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE)
			{
				pDevice->Release();
				pDeviceContext->Release();

				ImGui_ImplWin32_Shutdown();
				ImGui_ImplDX11_Shutdown();
				ImGui::DestroyContext();

				bInitialized = false;
			}

			static int iTime = clock();

			if (clock() - iTime > max(*(int*)OFF_FRAMETIME, *(int*)OFF_PING))
			{
				if (LocalClientIsInGame() && !IsMigrating() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
				{
					if (_profiler.gNameSpam->Current.bValue)
					{
						std::string szUserName = acut::RandomANString(0);

						strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })), strlen(szUserName.c_str()) + 1, szUserName.c_str(), 32);
						Cbuf_AddText(VariadicText("name \"%s\"\n", szUserName.c_str()));
					}

					if (_profiler.gChatSpam->Current.bValue)
					{
						Cbuf_AddText(VariadicText("say \"%s\"\n", _profiler.gChatSpamMessage->Current.szValue));
					}
				}

				iTime = clock();
			}
		}
	}
}

//=====================================================================================