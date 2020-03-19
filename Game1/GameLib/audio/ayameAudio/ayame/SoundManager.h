/**
 *  @file		SoundManager.h
 *  @brief		�T�E���h�}�l�[�W���N���X
 */
#ifndef __SOUNDMANAGER_H__
#define	__SOUNDMANAGER_H__

// �C���N���[�h
#include	"VoiceElement.h"


const	INT32		cnSndMgr_MasterVolumeNum	= 2;			//!< �}�X�^�[�{�����[����
const	INT32		cnSndMgr_BgmSlotNum			= 4;			//!< �a�f�l�X���b�g��
const	INT32		cnSndMgr_SeListSlotNum		= 4;			//!< �r�d���X�g�X���b�g��
const	INT32		cnSndMgr_MaxSeNum			= 64;			//!< �ő哯���r�d������

#define	gSoundManager		CSoundManager::GetInstance()

const	UINT32		cnSndMgr_LoopInfinity		= 0xffffffff;	//!< �������[�v
const	UINT32		cnSndMgr_PlayOnce			= 0;			//!< 1��Đ�


// �T�E���h���X�g�\���̒�`
typedef struct
{
			UINT16				wNumber;						//!< �i���o�[
			UINT16				cWavType;						//!< WAV�^�C�v
			UINT32				nLoop;							//!< ���[�v���邩�ǂ���
			TCHAR*				szFileName;						//!< �t�@�C����
			bool				bOnMemory;						//!< �I���������Đ�
			UINT32				nLoopInSample;					//!< ���[�v�C���|�C���g
			UINT32				nFlag;							//!< �t���O
}SOUND_LIST;


// �a�f�l���\���̒�`
typedef struct
{
			CVoiceElement*		pVoice[2];						//!< Voice�G�������g
			UINT32				dwPlayingBgmNum;				//!< ���ݍĐ����̂a�f�l�i���o�[
			UINT32				dwNextPlayBgmNum;				//!< ���ɍĐ�����a�f�l�i���o�[
			INT32				nVolume;						//!< �Đ��{�����[��
			INT32				nFadeInTick;					//!< �Đ����s���ۂ̃t�F�[�h�C���^�C��
			INT32				nFadeOutTick[2];				//!< �Đ�����߂�ۂ̃t�F�[�h�A�E�g�^�C��
			bool				bPause;							//!< �|�[�Y���t���O
}SNDMGR_BGMINFO;


//! CSoundManager�N���X
class CSoundManager
{
  private:
			INT32				m_nMasterVolume[cnSndMgr_MasterVolumeNum];
	  
	  const	SOUND_LIST*			m_pBgmList;
			UINT32				m_dwBgmListNum;
			SNDMGR_BGMINFO		m_stBgmInfo[cnSndMgr_BgmSlotNum];
			INT32				m_iNowPlayingBgmSlot;
			INT32				m_iNextPlayBgmSlot;

	const	SOUND_LIST*			m_pSeList[cnSndMgr_SeListSlotNum];
			UINT32				m_dwSeListNum[cnSndMgr_SeListSlotNum];
			CVoiceElement*		m_pSeVoice[cnSndMgr_MaxSeNum];

  private:
			void				ReleaseBgmData(void);
			void				ReleaseSeData(UINT32 nSlot);
	const	SOUND_LIST*			GetListData(const SOUND_LIST* pList, UINT32 nListNum, UINT32 dwNum)	const	{
									for (UINT32 i = 0; i < nListNum; i++, pList++) {
										if (pList->wNumber == dwNum)		return pList;
									}
									return NULL;
								}

  protected:
								CSoundManager(void);
	virtual						~CSoundManager(void);

								//!	�{�C�X�G�������g�쐬
								/**
								 *	�{�C�X�G�������g���쐬���܂��B<br>
								 *	�Ǝ��̃{�C�X�G�������g�𐶐��������ꍇ�́A�h�����Ă��������B<br>
								 *
								 *	@return	���������{�C�X�G�������g�|�C���^
								 */
	virtual	CVoiceElement*		CreateVoiceElement(UINT32 nType) {
									return new CVoiceElement;
								}
	virtual	void*				GetDataPtr(const TCHAR* szName, unsigned long& nSize)	const	{
									nSize = 0;
									return NULL;
								}
	virtual	void				ReleaseDataPtr(const TCHAR* szName) {};
	virtual	bool				SetData(CVoiceElement* pVoice, const SOUND_LIST* pList);

