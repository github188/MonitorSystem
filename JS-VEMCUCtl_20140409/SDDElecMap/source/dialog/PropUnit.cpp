// PropUnit.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropUnit property page

IMPLEMENT_DYNCREATE(CPropUnit, CPropertyPage)

CPropUnit::CPropUnit() : CPropertyPage(CPropUnit::IDD)
, m_Unit_QLB(FALSE)
, m_Unit_CKLB(FALSE)
{
	//{{AFX_DATA_INIT(CPropUnit)
	m_YK_Caption = _T("");
	m_Unit_tq = FALSE;
	m_Unit_soe = FALSE;
	m_Unit_set = FALSE;
	m_Unit_lb = FALSE;
	m_Unit_canshu = FALSE;
	m_Unit_diandu = FALSE;
	m_Unit_yc = FALSE;
	m_Unit_Caption = _T("");
	m_Unit_yc_special = FALSE;
	m_Unit_pct = FALSE;
	m_Unit_yxfg=FALSE ;
	m_Unit_SGBW=FALSE;
	m_Unit_QLB=FALSE;
	m_Unit_CKLB=FALSE;
	//}}AFX_DATA_INIT
}

CPropUnit::~CPropUnit()
{
}

void CPropUnit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropUnit)
	DDX_Text(pDX, IDC_YK_Caption, m_YK_Caption);
	DDX_Check(pDX, IDC_Unit_tq, m_Unit_tq);
	DDX_Check(pDX, IDC_Unit_soe, m_Unit_soe);
	DDX_Check(pDX, IDC_Unit_set, m_Unit_set);
	DDX_Check(pDX, IDC_Unit_lb, m_Unit_lb);
	DDX_Check(pDX, IDC_Unit_canshu, m_Unit_canshu);
	DDX_Check(pDX, IDC_Unit_diandu, m_Unit_diandu);
	DDX_Check(pDX, IDC_Unit_yc, m_Unit_yc);
	DDX_Text(pDX, IDC_UNIT_Caption, m_Unit_Caption);
	DDX_Check(pDX, IDC_Unit_yx, m_Unit_yc_special);
	DDX_Check(pDX, IDC_Unit_PCt, m_Unit_pct);
	DDX_Check(pDX, IDC_Unit_yxfg, m_Unit_yxfg);
	DDX_Check(pDX, IDC_Unit_SGBW, m_Unit_SGBW);
	//}}AFX_DATA_MAP

	DDX_Check(pDX, IDC_Unit_qlb, m_Unit_QLB);
	DDX_Check(pDX, IDC_Unit_cklb, m_Unit_CKLB);
}


BEGIN_MESSAGE_MAP(CPropUnit, CPropertyPage)
	//{{AFX_MSG_MAP(CPropUnit)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropUnit message handlers

void CPropUnit::SetUnitParam(char text[],char UnitCaption[],BOOL bUnittq,BOOL bUnitsoe,BOOL bUnityc,BOOL bUnitycspecial,
		BOOL bUnitdiandu,BOOL bUnitlb,BOOL bUnitset,BOOL bUnitcanshu,BOOL bUnitpct ,BOOL bUnitYxfg, BOOL bUnitSGBW, BOOL bUnitQLB, BOOL bUnitCKLB)
{
    m_YK_Caption=text;
	m_Unit_Caption=UnitCaption;
	m_Unit_tq=(bUnittq)?1:0;
	m_Unit_soe=(bUnitsoe)?1:0;
	m_Unit_yc=(bUnityc)?1:0;
	m_Unit_yc_special=(bUnitycspecial)?1:0;
	m_Unit_diandu=(bUnitdiandu)?1:0;
	m_Unit_lb=(bUnitlb)?1:0;
	m_Unit_set=(bUnitset)?1:0;
	m_Unit_canshu=(bUnitcanshu)?1:0;
	m_Unit_pct=(bUnitpct)?1:0;
	m_Unit_yxfg = (bUnitYxfg) ? 1:0 ;
	m_Unit_SGBW = (bUnitSGBW) ? 1:0;
	m_Unit_QLB = (bUnitQLB) ? 0:1;
	m_Unit_CKLB = (bUnitCKLB) ? 0:1;
}

void CPropUnit::GetUnitParam(char text[],char UnitCaption[],BOOL& bUnittq,BOOL& bUnitsoe,BOOL& bUnityc,BOOL& bUnitycspecial,
		BOOL& bUnitdiandu,BOOL& bUnitlb,BOOL& bUnitset,BOOL& bUnitcanshu,BOOL& bUnitpct ,BOOL& bUnitYxfg,BOOL& bUnitSGBW,BOOL& bUnitQLB,BOOL& bUnitCKLB)
{
	strcpy(text,m_YK_Caption);
	strcpy(UnitCaption,m_Unit_Caption);
	bUnittq=(m_Unit_tq==1)?TRUE:FALSE;
	bUnitsoe=(m_Unit_soe==1)?TRUE:FALSE;
	bUnityc=(m_Unit_yc==1)?TRUE:FALSE;
	bUnitycspecial=(m_Unit_yc_special==1)?TRUE:FALSE;
	bUnitdiandu=(m_Unit_diandu==1)?TRUE:FALSE;
	bUnitlb=(m_Unit_lb==1)?TRUE:FALSE;
	bUnitset=(m_Unit_set==1)?TRUE:FALSE;
	bUnitcanshu=(m_Unit_canshu==1)?TRUE:FALSE;	
	bUnitpct=(m_Unit_pct==1)?TRUE:FALSE;
	bUnitYxfg = (m_Unit_yxfg == 1) ? TRUE:FALSE ;
	bUnitSGBW = (m_Unit_SGBW == 1) ? TRUE:FALSE;
	bUnitQLB = (m_Unit_QLB == 1) ? FALSE:TRUE;
	bUnitCKLB = (m_Unit_CKLB == 1) ? FALSE:TRUE;
}

void CPropUnit::OnButton1() 
{
	
}
