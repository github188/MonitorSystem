// DlgColumn2.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgColumn2.h"


// CDlgColumn2 对话框

IMPLEMENT_DYNAMIC(CDlgColumn2, CDialog)

CDlgColumn2::CDlgColumn2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColumn2::IDD, pParent)
{

}

CDlgColumn2::~CDlgColumn2()
{
}

void CDlgColumn2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColumn2, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgColumn2 消息处理程序

void CDlgColumn2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(181,212,254));
	// 不为绘图消息调用 CDialog::OnPaint()
}
