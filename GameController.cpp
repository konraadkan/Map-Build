#include "GameController.h"

Level* GameController::currentLevel;
bool GameController::Loading;
HPTimer* GameController::m_Timer;
bool GameController::Run;
D2D1_POINT_2F GetMousePositionForCurrentDpi(LPARAM lParam, ID2D1Factory* factory);
D2D1_POINT_2F GameController::p;
HWND GameController::hWnd;

void GameController::Init(HWND hwnd)
{
	currentLevel = 0;
	m_Timer = new HPTimer;
	Run = true;
	hWnd = hwnd;
}

void GameController::LoadInitialLevel(Level* level)
{
	Loading = true;
	currentLevel = level;
	currentLevel->Load();
	Loading = false;
}

void GameController::SwitchLevel(Level* level)
{
	Loading = true;
	currentLevel->Unload();
	delete currentLevel;
	currentLevel = level;
	currentLevel->Load();
	Loading = false;
}

void GameController::Render()
{
	if (Loading) return;
	if (currentLevel) currentLevel->Render();
}

void GameController::Update()
{
	if (m_Timer)
	{
		currentLevel->Update((float)m_Timer->GetTimeTotal(), (float)m_Timer->GetTimeDelta(), p);
	}
}

void GameController::UpdateTimer()
{
	if (m_Timer)
		m_Timer->Update();
}

void GameController::Update(WPARAM wParam)
{
	currentLevel->Update(wParam, p);
}

void GameController::Cleanup()
{
	if (currentLevel) currentLevel->Unload();
	delete currentLevel;
	currentLevel = 0;
	if (m_Timer) delete m_Timer;
	Run = false;
}

D2D1_POINT_2F GameController::GetMousePositionForCurrentDpi(LPARAM lParam, ID2D1Factory* factory)
{
	static D2D1_POINT_2F dpi = { 96,96 };	//default dpi

	factory->GetDesktopDpi(&dpi.x, &dpi.y);

	return D2D1::Point2F(static_cast<int>(static_cast<short>(LOWORD(lParam))) * 96 / dpi.x,
		static_cast<int>(static_cast<short>(HIWORD(lParam))) * 96 / dpi.y);
}