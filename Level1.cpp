#include "Level1.h"
#include "GameController.h"

void Level1::Load()
{
	//loading things
	m_DrawArea = { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };
	m_BgColor = { 0.9f, 0.9f, 0.9f, 1.0f };
	m_CurrentLayer = 0;
	m_NumberLayers = 0;
	m_Size = 128.0f;
	m_Scale = 1.0f;
	DestroyMenu(m_Menu);
	m_Menu = CreateMenu();	
	BuildMenu();
	std::wstring s = L"PCs";

	if (!BuildAssetList(L"init.dat", m_AssetList))
	{
		GameController::Run = false;
		return;
	}
	if (!UpdateAssetList("init.ini", m_AssetList))
	{
		GameController::Run = false;
		return;
	}
	if (m_RebuildInitDat)
		RebuildInitDat("init.dat", m_AssetList);
	for (int i = 0; i < (int)m_AssetList.size(); i++)
	{
		if (m_AssetList[i].m_DefaultInitOrder)
			m_InitArea.m_InitOrder.push_back(&m_AssetList[i]);
	}
	BuildSpriteList(m_AssetList);
	LoadSpriteList();
	BindSpriteToAsset(m_AssetList);
	m_ItemMenuArea.BuildMenuItem(m_AssetList);

	SetDefaultKeys();
	gfx->BuildVariableText(m_Size, m_Scale);

	m_OnFire = new SpriteSheet(FIRE_EFFECT_PATH, gfx);
	m_Counter.m_SetGfx(gfx);
	m_Counter.m_SetDrawArea({ m_DrawArea.left, 0.0f, m_DrawAreaRect.right, 128.0f });

	if (m_Rooms.empty())
	{
		Room empty;
		m_Rooms.push_back(empty);
	}
}

void Level1::SetDefaultKeys()
{
	m_Keyboard.m_UpKey.SetKey(VK_UP, "UpKey");
	m_Keyboard.m_DownKey.SetKey(VK_DOWN, "DownKey");
	m_Keyboard.m_LeftKey.SetKey(VK_LEFT, "LeftKey");
	m_Keyboard.m_RightKey.SetKey(VK_RIGHT, "RightKey");
	m_Keyboard.m_ZoomInKey.SetKey(VK_MULTIPLY, "ZoomInKey");
	m_Keyboard.m_ZoomOutKey.SetKey(VK_DIVIDE, "ZoomOutKey");
	m_Keyboard.m_NextTurnKey.SetKey(VK_SPACE, "NextTurnKey");
	m_Keyboard.m_PrevTurnKey.SetKey(VK_BACK, "PrevTurnKey");
	m_Keyboard.m_SelectKey.SetKey(VK_LBUTTON, "SelectKey");
	m_Keyboard.m_EraseKey.SetKey(VK_RBUTTON, "EraseKey");
	m_Keyboard.m_DrawKey.SetKey(VK_LBUTTON, "DrawKey");
	m_Keyboard.m_TagKey.SetKey(VK_SHIFT, "TagKey");
	m_Keyboard.m_BuffKey.SetKey(VK_F1, "BuffKey");
	m_Keyboard.m_DebuffKey.SetKey(VK_F2, "DebuffKey");
	m_Keyboard.m_UnconsciousKey.SetKey(VK_F3, "UnconsciousKey");
	m_Keyboard.m_OnFireKey.SetKey(VK_F6, "OnFireKey");
	m_Keyboard.m_RulerKey.SetKey(VK_SHIFT, "RulerKey");
	m_Keyboard.m_ResetPositionKey.SetKey(VK_HOME, "ResetPositionKey");
	m_Keyboard.m_RotationKey.SetKey(VK_PRIOR, "RotatationKey");
	m_Keyboard.m_IncreaseSize.SetKey(VK_F4, "IncreaseSizeKey");
	m_Keyboard.m_DecreaseSize.SetKey(VK_F5, "DecreaseSizeKey");
	m_Keyboard.m_InsertKey.SetKey(VK_MBUTTON, "InsertKey");
	m_Keyboard.m_SetGeometry.SetKey(VK_RETURN, "SetGeometryKey");
	
	FILE* file = NULL;
	if (fopen_s(&file, "keys.ini", "rb"))
		return;
	fseek(file, 0, SEEK_END);
	size_t bufferLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = NULL;
	buffer = new char[bufferLen];
	fread(buffer, bufferLen, 1, file);
	fclose(file);
	if (buffer)
	{
		SetKeys(buffer);
		delete[] buffer;
	}
}

