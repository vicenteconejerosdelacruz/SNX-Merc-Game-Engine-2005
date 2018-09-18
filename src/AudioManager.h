/*
 SNX MERC
 Manager
 - Clase Fachada, o cara visible del subsistema de sonido para Merc
 - Manipula la reproduccion y posicionamiento de los sonidos (Estaticos, Dinamicos)
 - Manipula la posicion del Auditor (Objeto Listener)
 
   Author Osman Romero A.
*/

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL/SDL.h>

#include "AudioHeaders.h"
#include "Listener.h"
#include "DynamicPlayList.h"
#include "StaticPlayList.h"

#include "Vector3.h"

#include "OggAudio.h"
#include "DynamicAudio.h"
#include "SndPlayer.h"

class AudioManager
{
private:

	/* se activa si se soporta EAX */
	ALboolean g_bEAX;

	/* variables necesarias para la iniciacion del sistema de sonido
	si no se usa DirectSound (Gnu/Linux) */
	ALCcontext *Context;
	ALCdevice *Device;

	/* source para poder reproducir sonidos en posiciones dadas ( playInXYZ() ) */
	ALuint source;

	/* nombre del mapa que trae las configuraciones de sonidos segun el escenario */
	char *mapName;

	/* Objeto Listener */
	Listener escucha;	
	DynamicPlayList dynamicList;
	StaticPlayList	staticList;			

	/* Listas de reproduccion (Estatica, Dinamica, Auditor) */
	int loadListenerPlayList();
	int loadStaticPlayList();
	int loadDynamicPlayList();
		
	/* Inicializacion de EAX 2.0 */
	ALboolean InitEAX();

	static AudioManager *manager;
protected:
	AudioManager();

public:	
	/* esta lista de reproduccion contiene objetos con timer */	
	static StaticPlayList timerPlayList;
	
	static AudioManager* getInstance();

	int InitAudio();
	int QuitAudio();

	char* getMapName();
	void setMapName(char* map);

	/* reproduce, detiene, pausa un sonido 
	dado su nombre y el PlayList al que corresponde:
	MERC_LISTENER o MERC_STATIC o MERC_DYNAMIC */
	void play(char* snd, int lst);
	void stop(char* snd, int lst);
	void pause(char* snd, int lst);

	/* carga todos los PlayList */
	int loadPlayLists();
	void unloadPlayLists();

	/* Obtener el Auditor para manipularlo directamente */
	Listener getListener();
	void setListenerPos(Vector3 vec);
	Vector3 getListenerPos();
	/* Operacion util si no se desea manipular el listener directamente */
	void updateListenerXYZ(Vector3 vec);
	
	/* La razon de este metodo es una solucion de parche al problema de los
	sonidos duplicados, ejemplo de esto es la clasificacion de los sonidos del listener
	como el de los disparos, los pies, etc... el asunto de este metodo es poder ejecutar
	un sonido del listener en otra posicion, ideal para solucionar el sonido de otro
	player que ejecute disparos, sonidos de pies en una posicion dada */
	void playInXYZ(char *key,ALfloat x, ALfloat y, ALfloat z);

	/* cambio de ambientes EAX ver AudioHeaders.h */
	void setEAXEnviroment(int env);

	void createTimers();
};

#endif