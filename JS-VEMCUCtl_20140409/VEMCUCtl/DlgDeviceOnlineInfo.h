#pragma once
#include "afxwin.h"
#include "SkinButton.h"
#include "XProgressCtrl.h"
#include "afxcmn.h"


//////////////////////////////////////////////////////////////////////////
#define  WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE  (WM_USER+1300) //设备在线信息更新消息

// CDlgDeviceOnlineInfo 对话框

class CDlgDeviceOnlineInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeviceOnlineInfo)

public:
	CDlgDeviceOnlineInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDeviceOnlineInfo();

public:
	BOOL LoadSkin();
	BOOL InitControl();
	BOOL ClearDisplayInfo();
	BOOL SetAndShowOnlineInfo(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum);
	BOOL SetAndShowOnlineInfo2(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum);
	BOOL SetAndShowOnlineInfo500KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum);
	BOOL SetAndShowOnlineInfo220KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum);
	BOOL SetAndShowOnlineInfo110KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum);

public:
	CBitmap   m_Bmp;

public:
	CString m_strProblemCameraNum;
	CString m_strAllCameraNum;
	CString m_strCameraOnlineRate;
	CString m_strOffStationNum;
	CString m_strAllStationNum;
	CString m_strStationOnlineRate;

	CString m_strProblemCameraNum2;
	CString m_strAllCameraNum2;
	CString m_strCameraOnlineRate2;
	CString m_strOffStationNum2;
	CString m_strAllStationNum2;
	CString m_strStationOnlineRate2;

	CString m_strProblemCameraNum_500KV;
	CString m_strAllCameraNum_500KV;
	CString m_strCameraOnlineRate_500KV;
	CString m_strOffStationNum_500KV;
	CString m_strAllStationNum_500KV;
	CString m_strStationOnlineRate_500KV;

	CString m_strProblemCameraNum_220KV;
	CString m_strAllCameraNum_220KV;
	CString m_strCameraOnlineRate_220KV;
	CString m_strOffStationNum_220KV;
	CString m_strAllStationNum_220KV;
	CString m_strStationOnlineRate_220KV;

	CString m_strProblemCameraNum_110KV;
	CString m_strAllCameraNum_110KV;
	CString m_strCameraOnlineRate_110KV;
	CString m_strOffStationNum_110KV;
	CString m_strAllStationNum_110KV;
	CString m_strStationOnlineRate_110KV;

public:
	CSkinButton m_btnOk;
	CComboBox m_comboType;
	CXProgressCtrl m_progressRateInfo;

// 对话框数据
	enum { IDD = IDD_DIALOG_DEVICE_ONLINE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOk();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCbnSelchangeComboType();
	LRESULT OnDeviceOnlineRateInfoUpdateMessageHandler(WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////
extern CDlgDeviceOnlineInfo *g_pDlgDeviceOnlineInfo;
