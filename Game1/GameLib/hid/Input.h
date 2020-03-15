#ifndef GAME_LIB_GAME_LIB_INPUT_H_
#define GAME_LIB_GAME_LIB_INPUT_H_

// DIRECTINPUT_VERSIONを指定しておかないと、警告が発生するので例外的に
// インクルード宣言の前でマクロ宣言をしている。
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
		// アプリケーションで使用するコントローラのプロパティを列挙して設定する
		static BOOL CALLBACK EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE* didev, VOID* context);
		// 利用可能なジョイスティックを列挙する
		static BOOL CALLBACK EnumJoyStickCallBack(const DIDEVICEINSTANCE* didIns, VOID* context);

		static bool GetButtonDown(const KeyName key);
		static bool GetButtonTrigger(const KeyName key);

		// joyスティック
		static DIJOYSTATE2 GetJoyInput();

		// マウス
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

		// 1フレーム前のキー情報
		static BYTE* pre_diks_;
		static KeyName prev_key_;
		static bool long_push_;
		static int long_push_count_;
		static bool prev_push_;
		static std::map<int, bool> prev_key_map_;
	};
}

#endif // !GAME_LIB_GAME_LIB_INPUT_H_
