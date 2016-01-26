#pragma once

#include "afxdlgs.h"
#include "YMcCurvePage1.h"
#include "YMcCurvePage2.h"

class CYMcCurveSheet :	public CPropertySheet
{
	DECLARE_DYNAMIC(CYMcCurveSheet)
public:
	CYMcCurveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CYMcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CYMcCurveSheet(void);
public:
	CYMcCurvePage1 m_ymccurvepage1 ;
	CYMcCurvePage2 m_ymccurvepage2 ;
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};
