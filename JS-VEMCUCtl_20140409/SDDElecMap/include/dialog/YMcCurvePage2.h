#pragma once
#include "../../resource.h"
#include "canvas/colorlist.h"
#include "canvas/lineprop.h"

#define YC_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"

struct ymcurve_info
{
	
	char ename[17] ;//对应累加量英文名
	char cname[9] ;//中文名
	DWORD color ;

};
typedef CArray<ymcurve_info,ymcurve_info> CurveInfo;
 
// CYMcCurvePage2 对话框
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

class CYMcCurvePage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CYMcCurvePage2)

public:
	CYMcCurvePage2();
	virtual ~CYMcCurvePage2();

// 对话框数据
	enum { IDD = IDD_YMCURVEPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CNewColorList m_CurveList;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();

public:
	int m_nEditIndex ;
	CString m_ename;
	CString m_cname;
	CStaticColor m_stColor;
	COLORREF m_nColor;
	CurveInfo curvearray ;
	
public:
	void SetList(CurveInfo*);
	ymcurve_info* GetCurSingleLine(int);
	void DrawListTitle();


	
	afx_msg void OnStnClickedStaticColor();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
