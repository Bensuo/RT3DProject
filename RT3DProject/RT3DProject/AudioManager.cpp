#include "AudioManager.h"
#include <SDL_mixer.h>
#include <cstdio>
#include <SDL.h>
#include <glm.hpp>

AudioManager::AudioManager()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0) // Initialize video
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void AudioManager::PlaySound(const std::string& path, const float& volume)
{
	glm::clamp(volume, 0.0f, 1.0f);
	Mix_Volume(BASIC_SOUNDS, volume * MIX_MAX_VOLUME);

	auto iterator = sounds.find(path);

	//if the sound has been loaded, play it
	if(iterator != sounds.end())
	{
		Mix_PlayChannel(BASIC_SOUNDS, sounds[path], 0);
	}
	//if the sound hasn't been loaded, load it and play it
	else
	{
		auto newMusic = Mix_LoadWAV(path.c_str());
		if (newMusic == nullptr) {
			printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			printf(path.c_str(), "\n");
			printf("\n");
		}
		else {
			sounds.emplace(path, newMusic);
			Mix_PlayChannel(BASIC_SOUNDS, sounds[path], 0);
		}
	}
}

void AudioManager::PlayMusic(const std::string& path, const float& volume, bool loop)
{
	glm::clamp(volume, 0.0f, 1.0f);

	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);

	auto iterator = music.find(path);

	int loops;
	if(loop) {
		loops = -1;
	} else {
		loops = 0;
	}

	//if the music has been loaded, play it
	if (iterator != music.end())
	{
		//if there is no music playing
		if (Mix_PlayingMusic() == 0)
		{
			//play the music
			Mix_PlayMusic(music[path], loops);
		}
		//if music is being played
		else
		{
			PauseMusic();
		}
	}
	//if the music hasn't been loaded, load it an play it
	else
	{
		auto newMusic = Mix_LoadMUS(path.c_str());
		if (newMusic == nullptr) {
			printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
			printf(path.c_str(), "\n");
			printf("\n");
		} else {
			music.emplace(path, newMusic);
			Mix_PlayMusic(music[path], loops);
		}
	}
}

void AudioManager::StopMusic() const
{	
	Mix_HaltMusic();
}

void AudioManager::PauseMusic() const
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

void AudioManager::Close()
{
	//free the music
	for (auto it = music.begin(); it != music.end(); ++it)
	{
		Mix_FreeMusic(it->second);
	}
	music.clear();

	//free the sounds
	for (auto it = sounds.begin(); it != sounds.end(); ++it)
	{
		Mix_FreeChunk(it->second);
	}
	sounds.clear();
}
