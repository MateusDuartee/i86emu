// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "CPUState.hpp"
#include "Register.hpp"
#include "MemoryBus.hpp"

namespace i8086
{

	/* Operation size in bits */

	constexpr u8 WORD = 16;
	constexpr u8 BYTE = 8;
	
	/**
	* @class Instr
	* 
	* @brief Contains static methods that implement operations for the Intel 8086 CPU.
	* 
	* @details
	* This class provides a set of static methods that perform operations to be used by the CPU.
	* 
	* @note
	* Why these methods aren't part of the CPU class?
	* - Improve organization
	* - Facilitate unit testing and reusability
	*
	*/
	class Instr
	{

	public:

		/*============================================================
		  ==================== Arithmetic Operations =================
		  ============================================================*/

		/**
		 * @brief Adds two 16-bit integers and updates the CPU flags accordingly.
		 * 
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the addition (a + b).
		 * 
		 * @details
		 * This method performs the addition of two 16-bit integers and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Carry flag is set if there is a carry out from the most significant bit.
		 * - Auxiliary carry flag is set if there is a carry out from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the addition results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static u16 ADD(const u16 a, const u16 b, CPUState* state)
		{
			const u32 result = a + b;

			state->SF.CheckCarryAdd(a, b, result, state->OperandSize);
			state->SF.CheckAuxiliaryCarryAdd(a, b, result);
			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckOverflowAdd(a, b, result, state->OperandSize);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Adds two 16-bit integers with carry and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the addition (a + b + carry).
		 *
		 * @details
		 * This method performs the addition of two 16-bit integers with an additional carry input and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Carry flag is set if there is a carry out from the most significant bit.
		 * - Auxiliary carry flag is set if there is a carry out from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the addition results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static u16 ADC(const u16 a, const u16 b, CPUState* state)
		{
			const u8 carryIn = static_cast<u8>(state->SF.C);
			const u32 result = a + b + carryIn;

			state->SF.CheckCarryAdd(a, b, result, state->OperandSize);
			state->SF.CheckAuxiliaryCarryAdd(a, b, result);
			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckOverflowAdd(a, b + carryIn, result, state->OperandSize);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Subtracts two 16-bit integers and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the addition (a - b).
		 *
		 * @details
		 * This method performs the subtraction of two 16-bit integers and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Carry flag is set if the second operand is greater than the first operand.
		 * - Auxiliary carry flag is set if there is a borrow from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the subtraction results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static u16 SUB(const u16 a, const u16 b, CPUState* state)
		{
			const u32 result = a - b;

			state->SF.CheckCarrySub(a, b, state->OperandSize);
			state->SF.CheckAuxiliaryCarrySub(a, b);
			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckOverflowSub(a, b, result, state->OperandSize);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Subtracts two 16-bit integers with borrow and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the subtraction (a - b - carry).
		 *
		 * @details
		 * This method performs the subtraction of two 16-bit integers with an additional borrow input and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set if the second operand is greater than the first operand.
		 * - Auxiliary carry flag is set if there is a borrow from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the subtraction results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static u16 SBB(const u16 a, const u16 b, CPUState* state)
		{
			const u8 carryIn = static_cast<u8>(state->SF.C);
			const u32 result = a - b - carryIn;

			state->SF.CheckCarrySub(a, b, state->OperandSize);
			state->SF.CheckAuxiliaryCarrySub(a, b);
			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckOverflowSub(a, b - carryIn, result, state->OperandSize);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);

		}

		/**
		 * @brief Increments a 16-bit register by 1 and updates the CPU flags accordingly.
		 *
		 * @param reg The register to be incremented.
		 * @param sf The current CPU flags.
		 *
		 * @details
		 * This method increments the value of a 16-bit register by 1 and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Auxiliary carry flag is set if there is a carry from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the increment results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static void INC(Register& reg, Flags& sf)
		{
			const u32 result = reg.X + 1;

			sf.CheckAuxiliaryCarryAdd(reg.X, 1, result);
			sf.CheckParity(result & 0xFF);
			sf.CheckOverflowAdd(reg.X, 1, result, 16);
			sf.CheckZero(result, 16);
			sf.CheckSign(result, 16);

			++reg.X;
		}

		/**
		 * @brief Decrements a 16-bit register by 1 and updates the CPU flags accordingly.
		 *
		 * @param reg The register to be decremented.
		 * @param sf The current CPU flags.
		 *
		 * @details
		 * This method decrements the value of a 16-bit register by 1 and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Auxiliary carry flag is set if there is a borrow from the least significant nibble.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Overflow flag is set if the decrement results in a value wrapping around the signed range.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static void DEC(Register& reg, Flags& sf)
		{
			const u32 result = reg.X - 1;

			sf.CheckAuxiliaryCarrySub(reg.X, 1);
			sf.CheckParity(result & 0xFF);
			sf.CheckOverflowSub(reg.X, 1, result, 16);
			sf.CheckZero(result, 16);
			sf.CheckSign(result, 16);

			--reg.X;
		}

		/**
		 * @brief ASCII Adjust for Addition (AAA) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method adjusts the value in the AL register after an addition operation involving ASCII values to ensure it is a valid BCD representation.
		 * It modifies both the AL and AH registers and updates the Auxiliary Carry and Carry flags accordingly.
		 * 
		 * @par Affected flags:
		 * - Auxiliary Carry Flag (AF)
		 * - Carry Flag (CF)
		 *
		 * @par How the flags are affected:
		 * - Auxiliary carry flag is set if there is a carry out from the least significant nibble after adjustment.
		 * - Carry flag is set if there is a carry out from the most significant digit after adjustment.
		 * 
		 * @note
		 * The AAA instruction assumes that the value in AL is the result of a previous addition operation involving ASCII values.
		 * If AL contains a non-ASCII value, the result after AAA may not be meaningful.
		 */
		static void AAA(CPUState* state)
		{
			if (((state->A.L & 0x0F) > 9) || state->SF.A)
			{
				state->A.L += 6;
				state->A.H += 1;
				state->SF.A = 1;
				state->SF.C = 1;
			}

			else
			{
				state->SF.A = 0;
				state->SF.C = 0;
			}

			state->A.L &= 0x0F;
		}

