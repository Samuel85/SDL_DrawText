#include "drawtext.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>

DrawText::DrawText(const char *fontPath, const int fontSize,
                   const SDL_Color &fontColor) {
  TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
  if (!font) {
    std::stringstream message;
    message << "DrawText::DrawText: " << TTF_GetError();
    throw std::runtime_error(message.str());
  }
  createAlphabet(font, fontColor);
  TTF_CloseFont(font);
}

DrawText::~DrawText() {
  for (auto &i : alphabet) {
    SDL_FreeSurface(i);
  }
}

void DrawText::createAlphabet(TTF_Font *font, const SDL_Color &fontColor) {
  for (Uint16 c = INITIAL_CHARACTER; c <= FINAL_CHARACTER; c++) {
    alphabet[c - INITIAL_CHARACTER] =
        TTF_RenderGlyph_Blended(font, c, fontColor);
  }
}

void DrawText::drawGlyph(SDL_Surface *destinationSurface,
                         const unsigned char character, int &x, int &y) {
  if (character == '\n') {
    SDL_Surface *glyph = alphabet[0];
    y = y + glyph->h;
    newLine = true;
    return;
  }

  unsigned char i = character - INITIAL_CHARACTER;
  SDL_Surface *glyph = alphabet[i];

  SDL_Rect dst_rect;
  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = glyph->w;
  dst_rect.h = glyph->h;

  x = x + glyph->w;
  SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}

void DrawText::print(SDL_Surface *destinationSurface, const char *string, int x,
                     int y, ...) {
  auto tmp_x = x;
  auto tmp_y = y;

  char buffer[BUFFER_SIZE] = {};
  {
    va_list list;
    va_start(list, y);
    vsnprintf(buffer, BUFFER_SIZE, string, list);
    va_end(list);
  };

  for (char *c = buffer; *c != '\0'; c++) {
    if (newLine) {
      tmp_x = x;
      newLine = false;
    }
    drawGlyph(destinationSurface, *c, tmp_x, tmp_y);
  }
}

void DrawText::drawGlyphW(SDL_Surface *destinationSurface,
                          const wchar_t character, int &x, int &y) {
  if (character == '\n') {
    SDL_Surface *glyph = alphabet[0];
    y = y + glyph->h;
    newLine = true;
    return;
  }
  unsigned char i = (unsigned char)character - INITIAL_CHARACTER;
  if (i >= FINAL_CHARACTER) {
    return;
  }
  SDL_Surface *glyph = alphabet[i];

  SDL_Rect dst_rect;
  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = glyph->w;
  dst_rect.h = glyph->h;

  x = x + glyph->w;
  SDL_BlitSurface(glyph, NULL, destinationSurface, &dst_rect);
}

void DrawText::print(SDL_Surface *destinationSurface, const wchar_t *string,
                     int x, int y, ...) {
  auto tmp_x = x;
  auto tmp_y = y;

  wchar_t buffer[BUFFER_SIZE] = {};
  {
    va_list list;
    va_start(list, y);
    vswprintf(buffer, BUFFER_SIZE, string, list);
    va_end(list);
  }

  for (wchar_t *character = buffer; *character != '\0'; character++) {
    if (newLine) {
      tmp_x = x;
      newLine = false;
    }
    drawGlyphW(destinationSurface, *character, tmp_x, tmp_y);
  }
}
