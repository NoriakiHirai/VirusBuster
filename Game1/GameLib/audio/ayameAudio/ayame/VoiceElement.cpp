/**
 *  @file		VoiceElement.cpp
 *  @brief		�P��̃T�E���h�Ǘ��N���X
 */

// �C���N���[�h
#include	"VoiceElement.h"


INT32			CVoiceElement::m_snGlobalVol = cnVoiceElm_VolumeMax;


//!	�R���X�g���N�^
CVoiceElement::CVoiceElement(void)
{
	m_dwNumber       = 0;
	m_dwUnique       = 0;
	m_bFinishRequest = false;
	m_bPauseRequest  = false;
	m_bPause         = false;
	m_bVolumeFade    = false;
	m_cVolume.SetNowValue(cnVoiceElm_VolumeMax);
	m_cPanpot.SetNowValue(cnVoiceElm_PanpotCenter);
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
	for (int i = 0; i < NUM_OF_SUB_VOLUME; i++) {
		m_cSubVolume[i].SetNowValue(cnVoiceElm_VolumeMax);
	}
#endif
}


//!	��Ꮘ��
/**
 *	���t���[���s�������ł��B<br>
 *	���܂�Ԋu���󂩂Ȃ��悤�Ăяo���Ă��������B<br>
 *
 *	@param	nSyncTick	[in] �O�񂩂�̌o�ߎ���[tick]
 */
void CVoiceElement::Run(INT32 nSyncTick)
{
	INT32	nValue;

	// �I�[�g�p������
	if (m_cPanpot.Run(nSyncTick, nValue) == true) {
		m_cPanpot.SetNowValue(nValue);
		_SetPan(nValue);
	}

	INT32	nSubVol = cnVoiceElm_VolumeMax;
	bool	bSubChange = false;
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
	// �T�u�{�����[���t�F�[�h����
	for (int i = 0; i < NUM_OF_SUB_VOLUME; i++) {
		if (m_cSubVolume[i].Run(nSyncTick, nValue) == true) {
			nSubVol *= nValue;
			nSubVol /= cnVoiceElm_VolumeRange;
			m_cSubVolume[i].SetNowValue(nValue);
			bSubChange = true;
		}
	}
#endif

	// ���C���{�����[���t�F�[�h����
	if (m_cVolume.Run(nSyncTick, nValue) == true) {
		_SetVolume(nValue * m_snGlobalVol * nSubVol / (cnVoiceElm_VolumeRange * cnVoiceElm_VolumeRange));
		if (m_bVolumeFade == true) {
			m_cVolume.SetNowValue(nValue);
		}
		if (m_cVolume.IsFade() == false) {
			// �t�F�[�h�I��
			if (m_bFinishRequest == true) {
				Stop();
			} else if (m_bPauseRequest == true) {
				_Pause();
				m_bPauseRequest = false;
				m_bPause        = true;
			}
			m_bVolumeFade = false;
		}
	} else {
		if (bSubChange != false) {
			_SetVolume(nValue * m_snGlobalVol * nSubVol / (cnVoiceElm_VolumeRange * cnVoiceElm_VolumeRange));
		}
	}
}


//!	�f�[�^�̉��
/**
 *	�f�[�^�̉�����s���܂��B<br>
 */
void CVoiceElement::Release(void)
{
	Stop();
	_Release();
}


//!	�Đ��I���`�F�b�N
/**
 *	�Đ����I�����Ă��邩�ǂ������擾���܂��B<br>
 *
 *	@return	�I�����Ă���ꍇ��true��Ԃ�
 */
bool CVoiceElement::IsFinished(void)	const
{
	if (_IsFinished() == true) {
		return true;
	}
	if (m_bFinishRequest == true && IsFade() == false) {
		return true;
	}

	return false;
}


//!	�t�F�[�h�̒��f
void	CVoiceElement::AbortFade(void)
{
	if (IsFade() == true) {
		m_cVolume.ResetTime();
		m_bVolumeFade = false;
	}
}


void	CVoiceElement::InternalFade(INT32 nVolume, INT32 nTick, INT32 nStartVolume)
{
	INT32		nSave = m_cVolume.GetNowValue();
	SetVolume(nVolume, nTick, nStartVolume);
	m_cVolume.SetNowValue(nSave);
	m_bVolumeFade = false;
}


