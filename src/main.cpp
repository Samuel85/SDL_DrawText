#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "drawtext.h"

#define MINIMUM_FRAME_TIME 10

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Surface *screen;
SDL_Texture *texture;

int init() {
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Examples", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    return -1;
  }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  screen = SDL_CreateRGBSurface(0, 800, 600, 32, 0xff000000, 0x00ff0000,
                                0x0000ff00, 0x000000ff);
#else
  screen = SDL_CreateRGBSurface(0, 800, 600, 32, 0x000000ff, 0x0000ff00,
                                0x00ff0000, 0xff000000);
#endif

  texture = SDL_CreateTextureFromSurface(renderer, screen);

  if (TTF_Init() < 0) {
    std::cout << "Can't init TTF: " << SDL_GetError() << std::endl;
    return -1;
  }
  return 0;
}

void updateScreen() {
  SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void mainLoop() {
  SDL_Event event;
  bool quit = false;
  unsigned int initialTime = 0;

  while (!quit) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      quit = true;
    }

    initialTime = SDL_GetTicks();
    updateScreen();

    if (SDL_GetTicks() - initialTime < MINIMUM_FRAME_TIME) {
      SDL_Delay(MINIMUM_FRAME_TIME - (SDL_GetTicks() - initialTime));
    }
  }
}

void quit() {
  SDL_FreeSurface(screen);
  SDL_DestroyTexture(texture);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
}

int main() {
  init();

  // Init DrawText object
  SDL_Color white = {255, 255, 255, 0};
  SDL_Color red = {255, 0, 0, 0};

  auto *text = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 25, white);
  auto *redText = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 20, red);

  // Print some text
  text->print(screen,
              L"Uicode string:\nEn un lugar de la Mancha,"
              "de cuyo nombre no quiero acordarme,\nno ha mucho tiempo"
              "que vivía un hidalgo de los de lanza en astillero,"
              "\nadarga antigua, rocín flaco y galgo corredor.",
              10, 50);

  text->print(screen,
              "Non-uicode string:\nEn un lugar de la Mancha,"
              "de cuyo nombre no quiero acordarme,\nno ha mucho tiempo"
              "que vivía un hidalgo de los de lanza en astillero,"
              "\nadarga antigua, rocín flaco y galgo corredor.",
              10, 200);

  redText->print(screen, "Pi=%f", 10, 350, 3.141592);

  mainLoop();

  quit();
  return 0;
}
