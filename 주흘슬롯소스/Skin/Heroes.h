#pragma once
#include <Windows.h>
#include <iostream>
enum Heroes
{
	ANA,
	ASHE,
	BARTIST,
	BASTION,
	BRIGITTE,
	DOOMFIST,
	DVA,
	GENJI,
	HANZO,
	JUNKRAT,
	LUCIO,
	MCCREE,
	MEI,
	MERCY,
	MOIRA,
	ORISA,
	PHARAH,
	REAPER,
	REINHARDT,
	ROADHOG,
	SOLDIER,
	SOMBRA,
	SYMMETRA,
	TORBJORN,
	TRACER,
	WIDOWMAKER,
	WINSTON,
	WRECKINGBALL,
	ZARYA,
	ZENYATTA,
	SIGMA,
	ECHO,
	TRAINING_BOT,
	AllHero,
	XXX
};

LPCSTR Hero2Str(Heroes h);

Heroes Skin2Hero(WORD Skin);

Heroes GUID2Hero(DWORD64 GUID);
