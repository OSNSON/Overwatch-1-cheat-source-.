#include "menu.h"
#include <urlmon.h>
#include <winhttp.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include "Vector3.h"
#include "Skin/Heroes.h"
#include <imgui/imgui_impl_dx11.cpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <VMProtectSDK.h>//아 시발 나 바보다 
#include <Windows.h>
#include <sstream>
static int TabCount = 0;

vector<string> split(string str, char Delimiter) {
	istringstream iss(str);             // istringstream에 str을 담는다.
	string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

	vector<string> result;

	// istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
	while (getline(iss, buffer, Delimiter)) {
		result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
	}

	return result;
}

static void HelpMarker2(const char* desc)
{
	ImGui::TextDisabled(u8"(추천 영웅)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled(u8"(도움말)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

#define DEG2RAD(x) x * M_PI / 180.0
#define M_PI       3.14159265358979323846
int my_image_width = 0; //이미지
int my_image_height = 0;
ID3D11ShaderResourceView* logo = NULL;

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}
void Menu::Initialize(ID3D11Device* pDev, ID3D11DeviceContext* pCont) {

	pDevice = pDev;
	pContext = pCont;
	HWND hWindow = FindWindow("TankWindowClass", NULL);

	ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
	ImGui_ImplDX11_CreateDeviceObjects();
	CreateStyle();
}

void SaveSetting(const char* HeroName)
{
	LPCSTR binPath = (LPCSTR)(u8"C:\\Settings\\" + (std::string)HeroName + u8".ini").c_str();
	WritePrivateProfileString(u8"Setting", u8"AimSpeed", (LPCSTR)to_string(Config::Get().AimSpeed).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"AimFov", (LPCSTR)to_string(Config::Get().Fov).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Aimbot HotKey", (LPCSTR)to_string(Config::Get().AimKey).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Y PITCH", (LPCSTR)to_string(Config::Get().YPITCH).c_str(), binPath);
}


void LoadSetting(const char* HeroName)
{
	LPCSTR binPath = (LPCSTR)(u8"C:\\Settings\\" + (std::string)HeroName + u8".ini").c_str();

	char Strs[MAX_PATH] = { NULL };
	GetPrivateProfileString(u8"Setting", u8"AimSpeed", "", Strs, MAX_PATH, binPath);
	Config::Get().AimSpeed = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"AimFov", "", Strs, MAX_PATH, binPath);
	Config::Get().Fov = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Aimbot HotKey", "0x", Strs, MAX_PATH, binPath);
	Config::Get().AimKey = strtoul(Strs, NULL, 16);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Y PITCH", "", Strs, MAX_PATH, binPath);
	Config::Get().YPITCH = atof(Strs);
}


const char* SettingItemList_English[] = { u8"D.va", u8"Reinhardt", u8"Wrecking Ball", u8"Roadhog", u8"Sigma", u8"Orisa", u8"Winston", u8"Zarya", u8"Tracer", u8"Mei", u8"Reaper", u8"Doomfist", u8"Symmetra", u8"Junkrat", u8"Pharah", u8"Torbjorn", u8"McCree", u8"Widowmaker", u8"Genji", u8"Hanzo", u8"Eco", u8"Soldier: 76", u8"Ashe", u8"Sombra", u8"Zenyatta", u8"Ana", u8"Bastion", u8"Moira", u8"Baptiste", u8"Mercy", u8"Brigitte", u8"Lucio" };
static const char* CurrentSettingItem_English = u8"Genji";

const char* SettingItemList_Korean[] = { u8"디바", u8"라인하르트", u8"레킹 볼", u8"로드호그", u8"시그마", u8"오리사", u8"윈스턴", u8"자리야", u8"트레이서", u8"메이", u8"리퍼", u8"둠피스트", u8"시메트라", u8"정크렛", u8"파라", u8"토르비욘", u8"캐서디", u8"위도우메이커", u8"겐지", u8"한조", u8"에코", u8"솔저: 76", u8"애쉬", u8"솜브라", u8"젠야타", u8"아나" u8"바스티온", u8"모이라", u8"바티스트", u8"메르시", u8"브리기테", u8"루시우" };
static const char* CurrentSettingItem_Korean = u8"겐지";

const char* Time;


