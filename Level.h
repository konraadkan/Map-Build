#pragma once
#include "graphics.h"
#include "gfx_structs.h"
#include "SpriteSheet.h"

class Level
{
protected:
	static Graphics* gfx;
public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
	}

	static bool InRect(D2D1_RECT_F area, D2D1_POINT_2F p)
	{
		if (p.x >= area.left && p.x <= area.right)
		{
			if (p.y >= area.top && p.y <= area.bottom)
				return true;
		}
		return false;
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Render() = 0;
	virtual void Update(float timeTotal, float timeDelta, D2D1_POINT_2F p) = 0;
	virtual void Update(WPARAM wParam, D2D1_POINT_2F p) = 0;
	virtual void AddLayerMenu() = 0;
	virtual void Open(const wchar_t* filePath) = 0;
};