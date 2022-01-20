#pragma once

class Inputs
{
public:


	void Update(HWND* hWnd, UINT* message, WPARAM* wParam, LPARAM* lParam);


	Inputs() {}
	~Inputs() {}

	void Initialize();
	void Update();

	bool m_keyState[256] = { false };
};