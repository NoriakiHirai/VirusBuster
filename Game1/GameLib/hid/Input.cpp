#include "Input.h"
#include <atlstr.h>
#include <graphics/DXUtil.h>

//#define USE_GAMEPAD

namespace Hirai
{
	LPDIRECTINPUT8 Input::d_input_ = nullptr;
	LPDIRECTINPUTDEVICE8 Input::key_device_ = nullptr;
	LPDIRECTINPUTDEVICE8 Input::mouse_ = nullptr;
	BYTE Input::diks[256] = {};
	DIMOUSESTATE2 Input::dims = { 0 };
	BYTE* Input::pre_diks_ = nullptr;
	bool Input::long_push_ = false;
	int Input::long_push_count_ = WAIT_COUNT_MAX;	// 20F
	bool Input::prev_push_ = false;
	KeyName Input::prev_key_ = KeyName::kButtonNone;
	std::map<int, bool> Input::prev_key_map_ = {
		{KeyName::kButtonNone, true},
		{KeyName::kButtonSpace, false},
		{KeyName::kButtonLeft, false},
		{KeyName::kButtonRight, false},
		{KeyName::kButtonUp, false},
		{KeyName::kButtonDown, false},
		{KeyName::kButtonEnter, false},
		{KeyName::kButtonReset, false}
	};

	DIJOYSTATE2 Input::GetJoyInput()
	{
		DIJOYSTATE2 js = { 0 };
		HRESULT hr = key_device_->Acquire();
		if ((hr == DI_OK) || (hr == S_FALSE)) {
			// 十字キーまたはスティックの入力を取得
			key_device_->GetDeviceState(sizeof(DIJOYSTATE2), &js);
		}
		return js;
	}

	DIMOUSESTATE2& Input::GetMouseInput()
	{
		dims = { 0 };
		if (FAILED(mouse_->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
		{
			mouse_->Acquire();
		}
		return dims;
	}

	bool Input::GetMouseLeftButtonTrigger()
	{
		DIMOUSESTATE2& dims = GetMouseInput();
		if ((dims.rgbButtons[0] && 0x80) && 
			long_push_count_ <= 0)
		{
			long_push_count_ = WAIT_COUNT_MAX;
			return true;
		}
		return false;
	}

	bool Input::Initialize(HWND hwnd)
	{
		if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& d_input_,
			nullptr)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		if (FAILED(d_input_->CreateDevice(GUID_SysKeyboard, &key_device_, nullptr)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

#ifndef USE_GAMEPAD
		// デバイスをキーボードに設定
		if (FAILED(key_device_->SetDataFormat(&c_dfDIKeyboard)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}
#else
		// 利用可能なゲームコントローラーの列挙関数を実行
		if (FAILED(d_input_->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyStickCallBack,
			nullptr, DIEDFL_ATTACHEDONLY))
		) {
			return false;
		}
		if (!key_device_) {
			return false;
		}
		// デバイスをジョイスティックに設定
		if (FAILED(key_device_->SetDataFormat(&c_dfDIJoystick2))) {
			return false;
		}
#endif // !USE_GAMEPAD

		// 強調レベルの設定
		if (FAILED(key_device_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

#ifdef USE_GAMEPAD
		 // アプリケーションで使用するコントローラのプロパティを設定する
		if (FAILED(key_device_->EnumObjects(EnumObjectsCallBack, nullptr, DIDFT_ALL))) {
			return false;
		}
#endif // USE_GAMEPAD

		// デバイスのアクセス権を取得する
		key_device_->Acquire();
		return true;
	}

	bool Input::InitializeForMouse(HWND hwnd)
	{
		if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&d_input_,
			nullptr)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		if (FAILED(d_input_->CreateDevice(GUID_SysMouse, &mouse_, nullptr)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		// デバイスをマウスに設定
		if (FAILED(mouse_->SetDataFormat(&c_dfDIMouse2)))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		// 強調レベルの設定
		if (FAILED(mouse_->SetCooperativeLevel(
			hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))
		) {
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		// デバイスを取得
		if (FAILED(mouse_->Acquire()))
		{
			MessageBox(0, TEXT("Inputクラスの初期化中にエラーが発生しました。"), NULL, MB_OK);
			return false;
		}

		return true;
	}

	void Input::Update()
	{
		if (long_push_count_ > 0)
			--long_push_count_;
	}

	void Input::Finalize()
	{
		if (key_device_) key_device_->Unacquire();
		SAFE_RELEASE(key_device_);
		SAFE_RELEASE(mouse_);
		SAFE_RELEASE(d_input_);
	}

	BYTE* Input::GetKey()
	{
		HRESULT hr = key_device_->Acquire();
		//DI_OK:アクセス権取得成功、S_FALSE：アクセス権取得済み
		if ((hr == DI_OK) || (hr == S_FALSE))
		{
			key_device_->GetDeviceState(sizeof(diks), &diks);
		}
		return diks;
	}

    BOOL Input::EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE* didev, VOID* context)
    {
		if (didev->dwType & DIDFT_AXIS) {
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = didev->dwType;
			diprg.lMin = 6;
			diprg.lMax = 6;

			if (FAILED(key_device_->SetProperty(DIPROP_RANGE, &diprg.diph))) {
				return DIENUM_STOP;
			}
		}
        return DIENUM_CONTINUE;
    }

	BOOL Input::EnumJoyStickCallBack(const DIDEVICEINSTANCE* didIns, VOID* context)
	{
		// TODO: 複数列挙される場合
		char szConfirm[256];
		sprintf_s(szConfirm, "この物理デバイスでデバイスオブジェクトを作成しますか？\n%s\n%s",
			didIns->tszProductName, didIns->tszInstanceName);
		if (MessageBox(0, CString(szConfirm), CString("確認"), MB_YESNO) == IDNO) {
			return DIENUM_CONTINUE;
		}
		// DirectInputデバイスオブジェクトの作成
		if (FAILED(d_input_->CreateDevice(didIns->guidInstance, &key_device_, nullptr)))
		{
			return DIENUM_CONTINUE;
		}
		return DIENUM_STOP;
	}

    bool Input::GetButtonDown(const KeyName key)
	{
		return (GetKey()[key] & 0x80);
	}

	bool Input::GetButtonTrigger(const KeyName key)
	{
		bool is_trigger_ = false;
		bool now_push = (GetKey()[key] & 0x80);

		if (prev_key_map_[key] && !now_push &&
			prev_key_ == key) {
			is_trigger_ = true;
		}
		prev_push_ = now_push;
		if (now_push) {
			prev_key_map_[key] = true;
			prev_key_ = key;
		}
		else {
			prev_key_map_[key] = false;
		}

		return is_trigger_;
	}
}