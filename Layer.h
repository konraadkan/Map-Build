#pragma once
#include <vector>
#include "mapstruct.h"
#include "custom_geometry.h"
#include "AssetList.h"
#include "SpriteSheet.h"

class Layer
{
public:
	Layer();
	~Layer();
public:
	bool m_Visible = true;
	std::vector<MapData> m_MapData;
	std::vector<CustomGeometry> m_CG;
	std::vector<ShapeData> m_Shapes;
	std::vector<WallsData> m_Walls;
public:
	void draw(float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx);
	void draw(D2D1_RECT_F DrawArea, float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx);
	void UpdateData(float m_Size, float m_Scale, D2D1_POINT_2F StartPoint, std::vector<Asset>& Assets);
	void ClearAll();
	size_t CalculateBufferSize();
	char* BuildBuffer();
	size_t FillFromBuffer(char* buffer, double version, Graphics* gfx);
};