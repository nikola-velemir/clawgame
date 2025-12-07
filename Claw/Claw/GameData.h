
#pragma once
#include "Toy.h"
#include "Lights.h"
#include "TokenInsert.h"

#ifndef GAME_DATA_H
#define GAME_DATA_H
struct GameData
{
	Toy* toys;
	int toyCount;
	Light* light;
	TokenInsert* tokenInsert;
};
#endif // !GAME_DATA_H
