#pragma once
#include "afxwin.h"

// CDlgAddDeviceLedger 对话框

class CDlgAddDeviceLedger : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddDeviceLedger)

public:
	CDlgAddDeviceLedger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddDeviceLedger();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_DEVICE_LEDGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	char m_device_name[32];
	char m_device_manufacturer[32];
	char m_device_ip[32];
	int m_device_port;
	char m_login_username[32];
	char m_login_password[32];
	char m_protocol_type[32];
	char m_device_type[32];
	char m_harddisk_capacity[32];
	int m_analog_video_num;
	int m_net_video_num;
	int m_nStationId;
private:
	int m_nDeviceId;
	_T_ASS_DEVICE_LEDGER_ m_tDeviceLedger;
public:
	CComboBox m_comboDevFactory;
	afx_msg void OnBnClickedOk();
	void InitComBox();
	void InitControl();
	int SaveDeviceLedger();
	int EditDeviceLedger(int nDevId);
	CEdit m_edtDevName;
	CEdit m_edtDevIp;
	CEdit m_edtDevPort;
	CEdit m_edtDevUser;
	CEdit m_edtDevPassword;
	CEdit m_edtDevProcotol;
	CEdit m_edtDevType;
	CEdit m_edtDevHarddish;
	CEdit m_edtDevAVNum;
	CEdit m_edtDVNum;
	afx_msg void OnBnClickedCancel();
	void SetDeviceLedger(_T_ASS_DEVICE_LEDGER_* pDeviceLedger);
	void InitControlData();
};
