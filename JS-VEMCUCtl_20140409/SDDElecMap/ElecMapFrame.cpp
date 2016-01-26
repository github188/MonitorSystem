// ElecMapFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "SDDElecMap.h"
#include "ElecMapFrame.h"


// CElecMapFrame

IMPLEMENT_DYNCREATE(CElecMapFrame, CMDIFrameWnd)

CElecMapFrame::CElecMapFrame()
{

}

CElecMapFrame::~CElecMapFrame()
{
}


BEGIN_MESSAGE_MAP(CElecMapFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CElecMapFrame 消息处理程序

BOOL CElecMapFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

int	CElecMapFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) ==-1)
	{
		return -1;
	}

	if (!m_wndDrawTool.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D|TBSTYLE_BUTTON,IDR_TOOLBAR_DRAWTYPE) ||
		!m_wndDrawTool.LoadToolBar(IDR_TOOLBAR_CANVAS))
	{
		TRACE0("不能创建绘图工具箱\n");
		return -1;
	}

	return 0;
}