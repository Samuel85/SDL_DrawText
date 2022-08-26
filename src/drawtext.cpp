#include "drawtext.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

DrawText::DrawText(const char* fontPath, const int fontSize, const SDL_Color& fontColor,
                   uint16_t initialCharacter, uint16_t finalCharacter)
    : initialCharacter_{initialCharacter}, finalCharacter_{finalCharacter},
      totalCharacters_{static_cast<size_t>(finalCharacter_ - initialCharacter_)},
      alphabet_{new SDL_Surface*[totalCharacters_]}
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
    delete[] alphabet_;
}

void DrawText::createAlphabet(TTF_Font* font, const SDL_Color& fontColor)
{
    for (auto c = initialCharacter_; c < finalCharacter_; c++) {
        auto glyph = TTF_RenderGlyph_Blended(font, c, fontColor);
        if (glyph == NULL) {
            throw std::runtime_error("Error creating alphabet");
        }
        alphabet_[c - initialCharacter_] = glyph;
    }
}

void DrawText::drawGlyph(SDL_Surface* destinationSurface, uint16_t character, int& x, int& y)
{
    if (character == '\n') {
        auto* glyph = alphabet_[0];
        y = y + glyph->h;
        newLine_ = true;
        return;
    }
    auto i = character - initialCharacter_;
    if (i >= totalCharacters_) {
        throw std::logic_error("Trying to access a glyph outside of the limits");
    }
    auto* glyph = alphabet_[i];
    SDL_Rect dst_rect = {x, y, glyph->w, glyph->h};

    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}