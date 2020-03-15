/**
 *  @file		SoundManager.cpp
 *  @brief		�T�E���h�}�l�[�W���N���X
 */

// �C���N���[�h
#include	"SoundManager.h"


//!	�R���X�g���N�^
CSoundManager::CSoundManager(void)
{
	m_pBgmList           = NULL;
	m_dwBgmListNum       = 0;
	m_iNowPlayingBgmSlot = -1;
	m_iNextPlayBgmSlot   = -1;
	ZeroMemory(m_stBgmInfo, sizeof(m_stBgmInfo));

	for(INT32 i = 0; i < cnSndMgr_MasterVolumeNum; i++) {
		m_nMasterVolume[i] = cnVoiceElm_VolumeMax;
	}
	CVoiceElement::SetGlobalVolume(cnVoiceElm_VolumeMax);

	for(INT32 i = 0; i < cnSndMgr_SeListSlotNum; i++) {
		m_pSeList[i]     = NULL;
		m_dwSeListNum[i] = 0;
	}
	ZeroMemory(m_pSeVoice, sizeof(m_pSeVoice));
}


//!	�f�X�g���N�^
CSoundManager::~CSoundManager(void)
{
	ResetSoundSystem();
}


bool	CSoundManager::SetData(CVoiceElement* pVoice, const SOUND_LIST* pList)
{
	if (pList->bOnMemory == true) {
		unsigned long	nSize;
		void*	pData = GetDataPtr(pList->szFileName, nSize);
		return (pData == NULL)? false : pVoice->Load((char*)pData, nSize, pList->nFlag);
	} else {
		return pVoice->Load(pList->szFileName, pList->nFlag, 0, 0);
	}
}


//!	���Z�b�g
/**
 *	�T�E���h�}�l�[�W���̃��Z�b�g���s���܂��B<br>
 */
void CSoundManager::ResetSoundSystem(void)
{
	// �SSE�̒�~
	for(INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		delete m_pSeVoice[i];
		m_pSeVoice[i] = NULL;
	}

	// BGM���X�g�̏���
	SetBgmList(NULL, 0);

	// SE���X�g�̏���
	for(INT32 i = 0; i < cnSndMgr_SeListSlotNum; i++) {
		SetSeList(i, NULL, 0);
	}
}


//!	��Ꮘ��
/**
 *	���t���[���s�������ł��B<br>
 *
 *	@param	fSyncTime	[in] �O�񂩂�̌o�ߎ���[sec]
 */
