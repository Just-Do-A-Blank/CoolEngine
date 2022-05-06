#include "ContentBrowser.h"
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Managers/GameManager.h"


ContentBrowser::ContentBrowser()
{
	m_sfilepath = GameManager::GetInstance()->GetWorkingDirectory();
}

void ContentBrowser::Draw()
{
	ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Back"))
		{
			int ilastSlashPos = m_sfilepath.find_last_of('\\');

			if (ilastSlashPos != std::string::npos)
			{
				m_sfilepath.erase(ilastSlashPos);
			}
		}

		ImGui::EndMenuBar();
	}

	std::string ssearchPath = m_sfilepath + "\\*.*";

	WIN32_FIND_DATAA fileData;

	HANDLE fileHandle = FindFirstFileA(ssearchPath.c_str(), &fileData);

	std::vector<WIN32_FIND_DATAA> fileDatas;
	std::vector<WIN32_FIND_DATAA> directoryDatas;

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN == true && fileData.cFileName[0] != '.')
		{
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				directoryDatas.push_back(fileData);
			}
			else
			{
				fileDatas.push_back(fileData);
			}
		}

		while (FindNextFileA(fileHandle, &fileData) == true)
		{
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN == false || fileData.cFileName[0] == '.')
			{
				continue;
			}

			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				directoryDatas.push_back(fileData);
			}
			else
			{
				fileDatas.push_back(fileData);
			}
		}

		FindClose(fileHandle);
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	for (int i = 0; i < directoryDatas.size(); ++i)
	{
		CreateDirectoryEntry(directoryDatas[i]);
	}

	for (int i = 0; i < fileDatas.size(); ++i)
	{
		CreateFileEntry(fileDatas[i]);
	}
	ImGui::PopStyleColor();

	ImGui::End();

	fileDatas.clear();
	directoryDatas.clear();
}

void ContentBrowser::CreateDirectoryEntry(const WIN32_FIND_DATAA& kfileData)
{
	if (ImGui::Button(kfileData.cFileName) == true)
	{
		std::string tempString = std::string(kfileData.cFileName);

		m_sfilepath += "\\" + tempString;
	}
}

void ContentBrowser::CreateFileEntry(const WIN32_FIND_DATAA& kfileData)
{
	ImGui::Text(kfileData.cFileName);

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID) == true)
	{
		std::string filepath = m_sfilepath + "\\" + std::string(kfileData.cFileName);
		ImGui::SetDragDropPayload("ContentBrowser", filepath.c_str(), filepath.size() * sizeof(char), ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}
}

void ContentBrowser::CreateEntry(const WIN32_FIND_DATAA& kfileData)
{
	if (kfileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN == false || kfileData.cFileName[0] == '.')
	{
		return;
	}

	if (kfileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		CreateDirectoryEntry(kfileData);
	}
	else
	{
		CreateFileEntry(kfileData);
	}
}
