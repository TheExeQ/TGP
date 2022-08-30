#pragma once
#include <windows.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

class ImGuiLayer
{
public:
	ImGuiLayer() {};
	~ImGuiLayer() = default;

	void OnAttach(HWND hwnd);
	void OnDetach();

	void Begin();
	void End();
};
