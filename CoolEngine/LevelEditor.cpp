#include "LevelEditor.h"

LevelEditor::LevelEditor(ID3D11Device* pdevice)
{
	m_pDevice = pdevice;
}

void LevelEditor::InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* phwnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;

	(void)io;

	ImGui::StyleColorsDark();

	m_phwnd = phwnd;

	ImGui_ImplWin32_Init(*m_phwnd);
	ImGui_ImplDX11_Init(pdevice, pcontext);
}

void LevelEditor::ShutdownIMGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void LevelEditor::DrawLevelEditor()
{
	ImGui::Begin("Level Editor");
	ImGui::End();
}

