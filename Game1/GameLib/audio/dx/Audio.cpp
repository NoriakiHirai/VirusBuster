#pragma warning(disable:4996)

#include "Audio.h"
#include <atlstr.h>
#include <utility/StringHelper.h>

DirectAudio::DirectAudio()
	:pDSound(nullptr)
{
}

DirectAudio::~DirectAudio()
{
	// AudioRack���̃o�b�t�@�f�[�^�͊e�V�[���̏I�����ɍ폜����Ă���O��
	// pDSound�̂ݍ폜����
	this->pDSound->Release();
}

HRESULT DirectAudio::Initialize(HWND hWnd)
{
	// DirectSound�I�u�W�F�N�g�𐶐�
	if (FAILED(DirectSoundCreate8(NULL, &this->pDSound, NULL)))
	{
		MessageBox(0, CString("�G���["), CString("�T�E���h�J�[�h���̏Ⴕ�Ă���Ȃǂ̌������l�����܂��B"), MB_OK);
		return E_FAIL;
	}	
	// �������x���̐ݒ�
	if (FAILED(this->pDSound->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE)))
	{
		MessageBox(0, CString("�������x���ݒ莸�s"), CString(""), MB_OK);
		return E_FAIL;
	}
	return S_OK;
}
HRESULT DirectAudio::LoadAudio(std::string audioFile, std::string audioName)
{
	HMMIO hMmio = NULL;//Windows�}���`���f�B�AAPI�̃n���h��
	DWORD dwWavSize = 0;//WAV�t�@�C����WAV�f�[�^�̃T�C�Y
	WAVEFORMATEX* pwfex;//WAV�̃t�H�[�}�b�g
	MMCKINFO ckInfo;//�@�`�����N���
	MMCKINFO riffckInfo = {};// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveForm;
	LPDIRECTSOUNDBUFFER pDSBuffer;

	//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
#ifdef UNICODE
	wchar_t* wcs = nullptr;
	size_t mbslen;
	// caluculate length
	mbslen = mbstowcs(nullptr, audioFile.c_str(), 0);
	if (mbslen == (size_t)-1) {
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}
	// allocate
	wcs = (wchar_t*)calloc(mbslen + 1, sizeof(wchar_t));
	if (wcs == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	// convert
	if (mbstowcs(wcs, audioFile.c_str(), mbslen + 1) == (size_t)-1) {
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}
	LPWSTR str = wcs;
#else
	LPSTR str = strdup(audioFile.c_str());
#endif // UNICODE
	hMmio = mmioOpen(str, NULL, MMIO_ALLOCBUF | MMIO_READ);

	//�t�@�C���|�C���^��RIFF�`�����N�̐擪�iRIFF�`�����N�w�b�_�[�j�ɃZ�b�g����
	if ((MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, 0)))
	{
		MessageBox(0, CString("�ŏ��̃`�����N�i�ŏ㕔�`�����N)���ǂݎ��܂���ł���"), CString(""), MB_OK);
		return E_FAIL;
	}
	//�`�����N���ɂ��WAV�t�@�C�����ǂ����m�F����
	if ((riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
	{
		MessageBox(0, CString("�����WAV�t�@�C���ł͂���܂���"), CString(" lol "), MB_OK);
		return E_FAIL;
	}
	// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK))
	{
		MessageBox(0, CString("fmt �`�����N��������܂���"), CString(""), MB_OK);
		return E_FAIL;
	}
	//�t�H�[�}�b�g��ǂݍ���
	if (mmioRead(hMmio, (HPSTR)& pcmWaveForm,
		sizeof(pcmWaveForm)) != sizeof(pcmWaveForm))
	{
		MessageBox(0, CString("WAV�t�H�[�}�b�g�̓ǂݍ��ݎ��s"), CString(""), MB_OK);
	}
	//���j�APCM���ǂ������f
	//wFormatTag���`�F�b�N����
	if (pcmWaveForm.wf.wFormatTag == WAVE_FORMAT_PCM)
	{
		pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if (NULL == pwfex)
		{
			return E_FAIL;
		}
		memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
		pwfex->cbSize = 0;
	}
	else
	{
		MessageBox(0, CString("�t�H�[�}�b�g�����j�APCM�ł͂���܂���"), CString(""), MB_OK);
		return E_FAIL;
	}
	if (MMSYSERR_NOERROR != mmioAscend(hMmio, &ckInfo, 0))
	{
		delete[] pwfex;
		return E_FAIL;
	}
	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK))
	{
		MessageBox(0, CString("data�`�����N��������܂���"), CString(""), MB_OK);
		return E_FAIL;
	}
	dwWavSize = ckInfo.cksize;
	// DirectSound�Z�J���_���[�o�b�t�@�[�쐬
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwWavSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = pwfex;
	if (FAILED(this->pDSound->CreateSoundBuffer(&dsbd, &pDSBuffer, NULL)))
	{
		MessageBox(0, CString("�Z�J���_���o�b�t�@�[�쐬���s"), CString(""), MB_OK);
		return E_FAIL;
	}
	delete[] pwfex;
	//�g�`�f�[�^���������ޏ����Ƃ��āA�Z�J���_���[�o�b�t�@�[�����b�N����
	VOID* pBuffer = NULL;
	DWORD dwBufferSize = 0;
	if (FAILED(pDSBuffer->Lock(0, dwWavSize, &pBuffer, &dwBufferSize, NULL, NULL, 0)))
	{
		return E_FAIL;
	}
	//���b�N�����o�b�t�@�[�T�C�Y�����ۂ̔g�`�f�[�^���傫���ꍇ�A�N���b�V���h�~�B
	DWORD dwSize = dwBufferSize;
	if (dwSize > ckInfo.cksize)
	{
		// ���ۂ̔g�`�f�[�^�T�C�Y�ɍ��킹��
		dwSize = ckInfo.cksize;
	}
	//�o�b�t�@�[�ɉ��f�[�^����������   
	FILE* fp = fopen(audioFile.c_str(), "rb");
	// �g�`�f�[�^�̐擪�܂Ń|�C���^���ړ�
	fseek(fp, riffckInfo.dwDataOffset + sizeof(FOURCC), SEEK_SET);
	BYTE* pWavData = new BYTE[dwSize];
	fread(pWavData, 1, dwSize, fp);//�܂��́A�ꎞ�I�ȏꏊ(pWavData)�ɔg�`�f�[�^��ǂݍ���
	for (DWORD i = 0; i < dwSize; i++)
	{
		*((BYTE*)pBuffer + i) = *((BYTE*)pWavData + i); //�ꎞ�I�ȏꏊ�̔g�`�f�[�^���Z�J���_���o�b�t�@�[�ɃR�s�[����
	}
	fclose(fp);
	delete[] pWavData;//�ꎞ�I�Ȕg�`�f�[�^���J������
	pDSBuffer->Unlock(pBuffer, dwBufferSize, NULL, 0);
	this->audioRack[audioName] = pDSBuffer;
	
	return S_OK;
}

