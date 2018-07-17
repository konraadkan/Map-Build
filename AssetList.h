#pragma once
#include "SpriteSheet.h"
#include <string>
#include <queue>

class Asset
{
public:
	long m_MenuID;
	long m_Layer;
	wchar_t m_Status = 0;
	std::string m_Type;
	std::string m_Submenu;
	std::string m_Name;
	std::string m_SpritePath;
	std::string m_IconPath;
	bool m_KeepAspect;
	bool m_KeepIconAspect;
	bool m_DefaultInitOrder;
	COLORREF_F m_BgColor;
	SpriteSheet* m_Sprite;
	SpriteSheet* m_Icon;
	std::queue<D2D1_RECT_F> m_SpriteFrames;
	std::queue<D2D1_RECT_F> m_IconFrames;
};