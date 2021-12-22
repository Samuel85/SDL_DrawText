#ifndef _DRAWTEXT_H_
#define _DRAWTEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class DrawText {
public:
  DrawText(const char *fontPath, int fontSize, const SDL_Color &fontColor);
  ~DrawText();

  void print(SDL_Surface *destinationSurface, const char *string, int x, int y,
             ...);
  void print(SDL_Surface *destinationSurface, const wchar_t *string, int x,
             int y, ...);

private:
  void createAlphabet(TTF_Font *font, const SDL_Color &fontColor);
  void drawGlyph(SDL_Surface *destinationSurface, unsigned char character,
                 int &x, int &y);
  void drawGlyphW(SDL_Surface *destinationSurface, wchar_t character, int &x,
                  int &y);

  static constexpr int BUFFER_SIZE = 256;
  static constexpr int INITIAL_CHARACTER = 32;
  static constexpr int FINAL_CHARACTER = 255;
  static constexpr int TOTAL_CHARACTERS = FINAL_CHARACTER - INITIAL_CHARACTER;

  SDL_Surface *alphabet[TOTAL_CHARACTERS]{};
  bool newLine{false};
};

#endif
