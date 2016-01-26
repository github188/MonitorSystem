#if !defined(AFX_PROPPOSITIONTEXT_H__F25783C4_F617_11D5_8B44_444553540000__INCLUDED_)
#define AFX_PROPPOSITIONTEXT_H__F25783C4_F617_11D5_8B44_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropPositionText.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropPositionText dialog

class CPropPositionText : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropPositionText)

// Construction
public:
	void GetContent(float& x0,float& y0);
	void SetContent(float x0,float y0);
	CPropPositionText();
	~CPropPositionText();

// Dialog Data
	//{{AFX_DATA(CPropPositionText)
	enum { IDD = IDD_POSITION_TEXT };
	float	m_x0;
	float	m_y0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPositionText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPositionText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPOSITIONTEXT_H__F25783C4_F617_11D5_8B44_444553540000__INCLUDED_)
