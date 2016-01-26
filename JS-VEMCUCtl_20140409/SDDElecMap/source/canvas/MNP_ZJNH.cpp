#include "stdafx.h"
#include "canvas/mnp_zjnh.h"
#include "canvas/TyTool.h"
#include "canvas/TyBase.h"
#include "dialog/MNP_ZJNHDlg.h"

IMPLEMENT_SERIAL(CMNP_ZJNH,CLinkObjectBase,0)

CMNP_ZJNH::CMNP_ZJNH(void)
:CLinkObjectBase(lnkMNP_ZJNH,"")
{
	Event1_Enable=FALSE;
	Event2_Enable=FALSE;
	Event3_Enable=FALSE;
	Event4_Enable=FALSE;
	Event5_Enable=FALSE;
	Event6_Enable=FALSE;
	Event7_Enable=FALSE;
	Event8_Enable=FALSE;
	Event9_Enable=FALSE;
	Event10_Enable=FALSE;
	Event11_Enable=FALSE;
	Event12_Enable=FALSE;
	Event13_Enable=FALSE;
	Event14_Enable=FALSE;
	Event15_Enable=FALSE;
	Event16_Enable=FALSE;
	Event17_Enable=FALSE;
}

CMNP_ZJNH::~CMNP_ZJNH(void)
{

}

BOOL CMNP_ZJNH::OnEditProperties()
{
	CMNP_ZJNHDlg dlg;
	dlg.SetEvent1Enable(Event1_Enable);
	dlg.SetEvent2Enable(Event2_Enable);
	dlg.SetEvent3Enable(Event3_Enable);
	dlg.SetEvent4Enable(Event4_Enable);
	dlg.SetEvent5Enable(Event5_Enable);
	dlg.SetEvent6Enable(Event6_Enable);
	dlg.SetEvent7Enable(Event7_Enable);
	dlg.SetEvent8Enable(Event8_Enable);
	dlg.SetEvent9Enable(Event9_Enable);
	dlg.SetEvent10Enable(Event10_Enable);
	dlg.SetEvent11Enable(Event11_Enable);
	dlg.SetEvent12Enable(Event12_Enable);
	dlg.SetEvent13Enable(Event13_Enable);
	dlg.SetEvent14Enable(Event14_Enable);
	dlg.SetEvent15Enable(Event15_Enable);
	dlg.SetEvent16Enable(Event16_Enable);
	dlg.SetEvent17Enable(Event17_Enable);
	
	if(dlg.DoModal()==IDOK)
	{
		strcpy(m_name,"浙江宁海模拟屏参数");
		Event1_Enable=dlg.GetEvent1Enable();
		Event2_Enable=dlg.GetEvent2Enable();
		Event3_Enable=dlg.GetEvent3Enable();
		Event4_Enable=dlg.GetEvent4Enable();
		Event5_Enable=dlg.GetEvent5Enable();
		Event6_Enable=dlg.GetEvent6Enable();
		Event7_Enable=dlg.GetEvent7Enable();
		Event8_Enable=dlg.GetEvent8Enable();
		Event9_Enable=dlg.GetEvent9Enable();
		Event10_Enable=dlg.GetEvent10Enable();
		Event11_Enable=dlg.GetEvent11Enable();
		Event12_Enable=dlg.GetEvent12Enable();
		Event13_Enable=dlg.GetEvent13Enable();
		Event14_Enable=dlg.GetEvent14Enable();
		Event15_Enable=dlg.GetEvent15Enable();
		Event16_Enable=dlg.GetEvent16Enable();
		Event17_Enable=dlg.GetEvent17Enable();		

		return TRUE;
	}

	return FALSE;
}

