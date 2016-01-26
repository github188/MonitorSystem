// SysLinkTy.cpp: implementation of the CLinkYk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SysLinkTy.h"
#include "dialog/PropYK.h"
#include "canvas/TyBase.h"
#include "dialog/PropColor.h"
#include "dialog/PropYcDot.h"
#include "dialog/PropNetBmp.h"
#include "dialog/PropYT.h"
//#include "PropYZTY.h"
#include "dialog/PropUnit.h"
#include "dialog/Prop_Yk_Sec.h"
#include "canvas/LinkTool.h"
#include "dialog/PropNode.h"
#include "dialog/PropLinkLine.h"
#include "dialog/propmodemdlg.h"
#include "dialog/PropYT2.h"
#include "dialog/propyxbinddlg.h"
#include "dialog/propqkdlg.h"
#include "dialog/propsoftrepair.h"
#include "dialog/propunitextendDlg.h"
#include "dialog/PropYZTYDlg.h"
#include "dialog/propkxdlg.h"
#include "dialog/propkxarea.h"
#include "dialog/PropKXAreaTypeDlg.h"
#include "dialog/PropDoubleCtrl.h"
#include "dialog/PropZk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLinkYk,CLinkObjectBase,0)

CLinkYk::CLinkYk()
:CLinkObjectBase(lnkYK,"")
,m_bWatchMan(false)
,m_bGroup(false)
,m_dwFlag(0)
{
	m_BS_Array.RemoveAll();
	InitMemory();  
}


CLinkYk::CLinkYk(CString strname,bool bWatchMan,bool bGroup,DWORD dwFlag)
:CLinkObjectBase(lnkYK,strname)
,m_bWatchMan(bWatchMan)
,m_bGroup(bGroup)
,m_dwFlag(dwFlag)
{
	m_BS_Array.RemoveAll();
  InitMemory();  
}

void CLinkYk::InitMemory()
{
	int i;
	int loop=m_BS_Array.GetSize();
	tagBSItem * pBSItem;

	for (i=0;i<loop;i++)
	{
		pBSItem=m_BS_Array[i];
		ASSERT(pBSItem!=NULL);
	  if (pBSItem!=NULL)
      delete pBSItem;
	}
  
	m_BS_Array.RemoveAll();

	m_bZK=TRUE;
	ZeroMemory(m_Ykname,sizeof(char)*17);
	ZeroMemory(m_Yxname_FX_Close,sizeof(char)*17);
	ZeroMemory(m_Yxname_FX_Open,sizeof(char)*17);
	ZeroMemory(m_Yxname_Hang,sizeof(char)*17);
	ZeroMemory(m_Yxname_Yk,sizeof(char)*17);
}


CLinkYk::~CLinkYk()
{
	InitMemory();
}

void CLinkYk::Serialize_Ty(CArchive &ar)
{
	//int loop,i;
	//tagBSItem * pBSItem;
	
	if(ar.IsStoring())  
	{
		ar<<m_bZK<<m_bWatchMan<<m_bGroup<<m_dwFlag;

/*
		ar.Write((unsigned char *)m_Ykname,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_FX_Close,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_FX_Open,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
	  loop=m_BS_Array.GetSize();
		ar<<loop;
		for (i=0;i<=loop-1;i++)
		{
			pBSItem=m_BS_Array[i];
			ASSERT(pBSItem!=NULL);
			ar.Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}
*/
	}
	else
	{
		ar>>m_bZK>>m_bWatchMan>>m_bGroup>>m_dwFlag;
/*	
		ar.Read((unsigned char *)m_Ykname,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_FX_Close,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_FX_Open,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
    ar>>loop;
		for (i=0;i<=loop-1;i++)
		{
	    pBSItem=new tagBSItem;
			if (pBSItem!=NULL)
			{
			  ar.Read((unsigned char *)pBSItem,sizeof(tagBSItem));
				m_BS_Array.Add(pBSItem);
			}
			else 
				AfxThrowMemoryException();
		}
*/
	}

}

BOOL CLinkYk::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropYK dlg;
	CProp_Yk_Sec dlgSec;
	dlg.SetYkParam(m_name,m_Ykname,m_bZK,m_Yxname_Hang,m_Yxname_FX_Close,m_Yxname_FX_Open,m_Yxname_Yk,m_BS_Array); 
	sheet.AddPage( (CPropertyPage*)&dlg );
	dlgSec.SetData(m_bWatchMan,m_bGroup,m_dwFlag);
	sheet.AddPage( (CPropertyPage*)&dlgSec);

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetYkParam(m_name,m_Ykname,m_bZK,m_Yxname_Hang,m_Yxname_FX_Close,m_Yxname_FX_Open,m_Yxname_Yk,m_BS_Array); 
		dlgSec.GetData(m_bWatchMan,m_bGroup,m_dwFlag);
		CLinkYkTool::c_bWatchMan=m_bWatchMan;							
		CLinkYkTool::c_bGroup=m_bGroup;								
		CLinkYkTool::c_dwFlag=m_dwFlag;										

		return TRUE;
  }

  return FALSE;
}

void CLinkYk::Save_Ty(CFile *file, BOOL Yn)
{
	int loop,i;
	tagBSItem * pBSItem;

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_bZK,sizeof(m_bZK));
		file->Write((unsigned char *)m_Ykname,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_FX_Close,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_FX_Open,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		loop=m_BS_Array.GetSize();
		file->Write((unsigned char *)&loop,sizeof(loop));


		for (i=0;i<=loop-1;i++)
		{
			pBSItem=m_BS_Array[i];
			ASSERT((pBSItem!=NULL));
			
			file->Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}

		file->Write((unsigned char *)&m_bWatchMan,sizeof(bool));
		file->Write((unsigned char *)&m_bGroup,sizeof(bool));
		file->Write((unsigned char *)&m_dwFlag,sizeof(DWORD));

	}
	else
	{
		file->Read((unsigned char *)&m_bZK,sizeof(m_bZK));
		file->Read((unsigned char *)m_Ykname,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_FX_Close,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_FX_Open,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		file->Read((unsigned char *)&loop,sizeof(loop));

		for (i=0;i<=loop-1;i++)
		{
			pBSItem=new tagBSItem;
			if (pBSItem!=NULL)
			{
			  file->Read((unsigned char *)pBSItem,sizeof(tagBSItem));
				m_BS_Array.Add(pBSItem);
			}
			else 
				AfxThrowMemoryException();
		}

		file->Read((unsigned char *)&m_bWatchMan,sizeof(bool));
		file->Read((unsigned char *)&m_bGroup,sizeof(bool));
		file->Read((unsigned char *)&m_dwFlag,sizeof(DWORD));
	}
}

