#include "sdl_audio.h"

#include <iostream>

sdl_audio::sdl_audio() : 
	_music(NULL),
	_sound(NULL)
{
	// Load music
	_music = Mix_LoadMUS("21_sound_effects_and_music/beat.wav");
	if (_music == NULL)
		std::cout << "Error! Failed to load music. SDL_mixer Error: " << Mix_GetError() << std::endl;
	
	// Load sound
	_sound = Mix_LoadWAV( "21_sound_effects_and_music/scratch.wav" );
	if(_sound == NULL)
		std::cout << "Error! Failed to load sound. SDL_mixer Error: " << Mix_GetError() << std::endl;

}

sdl_audio::~sdl_audio()
{
	// Free the sound effects
	Mix_FreeChunk(_sound);
	_sound = NULL;
	
	// Free the music
	Mix_FreeMusic(_music);
	_music = NULL;
}

void sdl_audio::handle_event(const SDL_Event& e)
{
	//Handle key press
	if(e.type == SDL_KEYDOWN)
	{
		switch(e.key.keysym.sym)
		{
			// Play sound effect
			case SDLK_1:
			Mix_PlayChannel(-1, _sound, 0);
			break;

			case SDLK_9:
			// If there is no music playing
			if(Mix_PlayingMusic() == 0)
			{
				//Play the music
				Mix_PlayMusic(_music, -1);
			}
			// If music is being played
			else
			{
				// If the music is paused
				if(Mix_PausedMusic() == 1)
				{
					// Resume the music
					Mix_ResumeMusic();
				}
				// If the music is playing
				else
				{
					//Pause the music
					Mix_PauseMusic();
				}
			}
			break;

			case SDLK_0:
			// Stop the music
			Mix_HaltMusic();
			break;
		}
	}
}
