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
	// AudioRack内のバッファデータは各シーンの終了時に削除されている前提
	// pDSoundのみ削除する
	this->pDSound->Release();
}

HRESULT DirectAudio::Initialize(HWND hWnd)
{
	// DirectSoundオブジェクトを生成
	if (FAILED(DirectSoundCreate8(NULL, &this->pDSound, NULL)))
	{
		MessageBox(0, CString("エラー"), CString("サウンドカードが故障しているなどの原因が考えられます。"), MB_OK);
		return E_FAIL;
	}	
	// 協調レベルの設定
	if (FAILED(this->pDSound->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE)))
	{
		MessageBox(0, CString("協調レベル設定失敗"), CString(""), MB_OK);
		return E_FAIL;
	}
	return S_OK;
}
HRESULT DirectAudio::LoadAudio(std::string audioFile, std::string audioName)
{
	HMMIO hMmio = NULL;//WindowsマルチメディアAPIのハンドル
	DWORD dwWavSize = 0;//WAVファイル内WAVデータのサイズ
	WAVEFORMATEX* pwfex;//WAVのフォーマット
	MMCKINFO ckInfo;//　チャンク情報
	MMCKINFO riffckInfo = {};// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT pcmWaveForm;
	LPDIRECTSOUNDBUFFER pDSBuffer;

	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
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

	//ファイルポインタをRIFFチャンクの先頭（RIFFチャンクヘッダー）にセットする
	if ((MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, 0)))
	{
		MessageBox(0, CString("最初のチャンク（最上部チャンク)が読み取れませんでした"), CString(""), MB_OK);
		return E_FAIL;
	}
	//チャンク情報によりWAVファイルかどうか確認する
	if ((riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
	{
		MessageBox(0, CString("これはWAVファイルではありません"), CString(" lol "), MB_OK);
		return E_FAIL;
	}
	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK))
	{
		MessageBox(0, CString("fmt チャンクが見つかりません"), CString(""), MB_OK);
		return E_FAIL;
	}
	//フォーマットを読み込む
	if (mmioRead(hMmio, (HPSTR)& pcmWaveForm,
		sizeof(pcmWaveForm)) != sizeof(pcmWaveForm))
	{
		MessageBox(0, CString("WAVフォーマットの読み込み失敗"), CString(""), MB_OK);
	}
	//リニアPCMかどうか判断
	//wFormatTagをチェックする
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
		MessageBox(0, CString("フォーマットがリニアPCMではありません"), CString(""), MB_OK);
		return E_FAIL;
	}
	if (MMSYSERR_NOERROR != mmioAscend(hMmio, &ckInfo, 0))
	{
		delete[] pwfex;
		return E_FAIL;
	}
	// WAVファイル内の音データの読み込み
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK))
	{
		MessageBox(0, CString("dataチャンクが見つかりません"), CString(""), MB_OK);
		return E_FAIL;
	}
	dwWavSize = ckInfo.cksize;
	// DirectSoundセカンダリーバッファー作成
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwWavSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = pwfex;
	if (FAILED(this->pDSound->CreateSoundBuffer(&dsbd, &pDSBuffer, NULL)))
	{
		MessageBox(0, CString("セカンダリバッファー作成失敗"), CString(""), MB_OK);
		return E_FAIL;
	}
	delete[] pwfex;
	//波形データを書き込む準備として、セカンダリーバッファーをロックする
	VOID* pBuffer = NULL;
	DWORD dwBufferSize = 0;
	if (FAILED(pDSBuffer->Lock(0, dwWavSize, &pBuffer, &dwBufferSize, NULL, NULL, 0)))
	{
		return E_FAIL;
	}
	//ロックしたバッファーサイズが実際の波形データより大きい場合、クラッシュ防止。
	DWORD dwSize = dwBufferSize;
	if (dwSize > ckInfo.cksize)
	{
		// 実際の波形データサイズに合わせる
		dwSize = ckInfo.cksize;
	}
	//バッファーに音データを書き込む   
	FILE* fp = fopen(audioFile.c_str(), "rb");
	// 波形データの先頭までポインタを移動
	fseek(fp, riffckInfo.dwDataOffset + sizeof(FOURCC), SEEK_SET);
	BYTE* pWavData = new BYTE[dwSize];
	fread(pWavData, 1, dwSize, fp);//まずは、一時的な場所(pWavData)に波形データを読み込み
	for (DWORD i = 0; i < dwSize; i++)
	{
		*((BYTE*)pBuffer + i) = *((BYTE*)pWavData + i); //一時的な場所の波形データをセカンダリバッファーにコピーする
	}
	fclose(fp);
	delete[] pWavData;//一時的な波形データを開放する
	pDSBuffer->Unlock(pBuffer, dwBufferSize, NULL, 0);
	this->audioRack[audioName] = pDSBuffer;
	
	return S_OK;
}

void DirectAudio::UnloadAudio(std::string audioName)
{
	// 存在チェック
	if (audioRack.find(audioName) == audioRack.end()) return;
	audioRack[audioName]->Release();
}

void DirectAudio::PlayLoop(std::string audioName)
{
	// 存在チェック
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Play(NULL, 0, DSBPLAY_LOOPING);
}

void DirectAudio::PlayOneShot(std::string audioName)
{
	// 存在チェック
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Play(NULL, 0, NULL);
}

void DirectAudio::Stop(std::string audioName)
{
	// 存在チェック
	if (this->audioRack.count(audioName) == 0) return;

	this->audioRack[audioName]->Stop();
}

void DirectAudio::Play(std::string audioName, bool isWait)
{
	// 存在チェック
	if (this->audioRack.count(audioName) == 0) return;

	// 再生中かチェック
	DWORD pStatus;
	this->audioRack[audioName]->GetStatus(&pStatus);
	if (isWait && (pStatus & DSBSTATUS_PLAYING) != false)
	{
		// 待機再生フラグ：オン かつ 再生中であれば、リターン
		return;
	}

	// 再生
	this->audioRack[audioName]->SetCurrentPosition(0);
	this->audioRack[audioName]->Play(NULL, 0, NULL);
}
