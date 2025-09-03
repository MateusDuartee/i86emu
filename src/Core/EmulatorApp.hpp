// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Application.hpp"

#include <Model/I8086.hpp>
#include <Model/RAM.hpp>
#include <Model/Disassembler.hpp>
#include <Model/MemoryBus.hpp>
#include <Model/IOBus.hpp>
#include <Controller/RAMController.hpp>
#include <Controller/CPUController.hpp>
#include <Controller/DisassemblerController.hpp>
#include <Controller/ColorThemeController.hpp>
#include <View/DisassemblerWindow.hpp>
#include <View/StateWindow.hpp>
#include <View/MemoryEditorWindow.hpp>

class EmulatorApp : public Application {

public:
    EmulatorApp();

protected:
    void OnRender() override;
    void OnEvent(const SDL_Event& event) override;

private:

    // Models
    i8086::RAM mRam;
    i8086::I8086 mCpu;
    i8086::MemoryBus mMemoryBus;
    i8086::IO::IOBus mIOBus;
    disassembler::Disassembler mDisassembler;

    // Controllers
    i8086::CPUController mCpuController;
    i8086::RAMController mRamController;
    disassembler::DisassemblerController mDisassemblerController;
    UI::ColorThemeController mColorThemeController;

    // Views
    UI::MemoryEditorWindow mMemoryEditorWindow;
    UI::DisassemblerWindow mDisassemblerWindow;
    UI::StateWindow mStateWindow;
};