#include "drawtext.h"
#include <string>
#include <stdio.h>

#define TRANSFORM_STRING(buffer, start_args) \
{ \
    va_list lst; \
    va_start(lst, start_args); \
    vsnprintf(buffer, BUFFER_SIZE, start_args, lst); \
    va_end(lst); \
}

#define TRANSFORM_WSTRING(buffer, start_args) \
{ \
    va_list lst; \
    va_start(lst, start_args); \
    vswprintf(buffer, BUFFER_SIZE, start_args, lst); \
    va_end(lst); \
}

DrawText::DrawText(const char *pathFont, const int size, const SDL_Color &color)
{
    TTF_Font *font = TTF_OpenFont(pathFont, size);
    createAlphabet(font, color);
    TTF_CloseFont(font);
    error = false;
    newLine = false;
}

DrawText::~DrawText()
{
    for (int i=0; i < TOTAL_CHARACTERS; i++){
        SDL_FreeSurface(alphabet[i]);
    }
}

void DrawText::createAlphabet(TTF_Font *font, const SDL_Color &color)
{
    for (Uint16 c = INITIAL_CHARACTER; c <= FINAL_CHARACTER; c++){
        alphabet[c-INITIAL_CHARACTER] = TTF_RenderGlyph_Blended(font, c, color);
    }
}

void DrawText::drawGlyph(SDL_Surface *dst, const unsigned char c, int &x, int &y)
{
    if (c == '\n'){
        SDL_Surface *glyph = alphabet[0];
        y = y + glyph->h;   
        newLine = true;
        return;
    }
    
    unsigned char i = c - INITIAL_CHARACTER;
    if (i > FINAL_CHARACTER) return;
    SDL_Surface *glyph = alphabet[i];
    
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = glyph->w;
    dst_rect.h = glyph->h;
        
    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, dst, &dst_rect); 
}

void DrawText::print(SDL_Surface *dst, const char *str, const int x, const int y, ...)
{
    if (error) return;
    
    int _x = x;
    int _y = y;
    
    char buffer[BUFFER_SIZE] = {};
    TRANSFORM_STRING(buffer, str)

    for (char *c = buffer; *c != '\0'; c++)
    {
        if (newLine){
            _x = x;
            newLine = false;
        }
        drawGlyph(dst, *c, _x, _y);
    }
}

void DrawText::drawGlyphW(SDL_Surface *dst, const wchar_t c, int &x, int &y)
{
    if (c == '\n'){
        SDL_Surface *glyph = alphabet[0];
        y = y + glyph->h;   
        newLine = true;
        return;
    }
    unsigned char i = (unsigned char) c - INITIAL_CHARACTER;    
    if (i >= FINAL_CHARACTER) return;

    SDL_Surface *glyph = alphabet[i];
    
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = glyph->w;
    dst_rect.h = glyph->h;
        
    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, dst, &dst_rect); 
}

void DrawText::print(SDL_Surface *dst, const wchar_t *str, const int x, const int y, ...)
{
    if (error) return;
    
    int _x = x;
    int _y = y;
    
    wchar_t buffer[BUFFER_SIZE] = {};
    TRANSFORM_WSTRING(buffer, str)

    for (wchar_t *c = buffer; *c != '\0'; c++)
    {
        if (newLine){
            _x = x;
            newLine = false;
        }
        drawGlyphW(dst, *c, _x, _y);
    }
}

