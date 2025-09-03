// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include <Core/EmulatorApp.hpp>

#include <filesystem>

int main(int argc, char** argv)
{

    std::filesystem::current_path(std::filesystem::path(__FILE__).parent_path().parent_path());

    EmulatorApp app;

    app.Run();
    
    return 0;
}