#pragma once
#include <vector>
#include "AssetList.h"
#include "Layer.h"

class Room
{
public:
	bool m_Visible;
	std::vector<Layer> m_Layer;
	void draw(float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx);
	void draw(D2D1_RECT_F DrawArea, float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx);
public:
	Room();
	~Room();
	void UpdateData(float m_Size, float m_Scale, D2D1_POINT_2F StartPoint, std::vector<Asset>& Assets);
	void ClearAll();
	size_t CalculateBufferSize();
	size_t FillFromBuffer(char* buffer, double version, Graphics* gfx);
};