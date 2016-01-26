#pragma once
#include "../../resource.h"
#include "canvas/rtucolorstatic.h"
#define YC_FINDDOTDLLNAME "c:\\sunpac\\dll\\Finddot.dll"


// CSZ_YcYwDlg 对话框
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

class CSZ_YcYwDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CSZ_YcYwDlg)

public:
	CSZ_YcYwDlg();
	virtual ~CSZ_YcYwDlg();

// 对话框数据
	enum { IDD = SZ_IDD_YCYW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CRTUColorStatic m_color;
	CRTUColorStatic m_disablecolor;
	CRTUColorStatic m_hilevelcolor;
	CRTUColorStatic m_lwlevelcolor;
	CString m_ename;
	float m_max;
	float m_min;
	int m_drawtype;
	COLORREF m_pcolor;
	COLORREF m_pdisablecolor;
	COLORREF m_philevelcolor;
	COLORREF m_plwlevelcolor;
	CString m_pename;
	float m_pmax;
	float m_pmin;
	BYTE m_pdrawtype;
public:
	void SetPara(COLORREF color,COLORREF disablecolor,COLORREF hilevelcolor,COLORREF lwlevelcolor,char* ename,float max,float min,BYTE drawtype);
	void GetPara(COLORREF* color,COLORREF* disablecolor,COLORREF* hilevelcolor,COLORREF* lwlevelcolor,char* ename,float* max,float* min,BYTE* drawtype);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedIdcButtonFinddot();
};
