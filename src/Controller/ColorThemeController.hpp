// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Model/ColorTheme.hpp>

#include <filesystem>

namespace UI
{

	class ColorThemeController
	{
	
	public:

		ColorThemeController(std::filesystem::path themeFilePath);
		
		const ColorTheme& GetColorTheme() const { return mColorTheme; }
		
		void SaveColorTheme() const;
		void LoadColorTheme(std::filesystem::path themeFilePath = "");

	private:

		ColorTheme mColorTheme;
		std::filesystem::path mThemeFilePath;

	};

} // namespace UI