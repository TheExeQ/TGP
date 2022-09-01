#include "SettingsPanel.h"
#include <Core/DX11.h>
#include <ImGui/imgui.h>

void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Clear Color", &DX11::myClearColor[0]);
	ImGui::End();
}
