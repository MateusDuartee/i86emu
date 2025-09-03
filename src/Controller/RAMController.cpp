// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "RAMController.hpp"

#include <fstream>
#include <stdexcept>

namespace i8086
{

    void RAMController::Write(u32 address, u16 data, u8 size)
	{

		if (size == 8)
		{
			mRAM->Write(address, data & 0xFF);
			return;
		}

		mRAM->Write(address, data & 0xFF);
		mRAM->Write(address + 1, (data >> 8) & 0xFF);
	}

	u16 RAMController::Read(u32 address, u8 size) const
	{

		if (size == 8)
		{
			return mRAM->Read(address);
		}

		return static_cast<u16>(mRAM->Read(address + 1) << 8) | mRAM->Read(address);
	}

    size_t RAMController::GetSize() const
    {
        return mRAM->GetSize();
    }

    void RAMController::LoadFile(const std::string& filepath, u32 address) const
    {
        std::ifstream file(filepath, std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("RAMController::LoadFile -> Cannot open the file");
        }

        file.seekg(0, std::ios_base::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios_base::beg);

        if (fileSize + address > mRAM->GetSize())
        {
            throw std::runtime_error("RAMController::LoadFile -> File is too large to fit in memory from the given address");
        }

        for (size_t i = 0; i < fileSize; ++i)
        {
            mRAM->Write(address + i, file.get());
        }
    }
    
} // namespace i8086