long Level1::CharToLong(char* vk_key)
{
	if (!_stricmp(vk_key, "VK_LBUTTON"))
	{
		return VK_LBUTTON;
	}
	if (!_stricmp(vk_key, "VK_RBUTTON"))
	{
		return VK_RBUTTON;
	}
	if (!_stricmp(vk_key, "VK_CANCEL"))
	{
		return VK_CANCEL;
	}
	if (!_stricmp(vk_key, "VK_MBUTTON"))
	{
		return VK_MBUTTON;
	}
	if (!_stricmp(vk_key, "VK_XBUTTON1"))
	{
		return VK_XBUTTON1;
	}
	if (!_stricmp(vk_key, "VK_XBUTTON2"))
	{
		return VK_XBUTTON2;
	}
	if (!_stricmp(vk_key, "VK_BACK"))
	{
		return VK_BACK;
	}
	if (!_stricmp(vk_key, "VK_TAB"))
	{
		return VK_TAB;
	}
	if (!_stricmp(vk_key, "VK_CLEAR"))
	{
		return VK_CLEAR;
	}
	if (!_stricmp(vk_key, "VK_RETURN"))
	{
		return VK_RETURN;
	}
	if (!_stricmp(vk_key, "VK_SHIFT"))
	{
		return VK_SHIFT;
	}
	if (!_stricmp(vk_key, "VK_CONTROL"))
	{
		return VK_CONTROL;
	}
	if (!_stricmp(vk_key, "VK_MENU"))
	{
		return VK_MENU;
	}
	if (!_stricmp(vk_key, "VK_PAUSE"))
	{
		return VK_PAUSE;
	}
	if (!_stricmp(vk_key, "VK_CAPITAL"))
	{
		return VK_CAPITAL;
	}
	if (!_stricmp(vk_key, "VK_KANA"))
	{
		return VK_KANA;
	}
	if (!_stricmp(vk_key, "VK_HANGUL"))
	{
		return VK_HANGUL;
	}
	if (!_stricmp(vk_key, "VK_JUNJA"))
	{
		return VK_JUNJA;
	}
	if (!_stricmp(vk_key, "VK_FINAL"))
	{
		return VK_FINAL;
	}
	if (!_stricmp(vk_key, "VK_HANJA"))
	{
		return VK_HANJA;
	}
	if (!_stricmp(vk_key, "VK_KANJI"))
	{
		return VK_KANJI;
	}
	if (!_stricmp(vk_key, "VK_ESCAPE"))
	{
		return VK_ESCAPE;
	}
	if (!_stricmp(vk_key, "VK_CONVERT"))
	{
		return VK_CONVERT;
	}
	if (!_stricmp(vk_key, "VK_NONCONVERT"))
	{
		return VK_NONCONVERT;
	}
	if (!_stricmp(vk_key, "VK_ACCEPT"))
	{
		return VK_ACCEPT;
	}
	if (!_stricmp(vk_key, "VK_MODECHANGE"))
	{
		return VK_MODECHANGE;
	}
	if (!_stricmp(vk_key, "VK_SPACE"))
	{
		return VK_SPACE;
	}
	if (!_stricmp(vk_key, "VK_PRIOR"))
	{
		return VK_PRIOR;
	}
	if (!_stricmp(vk_key, "VK_NEXT"))
	{
		return VK_NEXT;
	}
	if (!_stricmp(vk_key, "VK_END"))
	{
		return VK_END;
	}
	if (!_stricmp(vk_key, "VK_HOME"))
	{
		return VK_HOME;
	}
	if (!_stricmp(vk_key, "VK_LEFT"))
	{
		return VK_LEFT;
	}
	if (!_stricmp(vk_key, "VK_RIGHT"))
	{
		return VK_RIGHT;
	}
	if (!_stricmp(vk_key, "VK_UP"))
	{
		return VK_UP;
	}
	if (!_stricmp(vk_key, "VK_DOWN"))
	{
		return VK_DOWN;
	}
	if (!_stricmp(vk_key, "VK_SELECT"))
	{
		return VK_SELECT;
	}
	if (!_stricmp(vk_key, "VK_PRINT"))
	{
		return VK_PRINT;
	}
	if (!_stricmp(vk_key, "VK_EXECUTE"))
	{
		return VK_EXECUTE;
	}
	if (!_stricmp(vk_key, "VK_SNAPSHOT"))
	{
		return VK_SNAPSHOT;
	}
	if (!_stricmp(vk_key, "VK_INSERT"))
	{
		return VK_INSERT;
	}
	if (!_stricmp(vk_key, "VK_DELETE"))
	{
		return VK_DELETE;
	}
	if (!_stricmp(vk_key, "VK_HELP"))
	{
		return VK_HELP;
	}
	if (!_stricmp(vk_key, "VK_LWIN"))
	{
		return VK_LWIN;
	}
	if (!_stricmp(vk_key, "VK_RWIN"))
	{
		return VK_RWIN;
	}
	if (!_stricmp(vk_key, "VK_APPS"))
	{
		return VK_APPS;
	}
	if (!_stricmp(vk_key, "VK_SLEEP"))
	{
		return VK_SLEEP;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD0"))
	{
		return VK_NUMPAD0;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD1"))
	{
		return VK_NUMPAD1;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD2"))
	{
		return VK_NUMPAD2;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD3"))
	{
		return VK_NUMPAD3;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD4"))
	{
		return VK_NUMPAD4;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD5"))
	{
		return VK_NUMPAD5;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD6"))
	{
		return VK_NUMPAD6;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD7"))
	{
		return VK_NUMPAD7;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD8"))
	{
		return VK_NUMPAD8;
	}
	if (!_stricmp(vk_key, "VK_NUMPAD9"))
	{
		return VK_NUMPAD9;
	}
	if (!_stricmp(vk_key, "VK_MULTIPLY"))
	{
		return VK_MULTIPLY;
	}
	if (!_stricmp(vk_key, "VK_ADD"))
	{
		return VK_ADD;
	}
	if (!_stricmp(vk_key, "VK_SEPARATOR"))
	{
		return VK_SEPARATOR;
	}
	if (!_stricmp(vk_key, "VK_SUBTRACT"))
	{
		return VK_SUBTRACT;
	}
	if (!_stricmp(vk_key, "VK_DECIMAL"))
	{
		return VK_DECIMAL;
	}
	if (!_stricmp(vk_key, "VK_DIVIDE"))
	{
		return VK_DIVIDE;
	}
	if (!_stricmp(vk_key, "VK_F1"))
	{
		return VK_F1;
	}
	if (!_stricmp(vk_key, "VK_F2"))
	{
		return VK_F2;
	}
	if (!_stricmp(vk_key, "VK_F3"))
	{
		return VK_F3;
	}
	if (!_stricmp(vk_key, "VK_F4"))
	{
		return VK_F4;
	}
	if (!_stricmp(vk_key, "VK_F5"))
	{
		return VK_F5;
	}
	if (!_stricmp(vk_key, "VK_F6"))
	{
		return VK_F6;
	}
	if (!_stricmp(vk_key, "VK_F7"))
	{
		return VK_F7;
	}
	if (!_stricmp(vk_key, "VK_F8"))
	{
		return VK_F8;
	}
	if (!_stricmp(vk_key, "VK_F9"))
	{
		return VK_F9;
	}
	if (!_stricmp(vk_key, "VK_F10"))
	{
		return VK_F10;
	}
	if (!_stricmp(vk_key, "VK_F11"))
	{
		return VK_F11;
	}
	if (!_stricmp(vk_key, "VK_F12"))
	{
		return VK_F12;
	}
	if (!_stricmp(vk_key, "VK_F13"))
	{
		return VK_F13;
	}
	if (!_stricmp(vk_key, "VK_F14"))
	{
		return VK_F14;
	}
	if (!_stricmp(vk_key, "VK_F15"))
	{
		return VK_F15;
	}
	if (!_stricmp(vk_key, "VK_F16"))
	{
		return VK_F16;
	}
	if (!_stricmp(vk_key, "VK_F17"))
	{
		return VK_F17;
	}
	if (!_stricmp(vk_key, "VK_F18"))
	{
		return VK_F18;
	}
	if (!_stricmp(vk_key, "VK_F19"))
	{
		return VK_F19;
	}
	if (!_stricmp(vk_key, "VK_F20"))
	{
		return VK_F20;
	}
	if (!_stricmp(vk_key, "VK_F21"))
	{
		return VK_F21;
	}
	if (!_stricmp(vk_key, "VK_F22"))
	{
		return VK_F22;
	}
	if (!_stricmp(vk_key, "VK_F23"))
	{
		return VK_F23;
	}
	if (!_stricmp(vk_key, "VK_F24"))
	{
		return VK_F24;
	}
	if (!_stricmp(vk_key, "VK_NUMLOCK"))
	{
		return VK_NUMLOCK;
	}
	if (!_stricmp(vk_key, "VK_SCROLL"))
	{
		return VK_SCROLL;
	}
	if (!_stricmp(vk_key, "VK_LSHIFT"))
	{
		return VK_LSHIFT;
	}
	if (!_stricmp(vk_key, "VK_RSHIFT"))
	{
		return VK_RSHIFT;
	}
	if (!_stricmp(vk_key, "VK_LCONTROL"))
	{
		return VK_LCONTROL;
	}
	if (!_stricmp(vk_key, "VK_RCONTROL"))
	{
		return VK_RCONTROL;
	}
	if (!_stricmp(vk_key, "VK_LMENU"))
	{
		return VK_LMENU;
	}
	if (!_stricmp(vk_key, "VK_RMENU"))
	{
		return VK_RMENU;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_BACK"))
	{
		return VK_BROWSER_BACK;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_FORWARD"))
	{
		return VK_BROWSER_FORWARD;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_REFRESH"))
	{
		return VK_BROWSER_REFRESH;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_STOP"))
	{
		return VK_BROWSER_STOP;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_SEARCH"))
	{
		return VK_BROWSER_SEARCH;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_FAVORITES"))
	{
		return VK_BROWSER_FAVORITES;
	}
	if (!_stricmp(vk_key, "VK_BROWSER_HOME"))
	{
		return VK_BROWSER_HOME;
	}
	if (!_stricmp(vk_key, "VK_VOLUME_MUTE"))
	{
		return VK_VOLUME_MUTE;
	}
	if (!_stricmp(vk_key, "VK_VOLUME_DOWN"))
	{
		return VK_VOLUME_DOWN;
	}
	if (!_stricmp(vk_key, "VK_VOLUME_UP"))
	{
		return VK_VOLUME_UP;
	}
	if (!_stricmp(vk_key, "VK_MEDIA_NEXT_TRACK"))
	{
		return VK_MEDIA_NEXT_TRACK;
	}
	if (!_stricmp(vk_key, "VK_MEDIA_PREV_TRACK"))
	{
		return VK_MEDIA_PREV_TRACK;
	}
	if (!_stricmp(vk_key, "VK_MEDIA_STOP"))
	{
		return VK_MEDIA_STOP;
	}
	if (!_stricmp(vk_key, "VK_MEDIA_PLAY_PAUSE"))
	{
		return VK_MEDIA_PLAY_PAUSE;
	}
	if (!_stricmp(vk_key, "VK_LAUNCH_MAIL"))
	{
		return VK_LAUNCH_MAIL;
	}
	if (!_stricmp(vk_key, "VK_LAUNCH_MEDIA_SELECT"))
	{
		return VK_LAUNCH_MEDIA_SELECT;
	}
	if (!_stricmp(vk_key, "VK_LAUNCH_APP1"))
	{
		return VK_LAUNCH_APP1;
	}
	if (!_stricmp(vk_key, "VK_LAUNCH_APP2"))
	{
		return VK_LAUNCH_APP2;
	}
	if (!_stricmp(vk_key, "VK_OEM_1"))
	{
		return VK_OEM_1;
	}
	if (!_stricmp(vk_key, "VK_OEM_PLUS"))
	{
		return VK_OEM_PLUS;
	}
	if (!_stricmp(vk_key, "VK_OEM_COMMA"))
	{
		return VK_OEM_COMMA;
	}
	if (!_stricmp(vk_key, "VK_OEM_MINUS"))
	{
		return VK_OEM_MINUS;
	}
	if (!_stricmp(vk_key, "VK_OEM_PERIOD"))
	{
		return VK_OEM_PERIOD;
	}
	if (!_stricmp(vk_key, "VK_OEM_2"))
	{
		return VK_OEM_2;
	}
	if (!_stricmp(vk_key, "VK_OEM_3"))
	{
		return VK_OEM_3;
	}
	if (!_stricmp(vk_key, "VK_OEM_4"))
	{
		return VK_OEM_4;
	}
	if (!_stricmp(vk_key, "VK_OEM_5"))
	{
		return VK_OEM_5;
	}
	if (!_stricmp(vk_key, "VK_OEM_6"))
	{
		return VK_OEM_6;
	}
	if (!_stricmp(vk_key, "VK_OEM_7"))
	{
		return VK_OEM_7;
	}
	if (!_stricmp(vk_key, "VK_OEM_8"))
	{
		return VK_OEM_8;
	}
	if (!_stricmp(vk_key, "VK_OEM_102"))
	{
		return VK_OEM_102;
	}
	if (!_stricmp(vk_key, "VK_PROCESSKEY"))
	{
		return VK_PROCESSKEY;
	}
	if (!_stricmp(vk_key, "VK_PACKET"))
	{
		return VK_PACKET;
	}
	if (!_stricmp(vk_key, "VK_ATTN"))
	{
		return VK_ATTN;
	}
	if (!_stricmp(vk_key, "VK_CRSEL"))
	{
		return VK_CRSEL;
	}
	if (!_stricmp(vk_key, "VK_EXSEL"))
	{
		return VK_EXSEL;
	}
	if (!_stricmp(vk_key, "VK_EREOF"))
	{
		return VK_EREOF;
	}
	if (!_stricmp(vk_key, "VK_PLAY"))
	{
		return VK_PLAY;
	}
	if (!_stricmp(vk_key, "VK_ZOOM"))
	{
		return VK_ZOOM;
	}
	if (!_stricmp(vk_key, "VK_NONAME"))
	{
		return VK_NONAME;
	}
	if (!_stricmp(vk_key, "VK_PA1"))
	{
		return VK_PA1;
	}
	if (!_stricmp(vk_key, "VK_OEM_CLEAR"))
	{
		return VK_OEM_CLEAR;
	}

	return NULL;
}

void Level1::SetKey(char* beforeEqual, char* afterEqual)
{
	KeyClass* selectedKey = NULL;
	if (!_stricmp(beforeEqual, "UpKey"))
	{
		selectedKey = &m_Keyboard.m_UpKey;
	}
	else if (!_stricmp(beforeEqual, "DownKey"))
	{
		selectedKey = &m_Keyboard.m_DownKey;
	}
	else if (!_stricmp(beforeEqual, "LeftKey"))
	{
		selectedKey = &m_Keyboard.m_LeftKey;
	}
	else if (!_stricmp(beforeEqual, "RightKey"))
	{
		selectedKey = &m_Keyboard.m_RightKey;
	}
	else if (!_stricmp(beforeEqual, "ZoomInKey"))
	{
		selectedKey = &m_Keyboard.m_ZoomInKey;
	}
	else if (!_stricmp(beforeEqual, "ZoomOutKey"))
	{
		selectedKey = &m_Keyboard.m_ZoomOutKey;
	}
	else if (!_stricmp(beforeEqual, "NextTurnKey"))
	{
		selectedKey = &m_Keyboard.m_NextTurnKey;
	}
	else if (!_stricmp(beforeEqual, "PrevTurnKey"))
	{
		selectedKey = &m_Keyboard.m_PrevTurnKey;
	}
	else if (!_stricmp(beforeEqual, "SelectKey"))
	{
		selectedKey = &m_Keyboard.m_SelectKey;
	}
	else if (!_stricmp(beforeEqual, "EraseKey"))
	{
		selectedKey = &m_Keyboard.m_EraseKey;
	}
	else if (!_stricmp(beforeEqual, "DrawKey"))
	{
		selectedKey = &m_Keyboard.m_DrawKey;
	}
	else if (!_stricmp(beforeEqual, "RulerKey"))
	{
		selectedKey = &m_Keyboard.m_RulerKey;
	}
	else if (!_stricmp(beforeEqual, "TagKey"))
	{
		selectedKey = &m_Keyboard.m_TagKey;
	}
	else if (!_stricmp(beforeEqual, "BuffKey"))
	{
		selectedKey = &m_Keyboard.m_BuffKey;
	}
	else if (!_stricmp(beforeEqual, "DebuffKey"))
	{
		selectedKey = &m_Keyboard.m_DebuffKey;
	}
	else if (!_stricmp(beforeEqual, "UnconsciousKey"))
	{
		selectedKey = &m_Keyboard.m_UnconsciousKey;
	}
	else if (!_stricmp(beforeEqual, "ResetPositionKey"))
	{
		selectedKey = &m_Keyboard.m_ResetPositionKey;
	}
	else if (!_stricmp(beforeEqual, "RotationKey"))
	{
		selectedKey = &m_Keyboard.m_RotationKey;
	}
	else if (!_stricmp(beforeEqual, "DecreaseSize"))
	{
		selectedKey = &m_Keyboard.m_DecreaseSize;
	}
	else if (!_stricmp(beforeEqual, "IncreaseSize"))
	{
		selectedKey = &m_Keyboard.m_IncreaseSize;
	}
	else if (!_stricmp(beforeEqual, "OnFireKey"))
	{
		selectedKey = &m_Keyboard.m_OnFireKey;
	}
	else if (!_stricmp(beforeEqual, "InsertKey"))
	{
		selectedKey = &m_Keyboard.m_InsertKey;
	}
	else if (!_stricmp(beforeEqual, "SetGeometryKey"))
	{
		selectedKey = &m_Keyboard.m_SetGeometry;
	}

	if (selectedKey)
	{
		if (strlen(afterEqual) == 1)
		{
			selectedKey->SetKey(toupper(afterEqual[0]), beforeEqual);
		}
		else
		{
			long key = CharToLong(afterEqual);
			if(key)
				selectedKey->SetKey(key, beforeEqual);
		}
	}
	selectedKey = NULL;
}

void Level1::SetKeys(char* buffer)
{
	if (!buffer) return;
	size_t position = 0;
	char* line = NULL;
	while (position < strlen(buffer))
	{
		position += GetLine(buffer + position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		SetKey(line, t);

		delete[] t;
		delete[] line;
		line = NULL;
	}
}

void Level1::EmptyAssetList(std::vector<Asset>& asset)
{
	std::vector<Asset> empty;
	for (int i = 0; i < (int)asset.size(); i++)
	{
		asset[i].m_Sprite = NULL;
		asset[i].m_Icon = NULL;
	}
	std::swap(asset, empty);
}

void Level1::Unload()
{
	//cleanup memory if necessary
	m_ItemMenuArea.Cleanup();
	EmptySpriteVector(m_Sprites);
	EmptyLineQueue(m_Lines);
	EmptyAssetList(m_AssetList);
	EmptySpriteList(m_SpriteList);
	if (m_LayerMenu) DeleteMenu(GetMenu(GameController::hWnd), (UINT)m_LayerMenu, MF_BYCOMMAND);
	if (m_ShowLayerMenu) DeleteMenu(GetMenu(GameController::hWnd), (UINT)m_ShowLayerMenu, MF_BYCOMMAND);
	if (m_OnFire) delete m_OnFire;
	while (!m_PathGeometry.empty())
	{
		m_PathGeometry.back()->Release();
		m_PathGeometry.pop_back();
	}
	while (!m_SinkGeometry.empty())
	{
		m_SinkGeometry.back()->Release();
		m_SinkGeometry.pop_back();
	}
	while (!m_CG.empty())
	{
		if (m_CG.back().m_Sink) m_CG.back().m_Sink->Release();
		if (m_CG.back().m_Path) m_CG.back().m_Path->Release();
		m_CG.pop_back();
	}
	while (!m_Rooms.empty())
	{
		for (int i = 0; i < m_Rooms.back().m_Layer.size(); i++)
		{
			for (int w = 0; w < m_Rooms.back().m_Layer[i].m_CG.size(); w++)
			{
				CustomGeometry* tcg = &m_Rooms.back().m_Layer[i].m_CG[w];
				if (tcg->m_Path) tcg->m_Path->Release();
				if (tcg->m_Sink) tcg->m_Sink->Release();
			}
		}
		m_Rooms.pop_back();
	}
}

void Level1::CheckRoom()
{
	CheckMenuItem(m_RoomMenu, GetMenuItemID(m_RoomMenu, m_CurrentRoom), MF_CHECKED);
}

void Level1::UncheckRoom()
{
	CheckMenuItem(m_RoomMenu, GetMenuItemID(m_RoomMenu, m_CurrentRoom), MF_UNCHECKED);
}

void Level1::UncheckLayer()
{
	for (int i = 0; i < GetMenuItemCount(m_LayerMenu); i++)
	{
		CheckMenuItem(m_LayerMenu, MENU_LAYER0 + i, MF_UNCHECKED);
	}
}

void Level1::CheckLayer()
{
	CheckMenuItem(m_LayerMenu, GetMenuItemID(m_LayerMenu, m_CurrentLayer), MF_CHECKED);
}

void Level1::SetHideLayer(unsigned long menuid)
{
	CheckMenuItem(m_ShowLayerMenu, GetMenuItemID(m_ShowLayerMenu, menuid), MF_UNCHECKED);
}

void Level1::SetShowLayer(unsigned long menuid)
{
	CheckMenuItem(m_ShowLayerMenu, GetMenuItemID(m_ShowLayerMenu, menuid), MF_CHECKED);
}

bool Level1::MenuItemChecked(HMENU menu, unsigned long MENU_ITEM)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;

	if (menu)
	{
		if (GetMenuItemInfo(menu, (UINT)MENU_ITEM, false, &mii) != 0)
		{
			if ((mii.fState & MFS_CHECKED) != 0)
				return true;
		}
	}
	return false;
}

void Level1::ToggleShowHideLayer(unsigned long menuid)
{
	if (MenuItemChecked(m_ShowLayerMenu, menuid))
	{
		SetHideLayer(menuid);
		CheckMenuItem(m_ShowLayerMenu, menuid, MF_UNCHECKED);
		m_Rooms[m_CurrentRoom].m_Layer[menuid - MENU_SHOWLAYER0].m_Visible = false;
	}
	else
	{
		SetShowLayer(menuid);
		CheckMenuItem(m_ShowLayerMenu, menuid, MF_CHECKED);
		m_Rooms[m_CurrentRoom].m_Layer[menuid - MENU_SHOWLAYER0].m_Visible = true;
	}
}

void Level1::ToggleShowHideRoom(unsigned long menuid)
{
	if (MenuItemChecked(m_ShowRoomMenu, menuid))
	{
		CheckMenuItem(m_ShowRoomMenu, menuid, MF_UNCHECKED);
		m_Rooms[menuid - MENU_SHOWROOM0].m_Visible = false;
	}
	else
	{
		CheckMenuItem(m_ShowRoomMenu, menuid, MF_CHECKED);
		m_Rooms[menuid - MENU_SHOWROOM0].m_Visible = true;
	}
}

void Level1::Update(WPARAM wParam, D2D1_POINT_2F p)
{
	if (wParam >= (unsigned)MENU_ROOM0 && wParam <= (unsigned)(MENU_ROOM0 + m_Rooms.size() - 1))
	{
		UncheckRoom();
		EmptyLayerMenu();
		m_CurrentLayer = 0;
		m_CurrentRoom = (long)(wParam - MENU_ROOM0);
		RebuildLayerMenu();
		CheckRoom();
	}
	if (wParam >= (unsigned)MENU_SHOWROOM0 && wParam <= (unsigned)(MENU_SHOWROOM0 + m_Rooms.size() - 1))
	{
		ToggleShowHideRoom((long)wParam);
	}
	if (wParam >= (unsigned)MENU_LAYER0 && wParam <= (unsigned)(MENU_LAYER0 + m_Rooms[m_CurrentRoom].m_Layer.size() - 1))
	{
		UncheckLayer();
		m_CurrentLayer = (long)(wParam - MENU_LAYER0);
		CheckLayer();
	}

	if (wParam >= (unsigned)MENU_SHOWLAYER0 && wParam <= (unsigned)(MENU_SHOWLAYER0 + m_Rooms[m_CurrentRoom].m_Layer.size() - 1))
	{
		ToggleShowHideLayer((long)wParam);
	}

	if (wParam == MENU_ADDROOM)
	{
		AddRoomMenu();
	}

	if (wParam == MENU_OPEN)
	{
		Open(GetLoadFilePath());
		gfx->BuildVariableText(m_Size, m_Scale);
		m_RecalcLines = true;
	}

	if (wParam == MENU_SAVE)
	{
		std::string tPath = GetSaveFilePath();
		if (!tPath.length()) return;
		Save(tPath);
	}

	if (wParam == MENU_CHANGEMODE)
	{
		ChangeMode();
	}

	if (wParam == MENU_TOGGLEINIT)
	{
		m_ShowItemMenu ^= true;
	}

	if (wParam == MENU_TOGGLECOLO)
	{
		m_ShowBgColors ^= true;
	}

	if (wParam == MENU_KEEPASPECT)
	{
		m_NextKeepAspect ^= true;
	}

	if (wParam == MENU_GEOMETRY)
	{
		m_Geometry ^= true;
		CheckMenuItem(GetMenu(GameController::hWnd), MENU_GEOMETRY, ((m_Geometry << 3) | MF_UNCHECKED));
	}

	if (wParam == MENU_LOCKTOGRID)
	{
		m_ItemMenuArea.m_LockToGrid ^= true;
		UINT checked = m_ItemMenuArea.m_LockToGrid << 3;
		checked = checked | MF_UNCHECKED;
		CheckMenuItem(GetMenu(GameController::hWnd), MENU_LOCKTOGRID, checked);
	}

	if (wParam == MENU_GRIDONTOP)
	{
		m_ItemMenuArea.m_GridOnTop ^= true;
		UINT checked = m_ItemMenuArea.m_GridOnTop << 3;
		checked = checked | MF_UNCHECKED;
		CheckMenuItem(GetMenu(GameController::hWnd), MENU_GRIDONTOP, checked);
	}

	if (wParam == MENU_EDITBUTTONMAPPING)
	{
		//allows for tweaking the button configuration
		ButtonMapper m_ButtonMapper(m_Keyboard, gfx);
		Sleep(100);
	}
}

float Level1::CalcRulerDistance()
{
	if ((!m_RulerStartPoint.x) && (!m_RulerStartPoint.y) && (!m_RulerEndPoint.x) && (!m_RulerEndPoint.y))
		return 0.0f;

	float scaledSize = m_Size * m_Scale;
	float a = m_RulerEndPoint.x - m_RulerStartPoint.x;
	float b = m_RulerEndPoint.y - m_RulerStartPoint.y;
	float dist = sqrt(a * a + b * b);
	dist /= scaledSize;
	dist *= 5.0f;

	return dist;
}

void Level1::ReactInputsGeometryMode(float timeTotal, float timeDelta, D2D1_POINT_2F p)
{
	if (m_Keyboard.m_ZoomInKey.IsPressed(timeTotal, false)) IncreaseSize();
	if (m_Keyboard.m_ZoomOutKey.IsPressed(timeTotal, false)) DecreaseSize();
	if (m_Keyboard.m_LeftKey.IsPressed(timeTotal, 0.10)) m_StartPoint.x += m_Size;
	if (m_Keyboard.m_RightKey.IsPressed(timeTotal, 0.10)) m_StartPoint.x -= m_Size;
	if (m_Keyboard.m_UpKey.IsPressed(timeTotal, 0.10)) m_StartPoint.y += m_Size;
	if (m_Keyboard.m_DownKey.IsPressed(timeTotal, 0.10)) m_StartPoint.y -= m_Size;
	if (m_Keyboard.m_DecreaseSize.IsPressed(timeTotal, false))
	{
		m_Size--;
		m_RecalcLines = true;
	}
	if (m_Keyboard.m_IncreaseSize.IsPressed(timeTotal, false))
	{
		m_Size++;
		m_RecalcLines = true;
	}
	if (m_Keyboard.m_ResetPositionKey.IsPressed(timeTotal))
	{
		m_RecalcLines = true;
		m_StartPoint = {};
	}
	if (m_Keyboard.m_EraseKey.IsPressed(timeTotal, false))
	{
		//remove last point
		/*
		if (!cg.m_Points.empty())
		{
			cg.m_Points.pop_back();
		}
		*/
		if (!m_CurrentGeometry.empty())
			m_CurrentGeometry.pop_back();
		else
		{
			if (m_UseOld)
			{
				for (int i = 0; i < m_CG.size(); i++)
				{	//this isn't really working properly dont think its using the updated point values, just raw which obviously will not work if not in the default starting view
					D2D1_RECT_F tRect = {};
					tRect.left = m_CG[i].m_Points.front().x;
					tRect.left *= m_Scale;
					tRect.left = (int)((int)(tRect.left) / (int)(m_Size * m_Scale));
					tRect.left *= m_Size * m_Scale;
					tRect.top = m_CG[i].m_Points.front().y;
					tRect.top *= m_Scale;
					tRect.top = (int)((int)(tRect.top) / (int)(m_Size * m_Scale));
					tRect.top *= m_Size * m_Scale;
					tRect.left += (m_StartPoint.x - m_CG[i].m_Offset.x) * m_Scale;
					tRect.top += (m_StartPoint.y - m_CG[i].m_Offset.y) * m_Scale;
					tRect.right = tRect.left + m_Size * m_Scale;
					tRect.bottom = tRect.top + m_Size * m_Scale;

					D2D1_POINT_2F ttp = p;
					ttp.x += m_StartPoint.x;
					ttp.y += m_StartPoint.y;

					if (InRect(tRect, p))
						if (m_CurrentLayer == m_CG[i].m_Layer)
						{
							if (m_CG[i].m_Sink) m_CG[i].m_Sink->Release();
							if (m_CG[i].m_Path) m_CG[i].m_Path->Release();
							m_CG.erase(m_CG.begin() + i);
						}
				}
			}
			else
			{
				for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG.size(); i++)
				{
					CustomGeometry* tCG = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG[i];
					D2D1_RECT_F tRect = {};
					tRect.left = tCG->m_Points.front().x;
					tRect.left *= m_Scale;
					tRect.left = (int)((int)(tRect.left) / (int)(m_Size * m_Scale));
					tRect.left *= m_Size * m_Scale;
					tRect.top = tCG->m_Points.front().y;
					tRect.top *= m_Scale;
					tRect.top = (int)((int)(tRect.top) / (int)(m_Size * m_Scale));
					tRect.top *= m_Size * m_Scale;
					tRect.left += (m_StartPoint.x - tCG->m_Offset.x) * m_Scale;
					tRect.top += (m_StartPoint.y - tCG->m_Offset.y) * m_Scale;
					tRect.right = tRect.left + m_Size * m_Scale;
					tRect.bottom = tRect.top + m_Size * m_Scale;

					D2D1_POINT_2F ttp = p;
					ttp.x += m_StartPoint.x;
					ttp.y += m_StartPoint.y;

					if (InRect(tRect, p))
					{
						if (tCG->m_Sink) tCG->m_Sink->Release();
						if (tCG->m_Path) tCG->m_Path->Release();
						m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG.erase(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG.begin() + i);
					}
				}
			}
		}
	}
	if (m_Keyboard.m_DrawKey.IsPressed(timeTotal, false))
	{		
		if (InRect(m_DrawAreaRect, p))
		{
			//add points
			if (m_CurrentGeometry.empty())
			{
				D2D1_POINT_2F pptt = { p.x / m_Scale, p.y / m_Scale };
				m_CurrentGeometry.push_back(pptt);
				cg.m_Offset = m_StartPoint;
				cg.gfx = gfx;
			}
			else
			{
				D2D1_POINT_2F placeHolder = p;
				if (m_ItemMenuArea.m_LockToGrid)
				{
					D2D1_RECT_F tRect = {};					

					tRect.left = m_CurrentGeometry.front().x;
					tRect.left *= m_Scale;
					tRect.left = (int)((int)(tRect.left) / (int)(m_Size * m_Scale));
					tRect.left *= m_Size * m_Scale;
					tRect.top = m_CurrentGeometry.front().y;
					tRect.top *= m_Scale;
					tRect.top = (int)((int)(tRect.top) / (int)(m_Size * m_Scale));
					tRect.top *= m_Size * m_Scale;
					tRect.right = tRect.left + m_Size * m_Scale;
					tRect.bottom = tRect.top + m_Size * m_Scale;
					if (p.x < tRect.left)
						placeHolder.x = tRect.left;
					else if (p.x > tRect.right)
						placeHolder.x = tRect.right;
					else
						placeHolder.x = p.x;
					if (p.y < tRect.top)
						placeHolder.y = tRect.top;
					else if (p.y > tRect.bottom)
						placeHolder.y = tRect.bottom;
					else
						placeHolder.y = p.y;
				}
				placeHolder.x /= m_Scale;
				placeHolder.y /= m_Scale;

				m_CurrentGeometry.push_back(placeHolder);				
			}
		}
	}
	if (m_Keyboard.m_SetGeometry.IsPressed(timeTotal, false))
	{
		
		if (!m_CurrentGeometry.empty())
		{
			std::swap(m_CurrentGeometry, cg.m_Points);
			std::vector<D2D1_POINT_2F> empty;
			std::swap(m_CurrentGeometry, empty);
		}
		if (!cg.m_Points.empty())
		{
			if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
			{
				for (int i = 0; i < cg.m_Points.size(); i++)
				{
					WallsData wall;
					wall.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
					wall.m_Layer = m_CurrentLayer;
					wall.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
					wall.m_Thickness = m_ItemMenuArea.m_SelectedThickness;
					if ((i + 1) < cg.m_Points.size())
					{
						wall.m_StartPoint = cg.m_Points[i];
						wall.m_StartPoint.x -= m_StartPoint.x;
						wall.m_StartPoint.y -= m_StartPoint.y;
						wall.m_EndPoint = cg.m_Points[i + 1];
						wall.m_EndPoint.x -= m_StartPoint.x;
						wall.m_EndPoint.y -= m_StartPoint.y;
					}
					//m_Walls.push_back(wall);
					m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls.push_back(wall);
				}
				cg = {};
			}
			else
			{
				cg.m_BitmapID = m_ItemMenuArea.m_SelectedItem->m_MenuID;
				cg.m_Layer = m_CurrentLayer;
				cg.m_Size = m_ItemMenuArea.m_SelectedSize;
				m_CG.push_back(cg);
				m_CG.back().BuildGeometry(m_Scale, m_StartPoint);
				m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG.push_back(cg);
				m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_CG.back().BuildGeometry(m_Scale, m_StartPoint);
				cg.m_Sink = NULL;
				cg.m_Path = NULL;
				cg = {};
			}
		}
	
	}
	if (m_Keyboard.m_SelectKey.IsPressed(timeTotal, 0.2))
	{
		if (InRect(m_MenuAreaRect, p))
		{
			if (m_ShowItemMenu)
			{
				if (p.y < m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_AdditionalOptions;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle PC Colors"))
							{
								m_ShowBgColors ^= true;
								m_ItemMenuArea.m_ShowBgColor ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Initiative"))
							{
								m_ShowItemMenu ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Keep Aspect"))
							{
								m_NextKeepAspect ^= true;
								m_ItemMenuArea.m_KeepRatio = m_NextKeepAspect;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Lock to Grid"))
							{
								m_ItemMenuArea.m_LockToGrid ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_LOCKTOGRID, (m_ItemMenuArea.m_LockToGrid << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Grid on Top"))
							{
								m_ItemMenuArea.m_GridOnTop ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_GRIDONTOP, (m_ItemMenuArea.m_GridOnTop << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Turn Counter"))
							{
								if (!m_ItemMenuArea.m_ShowCounter)
								{
									m_ItemMenuArea.m_ShowCounter = true;
									m_Counter.m_SetMenuID(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (m_Counter.m_GetCount())
								{
									m_Counter.m_ResetCount(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (!m_ItemMenuArea.m_CounterCount)
								{
									m_ItemMenuArea.m_ShowCounter = false;
									m_Counter.m_ResetCount();
								}
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Add Custom Color"))
							{
								if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
								{
									CHOOSECOLOR ccolor = {};
									static COLORREF cColorArray[16];
									static DWORD rgbCurrent;

									ZeroMemory(&ccolor, sizeof(CHOOSECOLOR));
									ccolor.lStructSize = sizeof(CHOOSECOLOR);
									ccolor.hwndOwner = GameController::hWnd;
									ccolor.lpCustColors = (LPDWORD)cColorArray;
									ccolor.rgbResult = rgbCurrent;
									ccolor.Flags = CC_FULLOPEN | CC_RGBINIT;

									if (ChooseColor(&ccolor))
									{
										m_NumberCustomColors++;

										Asset tCus;
										tCus.m_MenuID = COLOR_CUSTOMS + m_NumberCustomColors;
										tCus.m_Layer = m_CurrentLayer;
										tCus.m_Type = m_ItemMenuArea.m_SelectedType->m_Type;
										tCus.m_Submenu = m_ItemMenuArea.m_SelectedSubMenu->m_Submenu;
										tCus.m_Name = std::to_string(tCus.m_MenuID);
										tCus.m_KeepAspect = false;
										tCus.m_KeepIconAspect = false;
										tCus.m_DefaultInitOrder = false;
										tCus.m_BgColor = { (float)(GetRValue(ccolor.rgbResult) / 255.0f), (float)(GetGValue(ccolor.rgbResult) / 255.0f), (float)(GetBValue(ccolor.rgbResult) / 255.0f), 1.0f };
										m_AssetList.push_back(tCus);
										m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
									}
								}
							}
						}
						temp.pop();
					}
				}
				else if (p.y < m_ItemMenuArea.m_ItemTypes.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_ItemTypes;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Type, temp.front().m_ItemText))
								{
									m_ItemMenuArea.m_SelectedType = &m_AssetList[i];
									break;
								}
							}
							break;
						}
						temp.pop();
					}
					m_ItemMenuArea.RebuildSubMenuList(m_AssetList);
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if (p.y < m_ItemMenuArea.m_SubMenus.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_SubMenus;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Submenu, temp.front().m_ItemText))
								{
									m_ItemMenuArea.m_SelectedSubMenu = &m_AssetList[i];
									break;
								}
							}
							break;
						}
						temp.pop();
					}
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if (p.y < m_ItemMenuArea.m_SelectedSizeCategory->back().m_DrawArea.bottom)
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
					{
						if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
						{
							std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Thickness;
							while (!temp.empty())
							{
								if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
								{
									m_ItemMenuArea.SetSelectedThickness(m_ItemMenuArea.m_Thickness, _wtof(temp.front().m_ItemText.c_str()));
									break;
								}
								temp.pop();
							}
						}
						else
						{
							std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Radius;
							while (!temp.empty())
							{
								if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
								{
									m_ItemMenuArea.SetSelectedRadius(m_ItemMenuArea.m_Radius, (int)(_wtof(temp.front().m_ItemText.c_str())));
									break;
								}
								temp.pop();
							}
						}
					}
					else if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Free Draw"))
					{
						std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_PenSize;
						while (!temp.empty())
						{
							if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
							{
								m_ItemMenuArea.SetSelectedPenSize(m_ItemMenuArea.m_PenSize, _wtof(temp.front().m_ItemText.c_str()));
								break;
							}
							temp.pop();
						}
					}
					else
					{
						std::queue<ItemMenuItem> temp = *m_ItemMenuArea.m_SelectedSizeCategory;
						while (!temp.empty())
						{
							if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
							{
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Fine"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Fine);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Diminutive"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Diminutive);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Tiny"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Tiny);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Small"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Small);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Medium"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Medium);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Large"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Large);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Huge"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Huge);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Gargantuan"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Gargantuan);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Colossal"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Colossal);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Original Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Original);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"2x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_TwoTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"3x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_ThreeTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"4x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_FourTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Half Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Half);
								}
								break;
							}
							temp.pop();
						}
						m_NextSize = m_ItemMenuArea.m_SelectedSize;
					}
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if ((!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line")) && (p.y < m_ItemMenuArea.m_Width.back().m_DrawArea.bottom))
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Width;
					while (!temp.empty())
					{
						if (InRect(temp.front().m_DrawArea, p))
						{
							m_ItemMenuArea.SetSelectedWidth(m_ItemMenuArea.m_Width, (int)(_wtof(temp.front().m_ItemText.c_str())));
							break;
						}
						temp.pop();
					}
				}
				else if (p.y < m_ItemMenuArea.m_Sprites.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Sprites;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Submenu, temp.front().m_SubMenu))
								{/*** moving to different section
								 if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Name, temp.front().m_ItemText))
								 {
								 if (!_wcsnicmp(temp.front().m_ItemText.c_str(), L"Add Custom", strlen("Add Custom")))
								 {
								 Asset tCus;
								 tCus.m_MenuID = COLOR_CUSTOMS;			//plus some sort of counter
								 tCus.m_Layer = m_CurrentLayer;
								 tCus.m_Type = m_ItemMenuArea.m_SelectedType->m_Type;
								 tCus.m_Submenu = m_ItemMenuArea.m_SelectedSubMenu->m_Submenu;
								 tCus.m_Name = std::to_string(1.0f);		//set this equal to the chosen rgb values
								 tCus.m_KeepAspect = false;
								 tCus.m_KeepIconAspect = false;
								 tCus.m_DefaultInitOrder = false;
								 tCus.m_BgColor = { 1.0f, 1.0f, 0.0f, 1.0f };	//set to chosen color value
								 m_AssetList.push_back(tCus);
								 }
								 else
								 m_ItemMenuArea.m_SelectedItem = &m_AssetList[i];
								 break;
								 }*/
									if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Name, temp.front().m_ItemText))
									{
										m_ItemMenuArea.m_SelectedItem = &m_AssetList[i];
										break;
									}
								}
							}
							break;
						}
						temp.pop();
					}
				}
			}
		}
	}
}

void Level1::ReactInputs(float timeTotal, float timeDelta, D2D1_POINT_2F p)
{
	if (m_Keyboard.m_DecreaseSize.IsPressed(timeTotal, false))
	{
		m_Size--;
		m_RecalcLines = true;
	}
	if (m_Keyboard.m_IncreaseSize.IsPressed(timeTotal, false))
	{
		m_Size++;
		m_RecalcLines = true;
	}

	if (m_Keyboard.m_NextTurnKey.IsPressed(timeTotal, 0.2f))
	{
		m_InitArea.NextTurn();
		if (m_ItemMenuArea.m_ShowCounter)
		{
			if (m_InitArea.m_InitOrder.front()->m_MenuID == m_Counter.m_GetMenuID())
			{ 
				m_Counter.m_Increase();
			}
		}
	}
	if (m_Keyboard.m_PrevTurnKey.IsPressed(timeTotal, 0.2f))
	{
		if (m_ItemMenuArea.m_ShowCounter)
		{
			if (m_InitArea.m_InitOrder.front()->m_MenuID == m_Counter.m_GetMenuID())
			{
				m_Counter.m_Decrease();
			}
		}
		m_InitArea.PreviousTurn();
	}
	if (m_Keyboard.m_ResetPositionKey.IsPressed(timeTotal))
	{
		m_RecalcLines = true;
		m_StartPoint = {};
	}

	if (m_Keyboard.m_RulerKey.IsPressed(timeTotal, false))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			if ((!m_RulerStartPoint.x) && (!m_RulerStartPoint.y))
			{
				if (m_ItemMenuArea.m_LockToGrid)
				{
					float scaledSize = m_Size * m_Scale;
					m_RulerStartPoint.x = (float)((int)(p.x / scaledSize));
					m_RulerStartPoint.x *= scaledSize;
					m_RulerStartPoint.x += scaledSize / 2.0f;
					m_RulerStartPoint.y = (float)((int)(p.y / scaledSize));
					m_RulerStartPoint.y *= scaledSize;
					m_RulerStartPoint.y += scaledSize / 2.0f;
				}
				else
				{
					m_RulerStartPoint = p;
				}
			}
			else
			{
				m_RulerEndPoint = p;
			}
		}
	}
	else if (!m_SelectedItem)
	{
		m_RulerStartPoint = m_RulerEndPoint = { };
	}
	else
		m_RulerEndPoint = p;

	if (m_Keyboard.m_EraseKey.IsPressed(timeTotal, 0.2f))
	{
		if (InRect(m_MenuAreaRect, p))
		{
			if (!m_ShowItemMenu)
			{
				m_InitArea.RemoveEntry(p);
			}
			else if (p.y < m_ItemMenuArea.m_Sprites.back().m_DrawArea.bottom)
			{
				std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Sprites;
				while (!temp.empty())
				{
					if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
					{
						for (int i = 0; i < (int)m_AssetList.size(); i++)
						{
							if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Submenu, temp.front().m_SubMenu))
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Name, temp.front().m_ItemText))
								{
									m_InitArea.AddTurn(m_AssetList[i].m_MenuID, m_AssetList);
								}
							}
						}
						break;
					}
					temp.pop();
				}
			}
		}
	}

	if (m_Keyboard.m_TagKey.IsPressed(timeTotal, false))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			long key = m_Keyboard.m_Range.KeyPressedInRange('0', '9');
			if (key == -1)
				key = m_Keyboard.m_Range.KeyPressedInRange('A', 'Z');
			if (key != -1)
			{
				if (m_Keyboard.m_Range.TimePassed(timeTotal) > 0.33f)
				{
					m_Keyboard.m_Range.UpdateTime(timeTotal);
					for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
					{
						MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
						if (InRect(tmap->m_ColorArea, p))
						{
							if (tmap->m_Tag == (wchar_t)key)
								tmap->m_Tag = 0;
							else
								tmap->m_Tag = (wchar_t)key;
						}
					}
					for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
					{
						ShapeData* tshape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
						if (InRect(tshape->m_ColorArea, p))
						{
							if (tshape->m_Tag == (wchar_t)key)
								tshape->m_Tag = 0;
							else
								tshape->m_Tag = (wchar_t)key;
						}
					}
				}
			}
		}
	}

	if (m_Keyboard.m_BuffKey.IsPressed(timeTotal, 0.2f))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
			{
				MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
				if (InRect(tmap->m_ColorArea, p))
				{
					if (tmap->m_Status == (wchar_t)L'↑')
						tmap->m_Status = 0;
					else
						tmap->m_Status = (wchar_t)L'↑';
				}
			}
		}
		else if (InRect(m_MenuAreaRect, p) && !m_ShowItemMenu)
		{
			int menu_id = (int)((p.y - 56.0f) / 128.0f);
			std::deque<Asset*> init = m_InitArea.m_InitOrder;
			if (menu_id < (int)init.size())
			{
				for (int i = 0; i < menu_id; i++)
					init.pop_front();
				menu_id = init.front()->m_MenuID;
				if (menu_id < (int)m_AssetList.size())
				{
					if (m_AssetList[menu_id].m_Status == (wchar_t)L'↑')
						m_AssetList[menu_id].m_Status = 0;
					else
						m_AssetList[menu_id].m_Status = (wchar_t)L'↑';
				}
			}
		}
	}

	if (m_Keyboard.m_DebuffKey.IsPressed(timeTotal, 0.2f))
	{
		
		if (InRect(m_DrawAreaRect, p))
		{
			for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
			{
				MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
				if (InRect(tmap->m_ColorArea, p))
				{
					if (tmap->m_Status == (wchar_t)L'↓')
						tmap->m_Status = 0;
					else
						tmap->m_Status = (wchar_t)L'↓';
				}
			}
		}
		else if (InRect(m_MenuAreaRect, p) && !m_ShowItemMenu)
		{
			int menu_id = (int)((p.y - 56.0f) / 128.0f);
			std::deque<Asset*> init = m_InitArea.m_InitOrder;
			if (menu_id < (int)init.size())
			{
				for (int i = 0; i < menu_id; i++)
					init.pop_front();
				menu_id = init.front()->m_MenuID;
				if (menu_id < (int)m_AssetList.size())
				{
					if (m_AssetList[menu_id].m_Status == (wchar_t)L'↓')
						m_AssetList[menu_id].m_Status = 0;
					else
						m_AssetList[menu_id].m_Status = (wchar_t)L'↓';
				}
			}
		}
	}

	if (m_Keyboard.m_UnconsciousKey.IsPressed(timeTotal, 0.2f))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
			{
				MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
				if (InRect(tmap->m_ColorArea, p))
				{
					if (tmap->m_Status == (wchar_t)L'╥')
						tmap->m_Status = 0;
					else
						tmap->m_Status = (wchar_t)L'╥';
				}
			}
		}
		else if (InRect(m_MenuAreaRect, p) && !m_ShowItemMenu)
		{
			int menu_id = (int)((p.y - 56.0f) / 128.0f);
			std::deque<Asset*> init = m_InitArea.m_InitOrder;
			if (menu_id < (int)init.size())
			{
				for (int i = 0; i < menu_id; i++)
					init.pop_front();
				menu_id = init.front()->m_MenuID;
				if (menu_id < (int)m_AssetList.size())
				{
					if (m_AssetList[menu_id].m_Status == (wchar_t)L'╥')
						m_AssetList[menu_id].m_Status = 0;
					else
						m_AssetList[menu_id].m_Status = (wchar_t)L'╥';
				}
			}
		}
	}

	if (m_Keyboard.m_OnFireKey.IsPressed(timeTotal, 0.2f))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			for (int i = 0; i < (int)m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
			{
				MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
				if (InRect(tmap->m_ColorArea, p))
				{
					if (tmap->m_Status == (wchar_t)L'Ω')
						tmap->m_Status = 0;
					else
						tmap->m_Status = (wchar_t)L'Ω';
				}
			}
		}
		else if (InRect(m_MenuAreaRect, p) && !m_ShowItemMenu)
		{
			int menu_id = (int)((p.y - 56.0f) / 128.0f);
			std::deque<Asset*> init = m_InitArea.m_InitOrder;
			if (menu_id < (int)init.size())
			{
				for (int i = 0; i < menu_id; i++)
					init.pop_front();
				menu_id = init.front()->m_MenuID;
				if (menu_id < (int)m_AssetList.size())
				{
					if (m_AssetList[menu_id].m_Status == (wchar_t)L'Ω')
						m_AssetList[menu_id].m_Status = 0;
					else
						m_AssetList[menu_id].m_Status = (wchar_t)L'Ω';
				}
			}
		}
	}

	if (m_Keyboard.m_EraseKey.IsPressed(timeTotal, false))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			if (m_GridType == Grid::m_Square || m_GridType == Grid::m_Hexagon)
			{
				D2D1_POINT_2F tempP = {};
				m_TempWallStartPoint = {};

				tempP.x = p.x - m_StartPoint.x;
				tempP.y = p.y - m_StartPoint.y;

				if (m_UseOld)
				{
					for (int i = 0; i < (int)m_MapData.size(); i++)
					{
						/*
						if (((int)(tempP.x * 1000) >= (int)(m_MapData[i].m_Position.x * 1000)) && (m_CurrentLayer == m_MapData[i].m_Layer))
							if ((int)(tempP.y * 1000) >= (int)(m_MapData[i].m_Position.y * 1000))
							{
								if (m_SelectedItem)
								{
									if (m_MapData[i].m_MenuId == m_SelectedItem->m_MenuId)
									{
										m_SelectedItem = NULL;
										m_SelectArea = {};
									}
								}
								m_MapData.erase(m_MapData.begin() + i);
							}
							*/
						if (tempP.x >= m_MapData[i].m_ColorArea.left - m_StartPoint.x && tempP.x <= m_MapData[i].m_ColorArea.right - m_StartPoint.x && m_CurrentLayer == m_MapData[i].m_Layer)
						{
							if (tempP.y >= m_MapData[i].m_ColorArea.top - m_StartPoint.y && tempP.y <= m_MapData[i].m_ColorArea.bottom - m_StartPoint.y)
							{
								if (m_SelectedItem)
								{
									if (m_MapData[i].m_MenuId == m_SelectedItem->m_MenuId)
									{
										m_SelectedItem = NULL;
										m_SelectArea = {};
									}
								}
								m_MapData.erase(m_MapData.begin() + i);
							}
						}
					}
					for (int i = 0; i < (int)m_Shapes.size(); i++)
					{
						if (m_Shapes[i].m_Shape == Shapes::Sphere)
						{
							if (abs(Distance({ m_Shapes[i].m_ColorArea.left, m_Shapes[i].m_ColorArea.top }, p)) < m_Shapes[i].m_ConvertedRadius)
							{
								if (m_SelectedShape)
								{
									if (m_Shapes[i].m_MenuId == m_SelectedShape->m_MenuId)
									{
										m_SelectedShape = NULL;
										m_SelectArea = {};
									}
								}
								m_Shapes.erase(m_Shapes.begin() + i);
							}
						}
						else
						{
							if (InRect(m_Shapes[i].m_ColorArea, p) && m_CurrentLayer == m_Shapes[i].m_Layer)
							{
								if (m_SelectedShape)
								{
									if (m_Shapes[i].m_MenuId == m_SelectedShape->m_MenuId)
									{
										m_SelectedShape = NULL;
										m_SelectArea = {};
									}
								}
								m_Shapes.erase(m_Shapes.begin() + i);
							}
						}
					}
					for (int i = 0; i < (int)m_Walls.size(); i++)
					{
						if (m_Walls[i].m_Layer == m_CurrentLayer)
						{
							if (TouchingLine(m_Walls[i].m_StartPoint, m_Walls[i].m_EndPoint, p))
							{
								m_Walls.erase(m_Walls.begin() + i);
							}
						}
					}
				}

				for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
				{
					if (InRect(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_ColorArea, p))
					{
						if (m_SelectedItem)
						{
							if (m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_MenuId == m_SelectedItem->m_MenuId)
							{
								m_SelectedItem = NULL;
								m_SelectArea = {};
							}
						}
						m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.erase(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.begin() + i);
					}
				}
				for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls.size(); i++)
				{
					if (TouchingLine(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls[i].m_StartPoint, m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls[i].m_EndPoint, p))
					{
						m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls.erase(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls.begin() + i);
					}
				}
				for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
				{
					ShapeData* tShape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
					if (tShape->m_Shape == Shapes::Sphere)
					{
						if (abs(Distance({ tShape->m_ColorArea.left, tShape->m_ColorArea.top }, p)) < tShape->m_ConvertedRadius)
						{
							if (m_SelectedShape)
							{
								if (tShape->m_MenuId == m_SelectedShape->m_MenuId)
								{
									m_SelectedShape = NULL;
									m_SelectArea = {};
								}
							}
							m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.erase(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.begin() + i);
						}
					}
					else
					{
						if (InRect(tShape->m_ColorArea, p))
						{
							if (m_SelectedShape)
							{
								if (tShape->m_MenuId == m_SelectedShape->m_MenuId)
								{
									m_SelectedShape = NULL;
									m_SelectArea = {};
								}
							}
							m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.erase(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.begin() + i);
						}
					}
				}
			}
			else
			{
				D2D1_POINT_2F tempP = {};
				tempP.x = m_MouseOverArea.p1.x / m_Scale;
				tempP.x -= m_StartPoint.x;
				tempP.y = m_MouseOverArea.p1.y / m_Scale - m_Size;
				tempP.y -= m_StartPoint.y;
				for (int i = 0; i < (int)m_MapData.size(); i++)
				{
					if (((int)(tempP.x * 1000) == (int)(m_MapData[i].m_Position.x * 1000)) && (m_CurrentLayer == m_MapData[i].m_Layer))
						if ((int)(tempP.y * 1000) == (int)(m_MapData[i].m_Position.y * 1000))
						{
							if (m_SelectedItem)
							{
								if (m_MapData[i].m_MenuId == m_SelectedItem->m_MenuId)
								{
									m_SelectedItem = NULL;
									m_SelectArea = {};
								}
							}
							m_MapData.erase(m_MapData.begin() + i);
						}
				}
			}
		}
	}

	if (m_Keyboard.m_InsertKey.IsPressed(timeTotal, 0.2f))
	{
		if (InRect(m_MenuAreaRect, p))
		{
			if (!m_ShowItemMenu)
			{
				if (p.y > m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom)
				{
					m_InitArea.InsertTurn(p);
				}
			}
		}
	}

	if (m_Keyboard.m_SelectKey.IsPressed(timeTotal, 0.2))
	{
		if (InRect(m_MenuAreaRect, p))
		{
			if (m_ShowItemMenu)
			{
				if (p.y < m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_AdditionalOptions;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle PC Colors"))
							{
								m_ShowBgColors ^= true;
								m_ItemMenuArea.m_ShowBgColor ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Initiative"))
							{
								m_ShowItemMenu ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Keep Aspect"))
							{
								m_NextKeepAspect ^= true;
								m_ItemMenuArea.m_KeepRatio = m_NextKeepAspect;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Lock to Grid"))
							{
								m_ItemMenuArea.m_LockToGrid ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_LOCKTOGRID, (m_ItemMenuArea.m_LockToGrid << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Grid on Top"))
							{
								m_ItemMenuArea.m_GridOnTop ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_GRIDONTOP, (m_ItemMenuArea.m_GridOnTop << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Turn Counter"))
							{
								if (!m_ItemMenuArea.m_ShowCounter)
								{
									m_ItemMenuArea.m_ShowCounter = true;
									m_Counter.m_SetMenuID(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (m_Counter.m_GetCount())
								{
									m_Counter.m_ResetCount(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (!m_ItemMenuArea.m_CounterCount)
								{
									m_ItemMenuArea.m_ShowCounter = false;
									m_Counter.m_ResetCount();								
								}
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Add Custom Color"))
							{
								if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
								{
									CHOOSECOLOR ccolor = {};
									static COLORREF cColorArray[16];
									static DWORD rgbCurrent;

									ZeroMemory(&ccolor, sizeof(CHOOSECOLOR));
									ccolor.lStructSize = sizeof(CHOOSECOLOR);
									ccolor.hwndOwner = GameController::hWnd;
									ccolor.lpCustColors = (LPDWORD)cColorArray;
									ccolor.rgbResult = rgbCurrent;
									ccolor.Flags = CC_FULLOPEN | CC_RGBINIT;

									if (ChooseColor(&ccolor))
									{
										m_NumberCustomColors++;

										Asset tCus;
										tCus.m_MenuID = COLOR_CUSTOMS + m_NumberCustomColors;
										tCus.m_Layer = m_CurrentLayer;
										tCus.m_Type = m_ItemMenuArea.m_SelectedType->m_Type;
										tCus.m_Submenu = m_ItemMenuArea.m_SelectedSubMenu->m_Submenu;
										tCus.m_Name = std::to_string(tCus.m_MenuID);
										tCus.m_KeepAspect = false;
										tCus.m_KeepIconAspect = false;
										tCus.m_DefaultInitOrder = false;
										tCus.m_BgColor = { (float)(GetRValue(ccolor.rgbResult) / 255.0f), (float)(GetGValue(ccolor.rgbResult) / 255.0f), (float)(GetBValue(ccolor.rgbResult) / 255.0f), 1.0f };
										m_AssetList.push_back(tCus);
										m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
									}
								}
							}
						}
						temp.pop();
					}
				}
				else if (p.y < m_ItemMenuArea.m_ItemTypes.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_ItemTypes;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Type, temp.front().m_ItemText))
								{
									m_ItemMenuArea.m_SelectedType = &m_AssetList[i];
									break;
								}
							}
							break;
						}
						temp.pop();
					}
					m_ItemMenuArea.RebuildSubMenuList(m_AssetList);
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if (p.y < m_ItemMenuArea.m_SubMenus.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_SubMenus;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Submenu, temp.front().m_ItemText))
								{
									m_ItemMenuArea.m_SelectedSubMenu = &m_AssetList[i];
									break;
								}
							}
							break;
						}
						temp.pop();
					}
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if (p.y < m_ItemMenuArea.m_SelectedSizeCategory->back().m_DrawArea.bottom)
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
					{
						if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
						{
							std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Thickness;
							while (!temp.empty())
							{
								if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
								{
									m_ItemMenuArea.SetSelectedThickness(m_ItemMenuArea.m_Thickness, _wtof(temp.front().m_ItemText.c_str()));
									break;
								}
								temp.pop();
							}
						}
						else
						{
							std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Radius;
							while (!temp.empty())
							{
								if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
								{
									m_ItemMenuArea.SetSelectedRadius(m_ItemMenuArea.m_Radius, (int)(_wtof(temp.front().m_ItemText.c_str())));
									break;
								}
								temp.pop();
							}
						}
					}	
					else if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Free Draw"))
					{
						std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_PenSize;
						while (!temp.empty())
						{
							if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
							{
								m_ItemMenuArea.SetSelectedPenSize(m_ItemMenuArea.m_PenSize, _wtof(temp.front().m_ItemText.c_str()));
								break;
							}
							temp.pop();
						}
					}
					else
					{
						std::queue<ItemMenuItem> temp = *m_ItemMenuArea.m_SelectedSizeCategory;
						while (!temp.empty())
						{
							if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
							{
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Fine"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Fine);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Diminutive"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Diminutive);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Tiny"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Tiny);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Small"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Small);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Medium"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Medium);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Large"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Large);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Huge"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Huge);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Gargantuan"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Gargantuan);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Colossal"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Colossal);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Original Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Original);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"2x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_TwoTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"3x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_ThreeTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"4x Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_FourTimes);
								}
								if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Half Size"))
								{
									m_ItemMenuArea.SetSelectedSize(m_ItemMenuArea.m_SizeCategory, Size::m_Half);
								}
								break;
							}
							temp.pop();
						}
						m_NextSize = m_ItemMenuArea.m_SelectedSize;
					}
					m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
				}
				else if ((!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line")) && (p.y < m_ItemMenuArea.m_Width.back().m_DrawArea.bottom))
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Width;
					while (!temp.empty())
					{
						if (InRect(temp.front().m_DrawArea, p))
						{
							m_ItemMenuArea.SetSelectedWidth(m_ItemMenuArea.m_Width, (int)(_wtof(temp.front().m_ItemText.c_str())));
							break;
						}
						temp.pop();
					}
				}
				else if (p.y < m_ItemMenuArea.m_Sprites.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_Sprites;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							for (int i = 0; i < (int)m_AssetList.size(); i++)
							{
								if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Submenu, temp.front().m_SubMenu))
								{/*** moving to different section
									if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Name, temp.front().m_ItemText))
									{
										if (!_wcsnicmp(temp.front().m_ItemText.c_str(), L"Add Custom", strlen("Add Custom")))
										{
											Asset tCus;
											tCus.m_MenuID = COLOR_CUSTOMS;			//plus some sort of counter
											tCus.m_Layer = m_CurrentLayer;
											tCus.m_Type = m_ItemMenuArea.m_SelectedType->m_Type;
											tCus.m_Submenu = m_ItemMenuArea.m_SelectedSubMenu->m_Submenu;
											tCus.m_Name = std::to_string(1.0f);		//set this equal to the chosen rgb values
											tCus.m_KeepAspect = false;
											tCus.m_KeepIconAspect = false;
											tCus.m_DefaultInitOrder = false;
											tCus.m_BgColor = { 1.0f, 1.0f, 0.0f, 1.0f };	//set to chosen color value
											m_AssetList.push_back(tCus);
										}
										else
										m_ItemMenuArea.m_SelectedItem = &m_AssetList[i];
										break;
									}*/
									if (m_ItemMenuArea.StringEqual(m_AssetList[i].m_Name, temp.front().m_ItemText))
									{
										m_ItemMenuArea.m_SelectedItem = &m_AssetList[i];
										break;
									}
								}
							}
							break;
						}
						temp.pop();
					}
				}
			}
			else
			{
				//this is where you can choose an initiative entry and have it swap with another entry
				if (p.y < m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom)
				{
					std::queue<ItemMenuItem> temp = m_ItemMenuArea.m_AdditionalOptions;
					while (!temp.empty())
					{
						if (p.x > temp.front().m_DrawArea.left && p.x < temp.front().m_DrawArea.right && p.y < temp.front().m_DrawArea.bottom && p.y > temp.front().m_DrawArea.top)
						{
							if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle PC Colors"))
							{
								m_ItemMenuArea.m_ShowBgColor ^= true;
								m_ShowBgColors ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Keep Aspect"))
							{
								m_NextKeepAspect ^= true;
								m_ItemMenuArea.m_KeepRatio = m_NextKeepAspect;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Toggle Initiative"))
							{
								m_ShowItemMenu ^= true;
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Lock to Grid"))
							{
								m_ItemMenuArea.m_LockToGrid ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_LOCKTOGRID, (m_ItemMenuArea.m_LockToGrid << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Grid on Top"))
							{
								m_ItemMenuArea.m_GridOnTop ^= true;
								CheckMenuItem(GetMenu(GameController::hWnd), MENU_GRIDONTOP, (m_ItemMenuArea.m_GridOnTop << 3));
							}
							else if (!_wcsicmp(temp.front().m_ItemText.c_str(), L"Turn Counter"))
							{
								if (!m_ItemMenuArea.m_ShowCounter)
								{
									m_ItemMenuArea.m_ShowCounter = true;
									m_Counter.m_SetMenuID(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (m_Counter.m_GetCount())
								{
									m_Counter.m_ResetCount(m_InitArea.m_InitOrder.front()->m_MenuID);
								}
								else if (!m_ItemMenuArea.m_CounterCount)
								{
									m_ItemMenuArea.m_ShowCounter = false;
									m_Counter.m_ResetCount();
								}
							}
						}
						temp.pop();
					}
				}
				else
					m_InitArea.SwapTurn(p);
			}
		}
		else
		{
			if (!m_ShowItemMenu)
			{
				if (m_SelectedItem || m_SelectedShape)
				{
					D2D1_POINT_2F Position = {};
					Position.x = m_MouseOverArea.p1.x / m_Scale;
					Position.x -= m_StartPoint.x;
					Position.y = m_MouseOverArea.p1.y / m_Scale;
					Position.y -= m_StartPoint.y;

					bool blankspace = true;

					if (m_SelectedItem || m_SelectedShape)
					{
						for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
						{
							MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
							if (InRect(tmap->m_ColorArea, p))
							{
								D2D1_POINT_2F temp = tmap->m_Position;
								if (m_SelectedItem)
								{
									tmap->m_Position = m_SelectedItem->m_Position;
									m_SelectedItem->m_Position = temp;
									m_SelectedItem = NULL;
								}
								if (m_SelectedShape)
								{
									tmap->m_Position = m_SelectedShape->m_Position;
									m_SelectedShape->m_Position = temp;
									m_SelectedShape = NULL;
								}
							}
						}
						for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
						{
							ShapeData* tshape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
							if (InRect(tshape->m_ColorArea, p))
							{
								D2D1_POINT_2F temp = tshape->m_Position;
								if (m_SelectedItem)
								{
									tshape->m_Position = m_SelectedItem->m_Position;
									m_SelectedItem->m_Position = temp;
									m_SelectedItem = NULL;
								}
								if (m_SelectedShape)
								{
									tshape->m_Position = m_SelectedShape->m_Position;
									m_SelectedShape->m_Position = temp;
									m_SelectedShape = NULL;
								}
								blankspace = false;
								break;
							}
						}
						/*
						for (int i = 0; i < (int)m_MapData.size(); i++)
						{
							if (p.x >= m_MapData[i].m_ColorArea.left && p.x <= m_MapData[i].m_ColorArea.right && m_CurrentLayer == m_MapData[i].m_Layer)
							{
								if (p.y >= m_MapData[i].m_ColorArea.top && p.y <= m_MapData[i].m_ColorArea.bottom)
								{
									D2D1_POINT_2F temp = m_MapData[i].m_Position;
									if (m_SelectedItem)
									{
										m_MapData[i].m_Position = m_SelectedItem->m_Position;
										m_SelectedItem->m_Position = temp;
										m_SelectedItem = NULL;
									}
									if (m_SelectedShape)
									{
										m_MapData[i].m_Position = m_SelectedShape->m_Position;
										m_SelectedShape->m_Position = temp;
										m_SelectedShape = NULL;
									}										
									blankspace = false;
									break;
								}
							}
						}
						
						for (int i = 0; i < (int)m_Shapes.size(); i++)
						{
							if (p.x >= m_Shapes[i].m_ColorArea.left && p.x <= m_Shapes[i].m_ColorArea.right && m_CurrentLayer == m_Shapes[i].m_Layer)
							{
								if (p.y >= m_Shapes[i].m_ColorArea.top && p.y <= m_Shapes[i].m_ColorArea.bottom)
								{
									D2D1_POINT_2F temp = m_Shapes[i].m_Position;
									if (m_SelectedItem)
									{
										m_Shapes[i].m_Position = m_SelectedItem->m_Position;
										m_SelectedItem->m_Position = temp;
										m_SelectedItem = NULL;
									}
									if (m_SelectedShape)
									{
										m_Shapes[i].m_Position = m_SelectedShape->m_Position;
										m_SelectedShape->m_Position = temp;
										m_SelectedShape = NULL;
									}			
									blankspace = false;
									break;
								}
							}
						}
						*/
					}
					if (blankspace)
					{
						if (m_ItemMenuArea.m_LockToGrid)
						{
							if (m_SelectedItem)
								m_SelectedItem->m_Position = Position;
							if (m_SelectedShape)
								m_SelectedShape->m_Position = Position;
						}
						else
						{
							if (m_SelectedItem)
								m_SelectedItem->m_Position = { (p.x / m_Scale) - m_StartPoint.x, (p.y / m_Scale) - m_StartPoint.y };
							if (m_SelectedShape)
								m_SelectedShape->m_Position = { (p.x / m_Scale) - m_StartPoint.x, (p.y / m_Scale) - m_StartPoint.y };
						}
						if (m_SelectedItem)
						{
							m_SelectedItem->m_Layer = m_CurrentLayer;
							m_SelectedItem = NULL;
						}
						if (m_SelectedShape)
						{
							m_SelectedShape->m_Layer = m_CurrentLayer;
							m_SelectedShape = NULL;
						}
					}
				}
				else
				{
					D2D1_POINT_2F Position = {};
					Position.x = p.x;
					Position.y = p.y;

					for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
					{
						MapData* tmap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
						if (InRect(tmap->m_ColorArea, Position))
						{
							m_SelectedItem = tmap;
							float scaledSize = m_Size * m_Scale;
							m_RulerStartPoint.x = (float)((int)(p.x / scaledSize));
							m_RulerStartPoint.x *= scaledSize;
							m_RulerStartPoint.x += scaledSize / 2.0f;
							m_RulerStartPoint.y = (float)((int)(p.y / scaledSize));
							m_RulerStartPoint.y *= scaledSize;
							m_RulerStartPoint.y += scaledSize / 2.0f;
							break;
						}
					}
					if (!m_SelectedItem)
					{
						for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
						{
							ShapeData* tshape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
							if (InRect(tshape->m_ColorArea, Position))
							{
								m_SelectedShape = tshape;
								break;
							}
						}
					}
				}
			}
		}
	}

	if (m_Keyboard.m_DrawKey.IsPressed(timeTotal, 0.10f))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			if (m_ShowItemMenu)
			{
				if (m_GridType == Grid::m_Square)
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Type.c_str(), "Shapes"))
					{
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Walls"))
						{
							if (m_TempWallStartPoint.x != 0 || m_TempWallStartPoint.y != 0)
							{
								WallsData w;
								w.m_StartPoint = m_TempWallStartPoint;
								w.m_EndPoint = m_PreviewPoint;
								w.m_EndPoint.x /= m_Scale;
								w.m_EndPoint.y /= m_Scale;
								w.m_EndPoint.x -= m_StartPoint.x;
								w.m_EndPoint.y -= m_StartPoint.y;
								w.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
								w.m_Thickness = m_ItemMenuArea.m_SelectedThickness;
								w.m_Layer = m_CurrentLayer;
								m_TempWallStartPoint = {};
								//m_Walls.push_back(w);
								m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Walls.push_back(w);
							}
							else
							{
								m_TempWallStartPoint = m_PreviewPoint;
								m_TempWallStartPoint.x /= m_Scale;
								m_TempWallStartPoint.y /= m_Scale;
								m_TempWallStartPoint.x -= m_StartPoint.x;
								m_TempWallStartPoint.y -= m_StartPoint.y;
							}
						}
						else if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Cone"))
						{
							if (m_TempWallStartPoint.x != 0 || m_TempWallStartPoint.y != 0)
							{//if an anchor point is set already, figure out the direction angle
								//after thinking this over, i don't see a reason to store the result; i cannot think of any persistent cone spells, so i'll just leave the preview
								//and that'll be it; at least for now; group has a way of proving me wrong.
							}
							else
							{//if no anchor point set yet, set one
								if (m_ItemMenuArea.m_LockToGrid)
								{
									m_TempWallStartPoint.x = p.x / m_Scale - m_StartPoint.x;
									m_TempWallStartPoint.x = (float)((int)(m_TempWallStartPoint.x / m_Size));
									m_TempWallStartPoint.x *= m_Size;
									if ((int)p.x % (int)(m_Size * m_Scale) >= m_Size * m_Scale / 2.0f)
									{
										m_TempWallStartPoint.x += m_Size;
									}
									m_TempWallStartPoint.y = p.y / m_Scale - m_StartPoint.y;
									m_TempWallStartPoint.y = (float)((int)(m_TempWallStartPoint.y / m_Size));
									m_TempWallStartPoint.y *= m_Size;
									if ((int)p.y % (int)(m_Size * m_Scale) >= m_Size * m_Scale / 2.0f)
									{
										m_TempWallStartPoint.y += m_Size;
									}
								}
								else
								{
									m_TempWallStartPoint.x = (p.x / m_Scale - m_StartPoint.x);
									m_TempWallStartPoint.y = (p.y / m_Scale - m_StartPoint.y);
								}
							}
								
						}
						else if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Line"))
						{
							if (m_TempWallStartPoint.x != 0 || m_TempWallStartPoint.y != 0)
							{//if an anchor point is set already, figure out the direction angle
							 //after thinking this over, i don't see a reason to store the result; i cannot think of any persistent cone spells, so i'll just leave the preview
							 //and that'll be it; at least for now; group has a way of proving me wrong.
							}
							else
							{//if no anchor point set yet, set one
								if (m_ItemMenuArea.m_LockToGrid)
								{
									m_TempWallStartPoint.x = p.x / m_Scale - m_StartPoint.x;
									m_TempWallStartPoint.x = (float)((int)(m_TempWallStartPoint.x / m_Size));
									m_TempWallStartPoint.x *= m_Size;
									if ((int)p.x % (int)(m_Size * m_Scale) >= m_Size * m_Scale / 2.0f)
									{
										m_TempWallStartPoint.x += m_Size;
									}
									m_TempWallStartPoint.y = p.y / m_Scale - m_StartPoint.y;
									m_TempWallStartPoint.y = (float)((int)(m_TempWallStartPoint.y / m_Size));
									m_TempWallStartPoint.y *= m_Size;
									if ((int)p.y % (int)(m_Size * m_Scale) >= m_Size * m_Scale / 2.0f)
									{
										m_TempWallStartPoint.y += m_Size;
									}
								}
								else
								{
									m_TempWallStartPoint.x = (p.x / m_Scale - m_StartPoint.x);
									m_TempWallStartPoint.y = (p.y / m_Scale - m_StartPoint.y);
								}
							}

						}
					}
				}
			}
		}
	}

	if (m_Keyboard.m_DrawKey.IsPressed(timeTotal, false))
	{
		if (InRect(m_DrawAreaRect, p))
		{
			if (m_ShowItemMenu)
			{
				if (m_GridType == Grid::m_Square)
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Type.c_str(), "Shapes"))
					{
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Filled Cube"))
						{
							ShapeData s;
							s.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							s.m_Shape = Shapes::FilledCube;
							s.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							s.m_Layer = m_CurrentLayer;

							s.m_Position = m_MouseOverArea.p1;
							s.m_Position.x /= m_Scale;
							s.m_Position.y /= m_Scale;
							s.m_Position.x -= m_StartPoint.x;
							s.m_Position.y -= m_StartPoint.y;

							s.m_Radius = m_ItemMenuArea.m_SelectedRadius;

							bool add = true;

							/*
							for (int i = 0; i < (int)m_Shapes.size(); i++)
							{
								if (((int)(s.m_Position.x * 1000) == (int)(m_Shapes[i].m_Position.x * 1000)) && (s.m_Layer == m_Shapes[i].m_Layer))
								{
									if ((int)(s.m_Position.y * 1000) == (int)(m_Shapes[i].m_Position.y * 1000))
									{
										add = false;
										m_Shapes[i].m_MenuId = s.m_MenuId;
										m_Shapes[i].m_Color = s.m_Color;
										m_Shapes[i].m_Radius = s.m_Radius;
										break;
									}
								}
							}
							*/
							for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
							{
								ShapeData* tshape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
								if ((int)(s.m_Position.y * 1000) == (int)(tshape->m_Position.y * 1000))
								{
									if ((int)(s.m_Position.x * 1000) == (int)(tshape->m_Position.x * 1000))
									{
										add = false;
										tshape->m_MenuId = s.m_MenuId;
										tshape->m_Color = s.m_Color;
										tshape->m_Radius = s.m_Radius;
										break;
									}
								}
							}
							if (add)
							{
								//m_Shapes.push_back(s);
								m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.push_back(s);
							}
						}
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Cube"))
						{
							ShapeData s;
							s.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							s.m_Shape = Shapes::Cube;
							s.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							s.m_Layer = m_CurrentLayer;

							s.m_Position = m_MouseOverArea.p1;
							s.m_Position.x /= m_Scale;
							s.m_Position.y /= m_Scale;
							s.m_Position.x -= m_StartPoint.x;
							s.m_Position.y -= m_StartPoint.y;

							s.m_Radius = m_ItemMenuArea.m_SelectedRadius;

							bool add = true;
							for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.size(); i++)
							{
								ShapeData* tshape = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes[i];
								if ((int)(s.m_Position.y * 1000) == (int)(tshape->m_Position.y * 1000))
								{
									if ((int)(s.m_Position.x * 1000) == (int)(tshape->m_Position.x * 1000))
									{
										add = false;
										tshape->m_MenuId = s.m_MenuId;
										tshape->m_Color = s.m_Color;
										tshape->m_Radius = s.m_Radius;
										break;
									}
								}
							}
							if (add)
							{
								//m_Shapes.push_back(s);
								m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.push_back(s);
							}
						}
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Line"))
						{

						}
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Sphere"))
						{
							ShapeData s;
							s.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							s.m_Shape = Shapes::Sphere;
							s.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							s.m_Layer = m_CurrentLayer;
							
							s.m_Position = m_MouseOverArea.p1;
							s.m_Position.x /= m_Scale;
							s.m_Position.y /= m_Scale;
							s.m_Position.x -= m_StartPoint.x;
							s.m_Position.y -= m_StartPoint.y;

							s.m_Radius = m_ItemMenuArea.m_SelectedRadius;
							s.m_ConvertedRadius = (s.m_Radius / 5.0f) * m_Size * m_Scale;
							
							bool add = true;
							for (int i = 0; i < (int)m_Shapes.size(); i++)
							{
								//check if location is in use already, overwrite if it is
							}
							if (add)
							{
								//m_Shapes.push_back(s);
								m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_Shapes.push_back(s);
							}
						}
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Cone"))
						{

						}
						
					}
					else if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Type.c_str(), "Free Draw"))
					{
						if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Circular"))
						{
							ShapeData s;
							s.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							s.m_Shape = Shapes::FilledSphere;
							s.m_Color = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							s.m_Layer = m_CurrentLayer;

							//s.m_Position = m_MouseOverArea.p1;
							s.m_Position.x = m_MouseOverArea.p1.x + ((m_MouseOverArea.p2.x - m_MouseOverArea.p1.x) / 2.0f);
							s.m_Position.y = m_MouseOverArea.p1.y + ((m_MouseOverArea.p4.y - m_MouseOverArea.p1.y) / 2.0f);
							s.m_Position.x /= m_Scale;
							s.m_Position.y /= m_Scale;
							s.m_Position.x -= m_StartPoint.x;
							s.m_Position.y -= m_StartPoint.y;

							s.m_Radius = m_ItemMenuArea.m_SelectedPenSize;

							bool add = true;
							for (int i = 0; i < (int)m_Shapes.size(); i++)
							{
								if (((int)(s.m_Position.x * 1000) == (int)(m_Shapes[i].m_Position.x * 1000)) && (s.m_Layer == m_Shapes[i].m_Layer))
								{
									if ((int)(s.m_Position.y * 1000) == (int)(m_Shapes[i].m_Position.y * 1000))
									{
										add = false;
										m_Shapes[i].m_MenuId = s.m_MenuId;
										m_Shapes[i].m_Color = s.m_Color;
										m_Shapes[i].m_Radius = s.m_Radius;
										m_Shapes[i].m_Shape = s.m_Shape;
										break;
									}
								}
							}
							if (add)
								m_Shapes.push_back(s);
						}
						else if (!_stricmp(m_ItemMenuArea.m_SelectedItem->m_Submenu.c_str(), "Square"))
						{
							//to be added later
						}
					}
					else
					{
						if (m_UseOld)
						{
							int mapSize = (int)m_MapData.size();

							MapData m;
							m.m_SizeCategory = m_NextSize;
							m.m_KeepRatio = m_NextKeepAspect;
							m.m_Position = m_MouseOverArea.p1;
							m.m_Position.x /= m_Scale;
							m.m_Position.x -= m_StartPoint.x;
							m.m_Position.y /= m_Scale;
							m.m_Position.y -= m_StartPoint.y;
							m.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							m.m_Layer = m_CurrentLayer;
							m.m_BgColor = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							if (m.m_BgColor.r == 0.0f && m.m_BgColor.g == 0.0f && m.m_BgColor.b == 0.0f)
								m_ItemMenuArea.m_SelectedItem->m_BgColor.a = 0.0f;

							bool add = true;
							for (int i = 0; i < (int)m_MapData.size(); i++)
							{
								if (((int)(m.m_Position.x * 1000) == (int)(m_MapData[i].m_Position.x * 1000)) && (m.m_Layer == m_MapData[i].m_Layer))
									if ((int)(m.m_Position.y * 1000) == (int)(m_MapData[i].m_Position.y * 1000))
									{
										add = false;
										m_MapData[i].m_MenuId = m.m_MenuId;
										m_MapData[i].m_SizeCategory = m.m_SizeCategory;
										m_MapData[i].m_BgColor = m.m_BgColor;
										m_MapData[i].m_Tag = 0;
										break;
									}
							}

							if (add)
							{
								m_MapData.push_back(m);
							}
						}
						else
						{
							MapData m;
							m.m_SizeCategory = m_NextSize;
							m.m_KeepRatio = m_NextKeepAspect;
							m.m_Position = m_MouseOverArea.p1;
							m.m_Position.x /= m_Scale;
							m.m_Position.x -= m_StartPoint.x;
							m.m_Position.y /= m_Scale;
							m.m_Position.y -= m_StartPoint.y;
							m.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
							m.m_Layer = m_CurrentLayer;
							m.m_BgColor = m_ItemMenuArea.m_SelectedItem->m_BgColor;
							if (m.m_BgColor.r == 0.0f && m.m_BgColor.g == 0.0f && m.m_BgColor.b == 0.0f)
								m_ItemMenuArea.m_SelectedItem->m_BgColor.a = 0.0f;

							bool add = true;
							for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
							{
								MapData* cMap = &m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i];
								if ((int)(m.m_Position.x * 1000) == (int)(cMap->m_Position.x * 1000))
								{
									if ((int)(m.m_Position.y * 1000) == (int)(cMap->m_Position.y * 1000))
									{

										add = false;
										cMap->m_MenuId = m.m_MenuId;
										cMap->m_SizeCategory = m.m_SizeCategory;
										cMap->m_BgColor = m.m_BgColor;
										cMap->m_Tag = 0;
									}
								}
							}

							if (add)
								m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.push_back(m);
						}
					}
				}
				else if (m_GridType == Grid::m_Hexagon)
				{
					MapData m;
					m.m_SizeCategory = m_NextSize;
					m.m_KeepRatio = m_NextKeepAspect;
					m.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
					m.m_Layer = m_CurrentLayer;
					m.m_Position = { (m_HexMouseOverArea.p5.x / m_Scale) - m_StartPoint.x, (m_HexMouseOverArea.p0.y / m_Scale) - m_StartPoint.y };
					if (m.m_BgColor.r == 0.0f && m.m_BgColor.g == 0.0f && m.m_BgColor.b == 0.0f)
						m_ItemMenuArea.m_SelectedItem->m_BgColor.a = 0.0f;
					m_MapData.push_back(m);
				}
				else
				{
					MapData m;
					m.m_SizeCategory = m_NextSize;
					m.m_KeepRatio = m_NextKeepAspect;
					if (m_ItemMenuArea.m_LockToGrid)
					{
						m.m_Position = { 0.0f, 0.0f };
						m.m_Position.x = (float)((int)(p.x / (m_Size * m_Scale)));
						m.m_Position.x *= m_Size;
						m.m_Position.x -= m_StartPoint.x;
						m.m_Position.y = (float)((int)(p.y / (m_Size * m_Scale)));
						m.m_Position.y *= m_Size;
						m.m_Position.y -= m_StartPoint.y;
					}
					else
					{
						m.m_Position.x = p.x - m_StartPoint.x;
						m.m_Position.y = p.y - m_StartPoint.y;
					}
					m.m_MenuId = m_ItemMenuArea.m_SelectedItem->m_MenuID;
					m.m_Layer = m_CurrentLayer;
					m.m_BgColor = m_ItemMenuArea.m_SelectedItem->m_BgColor;
					if (m.m_BgColor.r == 0.0f && m.m_BgColor.g == 0.0f && m.m_BgColor.b == 0.0f)
						m_ItemMenuArea.m_SelectedItem->m_BgColor.a = 0.0f;

					bool add = true;
					for (int i = 0; i < (int)m_MapData.size(); i++)
					{
						if (((int)(m.m_Position.x * 1000) == (int)(m_MapData[i].m_Position.x * 1000)) && (m.m_Layer == m_MapData[i].m_Layer))
							if ((int)(m.m_Position.y * 1000) == (int)(m_MapData[i].m_Position.y * 1000))
							{
								add = false;
								m_MapData[i].m_MenuId = m.m_MenuId;
								m_MapData[i].m_SizeCategory = m.m_SizeCategory;
								m_MapData[i].m_BgColor = m.m_BgColor;
								m_MapData[i].m_Tag = 0;
							}
					}
					if (add)
						m_MapData.push_back(m);
				}
			}
		}
	}

	if (m_Keyboard.m_RotationKey.IsPressed(timeTotal, 0.10f))
	{
		if (m_UseOld)
		{
			for (int i = 0; i < m_MapData.size(); i++)
			{
				if (InRect(m_MapData[i].m_ColorArea, p))
				{
					if (m_MapData[i].m_Layer == m_CurrentLayer)
					{
						m_MapData[i].m_Rotation += 45.0f;
						if (m_MapData[i].m_Rotation == 360.0f)
							m_MapData[i].m_Rotation = 0.0f;
						break;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData.size(); i++)
			{
				if (InRect(m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_ColorArea, p))
				{
					m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_Rotation += 45.0f;
					if (m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_Rotation == 360.0f)
						m_Rooms[m_CurrentRoom].m_Layer[m_CurrentLayer].m_MapData[i].m_Rotation = 0.0f;
					break;
				}
			}
		}
	}

	if (m_Keyboard.m_ZoomInKey.IsPressed(timeTotal, false)) IncreaseSize();
	if (m_Keyboard.m_ZoomOutKey.IsPressed(timeTotal, false)) DecreaseSize();
	if (m_Keyboard.m_LeftKey.IsPressed(timeTotal, 0.10)) m_StartPoint.x += m_Size;
	if (m_Keyboard.m_RightKey.IsPressed(timeTotal, 0.10)) m_StartPoint.x -= m_Size;
	if (m_Keyboard.m_UpKey.IsPressed(timeTotal, 0.10)) m_StartPoint.y += m_Size;
	if (m_Keyboard.m_DownKey.IsPressed(timeTotal, 0.10)) m_StartPoint.y -= m_Size;
}

void Level1::Update(float timeTotal, float timeDelta, D2D1_POINT_2F p)
{
	//calculations/movments/ect.

	if (GetActiveWindow() == GameController::hWnd)
		if (!m_Geometry)
			ReactInputs(timeTotal, timeDelta, p);
		else
			ReactInputsGeometryMode(timeTotal, timeDelta, p);
	else
		Sleep(5);
	
	if (m_ItemMenuArea.m_AdditionalOptions.size())
		m_InitArea.m_DrawArea.top = m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom;

	if (m_RecalcLines)
	{
		EmptyLineQueue(m_Lines);
		if (m_GridType == Grid::m_Dimetrique)
		{
			m_Lines = CalculateGridPointsDimetrique(m_DrawArea, m_Size, m_Scale);
			gfx->CreateDimetriqueGridPatternBrush(m_Size, m_Scale);
		}
		else if (m_GridType == Grid::m_Hexagon)
		{
			CalcHexGrid();
			gfx->CreateHexGridPatternBrush(m_Size, m_Scale);
		}
		else
		{
			//m_Lines = CalculateGridPoints(m_DrawArea, m_Size, m_Scale);
			gfx->CreateSquareGridPatternBrush(m_Size, m_Scale);
			gfx->CreateSquareGridPattern();
		}

		m_RecalcLines = false;
	}
	if (InRect(m_DrawAreaRect, p))
	{
		if (m_GridType == Grid::m_Hexagon)
			m_HexMouseOverArea = CalculateMouseOverArea(p);
		else
			m_MouseOverArea = CalculateMouseOverArea(m_Size * m_Scale, (int)(WINDOW_WIDTH / (m_Size * m_Scale)), (int)(WINDOW_HEIGHT / (m_Size * m_Scale)), p);
		if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Walls") || !_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Cone") || !_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line") || !_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Circular"))
		{
			if (m_ItemMenuArea.m_LockToGrid)
			{
				m_PreviewPoint.x = (float)((int)(p.x / (m_Scale * m_Size)));
				m_PreviewPoint.y = (float)((int)(p.y / (m_Scale * m_Size)));
				m_PreviewPoint.x *= m_Scale * m_Size;
				m_PreviewPoint.y *= m_Scale * m_Size;

				double size = (double)(m_Scale * m_Size);
				double val = (double)(p.x - m_PreviewPoint.x);
				if (val > (size / 2.0))
					m_PreviewPoint.x += (float)(size);
				val = p.y - m_PreviewPoint.y;
				if (val > (size / 2.0))
					m_PreviewPoint.y += (float)(size);
			}
			else
			{
				m_PreviewPoint = p;
			}
		}
		m_MouseSelectArea = {};
	}
	else
	{
		if (m_ShowItemMenu)
			m_MouseSelectArea = m_ItemMenuArea.HoverArea(p);
		else
		{
			float topInitArea = 0;
			if (m_ItemMenuArea.m_AdditionalOptions.size())
				topInitArea = m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom + 10.0f;
			if (p.y > topInitArea)
			{
				m_MouseSelectArea.left = m_MenuAreaRect.left;
				m_MouseSelectArea.top = (float)((int)((p.y - topInitArea) / 128.0f));
				m_MouseSelectArea.top *= 128.0f;
				m_MouseSelectArea.top += topInitArea;
				m_MouseSelectArea.right = m_MenuAreaRect.right;
				m_MouseSelectArea.bottom = m_MouseSelectArea.top + 128.0f;
			}
			else
			{
				m_MouseSelectArea = m_ItemMenuArea.HoverArea(p);
			}
		}
	}

	for (int i = 0; i < (int)m_Sprites.size(); i++)
	{
		if (m_Sprites[i])
			m_Sprites[i]->UpdateDrawInfo(timeTotal);
	}

	std::queue<ItemMenuItem> updatedList;
	while (!m_ItemMenuArea.m_Sprites.empty())
	{
		if (m_ItemMenuArea.m_Sprites.front().m_Sprite)
			m_ItemMenuArea.m_Sprites.front().UpdateSprite({ m_ItemMenuArea.m_Sprites.front().m_DrawArea.left, m_ItemMenuArea.m_Sprites.front().m_DrawArea.top }, m_ItemMenuArea.m_Sprites.front().m_Sprite->GetFrame(), { 0.0f, 0.0f, 96.0f, 96.0f });
		else
			m_ItemMenuArea.m_Sprites.front().UpdateSprite({ m_ItemMenuArea.m_Sprites.front().m_DrawArea.left, m_ItemMenuArea.m_Sprites.front().m_DrawArea.top }, m_ItemMenuArea.m_Sprites.front().m_DrawArea, { 0.0f, 0.0f, 96.0f, 96.0f });
		updatedList.push(m_ItemMenuArea.m_Sprites.front());
		m_ItemMenuArea.m_Sprites.pop();
	}
	m_ItemMenuArea.m_Sprites = updatedList;

	UpdateMapValues();
	if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Cone"))
		CalcConeHighlightArea();
	else if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
		CalcLineHighlightArea();

	if(m_SelectedItem)
		UpdateSelectArea();

	if (!((!m_RulerEndPoint.x) && (!m_RulerEndPoint.y)))
	{
		m_RulerDistance.clear();
		m_RulerDistance = std::to_wstring(CalcRulerDistance());
		m_RulerDistance.append(L" Ft.");
	}

	if (m_OnFire) m_OnFire->UpdateDrawInfo(timeTotal);

	//if (cg.m_Path) cg.BuildGeometry(m_Scale, m_StartPoint);
	if (m_UseOld)
	{
		for (int i = 0; i < m_CG.size(); i++)
		{
			m_CG[i].BuildGeometry(m_Scale, m_StartPoint);
		}
	}
}

