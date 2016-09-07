#include "Configfile.h"
#include "Error.h"
#include "Log.h"

ConfigFile::~ConfigFile()
{
	if(ls){
		lua_close(ls);
		ls = NULL;
	}
}

void ConfigFile::read(const char *filename)
{
    ls = lua_open();
    if(luaL_loadfile(ls,filename) || lua_pcall(ls,0,0,0)){
		char Error[255];
		memset(Error,0,sizeof(Error));
		sprintf(Error,"Failed to load config file %s",filename);
		Throw(Error);
    }
}

void pushValuesToLuaState(lua_State *ls,const char *name)
{
    char *period;
    char *ptr = strdup(name);
    period = strchr(ptr,'.');
    if(period == NULL){ //suponemos que es una variable global
        lua_getglobal(ls,name);
    }else{ 
        *period = 0;
        char *table = ptr;
        char *value = period+1;
        
        lua_getglobal(ls,table);
        if(!lua_istable(ls,-1)){
			Throw("incorrect value when a table was expected");
        }
        
        lua_pushstring(ls,value);
        lua_gettable(ls,-2);
    }
    //en este momento nuestro valor esta en el tope de la pila
}

int ConfigFile::getInteger(char *fmt,...)
{
	va_list va;
    int r;
	char name[255];
	memset(name,0,255);
	va_start(va,fmt);
	vsprintf(name,fmt,va);
	va_end(va);
    pushValuesToLuaState(ls,name); 
    if(!lua_isnumber(ls,-1)){
        Throw("incorrect value when a number was expected");
    }
    
    r = (int)lua_tonumber(ls,-1);
    lua_pop(ls,1);
    return r;
}

bool ConfigFile::getBoolean(char *fmt,...)
{
    bool r;
	char name[255];
	va_list va;
	memset(name,0,255);
	va_start(va,fmt);
	vsprintf(name,fmt,va);
	va_end(va);
    pushValuesToLuaState(ls,name);
    if(!lua_isboolean(ls,-1)){
        Throw("incorrect value when a boolean was expected");
    }
    
    r = (bool)lua_toboolean(ls,-1);
    lua_pop(ls,1);
    return r;
}

char* ConfigFile::getString(char *fmt,...)
{
    char* r;
	char name[255];
	va_list va;
	memset(name,0,255);
	va_start(va,fmt);
	vsprintf(name,fmt,va);
	va_end(va);
    pushValuesToLuaState(ls,name);
    if(!lua_isstring(ls,-1)){
        Throw("incorrect value when a string was expected");
    }
    
    r = (char*)lua_tostring(ls,-1);
    lua_pop(ls,1);
    return r;
}

