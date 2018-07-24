#pragma once
#include "keys.h"
#include "graphics.h"
#include <Windows.h>

class ButtonMapper
{
private:
	Graphics* gfx;
	void Render(KeyboardKeys m_Keyboard, D2D1_POINT_2F p, long value);
	long GetButtonPressed();
public:	
	ButtonMapper(KeyboardKeys& m_Keyboard, Graphics* graphics);
	~ButtonMapper();
};
