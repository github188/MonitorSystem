#pragma once
#include "..\..\mysql/include/mysql.h"

class CMySqlInterface
{
private:
	CMySqlInterface();
	~CMySqlInterface();

	static MYSQL* m_mysqlInstance;
	static BOOL m_bIsOpen;

public:
	static MYSQL* getMysqlInstance();
	static BOOL isOpen();

	//database operation
	static BOOL openDatabase(	const char* host, \
								const char* user, \
								const char* passwd,\
								const char* dbName,\
								int port);
	static void closeMysql();

	static BOOL executeSQL(const char* sqlMsg);
};