void CLinkYk::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Ykname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Ykname,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Hang,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Yxname_Hang,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_FX_Close,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Yxname_FX_Close,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_FX_Open,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Yxname_FX_Open,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Yk,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Yxname_Yk,str);
	}

	tagBSItem* pItem;
	int count=m_BS_Array.GetCount();
	for (int i=0;i<count;i++)
	{
		pItem=m_BS_Array[i];
		ASSERT(pItem!=NULL);
		if (pItem==NULL) continue;
		ret=CTyBase::GetEname_From_ReplacRule(pItem->strYxName,str,ruleList);
		if (ret>0)
		{
			SetModifiedFlag(TRUE);
			strcpy(pItem->strYxName,str);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLinkZk,CLinkObjectBase,0)

CLinkZk::CLinkZk()
:CLinkObjectBase(lnkZK,"")

{
	InitMemory();  
}


CLinkZk::CLinkZk(CString strname,bool bWatchMan,bool bGroup,DWORD dwFlag)
:CLinkObjectBase(lnkZK,"直控")
{
	InitMemory();  
}

void CLinkZk::InitMemory()
{
	ZeroMemory(m_Yxname_Hang,sizeof(char)*17);

}


CLinkZk::~CLinkZk()
{
	InitMemory();
}

void CLinkZk::Serialize_Ty(CArchive &ar)
{
	

}

BOOL CLinkZk::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropZK dlg;
	if(m_node<0) m_node=0;
	if(m_line<0) m_line=0;
	if(m_rtu<0) m_rtu=0;
	dlg.SetYkParam(m_Yxname_Hang,m_node,m_line,m_rtu);
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
        dlg.GetYkParam(m_Yxname_Hang,&m_node,&m_line,&m_rtu);
		return TRUE;
	}
	return FALSE;
}

void CLinkZk::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{	
		file->Write((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
		file->Write((unsigned char *)&m_node,sizeof(m_node));
		file->Write((unsigned char *)&m_line,sizeof(m_line));
		file->Write((unsigned char *)&m_rtu,sizeof(m_rtu));
	}
	else
	{	
		file->Read((unsigned char *)m_Yxname_Hang,sizeof(char)*17);
        file->Read((unsigned char *)&m_node,sizeof(m_node));
		file->Read((unsigned char *)&m_line,sizeof(m_line));
		file->Read((unsigned char *)&m_rtu,sizeof(m_rtu));
	}
}

void CLinkZk::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret;

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Hang,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(TRUE);
		strcpy(m_Yxname_Hang,str);
	}
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNetColorPower,CLinkObjectBase,0)

CNetColorPower::CNetColorPower()
:CLinkObjectBase(lnkNetColorPower,"动态着色电源")
{
	InitMemory();  
}

void CNetColorPower::InitMemory()
{
	ZeroMemory(m_Ycname,sizeof(char)*17);
}

CNetColorPower::~CNetColorPower()
{

}

void CNetColorPower::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)m_Ycname,sizeof(char)*17);
	}
	else
	{
		ar.Read((unsigned char *)m_Ycname,sizeof(char)*17);
	}
}

BOOL CNetColorPower::OnEditProperties()
{
	CPropertySheet sheet( _T("着色层电源..") );
	CPropYcDot dlg;
	BOOL ret=FALSE;

	dlg.Set_Yc_Dot_Name(m_Ycname);
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.Get_Yc_Dot_Name(m_Ycname,17);
		ret=TRUE;
	}
	return ret;
}

void CNetColorPower::Save_Ty(CFile *file, BOOL Yn)
{

	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_Ycname,sizeof(char)*17);
	}
	else
	{
		file->Read((unsigned char *)m_Ycname,sizeof(char)*17);
	}
}

void CNetColorPower::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Ycname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Ycname,str);
	}
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNetColorVector,CLinkObjectBase,0)

CNetColorVector::CNetColorVector()
:CLinkObjectBase(lnkNetColorVect,"失电颜色")
{
  InitMemory();  
}


void CNetColorVector::InitMemory()
{
	//ZeroMemory(m_Ycname,sizeof(char)*17);
	m_color=RGB(200,200,200);
}


CNetColorVector::~CNetColorVector()
{

}

void CNetColorVector::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_color;
	}
	else
	{
		ar>>m_color;
	}
}

BOOL CNetColorVector::OnEditProperties()
{
	BOOL ret=FALSE;
	CPropertySheet sheet( _T("着色层连接线失电颜色..") );
	CPropColor dlg;
	dlg.SetColor(m_color);
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetColor(m_color); 
		ret=TRUE;
	}

	return ret;
}

void CNetColorVector::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&m_color,sizeof(m_color));
	}
	else
	{
		file->Read((unsigned char *)&m_color,sizeof(m_color));
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNetColorBitmap,CLinkObjectBase,0)

CNetColorBitmap::CNetColorBitmap()
:CLinkObjectBase(lnkNetColorBmp,"失电位图")
{
	InitMemory();  
}


void CNetColorBitmap::InitMemory()
{
	ZeroMemory(m_BmpName,sizeof(char)*33);
}


CNetColorBitmap::~CNetColorBitmap()
{

}

void CNetColorBitmap::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)m_BmpName,sizeof(char)*33);
	}
	else
	{
		ar.Read((unsigned char *)m_BmpName,sizeof(char)*33);
	}
}

BOOL CNetColorBitmap::OnEditProperties()
{
	BOOL ret=FALSE;

	CPropertySheetEx sheet( _T("着色层连接位图.."));
	CPropNetBmp dlg;
	dlg.Set_Bmp_Name(m_BmpName);
	sheet.AddPage( (CPropertyPageEx*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.Get_Bmp_Name(m_BmpName,33); 
		ret=TRUE;
	}

	return ret;
}

void CNetColorBitmap::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_BmpName,sizeof(char)*33);
	}
	else
	{
		file->Read((unsigned char *)m_BmpName,sizeof(char)*33);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNetColorYx,CLinkObjectBase,0)

CNetColorYx::CNetColorYx()
:CLinkObjectBase(lnkNetColorYx,"动态着色开关")
{

}




CNetColorYx::~CNetColorYx()
{

}

void CNetColorYx::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
	//	ar.Write((unsigned char *)m_BmpName,sizeof(char)*17);
	}
	else
	{
		//ar.Read((unsigned char *)m_BmpName,sizeof(char)*17);
	}
}