		/**
		 * @brief ASCII Adjust for Subtraction (AAS) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method adjusts the value in the AL register after a subtraction operation involving ASCII values to ensure it is a valid BCD representation.
		 * It modifies both the AL and AH registers and updates the Auxiliary Carry and Carry flags accordingly.
		 * 
		 * @par Affected flags:
		 * - Auxiliary Carry Flag (AF)
		 * - Carry Flag (CF)
		 *
		 * @par How the flags are affected:
		 * - Auxiliary carry flag is set if there is a borrow from the least significant nibble after adjustment.
		 * - Carry flag is set if there is a borrow from the most significant digit after adjustment.
		 * 
		 * @note
		 * The AAS instruction assumes that the value in AL is the result of a previous subtraction operation involving ASCII values.
		 * If AL contains a non-ASCII value, the result after AAS may not be meaningful.
		 */
		static void AAS(CPUState* state)
		{
			if ((state->A.L & 0x0F) > 9 || state->SF.A)
			{
				state->A.L -= 6;
				state->A.H -= 1;
				state->SF.A = 1;
				state->SF.C = 1;
			}

			else
			{
				state->SF.C = 0;
				state->SF.A = 0;
			}

			state->A.L &= 0x0F;
		}

		/**
		 * @brief Decimal Adjust for Addition (DAA) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method adjusts the value in the AL register after a BCD addition operation to ensure it is a valid BCD representation.
		 * It modifies the AL register and updates the CPU flags accordingly.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set if there is a carry out from the most significant digit after adjustment.
		 * - Auxiliary carry flag is set if there is a carry out from the least significant nibble after adjustment.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * 
		 * @note
		 * The DAA instruction assumes that the value in AL is the result of a previous addition operation involving BCD values.
		 * If AL contains a non-BCD value, the result after DAA may not be meaningful.
		 */
		static void DAA(CPUState* state)
		{
			if ((state->A.L & 0x0F) > 9 || state->SF.A)
			{
				state->A.L += 6;
				state->SF.A = 1;
			}

			else
			{
				state->SF.A = 0;
			}

			if (state->A.L > 0x9F || state->SF.C)
			{
				state->A.L += 0x60;
				state->SF.C = 1;
			}

			else
			{
				state->SF.C = 0;
			}

			state->SF.CheckSign(state->A.L, 8);
			state->SF.CheckZero(state->A.L, 8);
			state->SF.CheckParity(state->A.L);
		}

		/**
		 * @brief Decimal Adjust for Subtraction (DAS) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method adjusts the value in the AL register after a BCD subtraction operation to ensure it is a valid BCD representation.
		 * It modifies the AL register and updates the CPU flags accordingly.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Auxiliary Carry Flag (AF)
		 * - Parity Flag (PF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set if there is a borrow from the most significant digit after adjustment.
		 * - Auxiliary carry flag is set if there is a borrow from the least significant nibble after adjustment.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * 
		 * @note
		 * The DAS instruction assumes that the value in AL is the result of a previous subtraction operation involving BCD values.
		 * If AL contains a non-BCD value, the result after DAS may not be meaningful.
		 */
		static void DAS(CPUState* state)
		{
			if ((state->A.L & 0x0F) > 9 || state->SF.A)
			{
				state->A.L -= 6;
				state->SF.A = 1;
			}

			else
			{
				state->SF.A = 0;
			}

			if (state->A.L > 0x9F || state->SF.C)
			{
				state->A.L -= 0x60;
				state->SF.C = 1;
			}

			else
			{
				state->SF.C = 0;
			}

			state->SF.CheckSign(state->A.L, 8);
			state->SF.CheckZero(state->A.L, 8);
			state->SF.CheckParity(state->A.L);
		}

		/**
		 * @brief Convert Byte to Word (CBW) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method sign-extends the value in the AL register to the AX register.
		 * If the most significant bit of AL is set, AH is set to 0xFF; otherwise, AH is set to 0x00.
		 */
		static void CBW(CPUState* state)
		{
			if (state->A.L & 0x80)
			{
				state->A.H = 0xFF;
			}

			else
			{
				state->A.H = 0x00;
			}
		}

