#pragma once
#include <vector>
#include <d2d1.h>
#include "defines.h"
#include "graphics.h"

class CustomGeometry
{
private:
public:
	CustomGeometry() {}
	~CustomGeometry();
	size_t CalcSize()
	{
		if (m_Points.empty()) return 0;
		size_t size = 0;
		size += sizeof(float) * 2;		//this is the size of the offset value
		size += sizeof(long);			//this is for holding how many points are in the geometry
		size += (sizeof(float) * 2) * m_Points.size();	//this is for the size of all the points
		size += sizeof(long);			//this is for the layer
		size += sizeof(long);			//this is for the bitmap id
		size += sizeof(long);			//this is for the size category
		size += sizeof(unsigned long);	//this is for the size of the roomID
		return size;
	}
	bool BuildGeometry(float scale = 1.0f, D2D1_POINT_2F currentOffset = {});
	std::vector<D2D1_POINT_2F> GetUpdatedPoints(D2D1_POINT_2F currentOffset, float scale = 1.0f);
	char* BuildBuffer();
	char* BuildBuffer(size_t& size);
	size_t FillFromBuffer(char* buffer, Graphics* _gfx);
	void ClearAll();
public:
	D2D1_POINT_2F m_Offset = {};
	std::vector<D2D1_POINT_2F> m_Points;
	ID2D1GeometrySink* m_Sink = NULL;
	ID2D1PathGeometry* m_Path = NULL;
	Graphics* gfx = NULL;
	long m_Layer = 0;
	long m_BitmapID = 0;
	unsigned long m_Room = 0;
	Size m_Size = Size::m_Medium;
};