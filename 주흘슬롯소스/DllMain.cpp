#pragma region include

/* D3D */
#include "D3dhook.h"
#include <DirectXMath.h>
#include "Vector3.h"
/* D3D */

/* Input */
#include "Input.h"
/* Input */
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <conio.h>
#include <stdio.h>

/* VC++ */
#include <iostream>
#include <fstream>
#include <string>
#include <winhttp.h>
#include <time.h>
#include <Windows.h>
#include <Tlhelp32.h>
#include <atlstr.h>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <vector>
#include <thread>
#include <string>
#include <ostream>
#include <bitset>
#include <tuple>
#include <mutex>
#include <array>
#include <algorithm>
#include <Digitalv.h>; 
/* VC++ */

/* BreakPoint */
#include "BreakPoint.h"
/* BreakPoint */

/* OFFSETS */
#include "Offset.h"
/* OFFSETS */

/* Config */
#include "config.h"
/* Config */

/* Utils */
#include "Memory.h"
#include "Skin/Heroes.h"
#include "renderer.h"
#include "Skin/BotUtils.h"
#include "Memory.hpp"
#include "skCrypter.h"
#include "defs.h"
#include "load.h"
/* Utils */

#define M_PI       3.14159265358979323846
#define DEG2RAD(x) x * M_PI / 180.0
#pragma comment (lib, "urlmon.lib")
#pragma comment (lib, "winhttp.lib")
#include "Spoofcall.h"
#include "sha256.h"
#include "SendInput.h"
#include "Input.h"
#include <random>
#include <VMProtectSDK.h>
#include "SKD.hpp"
using namespace DirectX;
#pragma endregion

#pragma region Declare
HINSTANCE g_Module;

Heroes eSkinEnum;
Heroes mSkinEnum;

uint64_t m_getBoneStruct{};
uint64_t m_refreshBones{};

DWORD64 viewMatrixPtr;
Matrix viewMatrix;
MatrixTo viewMatrixTo;

float Health, HealthMax, Armor, ArmorMax, Barrier, BarrierMax;

#define GravityForce 9.81f * 0.5f * Distance / Hanzo_BulletSpeed * Distance / Hanzo_BulletSpeed

Vector3 MyAngle, TargetAngle, EnPos, GetMyPos, Velocity, RootPos;

static Vector3 staticAngle;

std::string EnCompoHook, EnFovHook, EnAngleHook, EnWallHook, EnGlow ,EnRayTrace, EnMatrixHook, EnVelocityHook, EnHero, EnHealthHook, EnRotationHook;

DirectX::XMFLOAT3 MyXMAngle;

uint64_t AnglePTR, ComponentParent, eHeroID, eRotation;

WORD BoneCount;

struct Entity
{
	Vector3 Location, savedVelocity, Velocity, lastPos, rootPos, BonePos, BoneCountPos;
	bool Enemy, Alive, MyTeam, Wall;
	uint8_t VisCheck;
	clock_t lastVelocityUpdate;
	float PlayerHealth, eVec, Health, HealthMax, ARMOR, ARMORMAX, BARRIER, BARRIERMAX, Rotation, Size;
	uint64_t eParent, eHealthComponent, ReaperParent, BoneBase;
	WORD SkinID, MySkinID, BoneCount;

	uint64_t lasteParentPTR, HeroID, SkillBase;
	clock_t lastWallUpdate;
};

uint32_t m_Timestamp_Zarya{};
uint32_t m_Timestamp_Mei{};

Entity Entitys[100];

vector<ULONG64>EntityPTR;

uint64_t SkillBase, RayRCX, RayRCX2, MyPos;
#pragma endregion

#pragma region EncryptData
std::string encrypt(UINT64 ui64)
{
	return sha256(std::to_string(ui64));
}

void SaveEncrypted() // VMP 
{
	/*EnCompoHook = encrypt(Config::Get().BaseAddress + offset::CompoenetHook);*/
	EnMatrixHook = encrypt(Config::Get().BaseAddress + offset::ViewMatrixOffset);
	EnFovHook = encrypt(Config::Get().BaseAddress + offset::FovHook);
	EnAngleHook = encrypt(Config::Get().BaseAddress + offset::AngleHook);
	EnWallHook = encrypt(Config::Get().BaseAddress + offset::BorderLine);
	EnGlow = encrypt(Config::Get().BaseAddress + offset::BorderLine2);
	//EnHealthHook = encrypt(Config::Get().BaseAddress + offset::Health);
	//EnHero = encrypt(Config::Get().BaseAddress + offset::HeroID);
	//EnRotationHook = encrypt(Config::Get().BaseAddress + offset::Rotation);
}
#pragma endregion

#pragma region Angle

Vector3 CalcAngle(Vector3 MyPos, Vector3 EnPos, float Dis)
{
	Vector3 Result;

	Result.x = (EnPos.x - MyPos.x) / Dis;
	Result.y = (EnPos.y - MyPos.y) / Dis;
	Result.z = (EnPos.z - MyPos.z) / Dis;

	return Result;
}

Vector3 GetAngle(Vector3 RAngle, Vector3 MPos, Vector3 EPos)
{
	float Distance = MPos.Distance(EPos);

	Vector3 Result;

	Result.x = (EPos.x - MPos.x) / Distance;
	Result.y = (EPos.y - MPos.y) / Distance;
	Result.z = (EPos.z - MPos.z) / Distance;

	return Result;
}

Vector3 SmoothAngle(Vector3 LocalAngle, Vector3 TargetAngle, float X_Speed, float Y_Speed)
{
	Vector3 Result;
	Result.x = (TargetAngle.x - LocalAngle.x) * X_Speed + LocalAngle.x;
	Result.y = (TargetAngle.y - LocalAngle.y) * Y_Speed + LocalAngle.y;
	Result.z = (TargetAngle.z - LocalAngle.z) * X_Speed + LocalAngle.z;

	return Result;
}
Vector3 AcAngle(Vector3 local, Vector3 target, float speed)
{
	float t = 1.0f / local.Distance(target) * (speed * 0.015f);
	t = min(1.0f, t);
	return Vector3
	{
		(target.x - local.x) * t + local.x,
		(target.y - local.y) * t + local.y,
		(target.z - local.z) * t + local.z
	};
}


void PressKey(int Key, float duration)
{
	clock_t previous = clock();
	while (clock() - previous < duration)
	{
		Config::Get().WPM<DWORD>(Config::Get().BaseAddress + 0x1034, Key);
	}
}

uint64_t GetSenstivePTR()
{
	uint64_t tmp = Config::Get().RPM<uintptr_t >(Config::Get().BaseAddress + offset::SensOffset);
	tmp = Config::Get().RPM<uintptr_t>(tmp + 0x38);
	tmp = Config::Get().RPM<uintptr_t>(tmp + 0x8);
	tmp = Config::Get().RPM<uintptr_t>(tmp + 0x8);
	tmp = Config::Get().RPM<uintptr_t>(tmp + 0x8);
	tmp += 0x6B0 + 0x14;
	return tmp;
}




#pragma endregion

#pragma region NAMETAG
vector<MEMORY_BASIC_INFORMATION> mbis;
bool UpdateMemoryQuery()
{
	MEMORY_BASIC_INFORMATION mbi = { 0, };
	MEMORY_BASIC_INFORMATION old = { 0, };
	ULONG64 current_address = 0x7ffe0000;
	vector<MEMORY_BASIC_INFORMATION> addresses;
	while (true)
	{
		if (!VirtualQueryEx(GetCurrentProcess(), (PVOID)current_address, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			break;
		if ((mbi.State & 0x1000) != 0 && (mbi.Protect & 0x100) == 0)
		{
			addresses.push_back(mbi);

			old = mbi;
		}
		current_address = ULONG64(mbi.BaseAddress) + mbi.RegionSize;
	}

	mbis = addresses;


	return (mbis.size() > 0);
}


ULONG64 FindPattern2(BYTE* buffer, BYTE* pattern, string mask, int bufSize)
{
	int pattern_len = mask.length();

	for (int i = 0; i < bufSize - pattern_len; i++)
	{
		bool found = true;
		for (int j = 0; j < pattern_len; j++)
		{
			if (mask[j] != '?' && pattern[j] != buffer[(i + j)])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return -1;
}

vector<ULONG64> FindPatternEx(ULONG64 start, ULONG64 end, BYTE* pattern, string mask, MEMORY_BASIC_INFORMATION mbi, ULONG64 RgSize)
{
	ULONG64 current_chunk = start;
	vector<ULONG64> found;
	if ((end - current_chunk > RgSize && RgSize != 0) || (end - current_chunk < RgSize && RgSize != 0))
		return found;
	while (current_chunk < end)
	{
		int bufSize = (int)(end - start);
		BYTE* buffer = new BYTE[bufSize];
		if (!ReadProcessMemory(GetCurrentProcess(), (LPVOID)current_chunk, buffer, bufSize, nullptr))
		{
			current_chunk += bufSize;
			delete[] buffer;
			continue;
		}

		ULONG64 internal_address = FindPattern2(buffer, pattern, mask, bufSize);
		if (internal_address != -1)
		{
			found.push_back(current_chunk + internal_address);
		}
		current_chunk += bufSize;
		delete[] buffer;

	}
	return found;
}

vector<ULONG64> _FindPatterns(BYTE* buffer, BYTE* pattern, string mask, int bufSize)
{
	vector<ULONG64> ret;
	int pattern_len = mask.length();
	for (int i = 0; i < bufSize - pattern_len; i++)
	{
		bool found = true;
		for (int j = 0; j < pattern_len; j++)
		{
			if (mask[j] != '?' && pattern[j] != buffer[i + j])
			{
				found = false;
				break;
			}
		}
		if (found)
			ret.push_back(i);
	}
	return ret;
}

ULONG64 FindPattern(BYTE* pattern, string mask, ULONG64 RgSize)
{
	if (!UpdateMemoryQuery())
		return 0;

	for (int i = 0; i < mbis.size(); i++) {
		MEMORY_BASIC_INFORMATION info = mbis[i];

		vector<ULONG64> arr = FindPatternEx(ULONG64(info.BaseAddress), info.RegionSize + ULONG64(info.BaseAddress), pattern, mask, info, RgSize);
		if (arr.size() > 0)
			return arr[0];
	}

	return 0;
}

vector<ULONG64> FindPatterns(BYTE* pattern, string mask, ULONG64 RgSize)
{
	vector<ULONG64> Result;
	ULONG64 PatternStart = FindPattern(pattern, mask, RgSize);
	if (PatternStart)
	{
		for (int i = 0; i < mbis.size(); i++)
		{
			if (ULONG64(mbis[i].BaseAddress) < PatternStart && PatternStart - ULONG64(mbis[i].BaseAddress) < mbis[i].RegionSize)
			{
				PatternStart = ULONG64(mbis[i].BaseAddress);
			}
		}

		BYTE* buf = new BYTE[RgSize];
		memcpy_s(buf, RgSize, PVOID(PatternStart), RgSize);

		vector<ULONG64> Pointers = _FindPatterns(buf, pattern, mask, RgSize);
		delete[] buf;

		for (int i = 0; i < Pointers.size(); i++)
			Pointers[i] += PatternStart;

		Result = Pointers;
	}

	return Result;
}

void Pointer() // 네임태그 포인터 쓰레드
{
	while (true)
	{
		EntityPTR = FindPatterns((PBYTE)"\xFA\x42\x00\x00\x00\x00\x00\x00\x00\x00\x01\x04\x1E\x00\x07", "xx????xxxxxxxxx", 0x16000);

		/*this_thread::sleep_for(500ms);*/
		Sleep(25);
	}
}
#pragma endregion

#pragma region Function
uint64_t GetViewMatrix()
{
	__try
	{
		uint64_t base = Config::Get().RPM<uintptr_t>(Config::Get().BaseAddress + offset::ViewMatrixOffset) ^ offset::ViewMatrixXorKey;

		uint64_t result = Config::Get().RPM<uintptr_t>(base + 0xA90);
		result = Config::Get().RPM<uintptr_t>(result + 0x198);
		result = Config::Get().RPM<uintptr_t>(result + 0x8);
		result = Config::Get().RPM<uintptr_t>(result + 0x0);

		if (!result)
		{
			result = Config::Get().RPM<uintptr_t>(base + 0xA80);
			result = Config::Get().RPM<uintptr_t>(result + 0x198);
			result = Config::Get().RPM<uintptr_t>(result + 0x8);
			result = Config::Get().RPM<uintptr_t>(result + 0x0);

			if (!result)
			{
				result = Config::Get().RPM<uintptr_t>(base + 0xA88);
				result = Config::Get().RPM<uintptr_t>(result + 0x198);
				result = Config::Get().RPM<uintptr_t>(result + 0x8);
				result = Config::Get().RPM<uintptr_t>(result + 0x0);
			}
		}

		return result + 0x460;
	}
	__except (1) {

	}
}
//void ReadView() // 뷰메 쓰레드
//{
//	
//	uint64_t viewMatrixVal = Config::Get().RPM<uint64_t>(Config::Get().BaseAddress + offset::ViewMatrixOffset); //0x478+0x568+0x198+0x60+0x460
//	viewMatrixVal = Config::Get().RPM<uint64_t>(viewMatrixVal + 0x478);
//	viewMatrixVal = Config::Get().RPM<uint64_t>(viewMatrixVal + 0x560);
//	viewMatrixVal = Config::Get().RPM<uint64_t>(viewMatrixVal + 0x5e8);
//	viewMatrixVal = Config::Get().RPM<uint64_t>(viewMatrixVal + 0x60);
//	viewMatrixPtr = viewMatrixVal + 0x430;
//	viewMatrix = Config::Get().RPM<Matrix>(viewMatrixPtr);
//	viewMatrixTo = Config::Get().RPM<MatrixTo>(viewMatrixPtr);
//}


void StructT() //네임태그 쓰레드
{
	while (true)
	{
		vector<ULONG64>tempEntityPTR = EntityPTR;
		if (tempEntityPTR.size())
		{
			for (int i = 0; i < EntityPTR.size(); i++)
			{
				clock_t currTime = clock();
				Entitys[i].Location = Config::Get().RPM<Vector3>(EntityPTR[i] + 0x5A);
				Entitys[i].Location.y -= Config::Get().YPITCH;
				if (Entitys[i].lastWallUpdate + 120 >= currTime) { Entitys[i].Wall = true; }
				else { Entitys[i].Wall = false; }
				//Entitys[i].Enemy = ((Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) <= 0x40) || Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) >= 0xD0) || Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) == 0x8 || Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) == 0xCA || Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) == 0x80 ? true : false; // 팀구분 ? true : false;
				//Entitys[i].Alive = (Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) != 0x60) && Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) != 0xB4 ? true : false;
				Entitys[i].Enemy = (Config::Get().RPM<BYTE>(EntityPTR[i] + 0x4) <= 0x14 || Config::Get().RPM<BYTE>(EntityPTR[i] + 0x4) >= 0xCA) ? true : false;
				//Entitys[i].Alive = (Config::Get().RPM<BYTE>(EntityPTR[i] + 0x4) != 0x60 && Config::Get().RPM<BYTE>(EntityPTR[i] + 0x4) != 0xB4 && Config::Get().RPM<BYTE>(EntityPTR[i] + 0x4) != 0xBC) ? true : false;
				Entitys[i].Alive = (Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) != 0x60 && Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) != 0xB4 && Config::Get().RPM<BYTE>(tempEntityPTR[i] + 0x4) != 0xBC) ? true : false;
			}
			this_thread::sleep_for(1ms);
			viewMatrix = Config::Get().RPM<Matrix>(GetViewMatrix());
			viewMatrixTo = Config::Get().RPM<MatrixTo>(GetViewMatrix());
		}
		else
		{
			this_thread::sleep_for(5ms);
		}

	}
}

void AimCorrection(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed, float Gravity)
{
	if (Config::Get().GravityBool)
	{
		float m_time = (Distance / Bulletspeed);

		(*InVecArg).x = (*InVecArg).x + ((currVelocity.x) * (Distance / (Bulletspeed)));
		(*InVecArg).y = (*InVecArg).y + ((currVelocity.y) * (Distance / (Bulletspeed)));
		(*InVecArg).z = (*InVecArg).z + ((currVelocity.z) * (Distance / (Bulletspeed)));

		(*InVecArg).y += (0.5f * 9.807f * m_time * m_time);
	}
	else
	{
		(*InVecArg).x = (*InVecArg).x + ((currVelocity.x) * (Distance / (Bulletspeed)));
		(*InVecArg).y = (*InVecArg).y + ((currVelocity.y) * (Distance / (Bulletspeed)));
		(*InVecArg).z = (*InVecArg).z + ((currVelocity.z) * (Distance / (Bulletspeed)));
	}
}

void RemovePeHeader(HMODULE hModule)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

	if (pNTHeader->FileHeader.SizeOfOptionalHeader)
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect((void*)hModule, Size, PAGE_EXECUTE_READWRITE, &Protect);
		SecureZeroMemory((void*)hModule, Size);
		VirtualProtect((void*)hModule, Size, Protect, &Protect);
	}
}

