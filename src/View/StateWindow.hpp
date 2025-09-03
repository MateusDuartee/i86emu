// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <Model/MemoryBus.hpp>
#include <Controller/CPUController.hpp>
#include <Interfaces/IViewWindow.hpp>

namespace UI
{

	class StateWindow : public IViewWindow
	{

	public:

		StateWindow(const i8086::CPUController* cpuController, const i8086::MemoryBus* bus) 
		: mCPUController(cpuController), mBus(bus), mCPUInitialState(mCPUController->GetInitialState()) {};

		void ShowIfOpen() override;

	private:

		inline void RenderGeneralRegisters() const;
		inline void RenderSegmentRegisters() const;
		inline void RenderFlags() const;
		inline void RenderModRM() const;
		inline void RenderStack() const;

	private:

		const i8086::CPUController* const mCPUController{ nullptr };
		const i8086::MemoryBus* const mBus { nullptr };

		i8086::CPUState mState{};
		const i8086::CPUState mCPUInitialState{};


	};

} // namespace UI