#include "SettingsPanel.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include <Core/DX11.h>
#include <ImGui/imgui.h>

void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Clear Color", &DX11::myClearColor[0]);
	if (ImGui::Button("Save"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.Serialize("../Assets/Scenes/default.scene");
	}
	ImGui::End();
}