BOOL CNetColorYx::OnEditProperties()
{
	return TRUE;
/*
	 BOOL ret=FALSE;

	 if (AfxMessageBox("真的要把这个开关作为网络着色层开关吗",MB_YESNO)==IDYES)
		 ret=TRUE;
	 return ret;
*/
}

void CNetColorYx::Save_Ty(CFile *file, BOOL Yn)
{

	if(Yn)	//如果是在进行保存
	{
//		file->Write((unsigned char *)m_BmpName,sizeof(char)*17);
	}
	else
	{
//		file->Read((unsigned char *)m_BmpName,sizeof(char)*17);
	}
}

IMPLEMENT_SERIAL(CLinkYT,CLinkObjectBase,0)

CLinkYT::CLinkYT()
:CLinkObjectBase(lnkYT,"")
{
	m_BS_Array.RemoveAll();
	InitMemory();  
}

CLinkYT::CLinkYT(CString strname)
:CLinkObjectBase(lnkYT,strname)
{
	m_BS_Array.RemoveAll();
	InitMemory();  
}

void CLinkYT::InitMemory()
{
	int i;
	int loop=m_BS_Array.GetSize();
	tagBSItem * pBSItem;

	for (i=0;i<loop;i++)
	{
		pBSItem=m_BS_Array[i];
		ASSERT(pBSItem!=NULL);
		if (pBSItem!=NULL)
			delete pBSItem;
	}

	m_BS_Array.RemoveAll();

	m_dataType=0;
	ZeroMemory(m_YTname,sizeof(char)*17);
	ZeroMemory(m_Yxname_Yk,sizeof(char)*17);
}


CLinkYT::~CLinkYT()
{
	InitMemory();
}

void CLinkYT::Serialize_Ty(CArchive &ar)
{
//	int loop,i;
//	tagBSItem * pBSItem;

	if(ar.IsStoring())  
	{
		ar<<m_dataType;
/*	
		ar.Write((unsigned char *)m_YTname,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
	  loop=m_BS_Array.GetSize();
		ar<<loop;
		for (i=0;i<=loop-1;i++)
		{
			pBSItem=m_BS_Array[i];
			ASSERT(pBSItem!=NULL);
			ar.Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}
*/
	}
	else
	{
		ar>>m_dataType;
/*	
		ar.Read((unsigned char *)m_YTname,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
    ar>>loop;
		for (i=0;i<=loop-1;i++)
		{
	    pBSItem=new tagBSItem;
			if (pBSItem!=NULL)
			{
			  ar.Read((unsigned char *)pBSItem,sizeof(tagBSItem));
				m_BS_Array.Add(pBSItem);
			}
			else 
				AfxThrowMemoryException();
		}
*/
	}
}

void CLinkYT::Save_Ty(CFile *file, BOOL Yn)
{
	int loop,i;
	tagBSItem * pBSItem;

	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_dataType,sizeof(m_dataType));
		file->Write((unsigned char *)m_YTname,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		loop=m_BS_Array.GetSize();
		file->Write((unsigned char *)&loop,sizeof(loop));

		for (i=0;i<=loop-1;i++)
		{
			pBSItem=m_BS_Array[i];
			ASSERT((pBSItem!=NULL));
			
			file->Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}
	}
	else
	{
		file->Read((int *)&m_dataType,sizeof(m_dataType));
		file->Read((unsigned char *)m_YTname,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		file->Read((unsigned char *)&loop,sizeof(loop));

		for (i=0;i<=loop-1;i++)
		{
			pBSItem=new tagBSItem;
			if (pBSItem!=NULL)
			{
				file->Read((unsigned char *)pBSItem,sizeof(tagBSItem));
				m_BS_Array.Add(pBSItem);
			}
			else 
				AfxThrowMemoryException();
		}
	}
}

BOOL CLinkYT::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropYT dlg;
	dlg.SetYTParam(m_name,m_YTname,m_dataType,m_Yxname_Yk,m_BS_Array); 
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetYTParam(m_name,m_YTname,m_dataType,m_Yxname_Yk,m_BS_Array); 
		return TRUE;
	}

	return FALSE;
}

void CLinkYT::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_YTname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YTname,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Yk,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Yxname_Yk,str);
	}

	tagBSItem* pItem;
	int count=m_BS_Array.GetCount();
	for (int i=0;i<count;i++)
	{
		pItem=m_BS_Array[i];
		ASSERT(pItem!=NULL);
		if (pItem==NULL) continue;
		ret=CTyBase::GetEname_From_ReplacRule(pItem->strYxName,str,ruleList);
		if (ret>0)
		{
			SetModifiedFlag(TRUE);
			strcpy(pItem->strYxName,str);
		}
	}
}


//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkYZTY,CLinkObjectBase,0)

CLinkYZTY::CLinkYZTY()
:CLinkObjectBase(lnkYZTY,"")
{
	InitMemory();  
}

CLinkYZTY::CLinkYZTY(CString strname)
:CLinkObjectBase(lnkYZTY,strname)
{
	InitMemory();  
}

void CLinkYZTY::InitMemory()
{
	m_ActionType_Up=0;
	m_ActionType_Down=0;
	m_ActionType_Stop=0;
	ZeroMemory(m_YZTYname_Up,sizeof(char)*17);
	ZeroMemory(m_YZTYname_Down,sizeof(char)*17);
	ZeroMemory(m_YZTYname_Stop,sizeof(char)*17);
	ZeroMemory(m_Yxname_Yk,sizeof(char)*17);
}

CLinkYZTY::~CLinkYZTY()
{
	InitMemory();
}

void CLinkYZTY::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_ActionType_Up;
		ar<<m_ActionType_Down;
		ar<<m_ActionType_Stop;
/*
		ar.Write((unsigned char *)m_YZTYname_Up,sizeof(char)*17);
		ar.Write((unsigned char *)m_YZTYname_Down,sizeof(char)*17);
		ar.Write((unsigned char *)m_YZTYname_Stop,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
*/
	}
	else
	{
		ar>>m_ActionType_Up;
		ar>>m_ActionType_Down;
		ar>>m_ActionType_Stop;
/*
		ar.Read((unsigned char *)m_YZTYname_Up,sizeof(char)*17);
		ar.Read((unsigned char *)m_YZTYname_Down,sizeof(char)*17);
		ar.Read((unsigned char *)m_YZTYname_Stop,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
*/
	}
}

