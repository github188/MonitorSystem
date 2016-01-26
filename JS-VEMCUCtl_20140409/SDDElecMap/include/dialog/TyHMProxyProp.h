#if !defined(AFX_HMPROXYPROP_H__5231BBD8_F910_4131_BE8F_07C172CAA4AA__INCLUDED_)
#define AFX_HMPROXYPROP_H__5231BBD8_F910_4131_BE8F_07C172CAA4AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TyHMProxyProp.h : header file
//
//#include "sp_draw.h"
#include "../../resource.h"
#include "canvas/LineProp.h"

/////////////////////////////////////////////////////////////////////////////
// CTyHMProxyProp dialog

class CTyHMProxyProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CTyHMProxyProp)

// Construction
public:
	void GetProp(char* hmname,int& display,BOOL& bFlash,char* str1,char* str2,COLORREF&  color1,COLORREF&  color2,BOOL&   bFangXiang,float& fTextHeight,float& fTextWidth,float& fJqWidth,char* TextFont,BOOL& bTranslate1,BOOL& bTranslate2);
	void SetProp(char hmname[33],int display,BOOL bFlash,char str1[33],char str2[33],COLORREF color1,COLORREF color2,BOOL  bFangXiang,float fTextHeight,float fTextWidth,float fJqWidth,char TextFont[16],BOOL bTranslate1,BOOL bTranslate2);
	CTyHMProxyProp();
	~CTyHMProxyProp();

// Dialog Data
	//{{AFX_DATA(CTyHMProxyProp)
	enum { IDD = IDD_HM_PROXY };
	CString	m_hmname;
	int		m_display;
	BOOL	m_bFlash;
	CString	m_strBmp1;
	CString	m_strBmp2;
	CString	m_strVect1;
	CString	m_strText1;
	CString	m_strText2;
	CString	m_strVect2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTyHMProxyProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  COLORREF m_TranslateColor1;
  COLORREF m_TranslateColor2;


	COLORREF m_VectColor1;
	COLORREF m_VectColor2;
  CStaticColor  m_ctrlVectColor1;
  CStaticColor  m_ctrlVectColor2;

	COLORREF m_TextColor1;
	COLORREF m_TextColor2;
  CStaticColor  m_ctrlTextColor1;
  CStaticColor  m_ctrlTextColor2;

  //文字表示时特殊的字段
	BOOL  m_bFangXiang;				//标注角度
														//  TRUE-横排  FALSE-竖排
	float m_TextHeight;				//字体高度
	float m_TextWidth;				//字体宽度
	float m_JqWidth;					//间隔宽度
	char	m_TextFont[16];			//字体

  //位图
	BOOL	m_bTranslate1;		//正常显示的位图背景是否透明
	BOOL	m_bTranslate2;		//异常显示的位图背景是否透明

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTyHMProxyProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnTextColor1();
	afx_msg void OnTextColor2();
	afx_msg void OnVectColor1();
	afx_msg void OnVectColor2();
	afx_msg void OnFont();
	afx_msg void OnBmp1();
	afx_msg void OnBmp2();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnVect1();
	afx_msg void OnVect2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitHMList();
	void UpdateUI();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMPROXYPROP_H__5231BBD8_F910_4131_BE8F_07C172CAA4AA__INCLUDED_)
