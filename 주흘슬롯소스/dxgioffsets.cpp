#include "dxgiOffsets.h"

bool DXGIOffsets::Initialize() {
	if (InitSystemPath()) {
		HMODULE dxgiModule = GetDXGIModule();

		if (!dxgiModule) {
			return false;
		}
		WNDCLASSA wc = { 0 };
		wc.style = CS_OWNDC;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.lpfnWndProc = (WNDPROC)DefWindowProcA;
		wc.lpszClassName = DUMMY_WNDCLASS;

		SetErrorMode(SEM_FAILCRITICALERRORS);

		if (!RegisterClassA(&wc)) {
			return false;
		}
		return true;
	}
	else {
		return false;
	}
}

bool DXGIOffsets::InitSystemPath()
{
	UINT ret = GetSystemDirectoryA(system_path, MAX_PATH);
	if (!ret) {
		return false;
	}
	return true;
}

HMODULE DXGIOffsets::GetDXGIModule()
{
	char base_path[MAX_PATH];
	strcpy_s(base_path, system_path);
	strcat_s(base_path, "\\");
	strcat_s(base_path, "dxgi.dll");
	return GetModuleHandleA(base_path);
}


void* DXGIOffsets::GetPresentAddr() {
	return (void*)((uintptr_t)GetDXGIModule() + (uintptr_t)GetDXGIOffsets().present);
}

dxgi_offsets DXGIOffsets::GetDXGIOffsets() {
	struct dxgi_offsets dxgi = { 0 };

	bool      success = initDXGI();
	HRESULT   hr;

	if (success) {
		dxgi.present = vtable_offset(info.module, info.swap, 8);
		dxgi.resize = vtable_offset(info.module, info.swap, 13);
		dxgi.drawindexed = vtable_offset(info.module, info.swap, 16);

		IDXGISwapChain1 *swap1;
		hr = info.swap->QueryInterface(__uuidof(IDXGISwapChain1),
			(void**)&swap1);
		if (SUCCEEDED(hr)) {
			dxgi.present1 =
				vtable_offset(info.module, swap1, 22);
			swap1->Release();
		}
	}

	closeDXGI();
	return dxgi;
}

bool DXGIOffsets::initDXGI()
{
	HMODULE       d3d10_module;
	d3d10create_t create;
	create_fac_t  create_factory;
	IDXGIFactory1 *factory;
	IDXGIAdapter1 *adapter;
	IUnknown      *device;
	HRESULT       hr;

	info.hwnd = CreateWindowExA(0, DUMMY_WNDCLASS, "",
		WS_POPUP, 0, 0, 2, 2, nullptr, nullptr,
		GetModuleHandleA(nullptr), nullptr);
	if (!info.hwnd) {
		return false;
	}

	info.module = LoadLibraryA("dxgi.dll");
	if (!info.module) {
		return false;
	}

	create_factory = (create_fac_t)GetProcAddress(info.module,
		"CreateDXGIFactory1");

	d3d10_module = LoadLibraryA("d3d10.dll");
	if (!d3d10_module) {
		return false;
	}

	create = (d3d10create_t)GetProcAddress(d3d10_module,
		"D3D10CreateDeviceAndSwapChain");
	if (!create) {
		return false;
	}

	IID factory_iid = IsWindows8OrGreater()
		? dxgiFactory2
		: __uuidof(IDXGIFactory1);

	hr = create_factory(&factory_iid, (void**)&factory);
	if (FAILED(hr)) {
		return false;
	}

	hr = factory->EnumAdapters1(0, &adapter);
	factory->Release();
	if (FAILED(hr)) {
		return false;
	}

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Width = 2;
	desc.BufferDesc.Height = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = info.hwnd;
	desc.SampleDesc.Count = 1;
	desc.Windowed = true;

	hr = create(adapter, D3D10_DRIVER_TYPE_HARDWARE, nullptr, 0,
		D3D10_SDK_VERSION, &desc, &info.swap, &device);
	adapter->Release();
	if (FAILED(hr)) {
		return false;
	}

	device->Release();
	return true;
}

uint32_t DXGIOffsets::vtable_offset(HMODULE module, void *cls,
	unsigned int offset)
{
	uintptr_t *vtable = *(uintptr_t**)cls;
	return (uint32_t)(vtable[offset] - (uintptr_t)module);
}

void DXGIOffsets::closeDXGI()
{
	if (info.swap)
		info.swap->Release();
	if (info.hwnd)
		DestroyWindow(info.hwnd);
}