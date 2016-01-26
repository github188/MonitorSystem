#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ImageView.h"


// CDlgPresetSetting 对话框

class CViewChannel;

class CDlgPresetSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgPresetSetting)

public:
	CDlgPresetSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPresetSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRESET_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	StationNode* m_pStationNode;
	int m_nPresetId;
	int m_nCameraPresetId;
	char m_szCameraCode[64];
	char m_szPresetName[64];
public:
	CImageList m_ilTreeIcons;
	int m_nCurrentID;
	CImageView* m_pImageWindow;
	CAMERA_INFO m_camera_info;
	bool m_bVideoOpen;
	int m_nDvrType;
	CBitmap* m_pBackBmp;
	CViewChannel *m_pViewChannel;
public:
	CTreeCtrl m_treeLinkageList;
	CTreeCtrl m_treePresetList;
	virtual BOOL OnInitDialog();
	void FreeNodeRes(HTREEITEM hItem,int nMethod);
	void InitLinkageEventReasonList();
	void InitLinageActionList();
	void SetNodeInfo(HTREEITEM hItem,int node_id,int parent_id,int station_id,int dvr_id,int channel_id,char* node_name,char* node_code,int node_type,int node_level);
	afx_msg void OnNMClickTreePresettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreePresettingList(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_combo_presetnum;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_playwnd;
	afx_msg void OnBnClickedButton1();
	void InitControl();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnBnClickedButtonToPreset();
	afx_msg void OnBnClickedButtonSetPreset();
	afx_msg void OnBnClickedButtonDeletePreset();
	afx_msg void OnNMDblclkTreeLinkageList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnStartYTControl(WPARAM wParam,LPARAM lParam);
	void FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void SetTreeNodeInfo(CTreeCtrl* pTreeList,HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId,char* szNodeCode);
	void InitPresettingList(StationNode* pNodeInfo);
	void SetStationNode(StationNode* pStationNode);
	void InitLinkageList(StationNode* pNodeInfo);
	BOOL QueryPresetListDB(int station_id, char* camera_code, int nDevId, int nPresetID);
	int GetPresetInfoByCameraCode(HTREEITEM hItem,int nStationId,char* szCameraCode,int nDvrId);
	afx_msg void OnNMClickTreeLinkageList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int CreatePresetTable(int nStationId);
	void InitLinkageRelationList(StationNode* pNodeInfo);
	int OpenCamera(char* szCameraCode,unsigned short usDvrId);
	BOOL UpdatePresetInfoToDB(int nStationId, char* szCameraCode, int nPresetId, char* szPresetName, int nDevId, int nLinkageVideoStatus);
	int GetDevIdByPresetIdAndCameraCode(int nStationId, int nPresetId, char *szCameraCode);
	void CloseCamera(int nId);
	void ToPreset(int nPresetId);
	afx_msg LRESULT OnCloseVideoHandle(WPARAM w, LPARAM l);
	CListCtrl m_lstLinkageRelation;
	afx_msg void OnStnClickedStaticCurrentCamera();
	void SetPresetEditText();
	afx_msg void OnCbnSelchangeComboPresetNum();
	int GetCameraInfoByDevId(HTREEITEM hItem, int nDevId);
	afx_msg void OnCloseVideo();
};
