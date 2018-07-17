#include "graphics.h"
#include "defines.h"


Graphics::Graphics()
{
	m_Factory = NULL;
	m_RenderTarget = NULL;
	m_Brush = NULL;
	m_WriteFactory = NULL;
	m_WriteFormat = NULL;
}

Graphics::~Graphics()
{
	if (m_Factory) m_Factory->Release();
	if (m_RenderTarget) m_RenderTarget->Release();
	if (m_Brush) m_Brush->Release();
	if (m_WriteFactory) m_WriteFactory->Release();
	if (m_WriteFormat) m_WriteFormat->Release();
	if (m_WriteFormatSmall) m_WriteFormatSmall->Release();
	if (m_LineShape) m_LineShape->Release();
	if (p_LineSink) p_LineSink->Release();
	if (m_ConeShape) m_ConeShape->Release();
	if (p_Sink) p_Sink->Release();
	if (m_WriteFormatVariable) m_WriteFormatVariable->Release();
	if (m_Hexagon) m_Hexagon->Release();
	if (p_HexSink) p_HexSink->Release();
	if (p_SquareGrid) p_SquareGrid->Release();
	if (p_HexGrid) p_HexGrid->Release();
	if (p_DimetriqueGrid) p_DimetriqueGrid->Release();
	if (m_SquareGrid) m_SquareGrid->Release();
}

bool Graphics::Init(HWND hWnd)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(hWnd, &rect);

	result = m_Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)), &m_RenderTarget);
	if (result != S_OK) return false;

	result = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f), &m_Brush);
	if (result != S_OK) return false;
	
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_WriteFactory));
	if (result != S_OK) return false;

	result = m_WriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 32.0f, L"en-us", &m_WriteFormat);
	if (result != S_OK) return false;

	result = m_WriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"en-us", &m_WriteFormatSmall);
	if (result != S_OK) return false;

	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	m_RenderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::ClearScreen(COLORREF_F color)
{
	m_RenderTarget->Clear(color);
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	m_Brush->SetColor(D2D1::ColorF(r, g, b, a));
	m_RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), m_Brush, 3.0f);
}

void Graphics::DrawCircle(float x, float y, float radius, COLORREF_F color)
{
	DrawCircle({ x, y }, radius, color, 3.0f);
}

void Graphics::DrawCircle(FPOINT center, float radius, COLORREF_F color, float thickness)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawEllipse(D2D1::Ellipse(center, radius, radius), m_Brush, thickness);
}

void Graphics::FillCircle(FPOINT center, float radius, COLORREF_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->FillEllipse(D2D1::Ellipse(center, radius, radius), m_Brush);
}

void Graphics::DrawLine(FPOINT p1, FPOINT p2)
{	
	DrawLine(p1, p2, COLOR_BLACK, 3.0f);
}

void Graphics::DrawLine(FPOINT p1, FPOINT p2, ID2D1BitmapRenderTarget* target)
{
	m_Brush->SetColor(D2D1::ColorF(COLOR_BLACK));
	target->DrawLine(p1, p2, m_Brush, 3.0f);
}

void Graphics::DrawLine(FPOINT p1, FPOINT p2, float r, float g, float b, float thickness)
{
	m_Brush->SetColor(D2D1::ColorF(r, g, b));
	m_RenderTarget->DrawLine(p1, p2, m_Brush, thickness);
}

void Graphics::DrawLine(FPOINT p1, FPOINT p2, COLORREF_F color, float thickness)
{
	DrawLine(p1, p2, color.r, color.g, color.b, thickness);
}

void Graphics::FillRect(FRECT area)
{
	FillRect(area, COLOR_BLACK);
}

void Graphics::FillRect(FRECT area, float r, float g, float b)
{
	FillRect(area, { r,g,b });
}

void Graphics::FillRect(FRECT area, COLORREF_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->FillRectangle(area, m_Brush);
}

void Graphics::DrawRect(FRECT area)
{
	DrawRect(area, COLOR_BLACK, 3.0f);
}

void Graphics::DrawRect(FRECT area, float r, float g, float b, float thickness)
{
	DrawRect(area, { r,g,b }, thickness);
}