BOOL CLinkYZTY::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	///////////////////////////////////////////
	//2004.05.21 pang 修改
	//CPropYZTY dlg;
	CPropYZTYDlg dlg ;
	dlg.SetParam(m_name,m_YZTYname_Up,m_YZTYname_Down,m_YZTYname_Stop,m_ActionType_Up,m_ActionType_Down,m_ActionType_Stop,m_Yxname_Yk); 
	//修改结束
	/////////////////////////////////////////////
	//dlg.SetYZTYParam(m_name,m_YZTYname_Up,m_YZTYname_Down,m_YZTYname_Stop,m_ActionType_Up,m_ActionType_Down,m_ActionType_Stop,m_Yxname_Yk); 

	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetParam(m_name,m_YZTYname_Up,m_YZTYname_Down,m_YZTYname_Stop,m_ActionType_Up,m_ActionType_Down,m_ActionType_Stop,m_Yxname_Yk); 
		//dlg.GetYZTYParam(m_name,m_YZTYname_Up,m_YZTYname_Down,m_YZTYname_Stop,m_ActionType_Up,m_ActionType_Down,m_ActionType_Stop,m_Yxname_Yk); 
		return TRUE;
	}
	
	return FALSE;
}

void CLinkYZTY::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_ActionType_Up,sizeof(m_ActionType_Up));
		file->Write((int *)&m_ActionType_Down,sizeof(m_ActionType_Down));
		file->Write((int *)&m_ActionType_Stop,sizeof(m_ActionType_Stop));
		file->Write((unsigned char *)m_YZTYname_Up,sizeof(char)*17);
		file->Write((unsigned char *)m_YZTYname_Down,sizeof(char)*17);
		file->Write((unsigned char *)m_YZTYname_Stop,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
	}
	else
	{
		file->Read((int *)&m_ActionType_Up,sizeof(m_ActionType_Up));
		file->Read((int *)&m_ActionType_Down,sizeof(m_ActionType_Up));
		file->Read((int *)&m_ActionType_Stop,sizeof(m_ActionType_Up));
		file->Read((unsigned char *)m_YZTYname_Up,sizeof(char)*17);
		file->Read((unsigned char *)m_YZTYname_Down,sizeof(char)*17);
		file->Read((unsigned char *)m_YZTYname_Stop,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
	}
}


void CLinkYZTY::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_YZTYname_Up,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YZTYname_Up,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_YZTYname_Down,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YZTYname_Down,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_YZTYname_Stop,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YZTYname_Stop,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Yk,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Yxname_Yk,str);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkUnit,CLinkObjectBase,0)

CLinkUnit::CLinkUnit()
:CLinkObjectBase(lnkUnit,"")
{
  InitMemory();  
}

CLinkUnit::CLinkUnit(CString strname)
:CLinkObjectBase(lnkUnit,strname)
{
  InitMemory();  
}

void CLinkUnit::InitMemory()
{
  m_Fun=0;
	ZeroMemory(m_Unitname,sizeof(char)*17);
}

CLinkUnit::~CLinkUnit()
{
	InitMemory();
}

void CLinkUnit::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_Fun;
//		ar.Write((unsigned char *)m_Unitname,sizeof(char)*17);
	}
	else
	{
		ar>>m_Fun;
//		ar.Read((unsigned char *)m_Unitname,sizeof(char)*17);
	}
}

BOOL CLinkUnit::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropUnit dlg;
	BOOL m_Bhtq,m_Soe,m_Yc,m_Yx,m_Diandu,m_Lb,m_Set,m_Canshu,m_pct ,m_yxfg,m_sgbw,m_qlb,m_cklb;
	dlg.SetUnitParam(m_name,m_Unitname,(BOOL)(m_Fun&0x1),(BOOL)(m_Fun&0x2),(BOOL)(m_Fun&0x4),(BOOL)(m_Fun&0x8),
		(BOOL)(m_Fun&0x10),(BOOL)(m_Fun&0x20),(BOOL)(m_Fun&0x40),(BOOL)(m_Fun&0x80),(BOOL)(m_Fun&0x100) ,(BOOL)(m_Fun&0x200), (BOOL)(m_Fun&0x400), (BOOL)(m_Fun&0x800), (BOOL)(m_Fun&0x1000)); 
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetUnitParam(m_name,m_Unitname,m_Bhtq,m_Soe,m_Yc,m_Yx,m_Diandu,m_Lb,m_Set,m_Canshu,m_pct ,m_yxfg, m_sgbw ,m_qlb ,m_cklb); 
		m_Fun=(m_Bhtq?1:0)|((m_Soe?1:0)<<1)|((m_Yc?1:0)<<2)|((m_Yx?1:0)<<3)
			|((m_Diandu?1:0)<<4)|((m_Lb?1:0)<<5)|((m_Set?1:0)<<6)|((m_Canshu?1:0)<<7)|((m_pct?1:0)<<8) | ((m_yxfg?1:0)<<9) | ((m_sgbw?1:0)<<10) | ((m_qlb?1:0)<<11) | ((m_cklb?1:0)<<12);
		return TRUE;
	}

	return FALSE;
}

void CLinkUnit::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_Fun,sizeof(m_Fun));
		file->Write((unsigned char *)m_Unitname,sizeof(char)*17);
	}
	else
	{
		file->Read((int *)&m_Fun,sizeof(m_Fun));
		file->Read((unsigned char *)m_Unitname,sizeof(char)*17);
	}
}

void CLinkUnit::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Unitname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Unitname,str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkNode ,CLinkObjectBase ,0) 

CLinkNode::CLinkNode()
:CLinkObjectBase(lnkNode ,"")
{
	InitMemory() ;
}

CLinkNode::~CLinkNode()
{
	InitMemory() ;
}

void CLinkNode::InitMemory()
{
	m_Fun=0;
	ZeroMemory(m_Nodename,sizeof(char)*17);
}

void CLinkNode::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())	
	{
		ar<<m_Fun ;
		ar.Write((unsigned char *)m_Nodename,sizeof(char)*17);
	}
	else
	{
		ar>>m_Fun ;
		ar.Read((unsigned char *)m_Nodename ,sizeof(char)*17) ;
	}
}

BOOL CLinkNode::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropNode dlg;
	BOOL m_Soe,m_Yc,m_Yx,m_Diandu,m_epd ;

	dlg.SetNodeParam(m_name,m_Nodename,(BOOL)(m_Fun&0x1),(BOOL)(m_Fun&0x2),(BOOL)(m_Fun&0x4),(BOOL)(m_Fun&0x8),(BOOL)(m_Fun&0x10)); 
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetNodeParam(m_name,m_Nodename,m_Soe,m_Yc,m_Yx,m_Diandu,m_epd); 
		m_Fun=(m_Soe?1:0)|((m_Yc?1:0)<<1)|((m_Yx?1:0)<<2)
			|((m_Diandu?1:0)<<3)| ((m_epd?1:0)<<4) ;
		return TRUE;
	}
	return FALSE;
}

