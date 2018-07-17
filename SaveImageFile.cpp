#include "SaveImageFile.h"
#include <cassert>
#include <Wincodecsdk.h>

SaveImageFile::SaveImageFile()
{
	
}

SaveImageFile::~SaveImageFile()
{
	if (pFactory) pFactory->Release();
	if (pBitmap) pBitmap->Release();
	if (renderTarget) renderTarget->Release();
}

HRESULT SaveImageFile::GetWICFactory(IWICImagingFactory** factory)
{
	static IWICImagingFactory* pWICFactory;
	HRESULT hr = S_OK;

	if (nullptr == pWICFactory)
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	}

	if (SUCCEEDED(hr))
	{
		assert(factory);
		*factory = pWICFactory;
		if (nullptr != (*factory))
		{
			(*factory)->AddRef();
		}
		return hr;
	}
	return hr;
}

void SaveImageFile::CreateWICBitmap(ID2D1Factory* d2dFactory, ID2D1Bitmap* bitmap)
{
	if (!bitmap) return;
	if (pFactory) pFactory->Release();
	if (pBitmap) pBitmap->Release();
	if (renderTarget) renderTarget->Release();
	GetWICFactory(&pFactory);
	pFactory->CreateBitmap(1920, 1080, GUID_WICPixelFormat32bppBGR, WICBitmapCacheOnLoad, &pBitmap);
	d2dFactory->CreateWicBitmapRenderTarget(pBitmap, D2D1::RenderTargetProperties(), &renderTarget);
	ID2D1SolidColorBrush* brush;
	renderTarget->CreateSolidColorBrush({ 0.0f,0.0f,0.0f,1.0f }, &brush);
	renderTarget->BeginDraw();
	renderTarget->Clear({ 1.0f,1.0f,1.0f,1.0f });
	renderTarget->DrawLine({ 0.0f,0.0f }, { 100.0f,100.0f }, brush, 5.0f);
	renderTarget->DrawBitmap(bitmap, { 0.0f, 0.0f, 1920.0f * 0.75f, 1080.0f * 0.75f }, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, { 0.0f, 0.0f, 1920.0f * 0.75f, 1080.0f * 0.75f });
	renderTarget->EndDraw();
	brush->Release();
	IWICBitmapEncoder* encoder = NULL;
	GUID containerFormat = GUID_ContainerFormatPng;
	pFactory->CreateEncoder(containerFormat, nullptr, &encoder);

	IWICStream* stream = NULL;
	pFactory->CreateStream(&stream);
	stream->InitializeFromFilename(L"Test.png", GENERIC_WRITE);
	encoder->Initialize(stream, WICBitmapEncoderNoCache);

	IWICMetadataBlockWriter* blockWriter;
	IWICBitmapFrameEncode* frameEncode;

	encoder->CreateNewFrame(&frameEncode, nullptr);
	frameEncode->Initialize(nullptr);
	frameEncode->SetSize(1920, 1080);

	//this is where you should set the pixel format i can't get the damn thing to do it though
	GUID destFormat;
	encoder->GetContainerFormat(&destFormat);

	frameEncode->QueryInterface(&blockWriter);
	frameEncode->WriteSource(pBitmap, nullptr);
	frameEncode->Commit();
	encoder->Commit();

	stream->Release();
	if (stream) stream = nullptr;
	if (encoder) encoder = nullptr;
	if (blockWriter) blockWriter = nullptr;
}