void CSoundManager::Run(FLOAT fSyncTime)
{
	// �a�f�l��Ꮘ��
	SNDMGR_BGMINFO*		pInfo = m_stBgmInfo;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++, pInfo++) {
		for (INT32 j = 0; j < 2; j++) {
			if (pInfo->pVoice[j] != NULL) {
				pInfo->pVoice[j]->Run((INT32)(fSyncTime * 480.0f));
				if (pInfo->pVoice[j]->IsFinished() == true) {
					delete pInfo->pVoice[j];
					pInfo->pVoice[j] = NULL;
				}
			}
		}
		if (pInfo->pVoice[0] == NULL && pInfo->pVoice[1] != NULL) {
			pInfo->pVoice[0] = pInfo->pVoice[1];
			pInfo->pVoice[1] = NULL;
		}
	}

	if (m_iNextPlayBgmSlot >= 0) {
		if (m_iNowPlayingBgmSlot >= 0) {
			// ���݂̃X���b�g�̂a�f�l���~������
			PauseBgm(true, m_iNowPlayingBgmSlot);
		}
		m_iNowPlayingBgmSlot = m_iNextPlayBgmSlot;
		m_iNextPlayBgmSlot   = -1;
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[m_iNowPlayingBgmSlot];
		if (pInfo->bPause == true) {
			PauseBgm(false, m_iNowPlayingBgmSlot);
		} else if (pInfo->dwNextPlayBgmNum == 0) {
			pInfo->dwNextPlayBgmNum = pInfo->dwPlayingBgmNum;
			pInfo->nFadeOutTick[1]  = pInfo->nFadeOutTick[0];
		}
	}

	if (m_iNowPlayingBgmSlot >= 0) {
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[m_iNowPlayingBgmSlot];
		UINT32				dwNextBgmNum = pInfo->dwNextPlayBgmNum;
		if (dwNextBgmNum != 0) {
			// ����BGM�ɐ؂�ւ���
			if (pInfo->pVoice[1] == NULL) {
				if (pInfo->pVoice[0] != NULL) {
					pInfo->pVoice[0]->Stop(pInfo->nFadeOutTick[0]);
				}
				const	SOUND_LIST*		pList = GetListData(m_pBgmList, m_dwBgmListNum, dwNextBgmNum);
				if (pList != NULL) {
					pInfo->pVoice[1] = CreateVoiceElement(pList->cWavType);
					if (pInfo->pVoice[1] != NULL) {
						if (SetData(pInfo->pVoice[1], pList) == false) {
							delete pInfo->pVoice[1];
							pInfo->pVoice[1] = NULL;
						} else {
							pInfo->pVoice[1]->Play(pList->nLoop, pInfo->nVolume, pInfo->nFadeInTick, pList->nLoopInSample);
						}
					}
				}
				pInfo->dwPlayingBgmNum  = dwNextBgmNum;
				pInfo->dwNextPlayBgmNum = 0;
				pInfo->nFadeOutTick[0]  = pInfo->nFadeOutTick[1];
				pInfo->nFadeOutTick[1]  = 0;
			}
		} else {
			if (pInfo->pVoice[0] == NULL && pInfo->pVoice[1] == NULL) {
				m_iNowPlayingBgmSlot = -1;
			}
		}
	}

	// �r�d�̒�Ꮘ��
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)		continue;

		m_pSeVoice[i]->Run((INT32)(fSyncTime * 480.0f));
		if (m_pSeVoice[i]->IsFinished() == true) {
			delete m_pSeVoice[i];
			m_pSeVoice[i] = NULL;
		}
	}
}


//! �}�X�^�[�{�����[���̎擾
/**
 *	�ŏI�I�ȃ}�X�^�[�{�����[���l���擾���܂��B
 *
 *	@return	�ŏI�I�ȃ}�X�^�[�{�����[���l
 */
INT32 CSoundManager::GetMasterVolume()	const
{
	INT32	nVol = m_nMasterVolume[0];
	for (INT32 i=1; i < cnSndMgr_MasterVolumeNum; i++) {
		nVol *= m_nMasterVolume[i];
		nVol /= cnVoiceElm_VolumeRange;
	}

	return nVol;
}

//! �}�X�^�[�{�����[���̎擾
/**
 *	�w�肵���}�X�^�[�{�����[���l���擾���܂��B
 *
 *	@param	nChannel	[in] �}�X�^�[�{�����[���`�����l��
 *
 *	@return	�}�X�^�[�{�����[���l
 */
INT32 CSoundManager::GetMasterVolume(UINT32 nChannel)	const
{
	return (nChannel >= cnSndMgr_MasterVolumeNum)? cnVoiceElm_VolumeMin : m_nMasterVolume[nChannel];
}


//! �}�X�^�[�{�����[���̐ݒ�
/**
 *	�}�X�^�[�{�����[���̐ݒ���s���܂��B
 *
 *	@param	nChannel	[in] �}�X�^�[�{�����[���`�����l��
 *	@param	nVolume		[in] �}�X�^�[�{�����[���l
 */
