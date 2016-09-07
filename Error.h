#ifndef MERC_ERROR_H
#define MERC_ERROR_H

#define Throw(x) throw_error(x,__FILE__,__LINE__)
void throw_error(const char*,const char* ,int);
#endif
