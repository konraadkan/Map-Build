#pragma once
#include "AssetList.h"
#include "graphics.h"
#include "defines.h"
#include <deque>
#include <vector>

class InitiativeArea
{
public:
	D2D1_COLOR_F m_BgColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	D2D1_RECT_F m_DrawArea = { WINDOW_WIDTH * .075f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };
	long m_SelectedID = -1;
	std::deque<Asset*> m_InitOrder;
	void NextTurn();
	void PreviousTurn();
	void SwapTurn(int id1, int id2);
	void SwapTurn(D2D1_POINT_2F p);
	void InsertTurn(int id1, int id2);
	void InsertTurn(D2D1_POINT_2F p);
	void AddTurn(int id, std::vector<Asset>& assetList);
	void DrawInitArea(D2D1_RECT_F drawArea, std::deque<Asset*> assetList, Graphics* gfx, D2D1_COLOR_F color = { 1.0f, 1.0f, 1.0f, 1.0f }, D2D1_COLOR_F textColor = { 0.0f, 0.0f, 0.0f, 1.0f });
	void RemoveEntry(D2D1_POINT_2F p);
	void RemoveEntry(long menu_id);
	void EmtpyQueue(std::deque<Asset*>& q);
};