#include "stdafx.h"
#include "GBKToUTF8.h"

#define MAX_LOCAL_UNICODE_BUF_LEN  1024

//将GBK转换成UTF8
//先通过MultiByteToWideChar函数把GBK转换成Unicode,
//然后再通过WideCharToMultiByte函数把Unicode拆分拼装成UTF-8。
BOOL ConvertGBKToUTF8(char *szGBK,char *szUTF8,int nUTF8BufSize,int &nRealLen)
{ 
	unsigned short * wsUnicode = NULL;
	int nLen = 0;
	bool bNewFlag = false;

	unsigned short Buffer[MAX_LOCAL_UNICODE_BUF_LEN] = {0};

	if (szGBK == NULL||szUTF8 == NULL)
		return FALSE;

	try
	{
		do 
		{
			memset(szUTF8,0,nUTF8BufSize);

			nLen = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)szGBK, -1, NULL,0);
			if (nLen == 0)
				break;

			if (nLen > MAX_LOCAL_UNICODE_BUF_LEN)
			{
				wsUnicode = new unsigned short[nLen]; 
				if (wsUnicode == NULL)
					break;
				memset(wsUnicode, 0, nLen*sizeof(unsigned short)); 
				bNewFlag = true;
			}
			else
			{
				memset(Buffer,0,sizeof(Buffer));
				wsUnicode = Buffer;
				bNewFlag = false;
			}

			nLen = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)szGBK, -1, (LPWSTR)wsUnicode, nLen); 
			if (nLen == 0)
				break;

			nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wsUnicode, -1, NULL, 0, NULL, NULL);
			if (nLen > nUTF8BufSize)
				break;

			nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wsUnicode, -1, szUTF8, nLen, NULL,NULL); 
			if (nLen == 0)
				break;

			nRealLen = nLen;

			if(bNewFlag&&wsUnicode != NULL)
			{
				delete [] wsUnicode;
				wsUnicode = NULL;
			}

			return TRUE;

		} while (FALSE);

		if(bNewFlag&&wsUnicode != NULL)
		{
			delete [] wsUnicode;
			wsUnicode = NULL;
		}

		nRealLen = 0;

		return FALSE;
	}
	catch(...)
	{

	}

	return FALSE;
}

//将UTF8转换成GBK
//先通过MultiByteToWideChar函数把UTF-8转换成Unicode,
//然后再通过WideCharToMultiByte函数把Unicode转换成GBK。
BOOL ConvertUTF8ToGBK(char *szUTF8,char *szGBK,int nGBKBufSize,int &nRealLen)
{ 
	unsigned short * wsUnicode = NULL;
	int nLen = 0;
	bool bNewFlag = false;

	unsigned short Buffer[MAX_LOCAL_UNICODE_BUF_LEN] = {0};

	if (szUTF8 == NULL||szGBK == NULL)
		return FALSE;

	try
	{
		do 
		{
			memset(szGBK,0,nGBKBufSize);

			nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)szUTF8, -1, NULL,0);
			if (nLen == 0)
				break;

			if (nLen > MAX_LOCAL_UNICODE_BUF_LEN)
			{
				wsUnicode = new unsigned short[nLen]; 
				if (wsUnicode == NULL) 
					break;
				memset(wsUnicode, 0, nLen*sizeof(unsigned short)); 
				bNewFlag = true;
			}
			else
			{
				memset(Buffer,0,sizeof(Buffer));
				wsUnicode = Buffer;
				bNewFlag = false;
			}

			nLen = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)szUTF8, -1, (LPWSTR)wsUnicode, nLen); 
			if (nLen == 0) 
				break;

			nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wsUnicode, -1, NULL, 0, NULL, NULL); 
			if (nLen > nGBKBufSize)
				break;

			nLen = WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wsUnicode, -1, szGBK, nLen, NULL,NULL);
			if (nLen == 0)
				break;

			nRealLen = nLen;

			if(bNewFlag&&wsUnicode != NULL)
			{
				delete [] wsUnicode;
				wsUnicode = NULL;
			}

			return TRUE;

		} while (FALSE);

		if(bNewFlag&&wsUnicode != NULL)
		{
			delete [] wsUnicode;
			wsUnicode = NULL;
		}

		nRealLen = 0;

		return FALSE;
	}
	catch(...)
	{

	}

	return FALSE;
}