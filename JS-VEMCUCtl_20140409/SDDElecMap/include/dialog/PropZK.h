#pragma once
#include "../../resource.h"
// CPropZK 对话框

class CPropZK : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropZK)

public:
	CPropZK();
	virtual ~CPropZK();

// 对话框数据
	enum { IDD = IDD_ZK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_zkename;
	afx_msg void OnBnClickedButton2();
	void SetYkParam(char Ykname[],int node,int line,int rtu);
	void GetYkParam(char Ykname[],int* node,int* line,int* rtu);

	int m_node;
	int m_line;
	int m_rtu;
};
