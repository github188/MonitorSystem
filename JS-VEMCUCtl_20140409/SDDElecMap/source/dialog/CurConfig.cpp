// CurConfig.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/curve.h"
#include "dialog/CurConfig.h"
#include "dialog/SecondCurConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurConfig dialog


CCurConfig::CCurConfig(): CPropertyPage(CCurConfig::IDD)
{
	//{{AFX_DATA_INIT(CCurConfig)
	m_curgroupname  =  _T("");
	m_singlecur = 0;
	m_firstleftunit = "单位名称";
	m_firstrightunit = "单位名称";
	m_levelcur = 0;
	m_secondleftunit = "单位名称";
	m_secondrightunit ="单位名称";
	m_firstleftinterval = 0.0f;
	m_firstleftmax = 0.0f;
	m_firstrightinterval = 0.0f;
	m_firstrightmax = 0.0f;
	m_secondleftinterval = 0.0f;
	m_secondleftmax = 0.0f;
	m_secondrightinterval = 0.0f;
	m_secondrightmax = 0.0f;
	m_coordinate = 0;
	m_firstleftmin = 0.0f;
	m_secondleftmin = 0.0f;
	m_secondrightmin = 0.0f;
	m_firstrightmin = 0.0f;
	//}}AFX_DATA_INIT
}


void CCurConfig::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurConfig)
//	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTINTERVALEDIT, m_firstrightintervaledit);
	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTUNITEDIT, m_firstrightunitedit);
	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTMAXEDIT, m_firstrightmaxedit);
	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTMINEDIT, m_firstrightminedit);
//	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTMAXSTATIC, m_firstrightmaxstatic);
//	DDX_Control(pDX, IDC_SHQ_FIRSTRIGHTMINSTATIC, m_firstrightminstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTMINSTATIC, m_secondrightminstatic);
//  DDX_Control(pDX, IDC_SHQ_SECONDLEFTMINSTATIC, m_secondleftminstatic);
	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTMINEDIT, m_secondrightminedit);
	DDX_Control(pDX, IDC_SHQ_SECONDLEFTMINEDIT, m_secondleftminedit);
	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTUNITEDIT, m_secondrightunitedit);
	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTMAXEDIT, m_secondrightmaxedit);
//	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTINTERVALEDIT, m_secondrightintervaledit);
	DDX_Control(pDX, IDC_SHQ_SECONDLEFTUNITEDIT, m_secondleftunitedit);
	DDX_Control(pDX, IDC_SHQ_SECONDLEFTMAXEDIT, m_secondleftmaxedit);
//	DDX_Control(pDX, IDC_SHQ_SECONDLEFTINTERVALEDIT, m_secondleftintervaledit);
//	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTUNITSTATIC, m_secondrightunitstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTMAXSTATIC, m_secondrightmaxstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDRIGHTINTERVALSTATIC, m_secondrightintervalstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDLEFTUNITSTATIC, m_secondleftunitstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDLEFTMAXSTATIC, m_secondleftmaxstatic);
//	DDX_Control(pDX, IDC_SHQ_SECONDLEFTINTERVALSTATIC, m_secondleftintervalstatic);
	DDX_Text(pDX, IDC_SHQ_CURGROUPNAMEEDIT, m_curgroupname);
	DDV_MaxChars(pDX, m_curgroupname, 32);
	DDX_Radio(pDX, IDC_SHQ_CURGROUPSINGLECUR, m_singlecur);
	DDX_Text(pDX, IDC_SHQ_FIRSTLEFTUNITEDIT, m_firstleftunit);
	DDV_MaxChars(pDX, m_firstleftunit, 8);
	DDX_Text(pDX, IDC_SHQ_FIRSTRIGHTUNITEDIT, m_firstrightunit);
	DDV_MaxChars(pDX, m_firstrightunit, 8);
	DDX_Radio(pDX, IDC_SHQ_LEVELCUR, m_levelcur);
	DDX_Text(pDX, IDC_SHQ_SECONDLEFTUNITEDIT, m_secondleftunit);
	DDV_MaxChars(pDX, m_secondleftunit, 8);
	DDX_Text(pDX, IDC_SHQ_SECONDRIGHTUNITEDIT, m_secondrightunit);
	DDV_MaxChars(pDX, m_secondrightunit, 8);
//	DDX_Text(pDX, IDC_SHQ_FIRSTLEFTINTERVALEDIT, m_firstleftinterval);
	DDX_Text(pDX, IDC_SHQ_FIRSTLEFTMAXEDIT, m_firstleftmax);
//	DDX_Text(pDX, IDC_SHQ_FIRSTRIGHTINTERVALEDIT, m_firstrightinterval);
	DDX_Text(pDX, IDC_SHQ_FIRSTRIGHTMAXEDIT, m_firstrightmax);
//	DDX_Text(pDX, IDC_SHQ_SECONDLEFTINTERVALEDIT, m_secondleftinterval);
	DDX_Text(pDX, IDC_SHQ_SECONDLEFTMAXEDIT, m_secondleftmax);
