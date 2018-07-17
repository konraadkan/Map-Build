#pragma once
#include <d2d1.h>
#include "defines.h"
#include "custom_geometry.h"

struct MapData
{
	size_t CalcSize()
	{
		size_t size = 0;
		size += sizeof(float) * 2;			//size of D2D1_POINT_2F
		size += sizeof(float) * 4;			//size of D2D1_RECT_F
		size += sizeof(float) * 4;			//D2D1_RECT_F
		size += sizeof(float) * 4;			//D2D1_RECT_F
		size += sizeof(float) * 4;			//D2D1_COLOR_F
		size += sizeof(int) * 4;			//size of all the integers within the struct and the SizeCategory converted into an integer
		size += sizeof(wchar_t);			//size of tag var
		size += sizeof(char);				//size of keep aspect boolean var
		size += sizeof(float);				//size of rotation value
		size += sizeof(unsigned long);		//size of roomID
		return size;
	}
	void FillFromBuffer(char* buffer, double version)
	{
		size_t pos = FillFromBuffer(buffer);
		if (version < 6.0)
		{
			pos -= sizeof(char);
			pos -= sizeof(float);
			pos -= sizeof(unsigned long);

			if (version <= 5.169)
			{
				memcpy(&m_KeepRatio, buffer + pos, sizeof(bool));
				pos += sizeof(bool);
			}
			else
			{
				char t;
				memcpy(&t, buffer + pos, sizeof(char));
				pos += sizeof(char);
				m_KeepRatio = (bool)t;
			}
			if (version >= 5.04 && pos)
			{
				memcpy(&m_Rotation, buffer + pos, sizeof(float));
				pos += sizeof(float);
			}
			if (version >= 5.169 && pos)
			{
				memcpy(&m_Room, buffer + pos, sizeof(unsigned long));
				pos += sizeof(unsigned long);
			}
		}
	}
	size_t FillFromBuffer(char* buffer)
	{
		if (!buffer) return NULL;
		size_t position = 0;
		memcpy(&m_Position.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Position.y, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&dest.left, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&dest.top, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&dest.right, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&dest.bottom, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&src.left, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&src.top, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&src.right, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&src.bottom, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.left, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.top, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.right, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.bottom, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_BgColor.r, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_BgColor.g, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_BgColor.b, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_BgColor.a, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_MenuId, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_Layer, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_Status, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_Tag, buffer + position, sizeof(wchar_t));
		position += sizeof(wchar_t);
		int size;
		memcpy(&size, buffer + position, sizeof(int));
		position += sizeof(int);
		m_SizeCategory = (Size)size;		
		char kr;
		memcpy(&kr, buffer + position, sizeof(char));
		m_KeepRatio = (bool)kr;
		position += sizeof(char);
		memcpy(&m_Rotation, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Room, buffer + position, sizeof(unsigned long));
		position += sizeof(unsigned long);
		return position;
	}
	char* BuildBuffer()
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;
		memcpy(buffer + position, &m_Position.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Position.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Status, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Tag, sizeof(wchar_t));
		position += sizeof(wchar_t);
		int size = (int)m_SizeCategory;
		memcpy(buffer + position, &size, sizeof(int));
		position += sizeof(int);
		char kr;
		kr = (char)m_KeepRatio;
		memcpy(buffer + position, &kr, sizeof(bool));
		position += sizeof(bool);
		memcpy(buffer + position, &m_Rotation, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Room, sizeof(unsigned long));
		position += sizeof(unsigned long);
		return buffer;
	}
	char* BuildBuffer(size_t& pos)
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;
		memcpy(buffer + position, &m_Position.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Position.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &dest.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &src.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_BgColor.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Status, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Tag, sizeof(wchar_t));
		position += sizeof(wchar_t);
		int size = (int)m_SizeCategory;
		memcpy(buffer + position, &size, sizeof(int));
		position += sizeof(int);
		char kr;
		kr = (char)m_KeepRatio;
		memcpy(buffer + position, &kr, sizeof(bool));
		position += sizeof(bool);
		memcpy(buffer + position, &m_Rotation, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Room, sizeof(unsigned long));
		position += sizeof(unsigned long);

		pos = position;
		return buffer;
	}
	D2D1_POINT_2F m_Position = {};
	D2D1_RECT_F dest = {};
	D2D1_RECT_F src = {};
	D2D1_RECT_F m_ColorArea = {};
	D2D1_COLOR_F m_BgColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	int m_MenuId = 0;
	int m_Layer = 0;
	int m_Status = 0;
	wchar_t m_Tag = 0;
	Size m_SizeCategory = Size::m_Medium;
	bool m_KeepRatio = true;
	float m_Rotation = 0.0f;
	unsigned long m_Room = 0;
};

