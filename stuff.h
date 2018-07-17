#pragma once
#include "gfx_structs.h"
#include <queue>
#include <math.h>
#include <string>

#define DIMETRIC_ANGLE 0.463646716

std::queue<FLINE> CalculateGridPointsDimetrique(FRECT area, float xSize, float scale)
{//currently ignores scale; since scale transform seems to be a pain to deal with so far
	std::queue<FLINE> points;
	
	int numberTilesYAxis = (int)((area.bottom - area.top) / (xSize / 2.0f));
	for (float y = area.top - (numberTilesYAxis * (xSize / 2.0f)); y <= area.bottom * 2.0f; y += (xSize / 2.0f))
	{
		FLINE line;
		line.startPoint.x = area.left;
		line.startPoint.y = y;
		line.endPoint.x = line.startPoint.x + (float)(tan((PI / 180.0f) * 63.434) * (area.bottom - y));
		line.endPoint.y = area.bottom;
		points.push(line);
		line.endPoint.x = line.startPoint.x + (float)(tan((PI / 180.0f) * -63.434) * (area.top - y));
		line.endPoint.y = area.top;
		points.push(line);
	}

	return points;
}

std::queue<FLINE> CalculateGridPoints(FRECT area, float xSize, float scale)
{
	std::queue<FLINE> points;

	for (float x = area.left; x <= area.right; x += xSize)
	{
		FLINE line;
		line.startPoint.x = x;
		line.endPoint.x = x;
		line.startPoint.y = area.top;
		line.endPoint.y = area.bottom;
		points.push(line);
	}
	for (float y = area.top; y <= area.bottom; y += xSize / 2.0f)
	{
		FLINE line;
		line.startPoint.x = area.left;
		line.startPoint.y = y;
		line.endPoint.x = area.right;
		line.endPoint.y = y;
		points.push(line);
	}
	return points;
}

void DrawGridLines(Graphics* gfx, std::queue<FLINE> lines)
{
	while (!lines.empty())
	{
		FLINE line = lines.front();
		lines.pop();
		gfx->DrawLine(line.startPoint, line.endPoint);
	}
}

void EmptyLineQueue(std::queue<FLINE>* lines)
{
	while (!lines->empty())
	{
		lines->pop();
	}
}

void DrawHighlightArea(Graphics* gfx, FRHOMBUS area)
{
	COLORREF_F color = { 0.75f, 0.0f, 0.1f, 1.0f };
	gfx->DrawLine(area.p1, area.p2, color, 3.5f);
	gfx->DrawLine(area.p2, area.p3, color, 3.5f);
	gfx->DrawLine(area.p3, area.p4, color, 3.5f);
	gfx->DrawLine(area.p4, area.p1, color, 3.5f);
}

float HighValue(float p1, float p2, float p3)
{
	if (p1 > p2)
	{
		if (p1 > p3)
			return p1;
	}
	else if (p2 > p3)
		return p2;
	return p3;
}

float LowValue(float p1, float p2, float p3)
{
	if (p1 < p2)
	{
		if (p1 < p3)
			return p1;
	}
	else if (p2 < p3)
		return p2;
	return p3;
}

