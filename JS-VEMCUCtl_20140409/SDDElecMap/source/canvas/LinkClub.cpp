// LinkClub.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/LinkClub.h"
#include "dialog/LinkClub_Prop1.h"
#include "dialog/LinkClub_Prop2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkClub
IMPLEMENT_SERIAL(CLinkClub,CLinkObjectBase,0)

CLinkClub::CLinkClub()
:CLinkObjectBase(lnkClub,"")
{
	sprintf(m_name,"未定义棒图");

	barcount=0;

	LOGFONT initfont;
	initfont.lfHeight=15;
	initfont.lfWidth=5;
	initfont.lfEscapement=0;//旋转角度
	initfont.lfOrientation=0;
	initfont.lfWeight=FW_MEDIUM;
	initfont.lfItalic=false;
//		initfont.lfWeight=700;
//		initfont.lfItalic=true;
	initfont.lfUnderline=false;
	initfont.lfStrikeOut=false;//强调线
	initfont.lfCharSet=GB2312_CHARSET;
	initfont.lfOutPrecision=OUT_DEFAULT_PRECIS;//输出精度
	initfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;//剪裁精度
	initfont.lfQuality=DEFAULT_QUALITY;
	initfont.lfPitchAndFamily=DEFAULT_PITCH;//间距和字体族
	sprintf(initfont.lfFaceName,"宋体");

	sprintf(clubinfo.title,"未知名称");
	clubinfo.titlefont=initfont;
	clubinfo.titlecolor=0xff0000;
	sprintf(clubinfo.yunit,"未知单位");
	clubinfo.ymax=100;//y轴最大值
	clubinfo.ymin=0;//y轴最小值
	clubinfo.yunitfont=initfont;//y轴单位字体
	clubinfo.yunitcolor=0xff0000;//Y轴单位颜色
	clubinfo.coordinatefont=initfont;//坐标系字体
	clubinfo.coordinatecolor=0xff0000;//坐标系颜色
	clubinfo.backcolor=0x000000;//背景色
	clubinfo.allmaxmin=0;//统一越限值0 not 1 yes
	clubinfo.maxvalue=0;//统一的最大值
	clubinfo.minvalue=0;//统一的最小值
	clubinfo.clubwidth=25;//棒宽度
	clubinfo.clubspace=10;//棒间距
}

CLinkClub::~CLinkClub()
{
}


BOOL CLinkClub::OnEditProperties()
{
	CPropertySheet sheet( _T("棒图对象..") );
	CLinkClub_Prop1 dlg;
	CLinkClub_Prop2 dlg2;
 	dlg.SetPara(m_name,clubinfo);
	dlg2.SetPara(barcount,barinfo);
	sheet.AddPage( (CPropertyPage*)&dlg );
	sheet.AddPage( (CPropertyPage*)&dlg2);
	if (sheet.DoModal() == IDOK)
	{
		dlg.GetPara(m_name,&clubinfo);
    	dlg2.GetPara(&barcount,(STRUCT_SINGLEBAR *)&barinfo);
		return TRUE;
	}
    return FALSE;
}

void CLinkClub::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		file->Write((unsigned char *)clubinfo.title,sizeof(char)*33);
		file->Write((unsigned char *)&clubinfo.titlefont,sizeof(LOGFONT));
		file->Write((unsigned char *)&clubinfo.titlecolor,sizeof(COLORREF));
		file->Write((unsigned char *)clubinfo.yunit,sizeof(char)*9);
		file->Write((unsigned char *)&clubinfo.ymax,sizeof(float));
		file->Write((unsigned char *)&clubinfo.ymin,sizeof(float));
		file->Write((unsigned char *)&clubinfo.yunitfont,sizeof(LOGFONT));
		file->Write((unsigned char *)&clubinfo.yunitcolor,sizeof(COLORREF));
		file->Write((unsigned char *)&clubinfo.coordinatefont,sizeof(LOGFONT));
		file->Write((unsigned char *)&clubinfo.coordinatecolor,sizeof(COLORREF));
		file->Write((unsigned char *)&clubinfo.backcolor,sizeof(COLORREF));
		file->Write((unsigned char *)&clubinfo.allmaxmin,sizeof(BYTE));
		file->Write((unsigned char *)&clubinfo.maxvalue,sizeof(float));
		file->Write((unsigned char *)&clubinfo.minvalue,sizeof(float));
		file->Write((unsigned char *)&clubinfo.clubwidth,sizeof(DWORD));
		file->Write((unsigned char *)&clubinfo.clubspace,sizeof(DWORD));

		file->Write((unsigned char *)&barcount,sizeof(BYTE));
		for (int i=0;i<barcount;i++)
		{
			file->Write((unsigned char *)barinfo[i].ename,sizeof(BYTE)*17);
			file->Write((unsigned char *)barinfo[i].cname,sizeof(BYTE)*9);
		}
	}
	else
	{
		file->Read((unsigned char *)clubinfo.title,sizeof(char)*33);
		file->Read((unsigned char *)&clubinfo.titlefont,sizeof(LOGFONT));
		file->Read((unsigned char *)&clubinfo.titlecolor,sizeof(COLORREF));
		file->Read((unsigned char *)clubinfo.yunit,sizeof(char)*9);
		file->Read((unsigned char *)&clubinfo.ymax,sizeof(float));
		file->Read((unsigned char *)&clubinfo.ymin,sizeof(float));
		file->Read((unsigned char *)&clubinfo.yunitfont,sizeof(LOGFONT));
		file->Read((unsigned char *)&clubinfo.yunitcolor,sizeof(COLORREF));
		file->Read((unsigned char *)&clubinfo.coordinatefont,sizeof(LOGFONT));
		file->Read((unsigned char *)&clubinfo.coordinatecolor,sizeof(COLORREF));
		file->Read((unsigned char *)&clubinfo.backcolor,sizeof(COLORREF));
		file->Read((unsigned char *)&clubinfo.allmaxmin,sizeof(BYTE));
		file->Read((unsigned char *)&clubinfo.maxvalue,sizeof(float));
		file->Read((unsigned char *)&clubinfo.minvalue,sizeof(float));
		file->Read((unsigned char *)&clubinfo.clubwidth,sizeof(DWORD));
		file->Read((unsigned char *)&clubinfo.clubspace,sizeof(DWORD));

		file->Read((unsigned char *)&barcount,sizeof(BYTE));
		for (int i=0;i<barcount;i++)
		{
			file->Read((unsigned char *)barinfo[i].ename,sizeof(BYTE)*17);
			file->Read((unsigned char *)barinfo[i].cname,sizeof(BYTE)*9);
		}
	}
}

