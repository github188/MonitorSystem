// CurEditDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/LineProp.h"
#include "dialog/CurEditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurEditDialog dialog


CCurEditDialog::CCurEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCurEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurEditDialog)
	m_compare = FALSE;
	m_ename = _T("");
	m_scale = 0.0f;
	m_precission = 0;
	m_width = 0;
	m_swing = 0;
	m_cname = _T("");
	m_reference = _T("");
	//}}AFX_DATA_INIT
}


void CCurEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurEditDialog)
	DDX_Control(pDX, IDC_COMBOX_XY, m_referencecombo);
	DDX_Control(pDX, IDC_BUTTON1, m_enamebutton);
	DDX_Check(pDX, IDC_CHECK1, m_compare);
	DDX_Text(pDX, IDC_EDIT1, m_ename);
	DDV_MaxChars(pDX, m_ename, 16);
	DDX_Text(pDX, IDC_EDIT2, m_scale);
	DDX_Text(pDX, IDC_EDIT3, m_precission);
	DDX_Text(pDX, IDC_EDIT4, m_width);
	DDX_Text(pDX, IDC_EDIT5, m_swing);
	DDX_Text(pDX, IDC_EDIT6, m_cname);
	DDV_MaxChars(pDX, m_cname, 32);
	DDX_CBString(pDX, IDC_COMBOX_XY, m_reference);
	DDV_MaxChars(pDX, m_reference, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurEditDialog, CDialog)
	//{{AFX_MSG_MAP(CCurEditDialog)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurEditDialog message handlers

BOOL CCurEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_referencecombo.InsertString(0,"左坐标系");
	m_referencecombo.InsertString(1,"右坐标系");
	if(m_reference == "左坐标系")
    	m_referencecombo.SetCurSel(0);
	else
		m_referencecombo.SetCurSel(1);

	m_ctrlColor.SubclassDlgItem(IDC_COLOR,this);
	
	UpdateColor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurEditDialog::OnColor() 
{
	if (m_color == m_ctrlColor.m_Color) 
		return;
  
 	m_color=m_ctrlColor.m_Color;

	UpdateColor();
}

void CCurEditDialog::UpdateColor()
{
	if (m_ctrlColor.m_Color != m_color)
	{
		m_ctrlColor.m_Color=m_color;
		m_ctrlColor.Invalidate();
	}
}

void CCurEditDialog::OnButton1() 
{
	char s[256],myename[17],mycname[33];
	HINSTANCE hmydll;
	hmydll = LoadLibrary("FindDot.dll");
	if (hmydll==NULL)
	{
		sprintf(s,"%s 调入失败！","FindDot.dll");
		AfxMessageBox(s);
		return;
	}

	ShowFindDotDlg * pf;
	pf = (ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
	if (pf == NULL)
	{
		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！","FindDot.dll");
		AfxMessageBox(s);
		FreeLibrary(hmydll);
		hmydll = NULL;
		return;
	}

	if ((*pf)(1,myename,mycname))
	{
		m_ename = myename;
		char cname[33];
		sprintf(cname,"%s曲线",mycname);
		m_cname = cname;
	}

	UpdateData(FALSE);

	FreeLibrary(hmydll);	
}
