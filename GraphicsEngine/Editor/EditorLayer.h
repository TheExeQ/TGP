#pragma once
#include "SceneHierarchyPanel.h"
#include "SettingsPanel.h"

enum ImGuiWindows
{
	SceneHierarchy,
	Settings,
	COUNT,
};

class EditorLayer
{
public:
	void OnRender();
	void WindowController();

private:
	bool myWindowEnabler[ImGuiWindows::COUNT];

	SceneHierarchyPanel mySceneHierarchyPanel;
	SettingsPanel mySettingsPanel;
};