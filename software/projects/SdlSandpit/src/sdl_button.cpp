#include "sdl_button.h"

sdl_button::sdl_button(size_t width, size_t height, std::vector<sdl_texture>& button_textures) :
	_w(width),
	_h(height),
	_button_textures(button_textures)
{
	_position.x = 0;
	_position.y = 0;

	_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
}

void sdl_button::setPosition(int x, int y)
{
	_position.x = x;
	_position.y = y;
}

void sdl_button::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < _position.x )
		{
			inside = false;
		}
		// Mouse is right of the button
		else if(x > _position.x + _w)
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < _position.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > _position.y + _h)
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				_current_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				_current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				_current_sprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}
	
void sdl_button::render()
{
	//Show current button sprite
	_button_textures[_current_sprite].render(_position.x, _position.y);
}
