// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Token.hpp"

#include <imgui.h>

#include <unordered_map>

namespace UI
{

	struct ColorTheme
	{
		ImVec4 AddressColor;
		ImVec4 BytesColor;
		ImU32 BreakpointHoveredColor;
		ImU32 BreakpointClickedColor;

		std::unordered_map<disassembler::TokenType, ImVec4> TokenColorsMap;
	};

} // namespace UI