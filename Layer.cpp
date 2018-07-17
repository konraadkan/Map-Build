#include "Layer.h"

bool InRect(D2D1_RECT_F Area, MapData mdata)
{
	Area.left -= 3.0f;
	Area.top -= 3.0f;
	if (mdata.m_ColorArea.left <= Area.left && mdata.m_ColorArea.right >= Area.right)
	{
		if (mdata.m_ColorArea.top <= Area.top && mdata.m_ColorArea.bottom >= Area.bottom)
			return true;
		if (mdata.m_ColorArea.top >= Area.top && mdata.m_ColorArea.top <= Area.bottom)
			return true;
		if (mdata.m_ColorArea.bottom >= Area.top && mdata.m_ColorArea.bottom <= Area.bottom)
			return true;
	}
	if (mdata.m_ColorArea.left >= Area.left && mdata.m_ColorArea.left <= Area.right)
	{
		if (mdata.m_ColorArea.top <= Area.top && mdata.m_ColorArea.bottom >= Area.bottom)
			return true;
		if (mdata.m_ColorArea.top >= Area.top && mdata.m_ColorArea.top <= Area.bottom)
			return true;
		if (mdata.m_ColorArea.bottom >= Area.top && mdata.m_ColorArea.bottom <= Area.bottom)
			return true;
	}
	if (mdata.m_ColorArea.right >= Area.left && mdata.m_ColorArea.right <= Area.right)
	{
		if (mdata.m_ColorArea.top <= Area.top && mdata.m_ColorArea.bottom >= Area.bottom)
			return true;
		if (mdata.m_ColorArea.top >= Area.top && mdata.m_ColorArea.top <= Area.bottom)
			return true;
		if (mdata.m_ColorArea.bottom >= Area.top && mdata.m_ColorArea.bottom <= Area.bottom)
			return true;
	}	
	return false;
}

bool InRect(D2D1_RECT_F Area, D2D1_POINT_2F Pt)
{
	if (Pt.x >= Area.left && Pt.x <= Area.right)
	{
		if (Pt.y >= Area.top && Pt.y <= Area.bottom)
			return true;
	}
	return false;
}

std::vector<D2D1_POINT_2F> GetUpdatedPoints(CustomGeometry cg, D2D1_POINT_2F currentOffset, float scale)
{
	std::vector<D2D1_POINT_2F> newVector;
	D2D1_POINT_2F offset = {};
	offset.x = currentOffset.x - cg.m_Offset.x;
	offset.y = currentOffset.y - cg.m_Offset.y;

	for (size_t i = 0; i < cg.m_Points.size(); i++)
	{
		D2D1_POINT_2F pP = {};
		pP.x = cg.m_Points[i].x + offset.x;
		pP.y = cg.m_Points[i].y + offset.y;

		pP.x *= scale;
		pP.y *= scale;

		newVector.push_back(pP);
	}

	return newVector;
}

bool InRect(D2D1_RECT_F Area, D2D1_POINT_2F Start, D2D1_POINT_2F End, D2D1_POINT_2F Offset, float scale)
{
	if (Start.x <= Area.left && End.x >= Area.right && Start.y <= Area.top && End.y >= Area.bottom)
		return true;
	if (Start.y <= Area.top && End.y >= Area.bottom)
	{
		if (Start.x >= Area.left && Start.x <= Area.right)
			return true;
		if (End.x >= Area.left && End.x <= Area.right)
			return true;
	}
	if (Start.x <= Area.left && End.x >= Area.bottom)
	{
		if (Start.y >= Area.top && Start.y <= Area.bottom)
			return true;
		if (End.y >= Area.top && End.y <= Area.bottom)
			return true;
	}
	if (Start.x >= Area.left && Start.x <= Area.right)
	{
		if (Start.y >= Area.top && Start.y <= Area.bottom)
			return true;
	}
	if (End.x >= Area.left && End.x <= Area.right)
	{
		if (End.y >= Area.top && End.y <= Area.bottom)
			return true;
	}
	if (Start.x < Area.left && End.x < Area.left) return false;
	if (Start.y < Area.top && End.y < Area.top) return false;
	if (Start.x > Area.right && End.x > Area.right) return false;
	if (Start.y > Area.bottom && End.y > Area.bottom) return false;

	float m = (End.y - Start.y) / (End.x - Start.x);
	float y = m * (Area.left - Start.x) + Start.y;
	if (y > Area.top && y < Area.bottom) return true;

	y = m * (Area.right - Start.x) + Start.y;
	if (y > Area.top && y < Area.bottom) return true;

	float x = (Area.top - Start.y) / m + Start.x;
	if (x > Area.left && x < Area.right) return true;

	x = (Area.bottom - Start.y) / m + Start.x;
	if (x > Area.left && x < Area.bottom) return true;

	return false;
}

