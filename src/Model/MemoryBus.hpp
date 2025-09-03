// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Register.hpp"
#include <Interfaces/IMemoryObserver.hpp>
#include <Interfaces/IMemoryDevice.hpp>
#include <Utils/types.hpp>

#include <vector>
#include <algorithm>

namespace i8086
{

    class MemoryBus
    {

    public:

        void AttachDevice(IMemoryDevice* device, u32 startAddress, u32 endAddress);
        void DetachDevice(IMemoryDevice* device);

        u16 Read(u16 address, const Register& segment, u8 size, bool notify = false) const;
        void Write(u16 address, u16 data, const Register& segment, u8 size, bool notify = false);

        void DumpMemory(std::vector<u8>& outMemory) const;
        size_t GetSize() const;

        void RegisterObserver(IMemoryObserver* observer);
        void UnregisterObserver(IMemoryObserver* observer);
        
    private:

        struct Mapping
        {
            IMemoryDevice* device{ nullptr };
            u32 startAddress{ 0 };
            u32 endAddress{ 0 };
        };

        std::vector<Mapping> mMappings;
        std::vector<IMemoryObserver*> mObservers;
    };

} // namespace i8086