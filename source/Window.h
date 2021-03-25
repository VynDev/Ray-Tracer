#include "rt.h"
#include "Vector3.h"
#include <SDL2/SDL.h>

class Window
{
private:

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    
public:
    Window(int width, int height)
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
            throw "EXIT_FAILURE";
        }
        window = SDL_CreateWindow("Ray Tracer - Vyn", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
        if (!window)
        {
            fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
            throw "EXIT_FAILURE";
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
            throw "EXIT_FAILURE";
        }

        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            throw "EXIT_FAILURE";
        }
        
        if (SDL_RenderClear(renderer))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            throw "EXIT_FAILURE";
        }
        
    }

    ~Window()
    {
        if (renderer)
        SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void WriteColor(Vector3 position, Color pixelColor, int samplesPerPixel) {

        auto r = pixelColor.X();
        auto g = pixelColor.Y();
        auto b = pixelColor.Z();

        auto scale = 1.0 / samplesPerPixel;

        r = sqrt(r * scale);
        g = sqrt(g * scale);
        b = sqrt(b * scale);

        SDL_SetRenderDrawColor(renderer, 256 * Clamp(r, 0.0, 0.999), 256 *  Clamp(g, 0.0, 0.999), 256 * Clamp(b, 0.0, 0.999), 255);
        SDL_RenderDrawPoint(renderer, position.X(), position.Y());
    }

    void Render() {
        SDL_RenderPresent(renderer);
    }

    void Wait() {
        SDL_Event event;
        while (event.type != SDL_QUIT) {
            SDL_WaitEvent(&event);
        }
    }
};