void Level1::UpdateSelectArea()
{
	float sizeMod;
	if (m_SelectedItem->m_SizeCategory < Size::m_Medium)
	{
		sizeMod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_SelectedItem->m_SizeCategory)));
	}
	else if (m_SelectedItem->m_SizeCategory > Size::m_Medium)
	{
		sizeMod = (float)((int)m_SelectedItem->m_SizeCategory - (int)Size::m_Medium + 1);
	}
	else
		sizeMod = 1.0f;
	m_SelectArea = {};
	m_SelectArea.left = (m_SelectedItem->m_Position.x + m_StartPoint.x) * m_Scale;
	m_SelectArea.top = (m_SelectedItem->m_Position.y + m_StartPoint.y) * m_Scale;
	m_SelectArea.right = m_SelectArea.left + m_Size * m_Scale * sizeMod;
	m_SelectArea.bottom = m_SelectArea.top + m_Size * m_Scale * sizeMod;
}

void Level1::UpdateMapValues()
{	
	if (m_UseOld)
	{
		if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Cone"))
		{
			if (m_TempWallStartPoint.x || m_TempWallStartPoint.y)
			{
				D2D1_POINT_2F p = GameController::p;
				float slope = (p.y - m_TempWallStartPoint.y) / (p.x - m_TempWallStartPoint.y);
				m_ConeAngle = atan(slope);
			}
		}

		for (int i = 0; i < (int)m_Shapes.size(); i++)
		{
			m_Shapes[i].m_ColorArea.left = (m_Shapes[i].m_Position.x + m_StartPoint.x) * m_Scale;
			m_Shapes[i].m_ColorArea.top = (m_Shapes[i].m_Position.y + m_StartPoint.y) * m_Scale;
			m_Shapes[i].m_ColorArea.right = m_Shapes[i].m_ColorArea.left + (m_Shapes[i].m_Radius / 5) * m_Size * m_Scale;
			m_Shapes[i].m_ColorArea.bottom = m_Shapes[i].m_ColorArea.top + (m_Shapes[i].m_Radius / 5) * m_Size * m_Scale;

			if (m_Shapes[i].m_Shape != Shapes::FilledSphere)
			{
				m_Shapes[i].m_ConvertedRadius = (m_Shapes[i].m_Radius / 5.0f) * m_Size * m_Scale;
			}
			else
			{
				//			m_Shapes[i].m_ColorArea.left = m_Shapes[i].m_ColorArea.left + ((m_Shapes[i].m_ColorArea.right - m_Shapes[i].m_ColorArea.left) / 2.0f);
				//			m_Shapes[i].m_ColorArea.top = m_Shapes[i].m_ColorArea.top + ((m_Shapes[i].m_ColorArea.bottom - m_Shapes[i].m_ColorArea.top) / 2.0f);
				m_Shapes[i].m_ConvertedRadius = m_Shapes[i].m_Radius * m_Scale;
			}
		}
		for (int i = 0; i < (int)m_MapData.size(); i++)
		{
			float sizeMod;
			if (m_MapData[i].m_SizeCategory < Size::m_Medium)
			{
				sizeMod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_MapData[i].m_SizeCategory)));
			}
			else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
			{
				sizeMod = -999.0f;
			}
			else if (m_MapData[i].m_SizeCategory > Size::m_Medium)
			{
				sizeMod = (float)(((int)m_MapData[i].m_SizeCategory - (int)Size::m_Medium) + 1.0f);
			}
			else
				sizeMod = 1.0f;

			if (m_AssetList[m_MapData[i].m_MenuId].m_Sprite)
			{
				if (sizeMod < 0.0f)
				{
					m_MapData[i].src = m_AssetList[m_MapData[i].m_MenuId].m_Sprite->GetFrame();

					float destSizeX = abs(m_MapData[i].src.right - m_MapData[i].src.left);
					float destSizeY = abs(m_MapData[i].src.bottom - m_MapData[i].src.top);

					destSizeX *= m_Scale;
					destSizeY *= m_Scale;

					D2D1_RECT_F d2d1Rect = {};

					d2d1Rect.left = (m_MapData[i].m_Position.x + m_StartPoint.x) * m_Scale;
					d2d1Rect.top = (m_MapData[i].m_Position.y + m_StartPoint.y) * m_Scale;

					switch (m_MapData[i].m_SizeCategory)
					{
					case Size::m_Original:
						d2d1Rect.right = d2d1Rect.left + destSizeX;
						d2d1Rect.bottom = d2d1Rect.top + destSizeY;
						break;
					case Size::m_TwoTimes:
						d2d1Rect.right = d2d1Rect.left + destSizeX * 2.0f;
						d2d1Rect.bottom = d2d1Rect.top + destSizeY * 2.0f;
						break;
					case Size::m_ThreeTimes:
						d2d1Rect.right = d2d1Rect.left + destSizeX * 3.0f;
						d2d1Rect.bottom = d2d1Rect.top + destSizeY * 3.0f;
						break;
					case Size::m_FourTimes:
						d2d1Rect.right = d2d1Rect.left + destSizeX * 4.0f;
						d2d1Rect.bottom = d2d1Rect.top + destSizeY * 4.0f;
						break;
					case Size::m_Half:
						d2d1Rect.right = d2d1Rect.left + destSizeX / 2.0f;
						d2d1Rect.bottom = d2d1Rect.top + destSizeY / 2.0f;
						break;
					}
					m_MapData[i].m_ColorArea = m_MapData[i].dest = d2d1Rect;
				}
				else
				{

					m_MapData[i].src = m_AssetList[m_MapData[i].m_MenuId].m_Sprite->GetFrame();

					float xMod = 0.0f;
					float yMod = 0.0f;

					D2D1_RECT_F size = { 0.0f, 0.0f, m_Size * sizeMod * m_Scale, m_Size * sizeMod * m_Scale };

					if (m_MapData[i].m_KeepRatio)
					{
						if (m_MapData[i].src.bottom < m_MapData[i].src.top)
						{
							float t = m_MapData[i].src.bottom;
							m_MapData[i].src.bottom = m_MapData[i].src.top;
							m_MapData[i].src.top = t;
						}
						if (m_MapData[i].src.right < m_MapData[i].src.left)
						{
							float t = m_MapData[i].src.right;
							m_MapData[i].src.right = m_MapData[i].src.left;
							m_MapData[i].src.left = t;
						}

						if ((m_MapData[i].src.bottom - m_MapData[i].src.top) >(m_MapData[i].src.right - m_MapData[i].src.left))
						{
							float ratio = (m_MapData[i].src.right - m_MapData[i].src.left) / (m_MapData[i].src.bottom - m_MapData[i].src.top);
							if (ratio < 0.0f) ratio *= -1;
							xMod = (size.right - size.left) - ((size.right - size.left) * ratio);
							xMod /= 2;

							if (xMod < 0.0f) xMod *= -1;

							size.right = (size.right - size.left) * ratio;
							size.left = 0.0f;
						}
						else
						{
							float ratio = (m_MapData[i].src.bottom - m_MapData[i].src.top) / (m_MapData[i].src.right - m_MapData[i].src.left);
							if (ratio < 0.0f) ratio *= -1;
							yMod = (size.bottom - size.top) - ((size.bottom - size.top) * ratio);
							yMod /= 2.0f;
							if (m_GridType == Grid::m_Dimetrique) yMod /= 2.0f;

							if (yMod < 0.0f) yMod *= -1;

							size.bottom = (size.bottom - size.top) * ratio;
							size.top = 0.0f;
						}
					}

					m_MapData[i].dest = D2D1::RectF((m_MapData[i].m_Position.x + m_StartPoint.x) * m_Scale + xMod, (m_MapData[i].m_Position.y + m_StartPoint.y) * m_Scale + yMod, (m_MapData[i].m_Position.x + m_StartPoint.x) * m_Scale + size.right + xMod + 1.0f, (m_MapData[i].m_Position.y + m_StartPoint.y) * m_Scale + size.bottom + yMod + 1.0f);

					m_MapData[i].m_ColorArea.left = (m_MapData[i].m_Position.x + m_StartPoint.x) * m_Scale;
					m_MapData[i].m_ColorArea.right = m_MapData[i].m_ColorArea.left + m_Size * m_Scale * sizeMod;
					m_MapData[i].m_ColorArea.top = (m_MapData[i].m_Position.y + m_StartPoint.y) * m_Scale;
					m_MapData[i].m_ColorArea.bottom = m_MapData[i].m_ColorArea.top + m_Size * m_Scale * sizeMod;
				}
			}
			else
			{
				m_MapData[i].m_ColorArea.left = (m_MapData[i].m_Position.x + m_StartPoint.x) * m_Scale;
				m_MapData[i].m_ColorArea.right = m_MapData[i].m_ColorArea.left + m_Size * m_Scale * sizeMod;
				m_MapData[i].m_ColorArea.top = (m_MapData[i].m_Position.y + m_StartPoint.y) * m_Scale;
				m_MapData[i].m_ColorArea.bottom = m_MapData[i].m_ColorArea.top + m_Size * m_Scale * sizeMod;
			}
		}
	}

	for (int i = 0; i < m_Rooms.size(); i++)
	{
		m_Rooms[i].UpdateData(m_Size, m_Scale, m_StartPoint, m_AssetList);
	}	
}

