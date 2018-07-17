#pragma once
#include <string>
#include <queue>
#include "defines.h"
#include "gfx_structs.h"
#include "SpriteSheet.h"

class ItemMenuItem
{
public:
	std::wstring m_ItemText;
	std::wstring m_SubMenu;
	FRECT m_DrawArea;
	D2D1_RECT_F src;
	D2D1_RECT_F dest;
	D2D1_POINT_2F m_StartPoint = { m_MenuAreaRect.left + 0.2f, 0.2f };
	D2D1_COLOR_F m_BgColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	SpriteSheet* m_Sprite = NULL;
	float m_Thickness = 1.0f;
	void SetSprite(SpriteSheet* spr) { m_Sprite = spr; };
	void SetBgColor(D2D1_COLOR_F color) { m_BgColor = color; }
	void DrawSprite(D2D1_POINT_2F pos)
	{
		if (!m_Sprite) return;

		m_Sprite->Draw(dest, src, 1.0f);
	}
	void DrawSprite(D2D1_POINT_2F pos, bool hideBgColor)
	{
		if (!hideBgColor)
			m_Sprite->gfx->FillRect(dest, m_BgColor);
		DrawSprite(pos);
	}

	void UpdateSprite(D2D1_POINT_2F position, D2D1_RECT_F frame, D2D1_RECT_F size, bool keepRatio = true);
};