#ifndef GAME_LIB_GAME_LIB_DX_UTIL_H_
#define GAME_LIB_GAME_LIB_DX_UTIL_H_
//#include <minwindef.h>
#include <d3d9.h>

namespace Hirai
{

	/**
	 *	@fn SAFE_RELEASE
	 *	@brief DirectX関連のポインタ変数のメモリ開放を行う。
	 */
	#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }

	VOID InitMaterial(
		D3DMATERIAL9& material,
		FLOAT r = 0.f, FLOAT g = 0.f, FLOAT b = 0.f,
		FLOAT a = 0.f
	);
}

#endif // !GAME_LIB_GAME_LIB_DX_UTIL_H_
