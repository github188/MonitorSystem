#pragma once
#include "../../resource.h"
// CPropLine 对话框

class CPropLinkLine : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropLinkLine)

public:
	void GetLineParam(char text[],char LineCaption[],BOOL& bLinesoe,BOOL& bLineyc,BOOL& bLineycspecial,
		BOOL& bLinediandu,BOOL& bLineEpd);

	void SetLineParam(char text[],char LineCaption[],BOOL bLinesoe,BOOL bLineyc,BOOL bLineycspecial,
		BOOL bLinediandu,BOOL bLineEpd);

	CPropLinkLine();
	virtual ~CPropLinkLine();

// 对话框数据
	enum { IDD = IDD_LINK_LINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	// SOE查询
	BOOL m_LineSoe;
	// EPD查询
	BOOL m_LineEpd;
	// 控制说明
	CString m_KZCaption;
	// 单元名
	CString m_LineNameCaption;
	// 遥测查询
	BOOL m_LineYC;
	// 遥测越限查询
	BOOL m_LineYX;
	// SOE查询
	BOOL m_LineDiandu;
	afx_msg void OnBnClickedButton1();
};
