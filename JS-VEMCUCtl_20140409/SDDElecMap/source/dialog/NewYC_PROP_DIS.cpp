// NewYC_PROP_DIS.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/NewYC_PROP_DIS.h"
#include "dialog/FontDlg.h"



// CNewYC_PROP_DIS 对话框

IMPLEMENT_DYNAMIC(CNewYC_PROP_DIS, CPropertyPage)
CNewYC_PROP_DIS::CNewYC_PROP_DIS(BYTE kind)
	: CPropertyPage(CNewYC_PROP_DIS::IDD)
	, m_editx0(0)
	, m_edity0(0)
	, m_editfh(0)
	, m_editfw(0)
	, m_editjj(0)
{
	itkind=kind;
}

CNewYC_PROP_DIS::~CNewYC_PROP_DIS()
{

}

void CNewYC_PROP_DIS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_YC_X0, m_editx0);
	DDV_MinMaxFloat(pDX, m_editx0, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_Y0, m_edity0);
	DDV_MinMaxFloat(pDX, m_edity0, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_FH, m_editfh);
	DDV_MinMaxFloat(pDX, m_editfh, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_FW, m_editfw);
	DDV_MinMaxFloat(pDX, m_editfw, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_JJ, m_editjj);
	DDV_MinMaxFloat(pDX, m_editjj, 0.f, 32000.f);
}


BEGIN_MESSAGE_MAP(CNewYC_PROP_DIS, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_YC_FONT, OnBnClickedButtonYcFont)
END_MESSAGE_MAP()


// CNewYC_PROP_DIS 消息处理程序

void CNewYC_PROP_DIS::SetPara(COLORREF disablec,COLORREF normalc,COLORREF leveluc,COLORREF leveluuc,
		COLORREF leveldc,COLORREF levelddc ,LOGFONT ff,float x0,float y0,float fontw,
			float fonth,float fontj)
{
	disablecolor=disablec;
	normalcolor=normalc;
	leveldowncolor=leveldc;
	levelupcolor=leveluc;
	leveluupcolor=leveluuc;
	levelddowncolor=levelddc;

	lgfont=ff;
	rectx0=x0;
	recty0=y0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;
}

void CNewYC_PROP_DIS::SetPara2(COLORREF disablec,COLORREF normalc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj)
{
	disablecolor=disablec;
	normalcolor=normalc;
	lgfont=ff;
	rectx0=x0;
	recty0=y0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;
}

void CNewYC_PROP_DIS::GetPara(COLORREF *disablec,COLORREF *normalc,COLORREF *leveluc,COLORREF *leveluuc,
		COLORREF *leveldc,COLORREF *levelddc ,LOGFONT *ff,float *x0,float *y0,float *fontw,
			float *fonth,float *fontj)
{
	//UpdateData(true);
	*disablec=disablecolor;
	*normalc=normalcolor;
	*leveldc=leveldowncolor;
	*leveluc=levelupcolor;
	*leveluuc=leveluupcolor;
	*levelddc=levelddowncolor;

	*ff=lgfont;
	*x0=rectx0;
	*y0=recty0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;
}

void CNewYC_PROP_DIS::GetPara2(COLORREF *disablec,COLORREF *normalc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj)
{
	//UpdateData(true);
	*disablec=disablecolor;
	*normalc=normalcolor;
	*ff=lgfont;
	*x0=rectx0;
	*y0=recty0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;
}

void CNewYC_PROP_DIS::OnBnClickedButtonYcFont()
{
	// TODO: 在此添加控件通知处理程序代码
	CPropertySheet sheet("选择字体");
 	CFontDlg dlg;
 	dlg.SetFontDlg(&lgfont,false,false,0x000000);
 	sheet.AddPage(&dlg);
 	if (sheet.DoModal()==IDOK)
	{
		dlg.GetCurLogfont(&lgfont);
	}
}

void CNewYC_PROP_DIS::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	
	m_upc.GetColor(&levelupcolor) ;
	m_uupc.GetColor(&leveluupcolor);

	m_downc.GetColor(&leveldowncolor) ;
	m_ddownc.GetColor(&levelddowncolor);

    m_normalc.GetColor(&normalcolor);
    m_disablec.GetColor(&disablecolor);
	
	rectx0=m_editx0;
	recty0=m_edity0;
	fontwidth=m_editfw;
	fontheight=m_editfh;
	fontjj=m_editjj;


	CPropertyPage::OnOK();
}

BOOL CNewYC_PROP_DIS::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editx0=rectx0;
	m_edity0=recty0;
	m_editfw=fontwidth;
	m_editfh=fontheight;
	m_editjj=fontjj;

	HWND hwnd;
	GetDlgItem(IDC_STATIC_YC_C1,&hwnd);
	m_normalc.SubclassWindow(hwnd);
	m_normalc.SetColor(normalcolor);
	
	GetDlgItem(IDC_STATIC_YC_C2,&hwnd);
	m_disablec.SubclassWindow(hwnd);
	m_disablec.SetColor(disablecolor);
	
	GetDlgItem(IDC_STATIC_YC_C3,&hwnd);
	m_upc.SubclassWindow(hwnd);
	m_upc.SetColor(levelupcolor);
	
	GetDlgItem(IDC_STATIC_YC_C4,&hwnd);
	m_downc.SubclassWindow(hwnd);
	m_downc.SetColor(leveldowncolor);

	//上上限
	GetDlgItem(IDC_STATIC_YC_C5,&hwnd);
	m_uupc.SubclassWindow(hwnd);
	m_uupc.SetColor(leveluupcolor);

	//下下限
	GetDlgItem(IDC_STATIC_YC_C6,&hwnd);
	m_ddownc.SubclassWindow(hwnd);
	m_ddownc.SetColor(levelddowncolor);
	
	//YC_IDC_DISUPSTATIC
	if (itkind==2)
	{
		
		m_upc.ShowWindow(SW_HIDE);
		m_uupc.ShowWindow(SW_HIDE);
				
		m_downc.ShowWindow(SW_HIDE);
		m_ddownc.ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_YC_DISC3,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_YC_DISC4 ,&hwnd) ;
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_YC_DISC5,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_YC_DISC6,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	}
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
