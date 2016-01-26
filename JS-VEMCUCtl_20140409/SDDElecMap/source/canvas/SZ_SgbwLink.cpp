// SZ_SgbwLink.cpp: implementation of the CSZ_SgbwLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SZ_SgbwLink.h"
#include "canvas/SysLinkTy.h"
#include "canvas/TyBase.h"
#include "dialog/PropColor.h"
#include "dialog/SZ_SgbwMainInfoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSZ_SgbwLink,CLinkObjectBase,0)

CSZ_SgbwLink::CSZ_SgbwLink()
:CLinkObjectBase(lnk_SZ_SGBW,"")
{
	strcpy(m_sMainInfo.RtuName,"");
	m_sMainInfo.Zbgt=true;
	m_sMainInfo.HaveUandI=true;
	m_sMainInfo.MaxU=0.00;
	m_sMainInfo.MaxI=0.00;
	m_sMainInfo.MinU=0.00;
	m_sMainInfo.MinI=0.00; 
	int i;
	for(i=0;i<SZ_SGBW_LINENUM;i++)
		strcpy(m_sLineInfo[i].DotName,"");
	m_nLineCount=0;
}

CSZ_SgbwLink::~CSZ_SgbwLink()
{

}

void CSZ_SgbwLink::Save_Ty(CFile *file, BOOL Yn)
{
	int i;
	SZ_S_SGBWLINEBASE LineInfo;

	if (Yn)
	{
		file->Write((unsigned char *)&m_sMainInfo,sizeof(SZ_S_SGBWMAININFO));
		file->Write((unsigned char *)&m_nLineCount,sizeof(m_nLineCount));
		for(i=0;i<m_nLineCount;i++)
		{
			LineInfo=m_sLineInfo[i];
			file->Write((unsigned char *)&LineInfo,sizeof(SZ_S_SGBWLINEBASE));
		}
	}
	else
	{
		file->Read((unsigned char *)&m_sMainInfo,sizeof(SZ_S_SGBWMAININFO));
		file->Read((unsigned char *)&m_nLineCount,sizeof(m_nLineCount));
		for(i=0;i<m_nLineCount;i++)
		{
			file->Read((unsigned char *)&LineInfo,sizeof(SZ_S_SGBWLINEBASE));
			m_sLineInfo[i]=LineInfo;
		}
	}
}

BOOL CSZ_SgbwLink::OnEditProperties()
{
	CSZ_SgbwMainInfoDlg Dlg;

	Dlg.SetInfo(m_sMainInfo,m_sLineInfo,m_nLineCount,m_name);

	if (Dlg.DoModal()==IDOK)
	{
		Dlg.GetInfo(&m_sMainInfo,m_sLineInfo,&m_nLineCount,m_name);

		return TRUE;
	}
	else return FALSE;
}

void CSZ_SgbwLink::Serialize_Ty(CArchive &ar)
{
	int i;
	SZ_S_SGBWLINEBASE LineInfo;

	if (ar.IsStoring())
	{
		ar.Write((unsigned char *)&m_sMainInfo,sizeof(SZ_S_SGBWMAININFO));
		ar.Write((unsigned char *)&m_nLineCount,sizeof(m_nLineCount));
		for(i=0;i<m_nLineCount;i++)
		{
			LineInfo=m_sLineInfo[i];
			ar.Write((unsigned char *)&LineInfo,sizeof(SZ_S_SGBWLINEBASE));
		}
	}
	else
	{
		ar.Read((unsigned char *)&m_sMainInfo,sizeof(SZ_S_SGBWMAININFO));
		ar.Read((unsigned char *)&m_nLineCount,sizeof(m_nLineCount));
		for(i=0;i<m_nLineCount;i++)
		{
			ar.Read((unsigned char *)&LineInfo,sizeof(SZ_S_SGBWLINEBASE));
			m_sLineInfo[i]=LineInfo;
		}
	}
}

CSZ_SgbwLink::CSZ_SgbwLink(CString name)
:CLinkObjectBase(lnk_SZ_SGBW,name)
{
	strcpy(m_sMainInfo.RtuName,"");
	m_sMainInfo.Zbgt=true;
	m_sMainInfo.HaveUandI=true;
	m_sMainInfo.MaxU=0.00;
	m_sMainInfo.MaxI=0.00;
	m_sMainInfo.MinU=0.00;
	m_sMainInfo.MinI=0.00; 
	int i;
	for(i=0;i<SZ_SGBW_LINENUM;i++)
		strcpy(m_sLineInfo[i].DotName,"");
	m_nLineCount=0;
}

void CSZ_SgbwLink::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=CTyBase::GetEname_From_ReplacRule(m_sMainInfo.RtuName,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_sMainInfo.RtuName,str);
	}

	for (int i=0;i<m_nLineCount;i++)
	{
		ret=CTyBase::GetEname_From_ReplacRule(m_sLineInfo[i].DotName,str,ruleList);
		if (ret>0)
		{
			SetModifiedFlag(true);
			strcpy(m_sLineInfo[i].DotName,str);
		}
	}
}
