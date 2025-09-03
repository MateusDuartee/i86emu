// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Interfaces/IMemoryObserver.hpp>
#include <Interfaces/IViewWindow.hpp>
#include <Model/MemoryBus.hpp>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include <vector>

namespace UI
{

	class MemoryEditorWindow : public i8086::IMemoryObserver, public IViewWindow
	{

	public:

		MemoryEditorWindow(i8086::MemoryBus* bus);

		void ShowIfOpen() override;

		void OnWrite(u32 address, u16 data) override;
		void OnRead(u32 address) override;

	private:
		std::vector<u8> mMemDump;
		MemoryEditor mMemoryEditor;
		i8086::MemoryBus* mBus{ nullptr };

	};

} // namespace UI