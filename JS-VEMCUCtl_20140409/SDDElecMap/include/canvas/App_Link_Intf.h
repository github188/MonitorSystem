#pragma once
#include "tybase.h"
#include "LinkObjectBase.h"


class CLinkTxtFile :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkTxtFile);
private:
	
public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkTxtFile(CString strname);
	CLinkTxtFile(void);

	~CLinkTxtFile(void);
};


class CLinkWebShot :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkWebShot);
private:
	DWORD m_webIP;					//web网站地址
	DWORD m_ServerIP;				//中继服务器地址
	int		m_ServerPort;			//控制端口号
	DWORD m_GroupIP;				//组播地址
	int  m_GroupPort;				//组播端口
	int  m_unitID;					//监控站点服务ID号
  char m_unitName[33];		//监控站点名称
	int  m_shotID;					//摄像点ID号
	char m_shotName[33];		//摄像点名称

	void SetDefaultProp();
public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkWebShot(CString strname,DWORD webIP,DWORD serverIP,DWORD serverPort,DWORD groupIP,DWORD groupPort,int unitID,CString unitName);
	CLinkWebShot(void);

	~CLinkWebShot(void);
};