void CLinkClub::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)clubinfo.title,sizeof(char)*33);
		ar.Write((unsigned char *)&clubinfo.titlefont,sizeof(LOGFONT));
		ar.Write((unsigned char *)&clubinfo.titlecolor,sizeof(COLORREF));
		ar.Write((unsigned char *)clubinfo.yunit,sizeof(char)*9);
		ar.Write((unsigned char *)&clubinfo.ymax,sizeof(float));
		ar.Write((unsigned char *)&clubinfo.ymin,sizeof(float));
		ar.Write((unsigned char *)&clubinfo.yunitfont,sizeof(LOGFONT));
		ar.Write((unsigned char *)&clubinfo.yunitcolor,sizeof(COLORREF));
		ar.Write((unsigned char *)&clubinfo.coordinatefont,sizeof(LOGFONT));
		ar.Write((unsigned char *)&clubinfo.coordinatecolor,sizeof(COLORREF));
		ar.Write((unsigned char *)&clubinfo.backcolor,sizeof(COLORREF));
		ar.Write((unsigned char *)&clubinfo.allmaxmin,sizeof(BYTE));
		ar.Write((unsigned char *)&clubinfo.maxvalue,sizeof(float));
		ar.Write((unsigned char *)&clubinfo.minvalue,sizeof(float));
		ar.Write((unsigned char *)&clubinfo.clubwidth,sizeof(DWORD));
		ar.Write((unsigned char *)&clubinfo.clubspace,sizeof(DWORD));

		ar.Write((unsigned char *)&barcount,sizeof(BYTE));
		for (int i=0;i<barcount;i++)
		{
			ar.Write((unsigned char *)&barinfo[i].ename,sizeof(BYTE)*17);
			ar.Write((unsigned char *)&barinfo[i].cname,sizeof(BYTE)*9);
		}
	}
	else
	{
		ar.Read((unsigned char *)clubinfo.title,sizeof(char)*33);
		ar.Read((unsigned char *)&clubinfo.titlefont,sizeof(LOGFONT));
		ar.Read((unsigned char *)&clubinfo.titlecolor,sizeof(COLORREF));
		ar.Read((unsigned char *)clubinfo.yunit,sizeof(char)*9);
		ar.Read((unsigned char *)&clubinfo.ymax,sizeof(float));
		ar.Read((unsigned char *)&clubinfo.ymin,sizeof(float));
		ar.Read((unsigned char *)&clubinfo.yunitfont,sizeof(LOGFONT));
		ar.Read((unsigned char *)&clubinfo.yunitcolor,sizeof(COLORREF));
		ar.Read((unsigned char *)&clubinfo.coordinatefont,sizeof(LOGFONT));
		ar.Read((unsigned char *)&clubinfo.coordinatecolor,sizeof(COLORREF));
		ar.Read((unsigned char *)&clubinfo.backcolor,sizeof(COLORREF));
		ar.Read((unsigned char *)&clubinfo.allmaxmin,sizeof(BYTE));
		ar.Read((unsigned char *)&clubinfo.maxvalue,sizeof(float));
		ar.Read((unsigned char *)&clubinfo.minvalue,sizeof(float));
		ar.Read((unsigned char *)&clubinfo.clubwidth,sizeof(DWORD));
		ar.Read((unsigned char *)&clubinfo.clubspace,sizeof(DWORD));

		ar.Read((unsigned char *)&barcount,sizeof(BYTE));
		for (int i=0;i<barcount;i++)
		{
			ar.Read((unsigned char *)&barinfo[i].ename,sizeof(BYTE)*17);
			ar.Read((unsigned char *)&barinfo[i].cname,sizeof(BYTE)*9);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CLinkClub message handlers

//DEL void CLinkClub::On_Show_Club(tagInterface_All &allInt)
//DEL {
//DEL 
//DEL }
