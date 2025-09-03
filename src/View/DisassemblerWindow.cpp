// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "DisassemblerWindow.hpp"

#include <charconv>

namespace UI
{
	constexpr ImGuiInputTextFlags INPUT_FLAGS = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase;
	constexpr ImVec2 breakpointBtnSize{ 16, 16 };

	DisassemblerWindow::DisassemblerWindow(
		disassembler::DisassemblerController* disassemblerController,
		i8086::CPUController* cpuController,
		const ColorThemeController* colorThemeController
	)
		: mDisassemblerController(disassemblerController), mCPUController(cpuController), mColorThemeController(colorThemeController)
	{}

	void DisassemblerWindow::ShowIfOpen()
	{
		if (!mIsOpen)
		{
			return;
		}

		mColorTheme = mColorThemeController->GetColorTheme();

		RenderWindow();
	}

    inline void DisassemblerWindow::RenderWindow()
    {

		if (ImGui::Begin("Disassembler", &mIsOpen))
		{
			ImGui::PushItemWidth(ImGui::CalcTextSize("XXXXXXX").x);
				
			if (ImGui::InputText("Start Address", mStartAddrBuf, sizeof(mStartAddrBuf), INPUT_FLAGS))
			{
				std::from_chars(mStartAddrBuf, mStartAddrBuf + sizeof(mStartAddrBuf), mDisassemblerController->startAddress, 16);
			}

			if (ImGui::InputText("End Address", mEndAddrBuf, sizeof(mEndAddrBuf), INPUT_FLAGS))
			{
				std::from_chars(mEndAddrBuf, mEndAddrBuf + sizeof(mEndAddrBuf), mDisassemblerController->endAddress, 16);
			}
				
			ImGui::PopItemWidth();

			ImGui::Spacing();
			ImGui::Dummy(ImVec2(1.0f,20.0f));
			ImGui::Spacing();

			if (ImGui::Button("Disassembly"))
			{
				mDisassemblerController->Disassembly();
			}

			ImGui::SameLine();
			RenderCheckBox("Address", "Show Address of instructions", &mShowAddress);
			ImGui::SameLine();
			RenderCheckBox("Bytes", "Show Bytes of instructions", &mShowBytes);

			RenderInstructions();

		}

		ImGui::End();

    }

    inline void DisassemblerWindow::RenderCheckBox(const char *label, const char *tooltip, bool *v) const
    {
		ImGui::Checkbox(label, v);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(tooltip);
		}
	}

	inline void DisassemblerWindow::RenderInstructions() const
	{
		ImGui::BeginChild("##Instructions", { 0,0 }, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
		
		ImGui::Spacing();

		ImGuiListClipper clipper;
		
		const int instructionsCount = static_cast<int>(mDisassemblerController->GetDisassembledInstructionsCount());

		clipper.Begin(instructionsCount, ImGui::GetTextLineHeightWithSpacing());

		while (clipper.Step()) {

			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {

				auto& instr = mDisassemblerController->GetDisassembledInstruction(i);

				RenderBreakpointButton(instr);
				
				if (ImGui::IsItemClicked())
				{
					instr.Breakpoint = !instr.Breakpoint;
					mCPUController->SetBreakpoint(instr.Address, instr.Breakpoint);
				}

				if (mShowAddress) {
					RenderAddress(instr.Address);
				}

				if (mShowBytes) {
					RenderBytes(instr.Bytes);
				}

				RenderTokens(instr.Tokens);
			}
		}

		ImGui::EndChild();
	}

	inline void DisassemblerWindow::RenderBreakpointButton(const disassembler::DisassembledInstruction& instr) const
	{
		const ImVec2 pos = ImGui::GetCursorScreenPos();
		const ImVec2 breakpointBtnCenter{ pos.x + breakpointBtnSize.x / 2, pos.y + breakpointBtnSize.y / 2 };

		ImGui::PushID(instr.Address);
		ImGui::InvisibleButton("##BP", breakpointBtnSize);
		ImGui::PopID();

		auto drawList = ImGui::GetWindowDrawList();

		if (ImGui::IsItemHovered())
		{
			drawList->AddCircleFilled(breakpointBtnCenter, 6.0f, mColorTheme.BreakpointHoveredColor);
		}

		if (instr.Breakpoint) {
			drawList->AddCircleFilled(breakpointBtnCenter, 6.0f, mColorTheme.BreakpointClickedColor);
		}

		ImGui::SameLine();
	}

	inline void DisassemblerWindow::RenderAddress(u32 address) const
	{
		ImGui::TextColored(mColorTheme.AddressColor, "%05X: ", address);
		ImGui::SameLine(0.0f, 0.0f);
	}

	inline void DisassemblerWindow::RenderBytes(const std::vector<u8>& bytes) const
	{
		const u8 maxBytesCount = mDisassemblerController->GetMaxInstrBytesCount();
		const u8 emptyBytesCount = maxBytesCount - static_cast<u8>(bytes.size());

		for (u8 i = 0; i < emptyBytesCount; ++i)
		{
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::Text("   ");
		}

		for (u8 i = 0; i < (maxBytesCount - emptyBytesCount); ++i)
		{
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::TextColored(mColorTheme.BytesColor, "%02X ", bytes[i]);
		}
	}

	inline void DisassemblerWindow::RenderTokens(const std::vector<disassembler::Token>& tokens) const
	{
		for (const auto& token : tokens)
		{
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, mColorTheme.TokenColorsMap.at(token.type));

			ImGui::Text(token.token.c_str());

			if (token.hasSpace)
			{
				ImGui::SameLine(0.0f, 0.0f);
				ImGui::TextUnformatted(" ");
			}
			
			ImGui::PopStyleColor();
		}

	}

} // namespace UI