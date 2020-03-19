#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdio.h>
#include <Dsound.h>
#include <map>
#include <string>

class DirectAudio
{
public:
	LPDIRECTSOUND8 pDSound;
	std::map<std::string, LPDIRECTSOUNDBUFFER> audioRack;

	DirectAudio();
	~DirectAudio();

	HRESULT Initialize(HWND);
	HRESULT LoadAudio(std::string audioFile, std::string audioName);
	//HRESULT LoadAudio(LPSTR audioFile, std::string audioName);
	void UnloadAudio(std::string audioName);
	void PlayLoop(std::string);	// ループ再生（BGMなどに使用）
	void Stop(std::string);
	void PlayOneShot(std::string);	// 一度のみの再生（SEなどに使用）
	/**
	 *	@brief 引数により待機再生or即時再生を行う
	 *	@param (audioName) audioRackのキーにしている名前
	 *	@param (isWait) true:待機再生、false:即時再生
	 */
	void Play(std::string audioName, bool isWait);
};

#endif // AUDIO_H_