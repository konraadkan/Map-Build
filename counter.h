#pragma once
#include "graphics.h"
#include "gfx_structs.h"

class counter
{
private:
	Graphics* gfx;
public:
	counter();
	counter(Graphics* graphics);
	~counter();
	void m_ResetCount(long menuID = -1);
	void m_Increase();
	void m_Decrease();
	void m_SetCount(long newCount);
	void m_SetMenuID(long menuID);
	void m_SetDrawArea(D2D1_RECT_F drawArea);
	void m_SetGfx(Graphics* graphics);
	void m_ReleaseGfx();
	
	long m_GetCount();
	long m_GetMenuID();

	void m_Draw();
	void m_Draw(D2D1_RECT_F position);
public:
	long m_Count = 0;
	long m_MenuID = -1;
	D2D1_RECT_F m_DrawArea = {};
};