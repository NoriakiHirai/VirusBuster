/**
 *  @file		SoundManagerWin.cpp
 *  @brief		�T�E���h�}�l�[�W���N���X
 */

// �C���N���[�h
#include	"SoundManagerWin.h"
#include	"VoiceElementAyame.h"

CVoiceElement* CSoundManagerWin::CreateVoiceElement(UINT32 nType)
{
	return new CVoiceElementAyame;
}


CSoundManagerWin& CSoundManagerWin::GetInstance(void)
{
	static	CSoundManagerWin	gsSoundManager;

	return gsSoundManager;
}


/*  Bottom of SoundManager.cpp  */
