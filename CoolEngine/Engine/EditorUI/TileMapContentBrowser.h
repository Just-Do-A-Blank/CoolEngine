#pragma once
#include "ContentBrowser.h"
class TileMapContentBrowser : public ContentBrowser
{
public:
	const std::string& GetSelectedPath() const;
	const std::wstring& GetRelativePath() const;

protected:
	std::string m_selectedFilePath = "";
	std::wstring m_relativePath = L"";

	void CreateFileEntry(const WIN32_FIND_DATAA& kfileData) override;

private:

};

