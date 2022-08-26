#include "drawtext.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

DrawText::DrawText(const char* fontPath, const int fontSize, const SDL_Color& fontColor,
                   uint16_t initialCharacter, uint16_t finalCharacter)
    : initialCharacter_{initialCharacter}, finalCharacter_{finalCharacter}
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
    for (auto c = initialCharacter_; c <= finalCharacter_; c++) {
        auto glyph = TTF_RenderGlyph_Blended(font, c, fontColor);
        if (glyph == NULL) {
            throw std::runtime_error("Error creating alphabet");
        }
        alphabet_[c] = glyph;
    }
}

void DrawText::drawGlyph(SDL_Surface* destinationSurface, uint16_t character, int& x, int& y)
{
    auto get_glyph_of_character = [&](uint16_t c) -> SDL_Surface* {
        try {
            auto* glyph = alphabet_.at(c);
            return glyph;
        } catch (std::out_of_range& e) {
            std::stringstream out;
            out << "Exception trying to get element " << std::to_string(c)
                << "  from alphabet with limits [" << std::to_string(initialCharacter_) << ";"
                << std::to_string(finalCharacter_) << "]." << static_cast<char>(c)
                << std::endl;

            std::cout << out.str();
        }
        return nullptr;
    };
    if (character == static_cast<uint16_t>('\n')) {
        auto* glyph = get_glyph_of_character(initialCharacter_);
        if (glyph == nullptr) {
            return;
        }
        y = y + glyph->h;
        newLine_ = true;
        return;
    }

    auto* glyph = get_glyph_of_character(character);
    if (glyph == nullptr) {
        return;
    }
    SDL_Rect dst_rect = {x, y, glyph->w, glyph->h};
    x = x + glyph->w;
    SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}