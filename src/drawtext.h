#ifndef _DRAWTEXT_H_
#define _DRAWTEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <cwchar>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

class DrawText
{
  public:
    /**
     * @brief Defines the properties of the text to draw on the screen.
     * @param fontPath Path to the TTF file to be used.
     * @param fontSize Size of the font to write on screen.
     * @param fontColor Color of the text to write.
     */
    DrawText(const char* fontPath, int fontSize, const SDL_Color& fontColor,
             uint16_t initialCharacter = 5, uint16_t finalCharacter = 255,
             bool throwExceptions = false);

    /**
     * @brief Destroys the alphabet and free memory.
     */
    virtual ~DrawText();

    /**
     * @brief Print text on a given surface
     * @param destinationSurface Surface where the text will be written.
     * @param text Unicode text to write on the screen.
     * @param x Horizontal position of the text.
     * @param y Vertical position of the text.
     * @param width Sets a maximum width for the text. 0 for no limitations.
     * @param height Sets a maxium height for the text. 0 for no limitations.
     * @throw Out of range if the string contains characters not contained in the alphabet
     */
    template <typename T, typename = std::enable_if<std::is_integral<T>::value, std::string>,
              typename = std::enable_if<std::is_integral<T>::value, std::wstring>>
    void print(SDL_Surface* destinationSurface, T const& text, int x, int y, int width = 0,
               int height = 0)
    {
        auto tmp_x = x;
        auto tmp_y = y;
        for (auto c = std::begin(text); c != std::end(text) && *c != '\0'; ++c) {
            if (newLine_) {
                tmp_x = x;
                newLine_ = false;
            }
            drawGlyph(destinationSurface, *c, tmp_x, tmp_y, Constrain{x, y, width, height});
        }
    }

    /**
     * @brief Formats a string
     * @param text The text or format specifier to convert
     * @param ... Extra parameters to replace by the format specifier
     * @return Resulting string after applying the format specifier
     * @throw Fails if the resulting string has a size of zero
     */
    template <typename... Args> static std::string format(const std::string& text, Args&&... args)
    {
        size_t n = std::snprintf(nullptr, 0, text.c_str(), std::forward<Args>(args)...);
        if (n < 0) {
            throw std::runtime_error("The formated string has a size of zero.");
        }
        if (n == 0) {
            return {};
        }
        auto buffer = new char[n+1];
        std::snprintf(buffer, n, text.c_str(), std::forward<Args>(args)...);
        std::string result = buffer;
        delete[] buffer;
        return result;
    }
    template <typename... Args> static std::wstring format(const std::wstring& text, Args&&... args)
    {
        int n = std::swprintf(nullptr, 0, text.c_str(), std::forward<Args>(args)...);
        if (n < 0) {
            throw std::runtime_error("The formated string has a size of zero.");
        }
        if (n == 0) {
            return {};
        }
        auto buffer = new wchar_t[n+1];
        swprintf(buffer, n, text.c_str(), std::forward<Args>(args)...);
        std::wstring result = buffer;
        delete[] buffer;
        return result;
    }

  private:
    struct Constrain {
        int x0{0};
        int y0{0};
        int width{0};
        int height{0};
    };
    /**
     * @brief Creates the glyphs that composes the alphabet.
     * @param font Pointer to the TTF_Font that contains a valid font.
     * @param fontColor Color for the glyphs.
     */
    void createAlphabet(TTF_Font* font, const SDL_Color& fontColor, uint16_t initialCharacter,
                        uint16_t finalCharacter);

    /**
     * @brief Draw a glyph on the screen
     * @param destinationSurface Surface where the character will be written.
     * @param character Unicode character to write on the screen.
     * @param x Horizontal position for the glyph.
     * @param y Vertical position for the glyph.
     * @param Constrain sets the drawing limitations.
     *
     * If the width or height in the constrain struct are zero the dimension is ignored.
     */
    void drawGlyph(SDL_Surface* destinationSurface, uint16_t character, int& x, int& y,
                   Constrain constrain = Constrain{0, 0, 0, 0});

    std::map<uint16_t, SDL_Surface*> alphabet_;

    bool newLine_{false};
    bool throwExceptions_{false};
};

#endif
