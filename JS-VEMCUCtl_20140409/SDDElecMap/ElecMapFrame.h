#pragma once


// CElecMapFrame 框架

class CElecMapFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CElecMapFrame)
public:
	CElecMapFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CElecMapFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	CToolBar m_wndToolBar;
	CToolBar m_wndDrawTool;
	CToolBar* m_pTool;

protected:
	afx_msg	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