bool Level1::InRect(D2D1_RECT_F area, D2D1_POINT_2F p)
{
	if (p.x < area.left) return false;
	if (p.x > area.right) return false;
	if (p.y > area.bottom) return false;
	if (p.y < area.top) return false;
	return true;
}

bool Level1::TouchingLine(FLINE line, D2D1_POINT_2F p)
{
	D2D1_POINT_2F a = line.startPoint;
	D2D1_POINT_2F b = line.endPoint;

	a.x *= m_Scale;
	a.y *= m_Scale;
	b.x *= m_Scale;
	b.y *= m_Scale;

	a.x += m_StartPoint.x * m_Scale;
	a.y += m_StartPoint.y * m_Scale;
	b.x += m_StartPoint.x * m_Scale;
	b.y += m_StartPoint.y * m_Scale;

	float epsilon = 3.0f;

	return (Distance(a, p) + Distance(p, b) - Distance(a, b) <= epsilon);
}

float Level1::Distance(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	float dx = abs(p2.x - p1.x);
	float dy = abs(p2.y - p1.y);

	dx *= dx;
	dy *= dy;

	return sqrt(dx + dy);
}

bool Level1::TouchingLine(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint, D2D1_POINT_2F p)
{
	return TouchingLine({ startPoint, endPoint }, p);
}