//uint64_t GetComponentParent()
//{
//	__try
//	{
//		uint64_t v1 = (0xC9AE31ABF507303Ai64 - __ROR8__(Config::Get().BaseAddress + 0x2CF113E, 0xC));
//		return v1;
//	}
//	__except (1)
//	{
//
//	}
//}

void GetKey(uint64_t* a0, uint64_t* a1)
{
	__try
	{
		spoof_call((PVOID)(Config::Get().BaseAddress + 0x1D90520), reinterpret_cast<uint64_t(__fastcall*)(uint64_t*, uint64_t*)>(Config::Get().BaseAddress + 0xb8e80), a0, a1); //0x24821a0
	}
	__except (1) {
	}
}

//uint64_t fnDecryptChildInParent(uint64_t parent, uint8_t compid) // 컴포넌트 복호화
//{
//	while ((Config::Get().HanzoAimbot) || (Config::Get().TPAimbot) || (Config::Get().MubanH));
//	{
//		__try
//		{
//			if (parent)
//			{
//				//unsigned __int64 v1 = parent;
//				//unsigned __int64 v2 = (uint64_t)1 << (uint64_t)(compid & 0x3F);
//				//unsigned __int64 v3 = v2 - 1;
//				//unsigned __int64 v4 = compid & 0x3F;
//				//unsigned __int64 v5 = compid / 0x3F;
//				//unsigned __int64 v6 = Config::Get().RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0x88);
//				//__int64 v7 = (v2 & Config::Get().RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0x88)) >> v4;
//				//unsigned __int64 v8 = (v3 & v6) - (((v3 & v6) >> 1) & 0x5555555555555555);
//				//unsigned __int64* v9 = (uint64_t*)(Config::Get().RPM<uint64_t>(v1 + 0x58) + 8 * (Config::Get().RPM<uint8_t>((uint32_t)v5 + v1 + 0xA8) + ((0x101010101010101 * (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333) + (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333)) >> 4)) & 0xF0F0F0F0F0F0F0F)) >> 0x38)));
//				//uint64_t Key1 = 0x7DD8D36C0E255712i64;// 키패치1
//				//uint64_t Key2 = 0x4CF8886B3307AFC7i64;// 키패치2
//				//GetKey(&Key2, &Key1);
//				//uint64_t v10 = Key2 ^ ((unsigned int)*v9 | (((unsigned int)(*v9 - (*(_QWORD*)(Config::Get().BaseAddress + 0x2C72850 + (Key1 >> 0x34)) >> 0x20)) ^ (((unsigned int)*v9 | (((unsigned __int64)((unsigned int)*v9 ^ 0x3307AFC7) ^ (*v9 >> 0x20)) << 0x20)) >> 0x20)) << 0x20));
//				//uint64_t v11 = (unsigned int)v10 | (((unsigned int)(v10 + __ROL4__(*(_QWORD*)(Config::Get().BaseAddress + 0x2C72850 + (v10 & 0xFFF)), 1)) ^ (Key1 >> 0x20)) << 0x20);
//				//uint64_t v12 = -(int)v7 & ((unsigned int)v10 | (((unsigned int)v11 ^ 0x4CF8886B ^ (v11 >> 0x20)) << 0x20));
//				//return v12;
//				unsigned __int64 v1 = parent;
//				unsigned __int64 v2 = (uint64_t)1 << (uint64_t)(compid & 0x3F);
//				unsigned __int64 v3 = v2 - 1;
//				unsigned __int64 v4 = compid & 0x3F;
//				unsigned __int64 v5 = compid / 0x3F;
//				unsigned __int64 v6 = Config::Get().RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0x88);
//				__int64 v7 = (v2 & Config::Get().RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0x88)) >> v4;
//				unsigned __int64 v8 = (v3 & v6) - (((v3 & v6) >> 1) & 0x5555555555555555);
//				unsigned __int64* v9 = (uint64_t*)(Config::Get().RPM<uint64_t>(v1 + 0x58) + 8 * (Config::Get().RPM<uint8_t>((uint32_t)v5 + v1 + 0xA8) + ((0x101010101010101 * (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333) + (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333)) >> 4)) & 0xF0F0F0F0F0F0F0F)) >> 0x38)));
//				uint64_t Key1 = 0x803EF3AA5AA13E0Ei64;
//				uint64_t Key2 = 0xA4E276689FA38C85i64;
//				spoof_call((PVOID)(Config::Get().BaseAddress + 0x1d7c140), reinterpret_cast<uint64_t(__fastcall*)(uint64_t*, uint64_t*)>(Config::Get().BaseAddress + 0xb7620), &Key2, &Key1);
//				uint64_t v10 = Key2 ^ ((unsigned int)*v9 | (((unsigned int)~*(_DWORD*)v9 ^ __ROR4__(*(_QWORD*)(Config::Get().BaseAddress + 0x2C668C0 + (Key1 >> 0x34)) >> 0x20, 0xB) ^ (((unsigned int)*v9 | (((unsigned __int64)((unsigned int)*v9 ^ 0x9FA38C85) ^ (*v9 >> 0x20)) << 0x20)) >> 0x20)) << 0x20));
//				uint64_t v11 = __ROL4__(*(_QWORD*)(Config::Get().BaseAddress + 0x2C668C0 + (Key1 & 0xFFF)), 1);
//				uint64_t v12 = -(int)v7 & ((unsigned int)v10 | (((unsigned int)v10 ^ 0x5B1D8997 ^ (((unsigned int)v10 | (((unsigned int)(v10 + v11) ^ (v10 >> 0x20)) << 0x20)) >> 0x20)) << 0x20));
//				return v12;
//			}
//		}
//		__except (1) {
//		}
//	}
//
//	
//}
//
//uint64_t GetComponent(uint64_t ComponentParent, DWORD ComponentID) // 컴포넌트 불러오기
//{
//	while ((Config::Get().HanzoAimbot) || (Config::Get().TPAimbot) || (Config::Get().MubanH));
//	{
//		__try
//		{
//			uint64_t Result = 0;
//
//			uint64_t pPointerTable = ComponentParent + 0x58;
//
//			uint64_t PointerTable = Config::Get().RPM<uint64_t>(pPointerTable + 0x0);
//			uint32_t PointerCount = Config::Get().RPM<uint32_t>(pPointerTable + 0x8);
//
//			if (IsBadReadPtr((PVOID)(PointerTable), 0x8 * PointerCount))
//				return 0;
//
//			for (int i = 0; i < PointerCount; i++)
//			{
//				uint64_t CurrentComponent = fnDecryptChildInParent(ComponentParent, ComponentID);
//
//				if (!CurrentComponent || IsBadReadPtr((PVOID)CurrentComponent, 0x510))
//					continue;
//
//				if (*(BYTE*)(CurrentComponent + 0x10) == ComponentID)
//				{
//					Result = CurrentComponent;
//					break;
//				}
//			}
//
//			return Result;
//		}
//		__except (1)
//		{
//			return 0;
//		}
//	}
//	
//}

int TarGetIndex = -1;

Vector3 GetVector3Predit()
{
	Vector3 target = Vector3(0, 0, 0);
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = 100000.f;

	if (TarGetIndex == -1)
	{
		if (EntityPTR.size())
		{
			for (int i = 0; i < EntityPTR.size(); i++)
			{
				if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].Wall)
				{

					Vector3 world = Entitys[i].BonePos;
					AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);

					Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
					Vector2 RealVe2 = Vector2(Vec2.x - CrossHair.x, Vec2.y - CrossHair.y);
					float CrossDist = CrossHair.Distance(Vec2);
					if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[i].Size)
					{
						target = world;
						origin = CrossDist;
						TarGetIndex = i;
					}
					else
					{
						TarGetIndex = -1;
					}
				}
				else
				{
					TarGetIndex = -1;
				}

			}
		}
	}
	else
	{
		if (Entitys[TarGetIndex].Alive && Entitys[TarGetIndex].Enemy)
		{
			Vector3 world = Entitys[TarGetIndex].BonePos;
			AimCorrection(&world, Entitys[TarGetIndex].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);

			Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);

			float CrossDist = CrossHair.Distance(Vec2);
			if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[TarGetIndex].Size)
			{
				target = world;
				origin = CrossDist;
			}
			else
			{
				TarGetIndex = -1;

			}
		}
		else
		{
			TarGetIndex = -1;
		}
	}

	return target;
}

Vector3 GetVector3Predit1()
{
	Vector3 target = Vector3(0, 0, 0);
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = 100000.f;

	if (TarGetIndex == -1)
	{
		if (EntityPTR.size())
		{
			for (int i = 0; i < EntityPTR.size(); i++)
			{
				if (Entitys[i].Alive && Entitys[i].Enemy)
				{
					Vector3 world = Entitys[i].Location;
					AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);

					Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
					Vector2 RealVe2 = Vector2(Vec2.x - CrossHair.x, Vec2.y - CrossHair.y);
					float CrossDist = CrossHair.Distance(Vec2);
					if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[i].Size)
					{
						target = world;
						origin = CrossDist;
						TarGetIndex = i;
					}
					else
					{
						TarGetIndex = -1;
					}
				}
				else
				{
					TarGetIndex = -1;
				}

			}
		}
	}
	else
	{
		if (Entitys[TarGetIndex].Alive && Entitys[TarGetIndex].Enemy)
		{
			Vector3 world = Entitys[TarGetIndex].BonePos;
			AimCorrection(&world, Entitys[TarGetIndex].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);

			Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);

			float CrossDist = CrossHair.Distance(Vec2);
			if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[TarGetIndex].Size)
			{
				target = world;
				origin = CrossDist;
			}
			else
			{
				TarGetIndex = -1;

			}
		}
		else
		{
			TarGetIndex = -1;
		}
	}

	return target;
}

