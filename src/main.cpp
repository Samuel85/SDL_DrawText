#include "drawtext.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <sstream>

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* screen;
SDL_Texture* texture;

void init()
{
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int WINDOW_BPP = 32;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Examples", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::stringstream out;
        out << "Can't create window: " << SDL_GetError();
        throw std::runtime_error(out.str());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::stringstream out;
        out << "Can't create render: " << SDL_GetError();
        throw std::runtime_error(out.str());
    }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    screen = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0xff000000,
                                  0x00ff0000, 0x0000ff00, 0x000000ff);
#else
    screen = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0x000000ff,
                                  0x0000ff00, 0x00ff0000, 0xff000000);
#endif

    texture = SDL_CreateTextureFromSurface(renderer, screen);

    if (TTF_Init() < 0) {
        std::stringstream out;
        out << "Can't init TTF: " << SDL_GetError();
        throw std::runtime_error(out.str());
    }
}

void updateScreen()
{
    SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void mainLoop()
{
    static constexpr unsigned int MINIMUM_FRAME_TIME = 10;

    SDL_Event event;
    auto quit = false;
    unsigned int initialTime = 0;

    while (!quit) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            quit = true;
        }

        initialTime = SDL_GetTicks();
        updateScreen();

        auto delta_time = SDL_GetTicks() - initialTime;
        if (delta_time < MINIMUM_FRAME_TIME) {
            SDL_Delay(MINIMUM_FRAME_TIME - delta_time);
        }
    }
}

void quit()
{
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{

    init();

    //// Init DrawText object
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color red = {255, 0, 0, 0};

    auto* text = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 25, white);
    auto* redText = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 20, red);

    text->print(screen,
                L"Non-unicode string\nEn un lugar de la Mancha,\n"
                "de cuyo nombre no quiero acordarme, no ha mucho tiempo\n"
                "que vivía un hidalgo de los de lanza en astillero,\n"
                "adarga antigua, rocín flaco y galgo corredor.",
                10, 50);

    text->print(screen,
                L"Non-unicode string\nEn un lugar de la Mancha,\n"
                "de cuyo nombre no quiero acordarme, no ha mucho tiempo\n"
                "que vivía un hidalgo de los de lanza en astillero,\n"
                "adarga antigua, rocín flaco y galgo corredor.",
                10, 200);

    redText->print(screen, DrawText::format("Pi=%f", 3.141592), 10, 350);

    mainLoop();

    // quit();

    return 0;
}
