// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>

namespace i8086
{

	struct RegByte {
			u16& ref;
			u8 shift;

			operator u8() const {
				return static_cast<u8>((ref >> shift) & 0xFF);
			}

			RegByte& operator=(u8 v) {
				ref = (ref & ~(0xFFu << shift)) | (static_cast<u16>(v) << shift);
				return *this;
			}

			RegByte& operator+=(u8 v) { return *this = static_cast<u8>(*this + v); }
			RegByte& operator-=(u8 v) { return *this = static_cast<u8>(*this - v); }
			RegByte& operator&=(u8 v) { return *this = static_cast<u8>(*this & v); }
			RegByte& operator|=(u8 v) { return *this = static_cast<u8>(*this | v); }
			RegByte& operator^=(u8 v) { return *this = static_cast<u8>(*this ^ v); }
    };

	class Register
	{

	public:
		Register() : mValue(0) {}
		Register(u16 value) : mValue(value) {}

		u16& X = mValue;
		RegByte L{mValue, 0};
		RegByte H{mValue, 8};

	public:

		Register& operator=(const Register& other)
		{
			X = other.X;
			return *this;
		}

		Register& operator=(u16 value)
		{
			X = value;
			return *this;
		}

		Register& operator+=(u16 value)
		{
			X += value;
			return *this;
		}

		Register& operator-=(u16 value)
		{
			X -= value;
			return *this;
		}

		u16 operator+(const Register& other)
		{
			return  X + other.X;
		}

		u16 operator-(const Register& other)
		{
			return  X - other.X;
		}

		Register& operator++()
		{
			++X;

			return *this;
		}

		Register operator++(int)
		{
			Register temp = *this;

			++X;

			return temp;
		}

		Register& operator--()
		{
			--X;

			return *this;
		}

		Register operator--(int)
		{
			Register temp = *this;

			--X;

			return temp;
		}

	private:
		u16 mValue{};

	};

} // namespace i8086