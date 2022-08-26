#ifndef _DRAWTEXT_H_
#define _DRAWTEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>

#include <wchar.h>

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
             uint16_t initialCharacter = 32, uint16_t finalCharacter = 255);

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
     */
    // template <typename T> void print(SDL_Surface* destinationSurface, T const& text, int x, int
    // y)
    //{
    //    auto process_elements = [&](auto& temp_string) -> void {
    //        auto tmp_x = x;
    //        auto tmp_y = y;
    //        for (auto c = std::begin(temp_string); c != std::end(temp_string) && *c != '\0'; c++)
    //        {
    //            if (newLine_) {
    //                tmp_x = x;
    //                newLine_ = false;
    //            }
    //            drawGlyph(destinationSurface, static_cast<unsigned char>(*c), tmp_x, tmp_y);
    //        }
    //    };

    //    if (std::is_convertible<T, std::string>()) {
    //        process_elements(text);
    //    } else if (std::is_convertible<T, std::wstring>()) {
    //        process_elements(text);
    //    }
    //}

    template <typename T, typename = std::enable_if<std::is_integral<T>::value, std::string>,
              typename = std::enable_if<std::is_integral<T>::value, std::wstring>>
    void print(SDL_Surface* destinationSurface, T const& text, int x, int y)
    {
        auto tmp_x = x;
        auto tmp_y = y;
        for (auto c = std::begin(text); c != std::end(text) && *c != '\0'; c++) {
            if (newLine_) {
                tmp_x = x;
                newLine_ = false;
            }
            drawGlyph(destinationSurface, static_cast<unsigned char>(*c), tmp_x, tmp_y);
        }
    }

    /**
     * @brief Formats a string
     * @param text The text or format specifier to convert
     * @param ... Extra parameters to replace by the format specifier
     * @return Resulting string after applying the format specifier
     */
    // static std::string format(const std::string text, ...);

    /**
     * @brief Formats a string
     * @param text The text or format specifier to convert
     * @param ... Extra parameters to replace by the format specifier
     * @return Resulting string after applying the format specifier
     */
    // static std::wstring format(const std::wstring string, ...);

    template <typename... Args> static std::string format(const std::string& text, Args&&... args)
    {
        size_t n = std::snprintf(nullptr, 0, text.c_str(), std::forward<Args>(args)...) + 1;
        auto buffer = new char[n];
        std::snprintf(buffer, n, text.c_str(), std::forward<Args>(args)...);
        std::string result = buffer;
        delete[] buffer;
        return result;
    }
    template <typename... Args> static std::wstring format(const std::wstring& text, Args&&... args)
    {
        int n = swprintf(nullptr, 0, text.c_str(), std::forward<Args>(args)...) + 1;
        auto buffer = new wchar_t[n];
        swprintf(buffer, n, text.c_str(), std::forward<Args>(args)...);
        std::wstring result = buffer;
        delete[] buffer;
        return result;
    }

  private:
    static constexpr size_t BUFFER_SIZE = 255;
    /**
     * @brief Creates the glyphs that composes the alphabet.
     * @param font Pointer to the TTF_Font that contains a valid font.
     * @param fontColor Color for the glyphs.
     */
    void createAlphabet(TTF_Font* font, const SDL_Color& fontColor);

    /**
     * @brief Draw a glyph on the screen
     * @param destinationSurface Surface where the character will be written.
     * @param character Unicode character to write on the screen.
     * @param x Horizontal position for the glyph.
     * @param y Vertical position for the glyph.
     */
    void drawGlyph(SDL_Surface* destinationSurface, uint16_t character, int& x, int& y);

    /// Initial character for the alphabet.
    uint16_t initialCharacter_;
    /// Final character for the alphabet.
    uint16_t finalCharacter_;
    ///  Total number of characters in alphabet.
    size_t totalCharacters_;

    SDL_Surface** alphabet_;

    bool newLine_{false};
};

#endif
