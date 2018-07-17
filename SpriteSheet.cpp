#include "SpriteSheet.h"
#include "defines.h"
#include <Windows.h>
#include <string>

SpriteSheet::SpriteSheet(const wchar_t* filename, Graphics* gfx)
{
	//temp solution to skip .sprs and .spr3s until i add support for them

	for (int i = (int)wcslen(filename); i > 0; i--)
	{
		if (filename[i] == L'.')
		{
			wchar_t* imageext = NULL;
			imageext = new wchar_t[wcslen(filename) - i + 1];
			imageext[wcslen(filename) - i] = '\0';
			memcpy(imageext, filename + i, sizeof(wchar_t) * (wcslen(filename) - i));
			
			if (!_wcsicmp(imageext, L".spr"))
			{
				decodeSPR(filename);
			}
			if (!_wcsicmp(imageext, L".spr3"))
			{
				size_t imagebufferlen = 0;
				char* imagebuffer = decodeSPR3(filename, imagebufferlen);
				isSPR3 = true;
				InitFromMemory(imagebuffer, imagebufferlen, gfx);
				delete[] imagebuffer;
			}
			delete[] imageext;
			break;
		}
	}

	if (!isSPR3)
	{
		//init
		this->gfx = gfx;		//save teh gfx parameter
		bmp = NULL;				//NULL for ID2D1Bitmap
		HRESULT hr;				//for checking errors

								//create factory
		IWICImagingFactory *wicFactory = NULL;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
		if (hr != S_OK)
		{
			//error control
			MessageBox(NULL, "Failed to create wicFactory", "Error", MB_OK | MB_ICONERROR);
		}

		unsigned int MaxBitmapSize = gfx->GetRenderTarget()->GetMaximumBitmapSize();

		//create decoder
		IWICBitmapDecoder *wicDecoder = NULL;
		if (newname)
			hr = wicFactory->CreateDecoderFromFilename(newname, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);
		else
			hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

		if (hr != S_OK)
		{
			//error control
			std::wstring eMsg = L"Failed to load \"";
			eMsg.append(filename);
			eMsg.append(L"\"");
			MessageBoxW(NULL, eMsg.c_str(), L"Error", MB_OK | MB_ICONERROR);
			gfx = NULL;
			bmp = NULL;
			wicFactory->Release();
			return;
		}

		//read frame from teh image
		IWICBitmapFrameDecode* wicFrame = NULL;
		hr = wicDecoder->GetFrame(0, &wicFrame);			//0 = number frame, in this case there is only 1 frame so zero
		if (hr != S_OK)
		{
			//error control
			MessageBox(NULL, "Failed to get frame.", "Error", MB_OK | MB_ICONERROR);
			gfx = NULL;
			bmp = NULL;
			wicFactory->Release();
			wicDecoder->Release();
			return;
		}
		unsigned int frameWidth, frameHeight;
		wicFrame->GetSize(&frameWidth, &frameHeight);

		if ((frameWidth > MaxBitmapSize) || (frameHeight > MaxBitmapSize))
		{
			std::wstring errmsg = L"Failed to load ";
			errmsg.append(filename);
			errmsg.append(L". Bitmap size is ");
			errmsg.append(std::to_wstring(frameWidth));
			errmsg.append(L" x ");
			errmsg.append(std::to_wstring(frameHeight));
			errmsg.append(L". Maximum supported size is ");
			errmsg.append(std::to_wstring(MaxBitmapSize));
			errmsg.append(L" x ");
			errmsg.append(std::to_wstring(MaxBitmapSize));
			errmsg.push_back('.');
			MessageBoxW(NULL, errmsg.c_str(), L"Error", MB_OK | MB_ICONERROR);
			gfx = NULL;
			bmp = NULL;
			wicFactory->Release();
			wicDecoder->Release();
			wicFrame->Release();
			return;
		}

		//create a converter
		IWICFormatConverter *wicConverter = NULL;
		hr = wicFactory->CreateFormatConverter(&wicConverter);
		if (hr != S_OK)
		{
			//error control
			MessageBox(NULL, "Faield to create wicConverter.", "Error", MB_OK | MB_ICONERROR);
			gfx = NULL;
			bmp = NULL;
			wicFactory->Release();
			wicDecoder->Release();
			wicFrame->Release();
			return;
		}

		hr = wicConverter->Initialize(
			wicFrame,						//frame
			GUID_WICPixelFormat32bppPBGRA,	//output Pixel format (P means packed; pixels are represented by 4 byts packed into an int: 0xRRGGBBAA
			WICBitmapDitherTypeNone,		//Irrelevant
			NULL,							//no palette needed
			0.0,							//alpha transparency % irrelevent
			WICBitmapPaletteTypeCustom		//irrelevant
		);
		if (hr != S_OK)
		{
			//error control
			MessageBox(NULL, "Failed to initialize the bitmap frame.", "Error", MB_OK | MB_ICONERROR);
		}

		//use converter to create D2D1Bitmap
		hr = gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
			wicConverter,					//converter
			NULL,							//D2D1_BITMAP_PROPERTIES
			&bmp							//destination D2D1 bitmap
		);

		if (hr != S_OK)
		{
			//error control
			MessageBox(NULL, "Failed to CreateBitmapFromWicBitmap", "Error", MB_OK | MB_ICONERROR);
			spriteWidth = 0;
			spriteHeight = 0;
		}
		else
		{
			spriteWidth = (int)bmp->GetSize().width;
			spriteHeight = (int)bmp->GetSize().height;
		}

		if (wicFactory) wicFactory->Release();
		if (wicDecoder) wicDecoder->Release();
		if (wicConverter) wicConverter->Release();
		if (wicFrame) wicFrame->Release();
	}
}
bool SpriteSheet::InitFromMemory(char* buffer, size_t bufferLen, Graphics* gfx)
{
	//init
	this->gfx = gfx;		//save teh gfx parameter
	bmp = NULL;				//NULL for ID2D1Bitmap
	HRESULT hr;				//for checking errors

							//create factory
	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
	if (hr != S_OK)
	{
		//error control
		MessageBox(NULL, "Failed to create wicFactory", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	unsigned int MaxBitmapSize = gfx->GetRenderTarget()->GetMaximumBitmapSize();

	//create stream
	IWICStream* pIWICStream = NULL;
	hr = wicFactory->CreateStream(&pIWICStream);
	if (hr != S_OK)
	{
		MessageBox(NULL, "Failed to create stream", "error", MB_OK | MB_ICONERROR);
		return false;
	}
	pIWICStream->InitializeFromMemory(reinterpret_cast<BYTE*>(buffer), bufferLen);
	if (hr != S_OK)
	{
		MessageBox(NULL, "Failed to initialize memory", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	//create decoder
	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromStream(pIWICStream, NULL, WICDecodeMetadataCacheOnLoad, &wicDecoder);
	if (hr != S_OK)
	{
		MessageBox(NULL, "Failed to create wic decoder!", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	
	//read frame from teh image
	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame);			//0 = number frame, in this case there is only 1 frame so zero
	if (hr != S_OK)
	{
		//error control
		MessageBox(NULL, "Failed to get frame.", "Error", MB_OK | MB_ICONERROR);
		gfx = NULL;
		bmp = NULL;
		wicFactory->Release();
		wicDecoder->Release();
		return false;
	}
	unsigned int frameWidth, frameHeight;
	wicFrame->GetSize(&frameWidth, &frameHeight);

	if ((frameWidth > MaxBitmapSize) || (frameHeight > MaxBitmapSize))
	{
		std::wstring errmsg = L"Failed to load. Bitmap size is ";
		errmsg.append(std::to_wstring(frameWidth));
		errmsg.append(L" x ");
		errmsg.append(std::to_wstring(frameHeight));
		errmsg.append(L". Maximum supported size is ");
		errmsg.append(std::to_wstring(MaxBitmapSize));
		errmsg.append(L" x ");
		errmsg.append(std::to_wstring(MaxBitmapSize));
		errmsg.push_back('.');
		MessageBoxW(NULL, errmsg.c_str(), L"Error", MB_OK | MB_ICONERROR);
		gfx = NULL;
		bmp = NULL;
		wicFactory->Release();
		wicDecoder->Release();
		wicFrame->Release();
		return false;
	}

	//create a converter
	IWICFormatConverter *wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);
	if (hr != S_OK)
	{
		//error control
		MessageBox(NULL, "Faield to create wicConverter.", "Error", MB_OK | MB_ICONERROR);
		gfx = NULL;
		bmp = NULL;
		wicFactory->Release();
		wicDecoder->Release();
		wicFrame->Release();
		return false;
	}

	hr = wicConverter->Initialize(
		wicFrame,						//frame
		GUID_WICPixelFormat32bppPBGRA,	//output Pixel format (P means packed; pixels are represented by 4 byts packed into an int: 0xRRGGBBAA
		WICBitmapDitherTypeNone,		//Irrelevant
		NULL,							//no palette needed
		0.0,							//alpha transparency % irrelevent
		WICBitmapPaletteTypeCustom		//irrelevant
	);
	if (hr != S_OK)
	{
		//error control
		MessageBox(NULL, "Failed to initialize the bitmap frame.", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	//use converter to create D2D1Bitmap
	hr = gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter,					//converter
		NULL,							//D2D1_BITMAP_PROPERTIES
		&bmp							//destination D2D1 bitmap
	);

	if (hr != S_OK)
	{
		//error control
		MessageBox(NULL, "Failed to CreateBitmapFromWicBitmap", "Error", MB_OK | MB_ICONERROR);
		spriteWidth = 0;
		spriteHeight = 0;
		return false;
	}
	else
	{
		spriteWidth = (int)bmp->GetSize().width;
		spriteHeight = (int)bmp->GetSize().height;
	}

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
	return true;
}

SpriteSheet::SpriteSheet(const wchar_t* filename, Graphics* gfx, int spriteWidth, int spriteHeight) : SpriteSheet(filename, gfx)
{
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
	this->spriteAcross = (int)bmp->GetSize().width / spriteWidth;
}

SpriteSheet::~SpriteSheet()
{
	if (bmp) bmp->Release();
	if (newname)
	{
		if (isSPR3)
			_wremove(newname);
		delete[] newname;
		newname = NULL;
	}
}

/*
void SpriteSheet::Draw()
{
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,							//Bitmap
		D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height),	//destination rectangle in this case start at 0, 0 and fill to the size of the bitmap
		1.0f,							//opacity
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)	//source rectangle
	);
}*/

void SpriteSheet::Draw(int index, float x, float y)
{
	D2D_RECT_F src = D2D1::RectF(
		(float)((index % spriteAcross) * spriteWidth),
		(float)((index / spriteAcross) * spriteHeight),
		(float)((index % spriteAcross) * spriteWidth + spriteWidth),
		(float)((index / spriteAcross) * spriteHeight + spriteHeight));

	D2D_RECT_F dest = D2D1::RectF(
		x, y,
		x + spriteWidth, y + spriteHeight);

	gfx->GetRenderTarget()->DrawBitmap(
		bmp,							//Bitmap
		dest,
		1.0f,							//opacity
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src
	);
}

void SpriteSheet::Draw(int index, D2D1_POINT_2F point, D2D1_RECT_F size)
{
	D2D1_RECT_F src = D2D1::RectF(0.0f, 0.0f, (float)bmp->GetSize().width, (float)bmp->GetSize().height);
	D2D1_RECT_F dest = D2D1::RectF(point.x, point.y, point.x + size.right, point.y + size.bottom);
	//possibly add in control for indexs; currently don't use them so won't bother
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src
	);
}

void SpriteSheet::Draw(int index, D2D1_POINT_2F p, D2D1_RECT_F src, D2D1_RECT_F size, bool keepRatio)
{
	D2D1_RECT_F dest;

	float xMod = 0.0f;
	float yMod = 0.0f;

	if (src.right < src.left)
	{
		float t = src.right;
		src.right = src.left;
		src.left = t;
	}
	if (src.bottom < src.top)
	{
		float t = src.bottom;
		src.bottom = src.top;
		src.top = t;
	}

	if (keepRatio)
	{
		if ((src.bottom - src.top) > (src.right - src.left))
		{
			float ratio = (src.right - src.left) / (src.bottom - src.top);
			if (ratio < 0.0f) ratio *= -1;
			xMod = (size.right - size.left) - ((size.right - size.left) * ratio);
			xMod /= 2;

			if (xMod < 0.0f) xMod *= -1;

			size.right = (size.right - size.left) * ratio;
			size.left = 0.0f;
		}
		else
		{
			float ratio = (src.bottom - src.top) / (src.right - src.left);
			if (ratio < 0.0f) ratio *= -1;
			yMod = (size.bottom - size.top) - ((size.bottom - size.top) * ratio);
			yMod /= 2;

			if (yMod < 0.0f) yMod *= -1;

			size.bottom = (size.bottom - size.top) * ratio;
			size.top = 0.0f;
		}
	}

	dest = D2D1::RectF(p.x + xMod, p.y + yMod, p.x + size.right + xMod, p.y + size.bottom + yMod);
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src
	);
}

char* SpriteSheet::decodeSPR3(const wchar_t* filename, size_t& imageBufferLen)
{
	wchar_t* fileExt = GetFileExt(filename);
	wchar_t* newPath = NULL;
/*	if (_wcsicmp(fileExt, L".spr3"))
	{
		newPath = new wchar_t[wcslen(filename) + 1];
		newPath[wcslen(filename)] = L'\0';
		memcpy(newPath, filename, sizeof(wchar_t) * wcslen(filename));
		delete[] fileExt;
		return newPath;
	}*/

	FILE* file = NULL;
	errno_t error = _wfopen_s(&file, filename, L"rb");
	if (error)
	{
		std::string msg = "Unable to open file. Error: " + std::to_string(error);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		if (file) fclose(file);
		return NULL;
	}

	char* buffer;
	fseek(file, 0, SEEK_END);
	int bufferlen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[bufferlen];
	fread(buffer, bufferlen, 1, file);
	fclose(file);

	int frames = 0;
	float version = 0.0f;
	int position = sizeof(float) * 4;
	memcpy(&m_FrameSpeed, buffer + position, sizeof(float));
	m_FrameSpeed /= 1000.0f;
	position += sizeof(float) + sizeof(double);
	memcpy(&frames, buffer + position, sizeof(int));
	//position += sizeof(int) + sizeof(bool) + sizeof(float) * frames * 4;
	position += sizeof(int);
	//custom bool irrelevant
	position += sizeof(bool);
	for (int i = 0; i < frames; i++)
	{
		D2D1_RECT_F tFrame = {};
		memcpy(&tFrame.left, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&tFrame.top, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&tFrame.right, buffer + position, sizeof(float));
		position += sizeof(float);
		memcpy(&tFrame.bottom, buffer + position, sizeof(float));
		position += sizeof(float);
		m_Frames.push(tFrame);
	}

	int extlen = 0;
	memcpy(&extlen, buffer + position, sizeof(int));
	position += sizeof(int);
	char* imageExt = new char[extlen + 1];
	imageExt[extlen] = '\0';
	memcpy(imageExt, buffer + position, extlen);
	position += extlen;

	imageBufferLen = 0;
	memcpy(&imageBufferLen, buffer + position, sizeof(int));
	position += sizeof(int);
	char* imageBuffer = new char[imageBufferLen];
	memcpy(imageBuffer, buffer + position, imageBufferLen);
	position += imageBufferLen;

	/*old method
	wchar_t* shortname = NULL;
	wchar_t* wExt = toWchar(imageExt);
	shortenName(filename, shortname);
	if (shortname)
		addExt(shortname, wExt, newname);
	else
		addExt(filename, wExt, newname);

	if (CreateDirectory(TEMP_DIR_NAME, NULL))
		SetFileAttributes(TEMP_DIR_NAME, FILE_ATTRIBUTE_HIDDEN);
	
	//std::wstring wStringName = L"delme\\";			//this is really lazy but whatever not planning on changing the path name anyway
	std::wstring wStringName = TEMP_DIR_NAME_W;
	wStringName.push_back('\\');
	wStringName.append(newname);

	delete[] newname;
	newname = new wchar_t[wStringName.size() + 1];
	newname[wStringName.size()] = '\0';
	memcpy(newname, &wStringName[0], wStringName.size() * sizeof(wchar_t));


	int tempFileBufferLen = 0;
	memcpy(&tempFileBufferLen, buffer + position, sizeof(int));
	position += sizeof(int);
	char* tempFileBuffer = new char[tempFileBufferLen];
	memcpy(tempFileBuffer, buffer + position, tempFileBufferLen);

	error = _wfopen_s(&file, newname, L"wb");
	if (!error)
	{
		fwrite(tempFileBuffer, tempFileBufferLen, 1, file);
		fclose(file);
	}
	*/

	delete[] imageExt;
/*	delete[] wExt;
	delete[] shortname;
	delete[] tempFileBuffer;*/
	delete[] fileExt;
	delete[] buffer;

	return imageBuffer;
}

void SpriteSheet::addExt(const wchar_t* filename, const wchar_t* ext, wchar_t*& newname)
{
	int len = (int)(wcslen(filename) + wcslen(ext));
	newname = new wchar_t[len + 1];
	swprintf_s(newname, len + 1, L"%s%s", filename, ext);
}

void SpriteSheet::shortenName(const wchar_t* filename, wchar_t*& newName)
{
	int len = (int)wcslen(filename);
	for (int i = len; i >= 0; i--)
	{
		if (filename[i] == 0x5C || filename[i] == 0x2F)
		{
			newName = new wchar_t[len - i + 1];
			memcpy(newName, &filename[i + 1], (len - i) * sizeof(wchar_t));
			return;
		}
	}
}

wchar_t* SpriteSheet::toWchar(const char* text)
{
	size_t numconv;
	int len = (int)(strlen(text) + 1);
	wchar_t* wc = new wchar_t[len];
	mbstowcs_s(&numconv, wc, len, text, len);
	return wc;
}

wchar_t* SpriteSheet::decodeSPR(const wchar_t* filename)
{
	wchar_t* fileExt = GetFileExt(filename);
	wchar_t* newPath = NULL;
	if (_wcsicmp(fileExt, L".spr"))
	{
		newPath = new wchar_t[wcslen(filename) + 1];
		newPath[wcslen(filename)] = L'\0';
		memcpy(newPath, filename, sizeof(wchar_t) * wcslen(filename));
		delete[] fileExt;
		return newPath;
	}

	FILE* file = NULL;
	errno_t error;
	error = _wfopen_s(&file, filename, L"r");
	if (error)
	{
		std::string msg = "Unable to open file. Error: " + std::to_string(error);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		if (file) fclose(file);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	int bufferlen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[bufferlen];
	fread(buffer, bufferlen, 1, file);
	fclose(file);

	int position = 0;
	char* line = NULL;

	while (position < bufferlen)
	{
		position += GetLine(buffer + position, line);

		char* t = SplitString(line);
		TrimString(line);
		TrimString(t);

		TranslateLine(line, t);

		if (t) delete[] t;
		delete[] line;
		line = NULL;
	}

	delete[] buffer;
	delete[] fileExt;

	return NULL;
}

void SpriteSheet::EmptyRectQueue(std::queue<D2D1_RECT_F>& q)
{
	std::queue<D2D1_RECT_F> empty;
	std::swap(q, empty);
}

void SpriteSheet::TranslateLine(const char* beforeEqual, const char* afterEqual)
{
	if (!_stricmp(beforeEqual, "version"))
	{

	}
	if (!_stricmp(beforeEqual, "BodyPath"))
	{
		size_t conv;
		const size_t size = strlen(afterEqual) + 1;
		if (newname) delete[] newname;
		newname = new wchar_t[strlen(afterEqual) + 1];
		mbstowcs_s(&conv, newname, size, afterEqual, size);
	}
	if (!_stricmp(beforeEqual, "FrameTime"))
	{
		m_FrameSpeed = (float)atof(afterEqual);
		m_FrameSpeed /= 1000.0f;
	}
	if (!_stricmp(beforeEqual, "FrameArray"))
	{
		EmptyRectQueue(m_Frames);
		char value[512] = {};
		int q = 0;
		int w = 0;
		D2D1_RECT_F tRect = {};

		for (int i = 0; i < (int)strlen(afterEqual); i++)
		{
			if (afterEqual[i] == ',' || afterEqual[i] == '\n' || afterEqual[i] == '\r')
			{
				switch (w)
				{
				case 0:
					tRect.left = (float)atof(value);
					q = 0;
					value[0] = '\0';
					w++;
					break;
				case 1:
					tRect.top = (float)atof(value);
					q = 0;
					value[0] = '\0';
					w++;
					break;
				case 2:
					tRect.right = (float)atof(value);
					q = 0;
					value[0] = '\0';
					w++;
					break;
				default:
					tRect.bottom = (float)atof(value);
					q = 0;
					value[0] = '\0';
					m_Frames.push(tRect);
					w = 0;
				}
			}
			else
			{
				value[q++] = afterEqual[i];
				value[q] = '\0';
			}
		}
	}

}

wchar_t* SpriteSheet::GetFileExt(const wchar_t* filename)
{
	wchar_t* ext = NULL;
	for (int i = (int)wcslen(filename); i > 0; i--)
	{
		if (filename[i] == L'.')
		{
			ext = new wchar_t[wcslen(filename) - i + 1];
			ext[wcslen(filename) - i] = '\0';
			memcpy(ext, filename + i, sizeof(wchar_t) * (wcslen(filename) - i));

			return ext;
		}
	}
	return ext;
}

void SpriteSheet::SetFrameSpeed(float frameSpeed)
{
	m_FrameSpeed = frameSpeed;
}

void SpriteSheet::AddFrame(D2D1_RECT_F frame)
{
	m_Frames.push(frame);
}

void SpriteSheet::NextFrame()
{
	D2D1_RECT_F temp = m_Frames.front();
	m_Frames.pop();
	m_Frames.push(temp);
}

void SpriteSheet::UpdateDrawInfo(double currentTime)
{
	if (!m_Frames.size())
	{
		AddFrame({ 0.0f, 0.f, (float)spriteWidth, (float)spriteHeight });
	}

	if (currentTime - m_LastTime >= m_FrameSpeed)
	{
		m_LastTime = currentTime;
		NextFrame();
	}
}

/*
void SpriteSheet::Draw(float transparency)
{	
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		dest,
		transparency,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		src
	);
}
*/

void SpriteSheet::Draw(D2D1_RECT_F target, D2D1_RECT_F src, float transparency)
{
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		target,
		transparency,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src
	);
}

D2D1_RECT_F SpriteSheet::GetFrame()
{
	if (!m_Frames.size())
	{
		AddFrame({ 0.0f, 0.f, (float)spriteWidth, (float)spriteHeight });
	}
	return m_Frames.front();
}

void SpriteSheet::LTrimString(char*& target)
{
	if (!target) return;
	int start = 0;

	for (int i = 0; i < (int)strlen(target); i++)
	{
		if (target[i] == '\n' || target[i] == '\r' || target[i] == ' ')
			continue;
		start = i;
		break;
	}

	char* newstr = new char[strlen(target) - start + 1];
	newstr[strlen(target) - start] = '\0';
	memcpy(newstr, target + start, strlen(target) - start);

	delete[] target;
	target = newstr;
}

void SpriteSheet::RTrimString(char*& target)
{
	if (!target) return;
	int end = 0;

	for (int i = (int)(strlen(target) - 1); i >= 0; i--)
	{
		if (target[i] == '\n' || target[i] == '\r' || target[i] == ' ')
			continue;
		end = i;
		break;
	}

	char* newstr = new char[strlen(target) - (strlen(target) - end) + 2];
	newstr[strlen(target) - (strlen(target) - end) + 1] = '\0';
	memcpy(newstr, target, strlen(target) - (strlen(target) - end) + 1);

	delete[] target;
	target = newstr;
}

void SpriteSheet::TrimString(char*& target)
{
	LTrimString(target);
	RTrimString(target);
}

int SpriteSheet::GetLine(const char* buffer, char*& target)
{
	if (target) delete[] target;
	target = NULL;
	int linelen = 0;

	for (int i = 0; i <= (int)strlen(buffer); i++)
	{
		if (i == strlen(buffer) || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == ';' || buffer[i] == 'þ')
		{
			linelen = i;
			break;
		}
	}

	target = new char[linelen + 1];
	target[linelen] = '\0';
	memcpy(target, buffer, linelen);

	return linelen + 1;
}

char* SpriteSheet::SplitString(char*& target)
{
	char* preEqual = 0;
	char* postEqual = 0;
	char* pos = strchr(target, '=');
	if (!pos) return NULL;

	preEqual = new char[strlen(target) - strlen(pos) + 1];
	preEqual[strlen(target) - strlen(pos)] = '\0';
	memcpy(preEqual, target, strlen(target) - strlen(pos));

	pos++;
	postEqual = new char[strlen(pos) + 1];
	postEqual[strlen(pos)] = '\0';
	memcpy(postEqual, pos, strlen(pos));

	delete[] target;
	target = preEqual;

	return postEqual;
}