Vector3 GetVector31()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = 100000.f;

	if (TarGetIndex == -1)
	{
		if (EntityPTR.size())
		{
			for (int i = 0; i < EntityPTR.size(); i++)
			{
				if (Entitys[i].Alive && Entitys[i].Enemy)
				{
					Vector2 Vec2 = viewMatrix.WorldToScreen(Entitys[i].BonePos, 1920, 1080);
					float CrossDist = CrossHair.Distance(Vec2);

					if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[i].Size)
					{
						target = Entitys[i].Location;
						origin = CrossDist;
						TarGetIndex = i;
					}
					else
					{
						TarGetIndex = -1;
					}
				}
				else
				{
					TarGetIndex = -1;
				}

			}
		}
	}
	else
	{
		if (Entitys[TarGetIndex].Alive && Entitys[TarGetIndex].Enemy)
		{
			Vector2 Vec2 = viewMatrix.WorldToScreen(Entitys[TarGetIndex].BonePos, 1920, 1080);
			float CrossDist = CrossHair.Distance(Vec2);

			if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[TarGetIndex].Size)
			{
				target = Entitys[TarGetIndex].BonePos;
				origin = CrossDist;
			}
			else
			{
				TarGetIndex = -1;

			}
		}
		else
		{
			TarGetIndex = -1;
		}
	}

	return target;
}

Vector3 GetVector3()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = 100000.f;

	if (TarGetIndex == -1)
	{
		if (EntityPTR.size())
		{
			for (int i = 0; i < EntityPTR.size(); i++)
			{
				if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].Wall)
				{
					Vector2 Vec2 = viewMatrix.WorldToScreen(Entitys[i].BonePos, 1920, 1080);
					float CrossDist = CrossHair.Distance(Vec2);

					if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[i].Size)
					{
						target = Entitys[i].BonePos;
						origin = CrossDist;
						TarGetIndex = i;
					}
					else
					{
						TarGetIndex = -1;
					}
				}
				else
				{
					TarGetIndex = -1;
				}

			}
		}
	}
	else
	{
		if (Entitys[TarGetIndex].Alive && Entitys[TarGetIndex].Enemy && Entitys[TarGetIndex].Wall)
		{
			Vector2 Vec2 = viewMatrix.WorldToScreen(Entitys[TarGetIndex].BonePos, 1920, 1080);
			float CrossDist = CrossHair.Distance(Vec2);

			if (CrossDist < origin && CrossDist < Config::Get().Fov * Entitys[TarGetIndex].Size)
			{
				target = Entitys[TarGetIndex].BonePos;
				origin = CrossDist;
			}
			else
			{
				TarGetIndex = -1;

			}
		}
		else
		{
			TarGetIndex = -1;
		}
	}

	return target;
}

int TarGetIndex2 = -1;


Vector3 GetVectorDistance()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	//float origin = FLT_MAX;
	float origin = 10000.0f;
	if (EntityPTR.size())
	{
		for (int i = 0; i < EntityPTR.size(); i++)
		{
			
			Vector3 mob = Entitys[i].Location;
			Entitys[i].BonePos.x = mob.x;
			Entitys[i].BonePos.z = mob.z;
			Vector3 world = Entitys[i].BonePos;
			AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), 1000.f, 9.8f);
			//AimCorrection(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed, float Gravity)
			if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].Wall)
			{
				Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
				float CrossDist = viewMatrix.GetCameraVec().Distance(world);

				if (CrossDist < origin)
				{
					target = world;
					origin = CrossDist;
					TarGetIndex2 = -1;
				}
			}

		}
	}
	return target;
}

Vector3 GetVector3123123()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = FLT_MAX;

	if (EntityPTR.size())
	{
		for (int i = 0; i < EntityPTR.size(); i++)
		{
			Vector3 world = Entitys[i].BonePos;
			AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);
			if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].Wall)
			{
				Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
				float CrossDist = viewMatrix.GetCameraVec().Distance(world);

				if (CrossDist < origin)
				{
					target = world;
					origin = CrossDist;
					TarGetIndex2 = i;
				}
			}

		}
	}
	return target;
}

Vector3 GetVector3123123Health()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = FLT_MAX;

	if (EntityPTR.size())
	{
		for (int i = 0; i < EntityPTR.size(); i++)
		{
			Vector3 world = Entitys[i].BonePos;
			AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);
			if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].PlayerHealth <= 55.f)
			{
				Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
				float CrossDist = viewMatrix.GetCameraVec().Distance(world);

				if (CrossDist < origin)
				{
					target = world;
					origin = CrossDist;
					TarGetIndex2 = i;
				}
			}

		}
	}
	return target;
}

Vector3 GetVector3123123Health1()
{
	Vector3 target{};
	Vector2 CrossHair = Vector2(1920 / 2.0f, 1080 / 2.0f);

	float origin = FLT_MAX;

	if (EntityPTR.size())
	{
		for (int i = 0; i < EntityPTR.size(); i++)
		{
			Vector3 world = Entitys[i].BonePos;
			if (Entitys[i].Alive && Entitys[i].Enemy && Entitys[i].PlayerHealth <= 35.f)
			{
				Vector2 Vec2 = viewMatrix.WorldToScreen(world, 1920, 1080);
				float CrossDist = viewMatrix.GetCameraVec().Distance(world);

				if (CrossDist < origin)
				{
					target = world;
					origin = CrossDist;
					TarGetIndex2 = i;
				}
			}
		}
	}
	return target;
}

enum eInput
{
	INPUT_NULL = 0x0,
	INPUT_LMOUSE_UNHOLD = 0x1000,
	INPUT_LMOUSE = 0x1,
	INPUT_RMOUSE = 0x2,
	INPUT_INTERACTION = 0x4,
	INPUT_SKILL1 = 0x8,
	INPUT_SKILL2 = 0x10,
	INPUT_ULT = 0x20,
	INPUT_JUMP = 0x40,
	INPUT_DUCK = 0x80,
	INPUT_RELOAD = 0x400,
	INPUT_MELEE = 0x800,
};

void clearTimestamp(uint64_t m_getSkillStruct, uint32_t skillIdx, uint32_t skillIdx2)
{
	__try
	{
		if (SkillBase)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + 0xD0;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return;
			Config::Get().WPM<uint32_t>(skill + 0x48, 1);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}

uint32_t getTimestamp(uint64_t m_getSkillStruct, uint32_t skillIdx, uint32_t skillIdx2)
{
	__try
	{
		if (SkillBase)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + 0xD0;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return NULL;
			return Config::Get().RPM<uint32_t>(skill + 0x50);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
	return NULL;
}

//Genji 7, 0x2BB //0, 0x28E3 ;dash
//Sombra 0, 0x7C5 //0, 0x25A8 ;teleport invisible
//Moira 0, 0xA0F
//Zarya 5, 0x2BB //7, 0x6D8 ;ult
//Reaper 1, 0x2BB
//Doomfist 0, 0x254
//Mei 6, 0x16F3
//Tracer 0, 0x34A
//Hanjo 0, 0x1DB5
//Reinhardt 0, 0x156
//Lucio 0, 0x34A
//Sigma 6, 0xA0F
void Jump()
{
	while (1)
	{
		if (Config::Get().Jump)
		{
			Config::Get().WPM<uint64_t>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x40);
			Sleep(100);
		}
	}

}

bool isSkillAvtivate(uint64_t m_getSkillStruct, uint16_t skillIdx, uint16_t skillIdx2)
{
	__try
	{
		if (SkillBase)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + 0xD0;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return false;
			return Config::Get().RPM<uint8_t>(skill + 0x48) == 1;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
	return false;
}

void SetKey(uint32_t Key)
{
	Config::Get().WPM<uint32_t>(AnglePTR + 0x1034, Key);
}

bool isTargetable(uint64_t m_getSkillStruct)
{
	if (SkillBase)
	{
		if (!(eSkinEnum == 7 && isSkillAvtivate(m_getSkillStruct, 7, 0x2BB)) &&
			!(eSkinEnum == 7 && isSkillAvtivate(m_getSkillStruct, 0, 0x28E3)) &&
			!(eSkinEnum == 21 && isSkillAvtivate(m_getSkillStruct, 0, 0x7C5)) &&
			!(eSkinEnum == 14 && isSkillAvtivate(m_getSkillStruct, 0, 0xA0F)) &&
			!(eSkinEnum == 28 && isSkillAvtivate(m_getSkillStruct, 5, 0x2BB)) &&
			!(eSkinEnum == 17 && isSkillAvtivate(m_getSkillStruct, 1, 0x2BB)) &&
			!(eSkinEnum == 5 && isSkillAvtivate(m_getSkillStruct, 0, 0x254)))
		{
			return true;
		}
	}
	return false;
}


void Taimbot() //에임봇 쓰레드
{
	while (TRUE)
	{
		//uint64_t pAngle = Config::Get().RPM<uint64_t>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
		bool shooted = false;
		BOOL IsMouse = FALSE;






		static float origin_sens = 0.f;
		if (Config::Get().RPM<float>(GetSenstivePTR()))
			origin_sens = Config::Get().RPM<float>(GetSenstivePTR());
		else if (origin_sens)
			Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);

		if (Config::Get().TPAimbot)
		{
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
					}
				}
				this_thread::sleep_for(1ms);
			}
		}

		if (Config::Get().TFAimbot) // 너 살아있냐 ? 이소스 압축해서 나한테 보내봐 VC 인가 그파일 삭제하고 ㅇㅋ ?
		{
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (Config::Get().LSGlogic)
				{
					if (!shooted)
					{
						Vector3 world = GetVector3Predit();
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
						{
							float speed = (Config::Get().LSGspeed / currentAngle.Distance(angle)) * 1.3f;
							Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= 15.f / viewMatrix.GetCameraVec().Distance(world))
							{
								Sendinput::SendVKcodesUp(0x4C);
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
								shooted = true;
								if (Config::Get().UseLSG)
								{
									Sleep(Config::Get().LSGdelay);
									shooted = false;
								}
							}
						}
					}
				}
				else
				{
					if (!shooted)
					{
						Vector3 world = GetVector3Predit();
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
						{
							float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
							speed = max(0.850f, speed);
							speed = min(Config::Get().AimSpeed, speed);
							Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= 15.f / viewMatrix.GetCameraVec().Distance(world))
							{
								Sendinput::SendVKcodesUp(0x4C);
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
								shooted = true;
								if (Config::Get().UseLSG)
								{
									Sleep(Config::Get().LSGdelay);
									shooted = false;
								}
							}
						}
					}

				}
				
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().Nf)
		{
			Config::Get().GravityBool = false;
			float acceleration = 0.0f;
			while (GetAsyncKeyState(Config::Get().m_AimKey1) || (GetAsyncKeyState(Config::Get().m_AimKey2)))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					if (world.x != 0 && world.y != 0 && world.z != 0)
					{
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= (Config::Get().Fov) / viewMatrix.GetCameraVec().Distance(world))
						{
							Vector3 smoothed = AcAngle(currentAngle, angle, (Config::Get().MC1 / 5000) + acceleration);
							acceleration += (Config::Get().MC1 / 5000) * (Config::Get().Accelerate / 100);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world))
							{
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
								acceleration = Config::Get().acceleration2;
								Sleep(2);
								shooted = true;
								if (Config::Get().su)
								{
									
									
								}
								
								if (Config::Get().UseLSG)
								{
									Sleep(Config::Get().LSGdelay);

									shooted = false;
								}
							}
						}
					}
					

				}
				this_thread::sleep_for(2ms);

			}
		}

		if (Config::Get().LSGMODE)
		{
			if (GetAsyncKeyState(Config::Get().m_AimKey3))
			{
				Config::Get().YPITCH = 0.5f;
				Config::Get().TriggerNOMU = 15.0f;
			}
		}

		if (Config::Get().LSGMODE)
		{
			if (GetAsyncKeyState(Config::Get().m_AimKey4))
			{
				Config::Get().YPITCH = 0.23f;
				Config::Get().TriggerNOMU = 8.0f;
			}
		}


		if (Config::Get().NLSGMODE)
		{
			if (GetAsyncKeyState(Config::Get().m_AimKey5))
			{
				Config::Get().YPITCH = 0.15f;
				Config::Get().TriggerNOMU = 8.5f;
			}
		}

		if (Config::Get().NLSGMODE)
		{
			if (GetAsyncKeyState(Config::Get().m_AimKey6))
			{
				Config::Get().YPITCH = 0.5f;
				Config::Get().TriggerNOMU = 15.0f;
			}
		}


		//if (Config::Get().Muban)
		//{
		//	
		//	Config::Get().GravityBool = false;
		//	Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
		//	while (GetAsyncKeyState(Config::Get().m_AimKey1) || (GetAsyncKeyState(Config::Get().m_AimKey2)))
		//	{

		//		Vector3 world = GetVector3();
		//		Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
		//		if ((Config::Get().T1) && (world.x != 0 && world.y != 0))
		//		{
		//			Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
		//			Sleep(15);
		//			Config::Get().T1 = false;


		//		}
		//		else if (!Config::Get().T1)
		//		{
		//			Vector3 A = Vector3(currentAngle.x * -1, -9999, currentAngle.z * -1);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, A);
		//			Sleep(Config::Get().sd2);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, currentAngle);
		//			Sleep(Config::Get().sd3);
		//		}
		//		this_thread::sleep_for(2ms);
		//	}

		//}

		//if (Config::Get().MubanH)
		//{
		//	Config::Get().GravityBool = true;
		//	Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
		//	while (GetAsyncKeyState(Config::Get().m_AimKey1) || (GetAsyncKeyState(Config::Get().m_AimKey2)))
		//	{

		//		Vector3 world = GetVector3Predit();
		//		Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
		//		if ((Config::Get().T2) && (world.x != 0 && world.y != 0))
		//		{
		//			Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
		//			Sendinput::SendVKcodesUp(0x4C);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
		//			Sleep(15);
		//			Config::Get().T2 = false;


		//		}
		//		else if (!Config::Get().T2)
		//		{
		//			Vector3 A = Vector3(currentAngle.x * -1, -9999, currentAngle.z * -1);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, A);
		//			Sleep(Config::Get().sd2);
		//			Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, currentAngle);
		//			Sleep(Config::Get().sd3);
		//		}
		//		this_thread::sleep_for(2ms);
		//	}
		//}

		if (Config::Get().HZSilent)
		{
			Config::Get().GravityBool = true;
			Config::Get().PreditLevel = 115.f;
			while (GetAsyncKeyState(Config::Get().m_AimKey2))
			{
				if (!shooted)
				{
					Config::Get().WPM<float>(GetSenstivePTR(), 0);
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
						if (GetAsyncKeyState(MK_LBUTTON))
						{
							Config::Get().Release = true;
							Sleep(90);
							Config::Get().Release = false;
							shooted = true;
						}
						else
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
							shooted = true;
							Sleep(245);
							shooted = false;

						}
																										  //Sendinput::SendVKcodesUp(0x4C); //L키? 
						//Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //폭풍화살
						Sleep(Config::Get().HZSDelay);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임
						shooted = true;
						Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
					}
				}
				this_thread::sleep_for(1ms);
			}
		}



		if (Config::Get().PSilent)
		{
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
						Sleep(20);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임
						shooted = true;
						if (Config::Get().UseLSG)
						{
							Sleep(Config::Get().LSGdelay);
							shooted = false;
						}
					}
				}
				/*this_thread::sleep_for(0ms);*/
			}
		}

		if (Config::Get().GFAimbot)
		{
			Config::Get().GravityBool = false;
			Config::Get().PreditLevel = 55.f;
			float acceleration = 0.0f;
			while (GetAsyncKeyState(Config::Get().m_AimKey2))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Vector3 smoothed = AcAngle(currentAngle, angle, (Config::Get().MC1 / 5000) + acceleration);
						acceleration += (Config::Get().MC1 / 5000) * (Config::Get().Accelerate / 100);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Gtrig / viewMatrix.GetCameraVec().Distance(world))
						{
							acceleration = 0.15f;
							Sleep(2);
							shooted = true;
							if (viewMatrix.GetCameraVec().Distance(world) > Config::Get().gfd3 / viewMatrix.GetCameraVec().Distance(world))
							{
								int abcd;
								for (abcd = 1; abcd < 2; ++abcd)
								{
									Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);//840
									shooted = false;
									Sleep(64);


								}
								
								
						
								
								if (Config::Get().GFAimbot)
								{
									Sleep(Config::Get().gfd1);
									shooted = false;
								}
							}
								
							
							else
							{
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 2);//600

								if (Config::Get().GFAimbot)
								{
									Sleep(Config::Get().gfd2);
									shooted = false;
								}
							}
								
						}
					}
				}
				this_thread::sleep_for(1ms);
			}
		}
		//		if (Config::Get().FAimbot)
		//{
		//	Config::Get().GravityBool = false;
		//	while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
		//	{
		//		if (!shooted)
		//		{
		//			Vector3 world = GetVector3();
		//			Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
		//			Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
		//			if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world)) //만약에 상대방이 포브 범위 내에 있을때
		//			{
		//				float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
		//				speed = max(0.850f, speed);
		//				speed = min(Config::Get().AimSpeed, speed);
		//				Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
		//				Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
