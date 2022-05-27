#pragma once
#include "Engine/Structure/Singleton.h"

#define NUM_KEYCODES 256

class Inputs : public Singleton<Inputs>
{
public:
	bool IsKeyPressed(int ikeycode);

	void Update(HWND* hWnd, UINT* message, WPARAM* wParam, LPARAM* lParam);


	Inputs() {}
	~Inputs() {}

	void Initialize();
	void Update();

	bool m_keyState[NUM_KEYCODES] = { false };
};