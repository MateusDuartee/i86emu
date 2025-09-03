// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/ChildPanel.hpp>

namespace UI
{

	template<typename Func>
	void Panel(const char* childId, const char* title, float contentWidth, ImU32 titleBgColor, Func&& contentFunc)
	{
		ChildPanel panel(childId, title, contentWidth, titleBgColor);
		contentFunc();
	}

} // namespace UI