#pragma once

#include "afxwin.h"
#include "../../Resource.h"

#define			SHQ_E_NOERROR					0
#define			SHQ_E_NOTLOADDLL				1
#define			SHQ_E_NOTFINDSERVER				2
#define			SHQ_E_OTHER						3
#define         SHQ_E_NOTCOMPUTERNAME           4

#pragma pack(push,before_serverinfo,1)
typedef struct
{
	BYTE wServerNum;           //服务器数目
	CHAR stMainServer[64];     //主服务器名
	CHAR stBackServer[64];     //备服务器名
	BYTE bMainServer;          //主服务器在线情况
	BYTE bBackServer;          //备服务器在线情况
} DLLSERVERINFO;
#pragma pack(pop,before_serverinfo,8)

// CMNP_SHXGDlg 对话框

class CMNP_SHXGDlg : public CDialog
{
	DECLARE_DYNAMIC(CMNP_SHXGDlg)

public:
	CMNP_SHXGDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMNP_SHXGDlg();

// 对话框数据
	enum { IDD = IDD_MNP_SHXG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitNode(void);
	void InitLine(void);
	void InitUnit(void);
protected:
	BYTE m_Node;
	BYTE m_Line;
	BYTE m_Unit;
	int ErrorCode;
	char MainServerName[64];
	char BackServerName[64];
public:
	void SetNode(BYTE node);
	void SetLine(BYTE line);
	void SetUnit(BYTE unit);
	BYTE GetNode(void);
	BYTE GetLine(void);
	BYTE GetUnit(void);
	char* GetServerName(void);
	int GetServerInfo(void);
	CComboBox m_NodeBox;
	CComboBox m_LineBox;
	CComboBox m_UnitBox;
	void ShowError(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeNodecombo();
	afx_msg void OnCbnSelchangeLinecombo();
};
