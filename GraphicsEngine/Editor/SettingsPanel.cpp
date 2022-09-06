#include "SettingsPanel.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "Core/GraphicsEngine.h"
#include <Core/DX11.h>
#include <ImGui/imgui.h>

void SettingsPanel::OnImGuiRender()
{
	static std::string preset1;
	static std::string preset2;
	static std::string savefilename;

	static std::array<float, 4> colorSlider = DX11::myClearColor;
	static std::array<float, 4> preset1Color;
	static std::array<float, 4> preset2Color;

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
	if (ImGui::InputText("First preset filename", buffer, sizeof(buffer)))
	{
		preset1 = std::string(buffer);
	}
	char buffer2[256];
	memset(buffer2, 0, sizeof(buffer2));
	std::strncpy(buffer2, preset2.c_str(), sizeof(buffer2));
	if (ImGui::InputText("Second preset filename", buffer2, sizeof(buffer2)))
	{
		preset2 = std::string(buffer2);
	}

	static float blend = 0.f;
	static bool blendActive = false;
	ImGui::SliderFloat("##Blend", &blend, 0.f, 1.f);
	ImGui::SameLine();
	ImGui::Checkbox("Blending", &blendActive);

	if (blendActive)
	{
		std::array<float, 4> result;

		for (uint32_t i = 0; i < 4; i++)
		{
			result[i] = preset1Color[i] + (preset2Color[i] * blend);
		}

		DX11::myClearColor = result;
	}
	else
	{
		DX11::myClearColor = colorSlider;
	}

	char buffer3[256];
	memset(buffer3, 0, sizeof(buffer3));
	std::strncpy(buffer3, savefilename.c_str(), sizeof(buffer3));
	if (ImGui::InputText("Save preset filename", buffer3, sizeof(buffer3)))
	{
		savefilename = std::string(buffer3);
	}

	if (ImGui::Button("Load presets"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.DeserializeSettings((std::string("../Assets/Settings/") + preset2).c_str());
		preset2Color = DX11::myClearColor;

		serializer.DeserializeSettings((std::string("../Assets/Settings/") + preset1).c_str());
		preset1Color = DX11::myClearColor; 
	}
	if (ImGui::Button("Save Preset"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.SerializeSettings((std::string("../Assets/Settings/") + savefilename).c_str());
	}
	if (ImGui::Button("Save Scene"))
	{
		SceneSerializer serializer(Scene::GetActiveScene());

		serializer.Serialize("../Assets/Scenes/default");
	}
	ImGui::End();
}
