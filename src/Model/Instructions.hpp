// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "CPUState.hpp"

namespace i8086
{
	
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