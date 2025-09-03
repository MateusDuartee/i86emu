// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "MemoryEditorWindow.hpp"

namespace UI
{

    MemoryEditorWindow::MemoryEditorWindow(i8086::MemoryBus* bus)
    {
        mMemoryEditor.Cols = 48;
        mMemoryEditor.ReadOnly = true;
        mMemoryEditor.PreviewEndianness = 1;
        mMemoryEditor.PreviewDataType = ImGuiDataType_U16;

        mBus = bus;

        mBus->RegisterObserver(this);
    }

    void MemoryEditorWindow::ShowIfOpen()
    {
        if (!mIsOpen)
        {
            return;
        }

        if (ImGui::Begin("Memory editor", &mIsOpen))
        {
            mBus->DumpMemory(mMemDump);

            mMemoryEditor.DrawContents(mMemDump.data(), mMemDump.size(), 0);
        }

        ImGui::End();
    }

    void MemoryEditorWindow::OnWrite(u32 address, u16 data)
    {
        if (mIsOpen)
        {
            mMemoryEditor.HighlightColor = IM_COL32(81, 245, 149, 50);
            mMemoryEditor.GotoAddrAndHighlight(address, address);
        }
    }

    void MemoryEditorWindow::OnRead(u32 address)
    {
        if (mIsOpen)
        {
            mMemoryEditor.HighlightColor = IM_COL32(15, 166, 247, 50);
            mMemoryEditor.GotoAddrAndHighlight(address, address);
        }
    }

} // namespace UI