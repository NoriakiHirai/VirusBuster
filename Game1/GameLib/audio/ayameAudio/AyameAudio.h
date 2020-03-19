#pragma once
#include <string>
#include <common/Component.h>
#include "ayame/VoiceElementAyame.h"

class AyameAudio : public Component
{
public:
	AyameAudio()
	{
		m_Loop = false;
	}

	void SetClip(const char* fileName);

	bool isPlaying() const;

	void SetLoop(bool loop);
	bool GetLoop() const;

	void SetVolume(float volume);
	float GetVolume() const;

	void Play();
	void Stop();
	void Pause();
	void UnPause();

protected:
	bool				m_Loop;
	CVoiceElementAyame	m_Sound;

	~AyameAudio();
};
