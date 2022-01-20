#pragma once

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/Scene/Scene.h"
#include "Engine/ResourceDefines.h"

class LevelEditor
{
public:
	LevelEditor(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();

	void DrawLevelEditor();

private:
	ID3D11Device*		m_pDevice;
	HWND*				m_phwnd;
	
};