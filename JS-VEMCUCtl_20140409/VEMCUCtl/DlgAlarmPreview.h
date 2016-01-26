#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BtnST.h"


// CDlgAlarmPreview 对话框

class CDlgAlarmPreview : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmPreview)

public:
	CDlgAlarmPreview(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlarmPreview();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALARM_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuery();
	void InitControl();
	void InitStationtree();
	afx_msg void OnNMDblclkListAlarm(NMHDR *pNMHDR, LRESULT *pResult);
	void InitAlarmListColumnAndWidth();
	CString GetQueryCondition(std::vector<HTREEITEM> vecSelectedItems);
	void AddStationAlarmToList(std::vector<_T_ASS_SUBSYSTEM_ALARM_COUNT> vecSubSystemAlarmCount);
	//弹出右键菜单
	void PopRightMenu();
	afx_msg void OnClickViewDetail();//查看详细
	afx_msg void OnClickProcessAll();//处理全部
	CButtonST m_btnQuery;
	CListCtrl m_lstAlarm;
	CMultiTreeCtrl m_treeStation;
	afx_msg void OnNMRClickListAlarm(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_nStationId;
	CImageList m_imageList;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
