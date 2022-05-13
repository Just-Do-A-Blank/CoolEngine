#include "TileMapContentBrowser.h"
#include "Engine/Includes/IMGUI/imgui.h"

const std::string& TileMapContentBrowser::GetSelectedPath() const
{
	return m_selectedFilePath;
}

const std::wstring& TileMapContentBrowser::GetRelativePath() const
{
	return m_relativePath;
}

void TileMapContentBrowser::CreateFileEntry(const WIN32_FIND_DATAA& kfileData)
{
	std::string pathToTest = m_sfilepath + "\\" + std::string(kfileData.cFileName);
	bool selected = pathToTest == m_selectedFilePath;

	ImGui::Selectable("##unique_id", &selected);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		if (selected == true)
		{
			m_selectedFilePath = "";
		}
		else
		{
			m_selectedFilePath = pathToTest;
			m_relativePath = std::wstring(m_selectedFilePath.begin(), m_selectedFilePath.end());

			//Check if that path points to an asset in the resources folder
			int index = m_relativePath.find(L"Resources");

			if (index == -1)
			{
				m_relativePath = L"";
			}
			else
			{
				//Get relative file path
				m_relativePath = m_relativePath.substr(index);
			}
		}

	}

	ImGui::SameLine();
	ContentBrowser::CreateFileEntry(kfileData);
}