///////////
		//				if (currentAngle.Distance(angle) * (180.f / M_PI) <= 10.f / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
		//				{
		//					Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
		//					shooted = true; //발사중지
		//				}
		//			}
		//		}
		//		this_thread::sleep_for(2ms);
		//	}
		if (Config::Get().HanzoAimbot)
		{
			if (Config::Get().sexlogic)
			{
				Config::Get().GravityBool = true;
				float acceleration = 0.0f;
				while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
				{
					if (!shooted)
					{
						Vector3 world = GetVector3Predit();
						if (world.x != 0 && world.y != 0 && world.z != 0)
						{
							Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
							Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= (Config::Get().Fov) / viewMatrix.GetCameraVec().Distance(world))
							{
								if (Config::Get().aimlock)
								{
									Config::Get().WPM<float>(GetSenstivePTR(), 0);
								}
								Vector3 smoothed = AcAngle(currentAngle, angle, (Config::Get().MC1 / 5000) + acceleration);
								acceleration += (Config::Get().MC1 / 5000) * (Config::Get().Accelerate / 100);
								Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
								if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world))
								{
									if (GetAsyncKeyState(MK_LBUTTON))
									{
										if (Config::Get().su)
										{
											
											
										}
										Config::Get().Release = true;
										Sleep(90);
										Config::Get().Release = false;
										shooted = true;
									}
									else
									{
										Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
										if (Config::Get().su)
										{
											
											
										}
										shooted = true;
										Sleep(245);
										shooted = false;

									}
									Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
								}
							}
						}
						
						
					}
					this_thread::sleep_for(2ms);
				}
			}
			else
			{
				Config::Get().GravityBool = true;
				while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
				{
					if (!shooted)
					{
						Vector3 world = GetVector3Predit();
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world))
						{
							if (Config::Get().aimlock)
							{
								Config::Get().WPM<float>(GetSenstivePTR(), 0);
							}
							float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
							speed = max(0.850f, speed);
							speed = min(Config::Get().AimSpeed, speed);
							Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world))
							{
								if (GetAsyncKeyState(MK_LBUTTON))
								{
									if (Config::Get().su)
									{
										
										
									}
									Config::Get().Release = true;
									Sleep(90);
									Config::Get().Release = false;
									shooted = true;
								}
								else
								{
									if (Config::Get().su)
									{
										
										
									}
									Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
									shooted = true;
									Sleep(245);
									shooted = false;

								}
								Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
							}
						}
					}
					this_thread::sleep_for(1ms);
				}
			}
		}



		if (Config::Get().TrigHanzoAimbot)
		{
			Config::Get().GravityBool = true;
			Config::Get().NameESP = true;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				Config::Get().PreditLevel = 115.f;
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					//						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 10.f / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
					//{
					//	Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
					//	shooted = true; //발사중지
					//}
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.0f, speed);
						speed = min(0.0f, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world))
						{
							if (GetAsyncKeyState(MK_LBUTTON))
							{
								Config::Get().Release = true;
								Sleep(90);
								Config::Get().Release = false;
								shooted = true;
							}
							else
							{
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
								shooted = true;
								Sleep(245);
								shooted = false;

							}
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().AnaBot)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey7))
			{
				Config::Get().PreditLevel = 52.f;
				Config::Get().YPITCH = 0.55f;
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x8);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
					}
				}
				this_thread::sleep_for(2ms);
			}
			while (GetAsyncKeyState(Config::Get().m_AimKey2))
			{
				Config::Get().YPITCH = 0.55f;
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world)&&(world.x!=0&& world.z!=0)) //만약에 상대방이 포브 범위 내에 있을때
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); 
							shooted = true; //발사중지
						}
					}
					this_thread::sleep_for(2ms);
				}
			}



			while (GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				Config::Get().PreditLevel = 105.0f;
				Config::Get().YPITCH = 0.55f;
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world)&&(world.x!=0&& world.z!=0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);//????????????? ?dhodhodhodho왜 이지랄임 폰트좀 내놔
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 15.f / viewMatrix.GetCameraVec().Distance(world))
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
							shooted = true;
						}

					}

				}
				this_thread::sleep_for(2ms);
			}
		}


		if (Config::Get().ESkill)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey7))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 10.f / viewMatrix.GetCameraVec().Distance(world))
						{
							shooted = true;
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().Roadhog)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey7))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x8);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 10.f / viewMatrix.GetCameraVec().Distance(world))
						{
							shooted = true;
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().realsil)
		{
			float acceleration = 0.0f;
			Config::Get().GravityBool = false;

			while (GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					if (world.x != 0 && world.y != 0 && world.z != 0)
					{
						Vector3 curr_angle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (world.x != 0 && world.y != 0)
						{
							//auto get_fov = [&]() -> float
							//{
							//	/*switch (localEntity.HeroID)
							//	{
							//	case Comp::eHero::HERO_ANA:
							//	case Compo::eHero::HERO_WIDOWMAKER:
							//		return 50.9395f;
							//	case Compo::eHero::HERO_ASHE:
							//		return 65.8005f;
							//	default:
							//		return 103.f;
							//	}*/
							//};

							auto angle_to_mouse = [&](Vector3 local, Vector3 target)
							{
								// 0.01f Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
								float curr_sens = Config::Get().RPM<float>(GetSenstivePTR());
								float tmp = atan2f(curr_angle.x, curr_angle.z) - atan2f(angle.x, angle.z);
								Vector2 delta = Vector2{ sinf(tmp), curr_angle.y - angle.y };
								//
								delta.x -= delta.x * 0.1f;
								delta.y -= delta.y * 0.1f;

								return Vector2{ round(delta.x / curr_sens), round(delta.y / curr_sens) };
							};

							auto result = angle_to_mouse(curr_angle, angle);

							/*input::left_down(result.X, result.Y);
							input::left_up(-result.X, -result.Y);*/

							float curr_sens = Config::Get().RPM<float>(GetSenstivePTR()) * 100.f;

							Vector2 out{};
							if (viewMatrix.WorldToScreen(world, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), out)) {
								out.x -= 960;
								out.y -= 540;
								out.x *= 11.5f / curr_sens;
								out.y *= 11.5f / curr_sens;
								input::left_down(out.x, out.y);
								input::left_up(-out.x, -out.y);
							}

							shooted = true;
						}
					}
				}
				std::this_thread::sleep_for(1ms);
			}
		}

		if (Config::Get().DomPredit)
		{
			Config::Get().GravityBool = false;
			Config::Get().PreditLevel = 45.f;
			while (GetAsyncKeyState(Config::Get().m_AimKey7))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && isTargetable(Config::Get().BaseAddress + ADDRESS_SKILL) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().TrQSkill)
		{
			Config::Get().GravityBool = false;
			Config::Get().PreditLevel = 15.f;
			while (GetAsyncKeyState(Config::Get().m_AimKey7))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3123123();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0) && viewMatrix.GetCameraVec().Distance(world) <= 3.5f)
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						Config::Get().WPM<BYTE>(AnglePTR + 0x1034, 0x20);
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().자동섬광)
		{
			Config::Get().PreditLevel = 20.f;
			Vector3 world1 = GetVector3123123Health1();
			while (viewMatrix.GetCameraVec().Distance(world1) <= 9.f)
			{
				if (!shooted)
				{

					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{

						float speed = (Config::Get().LSGspeed / currentAngle.Distance(angle)) * 1.3f;
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 20.f / viewMatrix.GetCameraVec().Distance(world))
						{
							Config::Get().WPM<uint64_t>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x10);
							shooted = true;
							if (Config::Get().자동섬광)
							{
								Sleep(10500);
								shooted = false;
							}
						}
					}

				}
			}
			
			
			this_thread::sleep_for(2ms);
				
			
		}
		if (Config::Get().자동평타)
		{
			Vector3 world1 = GetVector3123123Health1();
			while (viewMatrix.GetCameraVec().Distance(world1) <= 2.5f)
			{
				if (!shooted)
				{
					Vector3 world = GetVector3123123Health1();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						Config::Get().WPM<uint64_t>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x800);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 20.f / viewMatrix.GetCameraVec().Distance(world))
						{
							shooted = true;
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().SAimbot)
		{
			Config::Get().GravityBool = false;
			Config::Get().PreditLevel = 60.f;
			while (Config::Get().RPM<BYTE>(AnglePTR + OFFSET_GenjiQ) == 0xB6)
			{
				if (!shooted)
				{
					Vector3 world = GetVector3123123();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));

					if (viewMatrix.GetCameraVec().Distance(world) < 15.f && (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(VK_LBUTTON)) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Vector3 smoothed = SmoothAngle(currentAngle, angle, Config::Get().AimSpeed, Config::Get().AimSpeed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);

						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 30 / viewMatrix.GetCameraVec().Distance(world))
						{
							if (viewMatrix.GetCameraVec().Distance(world) > 5.f)
							{
								Config::Get().YPITCH = 2.f;
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x8);
							}


							else
							{
								Config::Get().YPITCH = 0.25f;
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
								if (Config::Get().SD)
								{
									Sleep(Config::Get().sd);
									shooted = false;
								}
							}
						}




					}

				}
				this_thread::sleep_for(2ms);
			}
		}
		

		if (Config::Get().GenjiRight)
		{
			//
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(VK_RBUTTON))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3123123();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));

					if ((world.x != 0 && world.z != 0 && world.y != 0) && viewMatrix.GetCameraVec().Distance(world) < 15.f)
					{
						Vector3 smoothed = SmoothAngle(currentAngle, angle, Config::Get().AimSpeed, Config::Get().AimSpeed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);

						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 30.f / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
						{
							if (viewMatrix.GetCameraVec().Distance(world) > 5.f)
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x8);
							else
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 2);

						}
					}
					else
					{
						Vector3 smoothed = SmoothAngle(currentAngle, angle, Config::Get().AimSpeed, Config::Get().AimSpeed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);

						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 30.f / viewMatrix.GetCameraVec().Distance(world))
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 2);
						}
					}

					shooted = false;
				}
				this_thread::sleep_for(1ms);
			}
			

			while (GetAsyncKeyState(VK_LBUTTON))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3Predit();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if ((world.x != 0 && world.z != 0 && world.y != 0) && currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 15.f / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
						{
							Sendinput::SendVKcodesUp(0x4C);
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
							shooted = true;
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}













		/*Config::Get().GravityBool = false;
		while (GetAsyncKeyState(VK_LBUTTON))
		{
			if (!shooted)
			{
				Vector3 world = GetVector3Predit();
				Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
				Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
				if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world))
				{
					Config::Get().PreditLevel = 50.f;
					float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
					speed = max(0.750f, speed);
					speed = min(Config::Get().AimSpeed, speed);
					Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
					Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
				}
			}
			this_thread::sleep_for(1ms);
		}*/
	









		if (Config::Get().ininput)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(MK_LBUTTON))
			{
				Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
				//this_thread::sleep_for(0ms);
			}
		}

		if (Config::Get().ininput)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(MK_RBUTTON))
			{
				Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 2);
				//this_thread::sleep_for(0ms);
			}
		}









		if (Config::Get().GENJISHIFT)
		{
			Config::Get().GravityBool = false;
			Config::Get().PreditLevel = 60.f;
			Vector3 world = GetVector3123123Health();
			Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
			Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));

			if (viewMatrix.GetCameraVec().Distance(world) <= 15.f && (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1) && (world.x != 0 && world.z != 0 && world.y != 0)))
			{
				Vector3 smoothed = SmoothAngle(currentAngle, angle, Config::Get().AimSpeed, Config::Get().AimSpeed);
				Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);

				if (currentAngle.Distance(angle) * (180.f / M_PI) <= 50.f / viewMatrix.GetCameraVec().Distance(world))
				{
					Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x8);
				}

			}
			this_thread::sleep_for(2ms);
		}

		if (Config::Get().TAimbot)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.750f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smooted = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smooted);
						shooted = false;
					}
				}
				this_thread::sleep_for(1ms);
			}
		}

		if (Config::Get().Rage)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 12.f / viewMatrix.GetCameraVec().Distance(world))
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
							std::this_thread::sleep_for(10ms);
						}
					}
				}
			}
		}
		//Config::Get().WPM<float>(GetSenstivePTR(), 0); = 에임락
		//Config::Get().WPM<float>(GetSenstivePTR(), origin_sens); = 원래감도로복구
		if (Config::Get().FAimbot)
		{
			if (Config::Get().LSGlogic)  
			{
				while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))  
				{
					if (!shooted)
					{
						Vector3 world = GetVector3();
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0)) //만약에 상대방이 포브 범위 내에 있을때
						{
							if (Config::Get().aimlock)
							{
								Config::Get().WPM<float>(GetSenstivePTR(), 0);
							}
							float speed = (Config::Get().LSGspeed / currentAngle.Distance(angle)) * 1.3f;
							Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed);
							Sleep(Config::Get().shootdil);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
							{
								
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); 
								shooted = true; //발사중지
								if (Config::Get().su)
								{
									
									

								}
								if (Config::Get().UseLSG)
								{
									Sleep(Config::Get().LSGdelay);
									shooted = false;
								}
								Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
							}
							
						}
					}
					
					this_thread::sleep_for(2ms);
				}
			}
			else
			{
				while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1)) 
				{
					if (!shooted)
					{
						Vector3 world = GetVector3();
						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0)) //만약에 상대방이 포브 범위 내에 있을때
						{
							if (Config::Get().aimlock)
							{
								Config::Get().WPM<float>(GetSenstivePTR(), 0);
							}
							float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
							speed = max(750.f, speed);
							speed = min(Config::Get().AimSpeed, speed);
							Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
							Sleep(Config::Get().shootdil);
							if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
							{
								if (Config::Get().su)
								{
									
									
								}
								Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); 
								shooted = true;//발사중지
								if (Config::Get().UseLSG)
								{
									Sleep(Config::Get().LSGdelay);
									shooted = false;
								}
								Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
							}
							
						}
						
					}
					
					this_thread::sleep_for(2ms);
				}
				
			}
			
		}

		if (Config::Get().TrigAimbot)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0)) //만약에 상대방이 포브 범위 내에 있을때
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.0f, speed);
						speed = min(0.0f, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
						Sleep(Config::Get().shootdil);
						if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().TriggerNOMU / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
						{
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); 
							Sleep(Config::Get().exitdil);
							shooted = true; //발사중지
						}
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().GosAimbot)
		{
			Config::Get().AimSpeed = 0.025f;
			Config::Get().GravityBool = false;
			while(!shooted)
			{
				Vector3 world = GetVector3();
				Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
				Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
				if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
				{
					float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
					speed = max(0.750f, speed);
					speed = min(Config::Get().AimSpeed, speed);
					Vector3 smooted = SmoothAngle(currentAngle, angle, speed, speed);
					Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smooted);
					shooted = true;
				}
				this_thread::sleep_for(1ms);
			}
		}


		if (Config::Get().Silent)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Config::Get().WPM<float>(GetSenstivePTR(), 0);
						Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사

						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); 

						

						Sleep(Config::Get().silentdil);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임
						Config::Get().WPM<float>(GetSenstivePTR(), origin_sens);
						shooted = true;
						if (Config::Get().UseLSG)
						{
							Sleep(Config::Get().LSGdelay);
							shooted = false;
						}
					}
				}
				this_thread::sleep_for(0ms);
			}
		}

		




		if (Config::Get().SuperSilent)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0))
					{
						Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
						Sleep(15);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임
						shooted = true;
					}
				}
				//this_thread::sleep_for(0ms);
			}
		}

		if (Config::Get().Nf) //부스트 플릭
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0)) //만약에 상대방이 포브 범위 내에 있을때
					{
						float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
						speed = max(0.850f, speed);
						speed = min(Config::Get().AimSpeed, speed);
						Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동

						if (currentAngle.Distance(angle) * (180.f / M_PI) <= 13.f / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
						{
							for (int i = 0; i < 15; ++i)
							{
								float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
								speed = max(0.850f, speed);
								speed = min(Config::Get().AimSpeed, speed);
								Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
								Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
								Sleep(1);
							}
							Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
							for (int i = 0; i < 15; ++i)
							{
								float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
								speed = max(0.850f, speed);
								speed = min(Config::Get().AimSpeed, speed);
								Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
								Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동
								Sleep(1);
							}
							shooted = true; //발사중지
							if (Config::Get().UseLSG)
							{
								Sleep(Config::Get().LSGdelay);
								shooted = false;
							}
						}
					}
				}
				//this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().BoostSilent) //부스트 사일런트
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVector3();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					if (currentAngle.Distance(angle) * (180.f / M_PI) <= Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world) && (world.x != 0 && world.z != 0 && world.y != 0)) //적이 포브 범위 내에 있을때 이거 Yㄴ는 안함?
					{
						Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						shooted = true;
						for (int i = 0; i < 15; ++i)
						{
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동
							Sleep(1);
						}
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L키 발사
						for (int i = 0; i < 15; ++i)
						{
							Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동
							Sleep(1);
						}
						//Sleep(10);
						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임 이거는 머임 모르겠어요
					}
				}
				//this_thread::sleep_for(0ms);
			}
		}

		if (Config::Get().TriggerBot)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(VK_MBUTTON))
			{
				Vector3 world = GetVector3();
				XMFLOAT3 min{ -0.1f, -0.05f, -0.1f }, max{ 0.1f, 0.17f, 0.1f };
				XMFLOAT3 min2{ -0.16f, -0.3f, -0.16f }, max2{ 0.16f, 0.23f, 0.16f };
				XMFLOAT3 XMEnPos = XMFLOAT3(world.x, world.y, world.z);

				if (viewMatrix.IntersectRayWithAABB(viewMatrixTo, viewMatrixTo.GetCameraVec(), MyXMAngle, GetAsyncKeyState(VK_XBUTTON2) ? min : min2, GetAsyncKeyState(VK_XBUTTON2) ? max : max2, Config::Get().m_TriggerScale / 100.f, XMEnPos, GetAsyncKeyState(VK_XBUTTON2)) && isTargetable(Config::Get().BaseAddress + ADDRESS_SKILL))
				{
					Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
				}

				this_thread::sleep_for(0ms);
			}
		}

		if (Config::Get().Mecry)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(VK_MBUTTON))
			{
				Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
				this_thread::sleep_for(2ms);
			}
		}
		this_thread::sleep_for(25ms);


		//거리기반 에임봇
		if (Config::Get().DistanceTracking) //거리 트래킹
		{
			Vector3 world = GetVectorDistance();
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (world.x != 1 && world.z != 2 && world.y != 1)
				{
					if (!shooted)
					{

						Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
						Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
						Vector3 smoothed = SmoothAngle(currentAngle, angle, Config::Get().AimSpeed, Config::Get().AimSpeed);


						Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동

						shooted = false;

					}
					this_thread::sleep_for(1ms);
				}
				
			}
		}

		if (Config::Get().DistanceFlick)
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVectorDistance();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					float speed = (Config::Get().AimSpeed / currentAngle.Distance(angle)) * 0.01f;
					speed = max(0.850f, speed);
					speed = min(Config::Get().AimSpeed, speed);
					Vector3 smoothed = SmoothAngle(currentAngle, angle, speed, speed);
					Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, smoothed); //에임 이동

					if (currentAngle.Distance(angle) * (180.f / M_PI) <= 13.f / viewMatrix.GetCameraVec().Distance(world)) //만약 상대방 머리에 에임이 닿는다면
					{
						Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1); //L 키 발사
						shooted = true; //발사중지
					}
				}
				this_thread::sleep_for(2ms);
			}
		}

		if (Config::Get().DistanceSilent) 
		{
			Config::Get().GravityBool = false;
			while (GetAsyncKeyState(Config::Get().m_AimKey2) || GetAsyncKeyState(Config::Get().m_AimKey1))
			{
				if (!shooted)
				{
					Vector3 world = GetVectorDistance();
					Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
					Vector3 SaveAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
					
					
					Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, angle);  //이동 후 발사
					Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 1);
					Sleep(15);
					Config::Get().WPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION, SaveAngle); //저장된 첫 에임
					shooted = true;
					

				}
				//this_thread::sleep_for(0ms);
			}
		}
	}
}