void CSoundManager::SetMasterVolume(UINT32 nChannel, INT32 nVolume)
{
	if (nChannel >= cnSndMgr_MasterVolumeNum)	return;

	nVolume = (nVolume < cnVoiceElm_VolumeMin)? cnVoiceElm_VolumeMin : ((nVolume > cnVoiceElm_VolumeMax)? cnVoiceElm_VolumeMax : nVolume);
	m_nMasterVolume[nChannel] = nVolume;

	// �{�����[���̍Čv�Z
	CVoiceElement::SetGlobalVolume(GetMasterVolume());

	// �{�����[���𔽉f����
	SNDMGR_BGMINFO*		pInfo = m_stBgmInfo;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++, pInfo++) {
		for (INT32 j = 0; j < 2; j++) {
			if (pInfo->pVoice[j] != NULL) {
				pInfo->pVoice[j]->ReflectMasterVolume();
			}
		}
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			m_pSeVoice[i]->ReflectMasterVolume();
		}
	}
}


//!	BGM���X�g�̓o�^
/**
 *	�T�E���h�}�l�[�W����BGM���X�g��o�^���܂��B<br>
 *	�ȑO�ɓo�^����Ă������X�g���㏑�����܂��B<br>
 *
 *	@param pList		[in] BGM���X�g�̃|�C���^
 *	@param nListNum		[in] ���X�g��
 */
void CSoundManager::SetBgmList(const SOUND_LIST* pList, UINT32 nListNum)
{
	ReleaseBgmData();
	m_pBgmList     = pList;
	m_dwBgmListNum = nListNum;
}


void CSoundManager::ReleaseBgmData(void)
{
	m_iNextPlayBgmSlot = -1;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++) {
		delete m_stBgmInfo[i].pVoice[0];
		delete m_stBgmInfo[i].pVoice[1];
		m_stBgmInfo[i].pVoice[0]        = NULL;
		m_stBgmInfo[i].pVoice[1]        = NULL;
		m_stBgmInfo[i].dwPlayingBgmNum  = 0;
		m_stBgmInfo[i].dwNextPlayBgmNum = 0;
	}

	UINT32 nListNum = m_dwBgmListNum;
	if (nListNum != 0) {
		const	SOUND_LIST* pList = m_pBgmList;
		for (UINT32 i = 0; i < nListNum; i++, pList++) {
			if (pList->bOnMemory == true) {
				ReleaseDataPtr(pList->szFileName);
			}
		}
	}
}


//!	BGM�i���o�[�̐ݒ�
/**
 *	�w�肵���X���b�g�ɂa�f�l�i���o�[��ݒ肵�܂��B<br>
 *	�t�F�[�h�^�C���́A���ۂɍĐ��E��~���s���ۂɁA�f�t�H���g���w�肳�ꂽ�ꍇ�Ɏg�p����܂��B<br>
 *	�Đ��E��~���ɖ��m�Ȏw����s��ꂽ�ꍇ�́A�����炪�D�悳��܂��B<br>
 *
 *	@param nSlot		[in] BGM�X���b�g
 *	@param dwBgmNum		[in] BGM�i���o�[
 *	@param nVolume		[in] �{�����[��
 *	@param nFadeInTick	[in] ����BGM���Đ�����ۂ̃t�F�[�h�C���^�C��
 *	@param nFadeOutTick	[in] ����BGM����~����ۂ̃t�F�[�h�A�E�g�^�C��
 */
void CSoundManager::SetBgm(UINT32 nSlot, UINT32 dwBgmNum, INT32 nVolume, INT32 nFadeInTick, INT32 nFadeOutTick)
{
	if (nSlot < cnSndMgr_BgmSlotNum) {
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
		if (pInfo->dwPlayingBgmNum != dwBgmNum && pInfo->dwNextPlayBgmNum != dwBgmNum) {
			pInfo->dwNextPlayBgmNum = dwBgmNum;
			pInfo->nVolume          = nVolume;
			pInfo->nFadeInTick      = nFadeInTick;
			pInfo->nFadeOutTick[1]  = nFadeOutTick;
		}
	}
}


