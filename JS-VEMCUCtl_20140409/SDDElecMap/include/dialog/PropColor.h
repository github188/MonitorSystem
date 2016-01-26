#if !defined(AFX_PROPCOLOR_H__F437E7DF_F1AE_4C28_B3C8_67C6F2B07AD0__INCLUDED_)
#define AFX_PROPCOLOR_H__F437E7DF_F1AE_4C28_B3C8_67C6F2B07AD0__INCLUDED_

#include "../../resource.h"
#include "canvas/LineProp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropColor dialog

class CPropColor : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropColor)

// Construction
public:
	void SetColor(COLORREF color);
	void GetColor(COLORREF& color);
	CPropColor();
	~CPropColor();

private:
	COLORREF m_Color;
  CStaticColor  m_ctrlColor;

// Dialog Data
	//{{AFX_DATA(CPropColor)
	enum { IDD = IDD_COLOR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateColor();
	// Generated message map functions
	//{{AFX_MSG(CPropColor)
	afx_msg void OnColor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPCOLOR_H__F437E7DF_F1AE_4C28_B3C8_67C6F2B07AD0__INCLUDED_)
