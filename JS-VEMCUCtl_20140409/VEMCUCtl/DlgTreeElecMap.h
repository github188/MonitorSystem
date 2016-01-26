#pragma once
#include "resource.h"
#include "SkinButton.h"
// CDlgTreeElecMap 对话框
#define TREEITEM_CLASS_PROVINCE 0x01
#define TREEITEM_CLASS_CITY		0x02
#define TREEITEM_CLASS_STATE    0x04
#define TREEITEM_CLASS_SCENE	0x08

#define TREEITEM_CLASS_VOLT		0x20

class CDlgTreeElecMap : public CDialog
{
	DECLARE_DYNAMIC(CDlgTreeElecMap)

public:
	CDlgTreeElecMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTreeElecMap();

// 对话框数据
	enum { IDD = IDD_DIALOG_TREE_ELECMAP };

public:
	virtual BOOL OnInitDialog();

private:
	void LoadSkin();
	void ReadDBSystemInfo();	// 读取数据库信息，包括视频平台信息、变电站信息及节点信息
	void InitCameraTreelist();	// 初始化列表树

	int CheckStationCurrentStatus2(int nStationId);
	void RefreshDeviceStatusBySecne(CString strItemName);

	BOOL addSceneToStation(HTREEITEM& hItem, const CString strStation);
	BOOL addSceneToVoltNode(HTREEITEM& hItem);
	BOOL addSceneToProvince(HTREEITEM& hItem);

protected:
	CTreeCtrl		m_ctrlTree;
	CImageList		m_imageList;
	CSkinButton		m_btnSearch;
	CString			m_strSearch;

	BOOL m_bIsAddProvinceScene;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnTreeSelChanging(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTreeSelChange(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnTreeNMDBLCLK(NMHDR* pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
