#pragma once
#include <Windows.h>
#include "Heroes.h"
int HEROID2YPITCH(Heroes HeroID)
{
	if (HeroID == Heroes::ANA)
		return 67;
	else if (HeroID == Heroes::ASHE)
		return 125;
	else if (HeroID == Heroes::BARTIST)
		return 167;
	else if (HeroID == Heroes::BASTION)
		return 81;
	else if (HeroID == Heroes::BRIGITTE)
		return 77;
	else if (HeroID == Heroes::DOOMFIST)
		return 83;
	else if (HeroID == Heroes::DVA)
		return 140;
	else if (HeroID == Heroes::GENJI)
		return 53;
	else if (HeroID == Heroes::HANZO)
		return 38;
	else if (HeroID == Heroes::JUNKRAT)
		return 100;
	else if (HeroID == Heroes::LUCIO)
		return 53;
	else if (HeroID == Heroes::MCCREE)
		return 53;
	else if (HeroID == Heroes::MEI)
		return 50;
	else if (HeroID == Heroes::MERCY)
		return 150;
	else if (HeroID == Heroes::MOIRA)
		return 64;
	else if (HeroID == Heroes::ORISA)
		return 50;
	else if (HeroID == Heroes::PHARAH)
		return 38;
	else if (HeroID == Heroes::REAPER)
		return 36;
	else if (HeroID == Heroes::REINHARDT)
		return 41;
	else if (HeroID == Heroes::ROADHOG)
		return 61;
	else if (HeroID == Heroes::SOLDIER)
		return 38;
	else if (HeroID == Heroes::SOMBRA)
		return 42;
	else if (HeroID == Heroes::SYMMETRA)
		return 89;
	else if (HeroID == Heroes::TORBJORN)
		return 45;
	else if (HeroID == Heroes::TRACER)
		return 52;
	else if (HeroID == Heroes::WIDOWMAKER)
		return 40;
	else if (HeroID == Heroes::WINSTON)
		return 109;
	else if (HeroID == Heroes::WRECKINGBALL)
		return 199;
	else if (HeroID == Heroes::ZARYA)
		return 65;
	else if (HeroID == Heroes::ZENYATTA)
		return 149;
	else if (HeroID == Heroes::SIGMA)
		return 180;
	else if (HeroID == Heroes::ECHO)
		return 42;
	else if (HeroID == Heroes::TRAINING_BOT)
		return 37;

	return 38;
}