void Layer::draw(D2D1_RECT_F DrawArea, float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx)
{
	if (!m_Visible)
		return;
	for (int i = 0; i < m_MapData.size(); i++)
	{
		//draw map data
		if (m_MapData[i].m_MenuId >= (int)Assets.size())
			continue;
		if (!InRect(DrawArea, m_MapData[i]))
			continue;
		if (Assets[m_MapData[i].m_MenuId].m_Sprite)
		{			
			if (showBGColor)
				gfx->FillRect(m_MapData[i].m_ColorArea, m_MapData[i].m_BgColor);			
			if (m_MapData[i].m_Rotation)
				gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(m_MapData[i].m_Rotation, D2D1::Point2F(m_MapData[i].dest.left + (m_MapData[i].dest.right - m_MapData[i].dest.left) / 2.0f, m_MapData[i].dest.top + (m_MapData[i].dest.bottom - m_MapData[i].dest.top) / 2.0f)));
			Assets[m_MapData[i].m_MenuId].m_Sprite->Draw(m_MapData[i].dest, m_MapData[i].src);
			if (m_MapData[i].m_Rotation)
				gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		}
		else
			gfx->FillRect(m_MapData[i].m_ColorArea, Assets[m_MapData[i].m_MenuId].m_BgColor);
		if (m_MapData[i].m_Tag)
			gfx->DrawVariableText(m_MapData[i].m_Tag, m_MapData[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
		if (m_MapData[i].m_Status)
		{
			D2D1_COLOR_F tempColor;
			D2D1_COLOR_F tempFillColor;
			if (m_MapData[i].m_Status == L'↑')
			{
				tempFillColor = { 0.0f, 0.392f, 0.0f, 0.66f };
				tempColor = { 1.0f, 0.545098f, 0.0f, 1.0f };
			}
			else if (m_MapData[i].m_Status == L'↓')
			{
				tempFillColor = { 1.0f, 0.412f, 0.71f, 0.66f };
				tempColor = { 1.0f, 0.2f, 0.0f, 1.0f };
			}
			else if (m_MapData[i].m_Status == L'Ω')
			{	//'Ω' or 234 or -21 will now represent being set on fire
				//fire does not need a color leave empty
				tempFillColor = { 0.0f, 0.0f, 0.0f, 0.0f };
				tempColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			}
			else
			{
				tempFillColor = { 0.502f, 0.502f, 0.502f, 0.66f };
				tempColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			}
			gfx->FillRect(m_MapData[i].m_ColorArea, tempFillColor);

			if (m_MapData[i].m_Status == L'Ω')
			{
				//draw fire here...
				if (m_OnFire->bmp) m_OnFire->Draw(m_MapData[i].m_ColorArea, m_OnFire->GetFrame(), 0.65f);
			}
			else if (m_MapData[i].m_Status != L'╥')
			{
				if (m_MapData[i].m_SizeCategory < Size::m_Medium)
				{
					float tTextSize = (float)Size::m_Medium - (float)m_MapData[i].m_SizeCategory + 1.0f;
					gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
				}
				else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
				{
					gfx->BuildVariableText(m_Size, m_Scale);
				}
				else
				{
					float tTextSize = (float)m_MapData[i].m_SizeCategory - (float)Size::m_Small;
					gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
				}
				gfx->DrawVariableText(m_MapData[i].m_Status, m_MapData[i].m_ColorArea, tempColor);
				gfx->BuildVariableText(m_Size, m_Scale);
			}
			else
			{
				if (m_MapData[i].m_SizeCategory < Size::m_Medium)
				{
					float tTextSize = (float)Size::m_Medium - (float)m_MapData[i].m_SizeCategory + 1.0f;
					gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
				}
				else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
				{
					gfx->BuildVariableText(m_Size, m_Scale);
				}
				else
				{
					float tTextSize = (float)m_MapData[i].m_SizeCategory - (float)Size::m_Small;
					gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
				}
				gfx->DrawVariableText(L"X_X", m_MapData[i].m_ColorArea, tempColor);
				gfx->BuildVariableText(m_Size, m_Scale);
			}
		}
	}
	for (int i = 0; i < m_CG.size(); i++)
	{
		//draw custom geometries
		if (!Assets[m_CG[i].m_BitmapID].m_Sprite)
			continue;
		bool OnScreen = false;
		std::vector<D2D1_POINT_2F> points = GetUpdatedPoints(m_CG[i], StartPoint, m_Scale);
		for (int pt = 0; pt < points.size(); pt++)
		{
			OnScreen = InRect(DrawArea, points[pt]);
			if (OnScreen) 
				break;
		}
		if (!OnScreen)
			continue;

		if (m_CG[i].m_Path)
		{
			ID2D1BitmapBrush* pBitmapBrush = NULL;
			ID2D1BitmapRenderTarget* pCompatibleTarget = NULL;

			float sizemod = 1.0f;
			if (m_CG[i].m_Size < Size::m_Medium)
			{
				sizemod = (float)(1.0f / (pow(2, (unsigned long)Size::m_Medium - (unsigned long)m_CG[i].m_Size)));
			}
			else if (m_CG[i].m_Size > Size::m_Medium)
			{
				sizemod = (float)((unsigned long)m_CG[i].m_Size - (unsigned long)Size::m_Medium + 1);
			}

			HRESULT hr = gfx->GetRenderTarget()->CreateCompatibleRenderTarget(D2D1::SizeF(m_Size * m_Scale * sizemod, m_Size * m_Scale * sizemod), &pCompatibleTarget);
			if (SUCCEEDED(hr))
			{
				pCompatibleTarget->BeginDraw();
				pCompatibleTarget->Clear({ 0.0f, 0.0f, 0.0f, 0.0f });
				D2D1_RECT_F dest = {};
				dest.right = dest.bottom = m_Size * m_Scale * sizemod;

				pCompatibleTarget->DrawBitmap(Assets[m_CG[i].m_BitmapID].m_Sprite->bmp, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, Assets[m_CG[i].m_BitmapID].m_Sprite->GetFrame());
				pCompatibleTarget->EndDraw();

				ID2D1Bitmap* tBitmap = NULL;
				hr = pCompatibleTarget->GetBitmap(&tBitmap);
				if (SUCCEEDED(hr))
				{
					D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
					hr = gfx->GetRenderTarget()->CreateBitmapBrush(tBitmap, brushProperties, &pBitmapBrush);

					tBitmap->Release();
				}
			}
			pCompatibleTarget->Release();

			if (SUCCEEDED(hr))
			{
				gfx->GetRenderTarget()->FillGeometry(m_CG[i].m_Path, pBitmapBrush);
				pBitmapBrush->Release();
			}
		}
	}
	for (int i = 0; i < m_Shapes.size(); i++)
	{
		//draw shapes
		D2D1_RECT_F cube = {};
		switch (m_Shapes[i].m_Shape)
		{
		case Shapes::Cube:
			gfx->DrawRect(m_Shapes[i].m_ColorArea, m_Shapes[i].m_Color, 5.0f);
			break;
		case Shapes::FilledCube:
			gfx->FillRect(m_Shapes[i].m_ColorArea, m_Shapes[i].m_Color);
			if (m_Shapes[i].m_Tag)
				gfx->DrawVariableText(m_Shapes[i].m_Tag, m_Shapes[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
			break;
		case Shapes::Sphere:
			gfx->DrawCircle({ m_Shapes[i].m_ColorArea.left, m_Shapes[i].m_ColorArea.top }, m_Shapes[i].m_ConvertedRadius, m_Shapes[i].m_Color, 5.0f);
			break;
		case Shapes::FilledSphere:
			gfx->FillCircle({ m_Shapes[i].m_ColorArea.left, m_Shapes[i].m_ColorArea.top }, m_Shapes[i].m_ConvertedRadius, m_Shapes[i].m_Color);
			break;
		}
	}
	for (int i = 0; i < m_Walls.size(); i++)
	{
		//draw walls
		D2D1_POINT_2F sP = D2D1::Point2F((m_Walls[i].m_StartPoint.x + StartPoint.x) * m_Scale, (m_Walls[i].m_StartPoint.y + StartPoint.y) * m_Scale);
		D2D1_POINT_2F eP = D2D1::Point2F((m_Walls[i].m_EndPoint.x + StartPoint.x) * m_Scale, (m_Walls[i].m_EndPoint.y + StartPoint.y) * m_Scale);
		if (InRect(DrawArea, sP, eP, StartPoint, m_Scale))
			gfx->DrawLine(sP, eP, m_Walls[i].m_Color, m_Walls[i].m_Thickness);
	}
}

void Layer::draw(float m_Size, float m_Scale, bool showBGColor, D2D1_POINT_2F StartPoint, SpriteSheet* m_OnFire, std::vector<Asset>& Assets, Graphics* gfx)
{
	if (!m_Visible)
		return;
	for (int i = 0; i < m_MapData.size(); i++)
	{
		//draw map data
		if (m_MapData[i].m_MenuId >= (int)Assets.size())
			continue;
		if (Assets[m_MapData[i].m_MenuId].m_Sprite)
		{
			if (showBGColor)
				gfx->FillRect(m_MapData[i].m_ColorArea, m_MapData[i].m_BgColor);
			if (m_MapData[i].m_Rotation)
				gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(m_MapData[i].m_Rotation, D2D1::Point2F(m_MapData[i].dest.left + (m_MapData[i].dest.right - m_MapData[i].dest.left) / 2.0f, m_MapData[i].dest.top + (m_MapData[i].dest.bottom - m_MapData[i].dest.top) / 2.0f)));
			Assets[m_MapData[i].m_MenuId].m_Sprite->Draw(m_MapData[i].dest, m_MapData[i].src);
			if (m_MapData[i].m_Rotation)
				gfx->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		}
		else
			gfx->FillRect(m_MapData[i].m_ColorArea, Assets[m_MapData[i].m_MenuId].m_BgColor);
		if (m_MapData[i].m_Tag)
			gfx->DrawVariableText(m_MapData[i].m_Tag, m_MapData[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
		if (m_MapData[i].m_Status)
		{
			D2D1_COLOR_F tempColor;
			D2D1_COLOR_F tempFillColor;
			if (m_MapData[i].m_Status == L'↑')
			{
				tempFillColor = { 0.0f, 0.392f, 0.0f, 0.66f };
				tempColor = { 1.0f, 0.545098f, 0.0f, 1.0f };
			}
			else if (m_MapData[i].m_Status == L'↓')
			{
				tempFillColor = { 1.0f, 0.412f, 0.71f, 0.66f };
				tempColor = { 1.0f, 0.2f, 0.0f, 1.0f };
			}
			else if (m_MapData[i].m_Status == L'Ω')
			{	//'Ω' or 234 or -21 will now represent being set on fire
				//fire does not need a color leave empty
				tempFillColor = { 0.0f, 0.0f, 0.0f, 0.0f };
				tempColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			}
			else
			{
				tempFillColor = { 0.502f, 0.502f, 0.502f, 0.66f };
				tempColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			}
			gfx->FillRect(m_MapData[i].m_ColorArea, tempFillColor);

			if (m_MapData[i].m_Status == L'Ω')
			{
				//draw fire here...
				if (m_OnFire->bmp) m_OnFire->Draw(m_MapData[i].m_ColorArea, m_OnFire->GetFrame(), 0.65f);
			}
			else if (m_MapData[i].m_Status != L'╥')
			{
				if (m_MapData[i].m_SizeCategory < Size::m_Medium)
				{
					float tTextSize = (float)Size::m_Medium - (float)m_MapData[i].m_SizeCategory + 1.0f;
					gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
				}
				else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
				{
					gfx->BuildVariableText(m_Size, m_Scale);
				}
				else
				{
					float tTextSize = (float)m_MapData[i].m_SizeCategory - (float)Size::m_Small;
					gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
				}
				gfx->DrawVariableText(m_MapData[i].m_Status, m_MapData[i].m_ColorArea, tempColor);
				gfx->BuildVariableText(m_Size, m_Scale);
			}
			else
			{
				if (m_MapData[i].m_SizeCategory < Size::m_Medium)
				{
					float tTextSize = (float)Size::m_Medium - (float)m_MapData[i].m_SizeCategory + 1.0f;
					gfx->BuildVariableText(m_Size / tTextSize, m_Scale);
				}
				else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
				{
					gfx->BuildVariableText(m_Size, m_Scale);
				}
				else
				{
					float tTextSize = (float)m_MapData[i].m_SizeCategory - (float)Size::m_Small;
					gfx->BuildVariableText(m_Size * tTextSize, m_Scale);
				}
				gfx->DrawVariableText(L"X_X", m_MapData[i].m_ColorArea, tempColor);
				gfx->BuildVariableText(m_Size, m_Scale);
			}
		}
	}
	for (int i = 0; i < m_CG.size(); i++)
	{
		//draw custom geometries
		if (!Assets[m_CG[i].m_BitmapID].m_Sprite)
			continue;

		if (m_CG[i].m_Path)
		{
			ID2D1BitmapBrush* pBitmapBrush = NULL;
			ID2D1BitmapRenderTarget* pCompatibleTarget = NULL;

			float sizemod = 1.0f;
			if (m_CG[i].m_Size < Size::m_Medium)
			{
				sizemod = (float)(1.0f / (pow(2, (unsigned long)Size::m_Medium - (unsigned long)m_CG[i].m_Size)));
			}
			else if (m_CG[i].m_Size > Size::m_Medium)
			{
				sizemod = (float)((unsigned long)m_CG[i].m_Size - (unsigned long)Size::m_Medium + 1);
			}

			HRESULT hr = gfx->GetRenderTarget()->CreateCompatibleRenderTarget(D2D1::SizeF(m_Size * m_Scale * sizemod, m_Size * m_Scale * sizemod), &pCompatibleTarget);
			if (SUCCEEDED(hr))
			{
				pCompatibleTarget->BeginDraw();
				pCompatibleTarget->Clear({ 0.0f, 0.0f, 0.0f, 0.0f });
				D2D1_RECT_F dest = {};
				dest.right = dest.bottom = m_Size * m_Scale * sizemod;
				
				pCompatibleTarget->DrawBitmap(Assets[m_CG[i].m_BitmapID].m_Sprite->bmp, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, Assets[m_CG[i].m_BitmapID].m_Sprite->GetFrame());
				pCompatibleTarget->EndDraw();

				ID2D1Bitmap* tBitmap = NULL;
				hr = pCompatibleTarget->GetBitmap(&tBitmap);
				if (SUCCEEDED(hr))
				{
					D2D1_BITMAP_BRUSH_PROPERTIES brushProperties = D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
					hr = gfx->GetRenderTarget()->CreateBitmapBrush(tBitmap, brushProperties, &pBitmapBrush);

					tBitmap->Release();
				}
			}
			pCompatibleTarget->Release();

			if (SUCCEEDED(hr))
			{
				gfx->GetRenderTarget()->FillGeometry(m_CG[i].m_Path, pBitmapBrush);
				pBitmapBrush->Release();
			}
		}
	}
	for (int i = 0; i < m_Shapes.size(); i++)
	{
		//draw shapes
		D2D1_RECT_F cube = {};
		switch (m_Shapes[i].m_Shape)
		{
		case Shapes::Cube:
			gfx->DrawRect(m_Shapes[i].m_ColorArea, m_Shapes[i].m_Color, 5.0f);
			break;
		case Shapes::FilledCube:
			gfx->FillRect(m_Shapes[i].m_ColorArea, m_Shapes[i].m_Color);
			if (m_Shapes[i].m_Tag)
				gfx->DrawVariableText(m_Shapes[i].m_Tag, m_Shapes[i].m_ColorArea, { 0.0f, 0.0f, 0.0f, 0.65f });
			break;
		case Shapes::Sphere:
			gfx->DrawCircle({ m_Shapes[i].m_ColorArea.left, m_Shapes[i].m_ColorArea.top }, m_Shapes[i].m_ConvertedRadius, m_Shapes[i].m_Color, 5.0f);
			break;
		case Shapes::FilledSphere:
			gfx->FillCircle({ m_Shapes[i].m_ColorArea.left, m_Shapes[i].m_ColorArea.top }, m_Shapes[i].m_ConvertedRadius, m_Shapes[i].m_Color);
			break;
		}
	}
	for (int i = 0; i < m_Walls.size(); i++)
	{
		//draw walls
		gfx->DrawLine({ (m_Walls[i].m_StartPoint.x + StartPoint.x) * m_Scale, (m_Walls[i].m_StartPoint.y + StartPoint.y) * m_Scale },
			{ (m_Walls[i].m_EndPoint.x + StartPoint.x) * m_Scale, (m_Walls[i].m_EndPoint.y + StartPoint.y) * m_Scale },
			m_Walls[i].m_Color, m_Walls[i].m_Thickness);
	}
}

void Layer::UpdateData(float m_Size, float m_Scale, D2D1_POINT_2F StartPoint, std::vector<Asset>& Assets)
{
	for (int i = 0; i < m_MapData.size(); i++)
	{
		float sizeMod = 1.0f;
		if (m_MapData[i].m_SizeCategory < Size::m_Medium)
		{
			sizeMod = (float)(1.0f / (pow(2, (int)Size::m_Medium - (int)m_MapData[i].m_SizeCategory)));
		}
		else if (m_MapData[i].m_SizeCategory > Size::m_Colossal)
		{
			sizeMod = -999.0f;
		}
		else if (m_MapData[i].m_SizeCategory > Size::m_Medium)
		{
			sizeMod = (float)(((int)m_MapData[i].m_SizeCategory - (int)Size::m_Medium) + 1.0f);
		}

		if (Assets[m_MapData[i].m_MenuId].m_Sprite)
		{
			if (sizeMod < 0.0f)
			{
				m_MapData[i].src = Assets[m_MapData[i].m_MenuId].m_Sprite->GetFrame();

				float destSizeX = abs(m_MapData[i].src.right - m_MapData[i].src.left);
				float destSizeY = abs(m_MapData[i].src.bottom - m_MapData[i].src.top);

				destSizeX *= m_Scale;
				destSizeY *= m_Scale;

				D2D1_RECT_F d2d1Rect = {};

				d2d1Rect.left = (m_MapData[i].m_Position.x + StartPoint.x) * m_Scale;
				d2d1Rect.top = (m_MapData[i].m_Position.y + StartPoint.y) * m_Scale;

				switch (m_MapData[i].m_SizeCategory)
				{
				case Size::m_Original:
					d2d1Rect.right = d2d1Rect.left + destSizeX;
					d2d1Rect.bottom = d2d1Rect.top + destSizeY;
					break;
				case Size::m_TwoTimes:
					d2d1Rect.right = d2d1Rect.left + destSizeX * 2.0f;
					d2d1Rect.bottom = d2d1Rect.top + destSizeY * 2.0f;
					break;
				case Size::m_ThreeTimes:
					d2d1Rect.right = d2d1Rect.left + destSizeX * 3.0f;
					d2d1Rect.bottom = d2d1Rect.top + destSizeY * 3.0f;
					break;
				case Size::m_FourTimes:
					d2d1Rect.right = d2d1Rect.left + destSizeX * 4.0f;
					d2d1Rect.bottom = d2d1Rect.top + destSizeY * 4.0f;
					break;
				case Size::m_Half:
					d2d1Rect.right = d2d1Rect.left + destSizeX / 2.0f;
					d2d1Rect.bottom = d2d1Rect.top + destSizeY / 2.0f;
					break;
				}
				m_MapData[i].m_ColorArea = m_MapData[i].dest = d2d1Rect;
			}
			else
			{
				m_MapData[i].src = Assets[m_MapData[i].m_MenuId].m_Sprite->GetFrame();

				float xMod = 0.0f;
				float yMod = 0.0f;

				D2D1_RECT_F size = { 0.0f, 0.0f, m_Size * sizeMod * m_Scale, m_Size * sizeMod * m_Scale };

				if (m_MapData[i].m_KeepRatio)
				{
					if (m_MapData[i].src.bottom < m_MapData[i].src.top)
					{
						float t = m_MapData[i].src.bottom;
						m_MapData[i].src.bottom = m_MapData[i].src.top;
						m_MapData[i].src.top = t;
					}
					if (m_MapData[i].src.right < m_MapData[i].src.left)
					{
						float t = m_MapData[i].src.right;
						m_MapData[i].src.right = m_MapData[i].src.left;
						m_MapData[i].src.left = t;
					}

					if ((m_MapData[i].src.bottom - m_MapData[i].src.top) > (m_MapData[i].src.right - m_MapData[i].src.left))
					{
						float ratio = (m_MapData[i].src.right - m_MapData[i].src.left) / (m_MapData[i].src.bottom - m_MapData[i].src.top);
						if (ratio < 0.0f) ratio *= -1;
						xMod = (size.right - size.left) - ((size.right - size.left) * ratio);
						xMod /= 2.0f;

						if (xMod < 0.0f) xMod *= -1;

						size.right = (size.right - size.left) * ratio;
						size.left = 0.0f;
					}
					else
					{
						float ratio = (m_MapData[i].src.bottom - m_MapData[i].src.top) / (m_MapData[i].src.right - m_MapData[i].src.left);
						if (ratio < 0.0f) ratio *= -1;
						yMod = (size.bottom - size.top) - ((size.bottom - size.top) / ratio);
						yMod /= 2.0f;
						//add conditional here for diametrique see below
						//if (m_GridType == Grid::m_Dimetrique) yMod /= 2.0f;
						if (yMod < 0.0f) yMod *= -1;

						size.bottom = (size.bottom - size.top) * ratio;
						size.top = 0.0f;
					}
				}
				m_MapData[i].dest = D2D1::RectF((m_MapData[i].m_Position.x + StartPoint.x) * m_Scale + xMod, (m_MapData[i].m_Position.y + StartPoint.y) * m_Scale + yMod,
					(m_MapData[i].m_Position.x + StartPoint.x) * m_Scale + size.right + xMod + 1.0f, (m_MapData[i].m_Position.y + StartPoint.y) * m_Scale + size.bottom + yMod + 1.0f);

				m_MapData[i].m_ColorArea.left = (m_MapData[i].m_Position.x + StartPoint.x) * m_Scale;
				m_MapData[i].m_ColorArea.right = m_MapData[i].m_ColorArea.left + m_Size * m_Scale * sizeMod;
				m_MapData[i].m_ColorArea.top = (m_MapData[i].m_Position.y + StartPoint.y) * m_Scale;
				m_MapData[i].m_ColorArea.bottom = m_MapData[i].m_ColorArea.top + m_Size * m_Scale * sizeMod;
			}
		}
		else
		{
			m_MapData[i].m_ColorArea.left = (m_MapData[i].m_Position.x + StartPoint.x) * m_Scale;
			m_MapData[i].m_ColorArea.right = m_MapData[i].m_ColorArea.left + m_Size * m_Scale * sizeMod;
			m_MapData[i].m_ColorArea.top = (m_MapData[i].m_Position.y + StartPoint.y) * m_Scale;
			m_MapData[i].m_ColorArea.bottom = m_MapData[i].m_ColorArea.top + m_Size * m_Scale * sizeMod;
		}
	}
	for (int i = 0; i < m_Shapes.size(); i++)
	{
		m_Shapes[i].m_ColorArea.left = (m_Shapes[i].m_Position.x + StartPoint.x) * m_Scale;
		m_Shapes[i].m_ColorArea.top = (m_Shapes[i].m_Position.y + StartPoint.y) * m_Scale;
		m_Shapes[i].m_ColorArea.right = m_Shapes[i].m_ColorArea.left + (m_Shapes[i].m_Radius / 5) * m_Size * m_Scale;
		m_Shapes[i].m_ColorArea.bottom = m_Shapes[i].m_ColorArea.top + (m_Shapes[i].m_Radius / 5) * m_Size * m_Scale;

		if (m_Shapes[i].m_Shape != Shapes::FilledSphere)
		{
			m_Shapes[i].m_ConvertedRadius = (m_Shapes[i].m_Radius / 5.0f) * m_Size * m_Scale;
		}
		else
		{
			m_Shapes[i].m_ConvertedRadius = m_Shapes[i].m_Radius * m_Scale;
		}
	}
	for (int i = 0; i < m_CG.size(); i++)
	{
		m_CG[i].BuildGeometry(m_Scale, StartPoint);
	}
}

Layer::Layer()
{
	m_Visible = true;
}

Layer::~Layer()
{
	ClearAll();
}

void Layer::ClearAll()
{
	std::vector<MapData> empty;
	std::swap(m_MapData, empty);
	std::vector<ShapeData> emptySD;
	std::swap(m_Shapes, emptySD);
	std::vector<WallsData> emptyWD;
	std::swap(m_Walls, emptyWD);
	/*
	while (!m_CG.empty())
	{
		if (m_CG.back().m_Sink)
			m_CG.back().m_Sink->Release();
		if (m_CG.back().m_Path)
			m_CG.back().m_Path->Release();
		m_CG.back().m_Sink = NULL;
		m_CG.back().m_Path = NULL;
		m_CG.pop_back();
	}*/
}

size_t Layer::CalculateBufferSize()
{	
	size_t size = sizeof(char);										//size visible

	size += sizeof(long);											//number map entries
	for (int i = 0; i < m_MapData.size(); i++)
	{
		size += m_MapData[i].CalcSize();							//map data size
	}

	size += sizeof(long);											//number custom geometries
	for (int i = 0; i < m_CG.size(); i++)
	{
		size += m_CG[i].CalcSize();									//custom geometry size
	}

	size += sizeof(long);											//number shape data
	for (int i = 0; i < m_Shapes.size(); i++)
	{
		size += m_Shapes[i].CalcSize();								//shape data size
	}

	size += sizeof(long);											//number walls
	for (int i = 0; i < m_Walls.size(); i++)
	{
		size += m_Walls[i].CalcSize();								//wall data size
	}

	return size;
}

char* Layer::BuildBuffer()
{
	size_t bufferSize = CalculateBufferSize();
	size_t position = 0;
	if (!bufferSize) return NULL;

	char* buffer = new char[bufferSize];
	memcpy(buffer + position, &m_Visible, sizeof(char));
	position += sizeof(char);

	long numberMapData = m_MapData.size();
	memcpy(buffer + position, &numberMapData, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberMapData; i++)
	{
		size_t s = 0;
		char* mapBuffer = m_MapData[i].BuildBuffer(s);
		if (!mapBuffer) continue;
		memcpy(buffer + position, mapBuffer, s);
		position += s;
		delete[] mapBuffer;
	}

	long numberGeometries = m_CG.size();
	memcpy(buffer + position, &numberGeometries, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberGeometries; i++)
	{
		size_t tsize = 0;
		char* tbuff = m_CG[i].BuildBuffer(tsize);
		memcpy(buffer + position, tbuff, tsize);
		position += tsize;
		delete[] tbuff;
	}

	long numberShapes = m_Shapes.size();
	memcpy(buffer + position, &numberShapes, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberShapes; i++)
	{
		size_t s = 0;
		char* tShape = m_Shapes[i].BuildBuffer(s);
		if (!tShape) continue;
		memcpy(buffer + position, tShape, s);
		position += s;
		delete[] tShape;
	}

	long numberWalls = m_Walls.size();
	memcpy(buffer + position, &numberWalls, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numberWalls; i++)
	{
		size_t s = 0;
		char* tWalls = m_Walls[i].BuildBuffer(s);
		if (!tWalls) continue;
		memcpy(buffer + position, tWalls, s);
		position += s;
		delete[] tWalls;
	}
	return buffer;
}

size_t Layer::FillFromBuffer(char* buffer, double version, Graphics* gfx)
{
	ClearAll();
	size_t position = 0;

	bool tvis;
	memcpy(&tvis, buffer, sizeof(char));
	m_Visible = (bool)tvis;
	position += sizeof(char);

	long numMapData;
	memcpy(&numMapData, buffer + position, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numMapData; i++)
	{		
		MapData tMap;
		size_t s = tMap.FillFromBuffer(buffer + position);
		position += s;
		m_MapData.push_back(tMap);
	}

	long numGeometries;
	memcpy(&numGeometries, buffer + position, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numGeometries; i++)
	{
		CustomGeometry tCG;
		size_t tsize = tCG.FillFromBuffer(buffer + position, gfx);
		position += tsize;
		m_CG.push_back(tCG);
	}

	long numShapes;
	memcpy(&numShapes, buffer + position, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numShapes; i++)
	{
		ShapeData tSD;
		size_t s = tSD.FillFromBuffer(buffer + position);
		position += s;
		m_Shapes.push_back(tSD);
	}

	long numWalls;
	memcpy(&numWalls, buffer + position, sizeof(long));
	position += sizeof(long);
	for (int i = 0; i < numWalls; i++)
	{
		WallsData tWD;
		size_t s = tWD.FillFromBuffer(buffer + position);
		position += s;
		m_Walls.push_back(tWD);
	}

	return position;
}