void Graphics::DrawRect(FRECT area, COLORREF_F color, float thickness)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawRectangle(area, m_Brush, thickness);
}

void Graphics::DrawText(const wchar_t* text, FRECT targetArea, float r, float g, float b)
{
	DrawText(text, targetArea, { r,g,b });
}

void Graphics::DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawTextA(text, lstrlenW(text), m_WriteFormat, targetArea, m_Brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

void Graphics::DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment)
{
	m_WriteFormat->SetTextAlignment(alignment);
	DrawText(text, targetArea, color);
	m_WriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
}

void Graphics::DrawText(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment)
{
	DWRITE_PARAGRAPH_ALIGNMENT p = m_WriteFormat->GetParagraphAlignment();
	m_WriteFormat->SetParagraphAlignment(p_alignment);
	m_WriteFormat->SetTextAlignment(alignment);
	DrawText(text, targetArea, color);
	m_WriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	m_WriteFormat->SetParagraphAlignment(p);
}

void Graphics::DrawText(const std::string text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment)
{
	std::wstring wtext(text.length(), L' ');
	std::copy(text.begin(), text.end(), wtext.begin());
	DrawText(wtext.c_str(), targetArea, color, alignment, p_alignment);
}

void Graphics::DrawTextSmall(const wchar_t* text, FRECT targetArea, float r, float g, float b)
{
	DrawTextSmall(text, targetArea, { r,g,b });
}

void Graphics::DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawTextA(text, lstrlenW(text), m_WriteFormatSmall, targetArea, m_Brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

void Graphics::DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment)
{
	m_WriteFormatSmall->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_WriteFormatSmall->SetTextAlignment(alignment);
	DrawTextSmall(text, targetArea, color);
	m_WriteFormatSmall->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
}

void Graphics::DrawTextSmall(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment)
{
	DWRITE_PARAGRAPH_ALIGNMENT p = m_WriteFormat->GetParagraphAlignment();
	m_WriteFormatSmall->SetParagraphAlignment(p_alignment);
	m_WriteFormatSmall->SetTextAlignment(alignment);
	DrawTextSmall(text, targetArea, color);
	m_WriteFormatSmall->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_WriteFormat->SetParagraphAlignment(p);
}