void CLinkNode::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_Fun,sizeof(m_Fun));
		file->Write((unsigned char *)m_Nodename,sizeof(char)*17);
	}
	else
	{
		file->Read((int *)&m_Fun,sizeof(m_Fun));
		file->Read((unsigned char *)m_Nodename,sizeof(char)*17);
	}
}

void CLinkNode::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Nodename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Nodename,str);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLinkLine,CLinkObjectBase,0)

CLinkLine::CLinkLine()
:CLinkObjectBase(lnkLine,"")
{
	InitMemory();  
}

CLinkLine::CLinkLine(CString strname)
:CLinkObjectBase(lnkLine,strname)
{
	InitMemory();  
}

void CLinkLine::InitMemory()
{
	m_Fun=0;
	ZeroMemory(m_Linename,sizeof(char)*17);
}

CLinkLine::~CLinkLine()
{
	InitMemory();
}

void CLinkLine::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_Fun;
//		ar.Write((unsigned char *)m_Unitname,sizeof(char)*17);
	}
	else
	{
		ar>>m_Fun;
//		ar.Read((unsigned char *)m_Unitname,sizeof(char)*17);
	}
}

BOOL CLinkLine::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropLinkLine dlg;
	BOOL m_Soe,m_Yc,m_Yx,m_Diandu,m_Epd;
	dlg.SetLineParam(m_name,m_Linename,(BOOL)(m_Fun&0x1),(BOOL)(m_Fun&0x2),(BOOL)(m_Fun&0x4),(BOOL)(m_Fun&0x8),(BOOL)(m_Fun&0x10)); 
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetLineParam(m_name,m_Linename,m_Soe,m_Yc,m_Yx,m_Diandu,m_Epd); 
		m_Fun=(m_Soe?1:0)|((m_Yc?1:0)<<1)|((m_Yx?1:0)<<2)
			|((m_Diandu?1:0)<<3) | ((m_Epd?1:0)<<4);
		return TRUE;
	}
	return FALSE;
}

void CLinkLine::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_Fun,sizeof(m_Fun));
		file->Write((unsigned char *)m_Linename,sizeof(char)*17);
	}
	else
	{
		file->Read((int *)&m_Fun,sizeof(m_Fun));
		file->Read((unsigned char *)m_Linename,sizeof(char)*17);
	}
}


void CLinkLine::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Linename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Linename,str);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////

//拨号图元
IMPLEMENT_SERIAL(CLinkModem,CLinkObjectBase,0)

CLinkModem::CLinkModem()
:CLinkObjectBase(lnkModem,"")
{
	InitMemory();  
}

void CLinkModem::InitMemory()
{
	m_modem.adptid	= 0 ;
	m_modem.dotime	= 0 ;
	ZeroMemory(m_modem.telno ,sizeof(char)*12) ;
}

CLinkModem::~CLinkModem()
{
	InitMemory();
}

void CLinkModem::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write(&m_modem ,sizeof(ModemManualDailEvent)) ;
	}
	else
	{
		ar.Read(&m_modem ,sizeof(ModemManualDailEvent)) ;
	}
}

BOOL CLinkModem::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropModemDlg dlg ;
    dlg.SetParam(m_name ,m_modem.adptid ,m_modem.telno ,m_modem.dotime) ;
	sheet.AddPage((CPropertyPage*)&dlg) ;
	if(sheet.DoModal() == IDOK)
	{
		dlg.GetParam(m_name ,m_modem.adptid ,m_modem.telno ,m_modem.dotime) ;
		if(m_modem.dotime == 0)
			AfxMessageBox("拨号无时间限制") ;
		return true ;
	}
	return false ;
}

void CLinkModem::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char *)&m_modem.adptid ,sizeof(m_modem.adptid)) ;
		file->Write((unsigned char *)&m_modem.dotime ,sizeof(m_modem.dotime)) ;
		file->Write((unsigned char *)m_modem.telno ,sizeof(char)*12) ;
	}
	else
	{
		file->Read((unsigned char *)&m_modem.adptid ,sizeof(m_modem.adptid)) ;
		file->Read((unsigned char *)&m_modem.dotime ,sizeof(m_modem.dotime)) ;
		file->Read((unsigned char *)m_modem.telno ,sizeof(char)*12) ;
	}
}

////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CLinkYT2,CLinkObjectBase,0)

CLinkYT2::CLinkYT2()
:CLinkObjectBase(lnkYT2,"")
{
	m_BS_Array.RemoveAll();
	InitMemory();  
}

CLinkYT2::CLinkYT2(CString strname)
:CLinkObjectBase(lnkYT2,strname)
{
	m_BS_Array.RemoveAll();
	InitMemory();  
}

void CLinkYT2::InitMemory()
{
	int i;
	int loop=m_BS_Array.GetSize();
	tagBSItem * pBSItem;

	for (i=0;i<loop;i++)
	{
		pBSItem=m_BS_Array[i];
		ASSERT(pBSItem!=NULL);
		if (pBSItem!=NULL)
			delete pBSItem;
	}
	m_BS_Array.RemoveAll();

	m_dataType=0;
	ZeroMemory(m_YTname,sizeof(char)*17);
	ZeroMemory(m_Yxname_Yk,sizeof(char)*17);

	m_fPianCha = 0;
	m_nXiaoShu = 0;
	m_nBiLi = 0;
}


CLinkYT2::~CLinkYT2()
{
	InitMemory();
}

void CLinkYT2::Serialize_Ty(CArchive &ar)
{
	//	int loop,i;
	//	tagBSItem * pBSItem;

	if(ar.IsStoring())  
	{
		ar<<m_dataType;
		/*	
		ar.Write((unsigned char *)m_YTname,sizeof(char)*17);
		ar.Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		loop=m_BS_Array.GetSize();
		ar<<loop;
		for (i=0;i<=loop-1;i++)
		{
		pBSItem=m_BS_Array[i];
		ASSERT(pBSItem!=NULL);
		ar.Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}
		*/
	}
	else
	{
		ar>>m_dataType;
		/*	
		ar.Read((unsigned char *)m_YTname,sizeof(char)*17);
		ar.Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		ar>>loop;
		for (i=0;i<=loop-1;i++)
		{
		pBSItem=new tagBSItem;
		if (pBSItem!=NULL)
		{
		ar.Read((unsigned char *)pBSItem,sizeof(tagBSItem));
		m_BS_Array.Add(pBSItem);
		}
		else 
		AfxThrowMemoryException();
		}
		*/
	}
}