void Level1::DrawHexGrid(std::vector<HEX> hex)
{
	for (int i = 0; i < (int)hex.size(); i++)
	{
		gfx->BuildHexagon(hex[i]);
		gfx->DrawHexagon({ 0.0f, 0.0f, 0.0f, 1.0f });
	}
}

void Level1::EmptyHexVector(std::vector<HEX>& v)
{
	std::vector<HEX> empty;
	std::swap(v, empty);
}

void Level1::CalcHexGrid()
{
	float theta = 0.523598776f;
	float scaledSize = m_Size * m_Scale;
	EmptyHexVector(m_Hexes);
	for(float y = 0; y < m_DrawAreaRect.bottom; y += scaledSize * 1.5f)
	{
		for (float i = (-scaledSize / 2.0f) + 6.0f; i < m_DrawAreaRect.right + scaledSize; i += cos(theta) * scaledSize)
		{
			HEX p;			
			p.p0 = { i, y };
			p.p1 = { i + cos(theta) * scaledSize / 2.0f, y + sin(theta) * scaledSize / 2.0f };
			p.p2 = { p.p1.x, p.p1.y + scaledSize / 2.0f };
			p.p3 = { i, y + scaledSize };
			p.p4 = { i - cos(theta) * scaledSize / 2.0f, p.p3.y - sin(theta) * scaledSize / 2.0f };
			p.p5 = { p.p4.x, p.p4.y - scaledSize / 2.0f };
			m_Hexes.push_back(p);

			HEX p2;
			p2.p0 = p.p4;
			p2.p1 = p.p3;
			p2.p2 = { p2.p1.x, p2.p1.y + scaledSize / 2.0f };
			p2.p3 = { p2.p0.x, p2.p0.y + scaledSize };
			p2.p4 = { p2.p0.x - cos(theta) * scaledSize / 2.0f, p2.p3.y - sin(theta) * scaledSize / 2.0f };
			p2.p5 = { p2.p4.x, p2.p4.y - scaledSize / 2.0f };
			m_Hexes.push_back(p2);
		}
	}
}

void Level1::Render()
{
	//draw commands here
	gfx->ClearScreen(m_BgColor);

	if(m_ItemMenuArea.m_GridOnTop)
		DrawMap(m_MapData, m_Shapes, m_Walls);

	if (m_GridType == Grid::m_Square)
	{
		//gfx->DrawSquareGrid(m_DrawAreaRect);
		//gfx->DrawSquareGridPattern(m_DrawAreaRect);		this is what i should be using, but currently disabled to test something
		gfx->DrawGridBasic(m_DrawAreaRect, m_Scale * m_Size);
	}
	else if (m_GridType == Grid::m_Hexagon)
		gfx->DrawHexGrid(m_DrawAreaRect);
	else if (m_GridType == Grid::m_Dimetrique)
		//DrawGridLines(m_Lines);
		gfx->DrawDimetriqueGrid(m_DrawAreaRect);

	if (!m_ItemMenuArea.m_GridOnTop)
		DrawMap(m_MapData, m_Shapes, m_Walls);
	if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
	{
		DrawHighlightArea(m_PreviewPoint, { 0.1f, 0.8f, 0.0f, 1.0f }, 10.0f);
		if (m_TempWallStartPoint.x || m_TempWallStartPoint.y)
			DrawHighlightArea({ (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale, (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale }, { 1.0f, 0.0f, 0.0f, 1.0f }, 10.0f);
	}
	else if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Free Draw"))
	{
		if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Circular"))
		{
			DrawHighlightArea(m_PreviewPoint, { 1.0f, 0.0f, 0.0f, 1.0f }, m_ItemMenuArea.m_SelectedPenSize * m_Scale, 6.0f);
		}
	}
	else if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Sphere"))
	{
		DrawHighlightArea(m_MouseOverArea.p1, { 1.0f, 0.0f, 0.0f, 1.0f }, (m_ItemMenuArea.m_SelectedRadius / 5.0f) * m_Size * m_Scale, 6.0f);
	}
	else if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Cone"))
	{
		if (!m_TempWallStartPoint.x || !m_TempWallStartPoint.y)
		{
			DrawHighlightArea(m_PreviewPoint, { 0.1f, 0.8f, 0.0f, 1.0f }, 10.0f);
		}
		else
		{
			DrawHighlightArea({ (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale, (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale }, { 1.0f, 0.0f, 0.0f, 1.0f }, 10.0f);
			D2D1_COLOR_F t_BgColor = m_ItemMenuArea.m_SelectedItem->m_BgColor;
			t_BgColor.a = 0.3f;
			gfx->DrawCone(m_ItemMenuArea.m_SelectedItem->m_BgColor, t_BgColor);
		}
	}
	else if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
	{
		if (!m_TempWallStartPoint.x || !m_TempWallStartPoint.y)
		{
			DrawHighlightArea(m_PreviewPoint, { 0.1f, 0.8f, 0.0f, 1.0f }, 10.0f);
		}
		else
		{
			DrawHighlightArea({ (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale, (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale }, { 1.0f, 0.0f, 0.0f, 1.0f }, 10.0f);
			D2D1_COLOR_F t_BgColor = m_ItemMenuArea.m_SelectedItem->m_BgColor;
			t_BgColor.a = 0.3f;
			gfx->DrawLineArea(m_ItemMenuArea.m_SelectedItem->m_BgColor, t_BgColor);
		}
	}
	else if (m_GridType == Grid::m_Hexagon)
	{
		gfx->BuildHexagon(m_HexMouseOverArea);
		gfx->DrawHexagon({ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.3f });
	}
	else
	{
		DrawHighlightArea(m_MouseOverArea);
	}
	if (m_ShowItemMenu)
	{
		m_ItemMenuArea.DrawItemMenuArea();
		gfx->FillRect(m_MouseSelectArea, { 0.8f, 0.0f, 0.8f, 0.6f });
	}
	else
	{
		//m_InitArea.DrawInitArea({ m_MenuAreaRect.left, m_MenuAreaRect.top + 56.0f, m_MenuAreaRect.right, m_MenuAreaRect.bottom }, m_InitArea.m_InitOrder, gfx);
		float topInitArea = 0;
		if (m_ItemMenuArea.m_AdditionalOptions.size())
			topInitArea = m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom + 10.0f;
		m_InitArea.DrawInitArea({ m_MenuAreaRect.left, topInitArea, m_MenuAreaRect.right, m_MenuAreaRect.bottom }, m_InitArea.m_InitOrder, gfx);
		m_ItemMenuArea.DrawItemMenuArea(m_ItemMenuArea.m_AdditionalOptions);

		std::queue<ItemMenuItem> t = m_ItemMenuArea.m_AdditionalOptions;
		while (!t.empty())
		{
			if (m_ItemMenuArea.m_LockToGrid)
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Lock to Grid"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
			}
			if (m_ItemMenuArea.m_GridOnTop)
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Grid On Top"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
			}
			if (m_ShowBgColors)
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Toggle PC Colors"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
			}
			if (m_NextKeepAspect)
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Toggle Keep Aspect"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
			}
			if (m_ItemMenuArea.m_ShowCounter)
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Turn Counter"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
			}
			t.pop();
		}
		gfx->DrawLine({ m_MenuAreaRect.left, m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_ItemMenuArea.m_AdditionalOptions.back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		gfx->FillRect(m_MouseSelectArea, { 0.8f, 0.0f, 0.9f, 0.6f });
	}
	if (m_ItemMenuArea.m_ShowCounter)
	{
		m_Counter.m_Draw();
		gfx->BuildVariableText(m_Size, m_Scale);
	}
	if (!((!m_RulerEndPoint.x) && (!m_RulerEndPoint.y)))
	{
		gfx->DrawLine(m_RulerStartPoint, m_RulerEndPoint, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		gfx->DrawText(m_RulerDistance.c_str(), { 0.0f, WINDOW_HEIGHT / 2.0f, 500.0f, WINDOW_HEIGHT + 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	if (m_Geometry)
	{
		for (int i = 0; i < m_CurrentGeometry.size(); i++)
		{
			D2D1_POINT_2F newP = m_CurrentGeometry[i];
			newP.x *= m_Scale;
			newP.y *= m_Scale;
			gfx->DrawCircle(newP, 3.0f, { 0.75f, 0.10f, 0.25f, 0.80f }, 1.0f);
			if (i < m_CurrentGeometry.size() - 1)
			{
				D2D1_POINT_2F newP2 = m_CurrentGeometry[i + 1];
				newP2.x *= m_Scale;
				newP2.y *= m_Scale;
				gfx->DrawLine(newP, newP2);
			}
		}
	}

	for (int i = 0; i < m_CG.size(); i++)
	{
		if (m_CG[i].m_Path)
		{

		}
	}
	/*
	if (!cg.m_Points.empty())
	{
		std::vector<D2D1_POINT_2F> tPps = cg.GetUpdatedPoints(m_StartPoint, m_Scale);
		for (int i = 0; i < tPps.size(); i++)
		{
			gfx->DrawCircle(tPps[i], 3.0f, { 0.75f, 0.10f, 0.25f, 0.80f }, 1.0f);
			if (i < tPps.size() - 1)
			{
				gfx->DrawLine(tPps[i], tPps[i + 1]);
			}
		}
	}*/
}

void Level1::CalcLineHighlightArea()
{
	FLINE line[5];

	line[0].startPoint = { (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale, (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale };

	float calcWidth = (m_ItemMenuArea.m_SelectedWidth / 5.0f) * m_Scale * m_Size;
	float halfWidth = calcWidth / 2.0f;
	float calcLen = (m_ItemMenuArea.m_SelectedRadius / 5.0f) * m_Scale * m_Size;
	float slope = (GameController::p.y - line[0].startPoint.y) / (GameController::p.x - line[0].startPoint.x);
	float theta = atan(slope);
	float thetaPi = theta - PI / 2.0f;
	
	if (GameController::p.x < line[0].startPoint.x)
	{
		line[0].endPoint.x = line[0].startPoint.x - (-1) * cos(thetaPi) * halfWidth;
		line[0].endPoint.y = line[0].startPoint.y - sin(-thetaPi) * halfWidth;
		line[1].startPoint = line[0].endPoint;
		line[1].endPoint.x = line[1].startPoint.x + (-1) * cos(theta) * calcLen;
		line[1].endPoint.y = line[1].startPoint.y + sin(-theta) * calcLen;
		line[2].startPoint = line[0].startPoint;
		line[2].endPoint.x = line[2].startPoint.x + (-1) * cos(thetaPi) * halfWidth;
		line[2].endPoint.y = line[2].startPoint.y + sin(-thetaPi) * halfWidth;
		line[3].startPoint = line[2].endPoint;
		line[3].endPoint.x = line[3].startPoint.x + (-1) * cos(theta) * calcLen;
		line[3].endPoint.y = line[3].startPoint.y + sin(-theta) * calcLen;
		line[4].startPoint = line[1].endPoint;
		line[4].endPoint = line[3].endPoint;
	}
	else
	{
		line[0].endPoint.x = line[0].startPoint.x - cos(thetaPi) * halfWidth;
		line[0].endPoint.y = line[0].startPoint.y - sin(thetaPi) * halfWidth;
		line[1].startPoint = line[0].endPoint;
		line[1].endPoint.x = line[1].startPoint.x + cos(theta) * calcLen;
		line[1].endPoint.y = line[1].startPoint.y + sin(theta) * calcLen;
		line[2].startPoint = line[0].startPoint;
		line[2].endPoint.x = line[2].startPoint.x + cos(thetaPi) * halfWidth;
		line[2].endPoint.y = line[2].startPoint.y + sin(thetaPi) * halfWidth;
		line[3].startPoint = line[2].endPoint;
		line[3].endPoint.x = line[3].startPoint.x + cos(theta) * calcLen;
		line[3].endPoint.y = line[3].startPoint.y + sin(theta) * calcLen;
		line[4].startPoint = line[1].endPoint;
		line[4].endPoint = line[3].endPoint;
	}

	gfx->BuildLineshape(line);
}

void Level1::CalcConeHighlightArea()
{
	D2D1_POINT_2F p2;
	FLINE line1 = {};
	FLINE line2 = {};
	line1.startPoint = { (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale, (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale };

	float calcRadius = ((m_ItemMenuArea.m_SelectedRadius / 5.0f) * m_Scale * m_Size);
	float slope = (GameController::p.y - line1.startPoint.y) / (GameController::p.x - line1.startPoint.x);
	float theta = atan(slope);


/*	if (GameController::p.x * m_Scale < m_TempWallStartPoint.x)		//this was for caclulating the center line, decided to remove it to decease number of calculations
		p2.y = sin(-theta) * calcRadius;
	else
		p2.y = sin(theta) * calcRadius;
	p2.y += (m_TempWallStartPoint.y + m_StartPoint.y) * m_Scale;
	if (GameController::p.x * m_Scale < m_TempWallStartPoint.x)
		p2.x = -cos(theta) * calcRadius;
	else
		p2.x = cos(theta) * calcRadius;
	p2.x += (m_TempWallStartPoint.x + m_StartPoint.x) * m_Scale;*/
	
	D2D1_POINT_2F curPos = {};
	curPos.x = GameController::p.x;
	curPos.y = GameController::p.y;

	float t = theta;
	theta -= (float)(atan(.5));

	if (curPos.x < line1.startPoint.x)
		p2.y = sin(-theta) * calcRadius;
	else
		p2.y = sin(theta) * calcRadius;
	p2.y += line1.startPoint.y;
	if (curPos.x < line1.startPoint.x)
		p2.x = -cos(theta) * calcRadius;
	else
		p2.x = cos(theta) * calcRadius;
	p2.x += line1.startPoint.x;

	line1.endPoint = p2;
	
	theta = (float)(t + atan(.5));
	if (curPos.x < line1.startPoint.x)
		p2.y = sin(-theta) * calcRadius;
	else
		p2.y = sin(theta) * calcRadius;
	p2.y += line1.startPoint.y;
	if (curPos.x < line1.startPoint.x)
		p2.x = -cos(theta) * calcRadius;
	else
		p2.x = cos(theta) * calcRadius;
	p2.x += line1.startPoint.x;

	line2.startPoint = p2;
	line2.endPoint = line1.startPoint;

	gfx->BuildConeShape(line1, line2, { calcRadius, calcRadius }, 0.0f);
}

std::queue<FLINE> Level1::CalculateGridPointsDimetrique(D2D1_RECT_F area, float xSize, float scale)
{
	std::queue<FLINE> points;
	xSize *= scale;

	int numberTilesYAxis = (int)((area.bottom - area.top) / (xSize / 2.0f));
	for (float y = area.top - (numberTilesYAxis * (xSize / 2.0f)); y <= area.bottom * 2.0f; y += (xSize / 2.0f))
	{
		FLINE line;
		line.startPoint.x = area.left;
		line.startPoint.y = y;
		line.endPoint.x = line.startPoint.x + (float)(tan((PI / 180.0f) * 63.434) * (area.bottom - y));
		line.endPoint.y = area.bottom;
		points.push(line);
		line.endPoint.x = line.startPoint.x + (float)(tan((PI / 180.0f) * -63.434) * (area.top - y));
		line.endPoint.y = area.top;
		points.push(line);
	}

	return points;
}

std::queue<FLINE> Level1::CalculateGridPoints(D2D1_RECT_F area, float xSize, float scale)
{
	std::queue<FLINE> points;
	xSize *= scale;

	float startMod = (float)((int)(m_StartPoint.x / xSize));
	startMod *= xSize;
	startMod = m_StartPoint.x * scale - startMod;
	
	for (float x = area.left + startMod; x <= area.right; x += xSize)
	{
		FLINE line;
		line.startPoint.x = x;
		line.endPoint.x = x;
		line.startPoint.y = area.top;
		line.endPoint.y = area.bottom;
		points.push(line);
	}

	startMod = (float)((int)(m_StartPoint.y / xSize));
	startMod *= xSize;
	startMod = m_StartPoint.y * scale - startMod;
	for (float y = area.top + startMod; y <= area.bottom; y += xSize)
	{
		FLINE line;
		line.startPoint.x = area.left;
		line.startPoint.y = y;
		line.endPoint.x = area.right;
		line.endPoint.y = y;
		points.push(line);
	}
	return points;
}

void Level1::DrawGridLines(std::queue<FLINE> lines)
{
	while (!lines.empty())
	{
		FLINE line = lines.front();
		lines.pop();
		gfx->DrawLine(line.startPoint, line.endPoint);
	}
}

void Level1::EmptyLineQueue(std::queue<FLINE>& lines)
{
	std::queue<FLINE> empty;
	std::swap(lines, empty);
}

void Level1::DrawHighlightArea(D2D1_POINT_2F p, D2D1_COLOR_F color, float radius, float thickness)
{
	if (m_ShowItemMenu)
		gfx->DrawCircle(p, radius, color, thickness);
}

void Level1::DrawHighlightArea(FRHOMBUS area)
{
	if (!m_ShowItemMenu) return;
	COLORREF_F color = { 0.75f, 0.0f, 0.1f, 1.0f };
	gfx->DrawLine(area.p1, area.p2, color, 3.5f);
	gfx->DrawLine(area.p2, area.p3, color, 3.5f);
	gfx->DrawLine(area.p3, area.p4, color, 3.5f);
	gfx->DrawLine(area.p4, area.p1, color, 3.5f);
}

void Level1::DrawHighlightArea(FRHOMBUS area, D2D1_COLOR_F color)
{
	if (!m_ShowItemMenu) return;
	gfx->DrawLine(area.p1, area.p2, color, 3.5f);
	gfx->DrawLine(area.p2, area.p3, color, 3.5f);
	gfx->DrawLine(area.p3, area.p4, color, 3.5f);
	gfx->DrawLine(area.p4, area.p1, color, 3.5f);
}

bool Level1::InTriangle(D2D1_POINT_2F p, FTRIANGLE area)
{
	float s, t, A;
	s = area.p1.y * area.p3.x - area.p1.x * area.p3.y + (area.p3.y - area.p1.y) * p.x + (area.p1.x - area.p3.x) * p.y;
	t = area.p1.x * area.p2.y - area.p1.y * area.p2.x + (area.p1.y - area.p2.y) * p.x + (area.p2.x - area.p1.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;
	A = -area.p2.y * area.p3.x + area.p1.y * (area.p3.x - area.p2.x) + area.p1.x * (area.p2.y - area.p3.y) + area.p2.x * area.p3.y;
	if (A < 0.0f)
	{
		s = -s;
		t = -t;
		A = -A;
	}
	return s > 0 && t > 0 && (s + t) <= A;
}

Part Level1::SquarePart(D2D1_POINT_2F p, FRECT rect)
{
	FTRIANGLE area({ rect.left, rect.top }, { rect.left + (rect.right - rect.left) / 2.0f, rect.top + (rect.bottom - rect.top) / 2.0f }, { rect.left, rect.bottom });

	if (InTriangle(p, area)) return LEFT;
	area.p3 = { rect.right, rect.top };
	if (InTriangle(p, area)) return TOP;
	area.p1 = area.p3;
	area.p3 = { rect.right, rect.bottom };
	if (InTriangle(p, area)) return RIGHT;
	return BOTTOM;
}

FRHOMBUS Level1::CalculateMouseOverArea(float xSize, int numberTilesX, int numberTilesY, D2D1_POINT_2F p)
{
	FRHOMBUS area;

	if (m_GridType == Grid::m_Dimetrique)
	{
		FRECT tArea;
		tArea.left = (float)((int)(p.x / xSize));
		tArea.left *= xSize;
		tArea.top = (float)((int)(p.y / (xSize / 2.0f)));
		tArea.top *= xSize / 2.0f;
		tArea.right = tArea.left + xSize;
		tArea.bottom = tArea.top + xSize / 2.0f;

		switch (SquarePart(p, tArea))
		{
		case LEFT:
			area.p1.x = tArea.left - xSize / 2.0f;
			area.p1.y = tArea.top + (tArea.bottom - tArea.top) / 2.0f;
			area.p2.x = tArea.left;
			area.p2.y = tArea.top;
			area.p3.x = area.p2.x + xSize / 2.0f;
			area.p3.y = area.p1.y;
			area.p4.x = tArea.left;
			area.p4.y = area.p2.y + xSize / 2.0f;
			return area;
		case TOP:
			area.p1.x = tArea.left;
			area.p1.y = tArea.top;
			area.p2.x = tArea.left + xSize / 2.0f;
			area.p2.y = tArea.top - xSize / 4.0f;
			area.p3.x = tArea.right;
			area.p3.y = tArea.top;
			area.p4.x = area.p2.x;
			area.p4.y = area.p2.y + xSize / 2.0f;
			return area;
		case RIGHT:
			area.p1.x = tArea.left + xSize / 2.0f;
			area.p1.y = tArea.top + xSize / 4.0f;
			area.p2.x = tArea.right;
			area.p2.y = tArea.top;
			area.p3.x = area.p1.x + xSize;
			area.p3.y = area.p1.y;
			area.p4.x = area.p2.x;
			area.p4.y = tArea.bottom;
			return area;
		}

		area.p1.x = (float)((int)(p.x / xSize));
		area.p1.y = (float)((int)(p.y / (xSize / 2.0f)));
		area.p1.y++;
		area.p1.y *= xSize / 2.0f;
		area.p1.x *= xSize;
		area.p2.x = area.p1.x + (xSize / 2.0f);
		area.p2.y = area.p1.y - (xSize / 4.0f);
		area.p3.x = area.p1.x + xSize;
		area.p3.y = area.p1.y;
		area.p4.x = area.p2.x;
		area.p4.y = area.p2.y + (xSize / 2.0f);
	}
	else if (m_GridType == Grid::m_Square)
	{
		if (m_SelectedItem)
		{
			if (m_ItemMenuArea.m_LockToGrid)
			{
				area.p1.x = (float)((int)(p.x / xSize));
				area.p1.x *= xSize;
				area.p1.y = (float)((int)(p.y / xSize));
				area.p1.y *= xSize;
				if ((int)m_SelectedItem->m_SizeCategory == (int)Size::m_Medium)
				{
					area.p2.x = area.p1.x + xSize;
					area.p3.x = area.p2.x;
					area.p4.x = area.p1.x;

					area.p2.y = area.p1.y;
					area.p3.y = area.p1.y + xSize;
					area.p4.y = area.p3.y;
				}
				else if ((int)m_SelectedItem->m_SizeCategory < (int)Size::m_Medium)
				{
					float sizeMod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_SelectedItem->m_SizeCategory)));
					sizeMod *= xSize;
					area.p1.x = (float)((int)(p.x / sizeMod));
					area.p1.y = (float)((int)(p.y / sizeMod));
					area.p1.x *= sizeMod;
					area.p1.y *= sizeMod;

					area.p2.x = area.p1.x + sizeMod;
					area.p2.y = area.p1.y;
					area.p3.x = area.p2.x;
					area.p3.y = area.p2.y + sizeMod;
					area.p4.x = area.p1.x;
					area.p4.y = area.p3.y;
				}
				else if ((int)m_ItemMenuArea.m_SelectedSize > (int)Size::m_Colossal)
				{
					area.p2.x = area.p1.x + 5.0f;
					area.p2.y = area.p1.y;
					area.p3.x = area.p2.x;
					area.p3.y = area.p1.y + 5.0f;
					area.p4.x = area.p1.x;
					area.p4.y = area.p3.y;
				}
				else if ((int)m_ItemMenuArea.m_SelectedSize > (int)Size::m_Medium)
				{
					float sizemod = (float)((int)m_SelectedItem->m_SizeCategory - (int)Size::m_Medium + 1);
					sizemod = sizemod * xSize;
					area.p2.x = area.p1.x + sizemod;
					area.p3.x = area.p2.x;
					area.p4.x = area.p1.x;

					area.p2.y = area.p1.y;
					area.p3.y = area.p1.y + sizemod;
					area.p4.y = area.p3.y;
				}
			}
		}
		else if (m_ItemMenuArea.m_SelectedItem)
		{
			if (m_ItemMenuArea.m_LockToGrid)
			{
				if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
					{
						area.p1.x = (float)((int)(p.x / xSize) + (xSize / 2.0f));
						area.p1.x -= (m_ItemMenuArea.m_SelectedWidth / 5.0f) / 2.0f;
						area.p1.y = (float)((int)(p.y / xSize));
					}
					else
					{
						area.p1.x = (float)((int)(p.x / xSize));
						area.p1.x *= xSize;
						area.p1.y = (float)((int)(p.y / xSize));
						area.p1.y *= xSize;
						area.p2.x = area.p1.x + (m_ItemMenuArea.m_SelectedRadius / 5.0f) * xSize;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p1.y + (m_ItemMenuArea.m_SelectedRadius / 5.0f) * xSize;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
				}
				else
				{
					area.p1.x = (float)((int)(p.x / xSize));
					area.p1.x *= xSize;
					area.p1.y = (float)((int)(p.y / xSize));
					area.p1.y *= xSize;
					if ((int)m_ItemMenuArea.m_SelectedSize == (int)Size::m_Medium)
					{
						area.p2.x = area.p1.x + xSize;
						area.p3.x = area.p2.x;
						area.p4.x = area.p1.x;

						area.p2.y = area.p1.y;
						area.p3.y = area.p1.y + xSize;
						area.p4.y = area.p3.y;
					}
					else if ((int)m_ItemMenuArea.m_SelectedSize < (int)Size::m_Medium)
					{
						float sizeMod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_ItemMenuArea.m_SelectedSize)));
						sizeMod *= xSize;
						area.p1.x = (float)((int)(p.x / sizeMod));
						area.p1.y = (float)((int)(p.y / sizeMod));
						area.p1.x *= sizeMod;
						area.p1.y *= sizeMod;
						//area.p1.x += (xSize - sizeMod) / 2.0f;
						//area.p1.y += (xSize - sizeMod) / 2.0f;
						area.p2.x = area.p1.x + sizeMod;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p2.y + sizeMod;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
					else if ((int)m_ItemMenuArea.m_SelectedSize > (int)Size::m_Colossal)
					{
						area.p2.x = area.p1.x + 5.0f;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p1.y + 5.0f;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
					else if ((int)m_ItemMenuArea.m_SelectedSize > (int)Size::m_Medium)
					{
						float sizemod = (float)((int)m_ItemMenuArea.m_SelectedSize - (int)Size::m_Medium + 1);
						sizemod = sizemod * xSize;
						area.p2.x = area.p1.x + sizemod;
						area.p3.x = area.p2.x;
						area.p4.x = area.p1.x;

						area.p2.y = area.p1.y;
						area.p3.y = area.p1.y + sizemod;
						area.p4.y = area.p3.y;
					}
				}
			}
			else
			{
				if (!_stricmp(m_ItemMenuArea.m_SelectedType->m_Type.c_str(), "Shapes"))
				{
					if (!_stricmp(m_ItemMenuArea.m_SelectedSubMenu->m_Submenu.c_str(), "Sphere"))
					{
						area = {};
						area.p1 = p;
					}
					else
					{
						float sizemod = m_ItemMenuArea.m_SelectedRadius / 5.0f;
						sizemod *= m_Size * m_Scale;
						area.p1 = p;
						area.p1.x -= sizemod / 2.0f;
						area.p1.y -= sizemod / 2.0f;
						area.p2.x = area.p1.x + sizemod;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p1.y + sizemod;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
				}
				else
				{
					if ((int)m_ItemMenuArea.m_SelectedSize > (int)Size::m_Colossal)
					{
						area.p1 = p;
						area.p2.x = area.p1.x + 5.0f;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p1.y + 5.0f;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
					else
					{
						float sizemod = (float)((int)m_ItemMenuArea.m_SelectedSize - (int)Size::m_Medium + 1);
						if (sizemod > 1) sizemod *= xSize;
						else if (m_ItemMenuArea.m_SelectedSize == Size::m_Medium) sizemod = xSize;
						else sizemod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_ItemMenuArea.m_SelectedSize))) * xSize;
						area.p1 = p;
						area.p1.x -= sizemod / 2.0f;
						area.p1.y -= sizemod / 2.0f;
						area.p2.x = area.p1.x + sizemod;
						area.p2.y = area.p1.y;
						area.p3.x = area.p2.x;
						area.p3.y = area.p1.y + sizemod;
						area.p4.x = area.p1.x;
						area.p4.y = area.p3.y;
					}
				}
			}
		}
	}
	return area;
}

bool Level1::InHex(HEX hex, D2D1_POINT_2F p)
{
	float sinTheta = 0.5f;
	float scaledSize = m_Size * m_Scale;
	D2D1_RECT_F centerBox = { hex.p5.x, hex.p5.y, hex.p2.x, hex.p2.y };
	if (InRect(centerBox, p)) return true;
	if (p.y < hex.p0.y) return false;
	if (p.y > hex.p3.y) return false;
	if (p.x < hex.p4.x) return false;
	if (p.x > hex.p1.x) return false;
	float distX = abs(p.x - hex.p0.x);
	if (p.y > centerBox.bottom)
	{
		if ((hex.p3.y - sinTheta * distX) < p.y) return false;
		return true;
	}
	if (p.y < centerBox.top)
	{
		if ((hex.p0.y + sinTheta * distX) > p.y) return false;
		return true;
	}
	
	return false;
}

HEX Level1::CalculateMouseOverArea(D2D1_POINT_2F p)
{
	HEX area = {};
	for (int i = 0; i < (int)m_Hexes.size(); i++)
	{
		if (InHex(m_Hexes[i], p))
		{
			area = m_Hexes[i];
			break;
		}
	}
	return area;
}

void Level1::IncreaseSize()
{
	m_Scale += m_StepSize;
	m_RecalcLines = true;
	gfx->BuildVariableText(m_Size, m_Scale);
}

void Level1::DecreaseSize()
{
	m_Scale -= m_StepSize;
	m_RecalcLines = true;
	if (m_Scale < m_StepSize) m_Scale = m_StepSize;
	gfx->BuildVariableText(m_Size, m_Scale);
}

void Level1::ChangeMode()
{
	if (m_GridType == Grid::m_Square)
		m_GridType = Grid::m_Dimetrique;
	else if (m_GridType == Grid::m_Dimetrique)
		m_GridType = Grid::m_Hexagon;
	else
		m_GridType = Grid::m_Square;
	m_RecalcLines = true;
}

void Level1::BuildMenu()
{
	//m_Menu = GetMenu(GameController::hWnd);

	HMENU subMenu = CreatePopupMenu();
	AppendMenu(subMenu, MF_STRING, MENU_NEW, "New");
	AppendMenu(subMenu, MF_STRING, MENU_OPEN, "Open");
	AppendMenu(subMenu, MF_STRING, MENU_SAVE, "Save");
	AppendMenu(subMenu, MF_STRING, MENU_QUIT, "Quit");

	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)subMenu, "File");

	CreateRoomMenu();
	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)m_RoomMenu, "Rooms");

	CreateShowRoomMenu();
	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)m_ShowRoomMenu, "Show/Hide Rooms");

	CreateLayerMenu();
	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)m_LayerMenu, "Layers");

	CreateShowLayerMenu();
	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)m_ShowLayerMenu, "Show/Hide Layers");

	subMenu = CreatePopupMenu();
	AppendMenu(subMenu, MF_STRING, MENU_CHANGEMODE, "Change View Mode");
	AppendMenu(subMenu, MF_STRING, MENU_TOGGLECOLO, "Toggle Item Color");
	AppendMenu(subMenu, MF_STRING, MENU_TOGGLEINIT, "Toggle Initiative");
	AppendMenu(subMenu, MF_STRING | MF_CHECKED, MENU_LOCKTOGRID, "Lock To Grid");
	//AppendMenu(subMenu, MF_STRING, MENU_GRIDONTOP, "Toggle Grid On Top");
	//AppendMenu(subMenu, MF_STRING, MENU_KEEPASPECT, "Toggle Keep Aspect");
	AppendMenu(subMenu, MF_STRING, MENU_EDITBUTTONMAPPING, "Configure Controls");
	AppendMenu(subMenu, MF_STRING, MENU_GEOMETRY, "Edit Geometry");					//this is for testing purposes only
	AppendMenu(m_Menu, MF_POPUP | MF_STRING, (UINT)subMenu, "Advanced");

	SetMenu(GameController::hWnd, m_Menu);
}

