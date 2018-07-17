#include "InitOrder.h"
#include "defines.h"

void InitiativeArea::NextTurn()
{
	Asset* t = m_InitOrder.front();
	m_InitOrder.pop_front();
	m_InitOrder.push_back(t);
}

void InitiativeArea::PreviousTurn()
{
	Asset* t = m_InitOrder.back();
	m_InitOrder.pop_back();
	m_InitOrder.push_front(t);
}

void InitiativeArea::DrawInitArea(D2D1_RECT_F drawArea, std::deque<Asset*> assetList, Graphics* gfx, D2D1_COLOR_F color, D2D1_COLOR_F textColor)
{
	gfx->FillRect(m_MenuAreaRect, color);

	D2D1_RECT_F entryArea = { drawArea.left, drawArea.top, drawArea.right, drawArea.top + 128.0f };

	while (!assetList.empty())
	{
		Asset* asset = assetList.front();
		assetList.pop_front();

		if (asset->m_Icon)
		{
			asset->m_Icon->Draw({ entryArea.left, entryArea.top, entryArea.left + 128.0f, entryArea.top + 128.0f }, asset->m_Icon->GetFrame());
		}
		std::wstring text(asset->m_Name.length(), L' ');
		std::copy(asset->m_Name.begin(), asset->m_Name.end(), text.begin());
		if (asset->m_Status)
		{
			if (asset->m_Status != L'╥')
			{
				text.push_back(L'\n');
				for (int i = 0; i < 10; i++)
					text.push_back(asset->m_Status);
				D2D1_COLOR_F cColor = { 1.0f, 1.0f, 1.0f, 0.0f };
				switch (asset->m_Status)
				{
				case L'↑':
					cColor = { 0.133f, 0.541f, 0.133f, 0.66f };
					break;
				case L'↓':
					cColor = { 1.0f, 0.078f, 0.576f, 0.66f };
					break;
				}
				gfx->FillRect({ entryArea.left, entryArea.top, entryArea.right, entryArea.top + 128.0f }, cColor);
			}
			else
			{
				gfx->FillRect({ entryArea.left, entryArea.top, entryArea.right, entryArea.top + 128.0f }, { .5f, .5f, .5f, .66f });
				text.append(L"\n(×﹏×)");
			}
		}
		gfx->DrawText(text.c_str(), { entryArea.left + 128.0f, entryArea.top, entryArea.right, entryArea.top + 128.0f }, textColor, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		if (m_SelectedID == asset->m_MenuID)
		{
			gfx->FillRect({ entryArea.left, entryArea.top, entryArea.right, entryArea.top + 128.0f }, { 0.0f, 5.0f, 0.0f, 0.5f });
		}
		entryArea.top += 128.0f;
	}
}

void InitiativeArea::InsertTurn(int id1, int id2)
{
	Asset* p1;
	Asset* p2;
	std::deque<Asset*> temp = m_InitOrder;
	while (!temp.empty())
	{
		if (temp.front()->m_MenuID == id1)
			p1 = temp.front();
		if (temp.front()->m_MenuID == id2)
			p2 = temp.front();
		temp.pop_front();
	}
	while (!m_InitOrder.empty())
	{
		if (m_InitOrder.front()->m_MenuID == id1)
			temp.push_back(p2);
		else if (m_InitOrder.front()->m_MenuID == id2)
		{
			m_InitOrder.pop_front();
			continue;
		}
		temp.push_back(m_InitOrder.front());
		m_InitOrder.pop_front();
	}
	m_InitOrder = temp;
}

void InitiativeArea::InsertTurn(D2D1_POINT_2F p)
{
	if (m_SelectedID >= 0)
	{

		int menu_id = (int)((p.y - m_DrawArea.top) / 128.0f);
		std::deque<Asset*> temp = m_InitOrder;

		if (menu_id < (int)temp.size())
		{
			for (int i = 0; i < menu_id; i++)
				temp.pop_front();
			InsertTurn(temp.front()->m_MenuID, m_SelectedID);
		}
		m_SelectedID = -1;
	}
	else
	{
		int menu_id = (int)((p.y - m_DrawArea.top) / 128.0f);
		std::deque<Asset*> temp = m_InitOrder;

		if (menu_id < (int)temp.size())
		{
			for (int i = 0; i < menu_id; i++)
				temp.pop_front();
			m_SelectedID = temp.front()->m_MenuID;
		}
	}
}

void InitiativeArea::SwapTurn(D2D1_POINT_2F p)
{
	if (m_SelectedID >= 0)
	{
		int menu_id = (int)((p.y - m_DrawArea.top) / 128.0f);
		std::deque<Asset*> temp = m_InitOrder;

		if (menu_id < (int)temp.size())
		{
			for (int i = 0; i < menu_id; i++)
				temp.pop_front();
			SwapTurn(temp.front()->m_MenuID, m_SelectedID);
		}
		m_SelectedID = -1;
	}
	else
	{
		int menu_id = (int)((p.y - m_DrawArea.top) / 128.0f);
		std::deque<Asset*> temp = m_InitOrder;
		
		if (menu_id < (int)temp.size())
		{
			for (int i = 0; i < menu_id; i++)
				temp.pop_front();
			m_SelectedID = temp.front()->m_MenuID;
		}
	}
}

void InitiativeArea::SwapTurn(int id1, int id2)
{
	Asset* p1;
	Asset* p2;
	std::deque<Asset*> temp = m_InitOrder;
	while (!temp.empty())
	{
		if (temp.front()->m_MenuID == id1)
			p1 = temp.front();
		if (temp.front()->m_MenuID == id2)
			p2 = temp.front();
		temp.pop_front();
	}
	while (!m_InitOrder.empty())
	{
		if (m_InitOrder.front()->m_MenuID == id1)
		{
			temp.push_back(p2);
		}
		else if (m_InitOrder.front()->m_MenuID == id2)
		{
			temp.push_back(p1);
		}
		else
			temp.push_back(m_InitOrder.front());
		m_InitOrder.pop_front();
	}
	m_InitOrder = temp;
}

void InitiativeArea::AddTurn(int id, std::vector<Asset>& assetList)
{
	std::deque<Asset*> temp = m_InitOrder;
	while (!temp.empty())
	{
		if (temp.front()->m_MenuID == id)
			return;
		temp.pop_front();
	}

	for (int i = 0; i < (int)assetList.size(); i++)
	{
		if (assetList[i].m_MenuID == id)
		{
			m_InitOrder.push_back(&assetList[i]);
			break;
		}
	}
}


void InitiativeArea::RemoveEntry(D2D1_POINT_2F p)
{
	std::deque<Asset*> temp = m_InitOrder;

	int y = (int)((p.y - m_DrawArea.top) / 128.0f);

	if (y < (int)temp.size())
	{
		for (int i = 0; i < y; i++)
			temp.pop_front();
		RemoveEntry(temp.front()->m_MenuID);
	}
}

void InitiativeArea::RemoveEntry(long menu_id)
{
	std::deque<Asset*> temp = m_InitOrder;
	std::deque<Asset*> newOrder;

	while (!temp.empty())
	{
		if (temp.front()->m_MenuID != menu_id)
			newOrder.push_back(temp.front());
		
		temp.pop_front();
	}
	m_InitOrder = newOrder;
}

void InitiativeArea::EmtpyQueue(std::deque<Asset*>& q)
{
	std::deque<Asset*> empty;
	std::swap(q, empty);
}