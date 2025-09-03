// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Model/Register.hpp>
#include <Model/RAM.hpp>
#include <Interfaces/IMemoryDevice.hpp>

#include <string>
#include <vector>

namespace i8086
{
    class RAMController : public IMemoryDevice
    {

    public:

        RAMController(RAM* const ram) : mRAM(ram) {}

        void Write(u32 address, u16 data, u8 size) override;
        u16 Read(u32 address, u8 size) const override;

        size_t GetSize() const override;

        void LoadFile(const std::string& filepath, u32 address) const;

        const std::vector<u8>& GetMemory() const
		{
			return mRAM->GetMemory();
		}

    private:

        RAM* const mRAM;

    };

} // namespace i8086