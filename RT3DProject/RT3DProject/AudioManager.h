#pragma once
#include <map>
#include <SDL_mixer.h>

class AudioManager
{
public:
	AudioManager();
	void PlaySound(const std::string& path, const float& volume = 128);
	void PlayMusic(const std::string& path, const float& volume = 128, bool loop = true);
	void StopMusic() const;
	void PauseMusic() const;
	void Close();
private:
	std::map<std::string, Mix_Music*> music;
	std::map<std::string, Mix_Chunk*> sounds;

	enum AudioChannels
	{
		BASIC_SOUNDS
	};
};