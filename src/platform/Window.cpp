#include "Window.h"

#include <iostream>

Window::Window(const std::string& title, int width, int height) : m_Title(title), m_Width(width), m_Height(height) 
{
    
}

Window::~Window() 
{
    Shutdown();
}

bool Window::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL Init Failed: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Log("Video Driver: %s",
            SDL_GetCurrentVideoDriver());

    m_Window = SDL_CreateWindow(
        "PhysicsEngine2D",
        1280,
        720,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!m_Window)
    {
        std::cerr << "Window Creation Failed: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    SDL_ShowWindow(m_Window);
    SDL_RaiseWindow(m_Window);

    SDL_Log("Window pointer: %p", m_Window);

    return true;
}

void Window::Shutdown()
{
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }

    SDL_Quit();
}

SDL_Window* Window::GetSDLWindow() const
{
    return m_Window;
}