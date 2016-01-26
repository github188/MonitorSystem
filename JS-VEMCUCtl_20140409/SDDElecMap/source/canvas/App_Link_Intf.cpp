#include "stdafx.h"
#include "canvas/app_link_intf.h"
#include "canvas/PropLinkFile.h"
#include "canvas/PropWebPhoto.h"
#include "canvas/LinkTool.h"

////////////////////////////////////////////////////////////////////////
//
//		CLinkTxtFile
//
////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkTxtFile,CLinkObjectBase,0)

CLinkTxtFile::CLinkTxtFile(void)
:CLinkObjectBase(lnkTxt,"")
{
}

CLinkTxtFile::CLinkTxtFile(CString strname)
:CLinkObjectBase(lnkTxt,strname)
{
}


CLinkTxtFile::~CLinkTxtFile(void)
{

}

void CLinkTxtFile::Save_Ty(CFile *file, BOOL Yn)
{

}

BOOL CLinkTxtFile::OnEditProperties()
{
 	CPropertySheet sheet( _T("文本链接对象"));
	CPropLinkFile dlg;
	
	dlg.Setfile("global\\txt",m_name,"txt"); 
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.Getfile(m_name); 
		return TRUE;
	}
	
	return FALSE;
}

void CLinkTxtFile::Serialize_Ty(CArchive &ar)
{

}


////////////////////////////////////////////////////////////////////////
//
//		CLinkWebShot
//
////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkWebShot,CLinkObjectBase,0)

CLinkWebShot::CLinkWebShot(void)
:CLinkObjectBase(lnkWebShot,"")
{
	memset(m_unitName,0,sizeof(char)*33);
	memset(m_shotName,0,sizeof(char)*33);
	m_ServerPort=8190;
	m_GroupPort=8191;
	m_shotID=0;
}

CLinkWebShot::CLinkWebShot(CString strname,DWORD webIP,DWORD serverIP,DWORD serverPort,DWORD groupIP,DWORD groupPort,int unitID,CString unitName)
:CLinkObjectBase(lnkWebShot,strname)
{
	memset(m_unitName,0,sizeof(char)*33);
	memset(m_shotName,0,sizeof(char)*33);
	m_webIP=webIP;
	m_ServerIP=serverIP;
	m_ServerPort=serverPort;
	m_GroupIP=groupIP;
	m_GroupPort=groupPort;
	m_unitID=unitID;
	if (!unitName.IsEmpty())
	{
		ASSERT(unitName.GetLength()<=32);
		if (unitName.GetLength()<=32)
			strcpy(m_unitName,unitName);
	}
	m_shotID=0;
}

CLinkWebShot::~CLinkWebShot(void)
{

}

void CLinkWebShot::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_webIP,sizeof(DWORD));
		file->Write((unsigned char *)&m_ServerIP,sizeof(DWORD));
		file->Write((unsigned char *)&m_ServerPort,sizeof(int));
		file->Write((unsigned char *)&m_GroupIP,sizeof(DWORD));
		file->Write((unsigned char *)&m_GroupPort,sizeof(int));
		file->Write((unsigned char *)&m_unitID,sizeof(int));
		file->Write((unsigned char *)m_unitName,sizeof(char)*33);
		file->Write((unsigned char *)&m_shotID,sizeof(int));
		file->Write((unsigned char *)m_shotName,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)&m_webIP,sizeof(DWORD));
		file->Read((unsigned char *)&m_ServerIP,sizeof(DWORD));
		file->Read((unsigned char *)&m_ServerPort,sizeof(int));
		file->Read((unsigned char *)&m_GroupIP,sizeof(DWORD));
		file->Read((unsigned char *)&m_GroupPort,sizeof(int));
		file->Read((unsigned char *)&m_unitID,sizeof(int));
		file->Read((unsigned char *)m_unitName,sizeof(char)*33);
		file->Read((unsigned char *)&m_shotID,sizeof(int));
		file->Read((unsigned char *)m_shotName,sizeof(char)*33);
	}
}

void CLinkWebShot::SetDefaultProp()
{

	CLinkWebShotTool::c_webIP=m_webIP;
	CLinkWebShotTool::c_serverIP=m_ServerIP;
	CLinkWebShotTool::c_serverPort=m_ServerPort;
	CLinkWebShotTool::c_groupIP=m_GroupIP;
	CLinkWebShotTool::c_groupPort=m_GroupPort;
	CLinkWebShotTool::c_unitID=m_unitID;
	strcpy(CLinkWebShotTool::c_unitName,m_unitName);
}

BOOL CLinkWebShot::OnEditProperties()
{
 	CPropertySheet sheet( _T("图像监控web系统 镜头链接对象"));
	CPropWebPhoto dlg;

	dlg.SetProp(m_name,m_webIP,m_ServerIP,m_ServerPort,m_GroupIP,m_GroupPort,m_unitID,m_unitName,m_shotID,m_shotName);
	sheet.AddPage((CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetProp(m_name,m_webIP,m_ServerIP,m_ServerPort,m_GroupIP,m_GroupPort,m_unitID,m_unitName,m_shotID,m_shotName);
		SetDefaultProp();
		return TRUE;
	}
	return FALSE;
}

void CLinkWebShot::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_webIP<<m_ServerIP<<m_ServerPort<<m_GroupIP<<m_GroupPort<<m_unitID;
		ar.Write((unsigned char *)m_unitID,sizeof(char)*33);
	}
	else
	{
		ar>>m_webIP>>m_ServerIP>>m_ServerPort>>m_GroupIP>>m_GroupPort>>m_unitID;
		ar.Read((unsigned char *)m_unitID,sizeof(char)*33);
	}
}




