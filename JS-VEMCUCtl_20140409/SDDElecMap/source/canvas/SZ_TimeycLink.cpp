#include "stdafx.h"
//#include "Sp_draw.h"
#include "canvas/sz_timeyclink.h"
#include "dialog/SZ_TimeycDlg.h"
#include "canvas/SysLinkTy.h"
#include "canvas/TyBase.h"

IMPLEMENT_SERIAL(CSZ_TimeycLink,CLinkObjectBase,0)

CSZ_TimeycLink::CSZ_TimeycLink(void)
:CLinkObjectBase(lnk_SZ_TIMEYC,"")
{
}

CSZ_TimeycLink::~CSZ_TimeycLink(void)
{
}

CSZ_TimeycLink::CSZ_TimeycLink(CString name)
:CLinkObjectBase(lnk_SZ_TIMEYC,name)
{
}

BOOL CSZ_TimeycLink::OnEditProperties()
{
	CSZ_TimeycDlg Dlg;

	Dlg.SetData(m_name);
	if (Dlg.DoModal()==IDOK)
	{
		Dlg.GetData(m_name);
		return TRUE;
	}
	else return FALSE;
}
