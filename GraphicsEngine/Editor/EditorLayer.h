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

	std::filesystem::path GetContentBrowserPath() const { return myContentBrowserPanel.m_CurrentDirectory; };

private:
	bool myWindowEnabler[ImGuiWindows::COUNT];

	SceneHierarchyPanel mySceneHierarchyPanel;
	ContentBrowserPanel myContentBrowserPanel;
	SettingsPanel mySettingsPanel;
};