#pragma once
#include <map>
#include <SDL_mixer.h>
#include "ResourceManager.h"
class AudioManager
{
public:
	static void Init(Utilities::ResourceManager& content);
	static void PlaySound(const std::string& path, const float& volume = 1);
	static void PlayMusic(const std::string& path, const float& volume = 1, bool loop = true);
	static void StopMusic();
	static void PauseMusic();
	static void Close();
private:
	AudioManager();
	enum AudioChannels
	{
		FREE_CHANNEL = -1,
		BASIC_SOUNDS = 0
	};
	static Utilities::ResourceManager* content;
};