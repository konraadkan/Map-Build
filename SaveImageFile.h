#pragma once
#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>

class SaveImageFile
{
private:
	IWICImagingFactory* pFactory = NULL;
	IWICBitmapEncoder* pEncoder = NULL;
	IWICBitmap* pBitmap = NULL;
	ID2D1RenderTarget* renderTarget = NULL;
public:
	SaveImageFile();
	~SaveImageFile();
	HRESULT GetWICFactory(IWICImagingFactory** factory);
	void CreateWICBitmap(ID2D1Factory* d2dFactory, ID2D1Bitmap* bitmap);
};