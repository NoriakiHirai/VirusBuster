_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
�y�^�C�g���z �T�E���h�h���C�o�[ Ayame
�y�t�@�C���z Ayame.zip
�y�쐬�����z 2003/07/25 - 2014/08/30
�y�J�e�S���z �T�E���h�h���C�o�[
�y�� �� �ҁz �����񂽁i����F�t���`�j
�y������z Windows2000�ȍ~
�y�z�z�`�ԁz �t���[�E�F�A
�y E-Mail �z sazanta@wb3.so-net.ne.jp
�yHomePage�z http://ayame-lib.blog.so-net.ne.jp
�y�]�@�@�ځz ���R
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

��������
�@Ayame��
�@�@�����kPCM Wave�t�@�C��
�@�@OggVorbis�t�@�C��
�@�@���kWave�t�@�C��
�@�@MP3�t�@�C��
�@�̍Đ����s���ׂ�Windows�p�T�E���h�h���C�o�[�݂����Ȃ��̂ł��B
�@MIDI�t�@�C���͍Đ��ł��܂���B

�@AyameS.dll�@�@�|�@�����kPCM Wave�t�@�C���̂ݍĐ��ł���ŏ��\���łł��B

�@Ayame.dll�@�@ �|�@��ʓI��DLL�ł��B
�@�@�@�@�@�@�@�@�@�@�Ή����Ă���S�Ă̌`�����Đ��\�ł��B

�@AyameSSE.dll�@�|�@SSE2�Ή���DLL�ł��B
�@�@�@�@�@�@�@�@�@�@�Ή����Ă���S�Ă̌`�����Đ��\�ł��B

�@AyameCLR.dll�@�|�@Ayame.dll��.NET2.0�ȍ~����g�p���邽�߂̃��b�p�[DLL�ł��B

  Ayame�ɂ�ShiftJIS�ł�Unicode�ł�2��ނ�����܂��B


���g�p�@��
�@�T���v�����Q�l�ɂ��Ă��������B
�@AyameTest1.exe
�@�@WindowsAPI�������g�p�����Đ��A�v���P�[�V����

�@AyameTest2.exe
�@�@Windows .NET2.0��C#���g�p�����Đ��A�v���P�[�V����


���@�\��
  DWORD Ayame_GetVersion( void )
�@�@DLL�o�[�W�����̎擾
      �o�[�W������16�i���ŕ\�����ꍇ�A0xAAAABBCC�ƂȂ�A
      AAAA�̓��W���[�o�[�W����
      BB�̓}�C�i�[�o�[�W����
      CC�̓��r�W������\���܂��B
      ���W���[�o�[�W�����E�}�C�i�[�o�[�W�������قȂ�ꍇ�A�݊������Ȃ��ꍇ������܂��B

  void Ayame_DirectSoundEnumerate( LPDSENUMCALLBACK callback, LPVOID  lpContext )
�@�@�T�E���h�f�o�C�X�̗�
�@�@�@�g�p�ł���T�E���h�f�o�C�X��񋓂��܂��B
�@�@�@�����Ŏ擾�ł����f�o�C�X��GUID��Ayame_Initialize���Ɏw�肷�邱�Ƃɂ��A
�@�@�@�Đ��f�o�C�X��I�����邱�Ƃ��ł��܂��B

�@bool Ayame_Initialize( HWND hWnd, void **ppDirectSound, LPCGUID lpcGuid = NULL )
�@�@�h���C�o�[������
�@�@�@�g�p���������͂ǂ�����Е��ł���A
�@�@�@hWnd��ppDirectSound��NULL�̎��ł���A
�@�@�@������DirectSound������������K�v������Ƃ��B
�@�@�@�O����DirectSound�����łɐ����ς݂̏ꍇ��
�@�@�@ppDrectSound�ɓn�����Ƃł��ꂪ�g����B

�@void Ayame_Uninitialize( void )
�@�@�h���C�o�[�I���B
�@�@�@�����Ŏg�p���Ă��郁�����̉���Ȃǂ��s���B
�@�@�@�����A���邢�͎擾����DirectSound�I�u�W�F�N�g��
�@�@�@���̊֐����ŉ�������B

�@bool Ayame_GetDirectSoundCaps( DSCAPS *Caps )
�@�@DirectSound��Caps�̎擾

�@CAyame *Ayame_CreateInstanceFromFile( const TCHAR *pFileName, unsigned long Start, unsigned long Size, unsigned long Flag )
�@�@�h���C�o�[�̃C���^�[�t�F�[�X�ł���CAyame�N���X���擾���邽�߂̊֐��B
�@�@���[�J���h���C�u�ɂ���T�E���h�t�@�C�����Đ����邽�߂̊֐��ł��B

�@�@pFileName
�@�@�@�ǂݍ��݃t�@�C����

�@�@Start
�@�@�@�t�@�C�����J�nByte

�@�@Size
�@�@�@�t�@�C�����T�C�YByte

�@�@Flag
�@�@�@AYAME_LOADFLAG_GLOBAL���w�肷�邱�ƂŃE�B���h�E����A�N�e�B�u���ł��Đ�����܂��B
�@�@�@AYAME_LOADFLAG_STREAM���w�肷�邱�ƂŃX�g���[�������ɂ��Đ����s���܂��B
�@�@�@AYAME_LOADFLAG_STATIC���w�肷�邱�Ƃňꊇ�Đ������ɂ��Đ����s���܂��B

�@CAyame *Ayame_CreateInstanceFromMemory( void *pData, unsigned long Size, unsigned long Flag )
�@�@�h���C�o�[�̃C���^�[�t�F�[�X�ł���CAyame�N���X���擾���邽�߂̊֐��B
�@�@��������ɂ���T�E���h�f�[�^���Đ����邽�߂̊֐��ł��B

�@�@pData
�@�@�@�ǂݍ��݃f�[�^

�@�@Size
�@�@�@�ǂݍ��݃f�[�^�T�C�Y

�@�@Flag
�@�@�@AYAME_LOADFLAG_GLOBAL���w�肷�邱�ƂŃE�B���h�E����A�N�e�B�u���ł��Đ�����܂��B
�@�@�@AYAME_LOADFLAG_STREAM���w�肷�邱�ƂŃX�g���[�������ɂ��Đ����s���܂��B
�@�@�@AYAME_LOADFLAG_STATIC���w�肷�邱�Ƃňꊇ�Đ������ɂ��Đ����s���܂��B

�@CAyame *Ayame_CreateInstanceFromUrl( const TCHAR *pUrl, unsigned long Start, unsigned long Size, unsigned long Flag )
�@�@�h���C�o�[�̃C���^�[�t�F�[�X�ł���CAyame�N���X���擾���邽�߂̊֐��B
�@�@�C���^�[�l�b�g��ɂ���T�E���h�t�@�C�����Đ����邽�߂̊֐��ł��B

�@�@pUrl
�@�@�@�ǂݍ���URL��

�@�@Start
�@�@�@�t�@�C�����J�nByte

�@�@Size
�@�@�@�ǂݍ��݃f�[�^�T�C�Y

�@�@Flag
�@�@�@AYAME_LOADFLAG_GLOBAL���w�肷�邱�ƂŃE�B���h�E����A�N�e�B�u���ł��Đ�����܂��B
�@�@�@AYAME_LOADFLAG_STREAM���w�肷�邱�ƂŃX�g���[�������ɂ��Đ����s���܂��B
�@�@�@AYAME_LOADFLAG_STATIC���w�肷�邱�Ƃňꊇ�Đ������ɂ��Đ����s���܂��B

�@bool Ayame_GetLastError( TCHAR *pErrStr, unsigned long Size )
�@�@�Ō�ɔ��������G���[���擾����

�@�@pErrStr
�@�@�@�G���[������i�[��

�@�@Size
�@�@�@�i�[�o�b�t�@�T�C�Y


�������Ӂ�
�@���̃v���O������p���邱�Ƃɂ���ĉ��炩�̔�Q�A�������M���ɂ������Ƃ��Ă��A
�@�����͈�ؐӔC�𕉂��܂���̂ł��������������B
�@���kWav�t�@�C�������MP3�t�@�C���̃f�R�[�h�́AWindows��ACM(Audio Compression Manager)���g�p���Ă��܂��B
�@���̂��߃f�R�[�_�[���\�߃C���X�g�[������Ă��Ȃ����ł́A�f�R�[�h���s�����Đ��ł��܂���B


���Ĕz�z��
�@�Ĕz�z�Ȃǂ͎��R�ɍs���Ă��������Ă��܂��܂���B
�@�܂��Ĕz�z���s���ۂɎ��̖��O���h�L�������g�Ȃǂɖ��L����K�v�͂���܂���B
�@���O����ɂ�����炸���ւ̘A���Ȃǂ���ؕK�v����܂���B


���ӎ���
�@���kWave����̏����́AAKIYAMA Kouhei����̏����Q�l�ɂ����Ă��������܂����B


�����C�Z���X��
�@Ogg, Vorbis, Xiph.org Foundation and their logos are trademarks (tm) of the Xiph.org Foundation.
�@Vox uses their SDK, especially ogg_static.lib vorbis_static.lib vorbisfile_static.lib
�@These library are copyright (C) 1994-2001 Xiph.org. All rights reserved.


���X�V������
�@2014.08.30 : phase 6.0.0
�@�@LoadDLL()�֐���Initialize()�֐����Ă΂Ȃ��悤�C���B
�@�@�@����ɂ�薾���I��Initialize()�֐����ĂԕK�v������܂��B
�@�@SoundDeviceEnumerate()�֐���ǉ��B
�@�@Uninitialize()�֐���ǉ��B
�@�@5.x.x�Ƃ̌݊����͂���܂���B

�@2008.10.27 : phase 5.3.0
�@�@GetVersion()�֐���ǉ��B
�@�@GetDirectSoundCaps()�֐���ǉ��B
�@�@5.2.0�Ƃ̌݊����͂���܂���B

�@2008.10.08 : phase 5.2.0
�@�@�]���̃X�g���[�~���O�Đ��ɉ����A�ꊇ�Đ��ɑΉ��B
�@�@�ꕔAPI�̈�����ύX��������5.10�Ƃ̌݊����͂���܂���B
�@�@Play()�ɍĐ��J�n�ʒu���w��ł���悤�@�\�ǉ��B

�@2008.10.03 : phase 5.1.0
�@�@unicode�ɑΉ��B

�@2006.10.10 : phase 5.05
�@�@SSE�Ή��ł�DLL���쐬�B
�@�@SSE�Ή��ł�SSE2�̋@�\���g���Ă���̂�Pentium4/Athlon64�ȍ~��CPU�łȂ��Ɠ��삵�܂���B

�@2006.08.21 : phase 5.04
�@�@COM�̏����������Ă��Ȃ��̂ɉ���������s���ꍇ���������s��̏C���B

�@2006.07.24 : phase 5.03
�@�@ogg/vorbis�̃��C�u�������ŐV�łɂ��ă��r���h�B

�@2005.08.23 : phase 5.02
�@�@�Đ����̃t�@�C���̃t�@�C���|�C���^���O���ŕύX���ꂽ�ꍇ�ɁA
�@�@�Đ����������s���Ȃ��Ȃ�s����C���B

�@2005.08.08 : phase 5.01
�@�@8BitWave���Ƀm�C�Y������s����C���B

�@2005.08.05 : phase 5.00
�@�@Subversion�ɂ��I�[�v���\�[�X���B
�@�@���kWave�ɑΉ��B
�@�@����������啝�ɕύX�B
�@�@�Đ����Ƀm�C�Y��������������B

�@2005.07.28 : phase 4.06
�@�@�T�E���h�o�b�t�@�̍X�V��1/4�b�P�ʂɍs���悤�ɂ��܂����B
�@�@��~���ɃT�E���h�o�b�t�@���N���A����悤�ɂ��܂����B

�@2005.06.29 : phase 4.04
�@�@�T�E���h�o�b�t�@�̍X�V��1�b����1/2�b�ɕύX�B
�@�@�C�x���g�̐���16����32�Ɋg���B

�@2005.06.28 : phase 4.03
�@�@Ayame_CreateInstanceFromHandle()�֐���
�@�@�ǂݍ��񂾃t�@�C�����������Đ�����Ȃ��s����C���B

�@2005.06.27 : phase 4.02
�@�@���񃋁[�v�Đ����ɂQ�b�ȉ��̃T�E���h�̎��A�Đ���኱�̃E�F�C�g������s��̏C���B
�@�@��A�N�e�B�u�E�B���h�E���Đ��̏����̕s����C���B

�@2005.06.26 : phase 4.01
�@�@Wave�Đ����ɍŌ�Ƀm�C�Y������s��̏C���B
�@�@Wave�Đ����Ƀ��[�v�������������s���Ȃ��s��̏C���B

�@2005.06.25 : phase 4.00
�@�@BCC�^GCC�ŗ�����s����C���B
�@�@OggVorbis�����łȂ������kWave�̍Đ��ɂ��Ή��B
�@�@���[�h���̃u���b�N���ɘa�B
�@�@�I���������Đ��@�\���폜�B
�@�@��A�N�e�B�u���̍Đ��ɑΉ��B

�@2005.06.13 : phase 3.70
�@�@�R���p�C���̈Ⴂ���z�����邽�߂Ƀt�@�C������֌W���R�[���o�b�N�֐��`���ɕύX�B
�@�@�t�@�C���̃T�C�Y�����̕����̕s����C���B

�@2005.05.16 : phase 3.50
�@�@2�b�ȉ��̃T�E���h�������I�ɔ�X�g���[�~���O�Đ����s���悤�ɕύX�B

�@2004.11.27 : phase 3.40
�@�@��X�g���[�~���O�Đ������ɑΉ��B
�@�@SE��{�C�X�Ȃǂ̒Z�������̏ꍇ�̓X�g���[�~���O
�@�@�����Ȃ��ق��������������ꍇ�������ł��B

�@2004.10.27 : phase 3.10
�@�@�t�@�C���n���h���ƊJ�n�A�h���X�A�T�C�Y�������ɂ���
�@�@�C���X�^���X�𐶐�����֐���ǉ��B
�@�@�R���ɂ�胁���������������ɃX�g���[�~���O�Đ����\�ɂȂ�܂��B

�@2004.09.21 : phase 3.01
�@�@�Đ������ǂ����擾���邽�߂�IsPlay()�֐���ǉ��B

�@2004.06.24 : phase 3.00
�@�@������������V�B�Đ��̓X�g���[�~���O�݂̂ɖ߂��A
�@�@�Z�����čĐ��ł��Ȃ��t�@�C�����͂����悤�ɕύX�B

�@2004.06.21 : phase 2.00
�@�@�\���ύX�ɔ����A�f�R�[�h�f�[�^��S�ă��������ɓW�J����悤�ɕύX�B
�@�@��ɁA�X�g���[�~���O�Đ��Ƃ̑I�����ɏo����悤�ɏC�����Ă����܂��B
�@�@�܂�����̃o�[�W�����ł̓���������̓ǂݍ��݂��o���Ȃ��Ȃ��Ă��܂��B

�@2004.02.05 : phase 1.50
�@�@����������̓ǂݍ��݋@�\���T�|�[�g�B
�@�@����ɔ����A�t�@�C������̓ǂݍ��݂��A
�@�@��x���������ɓW�J���Ă���Đ�����悤��
�@�@�ύX���܂����B
�@�@�܂����������ɓW�J�����̂͌��̃t�@�C���ł���
�@�@�f�R�[�h��̃f�[�^������������
�@�@�W�J�����킯�ł͂���܂���B

�@2003.10.07 : phase 1.40
�@�@DirectSound8���g���Ӗ����Ȃ��̂ŁA
�@�@DirectSound�ł̍Đ��ɕύX�B
�@�@�����DirectX�̃o�[�W�������C�ɂ��Ȃ��Ă�
�@�@�悭�Ȃ����Ǝv���܂��B

�@2003.10.05 : phase 1.30
�@�@����̃^�C�~���O�Ńo�b�t�@�s����������
�@�@�������G���[�ŗ�����o�O���C���B

�@2003.08.08 : phase 1.20
�@�@�P�b���Ƃɂ��������Ȃ�ꍇ������̂��C���B

�@2003.07.28 : phase 1.10
�@�@�X���b�h�֘A�̏������܂��Ƃɂ����B

�@2003.07.25 : phase 1.00
�@�@�Ƃ肠�������J���Ă��ƂŁB
