#pragma once
#include <windows.h>
#include <string>

class KeyClass
{
private:
	long v_Key = 0;
	double m_LastTime = 0;
	double m_Delay = 0.25;
public:
	KeyClass() {}
	KeyClass(const long k, double delay = 0.25) { SetKey(k); SetDelay(delay); }
	KeyClass(const std::string s, const long k, double delay = 0.25) { SetKey(k, s); SetDelay(delay); }
	~KeyClass() {}
	void SetKey(const long vKey, const std::string s = "Default");
	void SetDelay(double delay);
	void ResetTimer();
	bool IsPressed(double currentTime, bool usedelay = true);
	bool IsPressed(double currentTime, double delay);
	long KeyPressedInRange(const long low, const long high);
	long GetVKey() { return v_Key; }
	double TimePassed(double currentTime)
	{
		return currentTime - m_LastTime;
	}
	void UpdateTime(double currentTime)
	{
		m_LastTime = currentTime;
	}
	std::string Name;
	std::string GetBoundValue();
};

struct KeyboardKeys
{
	long m_NumberKeys = 23;
	KeyClass* GetKeyClass(const long pos);

	KeyClass m_UpKey;
	KeyClass m_DownKey;
	KeyClass m_LeftKey;
	KeyClass m_RightKey;
	KeyClass m_ZoomInKey;
	KeyClass m_ZoomOutKey;
	KeyClass m_NextTurnKey;
	KeyClass m_PrevTurnKey;
	KeyClass m_SelectKey;
	KeyClass m_EraseKey;
	KeyClass m_DrawKey;
	KeyClass m_RulerKey;
	KeyClass m_IncreaseSize;
	KeyClass m_DecreaseSize;
	KeyClass m_InsertKey;
	KeyClass m_TagKey;
	KeyClass m_BuffKey;
	KeyClass m_DebuffKey;
	KeyClass m_UnconsciousKey;
	KeyClass m_OnFireKey;
	KeyClass m_ResetPositionKey;
	KeyClass m_RotationKey;
	KeyClass m_SetGeometry;

	KeyClass m_Range;				//this keepclass works differently from the rest; rather than checking for a specific key, it checks to see if a number or letter was pushed
};
