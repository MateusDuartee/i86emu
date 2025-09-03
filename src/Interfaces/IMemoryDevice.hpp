// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>
#include <cstddef>

namespace i8086
{

    class IMemoryDevice
    {

    public:

        virtual void Write(u32 address, u16 data, u8 size) = 0;
        virtual u16 Read(u32 address, u8 size) const = 0;
        virtual size_t GetSize() const = 0;

    };

} // namespace i8086