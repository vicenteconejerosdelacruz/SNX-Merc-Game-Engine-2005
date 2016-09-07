#include "Error.h"
#include <stdio.h>
#include <windows.h>

void throw_error(const char* msg,const char* file,int line)
{
    char info[255];
    
    sprintf(info,"Error in %s at %d: %s",file,line,msg);
    MessageBox(NULL,info,"ERROR",MB_ICONSTOP|MB_OK);
    //idealmente deberiamos liberar memoria y
    //cerrar archivos
    exit(-1);
}

