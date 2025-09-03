// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>

#include <vector>
#include <stdexcept>

namespace i8086
{

	class RAM
	{

	public:

		RAM(u32 size) : mMemory(size), mMemorySize(size) {};

		void Write(u32 address, u8 data)
		{
			if (address >= mMemorySize)
			{
				throw std::runtime_error("RAM::Write -> Trying to write to an invalid address");
			}

			mMemory[address] = data;
		}

		u8 Read(u32 address) const
		{
			if (address >= mMemorySize)
			{
				throw std::runtime_error("RAM::Read -> Trying to read from an invalid address");
			}

			return mMemory[address];
		}

		const std::vector<u8>& GetMemory() const
		{
			return mMemory;
		}

		size_t GetSize() const
		{
			return mMemorySize;
		}

	private:
		std::vector<u8> mMemory;
		const size_t mMemorySize;
	};

} // namespace i8086