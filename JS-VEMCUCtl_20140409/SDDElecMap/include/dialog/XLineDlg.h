#if !defined(AFX_XLINEDLG_H__AC756A68_0ABA_4212_B3B7_629BD9EED98B__INCLUDED_)
#define AFX_XLINEDLG_H__AC756A68_0ABA_4212_B3B7_629BD9EED98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XLineDlg.h : header file
//
#include "../../resource.h"
#include "canvas/LineProp.h"

/////////////////////////////////////////////////////////////////////////////
// CXLineDlg dialog

class CXLineProp : public  CPropertyPage
{
// Construction
public:
	void GetXLine(COLORREF& color1,COLORREF & color2,BOOL& blTranslate,int & width,int type[]);
	void SetXLine(COLORREF color1,COLORREF color2,BOOL blTranslate,int width,int type[4]);
	virtual BOOL OnInitDialog( );
	CXLineProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXLineDlg)
	enum { IDD = IDD_XLINE };
	int		m_L1;
	int		m_L2;
	int		m_L3;
	int		m_L4;
	int		m_Width;
	BOOL	m_bTranslate;
	//}}AFX_DATA

	COLORREF m_color1;
	COLORREF m_color2;
  CStaticColor  m_ctrlColor1;
  CStaticColor  m_ctrlColor2;
	
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXLineDlg)
	afx_msg void OnColor1();
	afx_msg void OnColor2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLINEDLG_H__AC756A68_0ABA_4212_B3B7_629BD9EED98B__INCLUDED_)