  public:
	virtual	BOOL				IsEnable(void)	const	{
									return TRUE;
								}
	virtual	void				Run(FLOAT fSyncTime);
	virtual	void				DeleteThis(void) {}

			// ���Z�b�g
	virtual	void				ResetSoundSystem(void);

			// �}�X�^�[�{�����[��
			INT32				GetMasterVolume()	const;
			INT32				GetMasterVolume(UINT32 nChannel)	const;
			void				SetMasterVolume(UINT32 nChannel, INT32 nVolume);

			// BGM����
			void				SetBgmList(const SOUND_LIST* pList, UINT32 nListNum);
			void				SetBgm(UINT32 nSlot, UINT32 nBgmNum, INT32 nVolume = cnVoiceElm_VolumeMax, INT32 fFadeInTick = 0, INT32 fFadeOutTick = 0);
			void				PlayBgm(UINT32 nSlot, INT32 nVolume = -1, INT32 nFadeInTick = -1);
			void				StopBgm(UINT32 nSlot, INT32 nFadeOutTick = -1);
			void				PauseBgm(bool bPause, INT32 nSlot = -1, INT32 nFadeTick = -1);
			bool				IsPauseBgm(INT32 nSlot = -1)	const;

			INT32				GetBgmVolume(UINT32 nSlot)	const;
			void				SetBgmVolume(UINT32 nSlot, INT32 nVolume, INT32 nFadeTick = 0, INT32 nStartVolume = -1);

								//! �w�肵���X���b�g���Đ������ǂ���
								/**
								 *	@param	nSlot	[in] BGM�X���b�g�i���o�[
								 *
								 *	@return		�Đ����̏ꍇ��true��Ԃ�
								 */
			bool				IsPlayingBgm(UINT32 nSlot)	const	{
									return (nSlot < cnSndMgr_BgmSlotNum && m_iNowPlayingBgmSlot == nSlot);
								}
								//! �Đ�����BGM�X���b�g�i���o�[
								/**
								 *	@return		�Đ�����BGM�X���b�g�i���o�[
								 */
			INT32				GetPlayingBgmSlot(void)	const	{
									return (INT32)m_iNowPlayingBgmSlot;
								}
								//! �Đ�����BGM�i���o�[
								/**
								 *	@return		�Đ�����BGM�i���o�[
								 */
			UINT32				GetPlayingBgmNumber(void)	const	{
									if (m_iNowPlayingBgmSlot < 0) {
										return 0;
									}
									return m_stBgmInfo[m_iNowPlayingBgmSlot].dwPlayingBgmNum;
								}

			// SE����
			void				SetSeList(UINT32 nSlot, const SOUND_LIST* pList, UINT32 nListNum);

			bool				GetUnique(UINT32 nSlot, UINT32& nUnique)	const;

			bool				IsPlayingBySlot(UINT32 nSlot)	const;
			bool				IsPlayingByUnique(UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff)	const;
			bool				IsPlayingBySeNum(UINT32 nSlot, UINT32 nSeNum = 0xffffffff)	const;

			bool				PlaySe(UINT32 nSlot, UINT32 nSeNum, UINT32 nUnique, INT32 nVolume = cnVoiceElm_VolumeMax, INT32 nFadeInTick = 0);
			void				StopSe(UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nFadeOutTick = 0);
			void				StopSeNum(UINT32 nSlot, UINT32 nSeNum = 0xffffffff, INT32 nFadeOutTick = 0);
			void				PauseSe(bool bPause, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nFadeTick = 0);
			bool				IsPauseSe(UINT32 nUnique)	const;

			INT32				GetSeVolume(UINT32 nUnique)	const;
			void				SetSeVolume(INT32 nVolume, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nTick = 0);
			INT32				GetSePanpot(UINT32 nUnique)	const;
			void				SetSePanpot(INT32 nPanpot, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nTick = 0);

	static	CSoundManager&		GetInstance(void);
};


#endif		// __SOUNDMANAGER_H__
/*  Bottom of SoundManager.h  */
