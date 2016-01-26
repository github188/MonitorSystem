#pragma once

#include "afxwin.h"
#include <afxcmn.h>
#include "canvas/ListBase.h"
#include <Shlwapi.h>
#include "canvas/LineProp.h"
#include "canvas/colorlist.h"
#include "canvas/PropYcCurveOther.h"
#include "../../resource.h"
#define YC_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"

// CYcCurvePage 对话框

struct ycurve_info
{
	
	char ename[17] ;//对应累加量英文名
	char cname[33] ;//中文名
	DWORD color ;

};
typedef CArray<ycurve_info,ycurve_info> YcCurveInfo;
 
// CYMcCurvePage2 对话框
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);
class CYcCurvePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CYcCurvePage)

public:
	CYcCurvePage();
	virtual ~CYcCurvePage();

// 对话框数据
	enum { IDD = IDD_YCCURVEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char Ename[17];
	char Cname[33];
	float m_fMin;
	float m_fMax;
	int curvewide;
	DWORD curvecolor;
	int count;
	int m_nEditIndex ;
    float m_RfMin;
	float m_rfMax;
	int curvetype;
	COLORREF m_nColor;
    //CStaticColor m_stColor;
	
	//CNewColorList m_CList;
	CListBase m_CList;
    YcCurveInfo curvearray ;
	void DrawListTitle();
	virtual BOOL OnInitDialog();
	CString m_Ename;
	CString m_Cname;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStaticColor m_StaticColor;
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	void SetList(YcCurveInfo *pCurvearray);
	ycurve_info* GetCurSingleLine(int singleline);
	
	afx_msg void OnStnClickedStaticColor1();
	afx_msg void OnBnClickedAddp();
	afx_msg void OnBnClickedDeletep();
	//CButton m_haveplan;
	//CButton m_showplan;
	
};
#pragma once


// CYcCurvePage1 对话框
class CYcCurvePage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CYcCurvePage1)

public:
	CYcCurvePage1();
	virtual ~CYcCurvePage1();

// 对话框数据
	enum { IDD = IDD_YCCURVEPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_xfmin;
	float m_yfmin;
	float m_fheight;
	float m_xfwide;
};
#pragma once


// CYcCurvePage2 对话框

class CYcCurvePage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CYcCurvePage2)

public:
	CYcCurvePage2();
	virtual ~CYcCurvePage2();

// 对话框数据
	enum { IDD = IDD_YCCURVEPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_tyName;
	float m_fMin;
	float m_fMax;
	float m_ykdu;
	CString m_ydanwei;
};
#pragma once



// CYcCurveSheet

class CYcCurveSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CYcCurveSheet)

public:
	CYcCurveSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CYcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CYcCurveSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CYcCurvePage Page1;
	CYcCurvePage1 Page2;
	CYcCurvePage2 Page3;
	CPropYcCurveOther Page4;


};






