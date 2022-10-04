#pragma once
#include "Core/Base.h"
#include "Renderer/Texture.h"
#include <filesystem>

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();

	void Init();
	void OnImGuiRender();

private:
	friend class EditorLayer;

	std::filesystem::path m_CurrentDirectory;

	Ref<Texture> m_DirectoryIcon;
	Ref<Texture> m_FileIcon;
};
