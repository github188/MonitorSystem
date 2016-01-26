// ViewChannel.h: interface for the CViewChannel class.

//摄像头通道类，该类需要配合DevPlayVideoSdk.dll摄像头库使用
//
//////////////////////////////////////////////////////////////////////

#ifndef _VIEW_CHANNEL_H_
#define _VIEW_CHANNEL_H_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
#include <string>

using namespace std;

#define IDC_STATIC_TITLE 12222
#define IDC_STATIC_VIDEO 12223
#define IDC_MENU_CLOSE_VIDEO 12224
#define IDC_STATIC_TITLE_CAPTION 12225

/////////////////////////////////////////////////////////////////////////////
// CViewChannel window
// 视频通道视频显示窗口
//

class CViewChannel : public CWnd
{
	DECLARE_DYNAMIC( CViewChannel )
// Construction
public:
	CViewChannel();
	virtual ~CViewChannel();
public:
	//摄像头信息
	struct T_CAMERA_INFO
	{
		char szDvrIP[32];
		int nDvrPort;
		char szLoginName[32];
		char szLoginPassword[32];
		int nDvrType;
		int nChannel;
		char szCameraName[64];
	};
public:
	HCURSOR		m_hCursor_up;
	HCURSOR		m_hCursor_down;
	HCURSOR		m_hCursor_left;
	HCURSOR		m_hCursor_right;
	HCURSOR		m_hCursor_up_on;
	HCURSOR		m_hCursor_down_on;
	HCURSOR		m_hCursor_left_on;
	HCURSOR		m_hCursor_right_on;
	CFont   m_font; 
public:
	CStatic m_csTitle;//标题picture control
	CStatic m_csVideo;//视频picture control
	CStatic m_csTitleCaption;//标题信息
public:
	//显示窗口ID
	int	m_iWinID;
	//视频是否打开了
	bool m_video_opened;
	//浮动云台相关
	bool m_byt_ctl;
	int m_iyt_pos;
	//当前视频ID
	int m_nCurrentVideoId;
	//摄像头信息
	T_CAMERA_INFO m_tCameraInfo;
	//云台控制速度
	int m_nControlSpeed;
	
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
private:
	//关闭视频
	void OnCloseVideo();
	//设置背景
	void SetBackGround();
	CRect GetDstRect(BITMAP bmpInfo);
	void FillAllDialogBKColor();
	void SetBKPicture();
	void SetTitleBKPicture();
	//创建控件
	void CreateControls();
	//弹出右键菜单
	void PopRightMenu();

public:
	int OpenCamera();//打开视频
	void CloseCamera();//关闭视频
	//设置视频信息
	void SetCameraInfo(T_CAMERA_INFO tCameraInfo);
	//设置标题名称
	void SetTitleCaption(CString strText);
	//刷新背景
	void FlashChannel();
	//云台控制
	int CameraControl(int nDirection, int nStartOrStop);
	int CameraControl(unsigned short nCommand);
	//设置云台控制速度
	void SetControlSpeed(int nSpeed);
	//转预置位
	void ToPreset(int nPresetNum);
	//设置预置位
	BOOL SetPreset(int nPresetNum);
	//删除预置位
	BOOL DeletePreset(int nPresetNum);
};

#endif //_VIEW_CHANNEL_H_
