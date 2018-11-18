#ifndef SDL_GUI_H
#define	SDL_GUI_H

#include "sdl_texture.h"
#include "sdl_button.h"

#include <cstdlib>
#include <vector>

class sdl_gui
{
public:
	sdl_gui(size_t window_width, size_t window_height);
	~sdl_gui();
	
	size_t get_width() const { return _w; };
	size_t get_height() const { return _h; };
	
private:
	bool init();
	void run();
	
	size_t _w;
	size_t _h;
	
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	
	bool _init;
	
	std::vector<sdl_texture> _button_textures;
	std::vector<sdl_button> _buttons;
};

#endif	/* SDL_GUI_H */
