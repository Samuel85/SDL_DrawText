#include "drawtext.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>

DrawText::DrawText(const char* fontPath, const int fontSize, const SDL_Color& fontColor)
{
    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        std::stringstream message;
        message << "DrawText::DrawText: " << TTF_GetError();
        throw std::runtime_error(message.str());
    }
    createAlphabet(font, fontColor);
    TTF_CloseFont(font);
}

DrawText::~DrawText()
{
    for (auto& i : alphabet) {
        SDL_FreeSurface(i);
    }
}

void DrawText::createAlphabet(TTF_Font* font, const SDL_Color& fontColor)
{
    for (Uint16 c = INITIAL_CHARACTER; c <= FINAL_CHARACTER; c++) {
        alphabet[c - INITIAL_CHARACTER] = TTF_RenderGlyph_Blended(font, c, fontColor);
    }
}

void DrawText::drawGlyph(SDL_Surface* destinationSurface, const unsigned char character, int& x,
                         int& y)
{
    if (character == '\n') {
        SDL_Surface* glyph = alphabet[0];
        y = y + glyph->h;
        newLine = true;
        return;
    }

    unsigned char i = character - INITIAL_CHARACTER;
    SDL_Surface* glyph = alphabet[i];

    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = glyph->w;
    dst_rect.h = glyph->h;

    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}

void DrawText::print(SDL_Surface* destinationSurface, const std::string& text, int x, int y)
{
    auto tmp_x = x;
    auto tmp_y = y;

    for (const char* c = text.c_str(); *c != '\0'; c++) {
        if (newLine) {
            tmp_x = x;
            newLine = false;
        }
        drawGlyph(destinationSurface, *c, tmp_x, tmp_y);
    }
}

void DrawText::drawGlyphW(SDL_Surface* destinationSurface, const wchar_t character, int& x, int& y)
{
    if (character == '\n') {
        SDL_Surface* glyph = alphabet[0];
        y = y + glyph->h;
        newLine = true;
        return;
    }
    unsigned char i = (unsigned char)character - INITIAL_CHARACTER;
    if (i >= FINAL_CHARACTER) {
        return;
    }
    SDL_Surface* glyph = alphabet[i];

    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = glyph->w;
    dst_rect.h = glyph->h;

    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}

void DrawText::print(SDL_Surface* destinationSurface, const std::wstring& text, int x, int y)
{
    auto tmp_x = x;
    auto tmp_y = y;

    for (auto character = text.c_str(); *character != '\0'; character++) {
        if (newLine) {
            tmp_x = x;
            newLine = false;
        }
        drawGlyphW(destinationSurface, *character, tmp_x, tmp_y);
    }
}
std::string DrawText::format(const std::string text, ...)
{
    std::string a;

    char buffer[BUFFER_SIZE] = {};
    {
        va_list list;
        va_start(list, text);
        vsnprintf(buffer, BUFFER_SIZE, text.c_str(), list);
        va_end(list);
    };
    a = buffer;
    return a;
}

std::wstring DrawText::format(const std::wstring text, ...)
{
    std::wstring out;
    wchar_t buffer[BUFFER_SIZE] = {};
    {
        va_list list;
        va_start(list, text);
        vswprintf(buffer, BUFFER_SIZE, text.c_str(), list);
        va_end(list);
    };
    out = buffer;
    return out;
}