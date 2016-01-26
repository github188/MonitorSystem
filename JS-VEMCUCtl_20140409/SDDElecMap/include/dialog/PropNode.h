#pragma once

#include "../../resource.h"
// CPropNode 对话框

class CPropNode : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropNode)

public:
	void GetNodeParam(char text[],char NodeCaption[],BOOL& bNodesoe,BOOL& bNodeyc,BOOL& bNodeycspecial,
		BOOL& bNodediandu,BOOL& bNodeEpd);

	void SetNodeParam(char text[],char NodeCaption[],BOOL bNodesoe,BOOL bNodeyc,BOOL bNodeycspecial,
		BOOL bNodediandu,BOOL bNodeEpd);

	CPropNode();
	virtual ~CPropNode();

// 对话框数据
	enum { IDD = IDD_NODE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	// SOE查询
	BOOL m_NodeSoe;
	// EPD查询
	BOOL m_NodeEpd;
	// 控制说明
	CString m_KZCaption;
	// 单元名
	CString m_NodeNameCaption;
	// 遥测查询
	BOOL m_NodeYC;
	// 遥测越限查询
	BOOL m_NodeYX;
	// SOE查询
	BOOL m_NodeDiandu;
	afx_msg void OnBnClickedButton1();
};
