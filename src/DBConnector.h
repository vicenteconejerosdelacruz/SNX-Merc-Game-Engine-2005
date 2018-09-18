#ifndef MERC_DBCONNECTOR_H
#define MERC_DBCONNECTOR_H

//Uncoment this if you want a PostgreSQL DB
//#define MERC_DB

#ifdef MERC_DB

#include <libpq-fe.h>

class DBConnector
{
private:
	bool Connected;
	char Hostname[32];
	char Username[32];
	char Password[32];
	PGconn *DBConnection;
	bool QueryEnabled;
	PGresult *QResult;
public:
	DBConnector();
	~DBConnector();
	void Init(char *Host,char *User,char *Pass);
	inline bool isConnected(){return Connected;}
	inline void CloseQuery(){QueryEnabled=false;}
	inline bool isQueryPending(){return QueryEnabled;}
	void Query(const char *fmt,...);
	char *GetValue(int row,int column);
	int GetNTuples();
	int GetNFields();
};
#endif

#endif