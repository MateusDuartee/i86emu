// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Interfaces/IIODevice.hpp>
#include <Utils/types.hpp>

#include <vector>

namespace i8086::IO
{

	class IOBus
	{

	public:

		void AttachDevice(IO::IIODevice* device);
		void DetachDevice(IO::IIODevice* device);
		
		u16 Read(u16 port, u8 size) const;	
		void Write(u16 port, u16 data, u8 size) const;

	private:
		std::vector<IO::IIODevice*> mDevices;
	};

} // namespace i8086::IO