//!	BGM�̍Đ�
/**
 *	BGM�̍Đ����s���܂��B<br>
 *
 *	@param nSlot		[in] BGM�X���b�g
 *	@param nVolume		[in] �{�����[��(�}�C�i�X�̏ꍇ�͎w��Ȃ�)
 *	@param nFadeInTick	[in] �Đ�����ۂ̃t�F�[�h�C���^�C��(�}�C�i�X�̏ꍇ�͎w��Ȃ�)
 */
void CSoundManager::PlayBgm(UINT32 nSlot, INT32 nVolume, INT32 nFadeInTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum)		return;
	if (m_iNowPlayingBgmSlot == nSlot)		return;

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (nVolume >= 0) {
		pInfo->nVolume = nVolume;
	}
	if (nFadeInTick >= 0) {
		pInfo->nFadeInTick = nFadeInTick;
	}
	m_iNextPlayBgmSlot = nSlot;
}


//!	BGM�̒�~
/**
 *	BGM�̒�~���s���܂��B<br>
 *
 *	@param nSlot		[in] BGM�X���b�g
 *	@param nFadeOutTick	[in] �t�F�[�h�A�E�g�^�C��
 */
void CSoundManager::StopBgm(UINT32 nSlot, INT32 nFadeOutTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum)		return;

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	pInfo->bPause = false;
	if (m_iNextPlayBgmSlot == nSlot) {
		m_iNextPlayBgmSlot = -1;
		return;
	}
	if (m_iNowPlayingBgmSlot == nSlot) {
		if (nFadeOutTick < 0) {
			nFadeOutTick = pInfo->nFadeOutTick[0];
		}
		if (pInfo->pVoice[1] != NULL) {
			pInfo->pVoice[1]->Stop(nFadeOutTick);
		}
		if (pInfo->pVoice[0] != NULL) {
			if (pInfo->pVoice[1] != NULL) {
				delete pInfo->pVoice[0];
				pInfo->pVoice[0] = pInfo->pVoice[1];
				pInfo->pVoice[1] = NULL;
			} else {
				pInfo->pVoice[0]->Stop(nFadeOutTick);
			}
		}
		pInfo->dwNextPlayBgmNum =  0;
		m_iNowPlayingBgmSlot    = -1;
	}
}


//!	BGM�̈ꎞ��~�E�ĊJ
/**
 *	BGM�̈ꎞ��~����эĊJ���s���܂��B<br>
 *
 *	@param bPause		[in] �ꎞ��~����ꍇ��true
 *	@param nSlot		[in] BGM�X���b�g(�}�C�i�X�ōĐ����X���b�g��I��)
 *	@param nFadeTick	[in] �t�F�[�h�^�C��
 */
void CSoundManager::PauseBgm(bool bPause, INT32 nSlot, INT32 nFadeTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return;
	}
	if (nSlot < 0) {
		nSlot = m_iNowPlayingBgmSlot;
	} else if (m_iNowPlayingBgmSlot != nSlot) {
		return;
	}
	if (nSlot == -1)		return;
	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (nFadeTick < 0) {
		if (bPause == false) {
			nFadeTick = pInfo->nFadeInTick;
		} else {
			nFadeTick = pInfo->nFadeOutTick[0];
		}
	}
	if (pInfo->pVoice[1] != NULL) {
		pInfo->pVoice[1]->Pause(bPause, nFadeTick);
	} else if (pInfo->pVoice[0] != NULL) {
		pInfo->pVoice[0]->Pause(bPause, nFadeTick);
	}
	pInfo->bPause = bPause;
}


//!	BGM���ꎞ��~���ǂ���
/**
 *	�w�肵���X���b�g��BGM���ꎞ��~�����ǂ������擾���܂��B<br>
 *
 *	@param nSlot		[in] BGM�X���b�g(�}�C�i�X�ōĐ����X���b�g��I��)
 *
 *	@return		�ꎞ��~���̏ꍇ��true��Ԃ�
 */
