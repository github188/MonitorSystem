#pragma once
#include "stdafx.h"
#include "resource.h"
#include "DlgLinkageConfig.h"
#include "DlgPresetSetting.h"
#include "DlgPtuManage.h"
#include "DlgDeviceLedger.h"
#include "DlgDeviceTypeConfig.h"
#include "afxcmn.h"
#include "afxwin.h"


// CPtuDlg 对话框

class CPtuDlg : public CDialog
{
	DECLARE_DYNAMIC(CPtuDlg)

public:
	CPtuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPtuDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PTU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDlgPresetSetting* m_pPresetSettingDlg;
	CDlgPtuManage* m_pPtuManageDlg;
	CDlgDeviceLedger* m_pDeviceLedgerDlg;
	CDlgLinkageConfig* m_pLinkageConfigDlg;
	CDlgDeviceTypeConfig* m_pDeviceTypeConfigDlg;
	CRect m_WndRect;
	CImageList m_ilTreeIcons;
	CBitmap* m_pTitleBmp;
	CBitmap* m_pBackBmp;
public:
	void InitChildWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPresetConfig();
	CTreeCtrl m_treeList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitStationList();
	void SetStationNodeInfo(HTREEITEM hItem,int station_id,char* station_code,char* station_name,int node_type,int connect_state);
	BOOL InitLocalAllDBTable();
	bool ConnectMySql(void);
	bool DisConnectMySql(void);
	BOOL InitDVR();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPtuConfig();
	afx_msg void OnNMRClickTreeStationList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuViewPtuInfo();
	afx_msg void OnNMDblclkTreeStationList(NMHDR *pNMHDR, LRESULT *pResult);
	void FreeStationList(HTREEITEM hItem);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitControlSkin();
	afx_msg void OnBnClickedButtonDeviceLedgerManage();
	BOOL GetUserInfoByUserName(char *szUserName);
	CButton m_btnPtuConfig;
	afx_msg void OnBnClickedButtonLinkageConfig();
	void ShowOneDlg(int nId);
	afx_msg void OnBnClickedButtonDeviceTypeConfig();
	afx_msg void OnBnClickedButtonSearch();
	CString GetStationTreeShowText(int nFlag, CString strStationName);
	void SetVolateClassStationToTree(char* szVolClass, HTREEITEM hItem);
};

void ReadStationNode();
void ReadStationAccessDev();
void ReadStationCamera();
void ReadStationYK();
void ReadStationYT();
void ReadStationYX();
void ReadStationYC();
void ReadStationObCard();
void ReadStationCameraPreset();
void ReadStationLinkageRelation();
void ReadStationCtUser();

void ReadRVUInfo();
void ReadRVUStationByUserID(int user_id);
void ReadRVUDVR();
void ReadRVUCamera();

void ReadRVUDevice();

void WriteAssUserOperateLogPre(char *szNote);

extern RECV_PACKET g_recv_packet;
extern int g_nDataLen;

extern COMMONTHREADPOOL g_parse_rvu_thread ;

extern CPtuDlg* g_pMainDlg;
extern VEM_USER_CONFIG_INFO* g_pConfigInfo;
extern DEV_PLAY_VIDEO_SDK_FUNS* g_pstruDevPlayVideoSdkFunsEx;

extern _T_ACS_STATION_OB_NODE_ g_tAcsStationObNode[MAX_STATION_OB_NODE_NUM];
extern int g_nStationObNodeNum;

extern _T_ACS_STATION_OB_ACCESS_DEV_ g_tAcsStationObAccessDev[MAX_STATION_OB_ACCESS_DEV];
extern int g_nStationObAccessDev;

extern _T_ACS_STATION_OB_CAMERA_ g_tAcsStationObCamera[MAX_STATION_OB_CAMERA];
extern int g_nStationObCamera;

extern _T_ACS_STATION_LINKAGE_RELATION_ g_tAcsStationLinkageRelation[MAX_STATION_ST_RELATION];
extern int g_nStationLinkageRelation;

extern _T_ACS_STATION_OB_YK_ g_tAcsStationObYK[MAX_STATION_OB_YK];
extern int g_nStationObYK;

extern _T_ACS_STATION_OB_YT_ g_tAcsStationObYT[MAX_STATION_OB_YT];
extern int g_nStationObYT;

extern _T_ACS_STATION_OB_YX_ g_tAcsStationObYX[MAX_STATION_OB_YX];
extern int g_nStationObYX;

extern _T_ACS_STATION_OB_YC_ g_tAcsStationObYC[MAX_STATION_OB_YC];
extern int g_nStationObYC;

extern _T_ACS_STATION_OB_CARD_ g_tAcsStationObCard[MAX_STATION_OB_CARD];
extern int g_nStationObCard;

extern _T_ACS_STATION_CAMERA_PRESET_ g_tAcsStationObPreset[MAX_STATION_CAMERA_PRESET];
extern int g_nStationObCameraPreset;

extern _T_ACS_STATION_CT_USER_ g_tAcsStationCtUser[MAX_STATION_CT_USER];
extern int g_nStationCtUser;

extern _T_ACS_RVU_CAMERA_INFO_ g_tAcsRVUCameraInfo[MAX_CAMERA_NUM_PER_USER];
extern int g_nAcsRVUStation;

extern _T_ACS_RVU_DVR_INFO_ g_tAcsRVUDVRInfo[MAX_DVR_NUM_PER_USER];
extern int g_nAcsRVUDVR;

extern _T_ACS_RVU_STATION_INFO_ g_tAcsRVUStationInfo[MAX_STATION_NUM_PER_USER];
extern int g_nAcsRVUCamera;

extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;

extern _T_ASS_RVU_DEVICE_INFO_ g_tAssRVUDevice[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int g_nAssRVUDeviceNum;

extern TSM g_t_TSM[MAX_SM_NUM_PER_STATION];
extern int g_nTSM;

extern _T_ASS_STATION_OB_NODE_ g_tAssStationObNode[MAX_STATION_OB_NODE_NUM];
extern int g_nAssStationObNodeNum;

extern TNODE g_t_TNODE[MAX_NODE_NUM_PER_STATION];
extern int g_nTNODE;

extern T_USER *g_pUser;