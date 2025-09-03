// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "ColorThemeController.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

namespace UI
{

	using json = nlohmann::json;

	static inline const json ColorToJson(const ImVec4& color) {
		return json::array({ color.x, color.y, color.z, color.w });
	}

	static inline const ImVec4 JsonToColor(const json& j) {
		return ImVec4(j[0], j[1], j[2], j[3]);
	}

	ColorThemeController::ColorThemeController(std::filesystem::path themeFilePath) : mThemeFilePath(themeFilePath)
	{
		if (!std::filesystem::exists(mThemeFilePath)) {
			throw std::runtime_error("Theme file does not exist: " + mThemeFilePath.string());
		}

		LoadColorTheme();
	}

	void ColorThemeController::SaveColorTheme() const
	{

		json j;
		j["AddressColor"] = ColorToJson(mColorTheme.AddressColor);
		j["BytesColor"] = ColorToJson(mColorTheme.BytesColor);
		j["BreakpointHoveredColor"] = mColorTheme.BreakpointHoveredColor;
		j["BreakpointClickedColor"] = mColorTheme.BreakpointClickedColor;

		for (const auto& [type, color] : mColorTheme.TokenColorsMap) {
			j["tokenColors"][TokenTypeToString(type)] = ColorToJson(color);
		}

		std::ofstream outFile(mThemeFilePath);

		if (outFile.is_open()) {
			outFile.clear();
			outFile << j.dump(4);
			outFile.close();
		}

	}

	void ColorThemeController::LoadColorTheme(std::filesystem::path themeFilePath)
	{

		if (std::filesystem::exists(themeFilePath)) {
			mThemeFilePath = themeFilePath;
		}

		std::ifstream file(mThemeFilePath);

		if (!file.is_open())
		{
			throw std::runtime_error("ColorThemeController::LoadColorTheme -> Could not open theme file: " + mThemeFilePath.string());
			return;
		}

		json j;
		file >> j;

		mColorTheme.AddressColor = JsonToColor(j.at("AddressColor"));
		mColorTheme.BytesColor   = JsonToColor(j.at("BytesColor"));
		mColorTheme.BreakpointHoveredColor = j.at("BreakpointHoveredColor");
		mColorTheme.BreakpointClickedColor = j.at("BreakpointClickedColor");

		for (const auto& [tokenType, tokenColor] : j["TokenColors"].items()) {
			const auto type = disassembler::StringToTokenType(tokenType);
			mColorTheme.TokenColorsMap[type] = JsonToColor(tokenColor);
		}

	}

} // namespace UI