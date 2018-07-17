#include "ItemMenuItem.h"

void ItemMenuItem::UpdateSprite(D2D1_POINT_2F position, D2D1_RECT_F frame, D2D1_RECT_F size, bool keepRatio)
{
	src = frame;

	float xMod = 0.0f;
	float yMod = 0.0f;

	if (src.bottom < src.top)
	{
		float t = src.bottom;
		src.bottom = src.top;
		src.top = t;
	}
	if (src.right < src.left)
	{
		float t = src.right;
		src.right = src.left;
		src.left = t;
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

	dest = D2D1::RectF(position.x + xMod, position.y + yMod, position.x + size.right + xMod, position.y + size.bottom + yMod);
}