void TeamLSG()
{
	while (true)
	{
		while (Config::Get().Release)
		{
			Config::Get().WPM<BYTE>(AnglePTR + OFFSET_PLAYER_CONTROLLER_KEY, 0x0);
			Sleep(0.5);
		}

	}


}

enum eComponentType
{
	TYPE_ERROR = -1,
	TYPE_VELOCITY = 4,
	TYPE_TEAM = 25,
	TYPE_ROTATION = 39,
	TYPE_P_VISIBILITY = 44,
	TYPE_SKILL = 47,
	TYPE_HEALTH = 51,
	TYPE_P_HEROID = 75,
	TYPE_LINK = 0x2C,
};

unsigned __int64 __fastcall DecryptVis(__int64 a1) //벽구분 함수
{
	__int64 v1; // rbx
	unsigned __int64 v2; // rdi
	unsigned __int64 v3; // rax
	__int64 v4; // rbx
	unsigned __int64 v5; // rdx
	unsigned __int64 v6; // rcx
	__m128i v7; // xmm1
	__m128i v8; // xmm2
	__m128i v9; // xmm0
	__m128i v10; // xmm1

	v1 = a1;
	v2 = Config::Get().BaseAddress + 0x61EB10; // 어레이검색후 첫번째

	v3 = v2 + 0x8;

	DWORD64* VisibleKeyPTR = (DWORD64*)(Config::Get().BaseAddress + 0x2D2A600); // 두번째 값
	v4 = v2 ^ *(DWORD64*)((char*)&VisibleKeyPTR[((BYTE)v1 + 0x74) & 0x7F]
		+ (((unsigned __int64)(v1 - 0x1ED194AD58C0D68Ci64) >> 7) & 7)) ^ (v1 - 0x1ED194AD58C0D68Ci64);
	v5 = (v3 - v2 + 7) >> 3;
	v6 = 0i64;
	if (v2 > v3)
		v5 = 0i64;
	if (v5)
	{
		if (v5 >= 4)
		{
			ZeroMemory(&v7, sizeof(v7));
			ZeroMemory(&v8, sizeof(v8));
			do
			{
				v6 += 4i64;
				v7 = _mm_xor_si128(v7, _mm_loadu_si128((const __m128i*)v2));
				v9 = _mm_loadu_si128((const __m128i*)(v2 + 16));
				v2 += 0x20i64;
				v8 = _mm_xor_si128(v8, v9);
			} while (v6 < (v5 & 0xFFFFFFFFFFFFFFFCui64));
			v10 = _mm_xor_si128(v7, v8);
			__m128i P1 = _mm_xor_si128(v10, _mm_srli_si128(v10, 8));
			v4 ^= *(DWORD64*)&P1;
		}
		for (; v6 < v5; ++v6)
		{
			v4 ^= *(DWORD64*)v2;
			v2 += 8i64;
		}
	}
	return v4 ^ ~v3 ^ 0x1ED194AD58C0D68Ci64;
}

