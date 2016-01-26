#if !defined(AFX_FIRSTCURCONFIG_H__F749C5C2_F098_11D5_B808_0050BABDFD68__INCLUDED_)
#define AFX_FIRSTCURCONFIG_H__F749C5C2_F098_11D5_B808_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FirstCurConfig.h : header file
//
#include "../../resource.h"
#include "canvas/LineProp.h"
/////////////////////////////////////////////////////////////////////////////
// CFirstCurConfig dialog

class CFirstCurConfig : public CPropertyPage
{
//	DECLARE_DYNCREATE(CFirstCurConfig)
// Construction
public:
	void UpdateItem(LPTSTR cname);
	int m_itemindex;
	int m_curcount;
	curveinfo m_curinfolist[8];
	BOOL OnInitDialog();
	CFirstCurConfig();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFirstCurConfig)
	enum { IDD = IDD_FIRSTCURCONFIG };
	CListCtrl	m_infolist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirstCurConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFirstCurConfig)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPopCurAdd();
	afx_msg void OnPopCurDel();
	afx_msg void OnPopCurEdit();
	afx_msg void OnUpdatePopCurAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopCurDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopCurEdit(CCmdUI* pCmdUI);
	afx_msg void OnItemchangedShqFirstinfolist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkShqFirstinfolist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColor();
	afx_msg void OnColor3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bChecked;
	int m_nIndex;
	float m_nMax;
	float m_nMin;
	float m_nWidth;
	COLORREF m_color;//上限颜色
	COLORREF m_xcolor;//下限颜色
	//CStaticColor  m_ctrlColor;
private:
	void UpdateColor();
	void UpdateColor3();
public:
	afx_msg void OnBnClickedCheck1();
	CStaticColor m_colorCtr1;
	CStaticColor m_colorCtr2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRSTCURCONFIG_H__F749C5C2_F098_11D5_B808_0050BABDFD68__INCLUDED_)
