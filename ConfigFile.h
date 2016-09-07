#ifndef MERC_CONFIGFILE_H
#define MERC_CONFIGFILE_H

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
};
#include <string.h>
#include <stdarg.h>

class ConfigFile
{
private:
    //lua state
    lua_State *ls;
public:
    // constructors
    ConfigFile():ls(NULL){}
    ConfigFile(const char *filename){read(filename);}
    // destructor
    ~ConfigFile();
        
    // leer la configuracion
    void read(const char* filename);
    
    // acceso a los datos en el archivo de config
    int getInteger(char *fmt,...);
    bool getBoolean(char *fmt,...);
    char* getString(char *fmt,...);
};
#endif