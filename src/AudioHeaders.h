/*
 SNX MERC
 Audio Headers, 
- Contiene todos los headers necesarios para escribir el motor de sonido
- Contiene definiciones del motor ademas de los presets de EAX 2.0

  Author Osman Romero A.
*/

#ifndef __AUDIOHEADERS_H_
#define __AUDIOHEADERS_H_

#include <stdio.h>

	/* OpenAL */
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>

	/* Ogg Vorbis */
#include <vorbis/vorbisfile.h>

	/* Windows */
#ifdef _WIN32
	#include <windows.h>
#endif
	
	/* Definiciones de EAX */
#include "eax.h"

	/* maximo de sonidos por nivel reducir sources */
#define MAX_SONGS			30

	/* tipos de listas de reproduccion */
#define MERC_LISTENER		0x01
#define MERC_STATIC			0x02
#define MERC_DYNAMIC		0x03

	/* presets de EAX 2.0 */
#define MERC_NORMAL			-10000
#define MERC_ROOM			0x0001
#define MERC_BATHROOM		0x0002
#define MERC_AUDITORIUM		0x0003
#define MERC_CONCERTHALL	0x0004
#define MERC_CAVE			0x0005
#define MERC_ARENA			0x0006
#define MERC_HANGAR			0x0007
#define MERC_PARKINGLOT		0x0008
#define MERC_SEWERPIPE		0x0009
#define MERC_UNDERWATER		0x0010
#define MERC_PSYCHOTIC		0x0011


#endif