// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "IOBus.hpp"

#include <algorithm>
#include <stdexcept>

namespace i8086::IO
{

	void IOBus::AttachDevice(IO::IIODevice* newDevice)
	{
		for (const auto& device : mDevices)
		{
			if (newDevice->GetStartPort() <= device->GetEndPort() && newDevice->GetEndPort() >= device->GetStartPort())
			{
				throw std::runtime_error("IOBus::AttachDevice -> Port conflict detected.");
			}
		}

		mDevices.push_back(newDevice);
	}

	void IOBus::DetachDevice(IO::IIODevice* device)
	{
		const auto it = std::find(mDevices.begin(), mDevices.end(), device);

		if (it != mDevices.end())
		{
			mDevices.erase(it);
		}
	}


	u16 IOBus::Read(u16 port, u8 size) const
	{
		for (const auto& device : mDevices)
		{
			if (device->UsesPort(port))
			{
				return device->Read(port, size);
			}
		}

		return 0x0000;
	}

	void IOBus::Write(u16 port, u16 data, u8 size) const
	{
		for (const auto& device : mDevices)
		{
			if (device->UsesPort(port))
			{
				device->Write(port, data, size);
				return;
			}
		}
	}


} // namespace i8086::IO