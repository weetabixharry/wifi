#include "sdl_texture.h"
#include <cstdlib>
#include <iostream>
#include <string>

sdl_texture::sdl_texture(const char* font_path, int font_size,
						SDL_Renderer* renderer, int render_width, int render_height) :
	_texture(NULL),
	_w(0),
	_h(0),
	_font(NULL),
	_renderer(renderer),
	_render_width(render_width),
	_render_height(render_height),
	_font_path(font_path),
	_font_size(font_size)
{
	// Note: Don't allocate any memory in constructor. This way, we can get away
	// with shallow copies, on the understanding that _texture and _font must
	// be allocated after object is constructed.
}

sdl_texture::~sdl_texture()
{
	// Deallocate texture
	free();
	
	// Close font
	if (_font)
	{
		TTF_CloseFont(_font);
		_font = NULL;
	}
}

bool sdl_texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	if (_font == NULL)
		_font = TTF_OpenFont(_font_path.c_str(), _font_size);
	
	//Get rid of preexisting texture
	free();
	
	if(_font == NULL)
		std::cout << __FUNCTION__ << " : Error! Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
	
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(_font, textureText.c_str(), textColor);
	if(textSurface == NULL)
		std::cout << "Error! Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << std::endl;
	else
	{
		// Create texture from surface pixels
        _texture = SDL_CreateTextureFromSurface(_renderer, textSurface);
		if( _texture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			_w = textSurface->w;
			_h = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	
	//Return success
	return _texture != NULL;
}

void sdl_texture::free()
{
	//Free texture if it exists
	if(_texture != NULL)
	{
		SDL_DestroyTexture( _texture );
		_texture = NULL;
		_w = 0;
		_h = 0;
	}
}

void sdl_texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( _texture, red, green, blue );
}

void sdl_texture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( _texture, blending );
}
		
void sdl_texture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( _texture, alpha );
}

void sdl_texture::render(int x, int y, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, _render_width, _render_height};
	
	//Render to screen
	SDL_RenderCopyEx(_renderer, _texture, NULL, &renderQuad, angle, center, flip);
}
