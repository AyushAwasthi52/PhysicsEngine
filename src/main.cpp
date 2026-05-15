#include <SDL3/SDL.h>

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "TEST WINDOW",
        1280,
        720,
        0
    );

    if (!window)
    {
        SDL_Log("Window failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, nullptr);

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}