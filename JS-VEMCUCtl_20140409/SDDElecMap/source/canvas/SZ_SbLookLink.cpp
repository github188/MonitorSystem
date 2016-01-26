#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/sz_sblooklink.h"
#include "canvas/SysLinkTy.h"
#include "canvas/TyBase.h"
#include "dialog/SZ_SbLookDlg.h"

IMPLEMENT_SERIAL(CSZ_SbLookLink,CLinkObjectBase,0)

CSZ_SbLookLink::CSZ_SbLookLink(void)
:CLinkObjectBase(lnk_SZ_SBLOOK,"")
{
	ZeroMemory(m_sbbh,sizeof(char)*17);
}

CSZ_SbLookLink::~CSZ_SbLookLink(void)
{
}

CSZ_SbLookLink::CSZ_SbLookLink(CString strname)
:CLinkObjectBase(lnk_SZ_SBLOOK,strname)
{
	ZeroMemory(m_sbbh,sizeof(char)*17);
}

void CSZ_SbLookLink::Save_Ty(CFile* file, BOOL Yn)
{
	if (Yn)
		file->Write((unsigned char *)m_sbbh,sizeof(char)*17);
	else
		file->Read((unsigned char *)m_sbbh,sizeof(char)*17);
}

void CSZ_SbLookLink::Serialize_Ty(CArchive& ar)
{
	if (ar.IsStoring())
		ar.Write((unsigned char *)m_sbbh,sizeof(char)*17);
	else
		ar.Read((unsigned char *)m_sbbh,sizeof(char)*17);
}

BOOL CSZ_SbLookLink::OnEditProperties(void)
{
	CSZ_SbLookDlg Dlg;

	Dlg.SetInfo(m_name,m_sbbh);

	if (Dlg.DoModal()==IDOK)
	{
		Dlg.GetInfo(m_name,m_sbbh);

		return TRUE;
	}
	else return FALSE;
}

void CSZ_SbLookLink::ParamReplace_Ty(CStringList& ruleList)
{

}
