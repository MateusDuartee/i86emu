// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "MemoryBus.hpp"

#include <stdexcept>

namespace i8086
{

    void i8086::MemoryBus::AttachDevice(IMemoryDevice *device, u32 startAddress, u32 endAddress)
    {
        if (device == nullptr)
        {
            throw std::runtime_error("MemoryBus::AttachDevice -> Trying to attach a null device");
        }

        for (const auto& mapping : mMappings)
        {
            const bool overlap = (startAddress <= mapping.endAddress) && (endAddress >= mapping.startAddress);

            if (overlap)
            {
                throw std::runtime_error("MemoryBus::AttachDevice -> Device address range overlaps with an existing device");
            }
        }

        mMappings.push_back({ device, startAddress, endAddress });
    }

    void i8086::MemoryBus::DetachDevice(IMemoryDevice* device)
    {

        const auto it = std::find_if(mMappings.begin(), mMappings.end(),
            [device](const Mapping& mapping) { 
                return mapping.device == device; 
        });

        if (it == mMappings.end())
        {
            throw std::runtime_error("MemoryBus::DetachDevice -> Device not found");
        }
        
        mMappings.erase(it);

    }

    u16 i8086::MemoryBus::Read(u16 address, const Register& segment, u8 size, bool notify) const
    {
        const u32 segmentedAddress = (segment.X << 4) + address;

        for (const auto& mapping : mMappings)
        {
            if (segmentedAddress >= mapping.startAddress && segmentedAddress <= mapping.endAddress)
            {
                if (notify)
                {
                    for (const auto& observer : mObservers)
                    {
                        observer->OnRead(segmentedAddress);
                    }
                }

                return mapping.device->Read(segmentedAddress - mapping.startAddress, size);
            }
        }

        throw std::runtime_error("MemoryBus::Read -> No device mapped to the given address");

    }

    void i8086::MemoryBus::Write(u16 address, u16 data, const Register& segment, u8 size, bool notify)
    {
        const u32 physicalAddress = (segment.X << 4) + address;

        for (const auto& mapping : mMappings)
        {
            if (physicalAddress >= mapping.startAddress && physicalAddress <= mapping.endAddress)
            {
                mapping.device->Write(physicalAddress - mapping.startAddress, data, size);
                
                if (notify)
                {
                    for (const auto& observer : mObservers)
                    {
                        observer->OnWrite(physicalAddress, data);
                    }
                }

                return;
            }
        }

        throw std::runtime_error("MemoryBus::Write -> No device mapped to the given address");

    }

    void MemoryBus::DumpMemory(std::vector<u8> &outMemory) const
    {
        outMemory.clear();
        outMemory.resize(GetSize(), 0);

        for (const auto& mapping : mMappings)
        {
            for (u32 i = 0; i <= mapping.endAddress; ++i)
            {
                outMemory[mapping.startAddress + i] = mapping.device->Read(i, 8);
            }
        }
    }

    size_t MemoryBus::GetSize() const
    {
        size_t totalSize = 0;

        for (const auto& mapping : mMappings)
        {
            totalSize += (mapping.endAddress - mapping.startAddress + 1);
        }

        return totalSize;
    }

    void MemoryBus::RegisterObserver(IMemoryObserver* observer)
    {
        mObservers.push_back(observer);
    }

    void MemoryBus::UnregisterObserver(IMemoryObserver* observer)
    {
        mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
    }

} // namespace i8086