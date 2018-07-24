#pragma once
#include "keys.h"
#include "graphics.h"
#include <Windows.h>
#include <d2d1.h>

class ButtonMapper
{
private:
	Graphics* gfx;
	D2D1_SIZE_F m_WindowSize;
	D2D1_RECT_F commitRect;
	D2D1_RECT_F cancelRect;
	D2D1_SIZE_F buttonsize;
	long rowsize;
	void Render(KeyboardKeys m_Keyboard, D2D1_POINT_2F p, long value);
	long GetButtonPressed();
public:	
	ButtonMapper(KeyboardKeys& m_Keyboard, Graphics* graphics, D2D1_SIZE_F WindowSize);
	~ButtonMapper();
};
