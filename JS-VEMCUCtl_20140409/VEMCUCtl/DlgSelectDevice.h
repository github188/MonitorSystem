#pragma once

#include "afxcmn.h" 
#include "afxwin.h"
#include "SkinButton.h"


// CDlgSelectDevice 对话框

class CDlgSelectDevice : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectDevice)

public:
	CDlgSelectDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelectDevice();

public:
	BOOL InitTreeDevice();
	BOOL InitTreeDeviceByCity();
	BOOL InitTreeDeviceByVoltage();
	void SetCameraNodeInfo(HTREEITEM hItem, 
		char* ItemName, char* Itemnum, int StationOrCameraID,int ItemStatus,int Streamless, 
		float ItemLongitude, float ItemLatitude, 
		int ItemDecodeTag, int Itemtype, char* stationname);// 设置节点信息
	void DeleteTreeDevice();
	void DeleteChildTreeDeviceInfo(HTREEITEM hItem);

	//设置摄像头的DVR信息
	BOOL SetCameraDvrInfo(HTREEITEM hTreeItem);
	
	//设置站点摄像头DVR信息
	BOOL SetStationCameraDvrInfo(HTREEITEM hTreeItem);

	BOOL loadSkin();

public:
	HTREEITEM	m_hSelectItem;
	CImageList	m_TreeImage;	
	_T_NODE_INFO *m_pNodeInfo;
	CBitmap  m_Bmp;
	BITMAP   m_bm;

public:
	CTreeCtrl m_treeDevice;
	CSkinButton m_btnExit;
	CSkinButton m_btnSelect;

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnSelect();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMenuitemSelectDeviceReflashlist();
	afx_msg void OnNMRClickTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
};