bool CSoundManager::IsPauseBgm(INT32 nSlot)	const
{
	if (nSlot < 0) {
		nSlot = m_iNowPlayingBgmSlot;
	}
	if (nSlot < 0 || nSlot >= cnSndMgr_BgmSlotNum) {
		return false;
	}

	return m_stBgmInfo[nSlot].bPause;
}


//! BGM�{�����[���̎擾
INT32 CSoundManager::GetBgmVolume(UINT32 nSlot)	const
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return 0;
	}

	const	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (pInfo->pVoice[1] != NULL) {
		return pInfo->pVoice[1]->GetVolume();
	} else if (pInfo->pVoice[0] != NULL) {
		return pInfo->pVoice[0]->GetVolume();
	}

	return pInfo->nVolume;
}


//! BGM�{�����[���̐ݒ�
void CSoundManager::SetBgmVolume(UINT32 nSlot, INT32 nVolume, INT32 nFadeTick, INT32 nStartVolume)
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return;
	}

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (pInfo->pVoice[1] != NULL) {
		pInfo->pVoice[1]->SetVolume(nVolume, nFadeTick, nStartVolume);
	} else if (pInfo->pVoice[0] != NULL) {
		pInfo->pVoice[0]->SetVolume(nVolume, nFadeTick, nStartVolume);
	}
}


//!	SE���X�g�̓o�^
/**
 *	�T�E���h�}�l�[�W����SE���X�g��o�^���܂��B<br>
 *	�ȑO�ɓo�^����Ă������X�g���㏑�����܂��B<br>
 *
 *	@param nSlot		[in] SE�X���b�g
 *	@param pList		[in] SE���X�g�̃|�C���^
 *	@param nListNum		[in] ���X�g��
 */
void CSoundManager::SetSeList(UINT32 nSlot, const SOUND_LIST* pList, UINT32 nListNum)
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return;
	}
	ReleaseSeData(nSlot);
	m_pSeList[nSlot]     = pList;
	m_dwSeListNum[nSlot] = nListNum;
}


void CSoundManager::ReleaseSeData(UINT32 nSlot)
{
	UINT32 nListNum = m_dwSeListNum[nSlot];
	StopSeNum(nSlot);
	if (nListNum != 0) {
		const	SOUND_LIST* pList = m_pSeList[nSlot];
		nSlot <<= 24;
		for (UINT32 i = 0; i < nListNum; i++, pList++) {
			if (pList->bOnMemory == true) {
				ReleaseDataPtr(pList->szFileName);
			}
		}
	}
}


//! ����ID�̎擾
/**
 *	@param	nSlot		[in] SE�X���b�g
 *	@param	nUnique		[out] ����ID
 *
 *	@return	�������擾�o�����true��Ԃ�
 */
bool CSoundManager::GetUnique(UINT32 nSlot, UINT32& nUnique)	const
{
	if (nSlot >= cnSndMgr_MaxSeNum)		return false;
	if (m_pSeVoice[nSlot] == NULL)		return false;

	nUnique = m_pSeVoice[nSlot]->GetUnique();

	return true;
}


//! �w�肵������ID��SE���Đ������ǂ���
/**
 *	@param	nSlot		[in] SE�X���b�g
 *
 *	@return	�Đ����Ă����true��Ԃ�
 */
bool CSoundManager::IsPlayingBySlot(UINT32 nSlot)	const
{
	if (nSlot >= cnSndMgr_MaxSeNum)		return false;
	if (m_pSeVoice[nSlot] == NULL)		return false;

	return m_pSeVoice[nSlot]->IsPlaying();
}


//! �w�肵������ID��SE���Đ������ǂ���
/**
 *	@param	nUnique		[in] ����ID
 *	@param	nUniqueMask	[in] ����ID�}�X�N
 *
 *	@return	�P�ł��Đ����Ă����true��Ԃ�
 */
