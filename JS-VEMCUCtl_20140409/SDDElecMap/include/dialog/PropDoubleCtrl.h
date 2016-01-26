#pragma once
#include "../../resource.h"
// CPropDoubleCtrl 对话框

class CPropDoubleCtrl : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropDoubleCtrl)

public:
	CPropDoubleCtrl();
	virtual ~CPropDoubleCtrl();

// 对话框数据
	enum { IDD = ST_IDD_ZJYOUGONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sCtrlName;
	CString m_sOprtName1;
	CString m_sOprtName2;
	CString m_sYKName1;
	CString m_sYKName2;
	int m_nActType1;
	int m_nActType2;
	BOOL m_bOnceQuit;

public:
	void SetParam(char ctrlname[], char oprtname1[], char ykname1[], int acttype1, char oprtname2[], char ykname2[], int acttype2, bool oncequit);
	void GetParam(char ctrlname[], char oprtname1[], char ykname1[], int &acttype1, char oprtname2[], char ykname2[], int &acttype2, bool &oncequit);
	afx_msg void OnBnClickedSelyk1();
	afx_msg void OnBnClickedSelyk2();
};
