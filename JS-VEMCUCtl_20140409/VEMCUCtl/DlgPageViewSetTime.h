#pragma once
#include "afxwin.h"
#include "SkinButton.h"


// CDlgPageViewSetTime 对话框

class CDlgPageViewSetTime : public CDialog
{
	DECLARE_DYNAMIC(CDlgPageViewSetTime)

public:
	CDlgPageViewSetTime(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPageViewSetTime();

public:
	BOOL LoadSkin();

public:
	int m_nTimeValue;

	CSkinButton m_btnSet;
	CSkinButton m_btnCancel;
	CSkinButton m_btnExit;
	CBitmap  m_Bmp;
	BITMAP   m_bm;


// 对话框数据
	enum { IDD = IDD_DIALOG_PAGE_VIEW_SET_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedBtnExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
