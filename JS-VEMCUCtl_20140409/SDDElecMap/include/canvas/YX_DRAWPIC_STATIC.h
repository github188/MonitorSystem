#if !defined(AFX_YX_DRAWPIC_STATIC_H__AEACA316_1A02_4474_8002_968FF2650C72__INCLUDED_)
#define AFX_YX_DRAWPIC_STATIC_H__AEACA316_1A02_4474_8002_968FF2650C72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// YX_DRAWPIC_STATIC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYX_DRAWPIC_STATIC window

class CYX_DRAWPIC_STATIC : public CStatic
{
// Construction
public:
	int m_kind;
	COLORREF m_color;
	CString m_name;

	CYX_DRAWPIC_STATIC();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYX_DRAWPIC_STATIC)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearIt();
	void SetDrawVec(CString name,COLORREF color,int allcolor);
	void SetDrawBmp(CString name,COLORREF color,int trans);
	virtual ~CYX_DRAWPIC_STATIC();

	// Generated message map functions
protected:
	//{{AFX_MSG(CYX_DRAWPIC_STATIC)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YX_DRAWPIC_STATIC_H__AEACA316_1A02_4474_8002_968FF2650C72__INCLUDED_)
