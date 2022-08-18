#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include "dxgioffsets.h"
#include "msdetours/include/detours.h"
#include "menu.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "detours.lib")
#pragma warning(disable : 4244 4996 )

namespace D3dHook {
	extern ID3D11Device *pDevice;
	extern ID3D11DeviceContext *pContext;
	extern ID3D11RenderTargetView*  g_mainRenderTargetView;
	extern DWORD_PTR* pDeviceContextVTable;

	void CreateRenderTarget(IDXGISwapChain *swap);

	typedef HRESULT(__stdcall* OriginalPresent_t)(IDXGISwapChain*, UINT, UINT);

	HRESULT __stdcall HookedPresent(IDXGISwapChain *swap, UINT sync_interval, UINT flags);

	extern bool firstInit;
	extern bool g_Unload;

	bool AttachHook();
	bool DetachHook();
}