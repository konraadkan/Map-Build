#include "Room.h"

void Room::draw(D2D1_RECT_F DrawArea, float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx)
{
	if (!m_Visible)
		return;
	for (int i = 0; i < m_Layer.size(); i++)
	{
		if (m_Layer[i].m_Visible)
			m_Layer[i].draw(DrawArea, m_Size, m_Scale, showBGColor, StartPoint, m_OnFire, Assets, gfx);
	}
}

void Room::draw(float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx)
{
	if (!m_Visible)
		return;
	for (int i = 0; i < m_Layer.size(); i++)
	{
		if (m_Layer[i].m_Visible)
			m_Layer[i].draw(m_Size, m_Scale, showBGColor, StartPoint, m_OnFire, Assets, gfx);
	}
}

Room::Room()
{
	m_Visible = true;
	if (m_Layer.empty())
	{
		Layer newLayer;
		m_Layer.push_back(newLayer);
	}
}

Room::~Room()
{
	while (!m_Layer.empty())
	{
		std::vector<MapData> emptyMD;
		std::swap(m_Layer.back().m_MapData, emptyMD);
		std::vector<ShapeData> emptySD;
		std::swap(m_Layer.back().m_Shapes, emptySD);
		std::vector<WallsData> emptyWD;
		std::swap(m_Layer.back().m_Walls, emptyWD);
		m_Layer.pop_back();
	}
}

void Room::UpdateData(float m_Size, float m_Scale, D2D1_POINT_2F StartPoint, std::vector<Asset>& Assets)
{
	for (int i = 0; i < m_Layer.size(); i++)
	{
		m_Layer[i].UpdateData(m_Size, m_Scale, StartPoint, Assets);
	}
}

void Room::ClearAll()
{
	for (int i = 0; i < m_Layer.size(); i++)
	{
		m_Layer[i].ClearAll();
	}
}

size_t Room::CalculateBufferSize()
{
	size_t size = sizeof(char);												//size to hold visible
	size += sizeof(long);													//hold number of layers
	for (int i = 0; i < m_Layer.size(); i++)
	{
		size += m_Layer[i].CalculateBufferSize();
	}
	return size;
}

size_t Room::FillFromBuffer(char* buffer, double version, Graphics* gfx)
{
	size_t position = 0;

	ClearAll();
	while (!m_Layer.empty())
		m_Layer.pop_back();
	long numLayer;

	memcpy(&numLayer, buffer, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numLayer; i++)
	{
		Layer newLayer;
		size_t layersize = newLayer.FillFromBuffer(buffer + position, version, gfx);
		position += layersize;
		m_Layer.push_back(newLayer);
	}

	return position;
}