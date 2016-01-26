#if !defined(AFX_YX_PROP_DIS_H__90BC989A_374C_41E9_B6DA_9E4179DA9EF8__INCLUDED_)
#define AFX_YX_PROP_DIS_H__90BC989A_374C_41E9_B6DA_9E4179DA9EF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/RTUColorStatic.h"
#include "canvas/YX_DRAWPIC_STATIC.h"

// YX_PROP_DIS.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_DIS dialog
#define BMP_FILE  "c:\\sunpac\\par\\global\\bmp\\*.bmp"
#define VEC_FILE  "c:\\sunpac\\par\\global\\vector\\*.vec"

class CYX_PROP_DIS : public CPropertyPage
{
	DECLARE_DYNCREATE(CYX_PROP_DIS)

// Construction
public:
	void ShowRect(BOOL txt);
	void Hide2();
	void Hide1();
	void Hide0();
	void Show2vec();
	void Show1vec();
	void Show0vec();
	void Show2Bmp();
	void Show1Bmp();
	void Show0Bmp();
	void Show2Txt();
	void Show1Txt();
	void Show0Txt();
	void HideAll();


	CRTUColorStatic	m_veccolorstatic0;
	CRTUColorStatic	m_veccolorstatic1;
	CRTUColorStatic	m_veccolorstatic2;
	CRTUColorStatic	m_txtcolorstatic0;
	CRTUColorStatic	m_txtcolorstatic1;
	CRTUColorStatic	m_txtcolorstatic2;

	CYX_PROP_DIS(CString text0="0状态表示形式：",CString text1="1状态表示形式：",CString textdis="无效状态表示形式：",BOOL canseefla=true);
	~CYX_PROP_DIS();
	LOGFONT m_lfont;
	COLORREF m_veccolor0,m_veccolor1,m_veccolor2;
	COLORREF m_txtcolor0,m_txtcolor1,m_txtcolor2;
	COLORREF m_transcolor0,m_transcolor1,m_transcolor2;
	BOOL trans0,trans1,trans2;
	char m_bmpname0[33],m_bmpname1[33],m_bmpname2[33];
	char m_vecname0[33],m_vecname1[33],m_vecname2[33];
	char m_strname0[33],m_strname1[33],m_strname2[33];
	BYTE m_pickind0,m_pickind1,m_pickind2;
    float x0,y0,fontwidth,fontheight,fontjj;
	int m_diskind0,m_diskind1,m_diskind2;

	void GetPara(LOGFONT * lfont,COLORREF * color0,COLORREF * color1,COLORREF * colordis,
		char * picname0,char * picname1,char * picnamedis,BYTE * pickind0,BYTE * pickind1,
		BYTE * pickinddis,int *diskind0,int *diskind1,int *diskinddis,float *xx0,float *yy0,float *fontw,float *fonth,float *fontj);
	void SetPara(LOGFONT lfont,COLORREF color0,COLORREF color1,COLORREF colordis,
		char * picname0,char * picname1,char * picnamedis,BYTE pickind0,BYTE pickind1,
		BYTE pickinddis,int diskind0,int diskind1,int diskinddis,float xx0,float yy0,float fontw,float fonth,float fontj);
// Dialog Data
	//{{AFX_DATA(CYX_PROP_DIS)
	enum { IDD = CZ_IDD_YX_DIS };
	CButton	m_main2;
	CButton	m_main1;
	CButton	m_main0;
	CEdit	m_editcontrolzj;
	CButton	m_buttonfont;
	CStatic	m_staticzj;
	CStatic	m_staticw;
	CStatic	m_statich;
	CEdit	m_editbmp2;
	CEdit	m_editbmp1;
	CStatic	m_staticvec2;
	CStatic	m_staticvec1;
	CStatic	m_staticvec0;
	CStatic	m_statictxtc1;
	CStatic	m_statictxtc2;
	CStatic	m_statictxtc0;
	CStatic	m_statictxt2;
	CStatic	m_statictxt1;
	CStatic	m_statictxt0;
	CStatic	m_staticbmp2;
	CStatic	m_staticbmp1;
	CStatic	m_staticbmp0;
	CEdit	m_editvec2;
	CEdit	m_editvec1;
	CEdit	m_editvec0;
	CEdit	m_edittxt2;
	CEdit	m_edittxt1;
	CEdit	m_edittxt0;
	CEdit	m_editbmp0;
	CButton	m_checkflash2;
	CButton	m_checkflash1;
	CButton	m_checkflash0;
	CButton	m_checkfc2;
	CButton	m_checkfc1;
	CButton	m_checkfc0;
	CButton	m_buttonvecs2;
	CButton	m_buttonvecs1;
	CButton	m_buttonvecs0;
	CButton	m_buttonbmps2;
	CButton	m_buttonbmps1;
	CButton	m_buttonbmps0;
	float	m_editfh;
	float	m_editfw;
	float	m_editx;
	float	m_edity;
	float	m_editjj;
	BOOL	m_checkvaluefc0;
	BOOL	m_checkvaluefc1;
	BOOL	m_checkvaluefc2;
	BOOL	m_checkvalueflash0;
	BOOL	m_checkvalueflash1;
	BOOL	m_checkvalueflash2;
	int		m_radio0;
	int		m_radio1;
	int		m_radio2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CYX_PROP_DIS)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_distext0,m_distext1,m_distextdis;
	BOOL canseeflash;
	// Generated message map functions
	//{{AFX_MSG(CYX_PROP_DIS)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnIdcButtonfont();
	afx_msg void OnIdcRadiotxt0();
	afx_msg void OnIdcRadiobmp0();
	afx_msg void OnIdcRadiovec0();
	afx_msg void OnIdcRadiotxt1();
	afx_msg void OnIdcRadiobmp1();
	afx_msg void OnIdcRadiovec1();
	afx_msg void OnIdcRadiotxt2();
	afx_msg void OnIdcRadiobmp2();
	afx_msg void OnIdcRadiovec2();
	afx_msg void OnIdcButtonbmps0();
	afx_msg void OnIdcButtonbmps1();
	afx_msg void OnIdcButtonbmps2();
	afx_msg void OnIdcButtonvecs0();
	afx_msg void OnIdcButtonvecs1();
	afx_msg void OnIdcButtonvecs2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeIdcEdittxt0();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YX_PROP_DIS_H__90BC989A_374C_41E9_B6DA_9E4179DA9EF8__INCLUDED_)