/////////////로그인 패널 관련//////////////
wstring get_utf16(const string& str, int codepage)
{
	if (str.empty()) return wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



//string W2e2b2W2i2n2h2t2t2p(string details)
//{
//	DWORD dwSize = 0, dwDownloaded;
//	LPSTR source;
//	source = (char*)"";
//	string responsed = "";
//
//	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
//	BOOL bResults = FALSE;
//
//	hSession = WinHttpOpen(L"Winhttp API", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
//
//	if (hSession)
//		hConnect = WinHttpConnect(hSession, get_utf16("172.30.1.14", CP_UTF8).c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);//"cont0507.dothome.co.kr"
//
//	if (hConnect)
//		hRequest = WinHttpOpenRequest(hConnect, L"GET", get_utf16(details, CP_UTF8).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
//
//	if (hRequest)
//		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
//
//	if (bResults)
//		bResults = WinHttpReceiveResponse(hRequest, NULL);
//
//	if (bResults)
//	{
//		do {
//			dwSize = 0;
//			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
//				printf("Error %u", GetLastError());
//
//			source = (char*)malloc(dwSize + 1);
//			if (!source) {
//				printf("Out of memory\n");
//				dwSize = 0;
//			}
//			else {
//				ZeroMemory(source, dwSize + 1);
//
//				if (!WinHttpReadData(hRequest, (LPVOID)source, dwSize, &dwDownloaded))
//					printf("Error %u", GetLastError());
//				else
//					responsed = responsed + source;
//				free(source);
//			}
//		} while (dwSize > 0);
//	}
//
//	if (!bResults) {
//		exit(0);
//	}
//
//	if (hRequest) WinHttpCloseHandle(hRequest);
//	if (hConnect) WinHttpCloseHandle(hConnect);
//	if (hSession) WinHttpCloseHandle(hSession);
//	return responsed;
//}

string W2e2b2W2i2n2h2t2t2p(string details, int iPort)
{
	DWORD dwSize = 0, dwDownloaded;
	LPSTR source;
	source = (char*)"";
	string responsed = "";

	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
	BOOL bResults = FALSE;

	hSession = WinHttpOpen(L"Winhttp API", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
		hConnect = WinHttpConnect(hSession, get_utf16("cont.mcgo.kr", CP_UTF8).c_str(), iPort, 0);//"cont0507.dothome.co.kr"

	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", get_utf16(details, CP_UTF8).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	if (hRequest)
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	if (bResults)
	{
		do {
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u", GetLastError());

			source = (char*)malloc(dwSize + 1);
			if (!source) {
				printf("Out of memory\n");
				dwSize = 0;
			}
			else {
				ZeroMemory(source, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)source, dwSize, &dwDownloaded))
					printf("Error %u", GetLastError());
				else
					responsed = responsed + source;
				free(source);
			}
		} while (dwSize > 0);
	}

	if (!bResults) {
		exit(0);
	}

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return responsed;
}

string get_system_uuid()
{
	if (system("wmic csproduct get uuid > HWID.txt") == 0)
	{
		auto file = ::fopen("HWID.txt", "rt, ccs=UNICODE");

		enum { BUFFSZ = 1000, UUID_SZ = 36 };
		wchar_t wbuffer[BUFFSZ];

		if (file &&
			::fgetws(wbuffer, BUFFSZ, file) &&
			::fgetws(wbuffer, BUFFSZ, file))
		{
			char cstr[BUFFSZ];
			if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ)
			{
				string uuid = cstr;
				while (!uuid.empty() && isspace(uuid.back())) uuid.pop_back();
				return uuid;
			}
		}
	}
	return {};
}
////////////////////////////아아 참고로 이거 디텍먹은 소스인데 내가 디텍패치 해가지고 줬음 ㅇㅋ 잘했지 아 맞다 나 오늘 월급날임 ㄱㄷㄱㄱㅋㅋㅋㅋㅋㅋㅋ
//void Menu::LoginGo()
//{
//	if (Config::Get().StartLogin == true)
//	{
//		if (AllocConsole())
//		{
//			freopen("CONIN$", "rb", stdin);
//			freopen("CONOUT$", "wb", stdout);
//			freopen("CONOUT$", "wb", stderr);
//			while (Config::Get().LoginCheck == FALSE)
//			{
//				system("cls");
//				cout << "개구리 login" << endl;
//				cout << "코드를 입력해주세요. : ";
//				string codeStr;
//				cin >> codeStr;
//				cout << "코드 : " << codeStr << endl;
//				if (codeStr != "123")
//				{
//					string CodeVal = W2e2b2W2i2n2h2t2t2p("/api.php?mode=login&Code=" + codeStr + "&COM=" + get_system_uuid(), INTERNET_DEFAULT_HTTP_PORT); //
//					if (CodeVal.find("/U/") != string::npos)
//					{
//						system("cls");
//						cout << "로그인 완료!" << endl;
//						Config::Get().LoginCheck = TRUE;
//						cout << "잠시만 기다려주세요... 개구리가 탄생하는 중!!" << endl;
//						Sleep(100);
//						system("TASKKILL /F /IM conhost.exe 2>NULL");
//						break;
//
//
//					}
//					else if (CodeVal.find("Expired") != string::npos)
//					{
//						system("cls");
//						cout << "코드가 만료됐습니다." << endl;
//						Sleep(2500);
//					}
//					else if (CodeVal.find("Please Unlock") != string::npos)
//					{
//						system("cls");
//						cout << "하드락 코드입니다." << endl;
//						Sleep(2500);
//					}
//					else
//					{
//						system("cls");
//						cout << "코드가 올바르지 않습니다." << endl;
//						Sleep(2500);
//					}
//
//				}
//				else
//				{
//					system("cls");
//					cout << "인젝 안 되지? 개병신새끼 ㅋ." << endl;
//					Sleep(2500);
//				}
//
//			}
//			ios::sync_with_stdio();
//		}
//	}
//	else
//	{
//		Config::Get().LoginCheck = TRUE;
//		FreeConsole();
//	}
//
//}

