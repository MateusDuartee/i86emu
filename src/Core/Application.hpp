// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

class Application {

public:
    Application(const char* title, int width, int height);
    virtual ~Application();

    void Run();

protected:
    virtual void OnRender() {}
    virtual void OnEvent(const SDL_Event& event) {}

private:
    void InitSDL(const char* title, int width, int height);
    void InitImGui();
    void Shutdown();

    SDL_Window* mWindow = nullptr;
    SDL_GLContext mGLContext = nullptr;
    bool mRunning = true;

};