#if !defined(AFX_FONTPROP_H__DD00854A_0433_40A9_8058_0A3C66A13F2D__INCLUDED_)
#define AFX_FONTPROP_H__DD00854A_0433_40A9_8058_0A3C66A13F2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontProp.h : header file
//
#include "../../resource.h"
#include "canvas/LineProp.h"


/////////////////////////////////////////////////////////////////////////////
// CFontProp dialog

class CFontProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CFontProp)

// Construction
private:
	
	void UpdateColor();

public:
	virtual void OnOK( );
	void GetFontProp(float& height, float& width, float& jqwidth, BOOL& pl, COLORREF& color, char* font);
	void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	virtual BOOL OnInitDialog();
	CFontProp();
	~CFontProp();

// Dialog Data
	//{{AFX_DATA(CFontProp)
	enum { IDD = IDD_FONT2 };
	CString	m_TextFont;
	float	m_TextHeight;
	float	m_JqWidth;
	float	m_TextWidth;
	int		m_FangXiang;
	//}}AFX_DATA
  
	COLORREF			m_TextColor;
  CStaticColor  m_ctrlColor;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFontProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFontProp)
	afx_msg void OnColor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTPROP_H__DD00854A_0433_40A9_8058_0A3C66A13F2D__INCLUDED_)
