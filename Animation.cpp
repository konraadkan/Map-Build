#include "Animation.h"

Animation::Animation()
{
	//constructor
}

Animation::Animation(std::vector<D2D1_RECT_F> frames)
{
	m_Frames = frames;
}

Animation::~Animation()
{
	//deconstructor
	EmptyFrames();
}

void Animation::EmptyFrames()
{
	std::vector<D2D1_RECT_F> empty;
	std::swap(m_Frames, empty);
}

D2D1_RECT_F Animation::GetFrame()
{
	if (m_Frames.size())
	{
		if (m_CurrentFrame < m_Frames.size() && m_CurrentFrame >= 0)
			return m_Frames[m_CurrentFrame];
		else
		{
			m_CurrentFrame = 0;
			return m_Frames[m_CurrentFrame];
		}
	}
	return { 0.0f, 0.0f, 0.0f, 0.0f };
}

void Animation::NextFrame()
{
	m_CurrentFrame++;
	if (m_CurrentFrame >= m_Frames.size())
		m_CurrentFrame = 0;
}

void Animation::PreviousFrame()
{
	m_CurrentFrame--;
	if (m_CurrentFrame < 0)
		m_CurrentFrame = m_Frames.size() - 1;
}

void Animation::SetFrameSpeed(float speed)
{
	m_FrameSpeed = speed;
}

void Animation::NextFrame(double currentUpdateTime)
{
	if ((currentUpdateTime - m_LastUpdateTime) > m_FrameSpeed)
	{
		NextFrame();
		m_LastUpdateTime = currentUpdateTime;
	}
}

void Animation::PreviousFrame(double currentUpdateTime)
{
	if ((currentUpdateTime - m_LastUpdateTime) > m_FrameSpeed)
	{
		PreviousFrame();
		m_LastUpdateTime = currentUpdateTime;
	}
}

void Animation::SetFrames(std::vector<D2D1_RECT_F> frames)
{
	m_Frames = frames;
}

void Animation::AddFrame(D2D1_RECT_F frame)
{
	m_Frames.push_back(frame);
}