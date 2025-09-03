// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "EmulatorApp.hpp"

#include <portable-file-dialogs.h>

EmulatorApp::EmulatorApp()
    : Application("intel 8086", 800, 600),
      mRam(0x200000),
      mRamController(&mRam),
      mCpu(&mMemoryBus),
      mDisassembler(&mMemoryBus),
      mCpuController(&mCpu),
      mDisassemblerController(&mDisassembler),
      mColorThemeController("Resources/DasmColorTheme.json"),
      mDisassemblerWindow(&mDisassemblerController, &mCpuController, &mColorThemeController),
      mStateWindow(&mCpuController, &mMemoryBus),
      mMemoryEditorWindow(&mMemoryBus)
{

    mMemoryBus.AttachDevice(&mRamController, 0x00000, 0x1FFFFF);

}

void EmulatorApp::OnRender()
{
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItem("Open")) {

                auto filePath = pfd::open_file("Choose a file", ".", {"Executable", "*.com", "*.exe"}).result();

                if (!filePath.empty()) {
                    mRamController.LoadFile(filePath[0], 0x0);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug")) {

            if (ImGui::MenuItem("State Window", "", mStateWindow.IsOpen()))
            { 
                mStateWindow.ToggleVisibility();
            }

            if (ImGui::MenuItem("Memory Window", "", mMemoryEditorWindow.IsOpen()))
            {
                mMemoryEditorWindow.ToggleVisibility();
            }

            if (ImGui::MenuItem("Disassembler Window", "", mDisassemblerWindow.IsOpen()))
            {
                mDisassemblerWindow.ToggleVisibility();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    mDisassemblerWindow.ShowIfOpen();
    mStateWindow.ShowIfOpen();
    mMemoryEditorWindow.ShowIfOpen();
}

void EmulatorApp::OnEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_C)
    {
        mCpu.Cycles(1);
    }
}