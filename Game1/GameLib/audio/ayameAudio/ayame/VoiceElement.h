/**
 *  @file		VoiceElement.h
 *  @brief		�P��̃T�E���h�Ǘ��N���X
 */
#ifndef __VOICEELEMENT_H__
#define	__VOICEELEMENT_H__
#include	<windows.h>

#define	NUM_OF_SUB_VOLUME	2			//!< �T�u�{�����[���̐�

const	INT32		cnVoiceElm_VolumeMin	= 0;				//!< �ŏ��{�����[���l
const	INT32		cnVoiceElm_VolumeMax	= 128;				//!< �ő�{�����[���l
const	INT32		cnVoiceElm_VolumeRange	= (cnVoiceElm_VolumeMax - cnVoiceElm_VolumeMin);	//!< �{�����[���͈�
const	INT32		cnVoiceElm_PanpotCenter	= 0;				//!< �p���Z���^�[�l
const	INT32		cnVoiceElm_PanpotMaxR	= 128;				//!< �E�p���ő�l
const	INT32		cnVoiceElm_PanpotMaxL	= -128;				//!< ���p���ő�l
const	INT32		cnVoiceElm_PanpotRange	= (cnVoiceElm_PanpotMaxR - cnVoiceElm_PanpotMaxL);	//!< �p���͈�
const	INT32		cnVoiceElm_PanpotHalfRange	= (cnVoiceElm_PanpotRange / 2);					//!< �p���͈�(�n�[�t)


//! CVoiceElement�N���X
/**
 *	���̃N���X�́A�T�E���h�̃t�F�[�h�E�I�[�g�p���Ƃ��������ԕψʂ��s�����߂̃N���X�ł��B<br>
 *	�|�[�Y���Ƀt�F�[�h���s���ہA�{�����[����ۑ�����̂ŁA�|�[�Y�������Ɍ��̃{�����[���ɖ߂����Ƃ��o���܂��B<br>
 *	�܂��}�X�^�[�{�����[�����T�|�[�g���邽�߂̋@�\�������Ă��܂��B<br>
 *	�P�̃N���X�ɂ��A�P�̃{�C�X���Ǘ����邱�Ƃ��o���܂��B<br>
 *	�P�̂ł��g�p���邱�Ƃ͏o���܂����A�ʏ�̓T�E���h�}�l�[�W�����ɂ���ĊǗ�����܂��B<br>
 *	���̃N���X���̂́A�C���^�[�t�F�C�X��񋟂��邾���ł��̂ŁA���ۂɂ͂��̃N���X����h�������N���X�𗘗p���܂��B<br>
 */
class CVoiceElement
{
	class CFadeValue
	{
		private:
			INT32				m_nNowValue;				//!< ���݂̒l
			INT32				m_nNowTick;					//!< ���݂̃^�C��
			INT32				m_nTotalTick;				//!< ���^�C��
			INT32				m_nStartVal;				//!< �J�n�l
			INT32				m_nDistVal;					//!< �͈͒l

		public:
								CFadeValue(void) {
									m_nNowTick = 0;
								}
								~CFadeValue(void) {}

			bool				IsFade(void)	const	{
									return (m_nNowTick > 0);
								}
			void				ResetTime(void) {
									m_nNowTick = 0;
								}
			INT32				GetNowValue(void)	const	{
									return m_nNowValue;
								}
			void				SetNowValue(INT32 nValue) {
									m_nNowValue = nValue;
								}
			void				SetParam(INT32 nTick, INT32 nStart, INT32 nEnd) {
									m_nNowTick   = nTick;
									m_nTotalTick = nTick;
									m_nStartVal  = nEnd;
									m_nDistVal   = nStart - nEnd;
								}

			INT32				GetCalcValue(void)	const	{
									return ((m_nDistVal * m_nNowTick) / m_nTotalTick) + m_nStartVal;
								}
			bool				Run(INT32 nTick, INT32& nValue) {
									if (m_nNowTick <= 0)		return false;
									m_nNowTick -= nTick;
									if (m_nNowTick < 0) {
										m_nNowTick = 0;
									}
									nValue = GetCalcValue();
									return true;
								}
	};

  protected:
			UINT32				m_dwNumber;					//!< �{�C�X�i���o�[
			UINT32				m_dwUnique;					//!< ���ʂh�c
			bool				m_bFinishRequest;			//!< �t�F�[�h��I���v��
			bool				m_bPauseRequest;			//!< �t�F�[�h��|�[�Y�v��
			bool				m_bPause;					//!< �|�[�Y���t���O
			bool				m_bVolumeFade;				//!< �{�����[���t�F�[�h���t���O

			CFadeValue			m_cVolume;					//!< �{�����[��
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
			CFadeValue			m_cSubVolume[NUM_OF_SUB_VOLUME];	//!< �T�u�{�����[��
#endif
			CFadeValue			m_cPanpot;					//!< �p���|�b�g

	static	INT32				m_snGlobalVol;				//!< �O���[�o���{�����[��

  protected:
			void				AbortFade(void);
			void				InternalFade(INT32 nVolume, INT32 nTick = 0, INT32 nStartVolume = -1);
			INT32				InternalGetVolume(void)	const;
			INT32				CalculateSubVolume(void)	const;
			void				VolumeClamp(INT32& nVolume)	const	{
									nVolume = (nVolume < cnVoiceElm_VolumeMin)? cnVoiceElm_VolumeMin : ((nVolume > cnVoiceElm_VolumeMax)? cnVoiceElm_VolumeMax : nVolume);
								}

