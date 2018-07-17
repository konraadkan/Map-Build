#include "custom_geometry.h"


bool CustomGeometry::BuildGeometry(float scale, D2D1_POINT_2F currentOffset)
{
	if (m_Points.empty()) return false;

	if (m_Sink) m_Sink->Release();
	if (m_Path) m_Path->Release();

	m_Sink = NULL;
	m_Path = NULL;

	std::vector<D2D1_POINT_2F> targetPoints = GetUpdatedPoints(currentOffset, scale);
	
	HRESULT res = gfx->GetID2DFactory()->CreatePathGeometry(&m_Path);
	if (SUCCEEDED(res))
	{
		res = m_Path->Open(&m_Sink);
		if (SUCCEEDED(res))
		{
			m_Sink->BeginFigure(targetPoints.front(), D2D1_FIGURE_BEGIN_FILLED);
			for (int i = 1; i < targetPoints.size(); i++)
			{
				m_Sink->AddLine(targetPoints[i]);
			}
			m_Sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			m_Sink->Close();
			m_Sink->Release();
			m_Sink = NULL;
			return true;
		}
		m_Path->Release();
		m_Path = NULL;
		m_Sink = NULL;
	}
	return false;
}

std::vector<D2D1_POINT_2F> CustomGeometry::GetUpdatedPoints(D2D1_POINT_2F currentOffset, float scale)
{
	std::vector<D2D1_POINT_2F> newVector;
	D2D1_POINT_2F offset = {};
	offset.x = currentOffset.x - m_Offset.x;
	offset.y = currentOffset.y - m_Offset.y;

	for (size_t i = 0; i < m_Points.size(); i++)
	{
		D2D1_POINT_2F pP = {};
		pP.x = m_Points[i].x + offset.x;
		pP.y = m_Points[i].y + offset.y;

		pP.x *= scale;
		pP.y *= scale;

		newVector.push_back(pP);
	}

	return newVector;
}

CustomGeometry::~CustomGeometry()
{
/*	if (m_Path) m_Path->Release();
	if (m_Sink) m_Sink->Release();
	gfx = NULL;*/
}

/*
size_t BuildBuffer();
void FillFromBuffer(char* buffer);
*/

char* CustomGeometry::BuildBuffer()
{
	size_t sizeBuffer = CalcSize();
	char* buffer = new char[sizeBuffer];
	size_t position = 0;

	memcpy(buffer + position, &m_Offset.x, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_Offset.y, sizeof(float));
	position += sizeof(float);

	long numPts = (long)m_Points.size();
	memcpy(buffer + position, &numPts, sizeof(long));
	position += sizeof(long);

	for (int i = 0; i < numPts; i++)
	{
		D2D1_POINT_2F tPt = m_Points[i];
		memcpy(buffer + position, &tPt.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &tPt.y, sizeof(float));
		position += sizeof(float);
	}

	memcpy(buffer + position, &m_Layer, sizeof(long));
	position += sizeof(long);
	memcpy(buffer + position, &m_BitmapID, sizeof(long));
	position += sizeof(long);
	long tsize = (long)m_Size;
	memcpy(buffer + position, &tsize, sizeof(long));
	position += sizeof(long);
	memcpy(buffer + position, &m_Room, sizeof(unsigned long));
	position += sizeof(unsigned long);

	return buffer;
}

char* CustomGeometry::BuildBuffer(size_t& size)
{
	size_t sizeBuffer = CalcSize();
	char* buffer = new char[sizeBuffer];
	size_t position = 0;

	memcpy(buffer + position, &m_Offset.x, sizeof(float));
	position += sizeof(float);
	memcpy(buffer + position, &m_Offset.y, sizeof(float));
	position += sizeof(float);

	long numPts = (long)m_Points.size();
	memcpy(buffer + position, &numPts, sizeof(long));
	position += sizeof(long);

	for (int i = 0; i < numPts; i++)
	{
		D2D1_POINT_2F tPt = m_Points[i];
		memcpy(buffer + position, &tPt.x, sizeof(float));
		position += sizeof(float);
		memcpy(buffer + position, &tPt.y, sizeof(float));
		position += sizeof(float);
	}

	memcpy(buffer + position, &m_Layer, sizeof(long));
	position += sizeof(long);
	memcpy(buffer + position, &m_BitmapID, sizeof(long));
	position += sizeof(long);
	long tsize = (long)m_Size;
	memcpy(buffer + position, &tsize, sizeof(long));
	position += sizeof(long);
	memcpy(buffer + position, &m_Room, sizeof(unsigned long));
	position += sizeof(unsigned long);

	size = position;
	return buffer;
}

size_t CustomGeometry::FillFromBuffer(char* buffer, Graphics* _gfx)
{
	ClearAll();
	gfx = _gfx;

	size_t position = 0;

	memcpy(&m_Offset.x, buffer + position, sizeof(float));
	position += sizeof(float);
	memcpy(&m_Offset.y, buffer + position, sizeof(float));
	position += sizeof(float);

	long numPts;
	memcpy(&numPts, buffer + position, sizeof(long));
	position += sizeof(long);

	for (int i = 0; i < numPts; i++)
	{
		D2D1_POINT_2F tPt = {};
		memcpy(&tPt.x, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&tPt.y, buffer + position, sizeof(float));
		position += sizeof(float);
		m_Points.push_back(tPt);
	}

	memcpy(&m_Layer, buffer + position, sizeof(long));
	position += sizeof(long);
	memcpy(&m_BitmapID, buffer + position, sizeof(long));
	position += sizeof(long);
	long tsize;
	memcpy(&tsize, buffer + position, sizeof(long));
	position += sizeof(long);
	m_Size = (Size)tsize;
	memcpy(&m_Room, buffer + position, sizeof(unsigned long));
	position += sizeof(unsigned long);

	return position;
}

void CustomGeometry::ClearAll()
{
	std::vector<D2D1_POINT_2F> empty;
	std::swap(empty, m_Points);
	m_Offset = {};
	m_Layer = 0;
	m_BitmapID = 0;
	m_Room = 0;
	m_Size = Size::m_Medium;
}