// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

namespace UI
{

    class IViewWindow
    {

    public:
        virtual ~IViewWindow() = default;

        virtual void ShowIfOpen() = 0;

		virtual void ToggleVisibility()
		{
			mIsOpen = !mIsOpen;
		}

		virtual bool IsOpen() const
		{
			return mIsOpen;
		}

    protected:
        bool mIsOpen{ false };
        
    };

} // namespace UI