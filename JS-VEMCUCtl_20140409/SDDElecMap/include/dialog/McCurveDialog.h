#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "../../resource.h"
#include "canvas/mccurvedata.h"
#include "canvas/LineProp.h"
#include "canvas/precitionedit.h"

// CMcCurvePage1 对话框

class CMcCurvePage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMcCurvePage1)

public:
	CMcCurvePage1();
	virtual ~CMcCurvePage1();

// 对话框数据
	enum { IDD = IDD_MCCURVEPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMcCurveData *pmccurvedata;
	int m_nIprecision;
	int m_nVprecision;
	float m_fLeftcoord;
	float m_fTopcoord;
	float m_fXlen;
	float m_fYlen;
	float m_fImin;
	float m_fImax;
	float m_fVmin;
	float m_fVmax;
	BYTE m_nNode;
	BYTE m_nLine;
	int m_nRtu;
	BOOL m_bGt;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	CPrecitionEdit m_IminEdit;
	CPrecitionEdit m_ImaxEdit;
	CPrecitionEdit m_VminEdit;
	CPrecitionEdit m_VmaxEdit;
	afx_msg void OnBnClickedButton13();
	CString m_strmcename;
	BOOL m_bonlyrealline;
	virtual BOOL OnKillActive();
	CString m_UpName;
	CString m_DownName;
	BOOL m_realtime_style;
	afx_msg void OnBnClickedCheck2();
};

#pragma once
#include "canvas/lineprop.h"
#include "canvas/colorlist.h"
// CMcCurvePage2 对话框
//typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);
class CMcCurvePage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMcCurvePage2)

public:
	CMcCurvePage2();
	virtual ~CMcCurvePage2();

// 对话框数据
	enum { IDD = IDD_MCCURVEPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//ShowFindDotDlg *pShowFindDot;
	void SetList(CURVEARRAY*);
	CMcCurveData *pmccurvedata;
	TCurve* GetCurSingleLine(int);
public:
	int editindex;
	int nCurveSel;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton11();
	BYTE m_BchannelNO;
	BYTE m_BDot;
	int m_BPrecision;
	float m_fRatio;
	COLORREF m_nColor;
	afx_msg void OnBnClickedButton2();
	CColorList m_CurveList;
	virtual BOOL OnInitDialog();
public:
	void DrawListTitle();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox ModeBox;
	CStaticColor m_staticcolor;
	afx_msg void OnStnClickedColorstatic();
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedButton3();
	BYTE m_nlinenode;
	BYTE m_nlineline;
	BYTE m_nlinertu;
	virtual void OnOK();
	// 简称
	CString m_unit;
};

// CMcCurveSheet

class CMcCurveSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMcCurveSheet)

public:
	CMcCurveData *pmccurvedata;
	CMcCurveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMcCurveSheet();
	CMcCurvePage1 mccurvepage1;
	CMcCurvePage2 mccurvepage2;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};


