// SZ_SgbwLineInfoDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SZ_SgbwLineInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwLineInfoDlg dialog


CSZ_SgbwLineInfoDlg::CSZ_SgbwLineInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSZ_SgbwLineInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSZ_SgbwLineInfoDlg)
	m_name = _T("");
	m_enum = 0;
	m_prop = 0.0f;
	m_tdno = 0;
	//}}AFX_DATA_INIT
}


void CSZ_SgbwLineInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSZ_SgbwLineInfoDlg)
	DDX_Control(pDX, IDC_R_SZ_SGBWZB, m_zb);
	DDX_Control(pDX, IDC_R_SZ_SGBWU, m_u);
	DDX_Control(pDX, IDC_R_SZ_SGBWI, m_i);
	DDX_Control(pDX, IDC_R_SZ_SGBWGT, m_gt);
	DDX_Control(pDX, IDC_SZ_SGBWCOLOR, m_color);
	DDX_Text(pDX, IDC_E_SZ_DOTNAME, m_name);
	DDX_Text(pDX, IDC_E_SZ_ENUM, m_enum);
	DDV_MinMaxInt(pDX, m_enum, 0, 4);
	DDX_Text(pDX, IDC_E_SZ_PROP, m_prop);
	DDV_MinMaxFloat(pDX, m_prop, 0.f, 1.e+007f);
	DDX_Text(pDX, IDC_E_SZ_TDNO, m_tdno);
	DDV_MinMaxInt(pDX, m_tdno, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSZ_SgbwLineInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSZ_SgbwLineInfoDlg)
	ON_BN_CLICKED(IDC_R_SZ_SGBWZB, OnRSzSgbwzb)
	ON_BN_CLICKED(IDC_R_SZ_SGBWU, OnRSzSgbwu)
	ON_BN_CLICKED(IDC_R_SZ_SGBWI, OnRSzSgbwi)
	ON_BN_CLICKED(IDC_R_SZ_SGBWGT, OnRSzSgbwgt)
	ON_BN_CLICKED(IDC_SZ_BNDOT, OnSzBndot)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwLineInfoDlg message handlers

BOOL CSZ_SgbwLineInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_name=m_cDotname;
	m_enum=m_nEnum;
	m_prop=m_nProp;
	m_tdno=m_nTdNo;
	m_color.SetColor(m_nColor);

	if (m_nZbgt==1) m_zb.SetCheck(1);
	else m_gt.SetCheck(1);

	if (m_nUorI==1) m_u.SetCheck(1);
	else m_i.SetCheck(1);

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSZ_SgbwLineInfoDlg::OnRSzSgbwzb() 
{
	// TODO: Add your control notification handler code here
	if (m_zb.GetCheck()==1) m_gt.SetCheck(0);
	else m_gt.SetCheck(1);
}

void CSZ_SgbwLineInfoDlg::OnRSzSgbwu() 
{
	// TODO: Add your control notification handler code here
	if (m_u.GetCheck()==1) m_i.SetCheck(0);
	else m_i.SetCheck(1);
}

void CSZ_SgbwLineInfoDlg::OnRSzSgbwi() 
{
	// TODO: Add your control notification handler code here
	if (m_i.GetCheck()==1) m_u.SetCheck(0);
	else m_u.SetCheck(1);
}

void CSZ_SgbwLineInfoDlg::OnRSzSgbwgt() 
{
	// TODO: Add your control notification handler code here
	if (m_gt.GetCheck()==1) m_zb.SetCheck(0);
	else m_zb.SetCheck(1);
}

void CSZ_SgbwLineInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return ;

	if (lstrcmp(m_name,"")==0)
	{
		AfxMessageBox("请输入点英文名!");
		return ;
	}

	if (m_zb.GetCheck()==1) m_nZbgt=1;
	else m_nZbgt=2;

	if (m_u.GetCheck()==1) m_nUorI=1;
	else m_nUorI=2;

	strcpy(m_cDotname,m_name);
	m_color.GetColor(&m_nColor);

	m_nProp=m_prop;
	m_nTdNo=m_tdno;
	m_nEnum=m_enum;

	CDialog::OnOK();
}

void CSZ_SgbwLineInfoDlg::SetInfo(char *name, int TdNo, float Prop, int Enum, BYTE Zbgt, BYTE UorI, COLORREF Color,bool bZbgt, bool bHaveUandI)
{
	strcpy(m_cDotname,name);
	m_nTdNo=TdNo;
	m_nProp=Prop;
	m_nEnum=Enum;
	m_nColor=Color;
	m_nZbgt=Zbgt;
	m_nUorI=UorI;
	m_bZbgt=bZbgt;
	m_nColor=Color;
	m_bHaveUandI=bHaveUandI;
}

void CSZ_SgbwLineInfoDlg::GetInfo(char *name, int *TdNo, float *Prop, int *Enum, BYTE *Zbgt, BYTE *UorI, COLORREF *Color)
{
	strcpy(name,m_cDotname);
	*TdNo=m_nTdNo;
	*Prop=m_nProp;
	*Enum=m_nEnum;
	*Zbgt=m_nZbgt;
	*UorI=m_nUorI;
	*Color=m_nColor;
}

void CSZ_SgbwLineInfoDlg::SetRadio()
{
	if (m_bZbgt)
	{
		m_zb.EnableWindow(true);
		m_gt.EnableWindow(true);
		if (m_nZbgt==1)
		{
			m_zb.SetCheck(1);
			m_gt.SetCheck(0);
		}
		else
		{
			m_zb.SetCheck(0);
			m_gt.SetCheck(1);
		}
		m_u.EnableWindow(true);
		m_i.EnableWindow(true);
		if (m_nUorI==1)
		{
			m_u.SetCheck(1);
			m_i.SetCheck(0);
		}
		else
		{
			m_u.SetCheck(0);
			m_i.SetCheck(1);
		}
	}
	else
	{
		m_zb.EnableWindow(false);
		m_zb.SetCheck(1);
		m_gt.EnableWindow(false);
		m_gt.SetCheck(0);

		if (m_bHaveUandI)
		{
			m_u.EnableWindow(true);
			m_i.EnableWindow(true);
			if (m_nUorI==1)
			{
				m_u.SetCheck(1);
				m_i.SetCheck(0);
			}
			else
			{
				m_u.SetCheck(0);
				m_i.SetCheck(1);
			}
		}
		else
		{
			m_u.EnableWindow(false);
			m_i.EnableWindow(false);
			m_u.SetCheck(1);
			m_i.SetCheck(0);
		}
	}
}

void CSZ_SgbwLineInfoDlg::OnSzBndot() 
{
	UpdateData(TRUE);
	
}

void CSZ_SgbwLineInfoDlg::OnBnClickedOk()
{
	OnOK();
}
