#pragma once

#include "Level.h"
#include "graphics.h"
#include "HPTimer.h"
#include "resource.h"

class GameController
{
	GameController() { Loading = true; }
public:
	static Level* currentLevel;
	static bool Run;
	static bool Loading;
	static HPTimer* m_Timer;
	static void Init(HWND hwnd);
	static void LoadInitialLevel(Level* level);
	static void SwitchLevel(Level* level);
	static void Render();
	static void Update();
	static void UpdateTimer();
	static void Update(WPARAM wParam);
	static void Cleanup();
	static HWND hWnd;
	static D2D1_POINT_2F GetMousePositionForCurrentDpi(LPARAM lParam, ID2D1Factory* factory);
	static D2D1_POINT_2F p;
};