void Graphics::DrawVariableText(const wchar_t* text, FRECT targetArea, COLORREF_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawText(text, lstrlenW(text), m_WriteFormatVariable, targetArea, m_Brush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
}

void Graphics::DrawVariableText(const wchar_t* text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment)
{
	DWRITE_PARAGRAPH_ALIGNMENT p = m_WriteFormat->GetParagraphAlignment();
	m_WriteFormatVariable->SetParagraphAlignment(p_alignment);
	m_WriteFormatVariable->SetTextAlignment(alignment);
	DrawVariableText(text, targetArea, color);
	m_WriteFormatVariable->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_WriteFormatVariable->SetParagraphAlignment(p);
}

void Graphics::DrawVariableText(const wchar_t text, FRECT targetArea, COLORREF_F color, DWRITE_TEXT_ALIGNMENT alignment, DWRITE_PARAGRAPH_ALIGNMENT p_alignment)
{
	std::wstring Text(1, text);
	DrawVariableText(Text.c_str(), targetArea, color, alignment, p_alignment);
}

void Graphics::DrawVariableText(const wchar_t text, FRECT targetArea, COLORREF_F color)
{
	std::wstring Text(1, text);
	DrawVariableText(Text.c_str(), targetArea, color);
}
void Graphics::DrawEllipse(D2D1_POINT_2F p, float radiusX, float radiusY, D2D1_COLOR_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->DrawEllipse(D2D1::Ellipse(p, radiusX, radiusY), m_Brush);
}

void Graphics::FillEllipse(D2D1_POINT_2F p, float radiusX, float radiusY, D2D1_COLOR_F color)
{
	m_Brush->SetColor(color);
	m_RenderTarget->FillEllipse(D2D1::Ellipse(p, radiusX, radiusY), m_Brush);
}

bool Graphics::BuildLineshape(FLINE line[5])
{
	if (m_LineShape) m_LineShape->Release();
	if (p_LineSink) p_LineSink->Release();
	HRESULT hres = m_Factory->CreatePathGeometry(&m_LineShape);
	if (!SUCCEEDED(hres)) return false;

	hres = m_LineShape->Open(&p_LineSink);
	if (SUCCEEDED(hres))
	{
		p_LineSink->BeginFigure(line[2].startPoint, D2D1_FIGURE_BEGIN_FILLED);
		p_LineSink->AddLine(line[1].startPoint);
		p_LineSink->AddLine(line[1].endPoint);
		p_LineSink->AddLine(line[4].endPoint);
		p_LineSink->AddLine(line[2].endPoint);
		p_LineSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		p_LineSink->Close();
		return true;
	}
	return false;
}

bool Graphics::BuildConeShape(FLINE line1, FLINE line2, D2D1_SIZE_F radius, float rotAngle)
{
	if (m_ConeShape) m_ConeShape->Release();
	if (p_Sink) p_Sink->Release();
	HRESULT hres = m_Factory->CreatePathGeometry(&m_ConeShape);
	if (!SUCCEEDED(hres)) return false;

	hres = m_ConeShape->Open(&p_Sink);
	if (SUCCEEDED(hres))
	{
		//p_Sink->BeginFigure(line1.startPoint, D2D1_FIGURE_BEGIN_HOLLOW);
		p_Sink->BeginFigure(line1.startPoint, D2D1_FIGURE_BEGIN_FILLED);
		p_Sink->AddLine(line1.endPoint);
		p_Sink->AddArc(D2D1::ArcSegment(line2.startPoint, radius, rotAngle, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		p_Sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		p_Sink->Close();

		return true;
	}

	return false;
}

void Graphics::DrawHexagon(D2D1_COLOR_F color)
{
	if (!m_Hexagon) return;
	if (!p_HexSink) return;

	m_Brush->SetColor(color);
	m_RenderTarget->DrawGeometry(m_Hexagon, m_Brush, 6.0f);
}

void Graphics::DrawHexagon(D2D1_COLOR_F color, D2D1_COLOR_F fillColor)
{
	if (!m_Hexagon) return;
	if (!p_HexSink) return;

	DrawHexagon(color);
	m_Brush->SetColor(fillColor);
	m_RenderTarget->FillGeometry(m_Hexagon, m_Brush);
}

bool Graphics::BuildHexagon(D2D1_POINT_2F points[6])
{
	if (m_Hexagon) m_Hexagon->Release();
	if (p_HexSink) p_HexSink->Release();
	HRESULT hres = m_Factory->CreatePathGeometry(&m_Hexagon);
	if (!SUCCEEDED(hres)) return false;

	hres = m_Hexagon->Open(&p_HexSink);
	if (SUCCEEDED(hres))
	{
		p_Sink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
		for (int i = 1; i < 6; i++)
			p_Sink->AddLine(points[i]);
		p_Sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		p_Sink->Close();

		return true;
	}

	return false;
}

bool Graphics::BuildHexagon(HEX points)
{
	if (m_Hexagon) m_Hexagon->Release();
	if (p_HexSink) p_HexSink->Release();
	HRESULT hres = m_Factory->CreatePathGeometry(&m_Hexagon);
	if (!SUCCEEDED(hres)) return false;

	hres = m_Hexagon->Open(&p_HexSink);
	if (SUCCEEDED(hres))
	{
		p_HexSink->BeginFigure(points.p0, D2D1_FIGURE_BEGIN_FILLED);
		p_HexSink->AddLine(points.p1);
		p_HexSink->AddLine(points.p2);
		p_HexSink->AddLine(points.p3);
		p_HexSink->AddLine(points.p4);
		p_HexSink->AddLine(points.p5);
		p_HexSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		p_HexSink->Close();

		return true;
	}

	return false;
}

void Graphics::DrawCone(D2D1_COLOR_F color)
{
	if (!m_ConeShape) return;
	if (!p_Sink) return;

	m_Brush->SetColor(color);
	m_RenderTarget->DrawGeometry(m_ConeShape, m_Brush, 6.0f);
}

void Graphics::DrawCone(D2D1_COLOR_F color, D2D1_COLOR_F fillColor)
{
	if (!m_ConeShape) return;
	if (!p_Sink) return;

	DrawCone(color);

	m_Brush->SetColor(fillColor);
	m_RenderTarget->FillGeometry(m_ConeShape, m_Brush);
}

void Graphics::DrawLineArea(D2D1_COLOR_F color)
{
	if (!m_LineShape) return;
	if (!p_LineSink) return;

	m_Brush->SetColor(color);
	m_RenderTarget->DrawGeometry(m_LineShape, m_Brush, 6.0f);
}

void Graphics::DrawLineArea(D2D1_COLOR_F color, D2D1_COLOR_F fillColor)
{
	if (!m_LineShape) return;
	if (!p_LineSink) return;

	DrawLineArea(color);

	m_Brush->SetColor(fillColor);
	m_RenderTarget->FillGeometry(m_LineShape, m_Brush);
}

void Graphics::BuildVariableText(float size, float scale)
{
	float scaledsize = size * scale;
	scaledsize /= 2.0f;
	if (m_WriteFormatVariable) m_WriteFormatVariable->Release();
	HRESULT hres = m_WriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, scaledsize, L"en-us", &m_WriteFormatVariable);
}

void Graphics::ResizeWindow(D2D1_SIZE_U size)
{
	m_RenderTarget->Resize(size);
}

HRESULT Graphics::CreateSquareGridPatternBrush(float m_Size, float scale)
{
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	HRESULT hr = m_RenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(m_Size * scale, m_Size * scale), &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		//draw pattern
		ID2D1SolidColorBrush *pGridBrush = NULL;
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &pGridBrush);
		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->DrawRectangle({ 0.0f, 0.0f, m_Size * scale, m_Size * scale }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->EndDraw();

			ID2D1Bitmap* pGridBitmap = NULL;
			hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
			if (SUCCEEDED(hr))
			{
				//choose tiling mode for the brush
				D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

				//create brush
				if (p_SquareGrid) p_SquareGrid->Release();
				hr = m_RenderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, &p_SquareGrid);

				pGridBitmap->Release();
			}
			pGridBrush->Release();
		}
		pCompatibleRenderTarget->Release();
	}
	return hr;
}

