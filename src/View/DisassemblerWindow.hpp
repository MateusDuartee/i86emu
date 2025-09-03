// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Interfaces/IViewWindow.hpp>
#include <Controller/DisassemblerController.hpp>
#include <Controller/CPUController.hpp>
#include <Controller/ColorThemeController.hpp>
#include <Model/Token.hpp>
#include <Utils/types.hpp>

namespace UI
{

	class DisassemblerWindow : public IViewWindow
	{

	public:

		DisassemblerWindow(
			disassembler::DisassemblerController* disassemblerController,
			i8086::CPUController* cpuController,
			const ColorThemeController* colorThemeController
		);

		void ShowIfOpen() override;

	private:

		inline void RenderWindow();
		inline void RenderCheckBox(const char* label, const char* tooltip, bool* v) const;
		inline void RenderInstructions() const;
		inline void RenderBreakpointButton(const disassembler::DisassembledInstruction& instr) const;
		inline void RenderAddress(u32 address) const;
		inline void RenderBytes(const std::vector<u8>& bytes) const;
		inline void RenderTokens(const std::vector<disassembler::Token>& tokens) const;

	private:

		disassembler::DisassemblerController* mDisassemblerController{ nullptr };
		i8086::CPUController* mCPUController{ nullptr };
		const ColorThemeController* mColorThemeController{ nullptr };

		ColorTheme mColorTheme;

		bool mShowBytes{ true };
		bool mShowAddress{ true };

		char mStartAddrBuf[10]{};
		char mEndAddrBuf[10]{};
	};

} // namespace UI