enum class Shapes
{
	Cube,
	FilledCube,
	Line,
	Sphere,
	Cone,
	FilledSphere
};

struct WallsData
{
	size_t CalcSize()
	{
		size_t size = 0;
		size += sizeof(float) * 2;				//size of D2D1_POINT_2F
		size += sizeof(float) * 2;				//same
		size += sizeof(float) * 4;				//size of D2D1_COLOR_F
		size += sizeof(int) * 2;				//size of all the integer vars
		size += sizeof(float);					//size of all the float vars
		return size;
	}
	size_t FillFromBuffer(char* buffer)
	{
		if (!buffer) return NULL;
		size_t position = 0;

		memcpy(&m_StartPoint.x, buffer, sizeof(float));
		position += sizeof(float);
		memcpy(&m_StartPoint.y, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_EndPoint.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_EndPoint.y, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.r, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.g, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.b, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.a, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Thickness, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Layer, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_MenuId, buffer + position, sizeof(int));
		position += sizeof(int);

		return position;
	}
	char* BuildBuffer(size_t& s)
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;

		memcpy(buffer, &m_StartPoint.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_StartPoint.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_EndPoint.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_EndPoint.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Thickness, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);
		s = position;

		return buffer;
	}
	char* BuildBuffer()
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;

		memcpy(buffer, &m_StartPoint.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_StartPoint.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_EndPoint.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_EndPoint.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Thickness, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);

		return buffer;
	}
	D2D1_POINT_2F m_StartPoint = {};
	D2D1_POINT_2F m_EndPoint = {};
	D2D1_COLOR_F m_Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	float m_Thickness = 1.0f;
	int m_Layer = 0;
	int m_MenuId = 0;
};


struct ShapeData
{
	size_t CalcSize()
	{
		size_t size = 0;
		size += sizeof(long);					// size of Shapes converted to long
		size += sizeof(float) * 2;				// size of D2D1_POINT_2F
		size += sizeof(float) * 4;				// size of D2D1_RECT_F
		size += sizeof(float) * 4;				// size of D2D1_COlOR_F
		size += sizeof(int) * 3;				// size of all the int vars
		size += sizeof(wchar_t);				// size of tag
		size += sizeof(float);					// size of the float var
		return size;
	}
	size_t FillFromBuffer(char* buffer)
	{
		if (!buffer) return NULL;
		size_t position = 0;
		long shape = 0;
		
		memcpy(&shape, buffer + position, sizeof(long));
		position += sizeof(long);
		m_Shape = (Shapes)shape;
		memcpy(&m_Position.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Position.y, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.left, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.top, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.right, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_ColorArea.bottom, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.r, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.g, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.b, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Color.a, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Radius, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_ConvertedRadius, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&m_Layer, buffer + position, sizeof(int));
		position += sizeof(int);
		memcpy(&m_Tag, buffer + position, sizeof(wchar_t));
		position += sizeof(wchar_t);
		memcpy(&m_MenuId, buffer + position, sizeof(int));
		position += sizeof(int);

		return position;
	}
	char* BuildBuffer(size_t& s)
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;
		long shape = (long)m_Shape;

		memcpy(buffer, &shape, sizeof(long));
		position += sizeof(long);
		memcpy(buffer + position, &m_Position.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Position.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Radius, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_ConvertedRadius, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Tag, sizeof(wchar_t));
		position += sizeof(wchar_t);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);
		s = position;

		return buffer;
	}
	char* BuildBuffer()
	{
		size_t bufferLen = CalcSize();
		if (!bufferLen) return NULL;
		char* buffer = new char[bufferLen];
		size_t position = 0;
		long shape = (long)m_Shape;

		memcpy(buffer, &shape, sizeof(long));
		position += sizeof(long);
		memcpy(buffer + position, &m_Position.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Position.y, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.left, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.top, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.right, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_ColorArea.bottom, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.r, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.g, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.b, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Color.a, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Radius, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_ConvertedRadius, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &m_Layer, sizeof(int));
		position += sizeof(int);
		memcpy(buffer + position, &m_Tag, sizeof(wchar_t));
		position += sizeof(wchar_t);
		memcpy(buffer + position, &m_MenuId, sizeof(int));
		position += sizeof(int);

		return buffer;
	}
	Shapes m_Shape = Shapes::Cube;
	D2D1_POINT_2F m_Position = {};
	D2D1_RECT_F m_ColorArea = {};
	D2D1_COLOR_F m_Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	int m_Radius = 5;
	float m_ConvertedRadius = 0.0f;
	int m_Layer = 0;
	wchar_t m_Tag = 0;
	int m_MenuId = 0;
};