void Menu::LoginGo() //2022-08-03 New Login Logic
{
	if (Config::Get().StartLogin == true)
	{
		if (AllocConsole())
		{
			freopen("CONIN$", "rb", stdin);
			freopen("CONOUT$", "wb", stdout);
			freopen("CONOUT$", "wb", stderr);
			while (Config::Get().LoginCheck == FALSE)
			{
				system("cls");
				cout << "INT V1.0.4" << endl;
				cout << "코드를 입력해주세요. : ";
				string codeStr;
				cin >> codeStr;
				cout << "CODE : " << codeStr << endl;
				if (codeStr != "")
				{
					string CodeVal = W2e2b2W2i2n2h2t2t2p("/api/client/login/initalize?serial=" + codeStr + "&hwid=" + get_system_uuid(), INTERNET_DEFAULT_HTTP_PORT); //
					if (CodeVal.find("FAIL/INVAILD") != string::npos) //존재하지 않는 코드다..
					{
						system("cls");
						cout << "코드가 올바르지 않습니다." << endl;
						Sleep(2500);
					}
					else if (CodeVal.find("FAIL/EXPIRED") != string::npos) //기간이 만료된 코드다..
					{
						system("cls");
						cout << "기간이 만료된 코드입니다." << endl;
						Sleep(2500);
					}
					else if (CodeVal.find("FAIL/HWID") != string::npos) //하드락.
					{
						system("cls");
						cout << "하드락이 걸려있습니다." << endl;
						Sleep(2500);
					}
					else if (CodeVal.find("OK") != string::npos)
					{
						system("cls");
						vector<string> result = split(CodeVal, '/');
						cout << "인젝 성공! 남은 시간 : " << result[2] << endl; //남은 시간 표시용
						cout << "이 창은 닫으셔도 됩니다." << endl;
						Config::Get().LoginCheck = TRUE;
						FreeConsole();
						Sleep(2000);
						break;
					}
				}
				else
				{
					system("cls");
					cout << "구매하신 코드를 입력해주세요." << endl;
					Sleep(2500);
				}

			}
			ios::sync_with_stdio();
		}
	}
	else
	{
		Config::Get().LoginCheck = TRUE;
		FreeConsole();
	}

}

float test = 1.1f;
void SetLogo() {
	static bool isSet = false;
	if(!isSet){
		bool ret = LoadTextureFromFile("C:\\Program Files (x86)\\OW SPONGE INTERNAL\\SPONGE ONE\\Sponge ONE.png", &logo, &my_image_width, &my_image_height); IM_ASSERT(ret); isSet = true;
	}
}
void SetLogo2() {
	static bool isSet = false;
	if (!isSet) {
		bool ret = LoadTextureFromFile("C:\\Users\\sysan\\Downloads\\황금노무현.png", &logo, &my_image_width, &my_image_height); IM_ASSERT(ret); isSet = true;// 야 혹시 그 뭐냐 코드 넣을때 15자리 이상 못 들어가던데 좀 늘려줄수있냐
	}
}

