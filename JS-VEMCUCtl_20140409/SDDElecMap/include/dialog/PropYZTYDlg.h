#pragma once
#include "../../resource.h"
// CPropYZTYDlg 对话框

class CPropYZTYDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropYZTYDlg)

public:
	CPropYZTYDlg();
	virtual ~CPropYZTYDlg();

// 对话框数据
	enum { IDD = IDD_PROP_YZTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 升压动作方式
	int m_nUp_action;
	// 降压动作方式
	int m_nDown_action;
	// 急停动作方式
	int m_nStop_action;
	// 直控
	int m_nZK;
public:
	void SetParam(char text[], char Yknameup[], char Yknamedown[], char Yknamestop[], int bVTup,int bVTdown,int bVTstop, char Yxname_FK[]);
	void GetParam(char text[], char Yknameup[], char Yknamedown[], char Yknamestop[], int &bVTup, int &bVTdown, int &bVTstop, char Yxname_FK[]);
private:
	// 控制说明
	CString m_strName;
	// 升压遥控点
	CString m_strUpName;
	// 降压遥控点
	CString m_strDownName;
	// 急停遥控点
	CString m_strStopName;
	// 反馈点
	CString m_strFK;
public:
	afx_msg void OnBnClickedButton1();
private:
	// 0-升压，1-降压，2-急停，3-反馈点
	int m_nType;
	void AddName(int nType);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
