/*
 SNX MERC
 Static PlayList
 - Contiene el arreglo de sonidos estaticos, en esta clasificacion se pueden
   englobar los sonidos que pertenecen a el escenario que se esta jugando, tienen
   una posicion pero esto no es alterable.
  
   Author Osman Romero A.
*/

#ifndef MERC_STATIC_PLAYLIST_H
#define MERC_STATIC_PLAYLIST_H

#include "OggAudio.h"

class StaticPlayList
{
private:
	OggAudio *list;
	int cant;

public:
	StaticPlayList();
	~StaticPlayList();
	void add(OggAudio *a);
	OggAudio *search(char* key);

		/* Metodo necesario para el PlayList del Listener ya que necesita que
		las posiciones de los sonidos de su playlist sean los mismos que del listener */
	void setALLpos(Vector3 vec);
};

#endif