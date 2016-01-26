#pragma once

// CDlgMenu 对话框
#include "SkinButton.h"
#include "afxwin.h"
#include "SkinStatic.h"
#include "resource.h"

class CDlgMenu : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu)

public:
	CDlgMenu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMenu();

public:
	// 自定义变量
	CSkinButton m_MenuAlarmSet;
	CSkinButton m_MenuEmap;
	CSkinButton m_MenuView;
	CSkinButton m_MenuTuneCycle;
	CSkinButton m_MenuEventlog;
	CSkinButton m_MenuRealTalk;
	CSkinButton m_MenuTempFavorite;
	CSkinButton m_MenuSystem;
	CSkinButton m_MenuReplay;
	CSkinButton m_MenuQuit;
	CSkinButton m_MenuTray;
	CSkinButton m_MenuMin;
	CSkinButton m_MenuAlarmLog;
	CSkinButton m_MenuNetStruct;
	CSkinButton m_MenuManage;
	CSkinButton m_MenuLogin;
	CSkinButton m_btnShowControl;
	CSkinButton m_btnHelp;
	CSkinStatic	m_staticShowWeather;
	CSkinStatic m_staticUserInfo;
	CSkinStatic m_staticVersionInfo;

	CSkinButton m_MenuElecMap;
	CSkinButton m_MenuDeviceLedger;
	CSkinButton m_MenuLinkageSetting;
	CSkinButton m_MenuAssistance;
	CSkinButton m_btnAlarmPreview;

public:
	CBitmap   m_LeftBmp;
	CBitmap   m_CenterBmp;
	CBitmap   m_RightBmp;
	CBitmap   m_NameBmp;
	CBitmap   m_UpBmp;
	CBitmap   m_BottomBmp;
	BITMAP    m_LeftBm;
	BITMAP    m_CenterBm;
	BITMAP    m_RightBm;
	BITMAP    m_NameBm;
	BITMAP    m_UpBm;
	BITMAP    m_BottomBm;

// 对话框数据
	enum { IDD = IDD_DIALOG_MENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

// 消息函数
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnControlButton(UINT nID);		//响应按钮消息
	afx_msg void OnBnClickedMenuMin();
	afx_msg void OnBnClickedBtnShowControl();
	afx_msg void OnBnClickedMenuEmap();
	
};