bool CSoundManager::IsPlayingByUnique(UINT32 nUnique, UINT32 nUniqueMask)	const
{
	nUnique &= nUniqueMask;
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)		continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique && m_pSeVoice[i]->IsPlaying() == true) {
			return true;
		}
	}

	return false;
}


//! �w�肵��SE�i���o�[��SE���Đ������ǂ���
/**
 *	@param	nSlot	[in] SE�X���b�g�i���o�[
 *	@param	nSeNum	[in] SE�i���o�[(�w�肵�Ȃ��ꍇ��0xffffffff)
 *
 *	@return	�P�ł��Đ����Ă����true��Ԃ�
 */
bool CSoundManager::IsPlayingBySeNum(UINT32 nSlot, UINT32 nSeNum)	const
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return 0;
	}

	nSlot <<= 24;
	if (nSeNum != 0xffffffff) {
		nSlot |= nSeNum;
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			UINT32	nNum = m_pSeVoice[i]->GetNumber();
			if (nSeNum == 0xffffffff) {
				nNum &= 0xff000000;
			}
			if (nNum == nSlot && m_pSeVoice[i]->IsPlaying()) {
				return true;
			}
		}
	}

	return false;
}


//!	SE�̍Đ�
/**
 *	SE�̍Đ����s���܂��B<br>
 *	�Ȍ�̑���͊�{�I�ɂ͎���ID�ɂ���čs���܂��B<br>
 *
 *	@param nSeSlot		[in] SE�X���b�g�i���o�[
 *	@param nSeNum		[in] SE�i���o�[(0�`0xffffff)
 *	@param nUnique		[in] ����ID(32Bit)
 *	@param nVolume		[in] �{�����[��
 *	@param nFadeInTick	[in] �t�F�[�h�C���^�C��
 */
bool CSoundManager::PlaySe(UINT32 nSeSlot, UINT32 nSeNum, UINT32 nUnique, INT32 nVolume, INT32 nFadeInTick)
{
	if (nSeSlot >= cnSndMgr_SeListSlotNum) {
		return false;
	}

	nSeNum &= 0x00ffffff;
	const	SOUND_LIST*		pList = GetListData(m_pSeList[nSeSlot], m_dwSeListNum[nSeSlot], nSeNum);
	if (pList == NULL) {
		return false;
	}

	CVoiceElement*	pVoice = NULL;
	INT32			nSlot;
	for (nSlot = 0; nSlot < cnSndMgr_MaxSeNum; nSlot++) {
		if (m_pSeVoice[nSlot] == NULL) {
			pVoice = CreateVoiceElement(pList->cWavType);
			break;
		}
	}
	if (pVoice == NULL) {
		return false;
	}

	if (SetData(pVoice, pList) == false) {
		delete pVoice;
		return false;
	}
	pVoice->SetNumber((nSeSlot << 24) | nSeNum);
	pVoice->SetUnique(nUnique);
	pVoice->Play(pList->nLoop, nVolume, nFadeInTick, pList->nLoopInSample);
	m_pSeVoice[nSlot] = pVoice;

	return true;
}


//!	SE�̒�~
/**
 *	SE�̒�~���s���܂��B<br>
 *	�Y�����鎯��ID��SE���A�S�Ē�~���܂��B<br>
 *
 *	@param nUnique		[in] ����ID
 *	@param nUniqueMask	[in] ����ID�}�X�N
 *	@param nFadeOutTick	[in] �t�F�[�h�A�E�g�^�C��
 */
void CSoundManager::StopSe(UINT32 nUnique, UINT32 nUniqueMask, INT32 nFadeOutTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
				m_pSeVoice[i]->Stop(nFadeOutTick);
			}
		}
	}
}


