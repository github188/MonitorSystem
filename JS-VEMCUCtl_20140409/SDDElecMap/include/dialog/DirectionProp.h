#if !defined(AFX_DIRECTIONPROP_H__5A29CF2E_47DD_443B_A035_3B964769413E__INCLUDED_)
#define AFX_DIRECTIONPROP_H__5A29CF2E_47DD_443B_A035_3B964769413E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectionProp.h : header file
//
#include "canvas/LineProp.h"
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDirectionProp dialog

class CDirectionProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CDirectionProp)

// Construction
public:
	void SetProp(int len, int width, COLORREF  frontcolor, COLORREF  backcolor, BOOL  bTranslate, char dotname[], BOOL  bZeroShow);
	void GetProp(int& len,int& width,COLORREF&  frontcolor,COLORREF&  backcolor,BOOL& bTranslate,char dotname[17],BOOL& bZeroShow);
	CDirectionProp();
	~CDirectionProp();

// Dialog Data
	//{{AFX_DATA(CDirectionProp)
	enum { IDD = IDD_DIRECTION };
	BOOL	m_bTranslate;
	BOOL	m_bZeroShow;
	UINT	m_len;
	UINT	m_width;
	CString	m_dotname;
	//}}AFX_DATA


	COLORREF m_color1;
	COLORREF m_color2;
  CStaticColor  m_ctrlColor1;
  CStaticColor  m_ctrlColor2;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDirectionProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDirectionProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor1();
	afx_msg void OnColor2();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTIONPROP_H__5A29CF2E_47DD_443B_A035_3B964769413E__INCLUDED_)
