#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SkinButton.h"


// CDlgSelectUser 对话框

class CDlgSelectUser : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectUser)

public:
	CDlgSelectUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelectUser();

public:
	BOOL LoadSkin();
	BOOL InitControl();

public:
	CString m_strUserName;
	
public:
	CTreeCtrl m_treeUser;
	CSkinButton m_btnOK;
	CSkinButton m_btnExit;

public:
	CBitmap  m_Bmp;
	BITMAP   m_bm;

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnNMDblclkTreeUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnExit2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
