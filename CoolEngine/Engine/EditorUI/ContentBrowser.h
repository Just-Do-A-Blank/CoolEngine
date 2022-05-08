#pragma once
class ContentBrowser
{
public:
	ContentBrowser();

	void Draw();

protected:

private:
	std::string m_sfilepath;

	void CreateDirectoryEntry(const WIN32_FIND_DATAA& kfileData);
	void CreateFileEntry(const WIN32_FIND_DATAA& kfileData);
	void CreateEntry(const WIN32_FIND_DATAA& kfileData);
};

