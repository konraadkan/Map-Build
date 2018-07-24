#pragma once
#include <d2d1.h>

#define VERSION 6.05
#define PI 3.14159265359f

#define DEFAULT_WINDOW_WIDTH	1920.0f
#define DEFAULT_WINDOW_HEIGHT	1080.0f

static float WINDOW_WIDTH = DEFAULT_WINDOW_WIDTH;
static float WINDOW_HEIGHT = DEFAULT_WINDOW_HEIGHT;

#define MENU_NEW				0x1001
#define MENU_OPEN				0x1002
#define MENU_SAVE				0x1003
#define MENU_QUIT				0x1004
#define MENU_ADDLAYER			0x1005
#define MENU_CHANGEMODE			0x1006
#define MENU_TOGGLEINIT			0x1007
#define MENU_KEEPASPECT			0x1008
#define MENU_TOGGLECOLO			0x1009
#define MENU_LOCKTOGRID			0x100A
#define MENU_GRIDONTOP			0x100B
#define MENU_GEOMETRY			0x100C
#define MENU_ADDROOM			0x100D			//this variable is to be used when i add the room option, will work in a manner akin to how layers work
#define MENU_EDITBUTTONMAPPING	0x100E

#define MENU_LAYER0				0x1100			//0x1100-> 0x11FF are now reserved for layers
#define MENU_ROOM0				0x1400			//0x1400-> 0x14FF are now reserved for rooms
#define MENU_SHOWLAYER0			0x2100			//0x2100-> 0x21FF are now reserved for show/hide layers
#define MENU_SHOWROOM0			0x2200			//0x2200-> 0x22FF are now reserved for show/hide rooms

#define COLOR_START				0x1200			//0x1200-> 0x12FF are now reserved for colors
#define COLOR_CUSTOMS			0x1300			//0x1300-> 0x13FF are now reserved for colors

#define TEMP_DIR_NAME	"delme"
#define TEMP_DIR_NAME_W	L"delme"		//wide character

#define FIRE_EFFECT_PATH L"images\\fire_effect.spr3"

static D2D1_RECT_F m_DrawAreaRect = { 0.0f, 0.0f, WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT };
static D2D1_RECT_F m_MenuAreaRect = { WINDOW_WIDTH * 0.75f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };


//size categories
enum class Size
{
	m_Fine,								//probably won't use sizes smaller than tiny
	m_Diminutive,
	m_Tiny,
	m_Small,
	m_Medium,
	m_Large,
	m_Huge,
	m_Gargantuan,
	m_Colossal,
	m_Half,
	m_Original,
	m_TwoTimes,
	m_ThreeTimes,
	m_FourTimes
};

enum class Grid
{
	m_Square,
	m_Hexagon,
	m_Dimetrique
};