unsigned long RGBA2ARGB(int r, int g, int b, int a)
{
	return ((a & 0xff) << 24) + ((b & 0xff) << 16) + ((g & 0xff) << 8) + (r & 0xff);
}
//
//struct HeroID_Bone
//{
//	const char* heroName = "";
//	DWORD64 heroID = 0x0;
//	float heroHeadPos;
//	int defaultHeadBoneIndex;
//	HeroID_Bone() {}
//	HeroID_Bone(const char* name, DWORD64 id, int dhbi) : heroName(name), heroID(id), defaultHeadBoneIndex(dhbi) {}
//};
//
//std::array<HeroID_Bone, MAX_PATH> Head_HeroIDs
//{
//	HeroID_Bone("Ana", 0x2E000000000013B, 67),
//	HeroID_Bone("Ashe", 0x2E0000000000200, 125),
//	HeroID_Bone("Baptiste", 0x2E0000000000221, 167),
//	HeroID_Bone("Bastion", 0x2E0000000000015, 81),
//	HeroID_Bone("Brigitte", 0x2E0000000000195, 77),
//	HeroID_Bone("Doomfist", 0x2E000000000012F, 81),
//	HeroID_Bone("Dva", 0x2E000000000007A, 140), // 140
//	HeroID_Bone("Echo", 0x2E0000000000206, 42),
//	HeroID_Bone("Genji", 0x2E0000000000029, 53),
//	HeroID_Bone("Hanzo", 0x2E0000000000005, 38),
//	HeroID_Bone("Junkrat", 0x2E0000000000065, 100),
//	HeroID_Bone("Lucio", 0x2E0000000000079, 53),
//	HeroID_Bone("Mccree", 0x2E0000000000042, 53),
//	HeroID_Bone("Mei", 0x2E00000000000DD, 50),
//	HeroID_Bone("Mercy", 0x2E0000000000004, 150),
//	HeroID_Bone("Moira", 0x2E00000000001A2, 64),
//	HeroID_Bone("Orisa", 0x2E000000000013E, 50),
//	HeroID_Bone("Pharah", 0x2E0000000000008, 38),
//	HeroID_Bone("Reaper", 0x2E0000000000002, 36),
//	HeroID_Bone("Reinhardt", 0x2E0000000000007, 41),
//	HeroID_Bone("Roadhog", 0x2E0000000000040, 61),
//	HeroID_Bone("Soldier", 0x2E000000000006E, 38),
//	HeroID_Bone("Sombra", 0x2E000000000012E, 42),
//	HeroID_Bone("Sigma", 0x2E000000000023B, 180), //180
//	HeroID_Bone("Symmetra", 0x2E0000000000016, 89),
//	HeroID_Bone("Torbjorn", 0x2E0000000000006, 45),
//	HeroID_Bone("Tracer", 0x2E0000000000003, 52),
//	HeroID_Bone("Widowmaker", 0x2E000000000000A, 40),
//	HeroID_Bone("Winston", 0x2E0000000000009, 109),
//	HeroID_Bone("WreckingBall", 0x2E00000000001CA, 199),
//	HeroID_Bone("Zarya", 0x2E0000000000068, 65),
//	HeroID_Bone("Zenyatta", 0x2E0000000000020, 149),
//	HeroID_Bone("TrainingBot1", 0x2E000000000016B, 37),
//	HeroID_Bone("TrainingBot2", 0x2E000000000016C, 37),
//	HeroID_Bone("TrainingBot3", 0x2E000000000016D, 37),
//	HeroID_Bone("TrainingBot4", 0x2E000000000016E, 37),
//};
//
//enum eHero : uint64_t
//{
//	HERO_REAPER = 0x2E0000000000002,
//	HERO_TRACER = 0x2E0000000000003,
//	HERO_MERCY = 0x2E0000000000004,
//	HERO_HANJO = 0x2E0000000000005,
//	HERO_TORBJORN = 0x2E0000000000006,
//	HERO_REINHARDT = 0x2E0000000000007,
//	HERO_PHARAH = 0x2E0000000000008,
//	HERO_WINSTON = 0x2E0000000000009,
//	HERO_WIDOWMAKER = 0x2E000000000000A,
//	HERO_BASTION = 0x2E0000000000015,
//	HERO_SYMMETRA = 0x2E0000000000016,
//	HERO_ZENYATTA = 0x2E0000000000020,
//	HERO_GENJI = 0x2E0000000000029,
//	HERO_ROADHOG = 0x2E0000000000040,
//	HERO_MCCREE = 0x2E0000000000042,
//	HERO_JUNKRAT = 0x2E0000000000065,
//	HERO_ZARYA = 0x2E0000000000068,
//	HERO_SOLDIER76 = 0x2E000000000006E,
//	HERO_LUCIO = 0x2E0000000000079,
//	HERO_DVA = 0x2E000000000007A,
//	HERO_MEI = 0x2E00000000000DD,
//	HERO_ANA = 0x2E000000000013B,
//	HERO_SOMBRA = 0x2E000000000012E,
//	HERO_ORISA = 0x2E000000000013E,
//	HERO_DOOMFIST = 0x2E000000000012F,
//	HERO_MOIRA = 0x2E00000000001A2,
//	HERO_BRIGITTE = 0x2E0000000000195,
//	HERO_WRECKINGBALL = 0x2E00000000001CA,
//	HERO_ASHE = 0x2E0000000000200,
//	HERO_BAPTISTE = 0x2E0000000000221,
//	HERO_SIGMA = 0x2E000000000023B,
//	HERO_TRAININGBOT1 = 0x2E000000000016B,
//	HERO_TRAININGBOT2 = 0x2E000000000016C,
//	HERO_TRAININGBOT3 = 0x2E000000000016D,
//	HERO_TRAININGBOT4 = 0x2E000000000016E,
//};

LONG WINAPI Exc2pt10nHand1111er(EXCEPTION_POINTERS* e) //veh 훅 (메인)
{
	std::string EncryptEA = encrypt((UINT64)(e->ExceptionRecord->ExceptionAddress));
	auto ctx = e->ContextRecord;
	auto Fliters = e->ExceptionRecord->ExceptionAddress;
	if (EncryptEA == EnAngleHook) // 앵글훅
	{
		MyAngle = *(Vector3*)(ctx->Rsp + 0x20);
		AnglePTR = ctx->Rdi;
		*(Vector3*)(&ctx->Xmm0) = *(Vector3*)(ctx->Rsp + 0x20);
		ctx->Rip += 0x5;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if (Fliters == (PVOID)(Config::Get().BaseAddress + offset::BorderLine2)) {
		ctx->Rbp = ctx->Rsp;

		if (Config::Get().OutlineESP2)
		{
			ctx->Rdx = 0xC0 | 1;
		}

		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	} 
	


	if (EncryptEA == EnWallHook) //월핵
	{
		ctx->Rcx = ctx->Rsi;
		//printf("pVelocity : %IIX\n", ctx->Rcx);
		uint64_t Component04 = ctx->Rcx;
		RootPos = *(Vector3*)(ctx->Rcx + 0x160);
		Velocity = *(Vector3*)(ctx->Rcx + 0x50);
		//printf("RootPos : %f\n", RootPos.x);
		//printf("pVelocity : %IIX\n", ctx->Rcx);
		for (int i = 0; i < EntityPTR.size(); i++)
		{
			if (abs(RootPos.x - Entitys[i].Location.x) <= 1.f && abs(RootPos.z - Entitys[i].Location.z) <= 1.f) // 좌표를 비교하여 적에대한 정보를 얻기
			{
				Entitys[i].Velocity = Velocity; 
				Entitys[i].BonePos = Entitys[i].Location;
				//Entitys[i].Rotation = Config::Get().RPM<float>(Config::Get().RPM<uint64_t>(eRotation + 0x748) + 0xA7C);
				//printf("Entitys[%d] : %f\n", i, Entitys[i].Rotation);
				//uint64_t pBoneData = Config::Get().RPM<uint64_t>(ctx->Rcx + OFFSET_VELOCITYPTR_BONEDATA);
				//if (pBoneData)
				//{
				//	int boneIndex = NULL;
				//	auto it2 = std::find_if(Head_HeroIDs.begin(), Head_HeroIDs.end(), [&](HeroID_Bone& oh) {
				//		return oh.heroID == Config::Get().RPM<uint64_t>(eHeroID + 0xE8); });
				//	if (it2 != Head_HeroIDs.end())
				//		boneIndex = it2->defaultHeadBoneIndex;
				//	else
				//		continue;

				//	uint64_t bonesBase = Config::Get().RPM<uint64_t>(pBoneData + OFFSET_BONEDATA_BONEBASE);
				//	DirectX::XMFLOAT3 currentBone = Config::Get().RPM<DirectX::XMFLOAT3>(bonesBase + OFFSET_BONE_SIZE * boneIndex + OFFSET_BONE_LOCATION);
				//	DirectX::XMFLOAT3 Result{};
				//	XMMATRIX rotMatrix = XMMatrixRotationY(Config::Get().RPM<float>(Config::Get().RPM<uint64_t>(eRotation + 0x748) + 0xA7C)); //로테이션
				//	DirectX::XMStoreFloat3(&Result, XMVector3Transform(XMLoadFloat3(&currentBone), rotMatrix));

				//	if (Config::Get().RPM<uint64_t>(eHeroID + 0xE8) == eHero::HERO_WRECKINGBALL)
				//	{
				//		Entitys[i].BonePos = Vector3(Result.x, Result.y, Result.z) + RootPos - Vector3(0, 1.65f, 0);
				//	}
				//	if (Config::Get().RPM<uint64_t>(eHeroID + 0xE8) == eHero::HERO_DOOMFIST)
				//	{
				//		Entitys[i].BonePos = Vector3(Result.x, Result.y, Result.z) + RootPos - Vector3(0, 1.1f, 0);
				//	}
				//	else
				//	{
				//		Entitys[i].BonePos = Vector3(Result.x, Result.y, Result.z) + RootPos - Vector3(0, 1, 0);
				//	}
				//	break;
				//}
				break;
			}
		}

		if (Config::Get().OutlineESP)
		{
			ctx->Rdx = 0xC0;

			if (abs(RootPos.x - GetVector3().x) <= 0.5f && abs(RootPos.z - GetVector3().z) <= 0.5f)
			{
				ctx->R8 = RGBA2ARGB(Config::Get().E2SPColor.x * 255, Config::Get().E2SPColor.y * 255, Config::Get().E2SPColor.z * 255, Config::Get().E2SPColor.w * 255);
			}
		}

		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	//if (EncryptEA == EnHero)
	//{
	//	*(uint64_t*)(ctx->Rsi) = ctx->Rax;

	//	for (int i = 0; i < EntityPTR.size(); i++)
	//	{
	//		if (abs(RootPos.x - Entitys[i].Location.x) <= 1.f && abs(RootPos.z - Entitys[i].Location.z) <= 1.f) // 좌표를 비교하여 적에대한 정보를 얻기
	//		{
	//			eHeroID = ctx->R15; // r15 or rdi or rax
	//			break;
	//		}
	//	}
	//	ctx->Rip += 0x3;
	//	return EXCEPTION_CONTINUE_EXECUTION;
	//}

	//if (EncryptEA == EnRotationHook)
	//{
	//	*(uint64_t*)(ctx->Rbx) = *(uint64_t*)(&ctx->Xmm10);
	//	for (int i = 0; i < EntityPTR.size(); i++)
	//	{
	//		if (abs(RootPos.x - Entitys[i].Location.x) <= 1.f && abs(RootPos.z - Entitys[i].Location.z) <= 1.f) // 좌표를 비교하여 적에대한 정보를 얻기
	//		{
	//			eRotation = ctx->Rbx;
	//			break;
	//		}
	//	}
	//	ctx->Rip += 0x5;
	//	return EXCEPTION_CONTINUE_EXECUTION;
	//}

	//if (EncryptEA == EnCompoHook)
	//{
	//	ctx->Rdi = ctx->Rax;
	//	uint64_t ComponentBase = ctx->Rax;
	//	uint64_t ComponentParent = ctx->Rbx;
	//	unsigned char ComponentIDX = LOBYTE(ctx->Rcx);
	//	//printf("pHeroID : %IIX\n", eHeroID);

	//	if (ComponentIDX == eComponentType::TYPE_ROTATION)
	//	{
	//		eRotation = GetComponent(ComponentParent, eComponentType::TYPE_ROTATION);
	//		printf("pRotation : %IIX\n", eRotation);
	//	}

	//	ctx->Rip += 0x3;
	//	return EXCEPTION_CONTINUE_EXECUTION;
	//}

	if (EncryptEA == EnFovHook)
	{
		ctx->Rax = ctx->R9;
		DWORD Filter = *(DWORD*)(*(DWORD64*)(ctx->Rsp + 0x38) - 0xD);//여기는 28 아니면 38임 이것들은 맞거든??
		if (Filter == 0x8E8B49A5) //필터
		{
			DWORD mSkinChecker = *(DWORD*)(*(DWORD64*)(ctx->R14 + 0x40) + 0x48);
			DWORD eSkinChecker = *(DWORD*)(*(DWORD64*)(ctx->R14 + 0x48) + 0x48);
			EnPos = *(Vector3*)(ctx->Rsp + 0x300);//여기는 2F0 or 300 or 310 
			mSkinEnum = Skin2Hero(mSkinChecker);
			eSkinEnum = Skin2Hero(eSkinChecker);

			if (*(float*)(&ctx->Xmm7) == 0.01f || *(float*)(&ctx->Xmm8) == 0.01f)
			{
				MyPos = *(uint64_t*)(ctx->Rsp + 0x3B0); //
				uint64_t eParentPTR = *(uint64_t*)(ctx->R14 + 0x48); // 적 정보
				uint64_t mParentPTR = *(uint64_t*)(ctx->R14 + 0x40); // 내 정보
				for (int i = 0; i < EntityPTR.size(); i++)
				{
					if (abs(EnPos.x - Entitys[i].Location.x) <= 1.0f && abs(EnPos.z - Entitys[i].Location.z) <= 1.0f) // 좌표를 비교하여 적에대한 정보를 얻기
					{
						Entitys[i].lastWallUpdate = clock(); //벽뒤 현재값
						Entitys[i].HeroID = Config::Get().RPM<uint64_t>(eHeroID);

						Entitys[i].SkinID = Config::Get().RPM<WORD>(eParentPTR + 0x48);
					}
				}
			}
		}
		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	else if (EncryptEA == EnRayTrace) //월핵
	{
		RayRCX = *(uint64_t*)(ctx->Rcx + 0x1B8);
		RayRCX2 = *(uint64_t*)(ctx->Rcx + 0x1C0);

		*(uint64_t*)(ctx->Rsp + 0x8) = ctx->Rbx;
		ctx->Rip += 0x5;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void DrawQuad(const ImVec2& pos1, const ImVec2& pos2, const ImVec2& pos3, const ImVec2& pos4, ImU32 color, float thickness, float rounding)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddQuad(pos1, pos2, pos3, pos4, color, thickness);
}

void DrawQuadFilled(const ImVec2& pos1, const ImVec2& pos2, const ImVec2& pos3, const ImVec2& pos4, ImU32 color, float thickness, float rounding)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->AddQuadFilled(pos1, pos2, pos3, pos4, color);
}

//void DrawHealthBar(int width, const ImVec2& from, int currentHealth, int maxHealth, int currentArmor, int maxArmor, int currentBarrier, int maxBarrier)
//{
//	int height = width * 0.1;
//	int indent = width * 0.02;
//	int blockSize = width * 0.06;
//	int space = width * 0.01;
//	int nbBlock = width / blockSize;
//	int nbBlockArmor = 0;
//	if (maxArmor != 0 && currentArmor != 0) { //maxArmor != 0 &&
//		int nbBlockArmorMax = nbBlock * 0.25;
//		nbBlockArmor = (currentArmor * nbBlockArmorMax) / maxArmor;
//	}
//	int nbBlockBarrier = 0;
//	if (maxBarrier != 0 && currentBarrier != 0) { //maxBarrier != 0 &&
//		int nbBlockBarrierMax = nbBlock * 0.25;
//		nbBlockBarrier = (currentBarrier * nbBlockBarrierMax) / maxBarrier;
//	}
//	int nbBlockHealthFull = (currentHealth * (nbBlock - nbBlockArmor - nbBlockBarrier)) / maxHealth;
//
//	for (int i = 0; i < nbBlock; i++) {
//		int pos1X = from.x + (i * (blockSize + space));
//		int pos1Y = from.y;
//		ImVec2 pos1(pos1X, pos1Y);
//
//		int pos2X = from.x + blockSize + (i * (blockSize + space));
//		int pos2Y = from.y;
//		ImVec2 pos2(pos2X, pos2Y);
//
//		int pos3X = from.x + indent + blockSize + (i * (blockSize + space));
//		int pos3Y = from.y - height;
//		ImVec2 pos3(pos3X, pos3Y);
//
//		int pos4X = from.x + indent + (i * (blockSize + space));
//		int pos4Y = from.y - height;
//		ImVec2 pos4(pos4X, pos4Y);
//
//		if (i < nbBlockHealthFull) {
//			DrawQuadFilled(pos4, pos3, pos2, pos1, ImColor(255, 0, 0, 255), 0, 0);
//		}
//		else {
//			if (nbBlockArmor > 0 && i < (nbBlockHealthFull + nbBlockArmor)) {
//				DrawQuadFilled(pos4, pos3, pos2, pos1, ImColor(255, 220, 49, 255), 0, 0);
//			}
//			else {
//				if (nbBlockBarrier > 0 && i < (nbBlockHealthFull + nbBlockArmor + nbBlockBarrier)) {
//					DrawQuadFilled(pos4, pos3, pos2, pos1, ImColor(114, 189, 234, 255), 0, 0);
//				}
//				else {
//					DrawQuadFilled(pos4, pos3, pos2, pos1, ImColor(80, 80, 80, 125), 0, 0);
//				}
//			}
//		}
//	}
//}

void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
}

void Rainbow() {
	while (true)
	{
		if (Config::Get().sexsex)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 255);
			float R = dis(gen);
			float G = dis(gen);
			float B = dis(gen);
			Config::Get().SexCol = ImVec4(R, G, B, 0.9f);

		}
		Sleep(100); 
	}
}

