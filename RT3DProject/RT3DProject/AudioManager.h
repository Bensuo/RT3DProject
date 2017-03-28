#pragma once
#include <map>
#include <SDL_mixer.h>

class AudioManager
{
public:
	static void Init();
	static void PlaySound(const std::string& path, const float& volume = 1);
	static void PlayMusic(const std::string& path, const float& volume = 1, bool loop = true);
	static void StopMusic();
	static void PauseMusic();
	static void Close();
private:
	static std::map<std::string, Mix_Music*> music;
	static std::map<std::string, Mix_Chunk*> sounds;
	AudioManager();
	enum AudioChannels
	{
		BASIC_SOUNDS
	};
};