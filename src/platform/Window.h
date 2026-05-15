#pragma once

#include <SDL3/SDL.h>
#include <string>

class Window
{
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool Init();
    void Shutdown();

    SDL_Window* GetSDLWindow() const;

private:
    std::string m_Title;
    int m_Width;
    int m_Height;

    SDL_Window* m_Window = nullptr;
};