// DlgColumn1.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgColumn1.h"


// CDlgColumn1 对话框

IMPLEMENT_DYNAMIC(CDlgColumn1, CDialog)

CDlgColumn1::CDlgColumn1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColumn1::IDD, pParent)
{

}

CDlgColumn1::~CDlgColumn1()
{
}

void CDlgColumn1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColumn1, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgColumn1 消息处理程序

void CDlgColumn1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	//改变背景色
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(181,212,254));
	// 不为绘图消息调用 CDialog::OnPaint()
}