void DirectAudio::UnloadAudio(std::string audioName)
{
	// ���݃`�F�b�N
	if (audioRack.find(audioName) == audioRack.end()) return;
	audioRack[audioName]->Release();
}

void DirectAudio::PlayLoop(std::string audioName)
{
	// ���݃`�F�b�N
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Play(NULL, 0, DSBPLAY_LOOPING);
}

void DirectAudio::PlayOneShot(std::string audioName)
{
	// ���݃`�F�b�N
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Play(NULL, 0, NULL);
}

void DirectAudio::Stop(std::string audioName)
{
	// ���݃`�F�b�N
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Stop();
}

void DirectAudio::Play(std::string audioName, bool isWait)
{
	// ���݃`�F�b�N
	if (this->audioRack.count(audioName) == 0) return;

	// �Đ������`�F�b�N
	DWORD pStatus;
	this->audioRack[audioName]->GetStatus(&pStatus);
	if (isWait && (pStatus & DSBSTATUS_PLAYING) != false)
	{
		// �ҋ@�Đ��t���O�F�I�� ���� �Đ����ł���΁A���^�[��
		return;
	}

	// �Đ�
	this->audioRack[audioName]->SetCurrentPosition(0);
	this->audioRack[audioName]->Play(NULL, 0, NULL);
}
