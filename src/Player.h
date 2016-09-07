#ifndef MERC_PLAYER_H
#define MERC_PLAYER_H

#include <stdio.h>
#include <string.h>

#include "Vector3.h"

class Player
{
public:
	Player *next;
	Player *prev;
	char name[32];
	char modelname[32];
	char planetname[32];
	int id;
	Vector3 Position;
	Vector3 Velocity;
	float Rotation[2];
	int WeaponID;
	char agency[32];
	char race[32];
	Player(){
		WeaponID=0;
		next=NULL;
		prev=NULL;
		memset(name,0,sizeof(name));
		memset(race,0,sizeof(race));
		memset(agency,0,sizeof(agency));
		memset(modelname,0,sizeof(modelname));
		memset(planetname,0,sizeof(planetname));
		id=0;
	}
};

#endif