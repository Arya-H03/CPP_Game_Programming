#include<SFML/Audio.hpp>
#include <iostream>

class AudioData
{
	
private:
	
	sf::Music gameMusic;
	sf::Music hitMusic;
	sf::Music deathMusic;
	sf::Music shootMusic;
	

public:

	AudioData()
	{		
		if (!gameMusic.openFromFile("./src/media/audio/Music.wav"))std::cerr << "Could not find audio file for Music";
		else
		{
			gameMusic.setLooping(true);
		}

		if (!hitMusic.openFromFile("./src/media/audio/HitSFX.wav"))std::cerr << "Could not find audio file for HitSFX";
		else
		{
			hitMusic.setLooping(false);
		}

		if (!deathMusic.openFromFile("./src/media/audio/DeathSFX.wav"))std::cerr << "Could not find audio file for DeathSFX";
		else
		{
			deathMusic.setLooping(false);
		}


		if (!shootMusic.openFromFile("./src/media/audio/ShootSFX.wav"))std::cerr << "Could not find audio file for ShootSFX";
		else
		{
			shootMusic.setLooping(false);
		}
	}
		
	void PlayHitSFX(float volume)
	{
		hitMusic.setVolume(volume);
		hitMusic.play();
	}

	void PlayDeathSFX(float volume)
	{
		deathMusic.setVolume(volume);
		deathMusic.play();
	}

	void PlayShootSFX(float volume)
	{
		shootMusic.setVolume(volume);
		shootMusic.play();
	}
	void PlayMusic(float volume)
	{
		gameMusic.setVolume(volume);
		gameMusic.play();
	}

};