/**
*/

#ifndef ___AYAME_H___
#define ___AYAME_H___


//====================================================================
// INCLUDE
//====================================================================
#include <tchar.h>
#include "dsound.h"


//====================================================================
// DEFINE
//====================================================================
#define AYAME_LOOP_INFINITY		(0xFFFFFFFF)			///< �������[�v
#define	AYAME_LOADFLAG_GLOBAL	(1<<0)
#define	AYAME_LOADFLAG_STREAM	(0<<1)
#define	AYAME_LOADFLAG_STATIC	(1<<1)

#define	AYAME_VERSION_MAJOR		6
#define	AYAME_VERSION_MINOR		0
#define	AYAME_VERSION_REVISON	0
#define	AYAME_VERSION			((AYAME_VERSION_MAJOR<<16)|(AYAME_VERSION_MINOR<<8)|(AYAME_VERSION_REVISON))


//====================================================================
// TYPEDEF
//====================================================================
typedef DWORD (*AYAME_GETVERSION)( void );																										///< �o�[�W�����擾
typedef	void (*AYAME_DIRECTSOUNDENUMERATE_PROC)( LPDSENUMCALLBACK callback, LPVOID  lpContext );
typedef bool (*AYAME_INITIALIZE_PROC)( HWND hWnd, void** ppDirectSound, LPCGUID lpcGuid );														///< �h���C�o�[������
typedef void (*AYAME_UNINITIALIZE_PROC)( void );																								///< �h���C�o�[���
typedef bool (*AYAME_GETDIRECTSOUNDCAPS_PROC)( DSCAPS* pCaps );																					///< Caps�擾
typedef bool (*AYAME_GETERROR_PROC)( TCHAR* pErrStr, unsigned long Size );																		///< �G���[�擾
typedef class CAyame* (*AYAME_CREATE_FROM_FILE_PROC)( const TCHAR* pFileName, unsigned long Start, unsigned long Size, unsigned long Flag );	///< �h���C�o�[����I�u�W�F�N�g�擾
typedef class CAyame* (*AYAME_CREATE_FROM_MEMORY_PROC)( const void* pData, unsigned long Size, unsigned long Flag );							///< �h���C�o�[����I�u�W�F�N�g�擾
typedef class CAyame* (*AYAME_CREATE_FROM_URL_PROC)( const TCHAR* pUrl, unsigned long Start, unsigned long Size, unsigned long Flag );			///< �h���C�o�[����I�u�W�F�N�g�擾

//====================================================================
/**
	@brief �Đ��p�N���X

	DLL�p�ɏ������z�֐������̒��g�̂Ȃ��N���X�ł�
*/
//====================================================================
class CAyame
{
  private:
	CAyame& operator = ( const CAyame& cls );

  protected:
	virtual ~CAyame(){}												///< �f�X�g���N�^

  public:
	virtual bool __stdcall IsEmptyQueue( void )				= 0;	///< �L���[�ɓo�^�����邩�ǂ���
	virtual bool __stdcall IsReady( void ) const			= 0;	///< �����������ς݂��H
	virtual bool __stdcall IsNotDecoder( void ) const		= 0;	///< �f�R�[�_�����݂��邩�ǂ����H
	virtual bool __stdcall IsPlayStarted( void ) const		= 0;	///< �Đ��J�n�`�F�b�N
	virtual bool __stdcall IsPlay( void ) const				= 0;	///< �Đ��`�F�b�N
	virtual bool __stdcall IsPause( void ) const			= 0;	///< �ꎞ��~���`�F�b�N
	virtual bool __stdcall Prefetch( void )					= 0;	///< �v���t�F�b�`
	virtual bool __stdcall Predecode( void )				= 0;	///< �v���f�R�[�h
	virtual bool __stdcall Combine( CAyame *pAyame )		= 0;	///< �A��
	virtual bool __stdcall Play( unsigned long nLoopCount = 0, unsigned long nLoopInSample = 0, unsigned long nStartPos = 0 )	= 0;	///< �Đ�
	virtual bool __stdcall Stop( void )						= 0;	///< ��~
	virtual bool __stdcall Pause( void )					= 0;	///< �ꎞ��~
	virtual bool __stdcall SetVolume( float fParam )		= 0;	///< �{�����[���ݒ�
	virtual bool __stdcall SetPan( float fParam )			= 0;	///< �p���ݒ�
	virtual bool __stdcall SetFrequencyRate( float fParam )	= 0;	///< ���g���ݒ�
	virtual bool __stdcall Release( void )					= 0;	///< ���
	virtual bool __stdcall AddRef( void )					= 0;	///< �Q�ƃJ�E���g�C���N�������g
	virtual unsigned long __stdcall GetPlayingPoint( void )	= 0;	///< �����悻�̍Đ��ʒu�̎擾
	virtual unsigned long __stdcall GetTotalSample( void )	= 0;	///< �g�[�^���̃T���v�����̎擾
};


#endif // ___AYAME_H___
