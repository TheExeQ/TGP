#pragma once
#include "SceneHierarchyPanel.h"
#include "ContentBrowserPanel.h"
#include "SettingsPanel.h"

enum ImGuiWindows
{
	SceneHierarchy,
	ContentBrowser,
	Settings,
	COUNT,
};

class EditorLayer
{
public:
	void Init();
	void OnRender();
	void WindowController();
	void Dockspace();

	std::filesystem::path GetContentBrowserPath() const { return myContentBrowserPanel.myCurrentDirectory; };

private:
	bool myWindowEnabler[ImGuiWindows::COUNT];

	SceneHierarchyPanel mySceneHierarchyPanel;
	ContentBrowserPanel myContentBrowserPanel;
	SettingsPanel mySettingsPanel;
};