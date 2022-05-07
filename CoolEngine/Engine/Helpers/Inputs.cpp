#include "Inputs.h"
#include "Engine/Managers/Events/EventManager.h"

#if EDITOR
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/EditorUI/EditorUI.h"
#endif

bool Inputs::IsKeyPressed(int ikeycode)
{
	if (ikeycode >= NUM_KEYCODES || ikeycode < 0)
	{
		return false;
	}

	return m_keyState[ikeycode];
}

void Inputs::Update(HWND* hWnd, UINT* message, WPARAM* wParam, LPARAM* lParam)
{
	switch (*message)
	{
	case(WM_KEYDOWN):
	case(WM_KEYUP):
	{
#if EDITOR
		ImGuiIO io = ImGui::GetIO();

		if (io.WantCaptureKeyboard == true)
		{
			return;
		}
#endif
	}
	break;

	case(WM_LBUTTONDOWN):
	case(WM_MBUTTONDOWN):
	case(WM_RBUTTONDOWN):
	case(WM_XBUTTONDOWN):
	case(WM_LBUTTONUP):
	case(WM_MBUTTONUP):
	case(WM_RBUTTONUP):
	case(WM_XBUTTONUP):
	case(WM_MOUSEMOVE):
	{
#if EDITOR
		ImGuiIO io = ImGui::GetIO();

		if (io.WantCaptureMouse == true && EditorUI::GetIsViewportHovered() == false)
		{
			return;
		}
#endif
	}
	break;

	}

	switch (*message)
	{

		//Keyboard events
	case(WM_KEYDOWN):

		m_keyState[*wParam] = true;
		break;
	case(WM_KEYUP):
		m_keyState[*wParam] = false;
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


void Inputs::Update()
{
	for (int i = 0; i < NUM_KEYCODES; ++i)
	{
		if (m_keyState[i])
		{
			EventManager::Instance()->AddEvent(new KeyPressedEvent(i));
		}
	}
}

