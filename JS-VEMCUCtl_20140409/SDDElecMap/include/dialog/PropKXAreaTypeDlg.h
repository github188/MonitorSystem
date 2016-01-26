#pragma once
#include "../../resource.h"
#include "canvas/lineprop.h"

// CPropKXAreaTypeDlg 对话框

class CPropKXAreaTypeDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropKXAreaTypeDlg)

public:
	CPropKXAreaTypeDlg();
	virtual ~CPropKXAreaTypeDlg();

// 对话框数据
	enum { IDD = IDD_PROP_KXAREA_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 带电位图
	CString m_strBmpName1;
	// 失电位图
	CString m_strBmpName2;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual BOOL OnInitDialog();
private:
	CRTUColorStatic m_VCol1 ;
	CRTUColorStatic m_VCol2 ;

	COLORREF	colV1 ;
	COLORREF	colV2 ;

private:
	void EnableControl(bool bVector);

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void GetParam(COLORREF* col1 ,COLORREF* col2 ,char* bmpName1 ,char* bmpName2);
	void SetParam(COLORREF col1 ,COLORREF col2 ,char* bmpName1 ,char* bmpName2);
	virtual void OnOK();
};
