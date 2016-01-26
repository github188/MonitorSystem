#pragma once
#include "../../resource.h"
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);
// CPropSoftRepair 对话框

class CPropSoftRepair : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropSoftRepair)

public:
	CPropSoftRepair();
	virtual ~CPropSoftRepair();

// 对话框数据
	enum { IDD = IDD_PROP_SOFTPAIR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strDes;
	CString m_strEname;
	ShowFindDotDlg *pShowFindDot;
public:
	afx_msg void OnBnClickedButton1();
public:
	void GetParam(char* ename ,char* des);
	void SetParam(char* ename ,char* des) ;
};