void CLinkYT2::Save_Ty(CFile *file, BOOL Yn)
{
	int loop,i;
	tagBSItem * pBSItem;

	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_dataType,sizeof(m_dataType));
		file->Write((unsigned char *)m_YTname,sizeof(char)*17);
		file->Write((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		loop=m_BS_Array.GetSize();
		file->Write((unsigned char *)&loop,sizeof(loop));

		for (i=0;i<=loop-1;i++)
		{
			pBSItem=m_BS_Array[i];
			ASSERT((pBSItem!=NULL));

			file->Write((unsigned char *)pBSItem,sizeof(tagBSItem));
		}

		file->Write((float*)&m_fPianCha, sizeof(m_fPianCha));
		file->Write((unsigned char*)&m_nXiaoShu, sizeof(m_nXiaoShu));
		file->Write((unsigned char*)&m_nBiLi, sizeof(m_nBiLi));
	}
	else
	{
		file->Read((int *)&m_dataType,sizeof(m_dataType));
		file->Read((unsigned char *)m_YTname,sizeof(char)*17);
		file->Read((unsigned char *)m_Yxname_Yk,sizeof(char)*17);
		file->Read((unsigned char *)&loop,sizeof(loop));

		for (i=0;i<=loop-1;i++)
		{
			pBSItem=new tagBSItem;
			if (pBSItem!=NULL)
			{
				file->Read((unsigned char *)pBSItem,sizeof(tagBSItem));
				m_BS_Array.Add(pBSItem);
			}
			else 
				AfxThrowMemoryException();
		}

		file->Read((float*)&m_fPianCha, sizeof(m_fPianCha));
		file->Read((unsigned char*)&m_nXiaoShu, sizeof(m_nXiaoShu));
		file->Read((unsigned char*)&m_nBiLi, sizeof(m_nBiLi));
	}
}

BOOL CLinkYT2::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropYT2 dlg;
	dlg.SetYTParam(m_name,m_YTname,m_dataType,m_Yxname_Yk,m_BS_Array, m_fPianCha, m_nXiaoShu, m_nBiLi); 
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetYTParam(m_name,m_YTname,m_dataType,m_Yxname_Yk,m_BS_Array, m_fPianCha, m_nXiaoShu, m_nBiLi); 
		return TRUE;
	}

	return FALSE;
}

void CLinkYT2::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_YTname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YTname,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_Yxname_Yk,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Yxname_Yk,str);
	}
}


///////////////////////////////////////////////////////////////////////
//遥信绑定图元
//拨号图元
IMPLEMENT_SERIAL(CLinkYXBind,CLinkObjectBase,0)

CLinkYXBind::CLinkYXBind()
:CLinkObjectBase(lnkYXBind,"")
{
	memset(m_Ename ,0 ,17) ;
}
CLinkYXBind::~CLinkYXBind()
{
}

void CLinkYXBind::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_Ename,sizeof(char)*17);
	}
	else
	{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*17);
	}
}

BOOL CLinkYXBind::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropYxBindDlg dlg ;
	dlg.SetParam(m_Ename ,m_name) ;
    
	sheet.AddPage((CPropertyPage*)&dlg) ;
	if(sheet.DoModal() == IDOK)
	{		
		dlg.GetParam(m_Ename ,m_name) ;
		return true ;
	}
	return false ;
}

void CLinkYXBind::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
	else
	{
		file->Read((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
}

void CLinkYXBind::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Ename,str);
	}
}

///////////////////////////////////////////////////////////////////////
//监控模块绑定图元
IMPLEMENT_SERIAL(CLinkYSHBind,CLinkObjectBase,0)

CLinkYSHBind::CLinkYSHBind()
:CLinkObjectBase(lnkYSHBind,"")
{
	memset(m_Ename ,0 ,17) ;
}
CLinkYSHBind::~CLinkYSHBind()
{
}

void CLinkYSHBind::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring()){
		ar.Write((unsigned char *)m_Ename,sizeof(char)*17);
	}
	else{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*17);
	}
}

BOOL CLinkYSHBind::OnEditProperties()
{
	return TRUE ;
}

void CLinkYSHBind::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn){
		file->Write((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
	else{
		file->Read((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
}

void CLinkYSHBind::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Ename,str);
	}
}

///////////////////////////////////////////////////////////////////////
//群控链接图元
IMPLEMENT_SERIAL(CLinkQK,CLinkObjectBase,0)

CLinkQK::CLinkQK()
:CLinkObjectBase(lnkQK,"")
{
	memset(m_Ename ,0 ,17) ;
}
CLinkQK::~CLinkQK()
{
}

void CLinkQK::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_Ename,sizeof(char)*17);
	}
	else
	{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*17);
	}
}

BOOL CLinkQK::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropQkDlg dlg ;
	CProp_Yk_Sec dlgSec;	

	dlg.SetParam(m_Ename ,m_name) ;

	sheet.AddPage((CPropertyPage*)&dlg) ;

	dlgSec.SetData(m_bWatchMan,m_bGroup,m_dwFlag);
	sheet.AddPage((CPropertyPage*)&dlgSec) ;
	if(sheet.DoModal() == IDOK)
	{
		dlg.GetParam(m_Ename ,m_name) ;
		dlgSec.GetData(m_bWatchMan,m_bGroup,m_dwFlag);

		CLinkYkTool::c_bWatchMan=m_bWatchMan;							
		CLinkYkTool::c_bGroup=m_bGroup;								
		CLinkYkTool::c_dwFlag=m_dwFlag;		

		return true ;
	}
	return false ;
}

void CLinkQK::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char*)m_Ename ,sizeof(char)*17) ;

		file->Write((unsigned char*)&m_bWatchMan,sizeof(bool));
		file->Write((unsigned char*)&m_bGroup,sizeof(bool));
		file->Write((unsigned char*)&m_dwFlag,sizeof(DWORD));
	}
	else
	{
		file->Read((unsigned char*)m_Ename ,sizeof(char)*17) ;

		file->Read((unsigned char*)&m_bWatchMan,sizeof(bool));
		file->Read((unsigned char*)&m_bGroup,sizeof(bool));
		file->Read((unsigned char*)&m_dwFlag,sizeof(DWORD));	
	}
}

//////////////////////////////////////////////////////////////////////
//后台检修牌
IMPLEMENT_SERIAL(CLinkSoftRepair,CLinkObjectBase,0)

CLinkSoftRepair::CLinkSoftRepair()
:CLinkObjectBase(lnkSoftRepair,"")
{
	memset(m_Ename ,0 ,17) ;
}
CLinkSoftRepair::~CLinkSoftRepair()
{
}

