// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

/**
 * @brief Sets the nth bit of x to v.
 * 
 * @param x The variable whose bit is to be set.
 * @param n The bit position to set (0-indexed).
 * @param v The value to set the bit to.
 */
#define SET_BIT(x, n, v) (x) = ((x) & ~(1 << (n))) | ((v) << (n));

/**
* @brief Gets the nth bit of x.
* 
* @param x The variable from which to get the bit.
* @param n The bit position to get (0-indexed).
* 
* @return The value of the nth bit.
*/
#define GET_BIT(x, n) (((x) >> (n)) & 1)

/**
 * @brief Sets the high byte of a 16-bit value.
 * 
 * @param x The variable whose high byte is to be set.
 * @param v The value to set the high byte to (0x00 - 0xFF).
 */

#define SET_H(x, v) (x) = ((((v) << 8) & 0xFF00) | ((x) & 0x00FF))

/**
 * @brief Sets the low byte of a 16-bit value.
 * 
 * @param x The variable whose low byte is to be set.
 * @param v The value to set the low byte to (0x00 - 0xFF).
 */
#define SET_L(x, v) (x) = (((v) & 0x00FF) | ((x) & 0xFF00))

/**
 * @brief Gets the low byte of a 16-bit value.
 * 
 * @param x The variable from which to get the low byte.
 * 
 * @return The low byte of the 16-bit value.
 */
#define GET_L(x) ((x) & 0x00FF)

/**
 * @brief Gets the high byte of a 16-bit value.
 * 
 * @param x The variable from which to get the high byte.
 * 
 * @return The high byte of the 16-bit value.
 */
#define GET_H(x) ((x) & 0xFF00)

/**
 * @brief Gets the least significant bit of a value.
 * 
 * @param x The value from which to get the least significant bit.
 * 
 * @return The least significant bit of the value.
 */
#define LSB(x) ((x) & 1)

/**
 * @brief Gets the most significant bit of a value.
 * 
 * @param x The value from which to get the most significant bit.
 * @param operandSize The size of the operand in bits (e.g., 8, 16, 32, 64).
 * 
 * @return The most significant bit of the value.
 */
#define MSB(x, operandSize) (((x) >> ((operandSize) - 1)) & 1)

/**
 * @brief Masks the value to keep only the bits that match the operand size.
 * 
 * @param x The value to be masked.
 * @param operandSize The size of the operand in bits (e.g., 8, 16, 32).
 * 
 * @return The masked value.
 * 
 * @warning
 * - For operand sizes greater than 63, this macro has undefined behavior.
 */
#define MASK(x, operandSize) ((x) & ~(0xFFFFFFFFFFFFFFFF << (operandSize)))