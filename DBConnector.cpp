#include <stdio.h>
#include <string.h>

#include "DBConnector.h"
#include "Log.h"

DBConnector::DBConnector(){

	
	memset(Hostname,0,sizeof(Hostname));
	memset(Username,0,sizeof(Username));
	memset(Password,0,sizeof(Password));

	Connected=false;
	QueryEnabled=false;
}

DBConnector::~DBConnector()
{
	PQfinish(DBConnection);
	
}

void DBConnector::Init(char *Host,char *User,char *Pass){

	strcat(Hostname,Host);
	strcat(Username,User);
	strcat(Password,Pass);

	char Query[128];

	memset(Query,0,sizeof(Query));

	sprintf(Query,"host=%s dbname = merc-db user = %s password = %s",Hostname,Username,Password);

	Log::Output("%s\n",Query);
	DBConnection=PQconnectdb(Query);

	int status=PQstatus(DBConnection);

	if(status==CONNECTION_BAD)
	{
		Log::Output("Error Connecting to DB on %s %s %s\n",Hostname,Username,						Password);
		Connected=false;
	}
	else
	{
		Log::Output("Connection to DB establish\n");
		Connected=true;
	}	


}

void DBConnector::Query(const char *fmt,...){

	if(QueryEnabled==true)
	{
		Log::Output("Error there is a pending Query\n");
		return;
	}
	
	va_list va;
	char Query[1024];
	
	memset(Query,0,sizeof(Query));
	va_start(va,fmt);
	vsprintf(Query,fmt,va);
	va_end(va);
	
	QResult=PQexec(DBConnection,Query);

	if(QResult==NULL)
	{
		Log::Output("Error :%s\n",PQerrorMessage(DBConnection));
		return;
	}

	QueryEnabled=true;
}

char * DBConnector::GetValue(int row,int column){

	if(QueryEnabled!=true)
	{
		Log::Output("Getting Data from a unknow Query\n");
		return NULL;
	}

	if(GetNTuples() < row || GetNFields() < column || row < 0 || column < 0)
	{
		Log::Output("Requesting a value of out range\n");
		return NULL;
	}

	return PQgetvalue(QResult,row,column);
}

int DBConnector::GetNTuples()
{
	if(QueryEnabled!=true)return -1;

	return PQntuples(QResult);
}

int DBConnector::GetNFields(){

	if(QueryEnabled!=true)return -1;

	return PQnfields(QResult);
}