//	DDX_Text(pDX, IDC_SHQ_SECONDRIGHTINTERVALEDIT, m_secondrightinterval);
	DDX_Text(pDX, IDC_SHQ_SECONDRIGHTMAXEDIT, m_secondrightmax);
	DDX_Radio(pDX, IDC_SHQ_SINGLECOORDINATE, m_coordinate);
	DDX_Text(pDX, IDC_SHQ_FIRSTLEFTMINEDIT, m_firstleftmin);
	DDX_Text(pDX, IDC_SHQ_SECONDLEFTMINEDIT, m_secondleftmin);
	DDX_Text(pDX, IDC_SHQ_SECONDRIGHTMINEDIT, m_secondrightmin);
	DDX_Text(pDX, IDC_SHQ_FIRSTRIGHTMINEDIT, m_firstrightmin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurConfig, CPropertyPage)
	//{{AFX_MSG_MAP(CCurConfig)
	ON_BN_CLICKED(IDC_SHQ_CURGROUPDOUBLECUR, OnShqCurgroupdoublecur)
	ON_BN_CLICKED(IDC_SHQ_CURGROUPSINGLECUR, OnShqCurgroupsinglecur)
	ON_BN_CLICKED(IDC_SHQ_SINGLECOORDINATE, OnShqSinglecoordinate)
	ON_BN_CLICKED(IDC_SHQ_DOUBLECOORDINATE, OnShqDoublecoordinate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurConfig message handlers

void CCurConfig::OnShqCurgroupdoublecur() 
{
	m_singlecur = 1;
	if(m_secondpagecount == 0)
	{
      	p_sheet->AddPage(p_page);
		m_secondpagecount = 1;
	}

	if(m_coordinate == 0)
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(TRUE);  
		SetSecondRightCurEnable(FALSE);
	}
	else
	{
	   	SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(TRUE);  
		SetSecondRightCurEnable(TRUE);
	}
}

void CCurConfig::OnShqCurgroupsinglecur() 
{
	m_singlecur = 0;
	if((p_page!=NULL) && (m_secondpagecount == 1))
	{		
    	p_sheet->RemovePage(p_page);
		m_secondpagecount=0;
	}

	if(m_coordinate == 0)
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(FALSE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);
	}
	else
	{
		SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(FALSE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);
	}
}

void CCurConfig::OnShqSinglecoordinate() 
{
	m_coordinate = 0;
	if(m_singlecur == 0)
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(FALSE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);
	}
	else
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(TRUE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);
	}	
}

void CCurConfig::OnShqDoublecoordinate() 
{
	m_coordinate = 1;
	if(m_singlecur == 0)
	{		
     	SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(FALSE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);
	}
	else
	{
		SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(TRUE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(TRUE);
	}
}

void CCurConfig::SetSecondLeftCurEnable(BOOL EnableFlag)
{
	m_secondleftminedit.EnableWindow(EnableFlag);
	m_secondleftmaxedit.EnableWindow(EnableFlag);
	m_secondleftunitedit.EnableWindow(EnableFlag);
}

BOOL CCurConfig::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CCurConfig::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if((m_coordinate == 0)&&(m_singlecur == 0))
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(FALSE);  //使第二坐标系配置不可用
		SetSecondRightCurEnable(FALSE);

		p_sheet->RemovePage(p_page); //使第二曲线组设置不可见
		m_secondpagecount = 0;
	}
	else if((m_coordinate == 0) && (m_singlecur != 0))
	{
		SetFirstRightCurEnable(FALSE);
		SetSecondLeftCurEnable(TRUE);  
		SetSecondRightCurEnable(FALSE);
	}
	else if((m_coordinate != 0) && (m_singlecur == 0))
	{
		SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(FALSE);  
		SetSecondRightCurEnable(FALSE);

		p_sheet->RemovePage(p_page); 
		m_secondpagecount = 0;
	}
	else
	{
		SetFirstRightCurEnable(TRUE);
		SetSecondLeftCurEnable(TRUE);  
		SetSecondRightCurEnable(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurConfig::SetCurParam(LPCSTR content)
{
	m_curgroupname = content;
}

void CCurConfig::GetCurParam(LPSTR content)
{
	strcpy(content,m_curgroupname);
}

void CCurConfig::SetDoubleCurveEnable(BOOL EnableFlag)
{
	m_secondleftminedit.EnableWindow(EnableFlag);
	m_secondleftmaxedit.EnableWindow(EnableFlag);
	m_secondleftunitedit.EnableWindow(EnableFlag);
}

void CCurConfig::SetSecondRightCurEnable(BOOL EnableFlag)
{
  	m_secondrightminedit.EnableWindow(EnableFlag);
   	m_secondrightmaxedit.EnableWindow(EnableFlag);
   	m_secondrightunitedit.EnableWindow(EnableFlag);
}

void CCurConfig::SetFirstRightCurEnable(BOOL EnableFlag)
{
	m_firstrightminedit.EnableWindow(EnableFlag);
   	m_firstrightmaxedit.EnableWindow(EnableFlag);
   	m_firstrightunitedit.EnableWindow(EnableFlag);
}
