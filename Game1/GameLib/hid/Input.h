#ifndef GAME_LIB_GAME_LIB_INPUT_H_
#define GAME_LIB_GAME_LIB_INPUT_H_

// DIRECTINPUT_VERSION���w�肵�Ă����Ȃ��ƁA�x������������̂ŗ�O�I��
// �C���N���[�h�錾�̑O�Ń}�N���錾�����Ă���B
#define	DIRECTINPUT_VERSION	0x0800
#include <Windows.h>
#include <dinput.h>
#include <d3dx9.h>
#include <map>

namespace Hirai
{
	enum KeyName {
		kButtonNone = 0,
		kButtonSpace = DIK_SPACE,
		kButtonLeft = DIK_LEFT,
		kButtonRight = DIK_RIGHT,
		kButtonUp = DIK_UP,
		kButtonDown = DIK_DOWN,
		kButtonEnter = DIK_RETURN,
		kButtonReset = DIK_R
	};

	class Input
	{
	public:
		// �A�v���P�[�V�����Ŏg�p����R���g���[���̃v���p�e�B��񋓂��Đݒ肷��
		static BOOL CALLBACK EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE* didev, VOID* context);
		// ���p�\�ȃW���C�X�e�B�b�N��񋓂���
		static BOOL CALLBACK EnumJoyStickCallBack(const DIDEVICEINSTANCE* didIns, VOID* context);

		static bool GetButtonDown(const KeyName key);
		static bool GetButtonTrigger(const KeyName key);

		// joy�X�e�B�b�N
		static DIJOYSTATE2 GetJoyInput();

		// �}�E�X
		static DIMOUSESTATE2& GetMouseInput();
		static bool GetMouseLeftButtonTrigger();

		static bool Initialize(HWND hwnd);
		static bool InitializeForMouse(HWND hwnd);
		static void Update();
		static void Finalize();

	private:
		static const int WAIT_COUNT_MAX = 20;

		static BYTE* GetKey();

		static LPDIRECTINPUT8 d_input_;
		static LPDIRECTINPUTDEVICE8 key_device_;
		static LPDIRECTINPUTDEVICE8 mouse_;
		static BYTE diks[256];
		static DIMOUSESTATE2 dims;

		// 1�t���[���O�̃L�[���
		static BYTE* pre_diks_;
		static KeyName prev_key_;
		static bool long_push_;
		static int long_push_count_;
		static bool prev_push_;
		static std::map<int, bool> prev_key_map_;
	};
}

#endif // !GAME_LIB_GAME_LIB_INPUT_H_
