#include "stdafx.h"
#include "canvas/mnp_shxg.h"
#include "canvas/TyTool.h"
#include "canvas/TyBase.h"
#include "dialog/MNP_SHXGDlg.h"

IMPLEMENT_SERIAL(CMNP_SHXG,CLinkObjectBase,0)

CMNP_SHXG::CMNP_SHXG(void)
:CLinkObjectBase(lnkMNP_SHXG,"")
{
	Node=0;
	Line=0;
	Unit=0;
}

CMNP_SHXG::~CMNP_SHXG(void)
{

}

BOOL CMNP_SHXG::OnEditProperties()
{
	CMNP_SHXGDlg dlg;
	dlg.SetNode(Node);
	dlg.SetLine(Line);
	dlg.SetUnit(Unit);
	if(dlg.DoModal()==IDOK)
	{
		strcpy(m_name,"上海新光模拟屏清屏事件参数");
		Node=dlg.GetNode();
		Line=dlg.GetLine();
		Unit=dlg.GetUnit();
		return TRUE;
	}
	return FALSE;
}

void CMNP_SHXG::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&Node,sizeof(Node));
		file->Write((unsigned char *)&Line,sizeof(Line));
		file->Write((unsigned char *)&Unit,sizeof(Unit));
	}
	else
	{
		file->Read((unsigned char *)&Node,sizeof(Node));
		file->Read((unsigned char *)&Line,sizeof(Line));
		file->Read((unsigned char *)&Unit,sizeof(Unit));
	}
}

void CMNP_SHXG::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)&Node,sizeof(Node));
		ar.Write((unsigned char *)&Line,sizeof(Line));
		ar.Write((unsigned char *)&Unit,sizeof(Unit));
	}
	else
	{
		ar.Read((unsigned char *)&Node,sizeof(Node));
		ar.Read((unsigned char *)&Line,sizeof(Line));
		ar.Read((unsigned char *)&Unit,sizeof(Unit));
	}
}
