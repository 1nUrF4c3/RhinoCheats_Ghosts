//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"
#include "RapidXML/rapidxml_wrap.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cProfiler
	{
	public:

		struct sCvar
		{
			std::string szLabel;
			std::vector<std::string> szItems;

			union uCvarValue
			{
				bool bValue;
				int iValue;
				float flValue;
				DWORD dwValue;
				ImVec4 cValue;
				LPSTR szValue;

				uCvarValue(bool value) : bValue(value) {}
				uCvarValue(int value) : iValue(value) {}
				uCvarValue(float value) : flValue(value) {}
				uCvarValue(DWORD value) : dwValue(value) {}
				uCvarValue(ImVec4 value) : cValue(value) {}
				uCvarValue(LPSTR value) : szValue(value) {}
			} Custom, Default;

			union uMinValue
			{
				int	iMin;
				float flMin;
				DWORD dwMin;

				uMinValue(int min) : iMin(min) {}
				uMinValue(float min) : flMin(min) {}
				uMinValue(DWORD min) : dwMin(min) {}
			} MinValue;

			union uMaxValue
			{
				int	iMax;
				float flMax;
				DWORD dwMax;

				uMaxValue(int max) : iMax(max) {}
				uMaxValue(float max) : flMax(max) {}
				uMaxValue(DWORD max) : dwMax(max) {}
			} MaxValue;

			sCvar(std::string name, std::vector<std::string> items, bool value) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(NULL), MaxValue(NULL) {}
			sCvar(std::string name, std::vector<std::string> items, int value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(std::string name, std::vector<std::string> items, float value, float min, float max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(std::string name, std::vector<std::string> items, DWORD value, DWORD min, DWORD max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(std::string name, std::vector<std::string> items, ImVec4 value) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(NULL), MaxValue(NULL) {}
			sCvar(std::string name, std::vector<std::string> items, LPSTR value) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(NULL), MaxValue(NULL) {}
		};

		typedef enum
		{
			MENU_TAB_AIMBOT,
			MENU_TAB_WALLHACK,
			MENU_TAB_MISCELLANEOUS,
			MENU_TAB_TWEAKS,
			MENU_TAB_STYLES,
			MENU_TAB_MAX
		} eMenuTab;

		typedef enum
		{
			MENU_COLOR_NEUTRAL,
			MENU_COLOR_NEUTRAL_NEON,
			MENU_COLOR_RED,
			MENU_COLOR_RED_NEON,
			MENU_COLOR_ORANGE,
			MENU_COLOR_ORANGE_NEON,
			MENU_COLOR_YELLOW,
			MENU_COLOR_YELLOW_NEON,
			MENU_COLOR_GREEN,
			MENU_COLOR_GREEN_NEON,
			MENU_COLOR_BLUE,
			MENU_COLOR_BLUE_NEON,
			MENU_COLOR_PURPLE,
			MENU_COLOR_PURPLE_NEON,
			MENU_COLOR_RAINBOW,
			MENU_COLOR_RAINBOW_NEON,
			MENU_COLOR_MAX
		} eMenuColor;

		typedef enum
		{
			MENU_CURSOR_BLACK,
			MENU_CURSOR_WHITE,
			MENU_CURSOR_MAX
		} eMenuCursor;

		typedef enum
		{
			MENU_FONT_LIGHT,
			MENU_FONT_MEDIUM,
			MENU_FONT_BOLD,
			MENU_FONT_MAX
		} eMenuFont;

		typedef enum
		{
			AIMBOT_MODE_OFF,
			AIMBOT_MODE_MANUAL,
			AIMBOT_MODE_AUTO,
			AIMBOT_MODE_MAX
		} eAimBotMode;

		typedef enum
		{
			ANTIAIM_OFF,
			ANTIAIM_SPINBOT,
			ANTIAIM_JITTERBOT,
			ANTIAIM_RANDOMIZED,
			ANTIAIM_REVERSED,
			ANTIAIM_MAX
		} eAntiAim;

		typedef enum
		{
			BONESCAN_OFF,
			BONESCAN_ONTIMER,
			BONESCAN_IMMEDIATE,
			BONESCAN_MAX
		} eBoneScan;

		typedef enum
		{
			SORT_METHOD_FOV,
			SORT_METHOD_DISTANCE,
			SORT_METHOD_MAX
		} eSortMethod;

		typedef enum
		{
			WALLHACK_MODE_AXIS,
			WALLHACK_MODE_ALLIES,
			WALLHACK_MODE_ALL,
			WALLHACK_MODE_MAX
		} eWallHackMode;

		typedef enum
		{
			PLAYER_BOXES_OFF,
			PLAYER_BOXES_BORDER,
			PLAYER_BOXES_CORNER,
			PLAYER_BOXES_BORDER_FILLED,
			PLAYER_BOXES_CORNER_FILLED,
			PLAYER_BOXES_BORDER_3D,
			PLAYER_BOXES_CORNER_3D,
			PLAYER_BOXES_MAX
		} ePlayerBoxes;

		typedef enum
		{
			PLAYER_BONES_OFF,
			PLAYER_BONES_DOTS,
			PLAYER_BONES_LINES,
			PLAYER_BONES_MAX
		} ePlayerBones;

		typedef enum
		{
			PLAYER_SNAPLINES_OFF,
			PLAYER_SNAPLINES_TOP,
			PLAYER_SNAPLINES_BOTTOM,
			PLAYER_SNAPLINES_CROSSHAIR,
			PLAYER_SNAPLINES_MAX
		} ePlayerSnapLines;

		typedef enum
		{
			RIOTSHIELD_OFF,
			RIOTSHIELD_IGNOREPLAYER,
			RIOTSHIELD_TARGETFEET,
			RIOTSHIELD_MAX
		} eRiotShield;

		typedef enum
		{
			MASSKILL_OFF,
			MASSKILL_AXIS,
			MASSKILL_ALLIES,
			MASSKILL_ALL,
			MASSKILL_MAX
		} eMassKill;

		std::shared_ptr<sCvar> gMenuTabs = std::make_shared<sCvar>("Menu Tab", std::vector<std::string>({ "Aimbot", "ESP", "Misc.", "Tweaks", "Styles" }), MENU_TAB_AIMBOT, MENU_TAB_AIMBOT, MENU_TAB_MAX);
		std::shared_ptr<sCvar> gMenuColor = std::make_shared<sCvar>("Menu Color", std::vector<std::string>({ "Neutral", "Neutral (Neon)", "Red", "Red (Neon)", "Orange", "Orange (Neon)", "Yellow", "Yellow (Neon)", "Green", "Green (Neon)", "Blue", "Blue (Neon)", "Purple", "Purple (Neon)", "Rainbow", "Rainbow (Neon)" }), MENU_COLOR_NEUTRAL, MENU_COLOR_NEUTRAL, MENU_COLOR_MAX);
		std::shared_ptr<sCvar> gMenuCursor = std::make_shared<sCvar>("Menu Cursor", std::vector<std::string>({ "Black", "White" }), MENU_CURSOR_BLACK, MENU_CURSOR_BLACK, MENU_CURSOR_MAX);
		std::shared_ptr<sCvar> gMenuFont = std::make_shared<sCvar>("Menu Font", std::vector<std::string>({ "Light", "Medium", "Bold" }), MENU_FONT_LIGHT, MENU_FONT_LIGHT, MENU_FONT_MAX);

		std::shared_ptr<sCvar> gAimBotMode = std::make_shared<sCvar>("Mode", std::vector<std::string>({ "Off", "Manual", "Auto" }), AIMBOT_MODE_OFF, AIMBOT_MODE_OFF, AIMBOT_MODE_MAX);
		std::shared_ptr<sCvar> gAutoZoom = std::make_shared<sCvar>("Autozoom", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAutoFire = std::make_shared<sCvar>("Autofire", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAutoWall = std::make_shared<sCvar>("Autowall", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gSilentAim = std::make_shared<sCvar>("Silent-Aim", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetAgents = std::make_shared<sCvar>("Target Agents", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetMissiles = std::make_shared<sCvar>("Target Missiles", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAntiAim = std::make_shared<sCvar>("Anti-Aim", std::vector<std::string>({ "Off", "Spinbot", "Jitterbot", "Randomized", "Reversed" }), ANTIAIM_OFF, ANTIAIM_OFF, ANTIAIM_MAX);
		std::shared_ptr<sCvar> gBoneScan = std::make_shared<sCvar>("Bonescan", std::vector<std::string>({ "Off", "On Timer", "Immediate" }), BONESCAN_OFF, BONESCAN_OFF, BONESCAN_MAX);
		std::shared_ptr<sCvar> gSortMethod = std::make_shared<sCvar>("Sort Method", std::vector<std::string>({ "Field of View", "Distance" }), SORT_METHOD_FOV, SORT_METHOD_FOV, SORT_METHOD_MAX);
		
		std::shared_ptr<sCvar> gWallHackMode = std::make_shared<sCvar>("Mode", std::vector<std::string>({ "Axis", "Allies", "All" }), WALLHACK_MODE_AXIS, WALLHACK_MODE_AXIS, WALLHACK_MODE_MAX);
		std::shared_ptr<sCvar> gPlayerBoxes = std::make_shared<sCvar>("Boxes", std::vector<std::string>({ "Off", "Border", "Corner", "Border Filled", "Corner Filled", "Border 3D", "Corner 3D" }), PLAYER_BOXES_OFF, PLAYER_BOXES_OFF, PLAYER_BOXES_MAX);
		std::shared_ptr<sCvar> gPlayerBones = std::make_shared<sCvar>("Bones", std::vector<std::string>({ "Off", "Dots", "Lines" }), PLAYER_BONES_OFF, PLAYER_BONES_OFF, PLAYER_BONES_MAX);
		std::shared_ptr<sCvar> gPlayerSnapLines = std::make_shared<sCvar>("Snaplines", std::vector<std::string>({ "Off", "Top", "Bottom", "Crosshair" }), PLAYER_SNAPLINES_OFF, PLAYER_SNAPLINES_OFF, PLAYER_SNAPLINES_MAX);
		std::shared_ptr<sCvar> gPlayerDistances = std::make_shared<sCvar>("Distances", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerNames = std::make_shared<sCvar>("Names", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerWeapons = std::make_shared<sCvar>("Weapons", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAgents = std::make_shared<sCvar>("Agents", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gMissiles = std::make_shared<sCvar>("Missiles", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gItems = std::make_shared<sCvar>("Items", std::vector<std::string>(), false);

		std::shared_ptr<sCvar> gNameStealer = std::make_shared<sCvar>("Name Stealer", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gThirdPersonAntiAim = std::make_shared<sCvar>("Third Person Anti-Aim", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerCrossHair = std::make_shared<sCvar>("Crosshair", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerCompass = std::make_shared<sCvar>("Compass", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerRadar = std::make_shared<sCvar>("Radar", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gRiotShield = std::make_shared<sCvar>("Riotshields", std::vector<std::string>({ "Off", "Ignore Player", "Target Feet" }), RIOTSHIELD_OFF, RIOTSHIELD_OFF, RIOTSHIELD_MAX);

		std::shared_ptr<sCvar> gAimBone = std::make_shared<sCvar>("Aimbone", std::vector<std::string>(), BONE_HEAD, BONE_HEAD, BONE_MAX - 1);
		std::shared_ptr<sCvar> gAimAngle = std::make_shared<sCvar>("Aimangle", std::vector<std::string>(), 180.0f, 5.0f, 180.0f);
		std::shared_ptr<sCvar> gAimPower = std::make_shared<sCvar>("Aimpower", std::vector<std::string>(), 100, 5, 100);
		std::shared_ptr<sCvar> gAutoAimTime = std::make_shared<sCvar>("Autoaim Time", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoAimDelay = std::make_shared<sCvar>("Autoaim Delay", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoZoomDelay = std::make_shared<sCvar>("Autozoom Delay", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoFireDelay = std::make_shared<sCvar>("Autofire Delay", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gRecoilFactor = std::make_shared<sCvar>("Recoil Factor", std::vector<std::string>(), 1.0f, 0.0f, 1.0f);
		std::shared_ptr<sCvar> gSpreadFactor = std::make_shared<sCvar>("Spread Factor", std::vector<std::string>(), 1.0f, 0.0f, 1.0f);

		std::shared_ptr<sCvar> gColorAxis = std::make_shared<sCvar>("Axis", std::vector<std::string>(), ImVec4(ByteToFloat(0), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorAllies = std::make_shared<sCvar>("Allies", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorRiotShield = std::make_shared<sCvar>("Riotshield", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(0), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorCrossHair = std::make_shared<sCvar>("Crosshair", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(0), ByteToFloat(255), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorText = std::make_shared<sCvar>("Text", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(255), ByteToFloat(255), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorShadow = std::make_shared<sCvar>("Shadow", std::vector<std::string>(), ImVec4(ByteToFloat(0), ByteToFloat(0), ByteToFloat(0), ByteToFloat(255)));

		std::shared_ptr<sCvar> gNameSpam = std::make_shared<sCvar>("Namespam", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gChatSpam = std::make_shared<sCvar>("Chatspam", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gKillSpam = std::make_shared<sCvar>("Killspam", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gChatSpamMessage = std::make_shared<sCvar>("Chatspam Message", std::vector<std::string>(), _strdup(""));
		std::shared_ptr<sCvar> gKillSpamMessage = std::make_shared<sCvar>("Killspam Message", std::vector<std::string>(), _strdup(""));
		std::shared_ptr<sCvar> gSuperJump = std::make_shared<sCvar>("Super Jump", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gMassKill = std::make_shared<sCvar>("Masskill", std::vector<std::string>(), MASSKILL_OFF, MASSKILL_OFF, MASSKILL_MAX - 1);
		std::shared_ptr<sCvar> gBunnyHop = std::make_shared<sCvar>("Bunny Hop", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTeaBag = std::make_shared<sCvar>("Tea Bag", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTeaBagMessage = std::make_shared<sCvar>("Tea Bag Message", std::vector<std::string>(), _strdup(""));

		acut::XmlDoc<char> XML;

		bool SaveProfile(const std::string& path);
		bool LoadProfile(const std::string& path);
		void DisableAll();
	} extern _profiler;
}

//=====================================================================================
