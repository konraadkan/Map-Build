#pragma once
#include "graphics.h"
#include "ItemMenuItem.h"
#include "SpriteSheet.h"
#include "AssetList.h"
#include "defines.h"
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class ItemMenuArea
{
	static Graphics* gfx;
	COLORREF_F m_ItemMenuColor = { 1.0f, 1.0f, 1.0f, 1.0f };
public:
	ItemMenuArea() {}
	~ItemMenuArea();

	static void Init(Graphics* graphics) { gfx = graphics; }
	void Cleanup();
	void ClearItemMenuArea();
	void DrawItemMenuArea();
	void DrawItemMenuArea(std::queue<ItemMenuItem> type);
	void AddMenuItem(std::wstring text);
	void AddMenuItem(std::wstring text, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(std::string text);
	void AddMenuItem(std::string text, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(SpriteSheet* sprite, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(SpriteSheet* sprite, std::wstring text, std::wstring submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(SpriteSheet* sprite, std::string text, std::string submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(D2D1_COLOR_F color, std::wstring text, std::wstring submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void AddMenuItem(D2D1_COLOR_F color, std::string text, std::string submenu, D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& type);
	void EmptyItemMenuQueue(std::queue<ItemMenuItem>& q);
	void BuildMenuItem(std::vector<Asset>& assetList);
	void RebuildTypeMenuList(std::vector<Asset>& assetList);
	void RebuildSubMenuList(std::vector<Asset>& assetList);
	void RebuildItemMenuList(std::vector<Asset>& assetList);
	size_t EntryExists(std::string s, std::string submenu, std::queue<ItemMenuItem> list);
	size_t EntryExists(std::wstring s, std::wstring submenu, std::queue<ItemMenuItem> list);
	bool WStringEqual(std::wstring s1, std::wstring s2);
	bool StringEqual(std::string s1, std::string s2);
	bool StringEqual(std::string s1, std::wstring s2);
	void AdjustLocation(D2D1_POINT_2F startPoint, std::queue<ItemMenuItem>& list);
	void SetSelectedSize(std::queue<ItemMenuItem> sizelist, Size size);
	void SetSelectedRadius(std::queue<ItemMenuItem> radiuslist, int radius);
	void SetSelectedThickness(std::queue<ItemMenuItem> thicknesslist, double thickness);
	void SetSelectedWidth(std::queue<ItemMenuItem> widthlist, int width);
	void SetSelectedPenSize(std::queue<ItemMenuItem> pensizelist, double pensize);

	D2D1_RECT_F HoverArea(D2D1_POINT_2F p);

	Asset* m_SelectedType = NULL;
	Asset* m_SelectedSubMenu = NULL;
	Asset* m_SelectedItem = NULL;
	D2D1_RECT_F m_SelectedSizeArea = {};
	D2D1_RECT_F m_SelectedWidthArea = {};
	Size m_SelectedSize = Size::m_Medium;
	int m_SelectedRadius = 5;
	int m_SelectedWidth = 5;
	float m_SelectedThickness = 5.0f;
	double m_SelectedPenSize = 2.0;
public:
	std::queue<ItemMenuItem> m_AdditionalOptions;
	std::queue<ItemMenuItem> m_ItemTypes;
	std::queue<ItemMenuItem> m_SubMenus;
	std::queue<ItemMenuItem> m_Sprites;
	std::queue<ItemMenuItem> m_SizeCategory;
	std::queue<ItemMenuItem> m_Radius;
	std::queue<ItemMenuItem> m_Width;
	std::queue<ItemMenuItem> m_Thickness;
	std::queue<ItemMenuItem> m_Colors;
	std::queue<ItemMenuItem> m_PenSize;
	std::queue<ItemMenuItem>* m_SelectedSizeCategory = &m_SizeCategory;
	bool m_LockToGrid = true;
	bool m_GridOnTop = false;
	bool m_ShowBgColor = true;
	bool m_KeepRatio = true;
	bool m_ShowCounter = false;
	long m_CounterCount = 0;
};