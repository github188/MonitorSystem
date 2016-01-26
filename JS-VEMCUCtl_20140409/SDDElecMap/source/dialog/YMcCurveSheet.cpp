#include "stdafx.h"
#include "dialog./ymccurvesheet.h"

IMPLEMENT_DYNAMIC(CYMcCurveSheet, CPropertySheet)

CYMcCurveSheet::CYMcCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CYMcCurveSheet::CYMcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_ymccurvepage1) ;
	AddPage(&m_ymccurvepage2) ;

}

CYMcCurveSheet::~CYMcCurveSheet(void)
{

}

BEGIN_MESSAGE_MAP(CYMcCurveSheet, CPropertySheet)
END_MESSAGE_MAP()

BOOL CYMcCurveSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码
	//m_ymccurvepage2.curvearray = 
	return bResult;
}
