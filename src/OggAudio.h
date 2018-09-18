/*
 SNX MERC
 Ogg Audio
 - Clase padre de los sonidos reproducidos en Merc
 - Tiene una key que sirve para identificarla y asi buscarla
 - Tiene el buffer y source del sonido
 - Tiene una posicion y velocidad en (x,y,z) (Vector3)

   Author Osman Romero A.
*/

#ifndef MERC_AUDIO_H
#define MERC_AUDIO_H

#include <string>

using namespace std;

#include "AudioHeaders.h"
#include "Vector3.h"

class OggAudio
{
protected:
		/* nombre de los sonidos */
	char* key;
		/* buffer y source de los sonidos */
    ALuint buffer;
    ALuint source;

    Vector3 fuentePos;
    Vector3 fuenteVel;

		/* variables necesarias para load() */
	char* MemPtr;
	int DataSize;
	int DataRead;
	ov_callbacks callback;    

public:	
	OggAudio();
    OggAudio(char* fileName);

	//constructor de copia
	OggAudio* copyAudio(OggAudio *o);
			
		/* metodos necesarios para load() */
	inline char *GetMemPtr(){return MemPtr;}
	inline int GetDataSize(){return DataSize;}
	inline int GetDataRead(){return DataRead;}
	inline void SetDataRead(int offset){DataRead=offset;}        

		/* carga y descarga de el audio */
    int load();
	void unload();
        
       /* set y get de la clave */
    void setKeyName(char* keyName);  
    char* getKeyName();
        
	   /* set de las posiciones del sonido */        
    void setPosXYZ(Vector3 vec);
	Vector3 getPosXYZ();

		/* LOOP de los sonidos true = 1 o false = 0  */
	void setLOOP(int val);
        		
    ALuint getSource();
    ALuint getBuffer();          
};
#endif
