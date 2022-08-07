#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "drawtext.h"

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface* screen;
SDL_Texture* texture;

int init()
{
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
	static constexpr int WINDOW_BPP = 32;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Examples", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return -1;
	}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	screen = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	screen = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif

	texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (TTF_Init() < 0) {
		std::cout << "Can't init TTF: " << SDL_GetError() << std::endl;
		return -1;
	}
	return 0;
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
	bool quit = false;
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

	// Init DrawText object
	SDL_Color white = { 255, 255, 255, 0 };
	SDL_Color red = { 255, 0, 0, 0 };

	auto* text = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 25, white);
	auto* redText = new DrawText("fonts/OfenbacherSchwabCAT.ttf", 20, red);

	// Print some text
	text->print(screen, L"Uicode string:\nEn un lugar de la Mancha,"
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

	redText->print(screen, DrawText::format("Pi=%f", 3.141592), 10, 350);

	mainLoop();

	quit();
	return 0;
}