void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
{
	ImGuiWindow* Draw = ImGui::GetCurrentWindow();
	float scale = (bot.y - top.y) / 3.0f;
	float length = scale / 2;
	Draw->DrawList->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 2.0f); //  --- Top left
	Draw->DrawList->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 2.0f); // | Top left
	Draw->DrawList->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 2.0f); // --- Top right
	Draw->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 2.0f); // | Top right
	Draw->DrawList->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 2.0f); // --- Bottom left
	Draw->DrawList->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 2.0f); // | Bottom left
	Draw->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 2.0f); // --- Bottom right
	Draw->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 2.0f); // | Bottom right
}

void DROW3D(const const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, const Vector2& p4, const Vector2& p5, const Vector2& p6, const Vector2& p7, ImU32 col)
{
	ImGuiWindow* Draw = ImGui::GetCurrentWindow();

	Draw->DrawList->AddLine(ImVec2(p1.x, p1.y), ImVec2(p0.x, p0.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p0.x, p0.y), ImVec2(p2.x, p2.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p1.x, p1.y), ImVec2(p3.x, p3.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p3.x, p3.y), ImVec2(p2.x, p2.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p4.x, p4.y), ImVec2(p0.x, p0.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p4.x, p4.y), ImVec2(p6.x, p6.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p6.x, p6.y), ImVec2(p2.x, p2.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p5.x, p5.y), ImVec2(p4.x, p4.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p5.x, p5.y), ImVec2(p7.x, p7.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p7.x, p7.y), ImVec2(p6.x, p6.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p5.x, p5.y), ImVec2(p1.x, p1.y), col, 2.0f);
	Draw->DrawList->AddLine(ImVec2(p7.x, p7.y), ImVec2(p3.x, p3.y), col, 2.0f);

}

