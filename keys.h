#pragma once
#include <windows.h>

class KeyClass
{
private:
	long v_Key = 0;
	double m_LastTime = 0;
	double m_Delay = 0.25;
public:
	KeyClass() {}
	KeyClass(const long k, double delay = 0.25) { SetKey(k); SetDelay(delay); }
	~KeyClass() {}
	void SetKey(const long vKey);
	void SetDelay(double delay);
	void ResetTimer();
	bool IsPressed(double currentTime, bool usedelay = true);
	bool IsPressed(double currentTime, double delay);
	long KeyPressedInRange(const long low, const long high);
	double TimePassed(double currentTime)
	{
		return currentTime - m_LastTime;
	}
	void UpdateTime(double currentTime)
	{
		m_LastTime = currentTime;
	}
};