HRESULT Graphics::CreateHexGridPatternBrush(float m_Size, float scale)
{
	float theta = 0.523598776f;
	float scaledSize = scale * m_Size;
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	HRESULT hr = m_RenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(cos(theta) * scaledSize, sin(theta) * scaledSize * 3.0f), &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		//draw pattern
		ID2D1SolidColorBrush *pGridBrush = NULL;
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &pGridBrush);
		if (SUCCEEDED(hr))
		{
			HEX p = {};

			float i = cos(theta) * scaledSize / 2.0f;
			p.p0 = { i, 0.0f };
			p.p1 = { i + cos(theta) * scaledSize / 2.0f, sin(theta) * scaledSize / 2.0f };
			p.p2 = { p.p1.x, p.p1.y + scaledSize / 2.0f };
			p.p3 = { i, scaledSize };
			p.p4 = { i - cos(theta) * scaledSize / 2.0f, p.p3.y - sin(theta) * scaledSize / 2.0f };
			p.p5 = { p.p4.x, p.p4.y - scaledSize / 2.0f };

			pCompatibleRenderTarget->BeginDraw();
			BuildHexagon(p);
			pCompatibleRenderTarget->DrawGeometry(m_Hexagon, pGridBrush, 3.0f);
			pCompatibleRenderTarget->DrawLine(p.p3, { p.p3.x, sin(theta) * scaledSize * 3.0f }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->EndDraw();

			ID2D1Bitmap* pGridBitmap = NULL;
			hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
			if (SUCCEEDED(hr))
			{
				//choose tiling mode for the brush
				D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

				//create brush
				if (p_HexGrid) p_HexGrid->Release();
				hr = m_RenderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, &p_HexGrid);

				pGridBitmap->Release();
			}
			pGridBrush->Release();
		}
		pCompatibleRenderTarget->Release();
	}
	return hr;
}

