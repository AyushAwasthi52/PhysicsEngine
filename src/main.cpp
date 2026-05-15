#include "platform/Window.h"
#include "renderer/VulkanContext.h"

#include <SDL3/SDL.h>

int main()
{
    Window window("PhysicsEngine2D", 1280, 720);

    if (!window.Init())
        return -1;

    VulkanContext renderer;

    if (!renderer.Init(window))
        return -1;

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        renderer.BeginFrame();

        // Draw calls here later

        renderer.EndFrame();
    }

    renderer.Shutdown();

    return 0;
}