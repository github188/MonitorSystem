#if !defined(AFX_YC_PROP_DIS_H__A97B3ABE_C2CA_4A09_A26C_71C49DDE3D81__INCLUDED_)
#define AFX_YC_PROP_DIS_H__A97B3ABE_C2CA_4A09_A26C_71C49DDE3D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../resource.h"
#include "canvas/LineProp.h"

// YC_PROP_DIS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYC_PROP_DIS dialog

class CYC_PROP_DIS : public CPropertyPage
{
	DECLARE_DYNCREATE(CYC_PROP_DIS)
// Construction
public:
	CYC_PROP_DIS(BYTE kind=1);
	~CYC_PROP_DIS();
	
	CRTUColorStatic m_normalc;
	CRTUColorStatic m_disablec;
	CRTUColorStatic m_upc;
	CRTUColorStatic m_downc;

	BYTE itkind;//1 yc 2 ym
	
    LOGFONT lgfont;//字体
    COLORREF levelupcolor;//越上限颜色
    COLORREF leveldowncolor;//越下限颜色
    COLORREF normalcolor;//正常颜色
    COLORREF disablecolor;//无效颜色

	float rectx0;
	float recty0;
	float fontwidth;
	float fontheight;
	float fontjj;
	//yc
	void GetPara(COLORREF *disablec,COLORREF *normalc,COLORREF *leveldc,COLORREF *leveluc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj);
	void SetPara(COLORREF disablec,COLORREF normalc,COLORREF leveldc,COLORREF leveluc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj);
	//ym
	void GetPara2(COLORREF *disablec,COLORREF *normalc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj);
	void SetPara2(COLORREF disablec,COLORREF normalc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj);

// Dialog Data
	//{{AFX_DATA(CYC_PROP_DIS)
	enum { IDD = CZ_IDD_YC1_DIS };
	float	m_editx0;
	float	m_edity0;
	float	m_editfh;
	float	m_editfw;
	float	m_editjj;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYC_PROP_DIS)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYC_PROP_DIS)
	virtual BOOL OnInitDialog();
	afx_msg void OnIdcButtonFont();
	afx_msg void OnRadio1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YC_PROP_DIS_H__A97B3ABE_C2CA_4A09_A26C_71C49DDE3D81__INCLUDED_)
