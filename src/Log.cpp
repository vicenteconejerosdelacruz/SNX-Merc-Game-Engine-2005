#include <stdarg.h>
#include <stdio.h>
#include "Log.h"

bool Log::is_file_open = false;
FILE *Log::log = NULL;

void Log::Output(char *fmt,...){

	va_list va;
	
	if(!is_file_open){
		log = fopen("merc_log.txt","w");
		if(log == NULL){
			return;
		}
		is_file_open = true;
	}

	va_start(va,fmt);
	vfprintf(log,fmt,va);
	va_end(va);
	fflush(log);
}

void Log::Close(){

	if(is_file_open)
		fclose(log);
}