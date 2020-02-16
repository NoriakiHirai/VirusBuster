#include "DirectX.h"
#include <d3d9.h>
#include <atlstr.h>
//#include "StringHelper.h"
#include "DXUtil.h"

namespace Hirai
{
	HWND DirectX::hwnd_ = nullptr;
	LPDIRECT3D9 DirectX::direct3d_ = nullptr;
	LPDIRECT3DDEVICE9 DirectX::device_ = nullptr;
	LPDIRECTINPUT8 DirectX::input_ = nullptr;
	LPDIRECTINPUTDEVICE8 DirectX::key_device_ = nullptr;

	bool DirectX::Initialize(HWND hwnd)
	{
		hwnd_ = hwnd;

		// Direct3D�I�u�W�F�N�g�̍쐬
		if (NULL == (direct3d_ = Direct3DCreate9(D3D_SDK_VERSION)))
		{
			MessageBox(0, TEXT("Direct3D�̍쐬�Ɏ��s���܂����B"), TEXT(""), MB_OK);
			return false;
		}

		// Direct3D�f�o�C�X�I�u�W�F�N�g�̍쐬
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed = TRUE;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		if (FAILED(direct3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &device_)))
		{
			MessageBox(0, TEXT("HAL���[�h��Direct3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂��B"), NULL, MB_OK);

			if (FAILED(direct3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd,
				D3DCREATE_MIXED_VERTEXPROCESSING,
				&d3dpp, &device_)))
			{
				MessageBox(0, TEXT("Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂����B"), NULL, MB_OK);
				return false;
			}
		}
		return true;
	}
	
	void DirectX::SetPresentDevice()
	{
		device_->Present(nullptr, nullptr, nullptr, nullptr);
	}

	void DirectX::LoadTexture(string file_name, IDirect3DTexture9** texture)
	{
		//std::wstring w_file_name = StringHelper::ConvertStringToWString(file_name);
		CString cstr_file_name(file_name.c_str());
		if (FAILED(D3DXCreateTextureFromFile(device_, cstr_file_name, texture)))
		{
			MessageBox(0, TEXT("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B"), NULL, MB_OK);
		}
	}

	void DirectX::FreeDx()
	{
		if (key_device_)
		{
			key_device_->Unacquire();
		}
		SAFE_RELEASE(key_device_);
		SAFE_RELEASE(device_);
		SAFE_RELEASE(direct3d_);
	}
}