INT32 CVoiceElement::InternalGetVolume(void)	const
{
	return (m_cVolume.IsFade() == true && m_bVolumeFade == false)? m_cVolume.GetCalcValue() : m_cVolume.GetNowValue();
}


INT32 CVoiceElement::CalculateSubVolume(void)	const
{
	INT32	nSubVol = cnVoiceElm_VolumeMax;
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
	for (int i = 0; i < NUM_OF_SUB_VOLUME; i++) {
		nSubVol *= m_cSubVolume[i].GetNowValue() / cnVoiceElm_VolumeRange;
	}
#endif

	return nSubVol;
}


//!	�{�����[���̎擾
/**
 *	���݂̃{�����[�����擾���܂��B<br>
 *
 *	@return	�{�����[���l
 */
INT32 CVoiceElement::GetVolume(void)	const
{
	return m_cVolume.GetNowValue();
}


//!	�{�����[���̐ݒ�
/**
 *	�{�����[���̐ݒ���s���܂��B<br>
 *
 *	@param	nVolume			[in] �{�����[��(0 �` 100)
 *	@param	nTick			[in] �J�ڎ���[tick]
 *	@param	nStartVolume	[in] �X�^�[�g�{�����[��(�ȗ������ꍇ�͌��ݒl���Q��)
 */
void CVoiceElement::SetVolume(INT32 nVolume, INT32 nTick, INT32 nStartVolume)
{
	VolumeClamp(nVolume);
	m_bVolumeFade = false;
	if (nTick > 0) {
		if (nStartVolume < 0) {
			nStartVolume = InternalGetVolume();
		}
		m_cVolume.SetParam(nTick, nStartVolume, nVolume);
		m_bVolumeFade = true;
		nVolume = nStartVolume;
	} else {
		m_cVolume.ResetTime();
	}

	m_cVolume.SetNowValue(nVolume);
	_SetVolume(nVolume * m_snGlobalVol * CalculateSubVolume() / (cnVoiceElm_VolumeRange * cnVoiceElm_VolumeRange));
}


//! �T�u�{�����[���̎擾
/**
 *	�T�u�{�����[���l�̎擾���s���܂��B
 *
 *	@param	nChannel	[in] �T�u�{�����[���`�����l��
 *
 *	@return	�{�����[���l
 */
INT32 CVoiceElement::GetSubVolume(UINT32 nChannel)	const
{
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
	return (nChannel < NUM_OF_SUB_VOLUME)? m_cSubVolume[nChannel].GetNowValue() : cnVoiceElm_VolumeMax;
#else
	return cnVoiceElm_VolumeMax;
#endif
}


//! �T�u�{�����[���̐ݒ�
/**
 *	�T�u�{�����[���l�̐ݒ���s���܂��B
 *
 *	@param	nChannel		[in] �T�u�{�����[���`�����l��
 *	@param	nVolume			[in] �{�����[��
 *	@param	nTick			[in] �J�ڎ���[tick]
 *	@param	nStartVolume	[in] �X�^�[�g�{�����[��(�ȗ������ꍇ�͌��ݒl���Q��)
 */
void CVoiceElement::SetSubVolume(UINT32 nChannel, INT32 nVolume, INT32 nTick, INT32 nStartVolume)
{
#if defined(NUM_OF_SUB_VOLUME) && (NUM_OF_SUB_VOLUME > 0)
	if (nChannel >= NUM_OF_SUB_VOLUME)		return;

	VolumeClamp(nVolume);
	if (nTick > 0) {
		if (nStartVolume < 0) {
			nStartVolume = (m_cSubVolume[nChannel].IsFade() == true)? m_cSubVolume[nChannel].GetCalcValue() : m_cSubVolume[nChannel].GetNowValue();
		}
		m_cSubVolume[nChannel].SetParam(nTick, nStartVolume, nVolume);
		nVolume = nStartVolume;
	} else {
		m_cSubVolume[nChannel].ResetTime();
	}
	m_cSubVolume[nChannel].SetNowValue(nVolume);
	ReflectMasterVolume();
#endif
}


//!	�p���̎擾
/**
 *	���݂̃p�����擾���܂��B<br>
 *
 *	@return	�p���l
 */
