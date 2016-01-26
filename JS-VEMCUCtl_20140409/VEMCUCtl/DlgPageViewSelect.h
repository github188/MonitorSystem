#pragma once
#include "afxwin.h"
#include "SkinButton.h"


// CDlgPageViewSelect 对话框

class CDlgPageViewSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgPageViewSelect)

public:
	CDlgPageViewSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPageViewSelect();

public:
	BOOL LoadSkin();

public:
	int m_nPosType;
	CString m_strPreStation;
	CString m_strCurrentStation;
	CString m_strFirstStation;

public:
	CSkinButton m_btnStart;
	CSkinButton m_btnCancel;
	CSkinButton m_btnExit;

	CBitmap  m_Bmp;
	BITMAP   m_bm;

	CBrush    m_BgBrush;

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGEVIEW_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
