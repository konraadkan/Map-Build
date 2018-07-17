#include "counter.h"
#include <string>

counter::counter()
{
	gfx = NULL;
	m_Count = 0;
	m_MenuID = -1;
}

counter::counter(Graphics* graphics)
{
	gfx = graphics;
	m_Count = 0;
	m_MenuID = -1;
}

counter::~counter()
{
	gfx = NULL;
	m_Count = 0;
	m_MenuID = 0;
}

void counter::m_ResetCount(long menuID)
{
	m_Count = 0;
	m_SetMenuID(menuID);
}

void counter::m_Decrease()
{
	if (m_Count > 0) m_Count--;
}

void counter::m_Increase()
{
	m_Count++;
}

void counter::m_SetCount(long newCount)
{
	m_Count = newCount;
}

void counter::m_SetMenuID(long menuID)
{
	m_MenuID = menuID;
}

void counter::m_SetDrawArea(D2D1_RECT_F drawArea)
{
	m_DrawArea = drawArea;
}

long counter::m_GetCount()
{
	return m_Count;
}

long counter::m_GetMenuID()
{
	return m_MenuID;
}

void counter::m_Draw()
{
	m_Draw(m_DrawArea);
}

void counter::m_Draw(D2D1_RECT_F position)
{
	float fontsize = 300.0f;
	if (gfx == NULL) return;
	gfx->FillEllipse({ (position.right - position.left) / 2.0f, 0.0f }, 200.0f, 128.0f, { 0.69f, 0.88f, 0.90f, 0.75f });
	gfx->BuildVariableText(fontsize, 1.0f);
	std::wstring sCount = std::to_wstring(m_Count);
	gfx->DrawVariableText(sCount.c_str(), position, { 0.0f, 0.0f, 0.0f, 0.75f }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	sCount.clear();
}

void counter::m_SetGfx(Graphics* graphics)
{
	gfx = graphics;
}

void counter::m_ReleaseGfx()
{
	gfx = NULL;
}