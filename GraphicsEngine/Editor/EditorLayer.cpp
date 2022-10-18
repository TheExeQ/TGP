#include "gepch.h"
#include "EditorLayer.h"
#include <ImGui/imgui.h>

const char* g_ImGuiWindowNames[ImGuiWindows::COUNT] =
{
	"Hierarchy/Properties",
	"ContentBrowser",
	"Settings"
};

void EditorLayer::Init()
{
	myContentBrowserPanel.Init();
}

void EditorLayer::OnRender()
{
	WindowController();
	if (myWindowEnabler[ImGuiWindows::SceneHierarchy])
		mySceneHierarchyPanel.OnImGuiRender();
	if (myWindowEnabler[ImGuiWindows::ContentBrowser])
		myContentBrowserPanel.OnImGuiRender();
	if (myWindowEnabler[ImGuiWindows::Settings])
		mySettingsPanel.OnImGuiRender();
}

void EditorLayer::WindowController()
{
	ImGui::Begin("Windows");
	for (int i = 0; i < ImGuiWindows::COUNT; i++)
	{
		ImGui::Checkbox(g_ImGuiWindowNames[i], &myWindowEnabler[i]);
	}
	ImGui::End();
}
