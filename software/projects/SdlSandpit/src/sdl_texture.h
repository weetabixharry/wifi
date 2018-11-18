#ifndef SDL_TEXTURE_H
#define	SDL_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class sdl_texture
{
public:
	sdl_texture(const char* font_path, int font_size,
				SDL_Renderer* renderer, int render_width, int render_height);

	//Deallocates memory
	~sdl_texture();

	//Creates image from font string
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor( Uint8 red, Uint8 green, Uint8 blue );

	//Set blending
	void setBlendMode( SDL_BlendMode blending );

	//Set alpha modulation
	void setAlpha( Uint8 alpha );

	//Renders texture at given point
	void render(int x, int y, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int get_width() const { return _w; };
	int get_height() const { return _h; };

private:
	
	// The actual hardware texture (managed by this class)
	SDL_Texture* _texture;
	// Font (managed by this class)
	TTF_Font* _font;
	
	// Font parameters
	std::string _font_path;
	size_t _font_size;

	// Raw image dimensions
	int _w;
	int _h;

	// Rendered dimensions
	int _render_width;
	int _render_height;

	// Externally owned/managed
	SDL_Renderer* _renderer;
};

#endif	/* SDL_TEXTURE_H */
