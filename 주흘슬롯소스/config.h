#pragma once

#include <Windows.h>
#include <vector>
#include "imgui.h"
#include "menu.h"
#include "singleton.h"
#pragma warning(disable : 4244 4996 ) 

using namespace std;

class Config
	: public Singleton<Config>
{
	friend class Singleton<Config>;

public:
	BOOL LoginCheck = FALSE;
	string CODE, ID, PW, Key, Key2; // ´Ê°Ô¤À 
	BOOL IsLogin = FALSE;
	/* Memory Access */
	HANDLE hProcess;
	DWORD64 BaseAddress;
	int Result;
		
	uint64_t m_retSpoof{};

	template <typename RPMType>
	RPMType RPM(DWORD64 Address)
	{
		RPMType Buffer;
		ReadProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), NULL);
		return Buffer;
	}

	template <typename WPMType>
	BOOL WPM(DWORD64 Address, WPMType value)
	{
		return WriteProcessMemory(hProcess, LPVOID(Address), &value, sizeof(value), NULL);
	
	}


	bool LSGMODE = false; //º¸Á¶Å° 1,2 Çìµå ¸ö¼¦ 

	bool NLSGMODE = false;

	float TriggerNOMU = 8.f;
	bool ininput = false;
	
	bool Rage = false;

	bool teamAimbot = false;

	float LSGdelay = 495.f;
	int cfg_mode = 0;
	bool H = false;
	bool Ha = false;
	bool Hb = false;
	bool Hc = false;
	bool Hd = false;
	bool He = false;
	bool Hf = false;

	float TexX = 10.f;
	float TexY = 16.f;

	bool su = false;

	bool IF = true;

	float MAXAimSpeed = 0.850f;
	float hanzotrigfov = 10.f;

	bool DistancesexSilent = false;
	bool BoostsexSilent = false;//zzzzzzzzzzzzzzzzzz
	bool sexlogic = false;
	bool LSGlogic = false;
	float LSGspeed = 0.001f;

	float gfd1 = 800.f;
	float gfd2 = 620.f;
	float gfd3 = 30.f;


	bool UseLSG = false;
	bool realsil = false;
	bool SexESP = false;

	bool DrawLine = false;



	ImVec4 EdgeESPColor5 = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	ImVec4 SexCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	bool DESP = false;

	bool nomu = false;
	bool tabESP = false;
	bool tabAIM = false;
	bool tabSET = false;
	bool tabFUN = false;
	bool tabCOL = false;
	bool tabPRC = false;

	bool DESPColor = false;
	bool DESPColor2 = false;

	float acceleration2 = 0.15f;
	bool FovDraw = false;
	bool MenuCheck = true;
	bool UseAim = false;
	bool UseA1mb0t = false;
	bool UseAIMBOT = false;
	bool UseBox2SP = false;
	bool UseGlowESP = false;
	bool OutlineESP = false;
	bool OutlineESP2 = false;
	bool ESPList = false;
	bool sexList = false;
	bool UseHP = false;
	bool Fov360 = false;
	bool UsePredict = false;
	bool UseGenjiBB = false;
	bool UseAutoMelee = false;
	bool °ÕÁö¿ë°Ë = false;
	bool °ÕÁö½ä±â = false;
	bool ESkill = false;
	bool ÀÚµ¿ÆòÅ¸ = false;
	bool UseAutoShot = false;
	bool UseNoRecoil = false;
	bool AnaSkill = false;
	bool DogSkill = false;
	bool TrQSkill = false;
	bool LESkill = false;
	bool PlayerTag = false;
	bool DomPredit = false;
	bool GravityBool = false;
	bool DrawDist = false;
	bool DrawHeroName = false;
	bool HealthESP = false;
	bool BoneESP = false;
	bool Korean = true;
	bool English = false;
	bool ESP = false;
	bool UltGurad = false;
	bool m_SmoothType = true;
	bool PSilent = false;
	bool EdgeESP = false;
	bool MAimbot = false;
	bool m_FixedAccuracy = false;
	bool EBOXESP = false;
	float m_Accuracy = 100.f;
	float m_SmoothFactor = 10.f;
	float Sexl = 88.1f;
	float Sexll = 31.2f;
	float Sexlll = 2.9f;
	float m_Smooth = 7.f;
	float m_SmoothX = 1000.f;
	float m_SmoothY = 1000.f;


	float Accelerate = 0.f;
	bool Nf = false;
	float MC1 = 0.f;


	//bool UseAllChamp = false;
	ImVec4 E2SPColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 E2SPColor1 = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 ESPColor2 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 ESPColor3 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ESPColor4 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ESPColor5 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 UIColor2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 FovColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ImGuiCol_WindowBg = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 EdgeESPCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 EdgeESPCol2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	float AimFOV = 0.150f;
	float Fov = 250.f;
	float GenjiFOV = 9.5f;
	float silentdil = 15.0f;
	float GenjiSpeed = 1.0f;
	float DomGunBulletSpeed = 80.0f;
	float DomBulletSpeed = 40.0f;
	float TriggerFOV = 0.1f;
	float test = 0.f;
	float test1 = 50.f;
	float TRIGGER = 0.02f;
	float AimSpeed = 0.040f;
	float PredictVal = 24.0f;
	float PredictDistance = 10.0f;
	float YPITCH = 0.235f;
	float Humanize = 0.100f;
	float PreditLevel = 110.f;
	float shootdil = 0.f;
	float exitdil = 0.f;
	bool TriggerBot = false;
	bool Mecry = false;
	bool UseYAxis = false;
	bool aimlock = false;
	bool GENJISHIFT = false;
	bool GENJIQ = false;
	bool GosAimbot = false;
	bool TPAimbot = false;
	bool TFAimbot = false;
	bool HanzoAimbot = false;
	bool SAimbot = false;
	bool TAimbot = false;
	bool FAimbot = false;
	bool RFLICK = false;
	bool Predict = false;
	bool SombraESP = false;
	bool Genyata = false;
	bool Reinhardt = false;
	bool Roadhog = false;
	bool Mccree = false;
	bool NameESP = false;
	bool HanzoFlick = false;
	bool GFAimbot = false;
	bool HZSilent = false;
	bool Mouse1 = false;
	bool Mouse2 = false;
	bool AutoShot = false;
	bool p1 = false;
	bool p2 = false;
	bool p3 = false;
	bool TrigAimbot = false;
	bool TrigHanzoAimbot = false;
	bool AnaBot = false;
	bool AlltoShot = false;
	bool Silent = false;
	bool SuperSilent = false;
	bool m_LockTarget = true;
	bool m_Flick = false;
	bool BoostFlick = false;
	bool BoostSilent = false;
	bool GenjiRight = false;
	bool SD = true;
	float °Å¸® = 15.0f;
	bool Release = false;
	bool PCESP = false;
	bool FFFAIM = false;
	float Gtrig = 20.0f;
	DWORD Mouse5 = 0x05;
	DWORD Mouse6 = 0x06;
	float HZSDelay = 15.0f;
	float sd = 900.f;
	float BulletSpeed = 105.0f;
	float HanzoSpeed = 110.f;
	float Ana_BulletSpeed = 125.0f;
	float BulletSpeed2 = 110.0f;
	float AnaBulletSpeed = 60.0f;
	float DogBulletSpeed = 35.0f; 
	float GenjiBulletSpeed = 50.0f;
	float TracerBombSpeed = 15.0f;
	float Genji_BulletSpeed = 60.0f;
	float TorBulletSpeed = 70.0f;
	float SigmaBulletSpeed = 37.5;
	float ZenBulletSpeed = 90.0f;
	float EcoBulletSpeed = 75.0f;
	float OrisaBulletSpeed = 90.0f;
	float Sigma_BulletSpeed = 50.0f;
	float PHARAH_BulletSpeed = 35.0f;
	float Lucio_BulletSpeed = 50.0f;
	float m_TriggerScale = 180.f;
	DWORD AimKey;
	uint32_t Color = 0xFF;
	HWND hWindow;
	bool sexsex = false;
	bool BoxESP = false;
	//°Å¸® À§ÁÖ
	bool UseDistanceAim = false;
	bool DistanceTracking = false;
	bool DistanceFlick = false;
	bool DistanceSilent = false;
	bool Jump = false;
	bool Muban = false;
	bool MubanH = false;
	float sd1 = 485.f;
	float sd2 = 0.1f;
	float sd3 = 0.1f;
	bool T1 = true;

	bool T2 = true;
	float ¼¶1±¤ = 0.f;
	float ¼¶2±¤ = 0.f;
		
	bool ÀÚµ¿¼¶±¤ = false;
	int m_AimKey1 = VK_XBUTTON1, m_AimKey2 = VK_XBUTTON2, m_AimKey3 = VK_XBUTTON1, m_AimKey4 = VK_XBUTTON2, m_AimKey5 = VK_XBUTTON2, m_AimKey6 = VK_XBUTTON1, m_AimKey7 = VK_MBUTTON;

	/*int Lx = 960;
	int Ly = 540;
	int Lt = 1.f;*/

	// Menu
	bool menuEnabled = true;

	// ÀÌ¹ÌÁö ºÒ·¯¿À±â
	bool ¼½½ºº¸Áö½ºÆùÁö¹ä = false;

	bool StartLogin = true;
};
