#if !defined(AFX_SZLISTCTRL_H__BF71D3E6_3CBA_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZLISTCTRL_H__BF71D3E6_3CBA_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SZListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSZListCtrl window

class CSZListCtrl : public CListCtrl
{
// Construction
public:
	CSZListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSZListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColorList(int Listnum);
	void SetColor(COLORREF *ColorArray, int ColorArrayLen);
	virtual ~CSZListCtrl();

	// Generated message map functions
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CSZListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CRect GetFullCellRect(int nItem, BOOL outside);
	CRect GetCellRect(int nItem, int nColumn, BOOL outside);
	COLORREF m_aColorArray[32];
	//颜色数组
	int m_nColorArrayLen;
	//颜色数组长度
	int m_nListnum;//列数
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SZLISTCTRL_H__BF71D3E6_3CBA_11D6_A5ED_0080C8F60823__INCLUDED_)