int abc = 1;
void Menu::Render()
{
	ImGui_ImplDX11_NewFrame(); //tlqk ㅅ아니 괄호를 왜 안닫읍니노잘 닫았음 씨발
	Renderer::GetInstance()->BeginScene();
	Renderer::GetInstance()->DrawScene();
	Menu::esp();
	Renderer::GetInstance()->EndScene();

	if (Config::Get().menuEnabled)
	{
		/*if (AllocConsole())
		{
			freopen("CONIN$", "rb", stdin);
			freopen("CONOUT$", "wb", stdout);
			freopen("CONOUT$", "wb", stderr);

		}
		SetConsoleTitle("스폰지 응디 로그인");
		cout << "게임안에서 경량화한 아이디를 입력해주세요." << endl;*/
		ImGui::SetNextWindowPos(ImVec2{ 810, 440 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 1500, 500 }, ImGuiCond_Once);
		bool _visible = true;
		ImGuiStyle* style = &ImGui::GetStyle();
		
		
		style->Colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.93f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.94f, 0.93f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.47f, 0.68f, 0.76f, 1.0f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
		ImGui::SetNextWindowPos(ImVec2{ 100, 100 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 650, 430 }, ImGuiCond_Once);
		style->WindowTitleAlign = ImVec2(0.5, 0.5);
		
		if (ImGui::Begin(u8"INT ", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
		{
			SetLogo();
			ImGui::Columns(2);

			if (Config::Get().MenuCheck)
			{
				ImGui::SetColumnOffset(1, 220);
				ImGui::BeginChild("##", ImVec2(210, -1));
				{
					ImGui::Image((void*)logo, ImVec2(200, 200));
					ImGui::Separator();
					if (ImGui::Button("ESP", ImVec2(200, 41)))
						TabCount = 0;

					ImGui::Spacing();
					if (ImGui::Button(u8"AIM", ImVec2(200, 41)))
						TabCount = 1;

					ImGui::Spacing();
					if (ImGui::Button(u8"MISC", ImVec2(200, 41)))
						TabCount = 2;

					ImGui::EndChild();

				}

				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.63f);

				if (TabCount == 2)
				{

					ImGui::Hotkey(u8"부가기능 핫키 7 : ", &Config::Get().m_AimKey7);
					ImGui::Checkbox((u8"좌클 도움"), &Config::Get().GosAimbot);
					ImGui::Checkbox((u8"자동평타"), &Config::Get().자동평타);
					ImGui::Checkbox((u8"자동섬광"), &Config::Get().자동섬광);
					if (Config::Get().자동섬광)
					{
						ImGui::SliderFloat((u8"##d38j"), &Config::Get().LSGspeed, 0.0000f, 0.020f, (u8"SPEED : % .4f"));
					}

					ImGui::Checkbox((u8"트레 펄스"), &Config::Get().TrQSkill);
					ImGui::Checkbox((u8"겐지 러쉬"), &Config::Get().GenjiRight); //
					ImGui::Checkbox((u8"겐지 용검"), &Config::Get().SAimbot);
					ImGui::Checkbox((u8"겐지 플릭"), &Config::Get().GFAimbot);
					if (Config::Get().GFAimbot)
					{
						if (Config::Get().GFAimbot)
						{
							ImGui::SliderFloat((u8"##fdsg"), &Config::Get().MC1, 0.0000f, 100.00f, (u8"Flick Power : % .0f"));
							ImGui::SliderFloat((u8"##adfasdf"), &Config::Get().Accelerate, 0.0000f, 100.00f, (u8"Accelerate : % .0f"));

						}
						ImGui::BulletText((u8"좌클 딜레이"));
						ImGui::SliderFloat((u8"좌클"), &Config::Get().gfd1, 0.f, 1000.000f, "seex : %0.f ");
						ImGui::BulletText((u8"우클 딜레이"));
						ImGui::SliderFloat((u8"우클"), &Config::Get().gfd2, 0.f, 1000.000f);
						ImGui::BulletText((u8"우클 시작 거리"));
						ImGui::SliderFloat((u8"거리"), &Config::Get().gfd3, 0.f, 500.000f);
					}
					ImGui::Checkbox((u8"아나 수면"), &Config::Get().AnaBot);
					ImGui::SameLine(); HelpMarker(u8"에임핫키 1 = 좌클 | 에임핫키 2 = 우클 | 휠클릭 = 수면");
					ImGui::Checkbox((u8"호그 그랩"), &Config::Get().Roadhog);
					ImGui::Checkbox((u8"둠피 우클"), &Config::Get().DomPredit);
					ImGui::Checkbox((u8"좌클 연타"), &Config::Get().Mecry);
					ImGui::SameLine(); HelpMarker(u8"휠클릭 누를 시 좌클연타 (인게임 전용)");

					ImGui::BulletText(u8"꿀값");

					if (ImGui::Button(u8"맥크리", ImVec2(145, 25)))
					{
						Config::Get().TAimbot = false;
						Config::Get().Silent = false;
						Config::Get().TrigAimbot = false;
						Config::Get().TPAimbot = false;
						Config::Get().TFAimbot = false;
						Config::Get().PSilent = false;
						Config::Get().HanzoAimbot = false;
						Config::Get().HZSilent = false;
						Config::Get().TrigHanzoAimbot = false;
						Config::Get().UseBox2SP = false;

						Config::Get().UseLSG = true;
						Config::Get().LSGdelay = 310.f;
						Config::Get().FAimbot = true;
						//Config::Get().LSGlogic = true;
						//Config::Get().LSGspeed = 0.002f;
						Config::Get().MC1 = 100.f;
						Config::Get().Accelerate = 14.f;
						Config::Get().TriggerNOMU = 7.5f;
						Config::Get().YPITCH = 0.23;
					}
					ImGui::SameLine(); HelpMarker(u8"에임봇을 플릭 V2로 바꿔주세요.");
					if (ImGui::Button(u8"위도우", ImVec2(145, 25)))
					{
						Config::Get().TAimbot = false;
						Config::Get().Silent = false;
						Config::Get().TrigAimbot = false;
						Config::Get().TPAimbot = false;
						Config::Get().TFAimbot = false;
						Config::Get().PSilent = false;
						Config::Get().HanzoAimbot = false;
						Config::Get().HZSilent = false;
						Config::Get().TrigHanzoAimbot = false;
						Config::Get().UseBox2SP = false;

						Config::Get().UseLSG = false;

						Config::Get().FAimbot = true;
						//Config::Get().LSGlogic = true;
						//Config::Get().LSGspeed = 0.002f;
						Config::Get().MC1 = 100.f;
						Config::Get().Accelerate = 14.f;
						Config::Get().TriggerNOMU = 7.5f;
						Config::Get().YPITCH = 0.23;
					}
					ImGui::SameLine(); HelpMarker(u8"에임봇을 플릭 V2로 바꿔주세요.");
					if (ImGui::Button(u8"한조", ImVec2(145, 25)))
					{
						Config::Get().TAimbot = false;
						Config::Get().Silent = false;
						Config::Get().TrigAimbot = false;
						Config::Get().TPAimbot = false;
						Config::Get().TFAimbot = false;
						Config::Get().PSilent = false;
						Config::Get().FAimbot = false;
						Config::Get().HZSilent = false;
						Config::Get().TrigHanzoAimbot = false;
						Config::Get().UseBox2SP = true;

						Config::Get().HanzoAimbot = true;
						Config::Get().sexlogic = true;
						Config::Get().MC1 = 27.f;
						Config::Get().Accelerate = 55.f;
						Config::Get().YPITCH = 0.16;
					}
					ImGui::SameLine(); HelpMarker(u8"에임봇을 한조플릭으로 바꿔주세요.");
				}


				if (Config::Get().cfg_mode == 0)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = true;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 1)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = true;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 2)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = true;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 3)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = true;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 4)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = true;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 5)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = true;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 6)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = true;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 7)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = true;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 8)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = true;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 9)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = true;
				}
				if (Config::Get().cfg_mode == 10)
				{
					Config::Get().Nf = true;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}
				if (Config::Get().cfg_mode == 11)
				{
					Config::Get().Nf = false;
					Config::Get().TAimbot = false;
					Config::Get().FAimbot = false;
					Config::Get().Silent = false;
					Config::Get().TrigAimbot = false;
					Config::Get().TPAimbot = false;
					Config::Get().TFAimbot = false;
					Config::Get().PSilent = false;
					Config::Get().HanzoAimbot = false;
					Config::Get().HZSilent = false;
					Config::Get().TrigHanzoAimbot = false;
				}


				if (TabCount == 1)
				{

					ImGui::BulletText((u8"["));
					//if (Config::Get().TAimbot)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"트레킹을 키시면 에임이 따라가요!!");
					//}
					//if (Config::Get().FAimbot)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"플릭을 키시면 에임이 따라가서 발사해요!!");
					//}
					//if (Config::Get().Silent)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"리플릭을 키시면 유도탄처럼 발사해요!!");
					//}
					//if (Config::Get().TrigAimbot)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"보조키 누르고 적 머리에 에임을 두면 발사해요!!");
					//}
					//if (Config::Get().TPAimbot)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"예측트레킹을 키시면 예측하며 트레킹해요!!");
					//}
					//if (Config::Get().TFAimbot)
					//{

					//	ImGui::SameLine(); HelpMarker(u8"예측플릭을 키시면 예측하며 에임이 가서 쏴요!!");

					//}
					//if (Config::Get().PSilent)
					//{

					//	ImGui::SameLine(); HelpMarker(u8"예측리플릭을 키시면 예측하며 유도탄처럼 발사해요!!");
					//}
					//if (Config::Get().HanzoAimbot)
					//{

					//	ImGui::SameLine(); HelpMarker(u8"한조봇을 키시면 예측하여 활을 쏴요!!");
					//}
					//if (Config::Get().HZSilent)
					//{

					//	ImGui::SameLine(); HelpMarker(u8"한조 리플릭을 키시면 유도탄처럼 발사해요!!");
					//}
					//if (Config::Get().TrigHanzoAimbot)
					//{
					//	ImGui::SameLine(); HelpMarker(u8"보조키 누르고 적 머리에 에임을 두면 발사해요!!");
					//}
					//if (Config::Get().Nf)
					//{

					//	ImGui::SameLine(); HelpMarker(u8"새로운 플릭입니다 진짜 완전 좋읍니다.");
					//}

					ImGui::SameLine();
					//ImGui::Checkbox((u8"빡고모드 (에임봇 끄고 켜야함)"), &Config::Get().Rage);
					ImGui::Text((u8"INT AIMBOT ]"), &Config::Get().realsil);
					/*ImGui::Checkbox((u8"고무성근응디플릭"), &Config::Get().Nf);*/

					ImGui::Combo((u8"	에임봇 "), (int*)&Config::Get().cfg_mode, (u8"트레킹\0플릭\0리플릭\0트리거봇\0예측트레킹\0예측플릭\0예측리플릭\0한조플릭\0한조리플릭\0한조트리거봇\0플릭 V2\0에임봇 비활\0"));


					if (Config::Get().TAimbot)
					{

					}

					if (Config::Get().FAimbot)
					{
						ImGui::Checkbox((u8"에임락"), &Config::Get().aimlock);
						ImGui::Checkbox((u8"보조키 꾹!"), &Config::Get().UseLSG);
						if (Config::Get().UseLSG)
						{
							ImGui::SliderFloat((u8"           "), &Config::Get().LSGdelay, 0.0000f, 5000.00f, (u8"Flick delay : % .4f"));
							{
								if (ImGui::Button(u8"맥크리", ImVec2(55, 25)))
								{
									Config::Get().LSGdelay = 490.0f;
								}

								if (ImGui::Button(u8"애쉬", ImVec2(55, 25)))
								{
									Config::Get().LSGdelay = 635.0f;
								}
							}

							ImGui::Text(u8"맥크리 490");
							ImGui::SameLine();
							ImGui::Text(u8"애쉬 635");
						}
						ImGui::Checkbox((u8"등속로직"), &Config::Get().LSGlogic);
						if (Config::Get().LSGlogic)
						{
							ImGui::SliderFloat((u8"속도"), &Config::Get().LSGspeed, 0.0000f, 0.020f, (u8"등속SPEED : % .4f"));
							{
								if (ImGui::Button("-", ImVec2(20, 20)))
								{
									Config::Get().LSGspeed -= 0.0005f;
								}
								ImGui::SameLine();
								if (ImGui::Button("+", ImVec2(20, 20)))
								{
									Config::Get().LSGspeed += 0.0005f;
								}
							}

						}
						ImGui::BulletText(u8"트리거 포브");
						ImGui::SliderFloat((u8"##642 "), &Config::Get().TriggerNOMU, 0.000f, 25.00000f, (u8"TriggerFov : % .1fsex"));

					}

					if (Config::Get().Silent)
					{

						ImGui::SliderFloat((u8"리플릭 딜레이"), &Config::Get().silentdil, 0.000f, 100.00000f, (u8"Reflick sleep : % .1fsex"));
						ImGui::Checkbox((u8"보조키 꾹!"), &Config::Get().UseLSG);
						if (Config::Get().UseLSG)
						{
							ImGui::SliderFloat((u8"           "), &Config::Get().LSGdelay, 0.0000f, 5000.00f, (u8"REFlick delay : % .4f"));
						}

					}

					if (Config::Get().TrigAimbot)
					{

					}

					if (Config::Get().TPAimbot)
					{

					}

					if (Config::Get().TFAimbot)
					{
						ImGui::Checkbox((u8"보조키 꾹!"), &Config::Get().UseLSG);
						if (Config::Get().UseLSG)
						{
							ImGui::SliderFloat((u8"           "), &Config::Get().LSGdelay, 0.0000f, 5000.00f, (u8"Flick delay : % .4f"));
						}
						ImGui::Checkbox((u8"등속로직"), &Config::Get().LSGlogic);
						if (Config::Get().LSGlogic)
						{
							ImGui::SliderFloat((u8"속도"), &Config::Get().LSGspeed, 0.0000f, 0.020f, (u8"등속SPEED : % .4f"));
							{
								if (ImGui::Button("-", ImVec2(20, 20)))
								{
									Config::Get().LSGspeed -= 0.0005f;
								}
								ImGui::SameLine();
								if (ImGui::Button("+", ImVec2(20, 20)))
								{
									Config::Get().LSGspeed += 0.0005f;
								}
							}

						}


					}

					if (Config::Get().PSilent)
					{
						ImGui::Checkbox((u8"보조키 꾹!"), &Config::Get().UseLSG);
						if (Config::Get().UseLSG)
						{
							ImGui::SliderFloat((u8"           "), &Config::Get().LSGdelay, 0.0000f, 5000.00f, (u8"REFlick delay : % .4f"));
						}

					}

					if (Config::Get().HanzoAimbot)
					{
						ImGui::Checkbox((u8"에임락"), &Config::Get().aimlock);
						ImGui::Checkbox((u8"가속로직(추천)"), &Config::Get().sexlogic);
						if (Config::Get().sexlogic)
						{
							ImGui::SliderFloat((u8"##shash"), &Config::Get().MC1, 0.0000f, 100.00f, (u8"Flick Power : % .0f"));
							ImGui::SliderFloat((u8"##1af90j"), &Config::Get().Accelerate, 0.0000f, 100.00f, (u8"Accelerate : % .0f"));

						}


						ImGui::Checkbox((u8"한조 몸샷/머리샷 ON/OFF"), &Config::Get().NLSGMODE);
						ImGui::Hotkey(u8"머리샷 : ", &Config::Get().m_AimKey5);
						ImGui::Hotkey(u8"몸샷 : ", &Config::Get().m_AimKey6);

					}

					if (Config::Get().HZSilent)
					{
						ImGui::SliderFloat((u8"한조 리플릭 딜레이"), &Config::Get().HZSDelay, 0.0000f, 100.000f, (u8"hanzo delay : % .4f"));


					}

					if (Config::Get().TrigHanzoAimbot)
					{

					}

					if (Config::Get().Nf)
					{
						ImGui::SliderFloat((u8"##shash"), &Config::Get().MC1, 0.0000f, 100.00f, (u8"Flick Power : % .0f"));
						ImGui::SliderFloat((u8"##1af90j"), &Config::Get().Accelerate, 0.0000f, 100.00f, (u8"Accelerate : % .0f"));

						ImGui::Checkbox((u8"보조키 꾹!"), &Config::Get().UseLSG);
						if (Config::Get().UseLSG)
						{
							ImGui::SliderFloat((u8"           "), &Config::Get().LSGdelay, 0.0000f, 5000.00f, (u8"REFlick delay : % .4f"));
							ImGui::SliderFloat((u8"##cdvh4"), &Config::Get().acceleration2, 0.0000f, 1.00f, (u8"acceleration : % .3f"));
						}
						ImGui::BulletText(u8"트리거 포브");
						ImGui::SliderFloat((u8"##642 "), &Config::Get().TriggerNOMU, 0.000f, 25.00000f, (u8"TriggerFov : % .1fsex"));
					}

					if (Config::Get().HanzoAimbot || Config::Get().TrigHanzoAimbot)
					{
						ImGui::BulletText(u8"한조 트리거 범위");
						ImGui::SliderFloat((u8"한조 트리거 범위       "), &Config::Get().TriggerNOMU, 0.00000f, 35.f, "SeX : %0.f");
					}

					if (Config::Get().TPAimbot || Config::Get().TFAimbot || Config::Get().PSilent)
					{

						ImGui::BulletText(u8"예측 속도");
						ImGui::SliderFloat((u8"##i13nf"), &Config::Get().PreditLevel, 0.0000f, 135.f, "예측 SPEED : %0.f");
						ImGui::Checkbox((u8"탄낙차"), &Config::Get().GravityBool);

					}



					ImGui::Checkbox((u8"에임봇 핫키 몸샷/머리샷"), &Config::Get().LSGMODE);
					if (Config::Get().LSGMODE)
					{
						ImGui::Hotkey(u8"에임봇 머리   : ", &Config::Get().m_AimKey4);
						ImGui::Hotkey(u8"에임봇 몸통   : ", &Config::Get().m_AimKey3);
					}

					ImGui::Hotkey(u8"에임봇 핫키 : ", &Config::Get().m_AimKey1);
					ImGui::BulletText(u8"스무스");
					ImGui::SliderFloat((u8" "), &Config::Get().AimSpeed, 1.000f, 0.000f, "Smooth : %.3f ");
					ImGui::BulletText(u8"포브");
					ImGui::SliderFloat((u8"   "), &Config::Get().Fov, 1080.f, 0.f, "Fov : %0.f ");
					ImGui::BulletText(u8"Y축 높이");
					ImGui::SliderFloat((u8"    "), &Config::Get().YPITCH, 1.0f, 0.f, "Pitch : %.3f ");
					ImGui::Dummy(ImVec2(0.f, 30.f));
					/*ImGui::Checkbox((u8"발사음 재생"), &Config::Get().su);*/
					//ImGui::Checkbox((u8"360 에임봇 리스트"), &Config::Get().UseDistanceAim);
					//if (Config::Get().UseDistanceAim)
					//{


					//	{
					//		ImGui::Checkbox((u8"360 트레킹"), &Config::Get().DistanceTracking);
					//	}//거리 에임봇


					//	{
					//		ImGui::Checkbox((u8"360 플릭"), &Config::Get().DistanceFlick);
					//	}//거리 플릭


					//	{
					//		ImGui::Checkbox((u8"360 리플릭"), &Config::Get().DistanceSilent);
					//	}//거리 사일런트

					//}
					//ImGui::Checkbox((u8"자동 점프"), &Config::Get().Jump);
					//ImGui::Checkbox((u8"Input"), &Config::Get().ininput);
					//ImGui::Checkbox((u8"스핀봇(작동여부X)"), &Config::Get().Muban);
					//ImGui::Checkbox((u8"한조스핀봇(작동여부X)"), &Config::Get().MubanH);

					if (Config::Get().Muban || Config::Get().MubanH)
					{
						ImGui::SliderFloat((u8"##gn72bq"), &Config::Get().sd1, 1.f, 850.f, (u8"Delay1 : %.2f"));
						ImGui::SliderFloat((u8"##fmgyb"), &Config::Get().sd2, 0.001f, 10.f, (u8"Delay2 : %.3f"));
						ImGui::SliderFloat((u8"##vn89b"), &Config::Get().sd3, 0.001f, 10.f, (u8"Delay3 : %.3f"));
					}



				}

				if (TabCount == 0)
				{//노폰지를 

					ImGui::Checkbox((u8"테두리 ESP"), &Config::Get().OutlineESP);
					ImGui::Checkbox((u8"테두리 오브젝트"), &Config::Get().OutlineESP2);
					if (Config::Get().OutlineESP)
					{

						ImGui::ColorEdit3((u8"	타겟 컬러"), (float*)&Config::Get().E2SPColor);

					}
					ImGui::Checkbox((u8"예측 ESP"), &Config::Get().UseBox2SP);
					if (Config::Get().NameESP)
					{

						ImGui::ColorEdit3((u8"	예측 컬러"), (float*)&Config::Get().ESPColor2);

					}
					ImGui::Checkbox((u8"거리 ESP"), &Config::Get().BoneESP);
					ImGui::Checkbox((u8"예측거리박스 ESP"), &Config::Get().BoxESP);
					if (Config::Get().BoxESP)
					{
						ImGui::ColorEdit3((u8"	박스 컬러"), (float*)&Config::Get().ESPColor4);

					}

					ImGui::Checkbox((u8"포브그리기"), &Config::Get().FovDraw);

					ImGui::Checkbox((u8"3D박스 ESP"), &Config::Get().DESP);

					ImGui::Checkbox((u8"엣지 ESP"), &Config::Get().EdgeESP);

					ImGui::Checkbox((u8"팬티 ESP"), &Config::Get().SexESP);
					if (Config::Get().SexESP)
					{
						ImGui::ColorEdit3((u8"팬티 컬러"), (float*)&Config::Get().SexCol);
						ImGui::SliderFloat((u8"크기"), &Config::Get().Sexl, 150.f, 0.f, "%88.1f크기");
						ImGui::SliderFloat((u8"꼭짓점"), &Config::Get().Sexlll, 20.f, 0.f, "%2.9f꼭짓점");
						ImGui::SliderFloat((u8"두께"), &Config::Get().Sexll, 150.f, 0.f, "%88.1f두께");
					}

					ImGui::Checkbox((u8"PC방용 ESP"), &Config::Get().PCESP);
					/*ImGui::Checkbox((u8"라인 ESP"), &Config::Get().DrawLine);
					if (Config::Get().DrawLine)
					{
						ImGui::SliderInt((u8"##x8ngd"), &Config::Get().Lx, 0.0f, 1920.f, (u8"X축 : %.0f"));
						ImGui::SliderInt((u8"##ag8ghga"), &Config::Get().Ly, 0.0f, 1080.f, (u8"Y축 : %.0f"));
						ImGui::SliderInt((u8"##x8nadgd"), &Config::Get().Lt, 0.0f, 25.f, (u8"굵기 : %.0f"));
					}*/
					/*ImGui::Checkbox((u8"정보"), &Config::Get().IF);*/
				}

				//if (ImGui::Button(u8"사용X", ImVec2(115, 25)))
				//{
					//Config::Get().IsLogin = false;
					//Config::Get().OutlineESP = false;
					//Config::Get().IsLogin = false;
					//Config::Get().FovDraw = false;
					//Config::Get().BoneESP = false;
				//}
				ImGui::BulletText(u8"추가적인 최적화");
				if (ImGui::Button(u8"디코종료(추천)", ImVec2(115, 25)))
				{
					system("taskkill /f /im discord.exe");
				}
				if (ImGui::Button(u8"오버워치 종료", ImVec2(115, 25)))
				{
					exit(0);
				}
				ImGui::BulletText(u8"우선순위 ( 백신 켜져있으면 작동 안할수도 있음)");

				if (ImGui::Button(u8"실시간(추천)", ImVec2(115, 25)))
				{
					system("wmic process where name=\"Overwatch.exe\" call setpriority 256");
				}
				if (ImGui::Button(u8"높음(극추천)", ImVec2(115, 25)))
				{
					system("wmic process where name=\"Overwatch.exe\" call setpriority 128");
				}
				if (ImGui::Button(u8"높은 우선순위", ImVec2(115, 25)))
				{
					system("wmic process where name=\"Overwatch.exe\" call setpriority 32768");
				}
				if (ImGui::Button(u8"중간(기본)", ImVec2(115, 25)))
				{
					system("wmic process where name=\"Overwatch.exe\" call setpriority 32");
				}
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");
				ImGui::Text(u8" ");

			}
		}
		ImGui::End();
	}
	if (Config::Get().nomu)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiSetCond_Once);
		bool _visible = true;
		if (ImGui::Begin(u8"##Fov", &_visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs))
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			window->DrawList->AddCircle(ImVec2(1920 / 2.0f, 1080 / 2.0f), Config::Get().Fov, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), 100, 1.0f);

		}

		ImGui::End();
	}



    ImGui::Render();
}



void Menu::Shutdown() {
	ImGui_ImplDX11_Shutdown();
}

void Menu::CreateStyle()
{

}