#ifndef MERC_PLAYER_H
#define MERC_PLAYER_H

#include <stdio.h>
#include <string.h>

#include "Vector3.h"
#include "RenderAble.h"
#include "Model.h"

class Player:public RenderAble
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
	//FSM Data
	int FirstHierarchy;
	int SecondHierarchy;
	int UltraHierarchy;
	//Model
	Model model;
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
		FirstHierarchy=0;
		SecondHierarchy=0;
		UltraHierarchy=0;
		Rotation[0]=0;
		Rotation[1]=0;
	}
	void Render();
	void LoadModel(char *modelname);
};

#endif