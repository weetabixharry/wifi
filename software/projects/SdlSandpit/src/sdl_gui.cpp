#include "sdl_gui.h"
#include <iostream>

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 50;
const int TOTAL_BUTTONS = 4;


const std::string button_labels[4] = {"AWAY", "OVER", "DOWN", "UP"};
const SDL_Color textColors[4] = {{ 0, 0, 0 }, { 0, 255, 0 }, { 255, 0, 0 }, { 0, 0, 255 }};

sdl_gui::sdl_gui(size_t window_width, size_t window_height) :
	_w(window_width),
	_h(window_height),
	_window(NULL),
	_renderer(NULL),
	_init(init()),
	_button_textures(4, sdl_texture("C:/Users/Harry/Desktop/Calibrib.ttf", 36, _renderer, BUTTON_WIDTH, BUTTON_HEIGHT)),
	_buttons(TOTAL_BUTTONS, sdl_button(BUTTON_WIDTH, BUTTON_HEIGHT, _button_textures))
{	
	if (!_init)
		std::cout << __FUNCTION__ << " : Error! Failed to initialise." << std::endl;
	else
	{
		for (int i=0; i<_button_textures.size(); i++)
			if(!_button_textures[i].loadFromRenderedText(button_labels[i], textColors[i]))
				std::cout << __FUNCTION__ << " : Error! Failed to render text texture." << std::endl; 
		
		// Set buttons in corners
		_buttons[0].setPosition(0, 0);
		_buttons[1].setPosition(_w - _buttons[1].get_width(), 0);
		_buttons[2].setPosition(0, _h - _buttons[2].get_height());
		_buttons[3].setPosition(_w - _buttons[3].get_width(), _h - _buttons[3].get_height());
		
		run();
	}
}

bool sdl_gui::init()
{
	bool success = true;
	
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
		{
			std::cout <<  "Warning: Linear texture filtering not enabled!" << std::endl;
		}

		//Create window
		_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _w, _h, SDL_WINDOW_SHOWN);
		if(_window == NULL)
		{
			std::cout <<  "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			// Create vsynced renderer for window
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(_renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init(imgFlags) & imgFlags ) )
				{
					std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
					success = false;
				}
				
				 //Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
				{
					std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
					success = false;
				}
				
				 //Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: %s\n" << TTF_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	
	return success;
}

sdl_gui::~sdl_gui()
{
	// Destroy window
	if (_renderer != NULL)
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
	}
	if (_window != NULL)
	{
		SDL_DestroyWindow(_window);
		_window = NULL;
	}

	// Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void sdl_gui::run()
{	
	// Main loop flag
	bool quit = false;

	// Event handler
	SDL_Event e;

	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
				quit = true;
			else
			{
				//Handle button events
				for (int i=0; i<TOTAL_BUTTONS; i++)
					_buttons[i].handleEvent(&e);
				
				_audio.handle_event(e);
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor(_renderer, 0x80, 0x80, 0x80, 0xFF);
		SDL_RenderClear(_renderer);

		// Render buttons
		for (int i=0; i<_buttons.size(); i++)
			_buttons[i].render();

		// Update screen
		SDL_RenderPresent(_renderer);
	}
}
