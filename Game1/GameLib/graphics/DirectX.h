#ifndef GAME_LIB_GAME_LIB_DIRECTX_H_
#define GAME_LIB_GAME_LIB_DIRECTX_H_

// DIRECTINPUT_VERSIONを指定しておかないと、警告が発生するのでマクロ宣言をしている。
#define	DIRECTINPUT_VERSION	0x0800
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>

using std::string;

namespace Hirai
{
	struct RGB {
		int r; int g; int b;
	};

// なぜかD3DCOLOR_XRGBが使えないので、ここで再定義
#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)

	class DirectX
	{
	public:
		static bool Initialize(HWND hwnd);
		static void SetPresentDevice();
		static void LoadTexture(string file_name, IDirect3DTexture9** texture);
		static void FreeDx();

		static LPDIRECT3DDEVICE9& GetD3DDevice() { return device_; }

	private:
		static HWND						hwnd_;
		static LPDIRECT3D9				direct3d_;
		static LPDIRECT3DDEVICE9		device_;
		static LPDIRECTINPUT8			input_;
		static LPDIRECTINPUTDEVICE8		key_device_;
	};
}

#endif // !GAME_LIB_GAME_LIB_DIRECTX_H_
