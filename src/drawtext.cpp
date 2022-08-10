#include "drawtext.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>

DrawText::DrawText(const char* fontPath, const int fontSize, const SDL_Color& fontColor,
                   int initialCharacter, int finalCharacter)
    : initialCharacter_{initialCharacter}, finalCharacter_{finalCharacter},
      totalCharacters_{static_cast<size_t>(finalCharacter_ - initialCharacter_)},
      alphabet{new SDL_Surface*[totalCharacters_]}
{
    if (totalCharacters_ <= 0) {
        throw std::logic_error("The final character must be bigger than the initial character.");
    }

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
    delete[] alphabet;
}

void DrawText::createAlphabet(TTF_Font* font, const SDL_Color& fontColor)
{
    for (auto c = initialCharacter_; c < finalCharacter_; c++) {
        alphabet[c - initialCharacter_] =
            TTF_RenderGlyph_Blended(font, static_cast<uint16_t>(c), fontColor);
    }
}

void DrawText::drawGlyphW(SDL_Surface* destinationSurface, unsigned char character, int& x, int& y)
{
    if (character == '\n') {
        SDL_Surface* glyph = alphabet[0];
        y = y + glyph->h;
        newLine_ = true;
        return;
    }
    auto i = character - static_cast<size_t>(initialCharacter_);
    if (i >= totalCharacters_) {
        throw std::logic_error("Trying to access a glyph outside of the limits");
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

std::string DrawText::format(const std::string text, ...)
{
    char buffer[BUFFER_SIZE] = {};
    {
        va_list list;
        va_start(list, text);
        vsnprintf(buffer, BUFFER_SIZE, text.c_str(), list);
        va_end(list);
    };

    return {buffer};
}

std::wstring DrawText::format(const std::wstring text, ...)
{
    wchar_t buffer[BUFFER_SIZE] = {};
    {
        va_list list;
        va_start(list, text);
        vswprintf(buffer, BUFFER_SIZE, text.c_str(), list);
        va_end(list);
    };
    return {buffer};
}