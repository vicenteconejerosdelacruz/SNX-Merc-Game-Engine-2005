/*
 SNX MERC
 Listener
 - Clase que representa al auditor en el juego
 - Tiene una posicion y velocidad en (x,y,z) (Vector3)
 - Tiene su propia lista de sonidos estaticos para reproducirlos en la posicion del listener

   Author Osman Romero A.
*/

#ifndef __LISTENER_H_
#define __LISTENER_H_

#include "AudioHeaders.h"
#include "StaticPlayList.h"
#include "Vector3.h"

class Listener
{
private:
	Vector3 listenerPos;
	Vector3 listenerVel;
	ALfloat listenerOri[6];
	StaticPlayList pls;

public:
	Listener();
	void setPosXYZ(Vector3 vec);
	Vector3 getPosXYZ();

	ALfloat getX();
	ALfloat getY();
	ALfloat getZ();

	void setOrientation(Vector3 vec);
	void updateX(ALfloat units);
	void updateY(ALfloat units);
	void updateZ(ALfloat units);

	int loadPlayList(char *fileName);
	void unloadPlayList();

	void PlaySound(char *sound);
	void PauseSound(char *sound);

	OggAudio* getSoundByName(char *sound);
};

#endif
