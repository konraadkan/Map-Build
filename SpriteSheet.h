#pragma once

#include <wincodec.h>
#include <queue>
#include "graphics.h"

class SpriteSheet
{
private:
	float m_FrameSpeed = 0.33f;
	double m_LastTime = 0;
public:
	Graphics* gfx;
	ID2D1Bitmap* bmp;
	int spriteAcross;

	//constructor
	SpriteSheet(const wchar_t* filename, Graphics* gfx);
	SpriteSheet(const wchar_t* filename, Graphics* gfx, int spriteWidth, int spriteHeight);
	
	bool InitFromMemory(char* buffer, size_t bufferLen, Graphics* gfx);

	//destructor
	~SpriteSheet();

	//draw the bitmap to render target
	//void Draw();
	void Draw(int index, float x, float y);
	void Draw(int index, D2D1_POINT_2F point, D2D1_RECT_F size);
	void Draw(int index, D2D1_POINT_2F p, D2D1_RECT_F src, D2D1_RECT_F size, bool keepRatio = true);
	//void Draw(float transparency = 1.0f);
	//void Draw(D2D1_RECT_F pos, bool keepAspect = true, float transparency = 1.0f);
	void Draw(D2D1_RECT_F target, D2D1_RECT_F src, float transparency = 1.0f);

	void UpdateDrawInfo(double currentTime);

	int spriteWidth, spriteHeight;

	void SetFrameSpeed(float frameSpeed);
	void AddFrame(D2D1_RECT_F frame);
	void NextFrame();
	D2D1_RECT_F GetFrame();

	std::queue<D2D1_RECT_F> m_Frames;

	void EmptyRectQueue(std::queue<D2D1_RECT_F>& q);

	char* decodeSPR3(const wchar_t* filename, size_t& imageBufferLen);
	wchar_t* decodeSPR(const wchar_t* filename);
	wchar_t* GetFileExt(const wchar_t* filename);
	wchar_t* toWchar(const char* text);
	wchar_t* newname = NULL;
	
	void shortenName(const wchar_t* filename, wchar_t*& newName);
	void addExt(const wchar_t* filename, const wchar_t* ext, wchar_t*& newname);

	int GetLine(const char* buffer, char*& target);
	void LTrimString(char*& target);
	void RTrimString(char*& target);
	void TrimString(char*& target);
	char* SplitString(char*& target);

	void TranslateLine(const char* beforeEqual, const char* afterEqual);

	bool isSPR3 = false;
};