#include "Inputs.h"
#include "Engine/Managers/Events/EventManager.h"

void Inputs::Update(HWND* hWnd, UINT* message, WPARAM* wParam, LPARAM* lParam)
{
	switch (*message)
	{

		//Keyboard events
	case(WM_KEYDOWN):
		EventManager::Instance()->AddEvent(new KeyPressedEvent(*wParam));
		break;
	case(WM_KEYUP):
		EventManager::Instance()->AddEvent(new KeyReleasedEvent(*wParam));
		break;




		//Mouse events
	case(WM_LBUTTONDOWN):
		EventManager::Instance()->AddEvent(new MouseButtonPressedEvent(*wParam));
		break;
	case(WM_MBUTTONDOWN):
		EventManager::Instance()->AddEvent(new MouseButtonPressedEvent(*wParam));
		break;
	case(WM_RBUTTONDOWN):
		EventManager::Instance()->AddEvent(new MouseButtonPressedEvent(*wParam));
		break;
	case(WM_XBUTTONDOWN):
		EventManager::Instance()->AddEvent(new MouseButtonPressedEvent(*wParam));
		break;


	case(WM_LBUTTONUP):
		EventManager::Instance()->AddEvent(new MouseButtonReleasedEvent(*wParam));
		break;
	case(WM_MBUTTONUP):
		EventManager::Instance()->AddEvent(new MouseButtonReleasedEvent(*wParam));
		break;
	case(WM_RBUTTONUP):
		EventManager::Instance()->AddEvent(new MouseButtonReleasedEvent(*wParam));
		break;
	case(WM_XBUTTONUP):
		EventManager::Instance()->AddEvent(new MouseButtonReleasedEvent(*wParam));
		break;

	case(WM_MOUSEMOVE):
		EventManager::Instance()->AddEvent(new MouseMovedEvent(GET_X_LPARAM(*lParam), GET_Y_LPARAM(*lParam)));
		break;

	}





}
