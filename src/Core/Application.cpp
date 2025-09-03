// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#include "Application.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>

#include <filesystem>

constexpr const char* glslVersion = "#version 330";
constexpr ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

Application::Application(const char* title, int width, int height)
{
    InitSDL(title, width, height);
    InitImGui();
}

Application::~Application()
{
    Shutdown();
}

void Application::InitSDL(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    constexpr Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    
    mWindow = SDL_CreateWindow(title, width, height, windowFlags);

    if (mWindow == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    mGLContext = SDL_GL_CreateContext(mWindow);

    if (mGLContext == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_MakeCurrent(mWindow, mGLContext);
    SDL_ShowWindow(mWindow);
}

void Application::InitImGui()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    auto& style = ImGui::GetStyle();

    style.WindowMenuButtonPosition = ImGuiDir_None;

    ImGui::GetIO().Fonts->AddFontFromFileTTF(std::filesystem::path("Resources/Fonts/FiraCode-SemiBold.ttf").string().c_str(), 16.0f);

    ImGui_ImplSDL3_InitForOpenGL(mWindow, mGLContext);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void Application::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(mGLContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Application::Run() {
    while (mRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                mRunning = false;

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(mWindow))
                mRunning = false;

            OnEvent(event);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        OnRender();

        ImGui::Render();

        ImGuiIO& io = ImGui::GetIO();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
            SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
        }

        SDL_GL_SwapWindow(mWindow);
    }
}