HRESULT Graphics::CreateDimetriqueGridPatternBrush(float size, float scale)
{
	float scaledSize = scale * size;
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	HRESULT hr = m_RenderTarget->CreateCompatibleRenderTarget({ scaledSize, scaledSize / 2.0f }, &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		//draw pattern
		ID2D1SolidColorBrush *pGridBrush = NULL;
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &pGridBrush);
		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->DrawLine({ 0.0f, 0.0f }, { scaledSize / 2.0f, scaledSize / 4.0f }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->DrawLine({ scaledSize / 2.0f, scaledSize / 4.0f }, { scaledSize, 0.0f }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->DrawLine({ 0.0f, scaledSize / 2.0f }, { scaledSize / 2.0f, scaledSize / 4.0f }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->DrawLine({ scaledSize / 2.0f, scaledSize / 4.0f }, { scaledSize, scaledSize / 2.0f }, pGridBrush, 3.0f);
			pCompatibleRenderTarget->EndDraw();

			ID2D1Bitmap* pGridBitmap = NULL;
			hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
			if (SUCCEEDED(hr))
			{
				//choose tiling mode for the brush
				D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

				//create brush
				if (p_DimetriqueGrid) p_DimetriqueGrid->Release();
				hr = m_RenderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, &p_DimetriqueGrid);

				pGridBitmap->Release();
			}
			pGridBrush->Release();
		}
		pCompatibleRenderTarget->Release();
	}
	return hr;
}

void Graphics::DrawSquareGrid(D2D1_RECT_F area)
{
	/*this is what it used to do, changed its purpose to test a thing
	if (p_SquareGrid)
		m_RenderTarget->FillRectangle(area, p_SquareGrid);*/
}

void Graphics::DrawGridBasic(D2D1_RECT_F area, float size)
{
	//this function exists to deal with the weirdness that the main dnd pc has with loading the square patterns, it is less efficient, but at least it works
	for (float x = area.left; x <= area.right; x += size)
	{
		DrawLine({ x, area.top }, { x, area.bottom });
	}
	for (float y = area.top; y <= area.bottom; y += size)
	{
		DrawLine({ area.left, y }, { area.right, y });
	}
}

void Graphics::DrawHexGrid(D2D1_RECT_F area)
{
	if (p_HexGrid)
		m_RenderTarget->FillRectangle(area, p_HexGrid);
}

void Graphics::DrawDimetriqueGrid(D2D1_RECT_F area)
{
	if (p_DimetriqueGrid)
		m_RenderTarget->FillRectangle(area, p_DimetriqueGrid);
}

HRESULT Graphics::CreateSquareGridPattern()
{
	D2D1_RECT_F m_DrawAreaRect = { 0.0f, 0.0f, WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT };
	if (!p_SquareGrid) return NULL;
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	HRESULT hr = m_RenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(m_DrawAreaRect.right, m_DrawAreaRect.bottom), &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->Clear({ 1.0f, 1.0f, 1.0f, 0.0f });
			pCompatibleRenderTarget->FillRectangle(m_DrawAreaRect, p_SquareGrid);
			pCompatibleRenderTarget->EndDraw();

			if (m_SquareGrid) m_SquareGrid->Release();
			hr = pCompatibleRenderTarget->GetBitmap(&m_SquareGrid);
		}
		pCompatibleRenderTarget->Release();
	}
	return hr;
}

void Graphics::DrawSquareGridPattern(D2D1_RECT_F area)
{
	D2D1_RECT_F m_DrawAreaRect = { 0.0f, 0.0f, WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT };
	if (m_SquareGrid)
		m_RenderTarget->DrawBitmap(m_SquareGrid, area, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, m_DrawAreaRect);
}

bool Graphics::BuildCustomGeometry(std::vector<D2D1_POINT_2F> points, ID2D1PathGeometry* path, ID2D1GeometrySink* sink)
{
	if (points.empty()) return false;
	if (path) path->Release();
	if (sink) sink->Release();

	HRESULT res = m_Factory->CreatePathGeometry(&path);
	if (!SUCCEEDED(res)) return false;

	res = path->Open(&sink);
	if (SUCCEEDED(res))
	{
		sink->BeginFigure(points.front(), D2D1_FIGURE_BEGIN_FILLED);
		for (int i = 1; i < points.size(); i++)
		{
			sink->AddLine(points[i]);
		}
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		return true;
	}
	return false;
}