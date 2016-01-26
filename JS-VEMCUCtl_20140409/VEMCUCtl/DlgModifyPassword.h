#pragma once
#include "afxwin.h"
#include "SkinButton.h"


// CDlgModifyPassword 对话框

class CDlgModifyPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyPassword)

public:
	CDlgModifyPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModifyPassword();

public:
	BOOL LoadSkin();
	BOOL InitControl();

public:
	CString m_strUserName;
	CString m_strUserPassword;
	CString m_strUserNewPassword;
	CString m_strUserNewPassword2;

public:
	CEdit m_editUser;
	CEdit m_editPassword;
	CEdit m_editNewPassword;
	CEdit m_editNewPassword2;
	CSkinButton m_btnModify;
	CSkinButton m_btnCancel;
	CButton m_btnExit;

public:
	CBrush    m_BgBrush;
	CEnBitmap  m_Bmp;
	BITMAP   m_bm;

// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFY_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditUser();
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnEnChangeEditNewPassword();
	afx_msg void OnEnChangeEditNewPassword2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
