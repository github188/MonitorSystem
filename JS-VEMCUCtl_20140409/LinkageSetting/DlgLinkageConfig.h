#pragma once
#include "stdafx.h"
#include "afxcmn.h"

// CDlgLinkageConfig 对话框


class CDlgLinkageConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgLinkageConfig)

public:
	CDlgLinkageConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLinkageConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_LINKAGE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	_T_DEV_INFO_ m_tDevInfo;
	_T_LINK_CONFIG_INFO_ m_tLinkageConfigInfo;
	int m_nIndex;
	StationNode* m_pStationNode;
public:
	afx_msg void OnNMClickTreeDev(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_treeDev;
	virtual BOOL OnInitDialog();
	CListCtrl m_lstLinkage;
	afx_msg void OnNMDblclkTreeDev(NMHDR *pNMHDR, LRESULT *pResult);
	void SetLikageConfigListInfo(_T_DEV_INFO_ *pDevInfo);
	void GetLinkageTypeText(char *szLinkageTypeText,int nRelateDevType);
	void GetLinkageActionText(char *szLinkageActionText,int nRelateDevAction);
	void InitControl();
	void FreeLinkageList();
	afx_msg void OnNMRClickListLinkage(NMHDR *pNMHDR, LRESULT *pResult);
	void PopRightMenu();
	afx_msg void OnClickDeleteLinkageInRecord();
	bool DeleteLinkageRecordFromDB(int nId);
	void DeleteLinkageRecordFromList(int nIndex);
	afx_msg void OnBnClickedButtonAddLinkageDev();
	void SetStationNode(StationNode* pStationNode);
	void ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void SetTreeNodeInfo(CTreeCtrl* pTreeList,HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId,char* szNodeCode);
	void FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
	void InitDevTree(StationNode* pNodeInfo);
	void RefreshLinkageConfigDlg(StationNode* pNodeInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
