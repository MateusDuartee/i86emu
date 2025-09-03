// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>
#include <Utils/utils.hpp>

namespace i8086
{

	struct Flags
	{
		bool C{}; // Carry flag
		bool P{}; // Parity flag
		bool A{}; // Auxiliary carry flag
		bool Z{}; // Zero flag
		bool S{}; // Sign flag
		bool T{}; // Trap flag
		bool I{}; // Interrupt flag
		bool D{}; // Direction flag
		bool O{}; // Overflow flag

		u16 Get() const
		{
			return (O << 11) | (D << 10) | (I << 9) | (T << 8) | (S << 7) | (Z << 6) | (A << 4) | (P << 2) | (C << 0);
		}

		void Set(u16 value)
		{
			C = GET_BIT(value,  0);
			P = GET_BIT(value,  2);
			A = GET_BIT(value,  4);
			Z = GET_BIT(value,  6);
			S = GET_BIT(value,  7);
			T = GET_BIT(value,  8);
			I = GET_BIT(value,  9);
			D = GET_BIT(value, 10);
			O = GET_BIT(value, 11);
		}

		void CheckParity(u8 value) {
			u8 count = 0;

			while (value) {
				count++;
				value &= (value - 1);
			}

			P = ((count % 2) == 0);
		}

		void CheckCarryAdd(u16 a, u16 b, u32 result, u8 operandSize)
		{
			const u32 mask = 1 << operandSize;
			C = ((a ^ b ^ result) & mask) == mask;
		}

		void CheckCarrySub(u16 a, u16 b, u8 operandSize)
		{
			C = MASK(b, operandSize) > MASK(a, operandSize);
		}

		void CheckOverflowAdd(u16 a, u16 b, s32 result, u8 operandSize)
		{
			if (operandSize == 8)
			{
				s8 sa = static_cast<s8>(a);
				s8 sb = static_cast<s8>(b);
				s8 sres = static_cast<s8>(result);

				O = ((sa > 0 && sb > 0 && sres < 0) || (sa < 0 && sb < 0 && sres > 0));
			}

			else if (operandSize == 16)
			{
				s16 sa = static_cast<s16>(a);
				s16 sb = static_cast<s16>(b);
				s16 sres = static_cast<s16>(result);

				O = ((sa > 0 && sb > 0 && sres < 0) || (sa < 0 && sb < 0 && sres > 0));
			}

		}

		void CheckOverflowSub(u16 a, u16 b, s32 result, u8 operandSize)
		{

			if (operandSize == 8)
			{
				s16 sa = static_cast<s8>(a);
				s16 sb = static_cast<s8>(b);
				s16 sres = static_cast<s16>(sa - sb);

				O = sres < -128 || sres > 127;
			}

			else if (operandSize == 16)
			{
				s32 sa = static_cast<s16>(a);
				s32 sb = static_cast<s16>(b);
				s32 sres = sa - sb;

				O = sres < -32768 || sres > 32767;
			}

		}

		void CheckZero(u32 value, u8 operandSize)
		{
			Z = !MASK(value, operandSize);
		}

		void CheckAuxiliaryCarryAdd(u16 a, u16 b, u32 result)
		{
			A = ((a ^ b ^ result) & 0x10) == 0x10;
		}

		void CheckAuxiliaryCarrySub(u16 a, u16 b)
		{
			A = (b & 0xF) > (a & 0xF);
		}

		void CheckSign(u32 value, u8 operandSize)
		{
			S = LSB(value >> (operandSize - 1));
		}

	};

} // namespace i8086