INT32 CVoiceElement::GetPan(void)	const
{
	return m_cPanpot.GetNowValue();
}


//!	�p���̐ݒ�
/**
 *	�p���̐ݒ���s���܂��B<br>
 *
 *	@param	nPan		[in] �p��
 *	@param	nTick		[in] �J�ڎ���
 *	@param	nStartPan	[in] �X�^�[�g�p��(�ȗ������ꍇ�͌��ݒl���Q��)
 */
void CVoiceElement::SetPan(INT32 nPan, INT32 nTick, INT32 nStartPan)
{
	nPan = (nPan < cnVoiceElm_PanpotMaxL)? cnVoiceElm_PanpotMaxL : ((nPan > cnVoiceElm_PanpotMaxR)? cnVoiceElm_PanpotMaxR : nPan);
	if (nTick > 0) {
		if (nStartPan < cnVoiceElm_PanpotMaxL || nStartPan > cnVoiceElm_PanpotMaxR) {
			nStartPan = m_cPanpot.GetNowValue();
		}
		m_cPanpot.SetParam(nTick, nStartPan, nPan);
		nPan = nStartPan;
	} else {
		m_cPanpot.ResetTime();
	}
	m_cPanpot.SetNowValue(nPan);
	_SetPan(nPan);
}


//!	�Đ�
/**
 *	�Đ����s���܂��B<br>
 *	nLoop��0���w�肷��ƁA1�񉉑t���ďI�����܂��B<br>
 *	nLoopInSample��0�ȊO�ɐݒ肷��ƁA���[�v����ۂ̃��[�v�J�n�|�C���g��ݒ�ł��܂��B<br>
 *	�P�ʂ̓T���v���ł��B<br>
 *
 *	@param	nLoop			[in] ���[�v��(LOOP_INFINITY�Ŗ������[�v)
 *	@param	nVolume			[in] �{�����[��
 *	@param	nFadeInTick		[in] �t�F�[�h�C������[tick]
 *	@param	nLoopInSample	[in] ���[�v�C���T���v��
 */
void CVoiceElement::Play(UINT32 nLoop, INT32 nVolume, INT32 nFadeInTick, UINT32 nLoopInSample)
{
	Stop();
	SetVolume(nVolume, nFadeInTick, 0);
	m_cVolume.SetNowValue(nVolume);
	m_bVolumeFade = false;
	m_bFinishRequest = false;

	_Play(nLoop, nLoopInSample);
}


//!	��~
/**
 *	��~���܂��B<br>
 *	�t�F�[�h�A�E�g���Ԃ�0�łȂ��ꍇ�A�t�F�[�h�I�����~���܂��B<br>
 *
 *	@param	fFadeOutTime	[in] �t�F�[�h�A�E�g����
 */
void CVoiceElement::Stop(INT32 nFadeOutTick)
{
	m_bPauseRequest = false;
	if (nFadeOutTick > 0 && m_bPause == false) {
		InternalFade(0, nFadeOutTick);
		m_bFinishRequest = true;
	} else {
		AbortFade();
		m_bPause = false;
		_Stop();
	}
}


//!	�ꎞ��~
/**
 *	�ꎞ��~���܂��B<br>
 *
 *	@param	bPause		[in] �|�[�Y����ꍇ��true�A�|�[�Y����������ꍇ��false
 *	@param	fFadeTime	[in] �t�F�[�h����
 */
void CVoiceElement::Pause(bool bPause, INT32 nFadeTick)
{
	if (bPause == true) {
		// �|�[�Y����
		if (m_bPauseRequest == false && IsPause() == false) {
			m_bFinishRequest = false;
			if (nFadeTick > 0) {
				InternalFade(0, nFadeTick);
				m_bPauseRequest = true;
			} else {
				AbortFade();
				_Pause();
				m_bPause = true;
			}
		}
	} else {
		// �|�[�Y��������
		if (m_bPauseRequest == true || IsPause() == true) {
			if (IsPause() == true) {
				m_bPause = false;
				InternalFade(m_cVolume.GetNowValue(), nFadeTick, 0);
				_Pause();
			} else {
				InternalFade(m_cVolume.GetNowValue(), nFadeTick);
			}
			m_bPauseRequest = false;
		}
	}
}


/*  Bottom of VoiceElement.cpp  */
