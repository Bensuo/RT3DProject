#pragma once
#include <map>
#include "ResourceManager.h"

class AudioManager
{
public:
	static void init(Utilities::ResourceManager& content);
	static void playSound(const std::string& path, const float& volume = 1);
	static void playMusic(const std::string& path, const float& volume = 1, bool loop = true);
	static void stopMusic();
	static void pauseMusic();
	static void close();
private:
	AudioManager();
	enum AudioChannels
	{
		FREE_CHANNEL = -1,
		BASIC_SOUNDS = 0
	};
	static Utilities::ResourceManager* content;
};