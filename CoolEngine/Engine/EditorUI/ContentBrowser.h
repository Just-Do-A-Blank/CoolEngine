#pragma once

#if EDITOR
class ContentBrowser
{
public:
	ContentBrowser();

	void Draw();

protected:
	std::string m_sfilepath;
	std::string m_sfileName;

	virtual void CreateDirectoryEntry(const WIN32_FIND_DATAA& kfileData);
	virtual void CreateFileEntry(const WIN32_FIND_DATAA& kfileData);
	void CreateEntry(const WIN32_FIND_DATAA& kfileData);

private:

};
#endif
