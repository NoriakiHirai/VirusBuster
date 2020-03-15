/**
 *  @file		SoundManagerWin.h
 *  @brief		�T�E���h�}�l�[�W���N���X
 */
#ifndef __SOUNDMANAGERWIN_H__
#define	__SOUNDMANAGERWIN_H__

// �C���N���[�h
#include	"SoundManager.h"


#define	gSoundManagerWin	CSoundManagerWin::GetInstance()


//! CSoundManagerWin�N���X
class CSoundManagerWin : public CSoundManager
{
  protected:
								CSoundManagerWin(void) {
								}
	virtual						~CSoundManagerWin(void) {
								}

			CVoiceElement*		CreateVoiceElement(UINT32 nType);

  public:
	static	CSoundManagerWin&	GetInstance(void);
};


#endif		// __SOUNDMANAGERWIN_H__
/*  Bottom of SoundManagerWin.h  */
