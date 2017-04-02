#include "AudioManager.h"
#include <SDL_mixer.h>
#include <cstdio>
#include <SDL.h>
#include <glm/glm.hpp>

Utilities::ResourceManager* AudioManager::content;

AudioManager::AudioManager()
{

}

void AudioManager::init(Utilities::ResourceManager& contentManager)
{
	// start SDL with audio support
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	// open 44.1KHz, signed 16bit, system byte order,
	// stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	content = &contentManager;
}

void AudioManager::playSound(const std::string& path, const float& volume)
{
	glm::clamp(volume, 0.0f, 1.0f);
	Mix_Volume(FREE_CHANNEL, volume * MIX_MAX_VOLUME);
	auto chunk = content->loadSound(path);
	if (chunk != nullptr)
	{
		Mix_PlayChannel(FREE_CHANNEL, chunk, 0);

	}
	
}

void AudioManager::playMusic(const std::string& path, const float& volume, bool loop)
{
	auto music = content->loadMusic(path);
	if (music != nullptr)
	{
		auto newVolume = glm::clamp(volume, 0.0f, 1.0f);

		Mix_VolumeMusic(newVolume * MIX_MAX_VOLUME);

		int loops;
		if (loop) {
			loops = -1;
		}
		else {
			loops = 0;
		}


		//if there is no music playing
		if (Mix_PlayingMusic() == 0)
		{
			//play the music
			Mix_PlayMusic(music, loops);
		}
		//if music is being played
		else
		{
			pauseMusic();
		}
	}

}

void AudioManager::stopMusic()
{	
	Mix_HaltMusic();
}

void AudioManager::pauseMusic()
{
	//if the music is paused
	if (Mix_PausedMusic() == 1)
	{
		//resume the music
		Mix_ResumeMusic();
	}
	//if the music is playing
	else
	{
		//pause the music
		Mix_PauseMusic();
	}
}

void AudioManager::close()
{
	
}
