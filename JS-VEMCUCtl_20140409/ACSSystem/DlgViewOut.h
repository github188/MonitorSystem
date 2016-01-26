#pragma once
#include "afxwin.h"
#include "ViewChannel.h"
#include "label.h"

// CDlgViewOut 对话框

class CDlgViewOut : public CDialog
{
	DECLARE_DYNAMIC(CDlgViewOut)

public:
	CDlgViewOut(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgViewOut();

	// 对话框数据
	enum { IDD = IDD_DIALOG_VIEW_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void DrawSelectedWindowFrame(BOOL bSelected);
	void DrawRect(CDC *pDC,RECT &rect);

public:
	//视频显示窗口
	CViewChannel m_ViewChannel;

	//窗口ID
	int m_nWinID;

	//是否选中
	BOOL m_bSelected;

	//标题栏背景图
	CBitmap m_titleBk;

	BITMAP m_bmBk;

	//背景画刷
	CBrush m_brush;

	//Camera信息
	CAMERA_INFO m_camera_info;

	//LoginID
	int m_nLoginID;

	//PlayID
	int m_nPlayID;

	//dvr类型
	int m_nDvrType;

	//视频窗口标题栏中摄像机名称
	CLabel m_strCameraName;

	//是否关联灯光
	BOOL m_bLightRelating;

	//灯光控制主机编码
	CString  m_strLightCode;
};
