#include "drawtext.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

DrawText::DrawText(const char* fontPath, const int fontSize, const SDL_Color& fontColor,
                   uint16_t initialCharacter, uint16_t finalCharacter, bool throwExceptions)
    : initialCharacter_{initialCharacter}, finalCharacter_{finalCharacter}, throwExceptions_{
                                                                                throwExceptions}
{
    if (finalCharacter_ - initialCharacter_ <= 0) {
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
    for (std::map<uint16_t, SDL_Surface*>::iterator it = alphabet_.begin(); it != alphabet_.end();
         it++) {
        SDL_FreeSurface(it->second);
    }
}

void DrawText::createAlphabet(TTF_Font* font, const SDL_Color& fontColor)
{
    for (uint16_t c = initialCharacter_; c <= finalCharacter_; c++) {
        auto* glyph = TTF_RenderGlyph_Blended(font, c, fontColor);
        if (glyph == nullptr) {
            throw std::runtime_error("Error creating alphabet");
        }
        alphabet_[c] = glyph;
    }
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
            std::stringstream out;
            out << "Out of range exc. Trying to get element " << std::to_string(c)
                << "  from alphabet with limits [" << std::to_string(initialCharacter_) << ";"
                << std::to_string(finalCharacter_) << "]." << static_cast<char>(c) << std::endl;

            if (throwExceptions_) {
                throw e;
            }
            std::cout << out.str();
        }
        return nullptr;
    };

    // New line
    if (character == static_cast<uint16_t>('\n')) {
        auto* glyph = get_glyph_of_character(initialCharacter_);
        if (glyph == nullptr) {
            return;
        }
        y = y + glyph->h;
        newLine_ = true;
        return;
    }

    // Normal character
    auto* glyph = get_glyph_of_character(character);
    if (glyph == nullptr) {
        return;
    }

    auto calculate_increment = [](int x0, int w0, int x1, int w1) -> int {
        int dx = 0;
        if (w0 == 0) {
            dx = w1;
        } else {
            dx = (x0 + w0) > (x1 + w1) ? w1 : w1 - (x1 + w1 - x0 - w0);
        }
        return dx;
    };

    int dx = calculate_increment(constrain.x0, constrain.width, x, glyph->w);
    int dy = calculate_increment(constrain.y0, constrain.height, y, glyph->h);

    // if (constrain.width == 0) {
    //    dx = glyph->w;
    //} else {
    //    dx = (constrain.x0 + constrain.width) > (x + glyph->w)
    //             ? glyph->w
    //             : glyph->w - (x + glyph->w - constrain.x0 - constrain.width);
    //}

    // if (constrain.height == 0) {
    //    dy = glyph->h;
    //} else {
    //    dy = (constrain.y0 + constrain.height) > (y + glyph->h)
    //             ? glyph->h
    //             : glyph->h - (y + glyph->h - constrain.y0 - constrain.height);
    //}

    SDL_Rect src_rect = {0, 0, dx, dy};
    SDL_Rect dst_rect = {x, y, dx, dy};
    x = x + glyph->w;
    SDL_BlitSurface(glyph, &src_rect, destinationSurface, &dst_rect);
}