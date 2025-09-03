// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Model/Disassembler.hpp>

#include <stdexcept>

namespace disassembler
{
	class DisassemblerController
	{

	public:
		
		DisassemblerController(Disassembler* const disassembler) : mDisassembler(disassembler) 
		{
			if (!mDisassembler)
			{
				throw std::runtime_error("Disassembler is not initialized.");
			}
		}

		void Disassembly()
		{
			mDisassembler->Disassembly(startAddress, endAddress);
		}

		const size_t GetDisassembledInstructionsCount() const
		{
			return mDisassembler->disassembledInstructions.size();
		}

		DisassembledInstruction& GetDisassembledInstruction(size_t index) const
		{

			if (index >= mDisassembler->disassembledInstructions.size())
			{
				throw std::out_of_range("DisassemblerController::GetInstruction -> Index out of range");
			}

			return mDisassembler->disassembledInstructions[index];
		}

		u8 GetMaxInstrBytesCount() const
		{
			return mDisassembler->GetMaxInstrBytesCount();
		}

		
	public:
		
		u32 startAddress{ 0 };
		u32 endAddress{ 0 };

	private:
		Disassembler* const mDisassembler{ nullptr };
	};


} // namespace disassembler