void CLinkSoftRepair::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_Ename,sizeof(char)*17);	
	}
	else
	{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*17);
	}
}

BOOL CLinkSoftRepair::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropSoftRepair dlg ;
	dlg.SetParam(m_Ename ,m_name) ;
	
	sheet.AddPage((CPropertyPage*)&dlg) ;
	if(sheet.DoModal() == IDOK)
	{		
		dlg.GetParam(m_Ename ,m_name) ;
		return true ;
	}
	return false ;
}

void CLinkSoftRepair::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
	else
	{
		file->Read((unsigned char*)m_Ename ,sizeof(char)*17) ;
	}
}

void CLinkSoftRepair::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Ename,str);
	}

}

////////////////////////////////////////////////////////////////////////////
//单元扩展
IMPLEMENT_SERIAL(CLinkUnitExtend,CLinkObjectBase,0)

CLinkUnitExtend::CLinkUnitExtend()
:CLinkObjectBase(lnkUnitExtend,"")
{
	InitMemory();  
}

CLinkUnitExtend::CLinkUnitExtend(CString strname)
:CLinkObjectBase(lnkUnitExtend,strname)
{
	InitMemory();  
}

void CLinkUnitExtend::InitMemory()
{
	m_nFun=0;
	ZeroMemory(m_Unitname,sizeof(char)*17);
}

CLinkUnitExtend::~CLinkUnitExtend()
{
	InitMemory();
}

void CLinkUnitExtend::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_nFun;
//		ar.Write((unsigned char *)m_Unitname,sizeof(char)*17);
	}
	else
	{
		ar>>m_nFun;
//		ar.Read((unsigned char *)m_Unitname,sizeof(char)*17);
	}
}

BOOL CLinkUnitExtend::OnEditProperties()
{
	CPropertySheet sheet( _T("对象.."));
	CPropUnitExtendDlg dlg;
	bool bSGBW ,bSGBW_E ,bMC,bRefresh,bEpd;
	dlg.SetUnitExtParam(m_name ,m_Unitname ,(bool)(m_nFun&0x1) ,(bool)(m_nFun&2) ,(bool)(m_nFun&4),(bool)(m_nFun&8),(bool)(m_nFun&0x10)) ;
	//dlg.SetUnitExtParam(m_name ,m_Unitname ,(bool)(m_nFun&0x1)) ;
	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		//dlg.GetUnitExtParam(m_name ,m_Unitname ,bSGBW) ;
		dlg.GetUnitExtParam(m_name ,m_Unitname ,bSGBW ,bSGBW_E ,bMC ,bRefresh,bEpd) ;
		m_nFun = (bSGBW?1:0) | ((bSGBW_E?1:0)<<1) | ((bMC?1:0)<<2) | ((bRefresh?1:0)<<3) | ((bEpd?1:0)<<4);
		return TRUE;
	}
	return FALSE;
}

void CLinkUnitExtend::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((int *)&m_nFun,sizeof(m_nFun));
		file->Write((unsigned char *)m_Unitname,sizeof(char)*17);
	}
	else
	{
		file->Read((int *)&m_nFun,sizeof(m_nFun));
		file->Read((unsigned char *)m_Unitname,sizeof(char)*17);
	}
}


void CLinkUnitExtend::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_Unitname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_Unitname,str);
	}
}


//////////////////////////////////////////////////////////////////////
//馈线开关链接
IMPLEMENT_SERIAL(CLinkKX,CLinkObjectBase,0)

CLinkKX::CLinkKX()
:CLinkObjectBase(lnkKX,"")
{
	memset(m_kgEname ,0 ,17) ;
	memset(m_Ename ,0 ,4) ;
	memset(m_Cname ,0 ,13) ;
}

CLinkKX::~CLinkKX()
{
}

void CLinkKX::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_Ename,sizeof(char)*4);
		ar.Write((unsigned char *)m_Cname,sizeof(char)*13);
		ar.Write((unsigned char *)m_kgEname,sizeof(char)*17);		
	}
	else
	{
		ar.Read((unsigned char *)m_Ename,sizeof(char)*4);
		ar.Read((unsigned char *)m_Cname,sizeof(char)*13);
		ar.Read((unsigned char *)m_kgEname,sizeof(char)*17);
	}
}

BOOL CLinkKX::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropKXDlg dlg ;
	sheet.AddPage((CPropertyPage*)&dlg) ;
	dlg.SetParam(m_Ename ,m_kgEname ,m_name) ;

	if(sheet.DoModal() == IDOK)
	{	
		dlg.GetParam(m_Ename ,m_Cname ,m_kgEname ,m_name) ;
		return true ;
	}
	
	return false ;
}

void CLinkKX::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char *)m_Ename,sizeof(char)*4);
		file->Write((unsigned char *)m_Cname,sizeof(char)*13);
		file->Write((unsigned char *)m_kgEname,sizeof(char)*17);
	}
	else
	{
		file->Read((unsigned char *)m_Ename,sizeof(char)*4);
		file->Read((unsigned char *)m_Cname,sizeof(char)*13);
		file->Read((unsigned char *)m_kgEname,sizeof(char)*17);		
	}
}

void CLinkKX::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_kgEname,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_kgEname,str);
	}
}


//////////////////////////////////////////////////////////////////////
//馈线区段链接
IMPLEMENT_SERIAL(CLinkKXSection,CLinkObjectBase,0)

CLinkKXSection::CLinkKXSection()
:CLinkObjectBase(lnkKXSection,"")
{
	m_col1 = 0 ;
	m_col2 = 0 ;
	memset(m_qdEname ,0 ,4) ;
	memset(m_qdCname ,0 ,13) ;
	memset(m_ename ,0 ,4) ;
	memset(m_cname ,0 ,13) ;
	memset(m_bmpName1 ,0 ,33) ;
	memset(m_bmpName2 ,0 ,33) ;
}

CLinkKXSection::~CLinkKXSection()
{
}

