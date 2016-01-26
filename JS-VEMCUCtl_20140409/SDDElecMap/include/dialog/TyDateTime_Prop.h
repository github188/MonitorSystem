#if !defined(AFX_TYDATETIME_PROP_H__8AB86674_2A59_4DDD_8A88_1601C13974E6__INCLUDED_)
#define AFX_TYDATETIME_PROP_H__8AB86674_2A59_4DDD_8A88_1601C13974E6__INCLUDED_

#include "../../resource.h"
#include "canvas/RTUColorStatic.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TyDateTime_Prop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTyDateTime_Prop dialog

class CTyDateTime_Prop : public CPropertyPage
{
	DECLARE_DYNCREATE(CTyDateTime_Prop)

// Construction
public:
	CRTUColorStatic m_normalc;
    void GetPara(COLORREF *itcolor,LOGFONT *ff,float *x0,float* y0,float *fontw,float *fonth,float *fontj,BYTE* timekind,BYTE *datekind,BYTE *diskind);
	void SetPara(COLORREF itcolor,LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj,BYTE timekind,BYTE datekind,BYTE diskind,WORD year,BYTE month,BYTE day,BYTE hour,BYTE min,BYTE sec,WORD msec);
	BYTE m_timekind;
	BYTE m_datekind;
	BYTE m_diskind;
	COLORREF m_color;
	LOGFONT lgfont;
	float rectx0;
	float recty0;
	float fontwidth;
	float fontheight;
	float fontjj;
	BYTE bMonth,bDay,bHour,bMin,bSec;
	WORD wYear,wMSec;
	CString Trans(BYTE timekind,BYTE datekind,BYTE diskind);
	CTyDateTime_Prop();
	~CTyDateTime_Prop();

// Dialog Data
	//{{AFX_DATA(CTyDateTime_Prop)
	enum { IDD = CZ_IDD_DATETIME_DIS };
	CStatic	m_static;
	CListBox	m_list;
	float	m_editfh;
	float	m_editfw;
	float	m_editjj;
	float	m_editx0;
	float	m_edity0;
	int		m_radio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTyDateTime_Prop)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTyDateTime_Prop)
	afx_msg void OnRadio1();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	virtual BOOL OnInitDialog();
	afx_msg void OnIdcButtonFont();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYDATETIME_PROP_H__8AB86674_2A59_4DDD_8A88_1601C13974E6__INCLUDED_)
