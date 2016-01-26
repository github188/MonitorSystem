// TyDateTime_Prop.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/TyDateTime_Prop.h"
#include "dialog/FontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTyDateTime_Prop property page

IMPLEMENT_DYNCREATE(CTyDateTime_Prop, CPropertyPage)

CTyDateTime_Prop::CTyDateTime_Prop() : CPropertyPage(CTyDateTime_Prop::IDD)
{
	//{{AFX_DATA_INIT(CTyDateTime_Prop)
	m_editfh = 0.0f;
	m_editfw = 0.0f;
	m_editjj = 0.0f;
	m_editx0 = 0.0f;
	m_edity0 = 0.0f;
	m_radio = -1;
	//}}AFX_DATA_INIT
}

CTyDateTime_Prop::~CTyDateTime_Prop()
{
}

void CTyDateTime_Prop::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTyDateTime_Prop)
	DDX_Control(pDX, IDC_STATIC1, m_static);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, CZ_IDC_EDITFH, m_editfh);
	DDV_MinMaxFloat(pDX, m_editfh, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITFW, m_editfw);
	DDV_MinMaxFloat(pDX, m_editfw, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITJJ, m_editjj);
	DDV_MinMaxFloat(pDX, m_editjj, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITX0, m_editx0);
	DDV_MinMaxFloat(pDX, m_editx0, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITY0, m_edity0);
	DDV_MinMaxFloat(pDX, m_edity0, 0.f, 32000.f);
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTyDateTime_Prop, CPropertyPage)
	//{{AFX_MSG_MAP(CTyDateTime_Prop)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FONT, OnIdcButtonFont)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTyDateTime_Prop message handlers

void CTyDateTime_Prop::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_list.ResetContent();
	int index;
	int i;
	for (i=1;i<=4;i++)
	{
		index=m_list.AddString(Trans(i,0,0));
		m_list.SetItemData(index,i<<8);
	}
	m_list.SetCurSel(0);
	OnSelchangeList1();
}

void CTyDateTime_Prop::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_list.ResetContent();
	int index;
	int i;
	for (i=1;i<=7;i++)
	{
		index=m_list.AddString(Trans(0,i,0));
		m_list.SetItemData(index,i);
	}
	m_list.SetCurSel(0);
	OnSelchangeList1();
}

void CTyDateTime_Prop::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_list.ResetContent();
	int i,j;
	int index;
	for (i=1;i<=2;i++)
	{
		for (j=1;j<=5;j++)
		{
			index=m_list.AddString(Trans(i,j,0));
			m_list.SetItemData(index,(i<<8)|j);
		}
	}
	for (i=3;i<=4;i++)
	{
		for (j=6;j<=7;j++)
		{
			index=m_list.AddString(Trans(i,j,0));
			m_list.SetItemData(index,(i<<8)|j);
		}
	}
	m_list.SetCurSel(0);
	OnSelchangeList1();
	
}

void CTyDateTime_Prop::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	m_list.ResetContent();
	int i,j;
	int index;
	for (i=1;i<=2;i++)
	{
		for (j=1;j<=5;j++)
		{
			index=m_list.AddString(Trans(i,j,1));
			m_list.SetItemData(index,(i<<8)|j);
		}
	}
	
	for (i=3;i<=4;i++)
	{
		for (j=6;j<=7;j++)
		{
			index=m_list.AddString(Trans(i,j,1));
			m_list.SetItemData(index,(i<<8)|j);
		}

	}
	m_list.SetCurSel(0);
	OnSelchangeList1();
}

