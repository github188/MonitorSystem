#pragma once
#include "afxwin.h"


// CDlgDeviceRelationCamera 对话框

class CDlgDeviceRelationCamera : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeviceRelationCamera)

public:
	CDlgDeviceRelationCamera(CWnd* pParent = NULL);   // 标准构造函数
	CDlgDeviceRelationCamera(int nDevId, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDeviceRelationCamera();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEVICE_RELATION_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int m_nDevId;
public:
	CString m_strCameraName;
	char m_szCameraName[64];
	virtual BOOL OnInitDialog();
	CComboBox m_comboCamera;
	afx_msg void OnBnClickedOk();
	void GetRelationCameraInfoByDevId(int nDevId);
};
