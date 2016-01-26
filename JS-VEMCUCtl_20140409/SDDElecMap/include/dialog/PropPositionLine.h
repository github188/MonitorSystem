#if !defined(AFX_PROPPOSITIONLINE_H__F25783C5_F617_11D5_8B44_444553540000__INCLUDED_)
#define AFX_PROPPOSITIONLINE_H__F25783C5_F617_11D5_8B44_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PropPositionLine.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPropPositionLine dialog

class CPropPositionLine : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropPositionLine)

// Construction
public:
	void GetPosition(float& x0,float& y0,float& x1,float& y1);
	void SetPosition(float x0,float y0,float x1,float y1);
	CPropPositionLine();
	~CPropPositionLine();

// Dialog Data
	//{{AFX_DATA(CPropPositionLine)
	enum { IDD = IDD_POSITION_LINE };
	float	m_x0;
	float	m_y0;
	float	m_x1;
	float	m_y1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPositionLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPositionLine)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPOSITIONLINE_H__F25783C5_F617_11D5_8B44_444553540000__INCLUDED_)
