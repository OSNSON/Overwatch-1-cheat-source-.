#pragma once
#include <Windows.h>
#include <stdint.h>
#include <d3d11.h>
#include <VersionHelpers.h>
#include <dxgi1_2.h>
#include "singleton.h"

#define DUMMY_WNDCLASS "get_addrs_wndclass"

static const IID dxgiFactory2 =
{ 0x50c83a1c, 0xe072, 0x4c48, { 0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0 } };

typedef HRESULT(WINAPI *d3d10create_t)(IDXGIAdapter*, D3D10_DRIVER_TYPE,
	HMODULE, UINT, UINT, DXGI_SWAP_CHAIN_DESC*,
	IDXGISwapChain**, IUnknown**);
typedef HRESULT(WINAPI *create_fac_t)(IID *id, void**);

struct dxgi_info {
	HMODULE        module;
	HWND           hwnd;
	IDXGISwapChain *swap;
};
struct dxgi_offsets {
	uint32_t present;
	uint32_t resize;
	uint32_t present1;
	uint32_t drawindexed;
};
class DXGIOffsets
	: public Singleton<DXGIOffsets>
{
	friend class Singleton<DXGIOffsets>;

public:
	bool Initialize();
	dxgi_offsets GetDXGIOffsets();
	void* GetPresentAddr();
	HMODULE GetDXGIModule();
private:
	bool initDXGI();
	uint32_t vtable_offset(HMODULE module, void *cls,
		unsigned int offset);
	void closeDXGI();
	bool InitSystemPath();

	char system_path[MAX_PATH] = { 0 };
	dxgi_info info = {};
};