#ifndef SDL_BUTTON_H
#define	SDL_BUTTON_H

#include "sdl_texture.h"
#include "sdl_defines.h"

#include <vector>

// ### TODO: Replace position/w/h with SDL rectangle?? ######
class sdl_button
{
public:
	// Initializes internal variables
	sdl_button(size_t width, size_t height, std::vector<sdl_texture>& button_textures);

	//Sets top left position
	void setPosition( int x, int y );

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

	size_t get_width() const { return _w; };
	size_t get_height() const { return _h; };

private:
	//Top left position
	SDL_Point _position;

	size_t _w;
	size_t _h;

	//Currently used global sprite
	button_sprite_t _current_sprite;
	
	std::vector<sdl_texture>& _button_textures;
};

#endif	/* SDL_BUTTON_H */
