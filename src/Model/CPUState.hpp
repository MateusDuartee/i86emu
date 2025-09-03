// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Register.hpp"
#include "Flags.hpp"

#include <Utils/types.hpp>

namespace i8086
{
	struct RegisterOverride
	{
		bool pending{ false };
		Register segment{};
	};

	struct CPUState
	{

		/* ModR/m fields */

		u8 Mod{};
		u8 Reg{};
		u8 Rm{};

		/* Registers */

		Register IP{}; // Instruction pointer

		Flags SF{};    // Status flags

		/* General Purpose Registers */

		Register A{};  // Accumulator Register
		Register B{};  // Base Register
		Register C{};  // Count Register
		Register D{};  // Data Register

		Register SP{}; // Stack Pointer
		Register BP{}; // Base Pointer
		Register SI{}; // Source Index
		Register DI{}; // Destination Index

		/* Segment Registers */

		Register CS{}; // Code Segment
		Register DS{}; // Data Segment
		Register SS{}; // Stack Segment
		Register ES{}; // Extra Segment

		/* other auxiliary fields */

		Register mSeg{}; // Current Segment Register
		RegisterOverride mRegisterOverride{}; // segment override status
		u16 EA{ 0 }; // Effective Address
		u8 OperandSize{}; // Size of the operand
	};
}