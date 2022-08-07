#ifndef _DRAWTEXT_H_
#define _DRAWTEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <array>

class DrawText {
public:
	/**
	 * @brief Defines the properties of the text to draw on the screen.
	 * @param fontPath Path to the TTF file to be used.
	 * @param fontSize Size of the font to write on screen.
	 * @param fontColor Color of the text to write.
	 */
	DrawText(const char* fontPath, int fontSize, const SDL_Color& fontColor, int initialCharacter = 32, int finalCharacter = 255);

	/**
	 * @brief Destroys the alphabet and free memory.
	 */
	~DrawText();

	/**
	 * @brief Print text on a given surface
	 * @param destinationSurface Surface where the text will be written.
	 * @param text Text to write on the screen.
	 * @param x Horizontal position of the text.
	 * @param y Vertical position of the text.
	 * @param ... Optional variables for the special characters in the string.
	 */
	void print(SDL_Surface* destinationSurface, const std::string& text, int x, int y);

	/**
	 * @brief Print text on a given surface
	 * @param destinationSurface Surface where the text will be written.
	 * @param text Unicode text to write on the screen.
	 * @param x Horizontal position of the text.
	 * @param y Vertical position of the text.
	 */
	void print(SDL_Surface* destinationSurface, const std::wstring& text, int x, int y);

	/**
	 * @brief Formats a string
	 * @param text The text or format specifier to convert
	 * @param ... Extra parameters to replace by the format specifier
	 * @return Resulting string after applying the format specifier
	 */
	static std::string format(const std::string text, ...);

	/**
	 * @brief Formats a string
	 * @param text The text or format specifier to convert
	 * @param ... Extra parameters to replace by the format specifier
	 * @return Resulting string after applying the format specifier
	 */
	static std::wstring format(const std::wstring string, ...);
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
	 * @param character Character to write on the screen.
	 * @param x Horizontal position for the glyph.
	 * @param y Vertical position for the glyph.
	 */
	void drawGlyph(SDL_Surface* destinationSurface, unsigned char character,
		int& x, int& y);
	/**
	 * @brief Draw a glyph on the screen
	 * @param destinationSurface Surface where the character will be written.
	 * @param character Unicode character to write on the screen.
	 * @param x Horizontal position for the glyph.
	 * @param y Vertical position for the glyph.
	 */
	void drawGlyphW(SDL_Surface* destinationSurface, wchar_t character, int& x,
		int& y);


	/// Initial character for the alphabet.
	int initialCharacter_;
	/// Final character for the alphabet.
	int finalCharacter_;
	///  Total number of characters in alphabet.  
	size_t totalCharacters_;

	SDL_Surface** alphabet;

	bool newLine_{ false };
};

#endif
