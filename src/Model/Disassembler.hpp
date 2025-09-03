// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>
#include "MemoryBus.hpp"
#include "Token.hpp"

#include <vector>
#include <unordered_map>

namespace disassembler
{

	enum OperandType : s8
	{
		none,
		implied,
		rel8,
		se8,
		i8,
		i16,
		rm8,
		rm16,
		r8,
		r16,
		addr,
		segAddr
	};

	struct DisassembledInstruction
	{
		u32 Address{};
		std::vector<u8> Bytes;
		std::vector<Token> Tokens{};
		bool Breakpoint{ false };

		void Clear()
		{
			Address = 0;
			Bytes.clear();
			Tokens.clear();
			Breakpoint = false;
		}

	};

	struct Instr
	{
		constexpr Instr(const char* m = "", OperandType o1 = none, OperandType o2 = none, bool hasModRM = false)
			: mnemonic(m), operand1(o1), operand2(o2), hasModRM(hasModRM) {
		}

		const char* mnemonic{};
		const OperandType operand1;
		const OperandType operand2;
		const bool hasModRM{ false };
	};

	class Disassembler
	{

	public:

		Disassembler(const i8086::MemoryBus* bus);

		void Disassembly(u32 StartAddress, u32 EndAddress);
		
		u8 GetMaxInstrBytesCount() const
		{
			return mMaxInstrBytesCount;
		}

	public:

		std::vector<DisassembledInstruction> disassembledInstructions;

	private:

		u8 Fetch();
		u16 Fetch16();
		void Decode(u8 opcode);
		std::string GetInstrLine(const Instr& instr);
		const std::string ParseOperand(const OperandType& operandType);

		std::string GetRmAddressOperand();
		std::string GetNoneOperand();
		std::string GetImpliedOperand();
		std::string GetRelativeOperand();
		std::string GetSignExtendedOperand();
		std::string GetI8Operand();
		std::string GetI16Operand();
		std::string GetRm8Operand();
		std::string GetRm16Operand();
		std::string GetR8Operand();
		std::string GetR16Operand();
		std::string GetRegisterOverrideOperand();
		std::string GetAddressOperand();

		void Tokenizer(const std::string& line, std::vector<Token>& tokens) const;

	private:

		const i8086::MemoryBus* const mBus;
		std::unordered_map<OperandType, std::string(Disassembler::*)(void)> mOperandGetters;

		DisassembledInstruction mTempInstruction;
		u8 mMaxInstrBytesCount{ 0 };

		u32 IP{ 0 };

		u8 mMod{};
		u8 mReg{};
		u8 mRm{};
	};

} // namespace disassembler