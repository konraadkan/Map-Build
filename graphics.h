#pragma once
#include<Windows.h>
#include<d2d1.h>
#include<dwrite.h>
#include<string>
#include<vector>
#include "gfx_structs.h"
#include "SaveImageFile.h"

#pragma comment(lib, "dwrite.lib")

class Graphics
{
private:
	ID2D1Factory* m_Factory = NULL;
	ID2D1HwndRenderTarget* m_RenderTarget = NULL;
	ID2D1SolidColorBrush* m_Brush = NULL;

	IDWriteFactory* m_WriteFactory = NULL;
	IDWriteTextFormat* m_WriteFormat = NULL;
	IDWriteTextFormat* m_WriteFormatSmall = NULL;
	IDWriteTextFormat* m_WriteFormatVariable = NULL;						//for tags

	ID2D1PathGeometry* m_ConeShape = NULL;
	ID2D1GeometrySink* p_Sink = NULL;
	ID2D1PathGeometry* m_LineShape = NULL;
	ID2D1GeometrySink* p_LineSink = NULL;
	ID2D1PathGeometry* m_Hexagon = NULL;
	ID2D1GeometrySink* p_HexSink = NULL;

	ID2D1BitmapBrush* p_SquareGrid = NULL;
	ID2D1BitmapBrush* p_HexGrid = NULL;
	ID2D1BitmapBrush* p_DimetriqueGrid = NULL;

	ID2D1Bitmap* m_SquareGrid = NULL;
public:
	Graphics();
	~Graphics();

	bool Init(HWND hWnd);
	ID2D1RenderTarget* GetRenderTarget() { return m_RenderTarget; }
	ID2D1Factory* GetID2DFactory() { return m_Factory; }
	ID2D1BitmapBrush* m_BitmapBrush;
	ID2D1Bitmap* GetGrid() { return m_SquareGrid; }

	void BeginDraw() { m_RenderTarget->BeginDraw(); }
	void EndDraw() { m_RenderTarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void ClearScreen(COLORREF_F color);

	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void DrawCircle(float x, float y, float radius, COLORREF_F color);
	void DrawCircle(FPOINT center, float radius, COLORREF_F color, float thickness);
	void FillCircle(FPOINT center, float radius, COLORREF_F color);

	void DrawLine(FPOINT p1, FPOINT p2);
	void DrawLine(FPOINT p1, FPOINT p2, ID2D1BitmapRenderTarget* target);
	void DrawLine(FPOINT p1, FPOINT p2, float r, float g, float b, float thickness);
	void DrawLine(FPOINT p1, FPOINT p2, COLORREF_F color, float thickness);

	void FillRect(FRECT area);
	void FillRect(FRECT area, float r, float g, float b);
	void FillRect(FRECT area, COLORREF_F color);

	void DrawRect(FRECT area);
	void DrawRect(FRECT area, float r, float g, float b, float thickness);
	void DrawRect(FRECT area, COLORREF_F color, float thickness);

	void DrawText(const wchar_t* text, FRECT targetArea, float r, float g, float b);
	void DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color);
	void DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment);
	void DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment);
	void DrawText(const std::string text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment);

	void DrawTextSmall(const wchar_t* text, FRECT targetArea, float r, float g, float b);
	void DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F color);
	void DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F, DWRITE_TEXT_ALIGNMENT alignment);
	void DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment);

	void DrawVariableText(const wchar_t* text, FRECT targetArea, COLORREF_F color = { 0.0f, 0.0f, 0.0f, 1.0f });
	void DrawVariableText(const wchar_t text, FRECT targetArea, COLORREF_F color = { 0.0f, 0.0f, 0.0f, 1.0f });
	void DrawVariableText(const wchar_t* text, FRECT targetArea, COLORREF_F, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment);
	void DrawVariableText(const wchar_t text, FRECT targetArea, COLORREF_F, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment);

	void DrawEllipse(D2D1_POINT_2F p, float radiusX, float radiusY, D2D1_COLOR_F color = { 0.0f, 0.0f, 1.0f, 1.0f });
	void FillEllipse(D2D1_POINT_2F p, float radiusX, float radiusY, D2D1_COLOR_F color = { 0.0f, 0.0f, 1.0f, 1.0f });
	bool BuildConeShape(FLINE line1, FLINE line2, D2D1_SIZE_F radius, float rotAngle);
	bool BuildLineshape(FLINE line[5]);
	bool BuildHexagon(D2D1_POINT_2F points[6]);
	bool BuildHexagon(HEX points);
	void DrawCone(D2D1_COLOR_F color = { 0.0f, 0.0f, 1.0f, 1.0f });
	void DrawCone(D2D1_COLOR_F color, D2D1_COLOR_F fillColor);
	void DrawLineArea(D2D1_COLOR_F color = { 0.0f, 0.0f, 1.0f, 1.0f });
	void DrawLineArea(D2D1_COLOR_F color, D2D1_COLOR_F fillColor);
	void DrawHexagon(D2D1_COLOR_F color = { 0.0f, 0.0f, 1.0f, 1.0f });
	void DrawHexagon(D2D1_COLOR_F color, D2D1_COLOR_F fillColor);
	
	void BuildVariableText(float size, float scale);
	void ResizeWindow(D2D1_SIZE_U size);

	HRESULT CreateSquareGridPatternBrush(float size, float scale);
	HRESULT CreateHexGridPatternBrush(float size, float scale);
	HRESULT CreateDimetriqueGridPatternBrush(float size, float scale);

	HRESULT CreateSquareGridPattern();

	void DrawSquareGrid(D2D1_RECT_F area);
	void DrawHexGrid(D2D1_RECT_F area);
	void DrawDimetriqueGrid(D2D1_RECT_F area);
	void DrawSquareGridPattern(D2D1_RECT_F area);

	void DrawGridBasic(D2D1_RECT_F area, float size);		//this is just a test function to deal with the weirdness that happens on the one computer

	bool BuildCustomGeometry(std::vector<D2D1_POINT_2F> points, ID2D1PathGeometry* path, ID2D1GeometrySink* sink);
};