#ifndef MERC_CONSTANTS_H
#define MERC_CONSTANTS_H

#define MAP_DIR				"Maps/"
#define MAP_EXTENSION		".mmp"
#define	MAP_ID				"SNXM"
#define TEST_MAP			"Korhal"
#define MAP_VERSION			0x01
#define GAME_VERSION		"0.1a"
#define CONFIG_DIR			"Configs/"
#define TEXTURE_DIR			"Texures/"

#define MAX(a,b) ((a < b) ? (b) : (a))
#define SQR(x) x*x
#define CLAMP(x, min, max)	( ( x= ( x<min  ? min : x<max ? x : max ) ) )

const float PI = 3.14159265358979323846f;
const float EPSILON = 0.0001f;


#endif
