// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Model/I8086.hpp>

#include <stdexcept>

namespace i8086
{

	class CPUController
	{
	
	public:

		CPUController(I8086* const cpu) : mCPU(cpu)
		{
			if (!mCPU)
			{
				throw std::runtime_error("CPU is not initialized.");
			}

			mCPU->GetInternalState(mInitialState);
		}

		void SetBreakpoint(u32 address, bool state) const
		{
			mCPU->SetBreakpoint(address, state);
		}

		void GetState(CPUState& state) const
		{
			return mCPU->GetInternalState(state);
		}

		const CPUState& GetInitialState() const
		{
			return mInitialState;
		}

	private:

		I8086* const mCPU{ nullptr };
		CPUState mInitialState{};
	};

} // namespace i8086