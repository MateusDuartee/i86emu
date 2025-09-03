// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Utils/types.hpp>

namespace i8086::IO
{

	class IIODevice
	{

	public:
		virtual ~IIODevice() = default;

		virtual u16 Read(u16 port, u8 size) const = 0;
		virtual void Write(u16 port, u16 data, u8 size) = 0;

		virtual bool UsesPort(u16 port) const
		{
			return (port >= mStartPort && port <= mEndPort);
		}
	
		u16 GetStartPort() const { return mStartPort; }
		u16 GetEndPort() const { return mEndPort; }

	protected:
		const u16 mStartPort{};
		const u16 mEndPort{};
	};

} // namespace i8086::IO