void CMNP_ZJNH::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)&Event1_Enable,sizeof(Event1_Enable));
		file->Write((unsigned char *)&Event2_Enable,sizeof(Event2_Enable));
		file->Write((unsigned char *)&Event3_Enable,sizeof(Event3_Enable));
		file->Write((unsigned char *)&Event4_Enable,sizeof(Event4_Enable));
		file->Write((unsigned char *)&Event5_Enable,sizeof(Event5_Enable));
		file->Write((unsigned char *)&Event6_Enable,sizeof(Event6_Enable));
		file->Write((unsigned char *)&Event7_Enable,sizeof(Event7_Enable));
		file->Write((unsigned char *)&Event8_Enable,sizeof(Event8_Enable));
		file->Write((unsigned char *)&Event9_Enable,sizeof(Event9_Enable));
		file->Write((unsigned char *)&Event10_Enable,sizeof(Event10_Enable));
		file->Write((unsigned char *)&Event11_Enable,sizeof(Event11_Enable));
		file->Write((unsigned char *)&Event12_Enable,sizeof(Event12_Enable));
		file->Write((unsigned char *)&Event13_Enable,sizeof(Event13_Enable));
		file->Write((unsigned char *)&Event14_Enable,sizeof(Event14_Enable));
		file->Write((unsigned char *)&Event15_Enable,sizeof(Event15_Enable));
		file->Write((unsigned char *)&Event16_Enable,sizeof(Event16_Enable));
		file->Write((unsigned char *)&Event17_Enable,sizeof(Event17_Enable));
	}
	else
	{
		file->Read((unsigned char *)&Event1_Enable,sizeof(Event1_Enable));
		file->Read((unsigned char *)&Event2_Enable,sizeof(Event2_Enable));
		file->Read((unsigned char *)&Event3_Enable,sizeof(Event3_Enable));
		file->Read((unsigned char *)&Event4_Enable,sizeof(Event4_Enable));
		file->Read((unsigned char *)&Event5_Enable,sizeof(Event5_Enable));
		file->Read((unsigned char *)&Event6_Enable,sizeof(Event6_Enable));
		file->Read((unsigned char *)&Event7_Enable,sizeof(Event7_Enable));
		file->Read((unsigned char *)&Event8_Enable,sizeof(Event8_Enable));
		file->Read((unsigned char *)&Event9_Enable,sizeof(Event9_Enable));
		file->Read((unsigned char *)&Event10_Enable,sizeof(Event10_Enable));
		file->Read((unsigned char *)&Event11_Enable,sizeof(Event11_Enable));
		file->Read((unsigned char *)&Event12_Enable,sizeof(Event12_Enable));
		file->Read((unsigned char *)&Event13_Enable,sizeof(Event13_Enable));
		file->Read((unsigned char *)&Event14_Enable,sizeof(Event14_Enable));
		file->Read((unsigned char *)&Event15_Enable,sizeof(Event15_Enable));
		file->Read((unsigned char *)&Event16_Enable,sizeof(Event16_Enable));
		file->Read((unsigned char *)&Event17_Enable,sizeof(Event17_Enable));
	}
}

void CMNP_ZJNH::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)&Event1_Enable,sizeof(Event1_Enable));
		ar.Write((unsigned char *)&Event2_Enable,sizeof(Event2_Enable));
		ar.Write((unsigned char *)&Event3_Enable,sizeof(Event3_Enable));
		ar.Write((unsigned char *)&Event4_Enable,sizeof(Event4_Enable));
		ar.Write((unsigned char *)&Event5_Enable,sizeof(Event5_Enable));
		ar.Write((unsigned char *)&Event6_Enable,sizeof(Event6_Enable));
		ar.Write((unsigned char *)&Event7_Enable,sizeof(Event7_Enable));
		ar.Write((unsigned char *)&Event8_Enable,sizeof(Event8_Enable));
		ar.Write((unsigned char *)&Event9_Enable,sizeof(Event9_Enable));
		ar.Write((unsigned char *)&Event10_Enable,sizeof(Event10_Enable));
		ar.Write((unsigned char *)&Event11_Enable,sizeof(Event11_Enable));
		ar.Write((unsigned char *)&Event12_Enable,sizeof(Event12_Enable));
		ar.Write((unsigned char *)&Event13_Enable,sizeof(Event13_Enable));
		ar.Write((unsigned char *)&Event14_Enable,sizeof(Event14_Enable));
		ar.Write((unsigned char *)&Event15_Enable,sizeof(Event15_Enable));
		ar.Write((unsigned char *)&Event16_Enable,sizeof(Event16_Enable));
		ar.Write((unsigned char *)&Event17_Enable,sizeof(Event17_Enable));
	}
	else
	{
		ar.Read((unsigned char *)&Event1_Enable,sizeof(Event1_Enable));
		ar.Read((unsigned char *)&Event2_Enable,sizeof(Event2_Enable));
		ar.Read((unsigned char *)&Event3_Enable,sizeof(Event3_Enable));
		ar.Read((unsigned char *)&Event4_Enable,sizeof(Event4_Enable));
		ar.Read((unsigned char *)&Event5_Enable,sizeof(Event5_Enable));
		ar.Read((unsigned char *)&Event6_Enable,sizeof(Event6_Enable));
		ar.Read((unsigned char *)&Event7_Enable,sizeof(Event7_Enable));
		ar.Read((unsigned char *)&Event8_Enable,sizeof(Event8_Enable));
		ar.Read((unsigned char *)&Event9_Enable,sizeof(Event9_Enable));
		ar.Read((unsigned char *)&Event10_Enable,sizeof(Event10_Enable));
		ar.Read((unsigned char *)&Event11_Enable,sizeof(Event11_Enable));
		ar.Read((unsigned char *)&Event12_Enable,sizeof(Event12_Enable));
		ar.Read((unsigned char *)&Event13_Enable,sizeof(Event13_Enable));
		ar.Read((unsigned char *)&Event14_Enable,sizeof(Event14_Enable));
		ar.Read((unsigned char *)&Event15_Enable,sizeof(Event15_Enable));
		ar.Read((unsigned char *)&Event16_Enable,sizeof(Event16_Enable));
		ar.Read((unsigned char *)&Event17_Enable,sizeof(Event17_Enable));
	}
}
