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

	std::filesystem::path myCurrentDirectory;

	Ref<Texture> myDirectoryIcon;
	Ref<Texture> myFileIcon;
};
