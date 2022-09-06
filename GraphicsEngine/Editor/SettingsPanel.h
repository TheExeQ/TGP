#pragma once
#include <array>
#include <string>

class SettingsPanel
{
public:
	void OnImGuiRender();

	inline static std::string savefilename;

	inline static std::array<float, 4> colorSlider;
	inline static std::array<float, 4> preset1Color;
	inline static std::array<float, 4> preset2Color;

	inline static std::string preset1;
	inline static std::string preset2;
};
