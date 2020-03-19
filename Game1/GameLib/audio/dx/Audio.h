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
	void PlayLoop(std::string);	// ���[�v�Đ��iBGM�ȂǂɎg�p�j
	void Stop(std::string);
	void PlayOneShot(std::string);	// ��x�݂̂̍Đ��iSE�ȂǂɎg�p�j
	/**
	 *	@brief �����ɂ��ҋ@�Đ�or�����Đ����s��
	 *	@param (audioName) audioRack�̃L�[�ɂ��Ă��閼�O
	 *	@param (isWait) true:�ҋ@�Đ��Afalse:�����Đ�
	 */
	void Play(std::string audioName, bool isWait);
};

#endif // AUDIO_H_