		/**
		 * @brief Convert Word to Doubleword (CWD) instruction.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method sign-extends the value in the AX register to the DX:AX register pair.
		 * If the most significant bit of AX is set, DX is set to 0xFFFF; otherwise, DX is set to 0x0000.
		 */
		static void CWD(CPUState* state)
		{
			if (state->A.H & 0x80)
			{
				state->D.X = 0xFFFF;
			}

			else
			{
				state->D.X = 0x0000;
			}
		}

		/*============================================================
		  ==================== Logical Operations ====================
		  ============================================================*/

		/**
		 * @brief Performs a bitwise OR operation on two 16-bit integers and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the OR operation (a | b).
		 *
		 * @details
		 * This method performs a bitwise OR operation on two 16-bit integers and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is cleared.
		 * - Overflow flag is cleared.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * 
		 * @note
		 * Auxiliary carry (AF) is undefined for OR operation.
		 */
		static u16 OR(const u16 a, const u16 b, CPUState* state)
		{
			const u32 result = a | b;

			state->SF.C = 0;
			state->SF.O = 0;

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Performs a bitwise AND operation on two 16-bit integers and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the AND operation (a & b).
		 *
		 * @details
		 * This method performs a bitwise AND operation on two 16-bit integers and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is cleared.
		 * - Overflow flag is cleared.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * 
		 * @note
		 * Auxiliary carry (AF) is undefined for AND operation.
		 */
		static u16 AND(const u16 a, const u16 b, CPUState* state)
		{
			const u32 result = a & b;

			state->SF.C = 0;
			state->SF.O = 0;

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Performs a bitwise XOR operation on two 16-bit integers and updates the CPU flags accordingly.
		 *
		 * @param a The first operand.
		 * @param b The second operand.
		 * @param state The current CPU state.
		 * @return The result of the XOR operation (a ^ b).
		 *
		 * @details
		 * This method performs a bitwise XOR operation on two 16-bit integers and updates the CPU flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Parity Flag (PF)
		 * - Overflow Flag (OF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * 
		 * @par How the flags are affected:
		 * - Carry flag is cleared.
		 * - Overflow flag is cleared.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 *
		 * @note
		 * Auxiliary carry (AF) is undefined for XOR operation.
		 */
		static u16 XOR(const u16 a, const u16 b, CPUState* state)
		{
			const u32 result = a ^ b;

			state->SF.C = 0;
			state->SF.O = 0;

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Rotates the bits to the left through the carry flag.
		 *
		 * @param value The value to be rotated.
		 * @param count The number of bits to rotate.
		 * @param state The current CPU state.
		 * @return The value rotated left by the specified count.
		 *
		 * @details
		 * This method performs a left shift using the carry flag as part of the rotation.
		 * 
		 * @par Procedure:
		 * - Gets the most significant bit of the value.
		 * - Shifts the value to the left by one bit.
		 * - Sets the most significant bit to the carry flag.
		 * - Sets the carry flag to the most significant bit that was shifted out.
		 * - Repeats this procedure for the specified count.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was rotated out.
		 * - Overflow flag is set only if the count is 1, and it is set to the XOR of the most significant bit of the result and the carry flag.
		 * - if the count is different from 1, the overflow flag is undefined.
		 * - if the count is 0, no flags are affected and the original value is returned.
		 */
		static u16 RCL(const u16 value, const u8 count, CPUState* state)
		{

			u16 result = value;
			u8 tempCount = count % (state->OperandSize + 1);
			u8 tempCFlag{};

			while (tempCount != 0)
			{
				tempCFlag = MSB(result, state->OperandSize);

				result = (result << 1) | static_cast<u8>(state->SF.C);

				state->SF.C = tempCFlag;

				--tempCount;
			}

			if (tempCount == 1)
			{
				state->SF.O = MSB(result, state->OperandSize) ^ state->SF.C;
			}

			return MASK(result, state->OperandSize);;
		}

		/**
		 * @brief Rotates the bits to the right through the carry flag.
		 *
		 * @param value The value to be rotated.
		 * @param count The number of bits to rotate.
		 * @param state The current CPU state.
		 * @return The value rotated right by the specified count.
		 *
		 * @details
		 * This method performs a right shift using the carry flag as part of the rotation.
		 * 
		 * @par Procedure:
		 * - Gets the least significant bit of the value.
		 * - Shifts the value to the right by one bit.
		 * - Sets the most significant bit to the carry flag.
		 * - Sets the carry flag to the least significant bit that was shifted out.
		 * - Repeats this procedure for the specified count.
		 *
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was rotated out.
		 * - Overflow flag is set only if the count is 1, and it is set to the XOR of the most significant bit of the value and the carry flag.
		 * - if the count is different from 1, the overflow flag is undefined.
		 * - if the count is 0, no flags are affected and the original value is returned.
		 * 
		 * @note
		 * The count is taken modulo (OperandSize + 1) to ensure that the rotation does not exceed the size of the operand.
		 * The overflow flag has to be calculated before the rotation to ensure that it is set correctly.
		 */
		static u16 RCR(const u16 value, const u8 count, CPUState* state)
		{
			u16 result = MASK(value, state->OperandSize);
			u8 tempCount = count % (state->OperandSize + 1);
			u8 tempCFlag{};

			if (tempCount == 1)
			{
				state->SF.O = MSB(result, state->OperandSize) ^ state->SF.C;
			}

			while (tempCount != 0)
			{
				tempCFlag = result & 1;

				result = (result >> 1) | (state->SF.C << (state->OperandSize - 1));

				state->SF.C = tempCFlag;

				--tempCount;
			}

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Rotates the bits to the left.
		 *
		 * @param value The value to be rotated.
		 * @param count The number of bits to rotate.
		 * @param state The current CPU state.
		 * @return The value rotated left by the specified count.
		 *
		 * @details
		 * This method performs a left rotation without using the carry flag as part of the rotation like RCL does.
		 * 
		 * @par Procedure:
		 * - Gets the most significant bit of the value.
		 * - Shifts the value to the left by one bit.
		 * - Sets the least significant bit to the most significant bit that was shifted out.
		 * - Repeats this procedure for the specified count.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was rotated out.
		 * - Overflow flag is set only if the count is 1, and it is set to the XOR of the most significant bit of the result and the carry flag.
		 * - if the count is different from 1, the overflow flag is undefined.
		 * - if the count is 0, no flags are affected and the original value is returned.
		 * 
		 * @note
		 * Ensures that the count is taken modulo OperandSize to prevent unnecessary full rotations.
		 * Ensures that the carry flag is updated before the overflow flag.
		 */
		static u16 ROL(const u16 value, const u8 count, CPUState* state)
		{
			u16 result = value;
			u8 tempCount = count % state->OperandSize;
			u8 tempCFlag{};

			while (tempCount != 0)
			{
				tempCFlag = MSB(result, state->OperandSize);

				result = (result << 1) | tempCFlag;

				--tempCount;
			}

			if (count != 0)
			{
				state->SF.C = tempCFlag;
			}

			if (count == 1)
			{
				state->SF.O = MSB(result, state->OperandSize) ^ state->SF.C;
			}

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Rotates the bits to the right.
		 *
		 * @param value The value to be rotated.
		 * @param count The number of bits to rotate.
		 * @param state The current CPU state.
		 * @return The value rotated right by the specified count.
		 *
		 * @details
		 * This method performs a right rotation without using the carry flag as part of the rotation like RCR does.
		 * 
		 * @par Procedure:
		 * - Gets the least significant bit of the value.
		 * - Shifts the value to the right by one bit.
		 * - Sets the most significant bit to the least significant bit that was shifted out.
		 * - Repeats this procedure for the specified count.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was rotated out.
		 * - Overflow flag is set only if the count is 1.
		 * - Overflow flag is set to the XOR of the most significant bit and the second most significant bit of the result.
		 * - if the count is different from 1, the overflow flag is undefined.
		 * - if the count is 0, no flags are affected and the original value is returned.
		 * 
		 * @note
		 * Ensures that the count is taken modulo OperandSize to prevent unnecessary full rotations.
		 * Ensures that the carry flag is updated before the overflow flag.
		 * The overflow flag in this case is set to the XOR of the most significant bit and the value before the rotation
		 * (since count is 1, the second most significant bit of the result is the most significant bit of the value before the rotation).
		 */
		static u16 ROR(const u16 value, const u8 count, CPUState* state)
		{
			u16 result = MASK(value, state->OperandSize);
			u8 tempCount = count % state->OperandSize;
			u8 tempCFlag{};

			while (tempCount != 0)
			{
				tempCFlag = result & 1;

				result = (result >> 1) | (tempCFlag << (state->OperandSize - 1));

				--tempCount;
			}

			if (count != 0)
			{
				state->SF.C = tempCFlag;
			}

			if (count == 1)
			{
				state->SF.O = MSB(result, state->OperandSize) ^ MSB(value, state->OperandSize);
			}

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Shifts the bits to the left.
		 *
		 * @param value The value to be shifted.
		 * @param count The number of bits to shift.
		 * @param state The current CPU state.
		 * @return The value shifted left by the specified count.
		 *
		 * @details
		 * This method performs a left shift and updates the CPU flags accordingly.
		 * 
		 * @par Procedure:
		 * - shifts the value to the left by the specified count.
		 * - sets the carry flag to the last bit that was shifted out.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 * - Parity Flag (PF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was shifted out.
		 * - Overflow flag is set only if the count is 1, and it is set to the XOR of the most significant bit of the result and the carry flag.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * - If the count is 0, no flags are affected and the original value is returned.
		 * - If the count is different from 1, the overflow flag is undefined.
		 * 
		 * @note
		 * If count is greater than the operand size, the result is zero and all flags are set accordingly.
		 * Ensures that the carry flag is updated before the overflow flag.
		 */
		static u16 SHL(const u16 value, const u8 count, CPUState* state)
		{
			if (count == 0)
			{
				return value;
			}

			if (count > state->OperandSize)
			{
				state->SF.C = 0;
				state->SF.Z = 1;
				state->SF.S = 0;
				state->SF.P = 1;
				return 0;
			}

			state->SF.C = GET_BIT(value, state->OperandSize - count);

			const u32 result = MASK(value << count, state->OperandSize);

			if (count == 1)
			{
				state->SF.O = MSB(result, state->OperandSize) ^ state->SF.C;
			}

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return result;
		}

		/**
		 * @brief Shifts the bits to the right.
		 *
		 * @param value The value to be shifted.
		 * @param count The number of bits to shift.
		 * @param state The current CPU state.
		 * @return The value shifted right by the specified count.
		 *
		 * @details
		 * This method performs a right shift and updates the CPU flags accordingly.
		 * 
		 * @par Procedure:
		 * - shifts the value to the right by the specified count.
		 * - sets the carry flag to the last bit that was shifted out.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 * - Parity Flag (PF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was shifted out.
		 * - Overflow flag is set only if the count is 1, and it is set to the most significant bit of the value before the shift.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 * - If the count is 0, no flags are affected and the original value is returned.
		 * - If the count is different from 1, the overflow flag is undefined.
		 * 
		 * @note
		 * If count is greater than the operand size, the result is zero and all flags are set accordingly.
		 * Ensures that the carry flag is updated before the overflow flag.
		 */
		static u16 SHR(const u16 value, const u8 count, CPUState* state)
		{
			if (count == 0)
			{
				return value;
			}

			if (count > state->OperandSize)
			{
				state->SF.C = 0;
				state->SF.Z = 1;
				state->SF.S = 0;
				state->SF.P = 1;
				return 0;
			}

			const u16 maskedValue = MASK(value, state->OperandSize);

			state->SF.C = GET_BIT(maskedValue, count - 1);

			const u32 result = maskedValue >> count;

			if (count == 1)
			{
				state->SF.O = MSB(maskedValue, state->OperandSize);
			}

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/**
		 * @brief Performs an arithmetic right shift.
		 *
		 * @param value The value to be shifted.
		 * @param count The number of bits to shift.
		 * @param state The current CPU state.
		 * @return The value shifted right by the specified count, preserving the sign bit.
		 *
		 * @details
		 * This method performs an arithmetic right shift, which preserves the sign bit (most significant bit).
		 * 
		 * @par Procedure:
		 * - If the count is 0, returns the original value without affecting any flags.
		 * - If the count is greater than or equal to the operand size, sets the result to all 1s if the sign bit is set, otherwise sets it to 0.
		 * - Sets the carry flag to the last bit that was shifted out.
		 * - If the count is less than the operand size, shifts the value to the right by the specified count, preserving the sign bit.
		 * - If the count is 1, clears the overflow flag.
		 * - Updates the parity, zero, and sign flags based on the result.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Overflow Flag (OF)
		 * - Parity Flag (PF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 *
		 * @par How the flags are affected:
		 * - Carry flag is set to the last bit that was shifted out.
		 * - Overflow flag is cleared if the count is 1, otherwise it is undefined.
		 * - Parity flag is set if the least significant byte of the result has an even number of bits set.
		 * - Zero flag is set if the result is zero.
		 * - Sign flag is set if the most significant bit of the result is set.
		 */
		static u16 SAR(const u16 value, u8 count, CPUState* state)
		{
			if (count == 0)
			{
				return value;
			}

			const u16 maskedValue = MASK(value, state->OperandSize);
			s16 result = (state->OperandSize == 8) ? static_cast<s8>(maskedValue) : static_cast<s16>(maskedValue);

			if (count >= state->OperandSize) {
				const bool signBit = MSB(maskedValue, state->OperandSize);
				state->SF.C = signBit;

				result = signBit ? MASK(-1, state->OperandSize) : 0;
			}

			else {
				state->SF.C = GET_BIT(maskedValue, count - 1);

				result >>= count;
			}

			if (count == 1)
			{
				state->SF.O = 0;
			}

			state->SF.CheckParity(result & 0xFF);
			state->SF.CheckZero(result, state->OperandSize);
			state->SF.CheckSign(result, state->OperandSize);

			return MASK(result, state->OperandSize);
		}

		/*===========================================================
		  =================== Data movement =========================
		  ===========================================================*/

		/**
		 * @brief Pushes a register onto the stack.
		 *
		 * @param reg The register containing the value to be pushed.
		 * @param state The current CPU state.
		 * @param bus The memory bus for writing to memory.
		 *
		 * @details
		 * This method pushes a 16-bit value from a register onto the stack by decrementing the stack pointer and writing the value to memory.
		 * 
		 * @par Stack Growth:
		 * The stack grows downwards in memory, meaning that pushing a value decreases the stack pointer (SP).
		 * 
		 * @note
		 * The stack segment (SS) is used as the segment for the stack operations.
		 * The method assumes that the stack has enough space to accommodate the push operation.
		 */
		static void PUSH(const Register& reg, CPUState* state, MemoryBus* bus)
		{
			state->SP.X -= 2;
			bus->Write(state->SP.X, reg.X, state->SS, WORD);
		}

		/**
		 * @brief Pushes a 16-bit value onto the stack.
		 *
		 * @param value The 16-bit value to be pushed onto the stack.
		 * @param state The current CPU state.
		 * @param bus The memory bus for writing to memory.
		 *
		 * @details
		 * This method pushes a 16-bit value onto the stack by decrementing the stack pointer and writing the value to memory.
		 * 
		 * @par Stack Growth:
		 * The stack grows downwards in memory, meaning that pushing a value decreases the stack pointer (SP).
		 * 
		 * @note
		 * The stack segment (SS) is used as the segment for the stack operations.
		 * The method assumes that the stack has enough space to accommodate the push operation.
		 */
		static void PUSH(u16 value, CPUState* state, MemoryBus* bus)
		{
			state->SP.X -= 2;
			bus->Write(state->SP.X, value, state->SS, WORD);
		}

		/**
		 * @brief Pops a value from the stack into a register.
		 *
		 * @param reg The register to store the popped value.
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading from memory.
		 *
		 * @details
		 * This method pops a 16-bit value from the stack into a register by reading the value from memory and incrementing the stack pointer.
		 * 
		 * @par Stack Growth:
		 * The stack grows downwards in memory, meaning that popping a value increases the stack pointer (SP).
		 * 
		 * @note
		 * The stack segment (SS) is used as the segment for the stack operations.
		 * The method assumes that there are enough values on the stack to accommodate the pop operation.
		 */
		static void POP(Register& reg, CPUState* state, const MemoryBus* bus)
		{
			reg.X = bus->Read(state->SP.X, state->SS, WORD);
			state->SP.X += 2;
		}

		/**
		 * @brief Pops a value from the stack.
		 *
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading from memory.
		 * @return The 16-bit value popped from the stack.
		 *
		 * @details
		 * This method pops a 16-bit value from the stack by reading the value from memory and incrementing the stack pointer.
		 * 
		 * @par Stack Growth:
		 * The stack grows downwards in memory, meaning that popping a value increases the stack pointer (SP).
		 * 
		 * @note
		 * The stack segment (SS) is used as the segment for the stack operations.
		 * The method assumes that there are enough values on the stack to accommodate the pop operation.
		 */
		static u16 POP(CPUState* state, const MemoryBus* bus)
		{
			state->SP.X += 2;
			return bus->Read(state->SP.X - 2, state->SS, WORD);
		}

		/**
		 * @brief Exchanges the AX register with another register.
		 *
		 * @param reg The register to exchange with the AX register.
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method exchanges the values of the specified register and the AX register.
		 * It uses a temporary variable to hold one of the values during the exchange.
		 */
		static void XCHG_AX_Reg(Register& reg, CPUState* state)
		{
			const Register temp = state->A;
			state->A = reg;
			reg = temp;
		}

		/**
		 * @brief Pushes the flags register onto the stack.
		 *
		 * @param state The current CPU state.
		 * @param bus The memory bus for writing to memory.
		 *
		 * @details
		 * This method pushes the current state of the flags register onto the stack by calling the PUSH method with the flags value.
		 */
		static void PUSHF(CPUState* state, MemoryBus* bus)
		{
			Instr::PUSH(state->SF.Get(), state, bus);
		}

		/**
		 * @brief Pops a value from the stack into the flags register.
		 *
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading from memory.
		 *
		 * @details
		 * This method pops a 16-bit value from the stack and updates the flags register with this value.
		 */
		static void POPF(CPUState* state, const MemoryBus* bus)
		{
			state->SF.Set(Instr::POP(state, bus));
		}

		/**
		 * @brief Store AH into Flags.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method stores the value of the AH register into the lower byte of the flags register (SF).
		 * The upper byte of the flags register remains unchanged.
		 */
		static void SAHF(CPUState* state)
		{
			const u16 flags = state->SF.Get();
			state->SF.Set((flags & 0xFF00) | state->A.H);
		}

		/**
		 * @brief Load Flags into AH.
		 *
		 * @param state The current CPU state.
		 *
		 * @details
		 * This method loads the lower byte of the flags register (SF) into the AH register.
		 */
		static void LAHF(CPUState* state)
		{
			const u16 flags = state->SF.Get();
			state->A.H = flags & 0x00FF;
		}

		/*===========================================================
		  ==================== Control Transfer =====================
		  ===========================================================*/

		/**
		 * @brief Performs an unconditional jump to a new instruction pointer based on an 8-bit signed offset.
		 * 
		 * @param offset The 8-bit signed offset to add to the current instruction pointer (IP).
		 * @param state The current CPU state.
		 * 
		 * @details
		 * This method updates the instruction pointer (IP) by adding the provided 8-bit signed offset to it.
		 * This allows for short jumps within a range of -128 to +127 bytes from the current IP.
		 */
		static void JMP_SHORT(s8 offset, CPUState* state)
		{
			state->IP += offset;
		}

		/**
		 * @brief Performs an unconditional jump to a new instruction pointer based on a 16-bit signed offset.
		 * 
		 * @param offset The 16-bit signed offset to add to the current instruction pointer (IP).
		 * @param state The current CPU state.
		 * 
		 * @details
		 * This method updates the instruction pointer (IP) by adding the provided 16-bit signed offset to it.
		 * This allows for near jumps within a range of -32,768 to +32,767 bytes from the current IP.
		 */
		static void JMP_NEAR(s16 offset, CPUState* state)
		{
			state->IP += offset;
		}

		/**
		 * @brief Performs an unconditional far jump to a new code segment and instruction pointer.
		 * 
		 * @param address The new instruction pointer (IP) to jump to.
		 * @param segment The new code segment (CS) to jump to.
		 * @param state The current CPU state.
		 * 
		 * @details
		 * This method updates both the instruction pointer (IP) and the code segment (CS) to the specified values,
		 * allowing for a far jump to a different segment in memory.
		 */
		static void JMP_FAR(u16 address, u16 segment, CPUState* state)
		{
			state->IP = address;
			state->CS = segment;
		}

		/**
		 * @brief Performs a conditional jump based on a specified condition and an 8-bit signed offset.
		 * 
		 * @param condition The condition to evaluate for the jump (true to jump, false to not jump).
		 * @param offset The 8-bit signed offset to add to the current instruction pointer (IP) if the condition is true.
		 * @param state The current CPU state.
		 * 
		 * @details
		 * This method checks the provided condition, and if it evaluates to true, it updates the instruction pointer (IP)
		 * by adding the provided 8-bit signed offset to it. If the condition is false, the IP remains unchanged.
		 */
		static void JMP_COND(bool condition, s8 offset, CPUState* state)
		{
			if (condition)
			{
				JMP_SHORT(offset, state);
			}
		}

		/**
		 * @brief Calls a near procedure by pushing the current instruction pointer onto the stack and updating the IP with a signed offset.
		 * 
		 * @param offset The 16-bit signed offset to add to the current instruction pointer (IP) for the call.
		 * @param state The current CPU state.
		 * @param bus The memory bus for writing to memory.
		 * 
		 * @details
		 * This method simulates a near procedure call by first pushing the current instruction pointer (IP) onto the stack,
		 * then updating the IP by adding the provided 16-bit signed offset. This allows for calling procedures within
		 * a range of -32,768 to +32,767 bytes from the current IP.
		 */
		static void CALL_NEAR(s16 offset, CPUState* state, MemoryBus* bus)
		{
			Instr::PUSH(state->IP, state, bus);
			state->IP += offset;
		}

		/**
		 * @brief Calls a far procedure by pushing the current code segment and instruction pointer onto the stack,
		 *        then updating the CS and IP with the specified segment and address.
		 * 
		 * @param segment The new code segment (CS) to jump to for the call.
		 * @param address The new instruction pointer (IP) to jump to for the call.
		 * @param state The current CPU state.
		 * @param bus The memory bus for writing to memory.
		 * 
		 * @details
		 * This method simulates a far procedure call by first pushing the current code segment (CS) and instruction pointer (IP)
		 * onto the stack, then updating both the CS and IP to the specified values. This allows for calling procedures in
		 * different segments of memory.
		 */
		static void CALL_FAR(u16 segment, u16 address, CPUState* state, MemoryBus* bus)
		{
			Instr::PUSH(state->CS, state, bus);
			Instr::PUSH(state->IP, state, bus);

			state->IP = address;
			state->CS = segment;
		}

		/*===========================================================
		  =================== String instructions ===================
		  ===========================================================*/

		/**
		 * @brief Moves a byte from the source string to the destination string.
		 * 
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading and writing memory.
		 * 
		 * @details
		 * This method moves a byte from the memory location pointed to by the SI register in the DS segment
		 * to the memory location pointed to by the DI register in the ES segment.
		 * After the move, the SI and DI registers are updated based on the direction flag (DF) in the flags register (SF).
		 * If DF is set (1), SI and DI are decremented; if DF is clear (0), they are incremented.
		 * This allows for string operations to be performed in either direction.
		 */
		static void MOVSB(CPUState* state, MemoryBus* bus)
		{
			const u8 sourceByte = bus->Read(state->SI.X, state->DS, BYTE);

			bus->Write(state->DI.X, sourceByte, state->ES, BYTE);

			if (state->SF.D)
			{
				--state->SI;
				--state->DI;
			}

			else
			{
				++state->SI;
				++state->DI;
			}
		}

		/**
		 * @brief Moves a word (2 bytes) from the source string to the destination string.
		 * 
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading and writing memory.
		 * 
		 * @details
		 * This method moves a word (2 bytes) from the memory location pointed to by the SI register in the DS segment
		 * to the memory location pointed to by the DI register in the ES segment.
		 * After the move, the SI and DI registers are updated based on the direction flag (DF) in the flags register (SF).
		 * If DF is set (1), SI and DI are decremented by 2; if DF is clear (0), they are incremented by 2.
		 * This allows for string operations to be performed in either direction.
		 */
		static void MOVSW(CPUState* state, MemoryBus* bus)
		{
			const u16 sourceWord = bus->Read(state->SI.X, state->DS, WORD);

			bus->Write(state->DI.X, sourceWord, state->ES, WORD);

			if (state->SF.D)
			{
				state->SI.X -= 2;
				state->DI.X -= 2;
			}

			else
			{
				state->SI.X += 2;
				state->DI.X += 2;
			}
		}

		/*===========================================================
		  ================== Instructions groups ====================
		  ===========================================================*/

		/* The instructions groups are instructions that share the same opcode but
		   differ in the operation they perform based on the value of the Reg field
		   in the ModR/M byte. */

		/**
		 * @brief Executes an instruction from a group of instructions based on the value of the Reg field in the ModR/M byte.
		 *
		 * @param op1 The first operand.
		 * @param op2 The second operand.
		 * @param state The current CPU state.
		 * @return The result of the executed instruction.
		 * 
		 * @details
		 * This method executes one of the group instructions (ADD, OR, ADC, SBB, AND, SUB, XOR, CMP) based on the value of the Reg field in the ModR/M byte.
		 * The specific instruction to be executed is determined by the value of state->Reg.
		 * The method takes two operands (op1 and op2) and the current CPU state as parameters.
		 * It returns the result of the executed instruction.
		 * 
		 * @par Affected flags:
		 * - Carry Flag (CF)
		 * - Parity Flag (PF)
		 * - Auxiliary Carry Flag (AF)
		 * - Zero Flag (ZF)
		 * - Sign Flag (SF)
		 * - Overflow Flag (OF)
		 * 
		 * @par How the flags are affected:
		 * - The flags are affected according to the specific instruction executed.
		 * @see Instr::ADD for details on flag effects for the ADD instruction.
		 * @see Instr::OR for details on flag effects for the OR instruction.
		 * @see Instr::ADC for details on flag effects for the ADC instruction.
		 * @see Instr::SBB for details on flag effects for the SBB instruction.
		 * @see Instr::AND for details on flag effects for the AND instruction.
		 * @see Instr::SUB for details on flag effects for the SUB instruction.
		 * @see Instr::XOR for details on flag effects for the XOR instruction.
		 * 
		 * @note
		 * The CMP instruction (when Reg is 7) does not modify the destination operand (op1) but updates the flags based on the comparison.
		 * This group is used for 0x80/0x81/0x82/0x83 opcodes because they use the same instructions.
		 */
		static u16 GRP0(u16 op1, u16 op2, CPUState* state)
		{
			u16 result{};

			switch (state->Reg)
			{
			case 0:
				result = Instr::ADD(op1, op2, state);
				break;

			case 1:
				result = Instr::OR(op1, op2, state);
				break;

			case 2:
				result = Instr::ADC(op1, op2, state);
				break;

			case 3:
				result = Instr::SBB(op1, op2, state);
				break;

			case 4:
				result = Instr::AND(op1, op2, state);
				break;

			case 5:
				result = Instr::SUB(op1, op2, state);
				break;

			case 6:
				result = Instr::XOR(op1, op2, state);
				break;

			case 7:
				Instr::SUB(op1, op2, state);
				result = op1;
				return;
			}

			return result;
		}

		/*===========================================================
		  ======================= Interrupt =========================
		  ==========================================================*/

		/**
		 * @brief Handles a software interrupt.
		 * 
		 * @param interruptNumber The interrupt vector number.
		 * @param state The current CPU state.
		 * @param bus The memory bus for reading and writing memory.
		 * 
		 * @details
		 * This method handles a software interrupt by pushing the current flags, code segment, and instruction pointer onto the stack,
		 * then loading the new instruction pointer and code segment from the interrupt vector table.
		 * It also clears the interrupt and trap flags, and resumes execution if the CPU was halted.
		 * 
		 * @par Affected flags:
		 * - Interrupt Flag (IF)
		 * - Trap Flag (TF)
		 * 
		 * @par How the flags are affected:
		 * - Interrupt flag is cleared to disable further interrupts.
		 * - Trap flag is cleared to disable single-step mode.
		 * 
		 * @note
		 * The interrupt vector table is located at the beginning of memory, with each entry consisting of a 4-byte segment:offset pair.
		 * The interrupt number is used to index into this table to find the appropriate handler.
		 */
		static void INT(u8 interruptNumber, CPUState* state, MemoryBus* bus)
		{
			PUSH(state->SF.Get(), state, bus);
			PUSH(state->CS, state, bus);
			PUSH(state->IP, state, bus);

			state->IP = bus->Read(interruptNumber, state->CS, WORD);
			state->CS = bus->Read(interruptNumber + 2, state->CS, WORD);

			state->SF.I = 0;
			state->SF.T = 0;

			state->mHalted = false;
		}

		/*============================================================
		  ========================= Prefix ===========================
		  ============================================================*/

		/**
		 * @brief Sets a segment override for the next memory access.
		 * 
		 * @param reg The segment register to be used for the override.
		 * @param state The current CPU state.
		 * 
		 * @details
		 * This method sets a segment override for the next memory access by updating the CPU state's mRegisterOverride.
		 * 
		 * @note
		 * This override is only valid for the next memory access and will be cleared afterwards.
		 * Not all instructions support segment overrides.
		 */
		static void RegisterOverride(const Register& reg, CPUState* state)
		{
			state->mRegisterOverride.pending = true;
			state->mRegisterOverride.segment = reg;
		}

	};

} // namespace i8086