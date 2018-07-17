#pragma once
#include <d2d1.h>

#define LEFT			0
#define TOP             1
#define RIGHT           2
#define BOTTOM          3

typedef unsigned int Part;

/***** decided not to bother with this and just use D2D1_COLOR_F since this was just being converted anyway
struct COLORREF_F
{
	float R;
	float G;
	float B;
	float A;
	COLORREF_F(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) { R = r; G = g; B = g; A = a; }
};*/
typedef D2D1_COLOR_F COLORREF_F;

/*****
struct FPOINT
{
	float X;
	float Y;
	FPOINT(float x = 0.0f, float y = 0.0f) { X = x; Y = y; }
};*/

typedef D2D1_POINT_2F FPOINT;

/******	this was being used until I relized that D2D1_RECT_F is pretty much the same thing which made this pointelss, typedef so i don't have to rename everything
struct FRECT
{
	float left;
	float top;
	float right;
	float bottom;
	FRECT(float Left = 0.0f, float Top = 0.0f, float Right = 0.0f, float Bottom = 0.0f)
	{
		left = Left;
		top = Top;
		right = Right;
		bottom = Bottom;
	}
};*/
typedef D2D1_RECT_F FRECT;

/*
struct FLINE
{
	FPOINT startPoint;
	FPOINT endPoint;
	FLINE(FPOINT sPoint = { 0.0f, 0.0f }, FPOINT ePoint = { 0.0f, 0.0f })
	{
		startPoint = sPoint;
		endPoint = ePoint;
	}
};*/
struct FLINE
{
	D2D1_POINT_2F startPoint;
	D2D1_POINT_2F endPoint;
};

struct FRHOMBUS
{
	FPOINT p1;
	FPOINT p2;
	FPOINT p3;
	FPOINT p4;
	FRHOMBUS(FPOINT P1 = { 0.0f, 0.0f }, FPOINT P2 = { 0.0f, 0.0f }, FPOINT P3 = { 0.0f, 0.0f }, FPOINT P4 = { 0.0f, 0.0f })
	{
		p1 = P1;
		p2 = P2;
		p3 = P3;
		p4 = P4;
	}
};

struct FTRIANGLE
{
	FPOINT p1;
	FPOINT p2;
	FPOINT p3;
	FTRIANGLE(FPOINT P1 = { 0.0f, 0.0f }, FPOINT P2 = { 0.0f, 0.0f }, FPOINT P3 = { 0.0f, 0.0f })
	{
		p1 = P1;
		p2 = P2;
		p3 = P3;
	}
};

struct HEX
{
	D2D1_POINT_2F p0, p1, p2, p3, p4, p5;
};

#define COLOR_BLACK	{0.0f, 0.0f, 0.0f, 1.0f}