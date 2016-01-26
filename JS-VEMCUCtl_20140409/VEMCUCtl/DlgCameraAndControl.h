#pragma once
#include "resource.h"

#include "DlgResizeHelper.h"
#include "afxwin.h"
#include "afxcmn.h"

class CViewChannel;

#define OM_MESSAGE_CAMERA_CONTROL (WM_USER + 301)
// CDlgCameraAndControl 对话框

class CDlgCameraAndControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraAndControl)

public:
	CDlgCameraAndControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCameraAndControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAMERA_AND_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DlgResizeHelper m_resizeHelper;

	DECLARE_MESSAGE_MAP()
private:
	int m_nOpenCameraId;	//打开的视频ID
	bool m_bOpenFlag;//当前是否打开了摄像头
	ASS_CAMERA_INFO m_tAssCameraInfo;//成员变量
	int m_nSpeed;//云台速度
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OpenCamera(char *szCameraCode);
	void CloseCamera(int nID);
	void SetCameraOpenStatus(bool bFlag);
	CStatic m_staticCameraWnd;
	afx_msg void OnBnClickedButtonUp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnStartYTControl(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT GetAlarmInfoByStationIdHandle(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClickDlgCameraAndControlCloseVideo();
	void ProcessRClickCamera();
	void PopClickCameraRightMenu();
	void SetControlBackgroundColor();
	void InitCtrl();
	CComboBox m_comboSpeed;
	afx_msg void OnCbnSelchangeComboSpeed();
	CViewChannel *m_pViewChannel;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	CString SpellCondition(int nStationId);
	void AddAlarmInfoToList(std::vector<ACS_ALARM_INFO> vecAlarmInfo);
	CListCtrl m_lstAlarm;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