void Level1::CreateRoomMenu()
{
	m_RoomMenu = CreatePopupMenu();

	AppendMenu(m_RoomMenu, MF_STRING | MF_CHECKED, MENU_ROOM0, "Room 0");
	AppendMenu(m_RoomMenu, MF_STRING, MENU_ADDROOM, "Add Room");
}

void Level1::AddRoomMenu()
{
	if (!m_RoomMenu) return;
	std::string text = "Room " + std::to_string(m_Rooms.size());
	ModifyMenu(m_RoomMenu, m_Rooms.size(), MF_BYPOSITION | MF_STRING, MENU_ROOM0 + m_Rooms.size(), text.c_str());
	AppendMenu(m_RoomMenu, MF_STRING, MENU_ADDROOM, "Add Room");

	if (m_ShowRoomMenu)
		AppendMenu(m_ShowRoomMenu, MF_STRING | MF_CHECKED, MENU_SHOWROOM0 + m_Rooms.size(), text.c_str());
	Room newRoom;
	m_Rooms.push_back(newRoom);
}

void Level1::CreateShowRoomMenu()
{
	m_ShowRoomMenu = CreatePopupMenu();

	AppendMenu(m_ShowRoomMenu, MF_STRING | MF_CHECKED, MENU_SHOWROOM0, "Room 0");
}

void Level1::EmptyLayerMenu()
{
	for (int i = GetMenuItemCount(m_LayerMenu) - 1; i >= 0; i--)
	{
		RemoveMenu(m_LayerMenu, i, MF_BYPOSITION);
	}
}

void Level1::EmptyRoomMenu()
{
	for (int i = GetMenuItemCount(m_RoomMenu) - 1; i >= 0; i--)
	{
		RemoveMenu(m_RoomMenu, i, MF_BYPOSITION);
	}
}

void Level1::EmptyShowRoomMenu()
{
	for (int i = GetMenuItemCount(m_ShowRoomMenu) - 1; i >= 0; i--)
	{
		RemoveMenu(m_ShowRoomMenu, i, MF_BYPOSITION);
	}
}

void Level1::EmptyShowLayerMenu()
{
	for (int i = GetMenuItemCount(m_ShowLayerMenu) - 1; i >= 0; i--)
	{
		RemoveMenu(m_ShowLayerMenu, i, MF_BYPOSITION);
	}
}

void Level1::RebuildShowLayerMenu()
{
	EmptyShowLayerMenu();
	for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer.size(); i++)
	{
		std::string text = "Layer " + std::to_string(i);
		AppendMenu(m_ShowLayerMenu, MF_STRING, MENU_SHOWLAYER0 + i, text.c_str());

		if (m_Rooms[m_CurrentRoom].m_Layer[i].m_Visible)
		{
			CheckMenuItem(m_ShowLayerMenu, MENU_SHOWLAYER0 + i, MF_CHECKED);
		}
		else
		{
			CheckMenuItem(m_ShowLayerMenu, MENU_SHOWLAYER0 + i, MF_UNCHECKED);
		}
	}	
}

void Level1::RebuildLayerMenu()
{
	EmptyLayerMenu();
	for (int i = 0; i < m_Rooms[m_CurrentRoom].m_Layer.size(); i++)
	{
		std::string text = "Layer " + std::to_string(i);
		AppendMenu(m_LayerMenu, MF_STRING, MENU_LAYER0 + i, text.c_str());
	}
	AppendMenu(m_LayerMenu, MF_STRING, MENU_ADDLAYER, "Add Layer");
	CheckMenuItem(m_LayerMenu, MENU_LAYER0 + m_CurrentLayer, MF_CHECKED);
	RebuildShowLayerMenu();
}

void Level1::RebuildShowRoomMenu()
{
	EmptyShowRoomMenu();
	for (int i = 0; i < m_Rooms.size(); i++)
	{
		std::string text = "Room " + std::to_string(i);
		AppendMenu(m_ShowRoomMenu, MF_STRING, MENU_SHOWROOM0 + i, text.c_str());
		if (m_Rooms[i].m_Visible)
		{
			CheckMenuItem(m_ShowRoomMenu, MENU_SHOWROOM0 + i, MF_CHECKED);
		}
		else
		{
			CheckMenuItem(m_ShowRoomMenu, MENU_SHOWROOM0 + i, MF_UNCHECKED);
		}
	}
}

void Level1::RebuildRoomMenu()
{
	EmptyRoomMenu();
	for (int i = 0; i < m_Rooms.size(); i++)
	{
		std::string text = "Room " + std::to_string(i);
		AppendMenu(m_RoomMenu, MF_STRING, MENU_ROOM0 + i, text.c_str());
	}
	AppendMenu(m_RoomMenu, MF_STRING, MENU_ADDROOM, "Add Room");
	CheckMenuItem(m_RoomMenu, MENU_ROOM0 + m_CurrentRoom, MF_CHECKED);
	RebuildShowRoomMenu();
}

void Level1::CreateLayerMenu()
{
	m_LayerMenu = CreatePopupMenu();

	AppendMenu(m_LayerMenu, MF_STRING | MF_CHECKED, MENU_LAYER0, "Layer 0");
	AppendMenu(m_LayerMenu, MF_STRING, MENU_ADDLAYER, "Add Layer");
}

void Level1::CreateShowLayerMenu()
{
	m_ShowLayerMenu = CreatePopupMenu();

	AppendMenu(m_ShowLayerMenu, MF_STRING | MF_CHECKED, MENU_SHOWLAYER0, "Layer 0");
}

void Level1::AddLayerMenu()
{
	if (!m_LayerMenu) return;

	Layer newLayer;
	m_Rooms[m_CurrentRoom].m_Layer.push_back(newLayer);

	RebuildLayerMenu();
}

int Level1::GetLine(const char* buffer, const int bufferLen, char*& target)
{
	if (target) delete[] target;
	target = NULL;
	int lineLen = 0;

	for (int i = 0; i <= bufferLen; i++)
	{
		if (i == bufferLen || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == ';' || buffer[i] == 'þ')
		{
			lineLen = i;
			break;
		}
	}

	target = new char[lineLen + 1];
	target[lineLen] = '\0';
	memcpy(target, buffer, lineLen);

	return lineLen + 1;
}

int Level1::GetLine(const char* buffer, char*& target)
{
	if (target) delete[] target;
	target = NULL;
	int lineLen = 0;
	
	for (int i = 0; i <= (int)strlen(buffer); i++)
	{
		if (i == strlen(buffer) || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == ';' || buffer[i] == 'þ')
		{
			lineLen = i;
			break;
		}
	}

	target = new char[lineLen + 1];
	target[lineLen] = '\0';
	memcpy(target, buffer, lineLen);

	return lineLen + 1;
}

