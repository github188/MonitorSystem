#include "stdafx.h"
#include "gloable/mysql_interface.h"

MYSQL* CMySqlInterface::m_mysqlInstance;
BOOL   CMySqlInterface::m_bIsOpen = FALSE;

CMySqlInterface::CMySqlInterface()
{
	
}

CMySqlInterface::~CMySqlInterface()
{
	closeMysql();
	m_mysqlInstance = NULL;
}

MYSQL* CMySqlInterface::getMysqlInstance()
{
	return m_mysqlInstance;
}

BOOL CMySqlInterface::isOpen()
{
	return m_bIsOpen;
}

BOOL CMySqlInterface::openDatabase(const char* host, \
								const char* user, \
								const char* passwd,\
								const char* dbName,\
								int port)
{
	if (m_mysqlInstance ==NULL){
		m_mysqlInstance = mysql_init((MYSQL*)NULL);
	}

	int nValue = 1;
	if (mysql_real_connect(m_mysqlInstance,host,user,passwd,dbName,port,NULL,0))
	{
		if (mysql_select_db(m_mysqlInstance,dbName)<0)
		{
			TRACE0("已成功连接mysql, 但选择数据库出错!\n");
			mysql_close(m_mysqlInstance);

			m_bIsOpen = FALSE;
		}
		else
		{
			mysql_options(m_mysqlInstance,MYSQL_OPT_RECONNECT,(char *)&nValue);
			mysql_query(m_mysqlInstance,"SET NAMES GBK");

			m_bIsOpen = TRUE;
		}
	}
	return m_bIsOpen;
}

void CMySqlInterface::closeMysql()
{
	mysql_close(m_mysqlInstance);
}

BOOL CMySqlInterface::executeSQL( const char* sqlMsg )
{
	if(!mysql_query(m_mysqlInstance,sqlMsg)){
		TRACE0("execute sqlMsg successfully.");
		return TRUE;
	}
	else{
		TRACE1("query failed: Error=%s\n",mysql_error(m_mysqlInstance));
		return FALSE;
	}
	return FALSE;
}