void Menu::esp() // esp 기능 
{
	ImGuiWindow* window = ImGui::GetCurrentWindow(); // 임구이 오버레이
	for (int i = 0; i < EntityPTR.size(); i++)
	{
		viewMatrix = Config::Get().RPM<Matrix>(GetViewMatrix());
		Vector2 output{}, output2{};
		if (Entitys[i].Alive && Entitys[i].Enemy)
		{
			Vector3 Vecbone = Entitys[i].BonePos;
			Vector3 Vec3 = Entitys[i].Location;
			Vector3 world = Entitys[i].Location;

			AimCorrection(&world, Entitys[i].Velocity, viewMatrix.GetCameraVec().Distance(world), Config::Get().PreditLevel, 9.8f);

			if (viewMatrix.WorldToScreen(Vector3(Vec3.x, Vec3.y - 2.f, Vec3.z), 1920, 1080, output) && viewMatrix.WorldToScreen(Vector3(Vec3.x, Vec3.y - 0.f, Vec3.z), 1920, 1080, output2))
			{
				Vector2 WorldPos = viewMatrix.WorldToScreen(Vector3(world.x, world.y - 0.2f, world.z), 1920, 1080);
				Vector2 WorldPosz = viewMatrix.WorldToScreen(Vector3(world.x, world.y, world.z), 1920, 1080);
				float Size = abs(output.y - output2.y) / 2.0f;
				float Size2 = abs(output.y - output2.y) / 20.0f;
				float xpos = (output.x + output2.x) / 2;
				float xposz = (WorldPos.x + WorldPosz.x) / 2;
				float yposz = WorldPos.y;
				Entitys[i].Size = Size / 100.f;
				float ypos = output.y + Size / 2;

				Vector3 p1, p2, p3, p4, p5, p6, p7, p8;

				Vecbone.x = Vecbone.x + 0.5f;
				Vecbone.y = Vecbone.y + 0.5f;
				Vecbone.z = Vecbone.z + 0.5f;
				p1.x = Vecbone.x;
				p1.y = Vecbone.y;
				p1.z = Vecbone.z;
				p2.x = Vecbone.x;
				p2.y = Vecbone.y;
				p2.z = Vecbone.z - 1.0f;
				p3.x = Vecbone.x;
				p3.y = Vecbone.y - 2.0f;
				p3.z = Vecbone.z;
				p4.x = Vecbone.x;
				p4.y = Vecbone.y - 2.0f;
				p4.z = Vecbone.z - 1.0f;
				p5.x = Vecbone.x - 1.0f;
				p5.y = Vecbone.y;
				p5.z = Vecbone.z;
				p6.x = Vecbone.x - 1.0f;
				p6.y = Vecbone.y;
				p6.z = Vecbone.z - 1.0f;
				p7.x = Vecbone.x - 1.0f;
				p7.y = Vecbone.y - 2.0f;
				p7.z = Vecbone.z;
				p8.x = Vecbone.x - 1.0f;
				p8.y = Vecbone.y - 2.0f;
				p8.z = Vecbone.z - 1.0f;
				Vector2 po1 = viewMatrix.WorldToScreen(p1, 1920, 1080);
				Vector2 po2 = viewMatrix.WorldToScreen(p2, 1920, 1080);
				Vector2 po3 = viewMatrix.WorldToScreen(p3, 1920, 1080);
				Vector2 po4 = viewMatrix.WorldToScreen(p4, 1920, 1080);
				Vector2 po5 = viewMatrix.WorldToScreen(p5, 1920, 1080);
				Vector2 po6 = viewMatrix.WorldToScreen(p6, 1920, 1080);
				Vector2 po7 = viewMatrix.WorldToScreen(p7, 1920, 1080);
				Vector2 po8 = viewMatrix.WorldToScreen(p8, 1920, 1080);

				string dist = to_string((int)viewMatrix.GetCameraVec().Distance(Vec3)) + "M";

				Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
				Vector3 angle = CalcAngle(viewMatrix.GetCameraVec(), world, viewMatrix.GetCameraVec().Distance(world));
				int barSize = (ypos - ypos) * 1.5;
				int TextOffset = 0;

				if (Config::Get().NameESP)
				{
/*					ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
					window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f), ImGui::GetColorU32(Config::Get().ESPColor2), dist.c_str());*/ // 거리 색변경 
					window->DrawList->AddCircle(ImVec2(xposz, yposz), currentAngle.Distance(angle) * (180.f / M_PI) + Config::Get().Fov / viewMatrix.GetCameraVec().Distance(world), ImGui::GetColorU32(ImVec4(Config::Get().ESPColor2.x * 255, Config::Get().ESPColor2.y * 255, Config::Get().ESPColor2.z * 255, Config::Get().ESPColor2.w * 255)), 100, 0.5f);
				}
				if (Config::Get().BoneESP)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
					window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f), ImGui::GetColorU32(Config::Get().ESPColor3), dist.c_str()); //거리
					//ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
					/*window->DrawList->AddRect(ImVec2(xpos - Size / 1.5f, WorldPos.y - 10.z3), ImVec2(xpos + Size / 1.5f, WorldPosz.y + 30.8), ImGui::GetColorU32(Config::Get().ESPColor3));*/ //박스
				}//박스 첫 맨 위 높이 , 박스 크기
				if (Config::Get().SexESP)
				{
					window->DrawList->AddCircle(ImVec2(xpos, yposz), Config::Get().Sexl / viewMatrix.GetCameraVec().Distance(Vec3), ImGui::GetColorU32(Config::Get().SexCol), Config::Get().Sexlll, Config::Get().Sexll / viewMatrix.GetCameraVec().Distance(Vec3));
				}                 
				// 2.9 31.2 88.1

				if (Config::Get().PCESP)
				{
					window->DrawList->AddCircle(ImVec2(xpos, yposz), 35 / viewMatrix.GetCameraVec().Distance(Vec3), ImGui::GetColorU32(Config::Get().SexCol), 50, 150 / viewMatrix.GetCameraVec().Distance(Vec3));
					
				}

				if (Config::Get().BoxESP)
				{
					ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
					window->DrawList->AddRect(ImVec2(xpos - Size / 1.5f, WorldPos.y - 10.3), ImVec2(xpos + Size / 1.5f, WorldPosz.y + 30.8), ImGui::GetColorU32(Config::Get().ESPColor4)); //박스
				}
				
				if (Config::Get().EBOXESP)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
					window->DrawList->AddCircle(ImVec2(xpos - TextSize.x / 5.0f, WorldPos.y - TextSize.y / 5.0f), Config::Get().Fov, ImGui::GetColorU32(Config::Get().ESPColor5), 100, 1.0f);
				}
				
				if (Config::Get().UseBox2SP)
				{
					window->DrawList->AddCircle(ImVec2(xposz, yposz), Config::Get().TriggerNOMU + 1.f * Entitys[i].Size , ImGui::GetColorU32(Config::Get().ESPColor5), 1000.f, 1.2f);
				}

				if (Config::Get().FovDraw)
				{
					window->DrawList->AddCircle(ImVec2(output2.x, output2.y), Config::Get().Fov * Entitys[i].Size * Entitys[i].Wall, ImGui::GetColorU32(Config::Get().ESPColor5), 100, 0.5f);
				}

				if (Config::Get().DESP)
				{
					if (abs(Entitys[i].Location.x - GetVector3().x) <= 0.5f && abs(Entitys[i].Location.z - GetVector3().z) <= 0.5f)
					{
						DROW3D(po1, po2, po3, po4, po5, po6, po7, po8, ImGui::GetColorU32(Config::Get().DESPColor2));
					}
					else
					{
						DROW3D(po1, po2, po3, po4, po5, po6, po7, po8, ImGui::GetColorU32(Config::Get().DESPColor));
					}
				}

				if (Config::Get().EdgeESP)
				{
					if (abs(Entitys[i].Location.x - GetVector3().x) <= 0.5f && abs(Entitys[i].Location.z - GetVector3().z) <= 0.5f)
					{
						DrawEdges(ImVec2(xpos - Size / 1.5f, output.y - 10.0f), ImVec2(xpos + Size / 1.5f, output2.y - 10.0f), ImVec2(xpos - Size2 / 1.5f, output.y - 10.0f), ImGui::GetColorU32(Config::Get().EdgeESPCol));
					}
					else
					{
						DrawEdges(ImVec2(xpos - Size / 1.5f, output.y - 10.0f), ImVec2(xpos + Size / 1.5f, output2.y - 10.0f), ImVec2(xpos - Size2 / 1.5f, output.y - 10.0f), ImGui::GetColorU32(Config::Get().EdgeESPCol2));
					}
				}

				/*if (Config::Get().DrawLine)
				{
					window->DrawList->AddLine(ImVec2(Config::Get().Lx, Config::Get().Ly), ImVec2(xposz, yposz), ImGui::GetColorU32(Config::Get().EdgeESPColor5), Config::Get().Lt);
				}*/

				//if (Config::Get().HealthESP)
				//{
				//	if (Entitys[i].eHealthComponent)
				//	{
				//		if (barSize <= 60) {
				//			barSize = 60;
				//			TextOffset += 10;
				//		}
				//		if (barSize > 60 && barSize <= 160) {
				//			barSize = 120;
				//			TextOffset += 20;
				//		}

				//		if (barSize > 160) {
				//			barSize = 200;
				//			TextOffset += 30;
				//		}

				//		ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
				//		window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f), ImGui::GetColorU32(Config::Get().ESPColor3), dist.c_str()); // 거리 색변경 
				//		DrawHealthBar(barSize, ImVec2(xpos - (barSize / 2), ypos + (barSize * 0.10)), Entitys[i].Health, Entitys[i].HealthMax, Entitys[i].ARMOR, Entitys[i].ARMORMAX, Entitys[i].BARRIER, Entitys[i].BARRIERMAX);// 체력바
				////for (WORD x = 0; x < BoneCount && BoneCount < 500; ++x)
				////{
				////	DirectX::XMFLOAT3 currentBone2 = Config::Get().RPM<DirectX::XMFLOAT3>(Entitys[i].BoneBase + OFFSET_BONE_SIZE * x + OFFSET_BONE_LOCATION);
				////	DirectX::XMFLOAT3 Result2{};
				////	XMMATRIX rotMatrix2 = XMMatrixRotationY(Config::Get().RPM<float>(eRotation)); //로테이션 //패치
				////	DirectX::XMStoreFloat3(&Result2, XMVector3Transform(XMLoadFloat3(&currentBone2), rotMatrix2));
				////	Entitys[i].BoneCountPos = Vector3(Result2.x, Result2.y, Result2.z) + RootPos;
				////	Vector2 WorldPosc = viewMatrix.WorldToScreen(Vector3(Entitys[i].BoneCountPos.x, Entitys[i].BoneCountPos.y, Entitys[i].BoneCountPos.z), 1920, 1080);
				////	Vector2 WorldPoszz = viewMatrix.WorldToScreen(Vector3(Entitys[i].BoneCountPos.x, Entitys[i].BoneCountPos.y - 0.1f, Entitys[i].BoneCountPos.z), 1920, 1080);
				////	
				////	if (Config::Get().BoneESP)
				////	{
				////		RenderCircleFilled(ImVec2((WorldPosc.x + WorldPoszz.x) / 2, WorldPosc.y), 2, 0xFFFFFFF, 16);
				//	}
				//}
			}
		}
	}
	//string Fov = u8"에임 범위 : " + to_string((int)Config::Get().Fov);
	//string AS = u8"에임 속도 : " + to_string((int)Config::Get().AimSpeed);
	//string BS = u8"에임 속도 : " + to_string((int)Config::Get().LSGspeed);
	//string CS = u8"플릭파워 : " + (to_string((int)Config::Get().MC1) + u8"악셀 : " + (to_string((float)Config::Get().Accelerate)));
	//string PM = u8"Y축 높이 : " + to_string((int)Config::Get().YPITCH);

	//string Em = (u8"Sponge ONE 3.2.0");
	//string Em3 = (u8"로그인 하세욘,,");

	//ImVec2 Fov_S = ImGui::CalcTextSize(Fov.c_str());
	//ImVec2 ASS = ImGui::CalcTextSize(AS.c_str());
	//ImVec2 BSS = ImGui::CalcTextSize(BS.c_str());
	//ImVec2 CSS = ImGui::CalcTextSize(CS.c_str());
	//ImVec2 EM_S = ImGui::CalcTextSize(Em.c_str());
	//ImVec2 EM3_S = ImGui::CalcTextSize(Em3.c_str());
	//Vector3 currentAngle = Config::Get().RPM<Vector3>(AnglePTR + OFFSET_PLAYER_CONTROLLER_ROTATION);
	//string mX = u8"Myangle X = " + to_string((int)currentAngle.x);
	//string mY = u8"Myangle Y = " + to_string((int)currentAngle.y);
	//string mZ = u8"Myangle Z = " + to_string((int)currentAngle.z);
	//ImVec2 xs = ImGui::CalcTextSize(mX.c_str());
	//ImVec2 ys = ImGui::CalcTextSize(mY.c_str());
	//ImVec2 zs = ImGui::CalcTextSize(mZ.c_str());
	//ImVec2 PM_S = ImGui::CalcTextSize(PM.c_str());
	//
	//

	//if (Config::Get().IsLogin)
	//{
	//	window->DrawList->AddText(ImVec2((1920 - EM_S.x) / 2, Config::Get().TexY + 25.f), ImGui::GetColorU32(Config::Get().ESPColor3), +Em.c_str());

	//	if (Config::Get().IF)
	//	{


	//		window->DrawList->AddText(ImVec2((1920 - xs.x) / 2, Config::Get().TexY + 45.f), ImGui::GetColorU32(Config::Get().ESPColor3), +mX.c_str());
	//		window->DrawList->AddText(ImVec2((1920 - ys.x) / 2, Config::Get().TexY + 65.f), ImGui::GetColorU32(Config::Get().ESPColor3), +mY.c_str());
	//		window->DrawList->AddText(ImVec2((1920 - zs.x) / 2, Config::Get().TexY + 85.f), ImGui::GetColorU32(Config::Get().ESPColor3), +mZ.c_str());
	//		window->DrawList->AddText(ImVec2((1920 - Fov_S.x) / 2, Config::Get().TexY + 105.f), ImGui::GetColorU32(Config::Get().ESPColor3), +Fov.c_str());
	//		if (Config::Get().Nf || Config::Get().sexlogic)
	//		{
	//			window->DrawList->AddText(ImVec2((1920 - CSS.x) / 2, Config::Get().TexY + 125.f), ImGui::GetColorU32(Config::Get().ESPColor3), +CS.c_str());
	//		}
	//		else if (Config::Get().LSGlogic)
	//		{
	//			window->DrawList->AddText(ImVec2((1920 - BSS.x) / 2, Config::Get().TexY + 125.f), ImGui::GetColorU32(Config::Get().ESPColor3), +BS.c_str());
	//		}
	//		else
	//		{
	//			window->DrawList->AddText(ImVec2((1920 - ASS.x) / 2, Config::Get().TexY + 125.f), ImGui::GetColorU32(Config::Get().ESPColor3), +AS.c_str());
	//		}
	//		window->DrawList->AddText(ImVec2((1920 - PM_S.x) / 2, Config::Get().TexY + 145.f), ImGui::GetColorU32(Config::Get().ESPColor3), +PM.c_str());

	//	}
	//}
	//else
	//{
	//	//window->DrawList->AddText(ImVec2(2, Config::Get().TexY + 250.f), ImGui::GetColorU32(Config::Get().ESPColor3), +Em3.c_str());
	//	window->DrawList->AddText(ImVec2((1920 - EM3_S.x) / 2, Config::Get().TexY + 250.f), ImGui::GetColorU32(Config::Get().ESPColor3), +Em3.c_str());

	//}

}

#pragma endregion




#pragma region initsk







void SettingBreakPoints()
{
	HANDLE hMainThread = HW1BP->G2tMa1nThre2d();
	srand(GetTickCount64());
	PVOID pHandler = AddVectoredExceptionHandler(1, Exc2pt10nHand1111er);
	CONTEXT c{};
	c.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	SuspendThread(hMainThread);
	c.Dr0 = Config::Get().BaseAddress + offset::AngleHook;
	c.Dr1 = Config::Get().BaseAddress + offset::BorderLine;
	c.Dr2 = Config::Get().BaseAddress + offset::FovHook;
	c.Dr3 = Config::Get().BaseAddress + offset::BorderLine2;
	//c.Dr3 = Config::Get().BaseAddress + offset::Rotation;
	//c.Dr2 = Config::Get().BaseAddress + offset::CompoenetHook;
	c.Dr7 = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6);
	SetThreadContext(hMainThread, &c);
	ResumeThread(hMainThread);

	//_beginthread((_beginthread_proc_type)Rainbow, 0, nullptr);
	_beginthread((_beginthread_proc_type)Pointer, 0, nullptr);
	_beginthread((_beginthread_proc_type)StructT, 0, nullptr);
	_beginthread((_beginthread_proc_type)Taimbot, 0, nullptr);
	_beginthread((_beginthread_proc_type)TeamLSG, 0, nullptr);
	//_beginthread((_beginthread_proc_type)Jump, 0, nullptr);
	//_beginthread((_beginthread_proc_type)GetEntities, 0, nullptr);
}

void BaseSettings()
{
	Config::Get().hProcess = GetCurrentProcess();
	Config::Get().BaseAddress = (DWORD64)GetModuleHandleA(("Overwatch.exe"));
	auto wndStr = "TankWindowClass";
	Config::Get().hWindow = FindWindowA(wndStr, NULL);
}
#pragma endregion

#pragma region Dllmain
bool DisableProxies(void)
{
	HKEY hKey;
	DWORD data = 0;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false; // Key could not be opened

	if (RegSetValueExW(hKey, L"ProxyEnable", 0, REG_DWORD, (LPBYTE)&data, sizeof(data)) != ERROR_SUCCESS)
		return false; // Failed to set the value

	if (RegCloseKey(hKey) != ERROR_SUCCESS)
		return false; // Could not close key.

	return true;
}

//void Core()
//{
//	auto m_getBoneStructStr = skCrypt("? 8B ? ? 8B ? E8 ? ? ? ? ? 85 ? 74 ? ? 8B ? ? 8B ? ? 8B ? E8 ? ? ? ? 81");
//	m_getBoneStruct = Aeternum::Signature(m_getBoneStructStr).Scan().As<uint64_t>();
//	m_getBoneStructStr.clear();
//	auto m_refreshBonesStr = skCrypt("? 8B ? ? 8B ? E8 ? ? ? ? ? 85 ? 74 ? ? 8B ? ? 8B ? ? 8B ? E8 ? ? ? ? 81");
//	m_refreshBones = Aeternum::Signature(m_refreshBonesStr).Scan().Add(7).Rip().As<uint64_t>();
//	m_refreshBonesStr.clear();;
//
//	for (int i = 0; i < 20; i++)
//	{
//		if (Config::Get().RPM<uint8_t>(m_getBoneStruct - i) == 0xE8 && Config::Get().RPM<uint8_t>(m_getBoneStruct - i - 2) == 0x8B)
//		{
//			m_getBoneStruct = Config::Get().RPM<int32_t>(m_getBoneStruct - i + 1) + (m_getBoneStruct - i + 1) + 4;
//			break;
//		}
//	}
//}

DWORD WINAPI IMGUILOGIN(LPVOID lpParam) // 메인 쓰레드
{
	/*if (AllocConsole()) {

		freopen("CONIN$", "rb", stdin);

		freopen("CONOUT$", "wb", stdout);

		freopen("CONOUT$", "wb", stderr);
	}*/
	Menu::Get().LoginGo();


	InputSys::Get().Initialize();
	D3dHook::AttachHook();
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\DalseoHealingBold.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean()); //폰트 설정
	style->WindowRounding = 5.0f; 
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

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

	BaseSettings();
	SaveEncrypted();
	SettingBreakPoints();

	FreeLibraryAndExitThread((HMODULE)g_Module, 0);
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInst;
		DisableThreadLibraryCalls(g_Module);
		RemovePeHeader(g_Module);
		//Aeternum::get_retaddr();
		////////////////// Thread //////////////////
		CloseHandle(CreateThread(nullptr, 0, IMGUILOGIN, (LPVOID)hInst, 0, nullptr));
		////////////////// Thread //////////////////
	}
	return TRUE;
}
#pragma endregion 