CString CTyDateTime_Prop::Trans(BYTE timekind, BYTE datekind,BYTE diskind)
{
	CString datestr,timestr,disstr;
	datestr="";
	timestr="";
	if ((datekind==0)&&(timekind==0)) timekind=1;
	if (datekind!=0)
	{
		if (datekind>7) datekind=0;
		if (datekind==1) datestr.Format("%04d-%02d-%02d",wYear,bMonth,bDay);//2002-3-22
		if (datekind==2) datestr.Format("%04d/%02d/%02d",wYear,bMonth,bDay);//2002/3/22
		if (datekind==3) datestr.Format("%02d/%02d/%04d",bMonth,bDay,wYear);//3/22/2002
		if (datekind==4) datestr.Format("%02d.%02d.%04d",bMonth,bDay,wYear);//3.22.2002
		if (datekind==5) datestr.Format("%02d-%02d-%02d",wYear-2000,bMonth,bDay);//02.3.22
		if (datekind==6) datestr.Format("%04d年%02d月",wYear,bMonth);//2002年3月
		if (datekind==7) datestr.Format("%04d年%02d月%02d日",wYear,bMonth,bDay);//2002年3月22日
	}
	if (timekind!=0)
	{
		if (timekind>4) timekind=0;
		if (timekind==1) timestr.Format("%02d:%02d:%02d",bHour,bMin,bSec);//16:13:26
		if (timekind==2) timestr.Format("%02d:%02d",bHour,bMin);//16:13
		if (timekind==3) timestr.Format("%02d时%02d分%02d秒",bHour,bMin,bSec);//16时13分26秒
		if (timekind==4) timestr.Format("%02d时%02d分",bHour,bMin);//16时13分
	}
	if (diskind==1)
	{
		disstr="";
		disstr=datestr;
		if (disstr.GetLength()!=0)
		{
			disstr=disstr+" ";
		}
		disstr=disstr+timestr;
	}
	else
	{
		disstr="";
		disstr=timestr;
		if (disstr.GetLength()!=0)
		{
			disstr=disstr+" ";
		}
		disstr=disstr+datestr;
	}
	return disstr;
}

void CTyDateTime_Prop::SetPara(COLORREF itcolor,LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj,BYTE timekind,BYTE datekind,BYTE diskind,
							   WORD year,BYTE month,BYTE day,BYTE hour,BYTE min,BYTE sec,WORD msec)
{
	m_color=itcolor;
	lgfont=ff;
	rectx0=x0;
	recty0=y0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;

	wYear=year;
	bMonth=month;
	bDay=day;
	bHour=hour;
	bMin=min;
	bSec=sec;
	wMSec=msec;

	m_timekind=timekind;
	m_datekind=datekind;
	m_diskind=diskind;
}

void CTyDateTime_Prop::GetPara(COLORREF *itcolor,LOGFONT *ff,float *x0,float* y0,float *fontw,float *fonth,float *fontj,BYTE* timekind,BYTE *datekind,BYTE *diskind)
{
	*itcolor=m_color;
	*ff=lgfont;
	*x0=rectx0;
	*y0=recty0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;

	*timekind=m_timekind;
	*datekind=m_datekind;
	*diskind=m_diskind;
}

BOOL CTyDateTime_Prop::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editx0=rectx0;
	m_edity0=recty0;
	m_editfw=fontwidth;
	m_editfh=fontheight;
	m_editjj=fontjj;

	HWND hwnd;
	GetDlgItem(CZ_IDC_STATIC_C,&hwnd);
	m_normalc.SubclassWindow(hwnd);
	m_normalc.SetColor(m_color);
	
	m_radio=0;
	if ((m_timekind!=0)&&(m_datekind!=0))
	{
		if (m_diskind==0) m_radio=2;
		if (m_diskind==1) m_radio=3;
	}
	else
	{
		if (m_timekind!=0)
		{
			m_radio=0;
		}
		else
		{
			m_radio=1;
		}
	}
	UpdateData(false);
	if (m_radio==0) OnRadio1();
	if (m_radio==1) OnRadio4();
	if (m_radio==2) OnRadio5();
	if (m_radio==3) OnRadio6();
	for (int i=0;i<m_list.GetCount();i++)
	{
		if (m_list.GetItemData(i)==((m_timekind<<8)|m_datekind))
		{
			m_list.SetCurSel(i);
		}
	}
	OnSelchangeList1();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTyDateTime_Prop::OnIdcButtonFont() 
{
	// TODO: Add your control notification handler code here
 	CPropertySheet sheet("选择字体");
 	CFontDlg dlg;
 	dlg.SetFontDlg(&lgfont,false,false,0x000000);
 	sheet.AddPage(&dlg);
 	if (sheet.DoModal()==IDOK)
	{
		dlg.GetCurLogfont(&lgfont);
	}
	
}

void CTyDateTime_Prop::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	CString tmp;
	m_list.GetText(m_list.GetCurSel(),tmp);
	m_static.SetWindowText("所选："+tmp);
}

void CTyDateTime_Prop::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);

    m_normalc.GetColor(&m_color);
	
	rectx0=m_editx0;
	recty0=m_edity0;
	fontwidth=m_editfw;
	fontheight=m_editfh;
	fontjj=m_editjj;

	m_diskind=0;
	if (m_radio==3)
	{
		m_diskind=1;
	}
	m_timekind=(BYTE)(m_list.GetItemData(m_list.GetCurSel())>>8);
	m_datekind=(BYTE)(m_list.GetItemData(m_list.GetCurSel())&0xff);
	
	CPropertyPage::OnOK();
}
