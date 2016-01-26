// NewYC_PROP_INFO.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/NewYC_PROP_INFO.h"
#include "canvas/yc.h"

// CNewYC_PROP_INFO 对话框

IMPLEMENT_DYNAMIC(CNewYC_PROP_INFO, CPropertyPage)
CNewYC_PROP_INFO::CNewYC_PROP_INFO(BYTE kind)
	: CPropertyPage(CNewYC_PROP_INFO::IDD)
	, m_ename(_T(""))
	, m_unit(_T(""))
	, m_digitlength(0)
	, m_hint(_T(""))
	, m_edithintdis(0)
	, m_editunitdis(0)
	, m_fUUp(0)
	, m_fDDown(0)
{
	itkind=kind;
}

CNewYC_PROP_INFO::~CNewYC_PROP_INFO()
{

}

void CNewYC_PROP_INFO::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_YC_ENAME, m_ename);
	DDV_MaxChars(pDX, m_ename, 16);
	DDX_CBString(pDX, IDC_COMBO_YC_UINT, m_unit);
	DDV_MaxChars(pDX, m_unit, 16);
	DDX_Text(pDX, IDC_EDIT_YC_DIGITLENGTH, m_digitlength);
	DDV_MinMaxInt(pDX, m_digitlength, 4, 10);
	DDX_Text(pDX, IDC_EDIT_YC_HINT, m_hint);
	DDV_MaxChars(pDX, m_hint, 32);
	DDX_Text(pDX, IDC_EDIT_YC_HINTDIS, m_edithintdis);
	DDV_MinMaxFloat(pDX, m_edithintdis, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_UNITDIS, m_editunitdis);
	DDV_MinMaxFloat(pDX, m_editunitdis, 0.f, 32000.f);
	DDX_Text(pDX, IDC_EDIT_YC_UUP, m_fUUp);
	DDV_MinMaxFloat(pDX,m_fUUp,0.f ,32766.f) ;
	DDX_Text(pDX, IDC_EDIT_YC_DDOWN, m_fDDown);
	DDV_MinMaxFloat(pDX,m_fDDown,0.f ,32766.f) ;
}


BEGIN_MESSAGE_MAP(CNewYC_PROP_INFO, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CNewYC_PROP_INFO 消息处理程序

void CNewYC_PROP_INFO::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	char dllf[256],s[256],myename[17],mycname[33];
 	HINSTANCE hmydll;
 	sprintf(dllf,YC_FINDDOTDLLNAME);
 	hmydll=LoadLibrary(dllf);
 	if (hmydll==NULL){
 		sprintf(s,"%s 调入失败！",dllf);
 		AfxMessageBox(s);
 		return;
 	}
 	ShowFindDotDlg * pf;
 	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
 	if (pf==NULL){
 		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
 		AfxMessageBox(s);
 		FreeLibrary(hmydll);
 		hmydll=NULL;
 		return;
 	}
 	CWnd * pp=this->GetParent();
	BYTE dotkind=0;
	if (itkind==1)
	{
		dotkind=1;
	}
	if (itkind==2)
	{
		dotkind=3;
	}
 	if ((*pf)(dotkind,myename,mycname))
 	{
 		UpdateData(true);
 		m_ename=myename;
 		UpdateData(false);
 	}
 
 	FreeLibrary(hmydll);
}

void CNewYC_PROP_INFO::SetPara(char * ename,int digitlength,char * hint,char * unit,
		COLORREF unitc,COLORREF hintc,BYTE hintena,float hintj,float unitj,float UUp ,
			float DDown)
{
	char tempename[17],temphint[33],tempunit[17];
    memcpy(tempename,ename,sizeof(char)*17);
    memcpy(temphint,hint,sizeof(char)*33);
    memcpy(tempunit,unit,sizeof(char)*17);
	tempename[16]='\0';
	temphint[32]='\0';
	tempunit[16]='\0';
	m_ename=tempename;
	m_digitlength=digitlength;
	m_hint=temphint;
	m_unit=tempunit;
	unitcolor=unitc;
	hintcolor=hintc;
	hintenable=hintena;
	m_edithintdis=hintj;
	m_editunitdis=unitj;
	//各限值
	
	m_fUUp = UUp ;

	m_fDDown = DDown ;

}

void CNewYC_PROP_INFO::GetPara(char * ename,int *digitlength,char * hint,char * unit,
		COLORREF *unitc,COLORREF *hintc,BYTE * hintena,float *hintjj,float *unitjj,
			float* UUp ,float* DDown)
{
	//UpdateData(true);
    memcpy(ename,m_ename,sizeof(char)*17);
	ename[16]='\0';
	*digitlength=m_digitlength;
	char s1[33],s2[17];
	sprintf(s1,"%s",m_hint);
	s1[32]='\0';
	memcpy(hint,s1,sizeof(char)*33);
	sprintf(s2,"%s",m_unit);
	s2[16]='\0';
	memcpy(unit,s2,sizeof(char)*17);
	*unitc=unitcolor;
	*hintc=hintcolor;
	*hintena=hintenable;
	*hintjj=m_edithintdis;
	*unitjj=m_editunitdis;
	
	//各限值
	
	*UUp = m_fUUp ;
	
	*DDown =m_fDDown ;
}

BOOL CNewYC_PROP_INFO::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HWND hwnd;

	GetDlgItem(IDC_STATIC_YC_C5,&hwnd);
	m_hintc.SubclassWindow(hwnd);
	m_hintc.SetColor(hintcolor);
	
	GetDlgItem(IDC_STATIC_YC_C6,&hwnd);
	m_unitc.SubclassWindow(hwnd);
	m_unitc.SetColor(unitcolor);

	if (hintenable==0)
	{
		GetDlgItem(IDC_STATIC_YC_HINT,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_YC_HINT,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HINTCOLOR,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YC_C5,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YC_HINTDIS,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_YC_HINTDIS,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CNewYC_PROP_INFO::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	m_hintc.GetColor(&hintcolor);
    m_unitc.GetColor(&unitcolor);

	CPropertyPage::OnOK();
}
