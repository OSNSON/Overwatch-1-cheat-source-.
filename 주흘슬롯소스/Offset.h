#pragma once
#include <Windows.h>
#include <iostream>

struct offset
{
	static uint64_t AngleHook;
	static uint64_t BorderLine;
	//static uint64_t xSettings;
	static uint64_t FovHook;
	//static uint64_t VelocityHook;
	//static uint64_t EntityHook;
	//static uint64_t CompoenetHook;
	//static uint64_t Rotation;
	//static uint64_t HeroID;
	//static uint64_t Health;
	//static uint64_t Skill;
	static uint64_t ViewMatrixOffset;
	static uint64_t ViewMatrixXorKey;
	static uint64_t SensOffset;
	static uint64_t BorderLine2;
};
//사용
//이렇게 옵치 키고 로그인될때 취소눌러서 이런 화면 뜨게 만들어야 정지 안먹음 ㅇㅇ 그리고 이렇게 hex 키고 어레이로 바꾸고 wirtable이거 네모로 만들고 어레이 넣고 스캔 ㄱ 그리고 값 나온거 클릭하고 컨트롤 + D 하면 이런 창이 뜨는데 젤 위에꺼 우클릭하고 카피 이렇게 오버워치 + 값 이래 나옴 6FCA0C이게 
//패치값인데 여기 앞에다가 0x6FCA0C이렇게 해가지고 ㅇㅋ? 함 혼자 해보셈                 그리고 이제 포브훅 따는법 알려드림ㅇㅇ 포브훅은 존나 어려우니깐 좀 잘 보셈 근데ㅐ 왜 바로 포브훅임 밑에꺼는 똑같은데 포브훅만 유독 어려움 다른거는 처음했던거처럼 하면 됨?ㅇㅇ 모르겠으면 자주 DM 하면 바로바로 답장함ㅇㅋ
uint64_t offset::AngleHook = 0x6FBAAC; //앵글
uint64_t offset::BorderLine = 0xB92107; //월핵1
uint64_t offset::BorderLine2 = 0x1ABC667; //월핵2
uint64_t offset::FovHook = 0x6F9240; //포브훅 
uint64_t offset::ViewMatrixOffset = 0x2E2BEB0; //VEH 훅 뷰매  
uint64_t offset::ViewMatrixXorKey = 0x4CE58C933BAD54C5; //VEH 훅(뷰매)) 복호화키  // ''두번째꺼 ㅇㅋ? ㅇㅇ  Overwatch.exe+5960F5 - 48 B8 C2EFE605053F1F0C - mov rax,0C1F3F0505E6EFC2 이거는 이렇게 중간에 있거나 마지막꺼일떄도 있음 ㅇㅇ 그럼 다 한번씩 해봐야함? 그래도 중간부터 해보면 대부분 될거임 아 중간이 아니라 마지막 ㅇㅇ 저거 포브훅 en 뭐시기 고치셈이미 고침 일단 실행 해보셈 ㄱㄱ



//사용X

//uint64_t offset::xSettings = 0xbe0640;
//uint64_t offset::CompoenetHook = 0x17C6DF8;
//uint64_t offset::Rotation = 0xC22C42;
//uint64_t offset::VelocityHook = 0x1ae97d3;
//uint64_t offset::Health = 0x96f608;
//uint64_t offset::HeroID = 0x4E55E3;
uint64_t offset::SensOffset = 0x2DA4A10; //이건 운지함 ㅗㅗ

#define Compo__SKILL                     0x2F // 0x2F

#define ADDRESS_SKILL 0x1ba06d0

#define OFFSET_BADREADPTR_SIZEDEFAULT    0x540
#define OFFSET_BADREADPTR_SIZEBONES      0xE474
#define OFFSET_BADREADPTR_SIZEROTSTRUCT  0xB00


#define OFFSET_GenjiQ								0x247
#define OFFSET_PLAYER_CONTROLLER_ROTATION			0x1090
#define OFFSET_PLAYER_CONTROLLER_KEY				0x1034
#define OFFSET_PLAYER_CONTROLLER_DELTA				(OFFSET_PLAYER_CONTROLLER_ROTATION + 0x4C)

#define OFFSET_UNIT_VELOCITY						0x4
#define OFFSET_UNIT_COMPNENT_TO_WORLD				0x10
#define OFFSET_PLAYER_VISIBILITY					0x2D 
#define OFFSET_UNIT_HEALTH							0x33 
#define OFFSET_PLAYER_HEROIDENTITY					0x4B
#define OFFSET_UNIT_HASPLAYERID						0x2B
#define OFFSET_UNIT_ROTATIONBASE					0x27
#define OFFSET_UINT_LINK							0x2C
#define OFFSET_UINT_SKILL							0x2F
#define OFFSET_UINT_OUTLINE							0x53

#define OFFSET_HEALTHPTR_HEALTH						0xE0 // OK
#define OFFSET_HEALTHPTR_HEALTHMAX					0xDC // OK
#define OFFSET_HEALTHPTR_ARMOR						0x220 // OK
#define OFFSET_HEALTHPTR_ARMORMAX					0x21C // OK
#define OFFSET_HEALTHPTR_BARRIER					0x360 // OK
#define OFFSET_HEALTHPTR_BARRIERMAX					0x35C // OK

#define OFFSET_HEALTHPTR_TAG						0x8 // OK

#define OFFSET_UCWPIDPTR_COMPOID					0xD0 // OK
#define OFFSET_HEROIDPTR_COMPOID					0xE8// OK //0xD8
#define OFFSET_HEALTHPTR_TEAM						0x504// OK
#define OFFSET_HEROIDPTR_HEROID						0xE8 // OK
#define OFFSET_HEROIDPTR_SKINID						0xD0 // OK

#define OFFSET_VELOCITYPTR_LOCATION					0x140       // ㅇ
#define OFFSET_VELOCITYPTR_ENCRYPTED				0x80       // ㅇ
#define OFFSET_VELOCITYPTR_VELOCITY					0x50      // ㅇ
#define OFFSET_VELOCITYPTR_BONEDATA					0x6F0    // ㅇ
#define OFFSET_BONEDATA_BONEBASE					0x28    //  ㅇ
#define OFFSET_BONE_SIZE							0x30   //  ㅇ
#define OFFSET_BONE_LOCATION						0x20  // ㅇ
#define OFFSET_BONE_ROTCATION						0x0  // ㅇ

#define OFFSET_BONEDATA_BONESCOUNT					0x40 // OK?
#define OFFSET_VISIBILITYPTR_ISVISIBLE				0x98 //98

#define OFFSET_ROTATIONPTRPTR_ROTSTRUCT				0x1598 // // OK
#define OFFSET_ROTSTRUCT_ROT						0xA98 // OK
#define OFFSET__SKill								0xD0