#include "drawtext.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

DrawText::DrawText(const char* fontPath, int fontSize, const SDL_Color& fontColor,
                   uint16_t initialCharacter, uint16_t finalCharacter, bool throwExceptions)
    : throwExceptions_{throwExceptions}
{
    if (finalCharacter - initialCharacter <= 0) {
        throw std::logic_error("The final character must be bigger than the initial character.");
    }

    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        std::stringstream message;
        message << "DrawText::DrawText: " << TTF_GetError();
        throw std::runtime_error(message.str());
    }
    createAlphabet(font, fontColor, initialCharacter, finalCharacter);
    TTF_CloseFont(font);
}

DrawText::~DrawText()
{
    for (std::map<uint16_t, SDL_Surface*>::iterator it = alphabet_.begin(); it != alphabet_.end();
         it++) {
        SDL_FreeSurface(it->second);
    }
}

void DrawText::createAlphabet(TTF_Font* font, const SDL_Color& fontColor, uint16_t initialCharacter,
                              uint16_t finalCharacter)
{
    for (uint16_t c = initialCharacter; c <= finalCharacter; c++) {
        auto* glyph = TTF_RenderGlyph_Blended(font, c, fontColor);
        if (glyph == nullptr) {
            throw std::runtime_error("Error creating alphabet");
        }
        alphabet_[c] = glyph;
    }
}

static int calculateIncrement(int x0, int w0, int x1, int w1)
{
    int dx = 0;
    if (w0 == 0) {
        dx = w1;
    } else {
        dx = (x0 + w0) > (x1 + w1) ? w1 : w1 - (x1 + w1 - x0 - w0);
    }
    return dx;
}

void DrawText::drawGlyph(SDL_Surface* destinationSurface, uint16_t character, int& x, int& y,
                         Constrain constrain)
{
    if (y > (constrain.x0 + constrain.height) && constrain.height != 0) {
        return;
    }
    auto get_glyph_of_character = [&](uint16_t c) -> SDL_Surface* {
        try {
            auto* glyph = alphabet_.at(c);
            return glyph;
        } catch (std::out_of_range& e) {
            if (throwExceptions_) {
                throw std::logic_error("Trying to print character not defined in alphabet.");
            }
        }
        return nullptr;
    };

    // New line
    if (character == static_cast<uint16_t>('\n')) {
        auto* glyph = get_glyph_of_character(alphabet_.begin()->first);
        y = y + glyph->h;
        newLine_ = true;
        return;
    }

    // Normal character
    auto* glyph = get_glyph_of_character(character);
    if (glyph == nullptr) {
        return;
    }

    const int dx = calculateIncrement(constrain.x0, constrain.width, x, glyph->w);
    const int dy = calculateIncrement(constrain.y0, constrain.height, y, glyph->h);
    SDL_Rect src_rect = {0, 0, dx, dy};
    SDL_Rect dst_rect = {x, y, dx, dy};
    x = x + glyph->w;
    SDL_BlitSurface(glyph, &src_rect, destinationSurface, &dst_rect);
}