//!	SE�̒�~
/**
 *	SE�̒�~���s���܂��B<br>
 *	�Y������SE�i���o�[��SE���A�S�Ē�~���܂��B<br>
 *
 *	@param nSlot		[in] SE�X���b�g
 *	@param nSeNum		[in] SE�i���o�[
 *	@param nFadeOutTick	[in] �t�F�[�h�A�E�g�^�C��
 */
void CSoundManager::StopSeNum(UINT32 nSlot, UINT32 nSeNum, INT32 nFadeOutTick)
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return;
	}

	nSlot <<= 24;
	if (nSeNum != 0xffffffff) {
		nSlot |= nSeNum;
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			UINT32	nNum = m_pSeVoice[i]->GetNumber();
			if (nSeNum == 0xffffffff) {
				nNum &= 0xff000000;
			}
			if (nNum == nSlot) {
				if (nFadeOutTick == 0) {
					delete m_pSeVoice[i];
					m_pSeVoice[i] = NULL;
				} else {
					m_pSeVoice[i]->Stop(nFadeOutTick);
				}
			}
		}
	}
}


//!	SE�̈ꎞ��~�E�ĊJ
/**
 *	SE�̈ꎞ��~����эĊJ���s���܂��B<br>
 *	�Y�����鎯��ID��SE���A�S�Ĕ��f���܂��B<br>
 *
 *	@param bPause		[in] �|�[�Y����ꍇ��true
 *	@param nUnique		[in] ����ID
 *	@param nUniqueMask	[in] ����ID�}�X�N
 *	@param nFadeTick	[in] �t�F�[�h�^�C��
 */
void CSoundManager::PauseSe(bool bPause, UINT32 nUnique, UINT32 nUniqueMask, INT32 nFadeTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
				m_pSeVoice[i]->Pause(bPause, nFadeTick);
			}
		}
	}
}


//!	SE���ꎞ��~�����ǂ���
/**
 *	�Y�����鎯��ID��SE���ꎞ��~�����ǂ������擾���܂��B<br>
 *
 *	@param nUnique		[in] ����ID
 *
 *	@return		��~���̏ꍇ��true��Ԃ�
 */
bool CSoundManager::IsPauseSe(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->IsPause();
		}
	}

	return false;
}


//! SE�{�����[���̎擾
/**
 *	@param nUnique		[in] ����ID
 *
 *	@return		�{�����[��
 */
INT32 CSoundManager::GetSeVolume(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->GetVolume();
		}
	}

	return 0;
}


//! SE�{�����[���̐ݒ�
/**
 *	@param nVolume		[in] �{�����[��
 *	@param nUnique		[in] ����ID
 *	@param nUniqueMask	[in] ����ID�}�X�N
 *	@param nTick		[in] �J�ڎ���
 */
void CSoundManager::SetSeVolume(INT32 nVolume, UINT32 nUnique, UINT32 nUniqueMask, INT32 nTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
			m_pSeVoice[i]->SetVolume(nVolume, nTick);
		}
	}
}


//! SE�p���̎擾
/**
 *	@param nUnique		[in] ����ID
 *
 *	@return		�p���|�b�g
 */
INT32 CSoundManager::GetSePanpot(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->GetPan();
		}
	}

	return 0;
}


//! SE�p���̐ݒ�
/**
 *	@param nVolume		[in] �p��
 *	@param nUnique		[in] ����ID
 *	@param nUniqueMask	[in] ����ID�}�X�N
 *	@param nTick		[in] �J�ڎ���
 */
void CSoundManager::SetSePanpot(INT32 nPanpot, UINT32 nUnique, UINT32 nUniqueMask, INT32 nTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
			m_pSeVoice[i]->SetPan(nPanpot, nTick);
		}
	}
}


CSoundManager& CSoundManager::GetInstance(void)
{
	static	CSoundManager	gsSoundManager;

	return gsSoundManager;
}


/*  Bottom of SoundManager.cpp  */
