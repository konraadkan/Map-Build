#include "keys.h"

void KeyClass::SetKey(long vKey)
{
	v_Key = vKey;
}

void KeyClass::SetDelay(double delay)
{
	m_Delay = delay;
}

bool KeyClass::IsPressed(double currentTime, bool usedelay)
{
	if (usedelay)
	{
		if (currentTime - m_LastTime < m_Delay) 
			return false;
	}
	if (v_Key)
	{
		if (GetKeyState(v_Key) < 0)
		{
			m_LastTime = currentTime;
			return true;
		}
	}
	return false;
}

bool KeyClass::IsPressed(double currentTime, double delay)
{
	if (currentTime - m_LastTime < delay)
		return false;
	if (v_Key)
	{
		if (GetKeyState(v_Key) < 0)
		{
			m_LastTime = currentTime;
			return true;
		}
	}
	return false;
}

void KeyClass::ResetTimer()
{
	m_LastTime = 0;
}

long KeyClass::KeyPressedInRange(const long low, const long high)
{
	for (int i = low; i <= high; i++)
	{
		if (GetKeyState(i) < 0) 
			return i;
	}
	return -1;
}