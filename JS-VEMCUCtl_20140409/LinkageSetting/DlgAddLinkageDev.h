#pragma once
#include "stdafx.h"
#include "afxwin.h"

// CDlgAddLinkageDev 对话框

class CDlgAddLinkageDev : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddLinkageDev)

public:
	CDlgAddLinkageDev(CWnd* pParent = NULL);   // 标准构造函数
	CDlgAddLinkageDev(CWnd* pParent, _T_DEV_INFO_ *pDevInfo);	
	virtual ~CDlgAddLinkageDev();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_LINKAGE_DEV };

private:
	_T_DEV_INFO_* m_pDevInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitCtrl();
	void InitComboLinkageDev(char *szLinkageTypeText);
	void InitCameraInfo(int nStationId);
	void InitComboLinkageType();
	void InitDoorInfo(int nStationId);
	CComboBox m_comboLinkageType;
	CComboBox m_comboLinkageDevName;
	CComboBox m_comboLinkageAction;
	CComboBox m_comboPresetNum;
	afx_msg void OnCbnSelchangeComboLinkageType();
	void InitComboLinkageAction(char *szLinkageTypeText);
	afx_msg void OnCbnSelchangeComboLinkageAction();
	void InitComboPresetNum(int nIndex);
	afx_msg void OnBnClickedOk();
	bool CheckCondition();
	void SpellFormInfo(_T_LINK_CONFIG_INFO_ *pLinkConfigInfo);
	bool InsertLinkageInfoToDB(_T_LINK_CONFIG_INFO_ *pLinkConfigInfo);
	afx_msg void OnCbnSelchangeComboLinkageDevName();
};
