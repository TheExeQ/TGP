#include "gepch.h"
#include "SettingsPanel.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "Core/GraphicsEngine.h"
#include "Commands/CommandStack.h"

#include <Core/DX11.h>
#include <ImGui/imgui.h>
#include <stdio.h>
#include <cmath>

void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Clear Color", &colorSlider[0]);

	if (ImGui::IsWindowFocused())
	{
		GraphicsEngine::movementActive = false;
	}
	else
	{
		GraphicsEngine::movementActive = true;
	}

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	std::strncpy(buffer, preset1.c_str(), sizeof(buffer));
	if (ImGui::InputText("##Fpreset", buffer, sizeof(buffer)))
	{
		preset1 = std::string(buffer);
	}
	ImGui::SameLine();
	if (ImGui::Button("First Preset"))
	{
		auto temp = DX11::myClearColor;
		SceneSerializer serializer(Scene::GetActiveScene());
		serializer.DeserializePreset((std::string("../Assets/Settings/") + preset1).c_str());
		preset1Color = DX11::myClearColor;
		DX11::myClearColor = temp;
	}

	char buffer2[256];
	memset(buffer2, 0, sizeof(buffer2));
	std::strncpy(buffer2, preset2.c_str(), sizeof(buffer2));
	if (ImGui::InputText("##Spreset", buffer2, sizeof(buffer2)))
	{
		preset2 = std::string(buffer2);
	}
	ImGui::SameLine();
	if (ImGui::Button("Second Preset"))
	{
		auto temp = DX11::myClearColor;
		SceneSerializer serializer(Scene::GetActiveScene());
		serializer.DeserializePreset((std::string("../Assets/Settings/") + preset2).c_str());
		preset2Color = DX11::myClearColor;
		DX11::myClearColor = temp;
	}

	static bool singleFrameUpdate = true;

	static float blendOld = blend;
	
	if (singleFrameUpdate)
	{
		blendOld = blend;
	}

	if (ImGui::SliderFloat("##Blend", &blend, 0.f, 1.f))
	{
		singleFrameUpdate = false;
	}

	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		Ref<ValueCommand<float>> command = CreateRef<ValueCommand<float>>(&blend, blendOld, blend);
		CommandManager::DoCommand(command);
		singleFrameUpdate = true;
	}

	ImGui::SameLine();
	ImGui::Checkbox("Blending", &blendActive);

	if (blendActive)
	{
		std::array<float, 4> result;

		for (uint32_t i = 0; i < 4; i++)
		{
			result[i] = std::lerp(preset1Color[i], preset2Color[i], blend);
		}

		DX11::myClearColor = result;
	}
	else
	{
		DX11::myClearColor = preset1Color;
	}

	char buffer3[256];
	memset(buffer3, 0, sizeof(buffer3));
	std::strncpy(buffer3, savefilename.c_str(), sizeof(buffer3));
	if (ImGui::InputText("Save/Delete preset filename", buffer3, sizeof(buffer3)))
	{
		savefilename = std::string(buffer3);
	}

	if (ImGui::Button("Save Preset"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		DX11::myClearColor = colorSlider;
		serializer.SerializePreset((std::string("../Assets/Settings/") + savefilename).c_str());
		DX11::myClearColor = preset1Color;
	}
	if (ImGui::Button("Delete Preset"))
	{
		std::remove((std::string("../Assets/Settings/") + savefilename + ".preset").c_str());
	}
	if (ImGui::Button("Save Settings"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.SerializeSettings(std::string("../Assets/Settings/default").c_str());
	}
	if (ImGui::Button("Save Scene"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.Serialize("../Assets/Scenes/default");
	}
	ImGui::End();
}
