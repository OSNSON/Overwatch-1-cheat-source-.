#include "d3dhook.h"

namespace D3dHook {
	ID3D11Device *pDevice = NULL;
	ID3D11DeviceContext *pContext = NULL;
	ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
	OriginalPresent_t OriginalPresent = nullptr;
	void* present_addr = NULL;
	bool firstInit = true;

	bool AttachHook() {
		if (DXGIOffsets::Get().Initialize()) {
			OriginalPresent = (OriginalPresent_t)DXGIOffsets::Get().GetPresentAddr();
			if (OriginalPresent != nullptr) {
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)OriginalPresent, HookedPresent);
				DetourTransactionCommit();
				return true;
			}
		}
		return false;
	}

	bool DetachHook() {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)OriginalPresent, HookedPresent);
		DetourTransactionCommit();
		return true;
	}

	HRESULT STDMETHODCALLTYPE HookedPresent(IDXGISwapChain *swap,
		UINT sync_interval, UINT flags)
	{

		if (firstInit) {
			firstInit = false;
			HRESULT hr;

			hr = swap->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
			if (SUCCEEDED(hr)) {
				pDevice->GetImmediateContext(&pContext);
			}

			Menu::Get().Initialize(pDevice, pContext);
		}
		
		CreateRenderTarget(swap);
		if (GetAsyncKeyState(VK_INSERT)) 
		{
			Config::Get().menuEnabled = !Config::Get().menuEnabled;
			Sleep(100);
		}

		Menu::Get().Render(); 
		return OriginalPresent(swap, sync_interval, flags);
	}

	void CreateRenderTarget(IDXGISwapChain *swap)
	{
		static bool FirstInit = true;
		if (FirstInit)
		{
			FirstInit = false;
			DXGI_SWAP_CHAIN_DESC sd;
			swap->GetDesc(&sd);

			// Create the render target
			ID3D11Texture2D* pBackBuffer;
			D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
			ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
			render_target_view_desc.Format = sd.BufferDesc.Format;
			render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
			pBackBuffer->Release();
		}
		pContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	}
}