// YMcCurvePage1.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YMcCurvePage1.h"


// CYMcCurvePage1 对话框

IMPLEMENT_DYNAMIC(CYMcCurvePage1, CPropertyPage)
CYMcCurvePage1::CYMcCurvePage1()
	: CPropertyPage(CYMcCurvePage1::IDD)
	, m_strmcename(_T(""))
	, m_fLeftcoord(0)
	, m_fTopcoord(0)
	, m_fXlen(0)
	, m_fYlen(0)
	, m_hMin(0)
	, m_hMax(0)
	, m_hScale(0)
	, m_dMin(0)
	, m_dMax(0)
	, m_dScale(0)
	, m_mMin(0)
	, m_mMax(0)
	, m_mScale(0)
	, m_yMin(0)
	, m_yMax(0)
	, m_yScale(0)
	, m_unit(_T(""))
{
}

CYMcCurvePage1::~CYMcCurvePage1()
{
}

void CYMcCurvePage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strmcename);
	DDV_MaxChars(pDX, m_strmcename, 17);
	DDX_Text(pDX, IDC_EDIT2, m_fLeftcoord);
	DDX_Text(pDX, IDC_EDIT3, m_fTopcoord);
	DDX_Text(pDX, IDC_EDIT4, m_fXlen);
	DDX_Text(pDX, IDC_EDIT5, m_fYlen);
	DDX_Text(pDX, IDC_EDIT6, m_hMin);
	DDX_Text(pDX, IDC_EDIT7, m_hMax);
	DDX_Text(pDX, IDC_EDIT8, m_hScale);
	DDX_Text(pDX, IDC_EDIT9, m_dMin);
	DDX_Text(pDX, IDC_EDIT10, m_dMax);
	DDX_Text(pDX, IDC_EDIT11, m_dScale);
	DDX_Text(pDX, IDC_EDIT17, m_mMin);
	DDX_Text(pDX, IDC_EDIT18, m_mMax);
	DDX_Text(pDX, IDC_EDIT19, m_mScale);
	DDX_Text(pDX, IDC_EDIT12, m_yMin);
	DDX_Text(pDX, IDC_EDIT13, m_yMax);
	DDX_Text(pDX, IDC_EDIT15, m_yScale);
	DDX_Text(pDX, IDC_EDIT20, m_unit);
	DDV_MaxChars(pDX, m_unit, 8);
}


BEGIN_MESSAGE_MAP(CYMcCurvePage1, CPropertyPage)
END_MESSAGE_MAP()


// CYMcCurvePage1 消息处理程序
