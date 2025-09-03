// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>

namespace i8086
{

    class IMemoryObserver
    {

    public:

        virtual ~IMemoryObserver() = default;

        virtual void OnWrite(u32 address, u16 data) = 0;
        virtual void OnRead(u32 address) = 0;

    };

} // namespace i8086