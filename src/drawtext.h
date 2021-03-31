#ifndef _DRAWTEXT_H_
#define _DRAWTEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#define BUFFER_SIZE 256
#define INITIAL_CHARACTER 32
#define FINAL_CHARACTER 255
#define TOTAL_CHARACTERS FINAL_CHARACTER - INITIAL_CHARACTER

class DrawText
{
    private:
            SDL_Surface *alphabet[TOTAL_CHARACTERS];
            bool error;
            bool newLine;
            
            void createAlphabet(TTF_Font *font, const SDL_Color &color);
            void drawGlyph(SDL_Surface *dst, const unsigned char c, int &x, int &y);
            void drawGlyphW(SDL_Surface *dst, const wchar_t c, int &x, int &y);
    public:
            DrawText(const char *pathFont, const int size, const SDL_Color &color);
            ~DrawText();
            
            void print(SDL_Surface *dst, const char *str, const int x, const int y, ...);                        
            void print(SDL_Surface *dst, const wchar_t * str, const int x, const int y, ...);
};

#endif
