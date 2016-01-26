#if !defined(AFX_PROPPOSITIONRECT_H__5E05F760_F6E9_4C5C_B1FA_092317D35E8D__INCLUDED_)
#define AFX_PROPPOSITIONRECT_H__5E05F760_F6E9_4C5C_B1FA_092317D35E8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropPositionRect.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropPositionRect dialog

class CPropPositionRect : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropPositionRect)

// Construction
public:
	void GetContent(float& x0,float& y0,float& x1,float& y1);
	void SetContent(float x0,float y0,float x1,float y1);
	CPropPositionRect();
	~CPropPositionRect();

// Dialog Data
	//{{AFX_DATA(CPropPositionRect)
	enum { IDD = IDD_POSITION_RECT };
	float	m_x0;
	float	m_y0;
	float	m_x1;
	float	m_y1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPositionRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPositionRect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEdit1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPOSITIONRECT_H__5E05F760_F6E9_4C5C_B1FA_092317D35E8D__INCLUDED_)
