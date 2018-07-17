#pragma once
#include <vector>
#include "graphics.h"

class Animation
{
private:
	double m_LastUpdateTime = 0;
	int m_CurrentFrame = 0;
	int m_MaxFrames = 0;
	float m_FrameSpeed = 0.0f;
	std::vector<D2D1_RECT_F> m_Frames;
public:
	Animation();
	Animation(std::vector<D2D1_RECT_F> frames);
	~Animation();
	void NextFrame();
	void NextFrame(double currentUpdateTime);
	void PreviousFrame();
	void PreviousFrame(double currentUpdateTime);
	void EmptyFrames();
	void SetFrameSpeed(float speed);
	void SetFrames(std::vector<D2D1_RECT_F> frames);
	void AddFrame(D2D1_RECT_F frame);
	D2D1_RECT_F GetFrame();
};