void Level1::BuildAssetListFromBuffer(const char* buffer, const size_t bufferLen, std::vector<Asset>& assetList)
{
	size_t position = 0;
	char* line = NULL;

	while (position < bufferLen)
	{
		position += GetLine(buffer + position, bufferLen - position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		TranslateLine(line, t, assetList);

		if (t) delete[] t;

		delete[] line;
		line = NULL;
	}
}

bool Level1::BuildAssetList(const wchar_t* initFile, std::vector<Asset>& assetList)
{
	FILE* file;
	errno_t error = _wfopen_s(&file, initFile, L"rb");
	if (error)
	{
		std::wstring errmsg = L"Unable to open ";
		errmsg.append(initFile);
		errmsg.append(L". Please make sure ");
		errmsg.append(initFile);
		errmsg.append(L" is in the same folder as the executable.");
		MessageBoxW(NULL, errmsg.c_str(), L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	fseek(file, 0, SEEK_END);
	int bufferLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[bufferLen+1];
	buffer[bufferLen] = '\r';
	fread(buffer, bufferLen, 1, file);
	fclose(file);

	int position = 0;
	char* line = NULL;

	while (position < bufferLen)
	{
		position += GetLine(buffer + position, bufferLen - position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		TranslateLine(line, t, assetList);

		if (t) delete[] t;

		delete[] line;
		line = NULL;
	}

	delete[] buffer;
	return true;
}

bool Level1::UpdateAssetList(const char* filepath, std::vector<Asset>& assetList)
{
	FILE* file;
	errno_t error = fopen_s(&file, filepath, "rb");
	if (error)
	{
		std::string msg = "Unable to open ";
		msg.append(filepath);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK);
		return false;
	}

	fseek(file, 0, SEEK_END);
	int bufferlen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[bufferlen];
	fread(buffer, bufferlen, 1, file);
	fclose(file);

	int position = 0;
	char* line = NULL;

	Asset assetHold;
	int cmpPos = -1;

	while (position < bufferlen)
	{
		position += GetLine(buffer + position, bufferlen - position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		if (!_stricmp(line, "start"))
		{
			if (assetHold.m_Type.length())
			{
				if (cmpPos == -1)
				{
					assetHold.m_MenuID = (long)(assetList.size());
					assetList.push_back(assetHold);
					m_RebuildInitDat = true;
				}
			}
			cmpPos = -1;
			assetHold = {};
			assetHold.m_Type = t;
		}

		if (!_stricmp(line, "Name"))
		{
			assetHold.m_Name = t;
			for (int i = 0; i < (int)assetList.size(); i++)
			{
				if (!_stricmp(t, assetList[i].m_Name.c_str()))
				{
					cmpPos = i;
					break;
				}
			}
		}

		if (!_stricmp(line, "icon"))
		{
			assetHold.m_IconPath = t;
			if (cmpPos >= 0)
			{
				if (_stricmp(t, assetList[cmpPos].m_IconPath.c_str()))
				{
					assetList[cmpPos].m_IconPath = assetHold.m_IconPath;
				}
			}
		}

		if (!_stricmp(line, "sprite"))
		{
			assetHold.m_SpritePath = t;
			if (cmpPos >= 0)
			{
				if (_stricmp(t, assetList[cmpPos].m_SpritePath.c_str()))
				{
					assetList[cmpPos].m_SpritePath = assetHold.m_SpritePath;
				}
			}
		}

		if (!_stricmp(line, "submenu"))
		{
			assetHold.m_Submenu = t;
			if (cmpPos >= 0)
			{
				if (_stricmp(t, assetList[cmpPos].m_Submenu.c_str()))
				{
					assetList[cmpPos].m_Submenu = assetHold.m_Submenu;
				}
			}
		}

		if (!_stricmp(line, "defaultinitorder"))
		{
			assetHold.m_DefaultInitOrder = (_stricmp(t, "true") ? false : true);
			if (cmpPos >= 0)
			{
				if (assetHold.m_DefaultInitOrder != assetList[cmpPos].m_DefaultInitOrder)
					assetList[cmpPos].m_DefaultInitOrder = assetHold.m_DefaultInitOrder;
			}
		}

		if (!_stricmp(line, "keepaspect"))
		{
			assetHold.m_KeepAspect = (_stricmp(t, "true") ? false : true);
			if (cmpPos >= 0)
			{
				if (assetHold.m_KeepAspect != assetList[cmpPos].m_KeepAspect)
					assetList[cmpPos].m_KeepAspect = assetHold.m_KeepAspect;
			}
		}

		if (!_stricmp(line, "keepIconAspect"))
		{
			assetHold.m_KeepIconAspect = (_stricmp(t, "true") ? false : true);
			if (cmpPos >= 0)
			{
				if (assetHold.m_KeepIconAspect != assetList[cmpPos].m_KeepIconAspect)
					assetList[cmpPos].m_KeepIconAspect = assetHold.m_KeepIconAspect;
			}
		}

		if (!_stricmp(line, "R"))
		{
			assetHold.m_BgColor.r = (float)atoi(t);
			assetHold.m_BgColor.r /= 255.0f;
			if (cmpPos >= 0)
			{
				if (assetHold.m_BgColor.r != assetList[cmpPos].m_BgColor.r)
					assetList[cmpPos].m_BgColor.r = assetHold.m_BgColor.r;
			}
		}

		if (!_stricmp(line, "G"))
		{
			assetHold.m_BgColor.g = (float)atoi(t);
			assetHold.m_BgColor.g /= 255.0f;
			if (cmpPos >= 0)
			{
				if (assetHold.m_BgColor.g != assetList[cmpPos].m_BgColor.g)
					assetList[cmpPos].m_BgColor.g = assetHold.m_BgColor.g;
			}
		}

		if (!_stricmp(line, "B"))
		{
			assetHold.m_BgColor.b = (float)atoi(t);
			assetHold.m_BgColor.b /= 255.0f;
			if (cmpPos >= 0)
			{
				if (assetHold.m_BgColor.b != assetList[cmpPos].m_BgColor.b)
					assetList[cmpPos].m_BgColor.b = assetHold.m_BgColor.b;
			}
		}

		if (!_stricmp(line, "A"))
		{
			assetHold.m_BgColor.a = (float)(atof(t));
			assetHold.m_BgColor.a /= 255.0f;
			if (cmpPos >= 0)
			{
				if (assetHold.m_BgColor.a != assetList[cmpPos].m_BgColor.a)
					assetList[cmpPos].m_BgColor.a = assetHold.m_BgColor.a;
			}
		}

		if (!_stricmp(line, "load") || !_stricmp(line, "exec"))
		{
			UpdateAssetList(t, assetList);
		}

		if (t) delete[] t;

		delete[] line;
		line = NULL;
	}
	if (assetHold.m_Name.length() && cmpPos == -1)
	{
		assetHold.m_MenuID = (long)(assetList.size());
		assetList.push_back(assetHold);
		m_RebuildInitDat = true;
	}

	delete[] buffer;
	return true;
}

void Level1::BuildAssetList(const char* initFile, std::vector<Asset>& assetList)
{
	FILE* file;
	errno_t error = fopen_s(&file, initFile, "rb");
	if (error)
		return;

	fseek(file, 0, SEEK_END);
	int bufferLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[bufferLen + 1];
	buffer[bufferLen] = '\r';
	fread(buffer, bufferLen, 1, file);
	fclose(file);

	int position = 0;
	char* line = NULL;

	while (position < bufferLen)
	{
		position += GetLine(buffer + position, bufferLen - position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		TranslateLine(line, t, assetList);

		if (t) delete[] t;

		delete[] line;
		line = NULL;
	}

	delete[] buffer;
}

void Level1::LTrimString(char*& target)
{
	if (!target) return;
	int start = 0;

	for (int i = 0; i < (int)strlen(target); i++)
	{
		if (target[i] == '\n' || target[i] == '\r' || target[i] == ' ')
			continue;
		start = i;
		break;
	}

	char* newstr = new char[strlen(target) - start + 1];
	newstr[strlen(target) - start] = '\0';
	memcpy(newstr, target + start, strlen(target) - start);

	delete[] target;
	target = newstr;
}

void Level1::RTrimString(char*& target)
{
	if (!target) return;
	int end = 0;

	for (int i = (int)(strlen(target) - 1); i >= 0 ; i--)
	{
		if (target[i] == '\n' || target[i] == '\r' || target[i] == ' ')
			continue;
		end = i;
		break;
	}

	char* newstr = new char[strlen(target) - (strlen(target) - end) + 2];
	newstr[strlen(target) - (strlen(target) - end) + 1] = '\0';
	memcpy(newstr, target, strlen(target) - (strlen(target) - end) + 1);

	delete[] target;
	target = newstr;
}

void Level1::TrimString(char*& target)
{
	LTrimString(target);
	RTrimString(target);
}

char* Level1::SplitString(char*& target)
{
	//this will split the passed in character array into 2 arrays
	//the passed in array will change values to everything before the equals '=' sign
	//and it will return everything after
	char* preEqual = 0;
	char* postEqual = 0;
	char* pos = strchr(target, '=');
	if (!pos) return NULL;

	preEqual = new char[strlen(target) - strlen(pos) + 1];
	preEqual[strlen(target) - strlen(pos)] = '\0';
	memcpy(preEqual, target, strlen(target) - strlen(pos));
	
	pos++;
	postEqual = new char[strlen(pos) + 1];
	postEqual[strlen(pos)] = '\0';
	memcpy(postEqual, pos, strlen(pos));
	
	delete[] target;
	target = preEqual;

	return postEqual;
}

void Level1::TranslateLine(const char* beforeEqual, const char* afterEqual, std::vector<Asset>& assetList)
{
	if (!_stricmp(beforeEqual, "start"))
	{
		Asset asset;
		asset.m_Type = afterEqual;
		asset.m_MenuID = (long)(assetList.size());
		asset.m_BgColor.a = 0.6f;
		assetList.push_back(asset);
	}
	if (!_stricmp(beforeEqual, "name"))
	{
		if (assetList.size())
			assetList.back().m_Name = afterEqual;
	}
	if (!_stricmp(beforeEqual, "icon"))
	{
		if (assetList.size())
			assetList.back().m_IconPath = afterEqual;
	}
	if (!_stricmp(beforeEqual, "sprite"))
	{
		if (assetList.size())
			assetList.back().m_SpritePath = afterEqual;
	}
	if (!_stricmp(beforeEqual, "submenu"))
	{
		if (assetList.size())
			assetList.back().m_Submenu = afterEqual;
	}
	if (!_stricmp(beforeEqual, "DefaultInitOrder"))
	{
		assetList.back().m_DefaultInitOrder = (!_stricmp(afterEqual, "True")) ? true : false;
	}
	if (!_stricmp(beforeEqual, "KeepAspect"))
	{
		assetList.back().m_KeepAspect = (!_stricmp(afterEqual, "True")) ? true : false;
	}
	if (!_stricmp(beforeEqual, "KeepIconAspect"))
	{
		assetList.back().m_KeepIconAspect = (!_stricmp(afterEqual, "True")) ? true : false;
	}
	if (!_stricmp(beforeEqual, "R"))
	{
		assetList.back().m_BgColor.r = (float)atoi(afterEqual);
		assetList.back().m_BgColor.r /= 255.0f;
	}
	if (!_stricmp(beforeEqual, "G"))
	{
		assetList.back().m_BgColor.g = (float)atoi(afterEqual);
		assetList.back().m_BgColor.g /= 255.0f;
	}
	if (!_stricmp(beforeEqual, "B"))
	{
		assetList.back().m_BgColor.b = (float)atoi(afterEqual);
		assetList.back().m_BgColor.b /= 255.0f;
	}
	if (!_stricmp(beforeEqual, "A"))
	{
		assetList.back().m_BgColor.a = (float)atoi(afterEqual);
		assetList.back().m_BgColor.a /= 255.0f;
	}
	if (!_stricmp(beforeEqual, "load"))
	{
		BuildAssetList(afterEqual, assetList);
	}
	if (!_stricmp(beforeEqual, "framearray"))
	{
		D2D1_RECT_F trect = {};
		char value[512];
		int w = 0;
		int p = 0;
		for (int i = 0; i < (int)strlen(afterEqual); i++)
		{
			value[w++] = afterEqual[i];
			if (afterEqual[i] == ',')
			{
				value[w] = '\0';
				w = 0;
				switch (p)
				{
				case 0:
					trect.left = (float)(atof(value));
					p++;
					break;
				case 1:
					trect.top = (float)(atof(value));
					p++;
					break;
				case 2:
					trect.right = (float)(atof(value));
					p++;
					break;
				case 3:
					trect.bottom = (float)(atof(value));
					p=0;
					assetList.back().m_SpriteFrames.push(trect);
					break;
				}
			}
		}
	}
	if (!_stricmp(beforeEqual, "frameiconarray"))
	{
		D2D1_RECT_F trect = {};
		char value[512];
		int w = 0;
		int p = 0;
		for (int i = 0; i < (int)strlen(afterEqual); i++)
		{
			value[w++] = afterEqual[i];
			if (afterEqual[i] == ',')
			{
				value[w] = '\0';
				w = 0;
				switch (p)
				{
				case 0:
					trect.left = (float)(atof(value));
					p++;
					break;
				case 1:
					trect.top = (float)(atof(value));
					p++;
					break;
				case 2:
					trect.right = (float)(atof(value));
					p++;
					break;
				case 3:
					trect.bottom = (float)(atof(value));
					p = 0;
					assetList.back().m_IconFrames.push(trect);
					break;
				}
			}
		}
	}
}

bool Level1::SpriteExists(std::string path)
{
	if (!path.size()) return true;
	for (int i = 0; i < (int)m_SpriteList.size(); i++)
	{
		if (!_stricmp(m_SpriteList[i].c_str(), path.c_str()))
			return true;
	}
	return false;
}

void Level1::BuildSpriteList(std::vector<Asset> assetList)
{
	int maxSize = (int)assetList.size();
	while (!assetList.empty())
	{
/******* This section was originally put in for if the list was so long that the program would pause w/o showing anything, removed to improve speed
		gfx->BeginDraw();
		gfx->ClearScreen({ 1.0f, 1.0f, 1.0f, 1.0f });
		std::wstring msg = L"Building Sprite List " + std::to_wstring(maxSize - assetList.size());
		msg.append(L" of ");
		msg.append(std::to_wstring(maxSize));
		gfx->DrawText(msg.c_str(), { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT }, { 0.0f, 0.0f, 0.0f, 1.0f }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		gfx->EndDraw();*/
		std::string str = assetList.back().m_SpritePath;
		if (!SpriteExists(str))
		{
			m_SpriteList.push_back(str);
		}
		str = assetList.back().m_IconPath;
		if (!SpriteExists(str))
		{
			m_SpriteList.push_back(str);
		}
		assetList.pop_back();
	}
}

void Level1::LoadSpriteList()
{
	DeleteFile("loaded.log");
	DeleteFile("loading.log");
	for (int i = 0; i < (int)m_SpriteList.size(); i++)
	{
		FILE* file = NULL;
		FILE* load = NULL;
		fopen_s(&file, "loaded.log", "a");
		fopen_s(&load, "loading.log", "a");
		gfx->BeginDraw();
		gfx->ClearScreen({ 1.0f, 1.0f, 1.0f, 1.0f });
		std::wstring msg = L"Loading Sprite List " + std::to_wstring(i + 1);
		msg.append(L" of ");
		msg.append(std::to_wstring(m_SpriteList.size()));
		gfx->DrawText(msg.c_str(), { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT }, { 0.0f, 0.0f, 0.0f, 1.0f }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		gfx->EndDraw();

		if (i) fwprintf_s(load, L"\n");
		fwrite(&m_SpriteList[i][0], m_SpriteList[i].length(), 1, load);
		fclose(load);

		std::wstring path(m_SpriteList[i].length(), L' ');
		std::copy(m_SpriteList[i].begin(), m_SpriteList[i].end(), path.begin());
		SpriteSheet* sprite = new SpriteSheet(path.c_str(), gfx);
		if (!sprite->gfx || !sprite->bmp)
		{
			delete sprite;
			m_Sprites.push_back(NULL);
		}
		else
			m_Sprites.push_back(sprite);
		if (i) fwprintf_s(file, L"\n");
		//fwrite(&path[0], path.length() * sizeof(wchar_t), 1, file);
		std::string tstr(path.length(), '0');
		std::copy(path.begin(), path.end(), tstr.begin());
		fwrite(&tstr[0], tstr.length(), 1, file);
		tstr.clear();
		fclose(file);
	}
}

void Level1::EmptySpriteVector(std::vector<SpriteSheet*>& q)
{
	while (!q.empty())
	{
		if (q.back())
			delete q.back();
		q.pop_back();
	}
}

void Level1::EmptySpriteList(std::vector<std::string>& q)
{
	std::vector<std::string> empty;
	std::swap(q, empty);
}

void Level1::BindSpriteToAsset(std::vector<Asset>& assetList)
{
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		for (int w = 0; w < (int)m_SpriteList.size(); w++)
		{
			if (!_stricmp(m_SpriteList[w].c_str(), assetList[i].m_SpritePath.c_str()))
			{
				assetList[i].m_Sprite = m_Sprites[w];
				if (assetList[i].m_SpriteFrames.size())
					assetList[i].m_Sprite->m_Frames = assetList[i].m_SpriteFrames;
			}
			if (!_stricmp(m_SpriteList[w].c_str(), assetList[i].m_IconPath.c_str()))
			{
				assetList[i].m_Icon = m_Sprites[w];
				if (assetList[i].m_IconFrames.size())
					assetList[i].m_Icon->m_Frames = assetList[i].m_IconFrames;
			}
		}
	}
}

bool Level1::EntryExists(std::string path, std::queue<std::string> list)
{
	while (!list.empty())
	{
		if (!_stricmp(path.c_str(), list.front().c_str()))
			return true;
		list.pop();
	}
	return false;
}

void Level1::SortAssetList(std::vector<Asset> assetList)
{
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		if (!EntryExists(assetList[i].m_Type, m_TypeList))
		{
			m_TypeList.push(assetList[i].m_Type);
		}
		if (!EntryExists(assetList[i].m_Submenu, m_SubmenuList))
		{
			m_SubmenuList.push(assetList[i].m_Submenu);
		}
		if (!EntryExists(assetList[i].m_Name, m_ItemList))
		{
			m_ItemList.push(assetList[i].m_Name);
		}
	}
}

void Level1::BackupDatFile(const char* filepath)
{
	FILE* file;
	errno_t error;

	error = fopen_s(&file, filepath, "rb");
	if (error)
		return;

	fseek(file, 0, SEEK_END);
	int bufferlen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[bufferlen];
	fread(buffer, bufferlen, 1, file);
	fclose(file);

	char* backupPath;
	const char* p = strrchr(filepath, '.');
	const char ext[] = ".bak";
	backupPath = new char[strlen(filepath) - strlen(p) + strlen(ext) + 1];
	backupPath[strlen(filepath) - strlen(p) + strlen(ext)] = '\0';
	memcpy(backupPath, filepath, strlen(filepath) - strlen(p));
	memcpy(backupPath + strlen(filepath) - strlen(p), ext, strlen(ext));

	error = fopen_s(&file, backupPath, "wb");
	fwrite(buffer, bufferlen, 1, file);
	fclose(file);

	delete[] buffer;
	delete[] backupPath;
}

void Level1::RebuildInitDat(const char* filepath, std::vector<Asset>& assetList)
{
	BackupDatFile(filepath);

	FILE* file = NULL;
	errno_t error = fopen_s(&file, filepath, "wb");
	if (error)
	{
		MessageBox(NULL, "Unable to store updated assetlist.", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < (int)assetList.size(); i++)
	{
		fputs("start=", file);
		fputs(assetList[i].m_Type.c_str(), file);
		fputc(254, file);

		fputs("name=", file);
		fputs(assetList[i].m_Name.c_str(), file);
		fputc(254, file);

		fputs("icon=", file);
		fputs(assetList[i].m_IconPath.c_str(), file);
		fputc(254, file);

		fputs("sprite=", file);
		fputs(assetList[i].m_SpritePath.c_str(), file);
		fputc(254, file);

		fputs("submenu=", file);
		fputs(assetList[i].m_Submenu.c_str(), file);
		fputc(254, file);

		fputs("defaultinitorder=", file);
		fputs((assetList[i].m_DefaultInitOrder ? "true" : "false"), file);
		fputc(254, file);

		fputs("keepaspect=", file);
		fputs((assetList[i].m_KeepAspect ? "true" : "false"), file);
		fputc(254, file);

		fputs("KeepIconAspect=", file);
		fputs((assetList[i].m_KeepIconAspect ? "true" : "false"), file);
		fputc(254, file);

		fprintf(file, "R=%d", (int)(assetList[i].m_BgColor.r * 255.0f));
		fputc(254, file);

		fprintf(file, "G=%d", (int)(assetList[i].m_BgColor.g * 255.0f));
		fputc(254, file);

		fprintf(file, "B=%d", (int)(assetList[i].m_BgColor.b * 255.0f));
		putc(254, file);

		fprintf(file, "A=%d", (int)(assetList[i].m_BgColor.a * 255.0f));
		fputc(254, file);
	}
	fclose(file);
}

void Level1::DrawMap(std::vector<MapData> mapData, std::vector<ShapeData> shapes, std::vector<WallsData> walls)
{
//	D2D1_RECT_F VisibleArea = D2D1::RectF(m_DrawAreaRect.left + m_StartPoint.x * m_Scale, m_DrawAreaRect.top + m_StartPoint.y * m_Scale, m_DrawAreaRect.right + m_StartPoint.x * m_Scale, m_DrawAreaRect.bottom + m_StartPoint.y * m_Scale);
	if (m_UseOld)
		for (int layer = 0; layer <= m_NumberLayers; layer++)
		{
			if (!MenuItemChecked(m_ShowLayerMenu, MENU_SHOWLAYER0 + layer))
				continue;
			for (int i = 0; i < m_CG.size(); i++)
			{
				if (m_CG[i].m_Layer == layer)
				{
					if (m_CG[i].m_Path)
					{
						ID2D1BitmapBrush* pBitmapBrush = NULL;
						ID2D1BitmapRenderTarget *pComptibleTarget = NULL;

						float sizemod = 1.0f;
						if (m_CG[i].m_Size < Size::m_Medium)
						{
							sizemod = (float)(1.0f / (pow(2, (unsigned long)Size::m_Medium - (unsigned long)m_CG[i].m_Size)));
						}
						else if (m_ItemMenuArea.m_SelectedSize > Size::m_Medium)
						{
							sizemod = (float)((unsigned long)m_CG[i].m_Size - (unsigned long)Size::m_Medium + 1);
						}

						HRESULT hr = gfx->GetRenderTarget()->CreateCompatibleRenderTarget(D2D1::SizeF(m_Size * m_Scale * sizemod, m_Size * m_Scale * sizemod), &pComptibleTarget);
						if (SUCCEEDED(hr))
						{
							pComptibleTarget->BeginDraw();
							pComptibleTarget->Clear({ 0.0f, 0.0f, 0.0f, 0.0f });
							D2D1_RECT_F dest = {};
							dest.right = dest.bottom = m_Size * m_Scale * sizemod;
							//pComptibleTarget->DrawBitmap(m_ItemMenuArea.m_SelectedItem->m_Sprite->bmp, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, m_ItemMenuArea.m_SelectedItem->m_Sprite->GetFrame());
							if (m_AssetList[m_CG[i].m_BitmapID].m_Sprite)
								pComptibleTarget->DrawBitmap(m_AssetList[m_CG[i].m_BitmapID].m_Sprite->bmp, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, m_AssetList[m_CG[i].m_BitmapID].m_Sprite->GetFrame());
							pComptibleTarget->EndDraw();

							ID2D1Bitmap *tBitmap = NULL;
							hr = pComptibleTarget->GetBitmap(&tBitmap);
							if (SUCCEEDED(hr))
							{
								D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
								hr = gfx->GetRenderTarget()->CreateBitmapBrush(tBitmap, brushProperties, &pBitmapBrush);

								tBitmap->Release();
							}
						}
						pComptibleTarget->Release();

						if (SUCCEEDED(hr))
						{
							gfx->GetRenderTarget()->FillGeometry(m_CG[i].m_Path, pBitmapBrush);
							pBitmapBrush->Release();
						}
					}
				}
			}
			for (int i = 0; i < (int)mapData.size(); i++)
			{
				if (mapData[i].m_Layer == layer)
				{
					if (mapData[i].m_MenuId < (int)m_AssetList.size())
					{
						if (m_AssetList[mapData[i].m_MenuId].m_Sprite)
						{
							if (m_ShowBgColors)
							{
								gfx->FillRect(mapData[i].m_ColorArea, mapData[i].m_BgColor);
							}
							if (mapData[i].m_Rotation)
								gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(mapData[i].m_Rotation, D2D1::Point2F(mapData[i].dest.left + (mapData[i].dest.right - mapData[i].dest.left) / 2.0f, mapData[i].dest.top + (mapData[i].dest.bottom - mapData[i].dest.top) / 2.0f)));
							m_AssetList[mapData[i].m_MenuId].m_Sprite->Draw(mapData[i].dest, mapData[i].src);
							if (mapData[i].m_Rotation)
								gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
						}
						else
							gfx->FillRect(mapData[i].m_ColorArea, m_AssetList[mapData[i].m_MenuId].m_BgColor);
						if (mapData[i].m_Tag)
						{
							gfx->DrawVariableText(mapData[i].m_Tag, mapData[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
						}
						if (mapData[i].m_Status)
						{
							D2D1_COLOR_F tempColor;
							D2D1_COLOR_F tempFillColor;
							if (mapData[i].m_Status == L'↑')
							{
								tempFillColor = { 0.0, 0.392f, 0.0f, 0.66f };
								tempColor = { 1.0f, 0.545098f, 0.0f, 1.0f };
							}
							else if (mapData[i].m_Status == L'↓')
							{
								tempFillColor = { 1.0f, 0.412f, 0.71f, 0.66f };
								tempColor = { 1.0f, 0.2f, 0.0f, 1.0f };
							}
							else if (mapData[i].m_Status == L'Ω')
							{	//'Ω' or 234 or -21 will now represent being set on fire
								//fire does not need a color leave empty
								tempFillColor = { 0.0f, 0.0f, 0.0f, 0.0f };
								tempColor = { 0.0f, 0.0f, 0.0f, 0.0f };
							}
							else
							{
								tempFillColor = { 0.502f, 0.502f, 0.502f, 0.66f };
								tempColor = { 0.0f, 0.0f, 0.0f, 1.0f };
							}
							gfx->FillRect(mapData[i].m_ColorArea, tempFillColor);
							if (mapData[i].m_Status == L'Ω')
							{
								//draw fire here...
								if (m_OnFire->bmp) m_OnFire->Draw(mapData[i].m_ColorArea, m_OnFire->GetFrame(), 0.65f);
							}
							else if (mapData[i].m_Status != L'╥')
							{
								if (mapData[i].m_SizeCategory < Size::m_Medium)
								{
									float tTextSize = (float)Size::m_Medium - (float)mapData[i].m_SizeCategory + 1.0f;
									gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
								}
								else if (mapData[i].m_SizeCategory > Size::m_Colossal)
								{
									gfx->BuildVariableText(m_Size, m_Scale);
								}
								else
								{
									float tTextSize = (float)mapData[i].m_SizeCategory - (float)Size::m_Small;
									gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
								}
								gfx->DrawVariableText(mapData[i].m_Status, mapData[i].m_ColorArea, tempColor);
								gfx->BuildVariableText(m_Size, m_Scale);
							}
							else
							{
								if (mapData[i].m_SizeCategory < Size::m_Medium)
								{
									float tTextSize = (float)Size::m_Medium - (float)mapData[i].m_SizeCategory + 1.0f;
									gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
								}
								else if (mapData[i].m_SizeCategory > Size::m_Colossal)
								{
									gfx->BuildVariableText(m_Size, m_Scale);
								}
								else
								{
									float tTextSize = (float)mapData[i].m_SizeCategory - (float)Size::m_Small;
									gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
								}
								gfx->DrawVariableText(L"X_X", mapData[i].m_ColorArea, tempColor);
								gfx->BuildVariableText(m_Size, m_Scale);
							}
						}
					}
				}
			}
			for (int i = 0; i < (int)shapes.size(); i++)
			{
				if (shapes[i].m_Layer == layer)
				{
					D2D1_RECT_F cube = {};
					switch (shapes[i].m_Shape)
					{
					case Shapes::Cube:
						gfx->DrawRect(shapes[i].m_ColorArea, shapes[i].m_Color, 5.0f);
						break;
					case Shapes::FilledCube:
						gfx->FillRect(shapes[i].m_ColorArea, shapes[i].m_Color);
						if (shapes[i].m_Tag) gfx->DrawVariableText(shapes[i].m_Tag, shapes[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
						break;
					case Shapes::Sphere:
						gfx->DrawCircle({ shapes[i].m_ColorArea.left, shapes[i].m_ColorArea.top }, shapes[i].m_ConvertedRadius, shapes[i].m_Color, 5.0f);
						break;
					case Shapes::FilledSphere:
						gfx->FillCircle({ shapes[i].m_ColorArea.left, shapes[i].m_ColorArea.top }, shapes[i].m_ConvertedRadius, shapes[i].m_Color);
						break;
					}
				}
			}
			for (int i = 0; i < (int)walls.size(); i++)
			{
				if (walls[i].m_Layer == layer)
				{
					gfx->DrawLine({ (walls[i].m_StartPoint.x + m_StartPoint.x) * m_Scale, (walls[i].m_StartPoint.y + m_StartPoint.y) * m_Scale }, { (walls[i].m_EndPoint.x + m_StartPoint.x) * m_Scale, (walls[i].m_EndPoint.y + m_StartPoint.y) * m_Scale }, walls[i].m_Color, walls[i].m_Thickness);
				}
			}
		}
	else
		for (int i = 0; i < m_Rooms.size(); i++)
		{
			if (m_Rooms[i].m_Visible)
				m_Rooms[i].draw(m_DrawAreaRect, m_Size, m_Scale, m_ShowBgColors, m_StartPoint, m_OnFire, m_AssetList, gfx);
		}
	if (!m_ShowItemMenu && m_SelectedItem)
	{		
		gfx->DrawRect(m_SelectArea, { 1.0f, 0.0f, 0.0f, 1.0f }, 3.0f);
	}
	if (!m_ShowItemMenu && m_SelectedShape)
	{
		gfx->DrawRect(m_SelectedShape->m_ColorArea, { 1.0f, 0.0f, 0.0f, 1.0f }, 3.0f);
	}
}

void Level1::Save(std::wstring filePath)
{
	Save(filePath.c_str());
}

void Level1::Save(const wchar_t* filePath)
{
	FILE* file;
	errno_t error;

	int bufferLen = 0;
	char* buffer = FillBuffer(bufferLen);
	if (!buffer) return;

	error = _wfopen_s(&file, filePath, L"wb");
	if (error)
	{
		std::wstring Error = L"Error openning ";
		Error.append(filePath);
		Error.append(L". Error code: ");
		Error.append(std::to_wstring(error));
		MessageBoxW(NULL, Error.c_str(), L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	fwrite(buffer, bufferLen, 1, file);
	fclose(file);

	delete[] buffer;
}

void Level1::Save(std::string filePath)
{
	Save(filePath.c_str());
}

size_t Level1::CalcCustomColorSaveSize()
{
	size_t size = 0;
	size += sizeof(long);									//size of menuid
	size += sizeof(float) * 4;								//size of D2D1_COLOR_F
	return size;
}

size_t Level1::CalcSaveBufferSize()
{
	size_t size = 0;
	/*old method
	size += sizeof(double);									// size of the version information, using double for better precision
	size += sizeof(m_Scale);								// size of the scale variable
	size += sizeof(float) * 2;								// size of a D2D1_FLOAT_2F for start position
	size += sizeof(m_NumberLayers);							// size of var holding number of layers
	size += sizeof(m_CurrentLayer);							// size of var holding current layer
	size += sizeof(long);									// size of long var to hold number of entries in the initiative deque
	size += sizeof(long) * m_InitArea.m_InitOrder.size();	// size of long var to hold menu_id for initiative deque multiplied by the number of entries
	size += sizeof(long);									// size of long var to hold number of entries in the mapdata vector
	if (m_MapData.size())
	size += m_MapData.front().CalcSize() * m_MapData.size();// size of MapData struct multiplied by number of mapdata entries
	size += sizeof(long);									// size of long var to hold number of entries in shapesdata vector
	if (m_Shapes.size())
	size += m_Shapes.front().CalcSize() * m_Shapes.size();	// size of ShapesData struct multiplied by number of entries
	size += sizeof(long);									// size of long var to hold number of entries in walls vector
	if (m_Walls.size())
	size += m_Walls.front().CalcSize() * m_Walls.size();	// size of WallsData struct multiplied by number of entries
	size += sizeof(long);									// size of long to hold GridType
	//this is where custom color entries would be added as well as anything i've forgotten
	size += sizeof(long);									// size of long to hold number of custom colors
	size += CalcCustomColorSaveSize() * m_NumberCustomColors;	//calculate amount of space needed to store the required data in custom colors

	for (int i = 0; i < (int)m_AssetList.size(); i++)
	{
		if ((m_AssetList[i].m_MenuID & 0xFF00) == COLOR_CUSTOMS)	//this calculates amount of space needed to store the string result for the custom color entries
		{
			size += sizeof(long);							// size of long to hold length of assetlist
			size += sizeof(char) * m_AssetList[i].m_Submenu.length();	// size of string
		}
	}

	FILE* file = NULL;
	errno_t error = fopen_s(&file, "init.dat", "rb");
	if (error)
		return size;

	fseek(file, 0, SEEK_END);
	size += ftell(file);
	fclose(file);

	size += sizeof(float);		//added for storage of grid size
	size += sizeof(bool);		//added for storage of if grid lines are 'on top' or not

	size += sizeof(bool) * (m_NumberLayers + 1);		//added for storage of whether or not a particular layer is visible

	//added to support custom geometries
	size += sizeof(long);		//holds how many custom geometries
	for (int i = 0; i < m_CG.size(); i++)
	{
		size += m_CG[i].CalcSize();	//should calculate how much space is needed for the geometry
	}
	*/

	size += sizeof(double);									//version information
	size += sizeof(float);									//m_Size
	size += sizeof(char);									//m_ItemMenuArea.GridOfTop
	size += sizeof(float);									//m_Scale
	size += sizeof(float) * 2;								//m_StartPoint
	size += sizeof(unsigned long);							//m_CurrentRoom
	size += sizeof(unsigned long);							//m_CurrentLayer
	size += sizeof(long);									//number rooms
	for (int i = 0; i < m_Rooms.size(); i++)
	{
		size += m_Rooms[i].CalculateBufferSize();			//calculate space required for each room
	}
	
	size += sizeof(long);									//m_InitArea.m_InitOrder.size()
	size += sizeof(long) * m_InitArea.m_InitOrder.size();	//size to hold entries
	
	size += sizeof(long);									//grid type
	size += sizeof(long);									//number of custom colors
	size += m_NumberCustomColors * CalcCustomColorSaveSize();//space for custom colors
	for (int i = 0; i < (int)m_AssetList.size(); i++)
	{
		if ((m_AssetList[i].m_MenuID & 0xFF00) == COLOR_CUSTOMS)	//this calculates amount of space needed to store the string result for the custom color entries
		{
			size += sizeof(long);							// size of long to hold length of assetlist
			size += sizeof(char) * m_AssetList[i].m_Submenu.length();	// size of string
		}
	}
	FILE* file = NULL;
	fopen_s(&file, "init.dat", "rb");
	fseek(file, 0, SEEK_END);
	size_t p = ftell(file);
	fclose(file);
	size += p;
	
	return size;
}

void Level1::Save(const char* filePath)
{
	FILE* file;
	errno_t error;

	int bufferLen = 0;
	char* buffer = FillBuffer(bufferLen);
	if (!buffer) return;
	
	error = fopen_s(&file, filePath, "wb");
	if (error)
	{
		std::string Error = "Error openning ";
		Error.append(filePath);
		Error.append(". Error code: ");
		Error.append(std::to_string(error));
		MessageBox(NULL, Error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return;
	}
	fwrite(buffer, bufferLen, 1, file);
	fclose(file);

	delete[] buffer;
}

char* Level1::FillBuffer(int& bufferSize)
{
	/*old method
	bufferSize = (int)CalcSaveBufferSize();
	if (!bufferSize) return NULL;

	char* buffer = new char[bufferSize];
	size_t position = 0;
	double version = VERSION;
	
	memcpy(buffer, &version, sizeof(double));
	position += sizeof(double);
	memcpy(buffer + position, &m_Size, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_ItemMenuArea.m_GridOnTop, sizeof(bool));
	position += sizeof(bool);
	memcpy(buffer + position, &m_Scale, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_StartPoint.x, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_StartPoint.y, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_NumberLayers, sizeof(long));
	position += sizeof(long);
	memcpy(buffer + position, &m_CurrentLayer, sizeof(long));
	position += sizeof(long);

	//this next section will store a series of bool variables to represent wether or not a layer is visible
	for (int i = 0; i <= m_NumberLayers; i++)
	{
		bool visible = MenuItemChecked(m_ShowLayerMenu, MENU_SHOWLAYER0 + i);
		memcpy(buffer + position, &visible, sizeof(bool));
		position += sizeof(bool);
	}
	//end that section

	long numberEntries = (long)(m_InitArea.m_InitOrder.size());
	memcpy(buffer + position, &numberEntries, sizeof(long));
	position += sizeof(long);

	std::deque<Asset*> list = m_InitArea.m_InitOrder;
	while (!list.empty())
	{
		memcpy(buffer + position, &list.front()->m_MenuID, sizeof(long));
		position += sizeof(long);
		list.pop_front();
	}

	numberEntries = (long)m_MapData.size();
	memcpy(buffer + position, &numberEntries, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberEntries; i++)
	{
		size_t tempSize = m_MapData[i].CalcSize();
		char* tempBuffer = m_MapData[i].BuildBuffer();
		if (tempBuffer)
		{
			memcpy(buffer + position, tempBuffer, tempSize);
			delete[] tempBuffer;
		}
		position += tempSize;
	}

	numberEntries = (long)m_Shapes.size();
	memcpy(buffer + position, &numberEntries, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberEntries; i++)
	{
		size_t tempSize = m_Shapes[i].CalcSize();
		char* tempBuffer = m_Shapes[i].BuildBuffer();
		if (tempBuffer)
		{
			memcpy(buffer + position, tempBuffer, tempSize);
			delete[] tempBuffer;
		}
		position += tempSize;
	}

	numberEntries = (long)m_Walls.size();
	memcpy(buffer + position, &numberEntries, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberEntries; i++)
	{
		size_t tempSize = m_Walls[i].CalcSize();
		char* tempBuffer = m_Walls[i].BuildBuffer();
		if (tempBuffer)
		{
			memcpy(buffer + position, tempBuffer, tempSize);
			delete[] tempBuffer;
		}
		position += tempSize;
	}

	long gridtype = (long)m_GridType;
	memcpy(buffer + position, &gridtype, sizeof(long));
	position += sizeof(long);

	memcpy(buffer + position, &m_NumberCustomColors, sizeof(long));
	position += sizeof(long);

	for (int i = 0; i < (int)m_AssetList.size(); i++)
	{
		if ((m_AssetList[i].m_MenuID & COLOR_CUSTOMS) == COLOR_CUSTOMS)
		{
			memcpy(buffer + position, &m_AssetList[i].m_MenuID, sizeof(long));
			position += sizeof(long);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.r, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.g, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.b, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.a, sizeof(float));
			position += sizeof(float);
			long numChars = (long)m_AssetList[i].m_Submenu.length();
			memcpy(buffer + position, &numChars, sizeof(long));
			position += sizeof(long);
			memcpy(buffer + position, &m_AssetList[i].m_Submenu[0], numChars * sizeof(char));
			position += numChars * sizeof(char);
		}
	}

	//this is where the custom geometries are added
	long numGeo = m_CG.size();
	memcpy(buffer + position, &numGeo, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < m_CG.size(); i++)
	{
		memcpy(buffer + position, &m_CG[i].m_Offset.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_CG[i].m_Offset.y, sizeof(float));
		position += sizeof(float);
		long numPoin = m_CG[i].m_Points.size();
		memcpy(buffer + position, &numPoin, sizeof(long));
		position += sizeof(long);
		for (int w = 0; w < m_CG[i].m_Points.size(); w++)
		{
			memcpy(buffer + position, &m_CG[i].m_Points[w].x, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_CG[i].m_Points[w].y, sizeof(float));
			position += sizeof(float);
		}
		memcpy(buffer + position, &m_CG[i].m_Layer, sizeof(long));
		position += sizeof(long);
		memcpy(buffer + position, &m_CG[i].m_BitmapID, sizeof(long));
		position += sizeof(long);
		long sizecov = (long)m_CG[i].m_Size;
		memcpy(buffer + position, &sizecov, sizeof(long));
		position += sizeof(long);
		memcpy(buffer + position, &m_CG[i].m_Room, sizeof(unsigned long));
	}

	FILE* file = NULL;
	errno_t error = fopen_s(&file, "init.dat", "rb");
	fseek(file, 0, SEEK_END);
	size_t p = ftell(file);
	fseek(file, 0, SEEK_SET);
	fread(buffer + position, p, 1, file);
	fclose(file);
	position += p;	
	*/
	bufferSize = CalcSaveBufferSize();
	if (!bufferSize) return NULL;

	char* buffer = new char[bufferSize];
	size_t position = 0;

	double version = VERSION;

	memcpy(buffer + position, &version, sizeof(double));
	position += sizeof(double);
	memcpy(buffer + position, &m_Size, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_ItemMenuArea.m_GridOnTop, sizeof(char));
	position += sizeof(char);
	memcpy(buffer + position, &m_Scale, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_StartPoint.x, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_StartPoint.y, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_CurrentRoom, sizeof(unsigned long));
	position += sizeof(unsigned long);
	memcpy(buffer + position, &m_CurrentLayer, sizeof(unsigned long));
	position += sizeof(unsigned long);
	long numRooms = m_Rooms.size();
	memcpy(buffer + position, &numRooms, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numRooms; i++)
	{
		memcpy(buffer + position, &m_Rooms[i].m_Visible, sizeof(char));
		position += sizeof(char);
		long numLayers = m_Rooms[i].m_Layer.size();
		memcpy(buffer + position, &numLayers, sizeof(long));
		position += sizeof(long);
		for (int w = 0; w < numLayers; w++)
		{
			char* tbuff = m_Rooms[i].m_Layer[w].BuildBuffer();
			memcpy(buffer + position, tbuff, m_Rooms[i].m_Layer[w].CalculateBufferSize());
			position += m_Rooms[i].m_Layer[w].CalculateBufferSize();
			delete[] tbuff;
		}
	}
	
	long initsize = m_InitArea.m_InitOrder.size();
	memcpy(buffer + position, &initsize, sizeof(long));
	position += sizeof(long);

	std::deque<Asset*> list = m_InitArea.m_InitOrder;
	while (!list.empty())
	{
		memcpy(buffer + position, &list.front()->m_MenuID, sizeof(long));
		position += sizeof(long);
		list.pop_front();
	}
	
	long gridtype = (long)m_GridType;
	memcpy(buffer + position, &gridtype, sizeof(long));
	position += sizeof(long);

	memcpy(buffer + position, &m_NumberCustomColors, sizeof(long));
	position += sizeof(long);

	for (int i = 0; i < (int)m_AssetList.size(); i++)
	{
		if ((m_AssetList[i].m_MenuID & COLOR_CUSTOMS) == COLOR_CUSTOMS)
		{
			memcpy(buffer + position, &m_AssetList[i].m_MenuID, sizeof(long));
			position += sizeof(long);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.r, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.g, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.b, sizeof(float));
			position += sizeof(float);
			memcpy(buffer + position, &m_AssetList[i].m_BgColor.a, sizeof(float));
			position += sizeof(float);
			long numChars = (long)m_AssetList[i].m_Submenu.length();
			memcpy(buffer + position, &numChars, sizeof(long));
			position += sizeof(long);
			memcpy(buffer + position, &m_AssetList[i].m_Submenu[0], numChars * sizeof(char));
			position += numChars * sizeof(char);
		}
	}

	FILE* file = NULL;
	fopen_s(&file, "init.dat", "rb");
	fseek(file, 0, SEEK_END);
	size_t p = ftell(file);
	fseek(file, 0, SEEK_SET);
	fread(buffer + position, p, 1, file);
	fclose(file);
	position += p;
	
	return buffer;
}

void Level1::Open(std::wstring filePath)
{
	Open(filePath.c_str());
}

void Level1::Open(const wchar_t* filePath)
{
	FILE* file = NULL;
	errno_t error = _wfopen_s(&file, filePath, L"rb");
	if (error)
	{
		std::wstring Error = L"Unable to open ";
		Error.append(filePath);
		Error.append(L". Error Code: ");
		Error.append(std::to_wstring(error));
		MessageBoxW(NULL, Error.c_str(), L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	float version = 0;
	fscanf_s(file, "%f ", &version);
	if (!version)
	{
		fseek(file, 0, SEEK_END);
		size_t bufferLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* buffer = new char[bufferLen];
		fread(buffer, bufferLen, 1, file);
		fclose(file);

		LoadFromBuffer(buffer, bufferLen);
		delete[] buffer;
	}
	else
	{
		LoadOldVersion(file);
		fclose(file);
	}
}

void Level1::Open(std::string filePath)
{
	Open(filePath.c_str());
}

void Level1::LoadOldVersion(FILE* file)
{
	if (!file) return;
	fseek(file, 0, SEEK_SET);

	EmptyMapData(m_MapData);

	struct MapEntry
	{
		FPOINT pos;
		int viewMode;
		int currentAnimation;
		float sizeMod;
		unsigned long long value;
		int status;
		COLORREF_F color;
		char tag;
	};

	float version = 0.0f;
	float x, y, xSpeed, ySpeed, scale;
	long gridontop, showbgcolor, viewmode, currentlayer, radius, width;
	fscanf_s(file, "%f ", &version);
	fscanf_s(file, "%f ", &x);
	fscanf_s(file, "%f ", &y);
	fscanf_s(file, "%f ", &xSpeed);
	fscanf_s(file, "%f ", &ySpeed);
	fscanf_s(file, "%f ", &scale);
	fscanf_s(file, "%d ", &gridontop);
	fscanf_s(file, "%d ", &showbgcolor);
	fscanf_s(file, "%d ", &viewmode);
	fscanf_s(file, "%d ", &currentlayer);
	fscanf_s(file, "%d ", &radius);
	fscanf_s(file, "%d ", &width);

	m_Scale = 1.0f;
	m_GridOnTop = (bool)gridontop;
	m_ShowBgColors = (bool)showbgcolor;
	m_CurrentLayer = currentlayer;
	m_StartPoint.x = x * m_Size * m_Scale;
	m_StartPoint.y = y * m_Size * m_Scale;

	for (int i = 0; i < 5; i++)
	{
		int numberEntries = 0;
		fscanf_s(file, "%d ", &numberEntries);
		for (int loop = 0; loop <= numberEntries; loop++)
		{
			MapEntry mapEntry = {};
			MapData data = {};
			fread_s(&mapEntry, sizeof(MapEntry), sizeof(MapEntry), 1, file);
			data.m_Position = mapEntry.pos;
			data.dest.left = mapEntry.pos.x;
			data.dest.top = mapEntry.pos.y;
			data.dest.right = data.dest.left + m_Size * m_Scale * mapEntry.sizeMod;
			data.dest.bottom = data.dest.top + m_Size * m_Scale * mapEntry.sizeMod;
			data.m_MenuId = (int)mapEntry.value;
			if (data.m_MenuId > 0x1000) data.m_MenuId -= 0x1000;
			if (mapEntry.sizeMod < 1.0f)
				data.m_SizeCategory = Size::m_Small;
			if (mapEntry.sizeMod == 1.0f)
				data.m_SizeCategory = Size::m_Medium;
			if (mapEntry.sizeMod == 2.0f)
				data.m_SizeCategory = Size::m_Large;
			if (mapEntry.sizeMod == 3.0f)
				data.m_SizeCategory = Size::m_Huge;
			if (mapEntry.sizeMod == 4.0f)
				data.m_SizeCategory = Size::m_Gargantuan;
			if (mapEntry.sizeMod == 5.0f)
				data.m_SizeCategory = Size::m_Colossal;
			m_MapData.push_back(data);
		}
	}
}

void Level1::LoadFromBuffer(char* buffer, const size_t bufferLen)
{
	size_t position = 0;

	double version = 0;
	memcpy(&version, buffer, sizeof(double));
	position += sizeof(double);

	if (version >= 5.9999)
	{
		//loading data from the new save style
		memcpy(&m_Size, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ItemMenuArea.m_GridOnTop, buffer + position, sizeof(char));
		position += sizeof(char);
		memcpy(&m_Scale, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_StartPoint.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_StartPoint.y, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_CurrentRoom, buffer + position, sizeof(unsigned long));
		position += sizeof(unsigned long);
		memcpy(&m_CurrentLayer, buffer + position, sizeof(unsigned long));
		position += sizeof(unsigned long);
		long numRooms;
		memcpy(&numRooms, buffer + position, sizeof(long));
		position += sizeof(long);
		while (!m_Rooms.empty())
			m_Rooms.pop_back();
		for (int i = 0; i < numRooms; i++)
		{
			Room newRoom;
			char tvis;
			memcpy(&tvis, buffer + position, sizeof(char));
			newRoom.m_Visible = (bool)tvis;
			position += sizeof(char);
			size_t roomsize = newRoom.FillFromBuffer(buffer + position, version, gfx);
			position += roomsize;
			m_Rooms.push_back(newRoom);			
		}

		long initsize;
		memcpy(&initsize, buffer + position, sizeof(long));
		position += sizeof(long);
		m_InitArea.EmtpyQueue(m_InitArea.m_InitOrder);
		std::queue<long> initListRebuild;
		for (int i = 0; i < initsize; i++)
		{
			long menuid = 0;
			memcpy(&menuid, buffer + position, sizeof(long));
			position += sizeof(long);
			m_InitArea.AddTurn(menuid, m_AssetList);
			initListRebuild.push(menuid);
		}

		long gridtype;
		memcpy(&gridtype, buffer + position, sizeof(long));
		m_GridType = (Grid)gridtype;
		position += sizeof(long);

		memcpy(&m_NumberCustomColors, buffer + position, sizeof(long));
		position += sizeof(long);

		std::vector<Asset> tCustomColorList;
		for (int i = 0; i < m_NumberCustomColors; i++)
		{
			Asset tAsset;
			memcpy(&tAsset.m_MenuID, buffer + position, sizeof(long));
			position += sizeof(long);
			memcpy(&tAsset.m_BgColor.r, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.g, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.b, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.a, buffer + position, sizeof(float));
			position += sizeof(float);
			long numChars = 0;
			memcpy(&numChars, buffer + position, sizeof(long));
			position += sizeof(long);
			char* tSubMenu = new char[numChars + 1];
			tSubMenu[numChars] = '\0';
			memcpy(tSubMenu, buffer + position, sizeof(char) * numChars);
			position += sizeof(char) * numChars;
			tAsset.m_Type = "Shapes";
			tAsset.m_Submenu = tSubMenu;
			tAsset.m_Name = std::to_string(tAsset.m_MenuID);
			//m_AssetList.push_back(tAsset);						//this line became unnecessary when the not loading custom colors fix was added
			tCustomColorList.push_back(tAsset);
			delete[] tSubMenu;
		}

		EmptySpriteVector(m_Sprites);
		EmptySpriteList(m_SpriteList);
		EmptyAssetList(m_AssetList);

		BuildAssetListFromBuffer(buffer + position, bufferLen - position, m_AssetList);
		UpdateAssetList("init.ini", m_AssetList);
		m_InitArea.EmtpyQueue(m_InitArea.m_InitOrder);
		while (initListRebuild.size())
		{
			m_InitArea.AddTurn(initListRebuild.front(), m_AssetList);
			initListRebuild.pop();
		}
		BuildSpriteList(m_AssetList);
		LoadSpriteList();
		BindSpriteToAsset(m_AssetList);
		m_ItemMenuArea.Cleanup();
		m_ItemMenuArea.m_SelectedSizeCategory = &m_ItemMenuArea.m_SizeCategory;
		m_ItemMenuArea.BuildMenuItem(m_AssetList);

		for (int i = 0; i < tCustomColorList.size(); i++)
		{
			m_AssetList.push_back(tCustomColorList[i]);
		}

		for (int i = 0; i < m_Rooms.size(); i++)
		{
			for (int w = 0; w < m_Rooms[i].m_Layer.size(); w++)
			{
				for (int x = 0; x < m_Rooms[i].m_Layer[w].m_CG.size(); x++)
				{
					m_Rooms[i].m_Layer[w].m_CG[x].BuildGeometry(m_Scale, m_StartPoint);
				}
			}
		}
		RebuildRoomMenu();
		RebuildLayerMenu();			
	}
	else
	{
		if (version >= 5.129)
		{
			memcpy(&m_Size, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&m_ItemMenuArea.m_GridOnTop, buffer + position, sizeof(bool));
			position += sizeof(bool);
		}
		memcpy(&m_Scale, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_StartPoint.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_StartPoint.y, buffer + position, sizeof(float));
		position += sizeof(float);

		/*old layer
		long NumberLayers = m_NumberLayers;
		long loopcount = 0;
		memcpy(&loopcount, buffer + position, sizeof(long));
		position += sizeof(long);
		for (NumberLayers; NumberLayers < loopcount; NumberLayers++)
			AddLayerMenu();
		

		UncheckLayer();
		memcpy(&m_CurrentLayer, buffer + position, sizeof(long));
		position += sizeof(long);
		CheckLayer();
		*/
		long numberlayer = m_Rooms[0].m_Layer.size();
		long loopcount = 0;
		memcpy(&loopcount, buffer + position, sizeof(long));
		m_NumberLayers = loopcount;
		position += sizeof(long);
		for (numberlayer; numberlayer <= loopcount; numberlayer++)
		{
			AddLayerMenu();			
		}
		UncheckLayer();
		memcpy(&m_CurrentLayer, buffer + position, sizeof(long));
		position += sizeof(long);
		CheckLayer();

		if (version >= 5.149)
		{
			bool visible;
			for (long tlayer = 0; tlayer <= m_NumberLayers; tlayer++)
			{
				memcpy(&visible, buffer + position, sizeof(bool));
				position += sizeof(bool);
				if (visible)
				{
					SetShowLayer(MENU_SHOWLAYER0 + tlayer);
					m_Rooms[0].m_Layer[tlayer].m_Visible = true;
				}
				else
				{
					SetHideLayer(MENU_SHOWLAYER0 + tlayer);
					m_Rooms[0].m_Layer[tlayer].m_Visible = false;
				}
			}
		}

		long numberEntries = 0;
		memcpy(&numberEntries, buffer + position, sizeof(long));
		position += sizeof(long);
		m_InitArea.EmtpyQueue(m_InitArea.m_InitOrder);
		std::queue<long> initListRebuild;
		for (int i = 0; i < numberEntries; i++)
		{
			long menuid = 0;
			memcpy(&menuid, buffer + position, sizeof(long));
			position += sizeof(long);
			m_InitArea.AddTurn(menuid, m_AssetList);
			initListRebuild.push(menuid);
		}

		memcpy(&numberEntries, buffer + position, sizeof(long));
		position += sizeof(long);
		EmptyMapData(m_MapData);
		/*old method
		for (int i = 0; i < numberEntries; i++)
		{
			MapData temp;
			temp.FillFromBuffer(buffer + position, version);
			position += temp.CalcSize();
			if (version < 5.04)
				position -= sizeof(float);
			if (version < 5.17)
				position -= sizeof(unsigned long);
			if (version >= 5.169)
			{
				position -= sizeof(bool);
				position += sizeof(char);
			}
			m_MapData.push_back(temp);
		}*/
		m_Rooms[0].ClearAll();
		for (int i = 0; i < numberEntries; i++)
		{
			MapData temp;
			temp.FillFromBuffer(buffer + position, version);
			position += temp.CalcSize();
			if (version < 5.04)
				position -= sizeof(float);
			if (version < 5.17)
				position -= sizeof(unsigned long);
			if (version >= 5.169)
			{
				position -= sizeof(bool);
				position += sizeof(char);
			}
			m_Rooms[0].m_Layer[temp.m_Layer].m_MapData.push_back(temp);
		}

		memcpy(&numberEntries, buffer + position, sizeof(long));
		position += sizeof(long);
		EmptyShapeData(m_Shapes);
		/*old method
		for (int i = 0; i < numberEntries; i++)
		{
			ShapeData temp;
			temp.FillFromBuffer(buffer + position);
			position += temp.CalcSize();
			m_Shapes.push_back(temp);
		}
		*/
		for (int i = 0; i < numberEntries; i++)
		{
			ShapeData temp;
			temp.FillFromBuffer(buffer + position);
			position += temp.CalcSize();
			m_Rooms[0].m_Layer[temp.m_Layer].m_Shapes.push_back(temp);
		}

		memcpy(&numberEntries, buffer + position, sizeof(long));
		position += sizeof(long);
		EmptyWallData(m_Walls);
		/*old method
		for (int i = 0; i < numberEntries; i++)
		{
			WallsData temp;
			temp.FillFromBuffer(buffer + position);
			position += temp.CalcSize();
			m_Walls.push_back(temp);
		}
		*/
		for (int i = 0; i < numberEntries; i++)
		{
			WallsData temp;
			temp.FillFromBuffer(buffer + position);
			position += temp.CalcSize();
			m_Rooms[0].m_Layer[temp.m_Layer].m_Walls.push_back(temp);
		}

		long gridtype = 0;
		memcpy(&gridtype, buffer + position, sizeof(long));
		m_GridType = (Grid)gridtype;
		position += sizeof(long);

		memcpy(&m_NumberCustomColors, buffer + position, sizeof(long));
		position += sizeof(long);

		std::vector<Asset> tCustomColorList;
		for (int i = 0; i < m_NumberCustomColors; i++)
		{
			Asset tAsset;
			memcpy(&tAsset.m_MenuID, buffer + position, sizeof(long));
			position += sizeof(long);
			memcpy(&tAsset.m_BgColor.r, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.g, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.b, buffer + position, sizeof(float));
			position += sizeof(float);
			memcpy(&tAsset.m_BgColor.a, buffer + position, sizeof(float));
			position += sizeof(float);
			long numChars = 0;
			memcpy(&numChars, buffer + position, sizeof(long));
			position += sizeof(long);
			char* tSubMenu = new char[numChars + 1];
			tSubMenu[numChars] = '\0';
			memcpy(tSubMenu, buffer + position, sizeof(char) * numChars);
			position += sizeof(char) * numChars;
			tAsset.m_Type = "Shapes";
			tAsset.m_Submenu = tSubMenu;
			tAsset.m_Name = std::to_string(tAsset.m_MenuID);
			//m_AssetList.push_back(tAsset);						//this line became unnecessary when the not loading custom colors fix was added
			tCustomColorList.push_back(tAsset);
			delete[] tSubMenu;
		}

		if (version >= 5.159)
		{
			std::vector<CustomGeometry> empty;
			for (int i = 0; i < m_CG.size(); i++)
			{
				if (m_CG[i].m_Sink) m_CG[i].m_Sink->Release();
				if (m_CG[i].m_Path) m_CG[i].m_Path->Release();
			}
			std::swap(empty, m_CG);
			/*old method
			long numGeo = 0;
			memcpy(&numGeo, buffer + position, sizeof(long));
			position += sizeof(long);
			for (int i = 0; i < numGeo; i++)
			{
				CustomGeometry tcg;
				tcg.gfx = gfx;
				memcpy(&tcg.m_Offset.x, buffer + position, sizeof(float));
				position += sizeof(float);
				memcpy(&tcg.m_Offset.y, buffer + position, sizeof(float));
				position += sizeof(float);
				long numPoin = 0;
				memcpy(&numPoin, buffer + position, sizeof(long));
				position += sizeof(long);
				for (int w = 0; w < numPoin; w++)
				{
					D2D1_POINT_2F ttpp;
					memcpy(&ttpp.x, buffer + position, sizeof(float));
					position += sizeof(float);
					memcpy(&ttpp.y, buffer + position, sizeof(float));
					position += sizeof(float);
					tcg.m_Points.push_back(ttpp);
				}
				memcpy(&tcg.m_Layer, buffer + position, sizeof(long));
				position += sizeof(long);
				memcpy(&tcg.m_BitmapID, buffer + position, sizeof(long));
				position += sizeof(long);
				long ttsize;
				memcpy(&ttsize, buffer + position, sizeof(long));
				position += sizeof(long);
				tcg.m_Size = (Size)ttsize;
				if (version >= 5.169)
				{
					memcpy(&tcg.m_Room, buffer + position, sizeof(unsigned long));
					position += sizeof(unsigned long);
				}
				m_CG.push_back(tcg);
				*/
			long numGeo = 0;
			memcpy(&numGeo, buffer + position, sizeof(long));
			position += sizeof(long);
			for (int i = 0; i < numGeo; i++)
			{
				CustomGeometry tcg;
				tcg.gfx = gfx;
				memcpy(&tcg.m_Offset.x, buffer + position, sizeof(float));
				position += sizeof(float);
				memcpy(&tcg.m_Offset.y, buffer + position, sizeof(float));
				position += sizeof(float);
				long numPoin = 0;
				memcpy(&numPoin, buffer + position, sizeof(long));
				position += sizeof(long);
				for (int w = 0; w < numPoin; w++)
				{
					D2D1_POINT_2F ttpp;
					memcpy(&ttpp.x, buffer + position, sizeof(float));
					position += sizeof(float);
					memcpy(&ttpp.y, buffer + position, sizeof(float));
					position += sizeof(float);
					tcg.m_Points.push_back(ttpp);
				}
				memcpy(&tcg.m_Layer, buffer + position, sizeof(long));
				position += sizeof(long);
				memcpy(&tcg.m_BitmapID, buffer + position, sizeof(long));
				position += sizeof(long);
				long ttsize;
				memcpy(&ttsize, buffer + position, sizeof(long));
				position += sizeof(long);
				tcg.m_Size = (Size)ttsize;
				if (version >= 5.169)
				{
					memcpy(&tcg.m_Room, buffer + position, sizeof(unsigned long));
					position += sizeof(unsigned long);
				}
				m_Rooms[0].m_Layer[tcg.m_Layer].m_CG.push_back(tcg);
			}
		}

		if (version >= 5.079)
		{
			EmptySpriteVector(m_Sprites);
			EmptySpriteList(m_SpriteList);
			EmptyAssetList(m_AssetList);

			BuildAssetListFromBuffer(buffer + position, bufferLen - position, m_AssetList);
			UpdateAssetList("init.ini", m_AssetList);
			m_InitArea.EmtpyQueue(m_InitArea.m_InitOrder);
			while (initListRebuild.size())
			{
				m_InitArea.AddTurn(initListRebuild.front(), m_AssetList);
				initListRebuild.pop();
			}
			BuildSpriteList(m_AssetList);
			LoadSpriteList();
			BindSpriteToAsset(m_AssetList);
			m_ItemMenuArea.Cleanup();
			m_ItemMenuArea.m_SelectedSizeCategory = &m_ItemMenuArea.m_SizeCategory;
			m_ItemMenuArea.BuildMenuItem(m_AssetList);
		}

		for (int i = 0; i < tCustomColorList.size(); i++)
		{
			m_AssetList.push_back(tCustomColorList[i]);
		}
	}
	m_ItemMenuArea.m_SelectedType = &m_AssetList[0];
	m_ItemMenuArea.m_SelectedSubMenu = &m_AssetList[0];
	m_ItemMenuArea.m_SelectedItem = &m_AssetList[0];
	m_ItemMenuArea.RebuildItemMenuList(m_AssetList);
}

void Level1::Open(const char* filePath)
{
	FILE* file = NULL;
	errno_t error = fopen_s(&file, filePath, "rb");

	if (error)
	{
		std::string Error = "Unable to open ";
		Error.append(filePath);
		Error.append(". Error Code: ");
		Error.append(std::to_string(error));
		MessageBox(NULL, Error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return;
	}
	
	float version = 0.0f;
	double dVersion = 0.0;
	fread(&dVersion, sizeof(double), 1, file);
	fseek(file, 0, SEEK_SET);
	fread(&version, sizeof(float), 1, file);
	if (dVersion >= 5.0)
	{
		fseek(file, 0, SEEK_END);
		size_t bufferLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* buffer = new char[bufferLen];
		fread(buffer, bufferLen, 1, file);
		fclose(file);

		LoadFromBuffer(buffer, bufferLen);
		delete[] buffer;
	}
	else
	{
		LoadOldVersion(file);
		fclose(file);
	}
}

void Level1::EmptyMapData(std::vector<MapData>& data)
{
	std::vector<MapData> empty;
	std::swap(data, empty);
}

void Level1::EmptyShapeData(std::vector<ShapeData>& shape)
{
	std::vector<ShapeData> empty;
	std::swap(shape, empty);
}

void Level1::EmptyWallData(std::vector<WallsData>& wall)
{
	std::vector<WallsData> empty;
	std::swap(wall, empty);
}

std::string Level1::GetSaveFilePath()
{
	char FilePath[510];
	std::string Path;
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GameController::hWnd;
	ofn.lpstrFilter = "Map Files (*.d2m)\0*.d2m\0Map Files (*.dmap)\0*.dmap\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = "d2m";
	ofn.lpstrFile = FilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 510;
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&ofn))
	{
		Path = FilePath;
	}
	return Path;
}

std::wstring Level1::WGetSaveFilePath()
{
	std::wstring Path;
	wchar_t FilePath[510];

	OPENFILENAMEW ofn = {};
	
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = GameController::hWnd;
	ofn.lpstrFilter = L"Map Files (*.d2m)\0*.d2m\0Map Files (*.dmap)\0*.dmap\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"d2m";
	ofn.lpstrFile = FilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 510;
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	if (GetSaveFileNameW(&ofn))
	{
		Path = FilePath;
	}

	return Path;
}

std::string Level1::GetLoadFilePath()
{
	char FilePath[510];
	std::string Path;
	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GameController::hWnd;
	ofn.lpstrFilter = "Map Files (*.d2m)\0*.d2m\0Map Files (*.dmap)\0*.dmap\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = "d2m";
	ofn.lpstrFile = FilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 510;
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		Path = FilePath;
	}
	return Path;
}

std::wstring Level1::WGetLoadFilePath()
{
	std::wstring Path;
	wchar_t FilePath[510];

	OPENFILENAMEW ofn = {};

	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = GameController::hWnd;
	ofn.lpstrFilter = L"Map Files (*.d2m)\0*.d2m\0Map Files (*.dmap)\0*.dmap\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"d2m";
	ofn.lpstrFile = FilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 510;
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameW(&ofn))
	{
		Path = FilePath;
	}

	return Path;
}
