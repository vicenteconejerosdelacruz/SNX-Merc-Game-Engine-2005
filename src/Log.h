#ifndef MERC_LOG_H
#define MERC_LOG_H

#include <stdio.h>
#include <stdarg.h>

class Log{
private:
	static FILE* log;
	static bool is_file_open;
public:
	static void Output(char *fmt,...);
	static void Close();
};

#endif