	virtual	void				_Release(void) {}
	virtual	void				_SetPan(INT32 nPan) {}
	virtual	void				_SetVolume(INT32 nVolume) {}
	virtual	void				_SetSpeedRate(INT32 nRate) {}
	virtual	void				_Pause(void) {}
	virtual	bool				_Load(const char* pSoundData, UINT32 nDataSize, UINT32 nType) {
									return false;
								}
	virtual	bool				_Load(const TCHAR* pFileName, UINT32 nType, UINT32 nOffset, UINT32 nDataSize) {
									return false;
								}
	virtual	bool				_IsPlaying(void)	const	{
									return false;
								}
	virtual	bool				_IsFinished(void)	const	{
									return true;
								}
	virtual	void				_Play(UINT32 nLoop, UINT32 nLoopInSample = 0, UINT32 nStartPos = 0) {}
	virtual	void				_Stop(void) {}

  public:
								CVoiceElement(void);
								//!	�f�X�g���N�^
	virtual						~CVoiceElement(void) {}

			void				Run(INT32 nTick);

								//! �{�C�X�i���o�[�̐ݒ�
			void				SetNumber(UINT32 dwNumber) {
									m_dwNumber = dwNumber;
								}
								//! �{�C�X�i���o�[�̎擾
			UINT32				GetNumber(void)	const	{
									return m_dwNumber;
								}

								//! ����ID�̐ݒ�
			void				SetUnique(UINT32 dwUnique) {
									m_dwUnique = dwUnique;
								}
								//! ����ID�̎擾
			UINT32				GetUnique(void)	const	{
									return m_dwUnique;
								}

								//!	�f�[�^�̍쐬
								/**
								 *	�f�[�^�̍쐬���s���܂��B<br>
								 */
			bool				Load(const char* pSoundData, UINT32 nDataSize, UINT32 nFlag) {
									Release();
									return _Load(pSoundData, nDataSize, nFlag);
								}
								//!	�f�[�^�̍쐬
								/**
								 *	�f�[�^�̍쐬���s���܂��B<br>
								 */
			bool				Load(const TCHAR* pFileName, UINT32 nFlag, UINT32 nOffset, UINT32 nDataSize) {
									Release();
									return _Load(pFileName, nFlag, nOffset, nDataSize);
								}
			void				Release(void);

								//! �Đ������ǂ����`�F�b�N
								/**
								 *	�Đ������ǂ������擾���܂��B<br>
								 *
								 *	@return	�Đ����Ȃ�true
								 */
			bool				IsPlaying(void)	const
								{
									return _IsPlaying();
								}
								//!	��~�����`�F�b�N
								/**
								 *	��~�������������ǂ������擾���܂��B<br>
								 *
								 *	@return	��~�����Ȃ�true
								 */
			bool				IsFinished(void)	const;
								//!	�t�F�[�h�����ǂ����`�F�b�N
								/**
								 *	�t�F�[�h�����ǂ������擾���܂��B<br>
								 *
								 *	@return	�t�F�[�h���Ȃ�true
								 */
			bool				IsFade(void)	const	{
									return m_cVolume.IsFade();
								}
								//!	�|�[�Y�����ǂ����`�F�b�N
								/**
								 *	�|�[�Y�����ǂ������擾���܂��B<br>
								 *
								 *	@return	�|�[�Y���Ȃ�true
								 */
			bool				IsPause(void)	const	{
									return m_bPause;
								}

								//!	�}�X�^�[�{�����[���̔��f
								/**
								 *	�}�X�^�[�{�����[���𑦎����f�����܂��B<br>
								 */
			void				ReflectMasterVolume(void) {
									if (IsFade() == false) {
										SetVolume(-1);
									}
								}
			INT32				GetVolume(void)	const;
			void				SetVolume(INT32 nVolume, INT32 nTick = 0, INT32 nStartVolume = -1);
			INT32				GetSubVolume(UINT32 nChannel)	const;
			void				SetSubVolume(UINT32 nChannel, INT32 nVolume, INT32 nTick = 0, INT32 nStartVolume = -1);

			INT32				GetPan(void)	const;
			void				SetPan(INT32 nPan, INT32 nTick = 0, INT32 nStartPan = -(cnVoiceElm_PanpotHalfRange + 1));

			INT32				GetSpeedRate(void)	const	{
									return 4096;
								}
			void				SetSpeedRate(INT32 nRate) {
									_SetSpeedRate(nRate);
								}

			void				Play(UINT32 dwLoop = 0, INT32 nVolume = -1, INT32 nFadeInTick = 0, UINT32 nLoopInSample = 0);
			void				Stop(INT32 nFadeOutTick = 0);
			void				Pause(bool bPause, INT32 nFadeTick = 0);

	static	void				SetGlobalVolume(INT32 nVolume) {
									m_snGlobalVol = (nVolume < cnVoiceElm_VolumeMin)? cnVoiceElm_VolumeMin : ((nVolume > cnVoiceElm_VolumeMax)? cnVoiceElm_VolumeMax : nVolume);
								}
};


#endif		// __VOICEELEMENT_H__
/*  Bottom of VoiceElement.h  */
