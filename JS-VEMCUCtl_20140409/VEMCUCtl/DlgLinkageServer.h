#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "SkinButton.h"
#include "VMListCtrl.h"


// CDlgLinkageServer 对话框

class CDlgLinkageServer : public CDialog
{
	DECLARE_DYNAMIC(CDlgLinkageServer)

public:
	CDlgLinkageServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLinkageServer();

public:
	BOOL LoadSkin();
	BOOL InitControl();
	BOOL HideLinkageServer();
	BOOL ReadAndSetRelationListInfo(char *szLinkageTime);
	BOOL ReadAndSetCameraRelationListInfo(int nNodeId,char *szStationName,char *szLinkageTime);
	int  GetRelationListInfoId(char *szLinkageType,char *szStationName,char *szDeviceName,char *szSubType,char *szLinkageTime);
	BOOL ReadAndSetRelationListInfo2(char *szLinkageTime);
	BOOL ReadAndSetCameraRelationListInfo2(int nNodeId,char *szStationName,char *szLinkageTime);
	int  GetRelationListInfoId2(char *szLinkageType,char *szStationName,char *szDeviceName,char *szSubType,char *szLinkageTime);
	BOOL ReadRelateActionIni();
	BOOL DeleteRelationListWeatherInfo(char *szLinkageType,char szStationName,char *szDeviceName,char *szSubType);

	void   CameraRelationInsertCameraInfoByListRelation();


public:
	BOOL m_bRelationListExtendFlag;// 显示的列表是大的还是小的

public:
	CVMListCtrl m_listRelation;
	CVMListCtrl m_listCameraRelation;
	CSkinButton m_btnHideLinkageServer;
	CSkinButton m_btnExtend;

public:
	CBitmap   m_LinkageServerBmp;
	CBrush	m_Brush;

	CBitmap   m_LeftBmp;

	BITMAP    m_LeftBm;


// 对话框数据
	enum { IDD = IDD_DIALOG_LINKAGE_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListRelation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListCameraRelation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListRelation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnControlButton(UINT nID);
	afx_msg void OnBnClickedBtnExtend();
	CButton m_CheckRelateVideo;
	CButton m_CheckRelateAccident;
	CButton m_CheckRelateAbnormal;
	CButton m_CheckRelateState;
	CButton m_CheckRelateSwitchState;
	CButton m_CheckRelateLimit;
	CButton m_CheckRelateNotify;
	CButton m_CheckRelateMetroAlarm;
	CButton m_CheckRelateMetroPre;
	CButton m_CheckRelateTyphoonPre;
	CButton m_CheckRelateTyphoonAlarm;
	CButton m_CheckRelateThunderPre;
	CButton m_CheckRelateThunderAlarm;
	afx_msg void OnBnClickedCheckRelateVideo();
	afx_msg void OnBnClickedCheckRelateAccident();
	afx_msg void OnBnClickedCheckRelateAbnormal();
	afx_msg void OnBnClickedCheckRelateState();
	afx_msg void OnBnClickedCheckRelateSwitchstate();
	afx_msg void OnBnClickedCheckRelateLimit();
	afx_msg void OnBnClickedCheckRelateNotify();
	afx_msg void OnBnClickedCheckRelateMetroAlarm();
	afx_msg void OnBnClickedCheckRelateMetroPre();
	afx_msg void OnBnClickedCheckRelateTyphoonPre();
	afx_msg void OnBnClickedCheckRelateTyphoonAlarm();
	afx_msg void OnBnClickedCheckRelateThunderPre();
	afx_msg void OnBnClickedCheckRelateThunderAlarm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
