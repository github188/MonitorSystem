#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgPtuManage 对话框

class CDlgPtuManage : public CDialog
{
	DECLARE_DYNAMIC(CDlgPtuManage)

public:
	CDlgPtuManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPtuManage();

// 对话框数据
	enum { IDD = IDD_DIALOG_PTU_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_nRvuId;
	int m_nStationId;
	StationNode* m_pStationNode;
	CBitmap* m_pBackBmp;
public:
	CImageList m_ilTreeIcons;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetEditControlText(CString strPtuIP,CString strPtuPort,CString strPtuUserName,CString strPtuPassword,int connect_ptu,CString strPtuName,CString strRvuType, CString strRvuCode);
	CEdit m_ctrPtuIP;
	CEdit m_ctrPtuPort;
	CEdit m_ctrPtuUserName;
	CEdit m_ctrPtuPassword;
	afx_msg void OnBnClickedButtonChangeInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonConnectTest();
	int ConnectTest(CString strPtuIP,CString strPtuPort);
	void InialRvuDeviceList(StationNode *pNodeInfo);
	CStatic m_ctrStaticPtuStationValue;
	CTreeCtrl m_treeRvuDeviceList;
	afx_msg void OnBnClickedButtonGetDeviceInfo();
	int GetRVUServerDevice(char* ip,int port,char* user,char* password);
	CTreeCtrl m_treeRouteRvuDeviceList;
	afx_msg void OnBnClickedButtonAddToLocalDatabase();
	void SetRvuId(int nRvuId);
	void SetStationId(int nStationId);
	void InialRouteRvuDeviceList(StationNode *pNodeInfo);
	void SetStationNode(StationNode* pStationNode);
	void ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void SetRouteRvuTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId);
	void SetLocalRvuTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId);
	void FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void GetTableNameByType(char* szTableName,int nType);
	int GetSubSysByDeviceType(int nDeviceType);
	void InialSubSystemList(StationNode *pNodeInfo);
	void SetSusSystemTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId);
	void GetSMNameByDeviceTypeAndNodeNum(char* szSMName,int nDeviceType,int nNodeId);
	void GetGuid(char* szGuid);
	CEdit m_ctrPtuName;
	CTreeCtrl m_treeSubSystemList;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonDelRvu();
	afx_msg void OnBnClickedButtonEditRvu();
	bool CheckRvuParams(CString strPtuIP, CString strPtuPort, CString strPtuUserName, CString strPtuPassword, CString strRvuType, CString strRvuCode);
	void AddRvuTypeToCombo(std::vector<T_ASS_RVU_TYPE> vecRvuType);
	void InitComboRvuTypeData();
	void InitControl();
	CComboBox m_comboRvuType;
};

extern HANDLE g_hGetDeviceEvent;
