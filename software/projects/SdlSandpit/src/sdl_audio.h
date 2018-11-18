#ifndef SDL_AUDIO_H
#define	SDL_AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

class sdl_audio
{
public:
	sdl_audio();
	~sdl_audio();
	
	void handle_event(const SDL_Event& e);
	
private:
	Mix_Music* _music;
	Mix_Chunk* _sound;
};

#endif	/* SDL_AUDIO_H */
