#include "ItemMenuArea.h"

Graphics* ItemMenuArea::gfx;

ItemMenuArea::~ItemMenuArea()
{
	EmptyItemMenuQueue(m_ItemTypes);
	EmptyItemMenuQueue(m_SubMenus);
	EmptyItemMenuQueue(m_Sprites);
}

void ItemMenuArea::Cleanup()
{
	m_SelectedType = NULL;
	m_SelectedSubMenu = NULL;
	m_SelectedItem = NULL;
	m_SelectedSizeCategory = NULL;
	EmptyItemMenuQueue(m_ItemTypes);
	EmptyItemMenuQueue(m_SubMenus);
	EmptyItemMenuQueue(m_Sprites);
	EmptyItemMenuQueue(m_SizeCategory);
	EmptyItemMenuQueue(m_Radius);
	EmptyItemMenuQueue(m_Width);
	EmptyItemMenuQueue(m_Thickness);
	EmptyItemMenuQueue(m_Colors);
	EmptyItemMenuQueue(m_AdditionalOptions);
}

void ItemMenuArea::ClearItemMenuArea()
{
	gfx->FillRect(m_MenuAreaRect, m_ItemMenuColor);
}

void ItemMenuArea::DrawItemMenuArea()
{
	COLORREF_F bColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	ClearItemMenuArea();
	if (m_AdditionalOptions.size())
	{
		DrawItemMenuArea(m_AdditionalOptions);
		gfx->DrawLine({ m_MenuAreaRect.left, m_AdditionalOptions.back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_AdditionalOptions.back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		if (m_LockToGrid)
		{
			std::queue<ItemMenuItem> t = m_AdditionalOptions;
			while (!t.empty())
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Lock to Grid"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
		if (m_GridOnTop)
		{
			std::queue<ItemMenuItem> t = m_AdditionalOptions;
			while (!t.empty())
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Grid On Top"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
		if (m_ShowBgColor)
		{
			std::queue<ItemMenuItem> t = m_AdditionalOptions;
			while (!t.empty())
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Toggle PC Colors"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
		if (m_KeepRatio)
		{
			std::queue<ItemMenuItem> t = m_AdditionalOptions;
			while (!t.empty())
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Toggle Keep Aspect"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
		if (m_ShowCounter)
		{
			std::queue<ItemMenuItem> t = m_AdditionalOptions;
			while (!t.empty())
			{
				if (!_wcsicmp(t.front().m_ItemText.c_str(), L"Turn Counter"))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
	}
	if (m_ItemTypes.size())
	{
		DrawItemMenuArea(m_ItemTypes);
		gfx->DrawLine({ m_MenuAreaRect.left, m_ItemTypes.back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_ItemTypes.back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		if (m_SelectedType)
		{
			std::queue<ItemMenuItem> t = m_ItemTypes;
			while(!t.empty())
			{
				if (StringEqual(m_SelectedType->m_Type, t.front().m_ItemText))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
	}
	if (m_SubMenus.size())
	{
		DrawItemMenuArea(m_SubMenus);
		gfx->DrawLine({ m_MenuAreaRect.left, m_SubMenus.back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_SubMenus.back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		if (m_SelectedSubMenu)
		{
			std::queue<ItemMenuItem> t = m_SubMenus;
			while (!t.empty())
			{
				if (StringEqual(m_SelectedSubMenu->m_Submenu, t.front().m_ItemText))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
					break;
				}
				t.pop();
			}
		}
	}
	if (m_SelectedSizeCategory->size())
	{
		DrawItemMenuArea(*m_SelectedSizeCategory);
		gfx->DrawLine({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_SelectedSizeCategory->back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
		if (m_SelectedSizeArea.left != m_SelectedSizeArea.right)
		{
			gfx->FillRect(m_SelectedSizeArea, { 0.0f, 0.5f, 0.0f, 0.5f });
		}
	}
	if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
	{
		if (m_Width.size())
		{
			DrawItemMenuArea(m_Width);
			gfx->DrawLine({ m_MenuAreaRect.left, m_Width.back().m_DrawArea.bottom + 6.0f }, { WINDOW_WIDTH, m_Width.back().m_DrawArea.bottom + 6.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 8.0f);
			if (m_SelectedWidthArea.left != m_SelectedWidthArea.right)
				gfx->FillRect(m_SelectedWidthArea, { 0.0f, 0.5f, 0.0f, 0.5f });
		}
	}
	if (m_Sprites.size())
	{
		DrawItemMenuArea(m_Sprites);
		if (m_SelectedItem)
		{
			std::queue<ItemMenuItem> t = m_Sprites;
			while (!t.empty())
			{
				if (!t.front().m_Sprite)
				{
					gfx->FillRect(t.front().m_DrawArea, t.front().m_BgColor);
				}
				if (StringEqual(m_SelectedItem->m_Name, t.front().m_ItemText))
				{
					gfx->FillRect(t.front().m_DrawArea, { 0.0f, 0.5f, 0.0f, 0.5f });
				}
				t.pop();
			}
		}
	}
	m_SelectedType;
	m_SelectedSubMenu;
	m_SelectedItem;
}

void ItemMenuArea::EmptyItemMenuQueue(std::queue<ItemMenuItem>& q)
{
	std::queue<ItemMenuItem> empty;
	std::swap(q, empty);
}

void ItemMenuArea::DrawItemMenuArea(std::queue<ItemMenuItem> Type)
{
	COLORREF_F bColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	while (!Type.empty())
	{
		ItemMenuItem item = Type.front();
		Type.pop();
		item.DrawSprite({ item.m_DrawArea.left, item.m_DrawArea.top });
		gfx->DrawRect(item.m_DrawArea, bColor, 0.8f);
		if (item.m_Sprite)
			gfx->DrawTextSmall(item.m_ItemText.c_str(), item.m_DrawArea, bColor, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		else
			gfx->DrawTextSmall(item.m_ItemText.c_str(), item.m_DrawArea, bColor, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	}
}

void ItemMenuArea::AddMenuItem(std::string text)
{
	std::wstring t(text.length(), L' ');
	std::copy(text.begin(), text.end(), t.begin());
	AddMenuItem(t);
}

void ItemMenuArea::AddMenuItem(std::string text, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	std::wstring t(text.length(), L' ');
	std::copy(text.begin(), text.end(), t.begin());
	AddMenuItem(t, startPoint, type);
}

void ItemMenuArea::AddMenuItem(SpriteSheet* sprite, std::string text, std::string submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	std::wstring t(text.length(), L' ');
	std::copy(text.begin(), text.end(), t.begin());
	std::wstring wsubmenu(submenu.length(), L' ');
	std::copy(submenu.begin(), submenu.end(), wsubmenu.begin());
	AddMenuItem(sprite, t, wsubmenu, startPoint, type);
}

void ItemMenuArea::AddMenuItem(std::wstring text)
{
	ItemMenuItem item;
	FRECT targetArea = {};
	long maxPerRow = (long)((WINDOW_WIDTH * 0.25f) / 96.0f);
	
	targetArea.left = m_MenuAreaRect.left + 0.2f + 96.0f * (m_ItemTypes.size() % maxPerRow);
	targetArea.top = 0.2f + 32.0f * (int)(m_ItemTypes.size() / maxPerRow);
	targetArea.right = targetArea.left + 95.8f;
	targetArea.bottom = targetArea.top + 32.0f;
	
	item.m_DrawArea = targetArea;
	item.m_ItemText = text;
	m_ItemTypes.push(item);
}

void ItemMenuArea::AddMenuItem(std::wstring text, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	ItemMenuItem item;
	item.m_StartPoint = startPoint;
	FRECT targetArea = {};
	long maxPerRow = (long)((WINDOW_WIDTH - item.m_StartPoint.x) / 96.0f);

	targetArea.left = item.m_StartPoint.x + 0.2f + 96.0f * (type.size() % maxPerRow);
	targetArea.top = item.m_StartPoint.y + 32.0f * (int)(type.size() / maxPerRow);
	targetArea.right = targetArea.left + 95.8f;
	targetArea.bottom = targetArea.top + 32.0f;

	item.m_DrawArea = targetArea;
	item.m_ItemText = text;
	type.push(item);
}

void ItemMenuArea::AddMenuItem(SpriteSheet* sprite, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	ItemMenuItem item; 
	item.m_StartPoint = startPoint;
	FRECT targetArea = {};
	long maxPerRow = (long)((WINDOW_WIDTH - item.m_StartPoint.x) / 64.0f);

	targetArea.left = item.m_StartPoint.x + 0.2f + 64.0f * (type.size() % maxPerRow);
	targetArea.top = item.m_StartPoint.y + 64.0f * (int)(type.size() / maxPerRow);
	targetArea.right = targetArea.left + 63.8f;
	targetArea.bottom = targetArea.top + 64.0f;

	item.m_DrawArea = targetArea;
	item.SetSprite(sprite);

	type.push(item);
}

void ItemMenuArea::AddMenuItem(D2D1_COLOR_F color, std::string text, std::string submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	std::wstring s1(text.length(), L' ');
	std::wstring s2(submenu.length(), L' ');
	std::copy(text.begin(), text.end(), s1.begin());
	std::copy(submenu.begin(), submenu.end(), s2.begin());
	AddMenuItem(color, s1, s2, startPoint, type);
}

void ItemMenuArea::AddMenuItem(D2D1_COLOR_F color, std::wstring text, std::wstring submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	ItemMenuItem item;
	item.m_StartPoint = startPoint;
	FRECT targetArea = {};
	long maxPerRow = (long)((WINDOW_WIDTH - item.m_StartPoint.x) / 64.0f);

	targetArea.left = item.m_StartPoint.x + 0.2f + 96.0f * (type.size() % maxPerRow);
	targetArea.top = item.m_StartPoint.y + 96.0f * (int)(type.size() / maxPerRow);
	targetArea.right = targetArea.left + 95.8f;
	targetArea.bottom = targetArea.top + 96.0f;

	item.m_DrawArea = targetArea;
	item.m_ItemText = text;
	item.m_SubMenu = submenu;
	item.m_BgColor = color;

	type.push(item);
}

void ItemMenuArea::AddMenuItem(SpriteSheet* sprite, std::wstring text, std::wstring submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type)
{
	ItemMenuItem item;
	item.m_StartPoint = startPoint;
	FRECT targetArea = {};
	long maxPerRow = (long)((WINDOW_WIDTH - item.m_StartPoint.x) / 64.0f);

	targetArea.left = item.m_StartPoint.x + 0.2f + 96.0f * (type.size() % maxPerRow);
	targetArea.top = item.m_StartPoint.y + 96.0f * (int)(type.size() / maxPerRow);
	targetArea.right = targetArea.left + 95.8f;
	targetArea.bottom = targetArea.top + 96.0f;

	item.m_DrawArea = targetArea;
	item.m_ItemText = text;
	item.m_SubMenu = submenu;
	item.SetSprite(sprite);

	type.push(item);
}

bool ItemMenuArea::WStringEqual(std::wstring s1, std::wstring s2)
{
	if (!_wcsicmp(s1.c_str(), s2.c_str())) return true;
	return false;
}

bool ItemMenuArea::StringEqual(std::string s1, std::string s2)
{
	if (!_stricmp(s1.c_str(), s2.c_str())) return true;
	return false;
}

bool ItemMenuArea::StringEqual(std::string s1, std::wstring s2)
{
	std::wstring ws(s1.length(), L' ');
	std::copy(s1.begin(), s1.end(), ws.begin());

	return WStringEqual(s2, ws);
}

size_t ItemMenuArea::EntryExists(std::string s, std::string submenu, std::queue<ItemMenuItem> list)
{
	while (!list.empty())
	{
		std::wstring ws(s.length(), L' ');
		std::copy(s.begin(), s.end(), ws.begin());
		std::wstring wsubmenu(submenu.length(), L' ');
		std::copy(submenu.begin(), submenu.end(), wsubmenu.begin());

		if (WStringEqual(wsubmenu, list.front().m_SubMenu))
		{
			if (WStringEqual(ws, list.front().m_ItemText))
				return list.size();
		}
		list.pop();
	}
	return list.size();
}

size_t ItemMenuArea::EntryExists(std::wstring s, std::wstring submenu, std::queue<ItemMenuItem> list)
{
	while (!list.empty())
	{
		if (WStringEqual(submenu, list.front().m_SubMenu))
		{
			if (WStringEqual(s, list.front().m_ItemText))
				return list.size();
		}
		list.pop();
	}
	return list.size();
}

void ItemMenuArea::AdjustLocation(D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& list)
{
	std::queue<ItemMenuItem> lCopy = list;
	EmptyItemMenuQueue(list);
	long maxPerRow = (long)((WINDOW_WIDTH - startPoint.x) / 96.0f);

	while (!lCopy.empty())
	{
		ItemMenuItem i = lCopy.front();
		lCopy.pop();

		i.m_StartPoint = startPoint;

		if (i.m_Sprite)
		{
			i.m_DrawArea.left = i.m_StartPoint.x + 0.2f + 96.0f * (list.size() % maxPerRow);
			i.m_DrawArea.top = i.m_StartPoint.y + 10.0f + 96.0f * (int)(list.size() / maxPerRow);
			i.m_DrawArea.right = i.m_DrawArea.left + 95.8f;
			i.m_DrawArea.bottom = i.m_DrawArea.top + 96.0f;
		}
		else
		{
			i.m_DrawArea.left = i.m_StartPoint.x + 0.2f + 96.0f * (list.size() % maxPerRow);
			i.m_DrawArea.top = i.m_StartPoint.y + 10.0f + 32.0f * (int)(list.size() / maxPerRow);
			i.m_DrawArea.right = i.m_DrawArea.left + 95.8f;
			i.m_DrawArea.bottom = i.m_DrawArea.top + 32.0f;
		}

		list.push(i);
	}
}

void ItemMenuArea::BuildMenuItem(std::vector<Asset>& assetList)
{
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		std::string empty;
		if (!EntryExists(assetList[i].m_Type, empty, m_ItemTypes))
		{
			if (!i) m_SelectedType = &assetList[i];
			AddMenuItem(assetList[i].m_Type, { m_MenuAreaRect.left, 0.2f }, m_ItemTypes);
		}
		if (!EntryExists(assetList[i].m_Submenu, empty, m_SubMenus))
		{
			if (!i) m_SelectedSubMenu = &assetList[i];
			AddMenuItem(assetList[i].m_Submenu, {}, m_SubMenus);
		}
		if (!EntryExists(assetList[i].m_Name, assetList[i].m_Submenu, m_Sprites))
		{
			if (!i) m_SelectedItem = &assetList[i];
			if (assetList[i].m_Sprite)
				AddMenuItem(assetList[i].m_Sprite, assetList[i].m_Name, assetList[i].m_Submenu, {}, m_Sprites);
			else
				AddMenuItem(assetList[i].m_BgColor, assetList[i].m_Name, assetList[i].m_Submenu, {}, m_Sprites);
		}
	}
	AddMenuItem(L"Fine", {}, m_SizeCategory);
	AddMenuItem(L"Diminutive", {}, m_SizeCategory);
	AddMenuItem(L"Tiny", {}, m_SizeCategory);
	AddMenuItem(L"Small", {}, m_SizeCategory);
	AddMenuItem(L"Medium", {}, m_SizeCategory);
	AddMenuItem(L"Large", {}, m_SizeCategory);
	AddMenuItem(L"Huge", {}, m_SizeCategory);
	AddMenuItem(L"Gargantuan", {}, m_SizeCategory);
	AddMenuItem(L"Colossal", {}, m_SizeCategory);
	AddMenuItem(L"Original Size", {}, m_SizeCategory);
	AddMenuItem(L"2x Size", {}, m_SizeCategory);
	AddMenuItem(L"3x Size", {}, m_SizeCategory);
	AddMenuItem(L"4x Size", {}, m_SizeCategory);
	AddMenuItem(L"Half Size", {}, m_SizeCategory);

	AddMenuItem(L"Lock To Grid", {}, m_AdditionalOptions);
	AddMenuItem(L"Grid On Top", {}, m_AdditionalOptions);
	AddMenuItem(L"Toggle PC Colors", {}, m_AdditionalOptions);
	AddMenuItem(L"Add Custom Color", {}, m_AdditionalOptions);
	AddMenuItem(L"Toggle Initiative", {}, m_AdditionalOptions);
	AddMenuItem(L"Toggle Keep Aspect", {}, m_AdditionalOptions);
	AddMenuItem(L"Turn Counter", {}, m_AdditionalOptions);
	AddMenuItem(L"Square View Mode", {}, m_AdditionalOptions);
	AddMenuItem(L"Hexagon View Mode", {}, m_AdditionalOptions);
	AddMenuItem(L"Dimetrique View Mode", {}, m_AdditionalOptions);

	for (int i = 5; i <= 100; i += 5)
	{
		std::wstring rad = std::to_wstring(i);
		rad.append(L" Ft");
		AddMenuItem(rad.c_str(), {}, m_Radius);
		AddMenuItem(rad.c_str(), {}, m_Width);
	}

	for (double i = 5.0; i <= 10.0; i += 0.2)
	{
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(2) << i;
		std::wstring thick = ss.str();
		AddMenuItem(thick.c_str(), {}, m_Thickness);
	}

	for (double i = 1; i <= 40.0; i++)
	{
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(2) << i;
		std::wstring psize = ss.str();
		AddMenuItem(psize.c_str(), {}, m_PenSize);
	}
	
	AdjustLocation({ m_MenuAreaRect.left, 0.0f }, m_AdditionalOptions);
	AdjustLocation({ m_MenuAreaRect.left, m_AdditionalOptions.back().m_DrawArea.bottom + 3.0f }, m_ItemTypes);
	AdjustLocation({ m_MenuAreaRect.left, m_ItemTypes.back().m_DrawArea.bottom + 3.0f }, m_SubMenus);
	AdjustLocation({ m_MenuAreaRect.left, m_SubMenus.back().m_DrawArea.bottom + 3.0f }, *m_SelectedSizeCategory);
	AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Width);
	SetSelectedSize(m_SizeCategory, Size::m_Medium);
	SetSelectedWidth(m_Width, 5);
	AdjustLocation({ m_MenuAreaRect.left, m_SizeCategory.back().m_DrawArea.bottom + 3.0f }, m_Sprites);
	RebuildSubMenuList(assetList);
	RebuildItemMenuList(assetList);
}

void ItemMenuArea::SetSelectedRadius(std::queue<ItemMenuItem> radiuslist, int radius)
{
	if (radius < 5) radius = 5;
	m_SelectedRadius = radius;
	while (!radiuslist.empty())
	{
		std::wstring t = std::to_wstring(radius);
		t.append(L" Ft");
		if (!_wcsicmp(radiuslist.front().m_ItemText.c_str(), t.c_str()))
		{
			m_SelectedSizeArea = radiuslist.front().m_DrawArea;
			break;
		}
		radiuslist.pop();
	}
}

void ItemMenuArea::SetSelectedWidth(std::queue<ItemMenuItem> widthlist, int width)
{
	if (width < 5) width = 5;
	m_SelectedWidth = width;
	while (!widthlist.empty())
	{
		std::wstring t = std::to_wstring(width);
		t.append(L" Ft");
		if (!_wcsicmp(widthlist.front().m_ItemText.c_str(), t.c_str()))
		{
			m_SelectedWidthArea = widthlist.front().m_DrawArea;
			break;
		}
		widthlist.pop();
	}
}

void ItemMenuArea::SetSelectedThickness(std::queue<ItemMenuItem> thicknesslist, double thickness)
{
	m_SelectedThickness = (float)thickness;
	while(!thicknesslist.empty())
	{
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(2) << thickness;
		std::wstring w = ss.str();
		if (!_wcsicmp(thicknesslist.front().m_ItemText.c_str(), w.c_str()))
		{
			m_SelectedSizeArea = thicknesslist.front().m_DrawArea;
			break;
		}
		thicknesslist.pop();
	}
}

void ItemMenuArea::SetSelectedPenSize(std::queue<ItemMenuItem> pensizelist, double pensize)
{
	m_SelectedPenSize = pensize;
	while (!pensizelist.empty())
	{
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(2) << pensize;
		std::wstring w = ss.str();
		if (!_wcsicmp(pensizelist.front().m_ItemText.c_str(), w.c_str()))
		{
			m_SelectedSizeArea = pensizelist.front().m_DrawArea;
			break;
		}
		pensizelist.pop();
	}
}

void ItemMenuArea::SetSelectedSize(std::queue<ItemMenuItem> sizelist, Size size)
{
	m_SelectedSize = size;
	while (!sizelist.empty())
	{
		switch (size)
		{
		case Size::m_Fine:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Fine"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Diminutive:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Diminutive"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Tiny:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Tiny"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Small:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Small"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Medium:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Medium"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Large:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Large"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Huge:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Huge"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Gargantuan:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Gargantuan"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Colossal:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Colossal"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Original:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Original Size"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_TwoTimes:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"2x Size"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_ThreeTimes:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"3x Size"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_FourTimes:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"4x Size"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		case Size::m_Half:
			if (!_wcsicmp(sizelist.front().m_ItemText.c_str(), L"Half Size"))
			{
				m_SelectedSizeArea = sizelist.front().m_DrawArea;
			}
			break;
		}
		sizelist.pop();
	}
}

void ItemMenuArea::RebuildTypeMenuList(std::vector<Asset>& assetList)
{
	Asset temp = {};
	if (m_SelectedType)
		temp.m_Type = m_SelectedType->m_Type;

	EmptyItemMenuQueue(m_ItemTypes);
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		if (!EntryExists(assetList[i].m_Type, NULL, m_ItemTypes))
		{
			AddMenuItem(assetList[i].m_Type, { m_MenuAreaRect.left, 0.2f }, m_ItemTypes);
			if (StringEqual(temp.m_Type, assetList[i].m_Type))
				m_SelectedType = &assetList[i];
		}
	}

	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedType->m_Submenu.c_str(), "Walls"))
			m_SelectedSizeCategory = &m_Thickness;
		else
			m_SelectedSizeCategory = &m_Radius;
	}
	else
		m_SelectedSizeCategory = &m_SizeCategory;

	AdjustLocation({ m_MenuAreaRect.left, m_ItemTypes.back().m_DrawArea.bottom + 3.0f }, m_SubMenus);
	AdjustLocation({ m_MenuAreaRect.left, m_SubMenus.back().m_DrawArea.bottom + 3.0f }, *m_SelectedSizeCategory);
	AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Width);
	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
		{
			SetSelectedThickness(m_Thickness, m_SelectedThickness);
		}
		else
		{
			SetSelectedRadius(m_Radius, m_SelectedRadius);
			SetSelectedWidth(m_Width, m_SelectedWidth);
		}
	}
	else
		SetSelectedSize(m_SizeCategory, m_SelectedSize);
}

void ItemMenuArea::RebuildSubMenuList(std::vector<Asset>& assetList)
{
	Asset temp = {};
	if (m_SelectedType)
		temp.m_Type = m_SelectedType->m_Type;
	std::string empty;
	temp.m_Sprite = NULL;
	temp.m_Icon = NULL;
	EmptyItemMenuQueue(m_SubMenus);
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		if (!EntryExists(assetList[i].m_Submenu, empty, m_SubMenus))
		{
			if (StringEqual(temp.m_Type, assetList[i].m_Type))
			{
				AddMenuItem(assetList[i].m_Submenu, {}, m_SubMenus);
				if (StringEqual(temp.m_Submenu, assetList[i].m_Submenu))
					m_SelectedSubMenu = &assetList[i];
			}
		}
	}
	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedType->m_Submenu.c_str(), "Walls"))
			m_SelectedSizeCategory = &m_Thickness;
		else
			m_SelectedSizeCategory = &m_Radius;		
	}
	else if (!_stricmp(m_SelectedType->m_Type.c_str(), "Free Draw"))
	{
		m_SelectedSizeCategory = &m_PenSize;
	}
	else
		m_SelectedSizeCategory = &m_SizeCategory;
	AdjustLocation({ m_MenuAreaRect.left, m_ItemTypes.back().m_DrawArea.bottom + 3.0f }, m_SubMenus);
	AdjustLocation({ m_MenuAreaRect.left, m_SubMenus.back().m_DrawArea.bottom + 3.0f }, *m_SelectedSizeCategory);
	AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Width);
	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
		{
			SetSelectedThickness(m_Thickness, m_SelectedThickness);
		}
		else
		{
			SetSelectedRadius(m_Radius, m_SelectedRadius);
			SetSelectedWidth(m_Width, m_SelectedWidth);
		}
	}
	else if (!_stricmp(m_SelectedType->m_Type.c_str(), "Free Draw"))
	{
		SetSelectedPenSize(m_PenSize, m_SelectedPenSize);
	}
	else
		SetSelectedSize(m_SizeCategory, m_SelectedSize);
	
	if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
	{
		AdjustLocation({ m_MenuAreaRect.left, m_Width.back().m_DrawArea.bottom + 3.0f }, m_Sprites);
	}
	else
		AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Sprites);
}

void ItemMenuArea::RebuildItemMenuList(std::vector<Asset>& assetList)
{
	Asset temp = {};
	if (m_SelectedType)
		temp.m_Type = m_SelectedType->m_Type;
	if (m_SelectedSubMenu)
		temp.m_Submenu = m_SelectedSubMenu->m_Submenu;
	EmptyItemMenuQueue(m_Sprites);
	for (int i = 0; i < (int)assetList.size(); i++)
	{
		if (!EntryExists(assetList[i].m_Name, assetList[i].m_Submenu, m_Sprites))
		{
			if (StringEqual(temp.m_Submenu, assetList[i].m_Submenu) && StringEqual(temp.m_Type, assetList[i].m_Type))
			{
				if(assetList[i].m_Sprite)
					AddMenuItem(assetList[i].m_Sprite, assetList[i].m_Name, assetList[i].m_Submenu, {}, m_Sprites);
				else
					AddMenuItem(assetList[i].m_BgColor, assetList[i].m_Name, assetList[i].m_Submenu, {}, m_Sprites);
				if (StringEqual(temp.m_Name, assetList[i].m_Name))
					m_SelectedItem = &assetList[i];
			}
		}
	}
	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
			m_SelectedSizeCategory = &m_Thickness;
		else
			m_SelectedSizeCategory = &m_Radius;
	}
	else if (!_stricmp(m_SelectedType->m_Type.c_str(), "Free Draw"))
	{
		m_SelectedSizeCategory = &m_PenSize;
	}
	else
		m_SelectedSizeCategory = &m_SizeCategory;
	AdjustLocation({ m_MenuAreaRect.left, m_SubMenus.back().m_DrawArea.bottom + 3.0f }, *m_SelectedSizeCategory);
	AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Width);
	if (!_stricmp(m_SelectedType->m_Type.c_str(), "Shapes"))
	{
		if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Walls"))
		{
			SetSelectedThickness(m_Thickness, m_SelectedThickness);
		}
		else
		{
			SetSelectedRadius(m_Radius, m_SelectedRadius);
			SetSelectedWidth(m_Width, m_SelectedWidth);
		}
	}
	else if (!_stricmp(m_SelectedType->m_Type.c_str(), "Free Draw"))
	{
		SetSelectedPenSize(m_PenSize, m_SelectedPenSize);
	}
	else
		SetSelectedSize(m_SizeCategory, m_SelectedSize);

	if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
	{
		AdjustLocation({ m_MenuAreaRect.left, m_Width.back().m_DrawArea.bottom + 3.0f }, m_Sprites);
	}
	else
		AdjustLocation({ m_MenuAreaRect.left, m_SelectedSizeCategory->back().m_DrawArea.bottom + 3.0f }, m_Sprites);
}

D2D1_RECT_F ItemMenuArea::HoverArea(D2D1_POINT_2F p)
{
	std::queue<ItemMenuItem> temp = m_AdditionalOptions;
	
	while (!temp.empty())
	{
		if (p.y > temp.back().m_DrawArea.bottom)
			break;

		D2D1_RECT_F tArea = temp.front().m_DrawArea;
		if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
			return tArea;

		temp.pop();
	}

	temp = m_ItemTypes;
	while (!temp.empty())
	{
		if (p.y > temp.back().m_DrawArea.bottom)
			break;

		D2D1_RECT_F tArea = temp.front().m_DrawArea;
		if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
			return tArea;

		temp.pop();
	}
	if(temp.size())
		EmptyItemMenuQueue(temp);

	temp = m_SubMenus;
	while (!temp.empty())
	{
		if (p.y > temp.back().m_DrawArea.bottom)
			break;

		D2D1_RECT_F tArea = temp.front().m_DrawArea;
		if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
			return tArea;

		temp.pop();
	}
	if(temp.size())
		EmptyItemMenuQueue(temp);

	temp = *m_SelectedSizeCategory;
	while (!temp.empty())
	{
		if (p.y > temp.back().m_DrawArea.bottom)
			break;

		D2D1_RECT_F tArea = temp.front().m_DrawArea;
		if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
			return tArea;

		temp.pop();
	}

	if (!_stricmp(m_SelectedSubMenu->m_Submenu.c_str(), "Line"))
	{
		temp = m_Width;
		while (!temp.empty())
		{
			if (p.y > temp.back().m_DrawArea.bottom)
				break;
			D2D1_RECT_F tArea = temp.front().m_DrawArea;
			if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
				return tArea;

			temp.pop();
		}
	}

	temp = m_Sprites;
	while (!temp.empty())
	{
		if (p.y > temp.back().m_DrawArea.bottom)
			break;

		D2D1_RECT_F tArea = temp.front().m_DrawArea;
		if (p.x > tArea.left && p.x < tArea.right && p.y > tArea.top && p.y < tArea.bottom)
			return tArea;

		temp.pop();
	}
	return {};
}