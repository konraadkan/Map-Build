#include "button_mapper.h"
#include "GameController.h"
#include <vector>

bool inRect(D2D1_RECT_F rect, D2D1_POINT_2F p)
{
	if (p.x < rect.left || p.y < rect.top || p.x > rect.right || p.y > rect.bottom)
		return false;
	return true;
}

ButtonMapper::ButtonMapper(KeyboardKeys& m_Keyboard, Graphics* graphics, D2D1_SIZE_F WindowSize)
{
	std::vector<KeyClass> keychanges;
	std::vector<long> keyids;
	MSG msg;
	bool loop = true;
	gfx = graphics;
	m_WindowSize = WindowSize;
	rowsize = (long)(m_WindowSize.width / 480);
	buttonsize = D2D1::SizeF(480.0f, 64.0f);
	commitRect = D2D1::RectF(m_WindowSize.width / 2.0f, m_WindowSize.height - 64.0f, m_WindowSize.width / 2.0f + buttonsize.width, m_WindowSize.height);
	cancelRect = D2D1::RectF(commitRect.right, commitRect.top, commitRect.right + buttonsize.width, commitRect.bottom);
	while (loop)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_MOUSEMOVE)
				GameController::p = GameController::GetMousePositionForCurrentDpi(msg.lParam, gfx->GetID2DFactory());
		}
		if (GetButtonPressed() == VK_ESCAPE)
			loop = false;
		long value = (long)(GameController::p.x / buttonsize.width);
		value += (long)(GameController::p.y / buttonsize.height) * rowsize;
		if (GameController::p.x > (buttonsize.width * rowsize + 1))
			value = -1;
		Render(m_Keyboard, GameController::p, value);
		if (value < m_Keyboard.m_NumberKeys && value >= 0)
		{
			if (GetButtonPressed() == VK_LBUTTON)
			{
				Sleep(250);
				long buttonPressed = -1;
				while (buttonPressed == -1)
				{
					buttonPressed = GetButtonPressed();
					gfx->BeginDraw();
					gfx->ClearScreen({ 1.0,1.0,1.0,1.0 });
					gfx->DrawText("Press the new key", { 0,0,WindowSize.width,WindowSize.height }, { 0,0,0,1 }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
					gfx->EndDraw();
				}
				KeyClass* k = m_Keyboard.GetKeyClass(value);
				keyids.push_back(value);
				keychanges.push_back(*k);
				std::string sname = k->Name;
				if (k)
				{
					k->SetKey(buttonPressed);
					k->Name = sname;
				}
				Sleep(250);
			}
		}
		else if (inRect(commitRect, GameController::p))
		{
			//if commit clicked
			if (GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				FILE* file = NULL;
				size_t bsize = 0;
				errno_t error = fopen_s(&file, "keys.ini", "rb");
				if (error)
				{
					MessageBox(NULL, "Error openning keys.ini", "Error", MB_OK | MB_ICONERROR);
				}

				fseek(file, 0, SEEK_END);
				bsize = ftell(file);
				fseek(file, 0, SEEK_SET);
				char* buffer = new char[bsize];
				fread(buffer, bsize, 1, file);
				fclose(file);

				error = fopen_s(&file, "keys.bak", "wb");
				if (error)
				{
					MessageBox(NULL, "Error openning keys.bak", "Error", MB_OK | MB_ICONERROR);
				}
				fwrite(buffer, bsize, 1, file);
				fclose(file);
				delete[] buffer;

				error = fopen_s(&file, "keys.ini", "w");
				if (error)
				{
					MessageBox(NULL, "Error openning keys.ini", "Error", MB_OK | MB_ICONERROR);
				}
				for (long i = 0; i < m_Keyboard.m_NumberKeys; i++)
				{
					KeyClass* k = m_Keyboard.GetKeyClass(i);
					fprintf(file, "%s=%s\n", k->Name.c_str(), k->GetBoundValue().c_str());
				}
				fclose(file);
				loop = false;
			}
		}
		else if (inRect(cancelRect, GameController::p))
		{
			//if cancel clicked
			if (GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				if (keychanges.size() != keyids.size())
				{
					MessageBox(NULL, "There was an error reverting changes. These changes were not commited to the .ini file but may remain in the current instance.", "Error", MB_OK | MB_ICONERROR);
					loop = false;
					break;
				}
				while (keychanges.size())
				{
					KeyClass* k = m_Keyboard.GetKeyClass(keyids.back());
					k->SetKey(keychanges.back().GetVKey(), keychanges.back().Name);
					keychanges.pop_back();
					keyids.pop_back();
				}
				loop = false;
			}
		}
	}
}

ButtonMapper::~ButtonMapper()
{

}

void ButtonMapper::Render(KeyboardKeys m_Keyboard, D2D1_POINT_2F p, long value)
{
	gfx->BeginDraw();
	gfx->ClearScreen({ 1.0f,1.0f,1.0f,1.0f });
	KeyClass* k = NULL;
	float x = 100.0f;
	float y = 0.0f;
	
	for (long i = 0; i < m_Keyboard.m_NumberKeys; i++)
	{
		if (!(i % rowsize) && i != 0)
		{
			x = 100.0f;
			y += 64.0f;
		}
		k = m_Keyboard.GetKeyClass(i);			
		if (k)
		{
			gfx->DrawText(k->Name, { x, y, x + buttonsize.width, y + buttonsize.height }, { 0.0f,0.0f,0.0f,1.0f }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			gfx->DrawTextSmall(k->GetBoundValue(), { x, y + buttonsize.height / 2.0f, x + buttonsize.width, y + buttonsize.height }, { 0.0f, 0.0f,0.0f,1.0f }, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}
		x += buttonsize.width;
	}

	gfx->DrawText(L"Commit", commitRect, { 0,0,0,1 });
	gfx->DrawText(L"Cancel", cancelRect, { 0,0,0,1 });
	
	if (p.x < (buttonsize.width * rowsize + 1) && value < m_Keyboard.m_NumberKeys && value >= 0)
	{
		p.x = (long)(p.x / buttonsize.width);
		p.y = (long)(p.y / buttonsize.height);
		p.x *= buttonsize.width;
		p.y *= buttonsize.height;
		gfx->DrawRect({ p.x, p.y, p.x + buttonsize.width, p.y + buttonsize.height }, { 1.0f, 0.0f,0.0f,1.0f }, 5.0f);
		gfx->FillRect({ p.x, p.y, p.x + buttonsize.width, p.y + buttonsize.height }, { 1.0f,  0.41f, 0.71f, 0.8f });
	}
	else if (inRect(commitRect, p))
	{
		gfx->DrawRect(commitRect, { 1.0f,0.0f,0.0f,1.0f }, 5.0f);
		gfx->FillRect(commitRect, { 1.0f,0.41f,0.71f,0.8f });
	}
	else if (inRect(cancelRect, p))
	{
		gfx->DrawRect(cancelRect, { 1.0f,0.0f,0.0f,1.0f }, 5.0f);
		gfx->FillRect(cancelRect, { 1.0f,0.41f,0.71f,0.8f });
	}
	gfx->EndDraw();
}

long ButtonMapper::GetButtonPressed()
{
	for (long i = 0; i < 0xFF; i++)
	{
		if (GetAsyncKeyState(i) < 0)
			return i;
	}
	return -1;
}