bool InTriangle(D2D1_POINT_2F p, FTRIANGLE area)
{
	float s, t, A;
	s = area.p1.y * area.p3.x - area.p1.x * area.p3.y + (area.p3.y - area.p1.y) * p.x + (area.p1.x - area.p3.x) * p.y;
	t = area.p1.x * area.p2.y - area.p1.y * area.p2.x + (area.p1.y - area.p2.y) * p.x + (area.p2.x - area.p1.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;
	A = -area.p2.x * area.p3.x + area.p1.y * (area.p3.x - area.p2.x) + area.p1.x * (area.p2.y - area.p3.y) + area.p2.x * area.p3.y;
	if (A < 0.0f)
	{
		s = -s;
		t = -t;
		A = -A;
	}
	return s > 0 && t > 0 && (s + t) <= A;
}

Part SquarePart(D2D1_POINT_2F p, FRECT rect)
{
	FTRIANGLE area({ rect.left, rect.top }, { rect.left + (rect.right - rect.left) / 2.0f, rect.top + (rect.bottom - rect.top) / 2.0f }, { rect.left, rect.bottom });

	if (InTriangle(p, area)) return LEFT;
	area.p3 = { rect.right, rect.top };
	if (InTriangle(p, area)) return TOP;
	area.p1 = area.p3;
	area.p3 = { rect.right, rect.bottom };
	if (InTriangle(p, area)) return RIGHT;
	return BOTTOM;
}

FRHOMBUS CalculateMouseOverArea(HWND hWnd, float xSize, int numberTilesX, int numberTilesY, D2D1_POINT_2F p)
{
	FRHOMBUS area;
//	double iPart, fPart;

	FRECT tArea;
	tArea.left = (float)((int)(p.x / xSize));
	tArea.left *= xSize;
	tArea.top = (float)((int)(p.y / (xSize / 2.0f)));
	tArea.top *= xSize / 2.0f;
	tArea.right = tArea.left + xSize;
	tArea.bottom = tArea.top + xSize / 2.0f;

	switch (SquarePart(p, tArea))
	{
	case LEFT:
		area.p1.x = tArea.left - xSize / 2.0f;
		area.p1.y = tArea.top + (tArea.bottom - tArea.top) / 2.0f;
		area.p2.x = tArea.left;
		area.p2.y = tArea.top;
		area.p3.x = area.p2.x + xSize / 2.0f;
		area.p3.y = area.p1.y;
		area.p4.x = tArea.left;
		area.p4.y = area.p2.y + xSize / 2.0f;
		return area;
	case TOP:
		area.p1.x = tArea.left;
		area.p1.y = tArea.top;
		area.p2.x = tArea.left + xSize / 2.0f;
		area.p2.y = tArea.top - xSize / 4.0f;
		area.p3.x = tArea.right;
		area.p3.y = tArea.top;
		area.p4.x = area.p2.x;
		area.p4.y = area.p2.y + xSize / 2.0f;
		return area;
	case RIGHT:
		area.p1.x = tArea.left + xSize / 2.0f;
		area.p1.y = tArea.top + xSize / 4.0f;
		area.p2.x = tArea.right;
		area.p2.y = tArea.top;
		area.p3.x = area.p1.x + xSize;
		area.p3.y = area.p1.y;
		area.p4.x = area.p2.x;
		area.p4.y = tArea.bottom;
		return area;
	}

	/*
	area.p1.X = (int)(p.x / xSize);
	area.p1.X *= xSize;
	area.p1.Y = p.y / (xSize / 2.0f);
	fPart = modf(area.p1.Y, &iPart);
	if (fPart >= 0.5f)
		area.p1.Y += 1.0f;
	area.p1.Y = (int)area.p1.Y;
	area.p1.Y *= (xSize / 2.0f);

	//probably should split the square the cursor is in into 4 triangles, then have a test to see if the cursor is in a triangle, if not try the next one, ect.

	area.p2.X = area.p1.X + (xSize / 2.0f);
	area.p2.Y = area.p1.Y - (xSize / 4.0f);

	area.p3.X = area.p2.X + (xSize / 2.0f);
	area.p3.Y = area.p1.Y;

	area.p4.X = area.p1.X + (xSize / 2.0f);
	area.p4.Y = area.p1.Y + (xSize / 4.0f);
	*/

	area.p1.x = (float)((int)(p.x / xSize));
	area.p1.y = (float)((int)(p.y / (xSize / 2.0f)));
	area.p1.y++;
	area.p1.y *= xSize / 2.0f;
	area.p1.x *= xSize;
	area.p2.x = area.p1.x + (xSize / 2.0f);
	area.p2.y = area.p1.y - (xSize / 4.0f);
	area.p3.x = area.p1.x + xSize;
	area.p3.y = area.p1.y;
	area.p4.x = area.p2.x;
	area.p4.y = area.p2.y + (xSize / 2.0f);
	return area;
}

static D2D1_POINT_2F GetMousePositionForCurrentDpi(LPARAM lParam, ID2D1Factory* factory)
{
	static D2D1_POINT_2F dpi = { 96,96 };	//default dpi

	factory->GetDesktopDpi(&dpi.x, &dpi.y);

	return D2D1::Point2F(static_cast<int>(static_cast<short>(LOWORD(lParam))) * 96 / dpi.x,
		static_cast<int>(static_cast<short>(HIWORD(lParam))) * 96 / dpi.y);
}

