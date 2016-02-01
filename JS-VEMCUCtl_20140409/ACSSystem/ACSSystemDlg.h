#pragma once

#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgViewOut.h"

#define CAMERA_LIST_TYPE 0
#define YC_LIST_TYPE 1
#define YX_LIST_TYPE 2
#define YT_LIST_TYPE 3
#define YK_LIST_TYPE 4

#define	ID_DLG_ACSSYSTEM_CLOSE_VIDEO 11100

// CACSSystemDlg 对话框
extern DBConfigParam g_DBConfigParam;

class CACSSystemDlg : public CDialog
{
	DECLARE_DYNAMIC(CACSSystemDlg)

public:
	CACSSystemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CACSSystemDlg();

// 对话框数据
	enum { IDD = IDD_ACSSYSTEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL ReadLocalConfigFile(void);
	CRect m_WndRect;
	void InitCameraList(int ob_node_id);
	void SetDeviceNodeInfo(int nItem, int node_id, int parent_id, char *device_name, char *device_code, BYTE IsOnline, BYTE IsAlarm, BYTE IsVideo, int camera_id,int preset_id,int list_type);
	void InitCtrl();
	CListCtrl m_ListYX;
	CButton m_CheckAll;
	CButton m_CheckSecurity;
	CButton m_CheckFire;
	CButton m_CheckDoor;
	int m_ytcommand;
	int m_nCurrentID;
	CImageList m_ilTreeIcons;
public:
	CDlgViewOut* m_pViewOut;
	afx_msg void OnBnClickedCheckAllSubsystem();
	afx_msg void OnBnClickedCheckSecuritySystem();
	afx_msg void OnBnClickedCheckFireSystem();
	afx_msg void OnBnClickedCheckDoorSystem();
	BOOL CloseVideo(int dvrtype);
	BOOL InitDVR();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_staticElecmap;
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	BOOL FreeDVR();
	CComboBox m_ComboYTSpeed;
	afx_msg void OnBnClickedButtonUp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnStartYTControl(WPARAM wParam,LPARAM lParam);
	void SetStationNodeInfo(HTREEITEM hItem,int station_id,char* station_code,char* station_name,int node_type,int connect_state);
	void InitStationList();
	CTreeCtrl m_treeStation;
	afx_msg void OnNMDblclkTreeStation(NMHDR *pNMHDR, LRESULT *pResult);
	int GetRvuServerNode(char* ip,int port,char* user,char* password);
	void RefreshYXList(int nStationId);
	void RefreshYCList(int nStationId);
	void FreeYXList();
	void FreeYCList();
	int OpenCamera(char* szCameraCode);
	StationNode* m_pStationNode;
	int m_nRvuID;
	CAMERA_INFO m_camera_info;
	bool m_bVideoOpen;
	int m_nDvrType;
	afx_msg void OnNMDblclkListYxDevice(NMHDR *pNMHDR, LRESULT *pResult);
	void RefreshYXListRealData(char *pSmRealData, int nCount);
	afx_msg void OnNMRClickListYxDevice(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL SendSubscribeStationRequest(int nUserId, int nStationId);
	void PopRightMenu(T_LINKAGE_CAMERA_INFO *tCameraCode, int nCount);
	afx_msg void OnClickLinkageCamera(UINT nID);
	void ToPreset(T_LINKAGE_CAMERA_INFO *pLinkageCameraInfo);
	void InsertSmRealDataToYXList(int nIndex, T_SM_REALDATA *pSMRealData);
	void ProcessRClickCamera();
	void PopClickCameraRightMenu();
	afx_msg void OnClickACSSystemDlgCloseVideo();
	void CloseCamera(int nID);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CString GetStationTreeShowText(int nFlag, CString strStationName);
	void SetVolateClassStationToTree(char* szVolClass, HTREEITEM hItem);
};

extern CACSSystemDlg* g_pMainDlg;

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

void ReadRealDataThread(void* arg);

extern MYSQL mysql_cameralist;

extern VEM_USER_CONFIG_INFO* g_pConfigInfo;

extern HANDLE g_hGetNodeEvent;

extern TSM g_t_TSM[MAX_SM_NUM_PER_STATION];
extern int g_nTSM;

extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;

extern RECV_PACKET g_recv_packet;
extern int g_nDataLen;

extern COMMONTHREADPOOL g_parse_rvu_thread;
extern COMMONTHREADPOOL g_read_real_data;

extern TNODE g_t_TNODE[MAX_NODE_NUM_PER_STATION];
extern int g_nTNODE;