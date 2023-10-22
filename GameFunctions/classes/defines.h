#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

//teclas
#define escKey 27
#define enterKey 13
#define spaceKey 32
#define upKey 72
#define leftKey 75
#define rightKey 77
#define downKey 80


// Ids de objetos
#define QtdIdsFixos 6 // vazio, powerup, powerup, powerup, parede, fragil
#define vazioid 0
#define explosaoid 1
#define inimigoid 2
#define ghostPowerupId 3
#define bombExpanderId 4
#define ghostBombId 5
#define playerid 6
#define paredeid 7
#define paredefragilid 8
#define bombaid 9
#define ghostid 10

int idsCenario[QtdIdsFixos] = {vazioid, paredeid, paredefragilid, ghostPowerupId, bombExpanderId, ghostBombId};