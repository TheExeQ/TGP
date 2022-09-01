#pragma once
#include "Core/Base.h"
#include "Scene/Entity.h"

class SceneHierarchyPanel
{
public:
	void OnImGuiRender();
	Entity GetSelectedEntity() const { return mySelectionContext; };

private:
	template<typename T>
	void DisplayAddComponentEntry(const std::string& entryName);

	void DrawEntityNode(Entity aEntity);
	void DrawComponents(Entity aEntity);

	Ref<Scene> myContext = nullptr;
	Entity mySelectionContext;
};
