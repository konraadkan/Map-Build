#include <cstdio>
#include "defines.h"
#include "GameController.h"
#include "Level1.h"
#include "graphics.h"
#include "stuff.h"
#include <string>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

#define MYCLASSNAME "MapBuild"
#define MYWINDOWNAME "Tabletop Mapping Tool"

HMENU BuildMenu()
{
	HMENU menu = CreateMenu();
	HMENU subMenu = CreatePopupMenu();

	AppendMenu(subMenu, MF_STRING, MENU_NEW, "New");
	AppendMenu(subMenu, MF_STRING, MENU_OPEN, "Open");
	AppendMenu(subMenu, MF_STRING, MENU_SAVE, "Save");
	AppendMenu(subMenu, MF_STRING, MENU_QUIT, "Quit");

	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT)subMenu, "File");

	return menu;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case MENU_NEW:
		GameController::SwitchLevel(new Level1);
		break;
	case MENU_ADDLAYER:
		GameController::currentLevel->AddLayerMenu();
		break;
	default:
		if (!GameController::Loading && GameController::currentLevel)
			GameController::Update(wParam);
	}

	if (wParam == MENU_QUIT || msg == WM_QUIT || msg == WM_DESTROY)
	{
		GameController::Cleanup();
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPWSTR cmd, int iCmdShow)
{
	WNDCLASSEX wndClass = {};
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.lpszClassName = MYCLASSNAME;

	RegisterClassEx(&wndClass);

	RECT rect = { 0, 0, (long)WINDOW_WIDTH, (long)WINDOW_HEIGHT };
	RECT desktop = {};

	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	HMENU hMenu = BuildMenu();
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, true, WS_EX_OVERLAPPEDWINDOW);
	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, MYCLASSNAME, MYWINDOWNAME, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, (desktop.right / 2) - (rect.right / 2), (desktop.bottom / 2) - (rect.bottom / 2), rect.right - rect.left, rect.bottom - rect.top, NULL, hMenu, hInstance, 0);
	MSG msg = {};

	Graphics* gfx = new Graphics();
	if (!gfx->Init(hWnd))
	{
		delete gfx;
		MessageBox(NULL, "Failed to initialize graphics class.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	
	char tPath[512];
	GetModuleFileName(NULL, tPath, 512);
	for (size_t i = strlen(tPath); i > 0; i--)
	{
		if (tPath[i] == '\\')
		{
			std::string dirPath(i + 1, ' ');
			std::copy(&tPath[0], &tPath[i], dirPath.begin());
			SetCurrentDirectory(dirPath.c_str());
			break;
		}
	}

	Level::Init(gfx);
	ItemMenuArea::Init(gfx);
	GameController::Init(hWnd);
	GameController::LoadInitialLevel(new Level1);
	GameController::p = GameController::GetMousePositionForCurrentDpi(msg.lParam, gfx->GetID2DFactory());
	
	if (lstrlenW(cmd))
	{
		if (cmd[0] == L'\"')
		{
			std::wstring sCmd(lstrlenW(cmd), L' ');
			std::copy(&cmd[1], &cmd[lstrlenW(cmd) - 1], sCmd.begin());
			if (!GameController::currentLevel)
				GameController::currentLevel = new Level1;
			GameController::currentLevel->Open(sCmd.c_str());
		}
		else
		{
			if (!GameController::currentLevel)
				GameController::currentLevel = new Level1;
			GameController::currentLevel->Open(cmd);
		}
	}

	while (GameController::Run)
	{//try to figure out how to use the zooming options rather than manipulating the sizes this way; not sure if htis is worth it
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_MOUSEMOVE)
			{
				GameController::p = GameController::GetMousePositionForCurrentDpi(msg.lParam, gfx->GetID2DFactory());
			}
		}
		else
		{
			GameController::Update();

			gfx->BeginDraw();
			GameController::Render();
			gfx->EndDraw();

			GameController::UpdateTimer();
		}
	}

	RemoveDirectory(TEMP_DIR_NAME);
	delete gfx;
	return 0;
}