void CLinkKXSection::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar.Write((unsigned char *)m_ename,sizeof(char)*4);
		ar.Write((unsigned char *)m_cname,sizeof(char)*13);
		ar.Write((unsigned char *)m_qdEname,sizeof(char)*4);
		ar.Write((unsigned char *)m_qdCname,sizeof(char)*13);
		ar.Write((unsigned char*)m_bmpName1 ,sizeof(char)*33) ;
		ar.Write((unsigned char*)m_bmpName2 ,sizeof(char)*33) ;
		ar.Write((unsigned char*)&m_col1 ,sizeof(COLORREF)) ;
		ar.Write((unsigned char*)&m_col2 ,sizeof(COLORREF)) ;		
	}
	else
	{
		ar.Read((unsigned char *)m_ename,sizeof(char)*4);
		ar.Read((unsigned char *)m_cname,sizeof(char)*13);
		ar.Read((unsigned char *)m_qdEname,sizeof(char)*4);
		ar.Read((unsigned char *)m_qdCname,sizeof(char)*13);	
		ar.Read((unsigned char*)m_bmpName1 ,sizeof(char)*33) ;
		ar.Read((unsigned char*)m_bmpName2 ,sizeof(char)*33) ;
		ar.Read((unsigned char*)&m_col1 ,sizeof(COLORREF)) ;
		ar.Read((unsigned char*)&m_col2 ,sizeof(COLORREF)) ;
	}
}

BOOL CLinkKXSection::OnEditProperties()
{
	CPropertySheet sheet(_T("对象...")) ;
	CPropKXArea dlg ;
	CPropKXAreaTypeDlg dlgType ;

	//dlg.SetParam(m_Ename ,m_name) ;
	
	dlg.SetParam(m_ename ,m_qdEname ,m_name) ;
	
	dlgType.SetParam(m_col1 ,m_col2 ,m_bmpName1 ,m_bmpName2) ;
		
	sheet.AddPage((CPropertyPage*)&dlg) ;
	sheet.AddPage((CPropertyPage*)&dlgType) ;

	if(sheet.DoModal() == IDOK)
	{		
		dlg.GetParam(m_ename ,m_cname ,m_qdEname ,m_qdCname ,m_name) ;
		dlgType.GetParam(&m_col1 ,&m_col2 ,m_bmpName1 ,m_bmpName2) ;
		return true ;
	}
	
	return false ;
}

void CLinkKXSection::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)
	{
		file->Write((unsigned char *)m_ename,sizeof(char)*4);
		file->Write((unsigned char *)m_cname,sizeof(char)*13);
		file->Write((unsigned char *)m_qdEname,sizeof(char)*4);
		file->Write((unsigned char *)m_qdCname,sizeof(char)*13);
		file->Write((unsigned char*)m_bmpName1 ,sizeof(char)*33) ;
		file->Write((unsigned char*)m_bmpName2 ,sizeof(char)*33) ;
		file->Write((unsigned char*)&m_col1 ,sizeof(COLORREF)) ;
		file->Write((unsigned char*)&m_col2 ,sizeof(COLORREF)) ;
			
	}
	else
	{
		file->Read((unsigned char *)m_ename,sizeof(char)*4);
		file->Read((unsigned char *)m_cname,sizeof(char)*13);
		file->Read((unsigned char *)m_qdEname,sizeof(char)*4);
		file->Read((unsigned char *)m_qdCname,sizeof(char)*13);
		file->Read((unsigned char*)m_bmpName1 ,sizeof(char)*33) ;
		file->Read((unsigned char*)m_bmpName2 ,sizeof(char)*33) ;
		file->Read((unsigned char*)&m_col1 ,sizeof(COLORREF)) ;
		file->Read((unsigned char*)&m_col2 ,sizeof(COLORREF)) ;
	}
}

////////////////////////////////////////////////////////////////
//水行业控制链接图元
IMPLEMENT_SERIAL(CLinkDoubleCtrl,CLinkObjectBase,0)

CLinkDoubleCtrl::CLinkDoubleCtrl()
:CLinkObjectBase(lnkDoubleCtrl,"")
{
	InitMemory();  
}

CLinkDoubleCtrl::CLinkDoubleCtrl(CString strname)
:CLinkObjectBase(lnkDoubleCtrl,strname)
{
	InitMemory();  
}

void CLinkDoubleCtrl::InitMemory()
{
	ZeroMemory(m_OprtName1, sizeof(char)*10);
	ZeroMemory(m_OprtName2, sizeof(char)*10);
	ZeroMemory(m_YKName1, sizeof(char)*17);
	ZeroMemory(m_YKName2, sizeof(char)*17);
	m_nActType1 = 0;
	m_nActType2 = 0;
	m_bOnceQuit = false;
}

CLinkDoubleCtrl::~CLinkDoubleCtrl()
{
	InitMemory();
}

void CLinkDoubleCtrl::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar<<m_nActType1;
		ar<<m_nActType2;
		ar<<m_bOnceQuit;
	}
	else
	{
		ar>>m_nActType1;
		ar>>m_nActType2;
		ar>>m_bOnceQuit;
	}
}

BOOL CLinkDoubleCtrl::OnEditProperties()
{
	CPropertySheet sheet( _T("对象..") );
	CPropDoubleCtrl dlg ;
	dlg.SetParam(m_name, m_OprtName1, m_YKName1, m_nActType1, m_OprtName2, m_YKName2, m_nActType2, m_bOnceQuit); 

	sheet.AddPage( (CPropertyPage*)&dlg );
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetParam(m_name, m_OprtName1, m_YKName1, m_nActType1, m_OprtName2, m_YKName2, m_nActType2, m_bOnceQuit); 
		return TRUE;
	}
	return FALSE;
}

void CLinkDoubleCtrl::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)m_OprtName1, sizeof(char)*10);
		file->Write((unsigned char *)m_OprtName2, sizeof(char)*10);
		file->Write((unsigned char *)m_YKName1, sizeof(char)*17);
		file->Write((unsigned char *)m_YKName2, sizeof(char)*17);
		file->Write((int *)&m_nActType1, sizeof(m_nActType1));
		file->Write((int *)&m_nActType2, sizeof(m_nActType2));
		file->Write((bool *)&m_bOnceQuit, sizeof(m_bOnceQuit));
	}
	else
	{
		file->Read((unsigned char *)m_OprtName1, sizeof(char)*10);
		m_OprtName1[10] = '\0';
		file->Read((unsigned char *)m_OprtName2, sizeof(char)*10);
		m_OprtName2[10] = '\0';
		file->Read((unsigned char *)m_YKName1, sizeof(char)*17);
		m_YKName1[17] = '\0';
		file->Read((unsigned char *)m_YKName2, sizeof(char)*17);
		m_YKName2[17] = '\0';
		file->Read((int *)&m_nActType1, sizeof(m_nActType1));
		file->Read((int *)&m_nActType2, sizeof(m_nActType2));
		file->Read((bool *)&m_bOnceQuit, sizeof(m_bOnceQuit));
	}

}

void  CLinkDoubleCtrl::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_YKName1,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YKName1,str);
	}

	ret=CTyBase::GetEname_From_ReplacRule(m_YKName2,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_YKName2,str);
	}
}


