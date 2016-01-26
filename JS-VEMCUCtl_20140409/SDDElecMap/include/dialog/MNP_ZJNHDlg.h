#pragma once

#include "afxwin.h"
#include "../../Resource.h"

// CMNP_ZJNHDlg 对话框

class CMNP_ZJNHDlg : public CDialog
{
	DECLARE_DYNAMIC(CMNP_ZJNHDlg)

public:
	CMNP_ZJNHDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMNP_ZJNHDlg();

// 对话框数据
	enum { IDD = IDD_MNP_ZJNH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	BOOL Event1_Enable;
	BOOL Event2_Enable;
	BOOL Event3_Enable;
	BOOL Event4_Enable;
	BOOL Event5_Enable;
	BOOL Event6_Enable;
	BOOL Event7_Enable;
	BOOL Event8_Enable;
	BOOL Event9_Enable;
	BOOL Event10_Enable;
	BOOL Event11_Enable;
	BOOL Event12_Enable;
	BOOL Event13_Enable;
	BOOL Event14_Enable;
	BOOL Event15_Enable;
	BOOL Event16_Enable;
	BOOL Event17_Enable;

public:
	void SetEvent1Enable(BOOL Enable);
	void SetEvent2Enable(BOOL Enable);
	void SetEvent3Enable(BOOL Enable);
	void SetEvent4Enable(BOOL Enable);
	void SetEvent5Enable(BOOL Enable);
	void SetEvent6Enable(BOOL Enable);
	void SetEvent7Enable(BOOL Enable);
	void SetEvent8Enable(BOOL Enable);
	void SetEvent9Enable(BOOL Enable);
	void SetEvent10Enable(BOOL Enable);
	void SetEvent11Enable(BOOL Enable);
	void SetEvent12Enable(BOOL Enable);
	void SetEvent13Enable(BOOL Enable);
	void SetEvent14Enable(BOOL Enable);
	void SetEvent15Enable(BOOL Enable);
	void SetEvent16Enable(BOOL Enable);
	void SetEvent17Enable(BOOL Enable);
	BOOL GetEvent1Enable(void);
	BOOL GetEvent2Enable(void);
	BOOL GetEvent3Enable(void);
	BOOL GetEvent4Enable(void);
	BOOL GetEvent5Enable(void);
	BOOL GetEvent6Enable(void);
	BOOL GetEvent7Enable(void);
	BOOL GetEvent8Enable(void);
	BOOL GetEvent9Enable(void);
	BOOL GetEvent10Enable(void);
	BOOL GetEvent11Enable(void);
	BOOL GetEvent12Enable(void);
	BOOL GetEvent13Enable(void);
	BOOL GetEvent14Enable(void);
	BOOL GetEvent15Enable(void);
	BOOL GetEvent16Enable(void);
	BOOL GetEvent17Enable(void);
	virtual BOOL OnInitDialog();
	CButton m_Check1;
	CButton m_Check2;
	CButton m_Check3;
	CButton m_Check4;
	CButton m_Check5;
	CButton m_Check6;
	CButton m_Check7;
	CButton m_Check8;
	CButton m_Check9;
	CButton m_Check10;
	CButton m_Check11;
	CButton m_Check12;
	CButton m_Check13;
	CButton m_Check14;
	CButton m_Check15;
	CButton m_Check16;
	CButton m_Check17;
	afx_msg void OnBnClickedOk();

	
};
