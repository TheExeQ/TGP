#pragma once
#include "Scene/Entity.h"
#include <imgui/imgui.h>

/*
 
class SceneViewportPanel
{
private:
	friend class EditorLayer;
	friend class EditorCamera;
	friend class WindowContainer;

	void Init();
	void OnImGuiRender(Entity aSelectedEntity);
	void OnResize();

	static SceneViewportPanel& Get() { return *myInstace; }

	bool myOpened = true;
	bool myShowGizmos = true;
	ImGuizmo::OPERATION myGizmoOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE myGizmoMode = ImGuizmo::LOCAL;

	glm::vec2 myViewportSize = { 0.f, 0.f };

	inline static SceneViewportPanel* myInstace = nullptr;
};

*/
