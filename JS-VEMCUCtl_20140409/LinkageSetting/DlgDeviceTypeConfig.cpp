// DlgDeviceTypeConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgDeviceTypeConfig.h"
#include "common.h"
#include "DBExcute.h"


// CDlgDeviceTypeConfig 对话框

IMPLEMENT_DYNAMIC(CDlgDeviceTypeConfig, CDialog)

//拖拽到树上时候的回调函数
void CB_AddItems(CMultiTreeCtrl *pSrcTree, CMultiTreeCtrl *pDestTree, HTREEITEM hSrcTItem, HTREEITEM hDestTItem, int nImage, int nSelImage)
{
	////如果拖拽的目的地是自己，则返回
	//if (pSrcTree == pDestTree)
	//	return;

	//如果拖拽的源不是设备类型节点返回
	T_ASS_TREE_NODE* pSrcTreeNode = (T_ASS_TREE_NODE*)pSrcTree->GetItemData(hSrcTItem);
	if (pSrcTreeNode->enumTreeNodeType != ENUM_DEVICE_TYPE_NODE)
		return;

	//如果拖拽的目的地不是子系统，则返回
	T_ASS_TREE_NODE* pDstTreeNode = (T_ASS_TREE_NODE*)pDestTree->GetItemData(hDestTItem);
	if (pDstTreeNode->enumTreeNodeType != ENUM_SUBSYSTEM_NODE)
		return;

	//添加目的节点
	HTREEITEM hChildItem = pDestTree->AddItem(hDestTItem, pSrcTree->GetItemText(hSrcTItem), TVI_LAST, nImage, nSelImage, 0);
	SetTreeItemData(pDestTree, hChildItem, pSrcTreeNode->enumTreeNodeType, (char*)pSrcTreeNode->body, sizeof(T_ASS_DEVICE_TYPE));
	pDestTree->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	pDestTree->Expand(hDestTItem, TVE_EXPAND);

	//更新数据库,设置设备类型的子系统
	T_ASS_SUBSYSTEM *pSubSystem = (T_ASS_SUBSYSTEM*)pDstTreeNode->body;
	int nSubSystemId = pSubSystem->nId;
	UpdateSubSystemOfDeviceType(pSrcTree->GetItemText(hSrcTItem), nSubSystemId);

	//删除源节点
	FreeOneTreeNode(pSrcTree, hSrcTItem);

	return;
}

CDlgDeviceTypeConfig::CDlgDeviceTypeConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceTypeConfig::IDD, pParent)
{

}

CDlgDeviceTypeConfig::~CDlgDeviceTypeConfig()
{
}

void CDlgDeviceTypeConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDeviceTypeConfig, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgDeviceTypeConfig 消息处理程序

BOOL CDlgDeviceTypeConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	InitDeviceTypeTree();

	InitSubSystemTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDeviceTypeConfig::InitControl()
{
	m_pBackBmp = new CBitmap;
	m_pBackBmp->LoadBitmap(IDB_BITMAP_BACK);

	RECT rect = {10,10,210,300};
	m_treeDeviceType.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_TRACKSELECT
		|TVS_SHOWSELALWAYS, rect, this, 1001);

	RECT rect1 = {10,200,210,300};
	m_treeSubSystem.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_TRACKSELECT
		|TVS_SHOWSELALWAYS, rect1, this, 1002);

	m_imageList.Create(IDB_BITMAP_TREE,16,0,RGB(255,0,255));
	m_treeDeviceType.SetImageList(&m_imageList, TVSIL_NORMAL);
	m_treeSubSystem.SetImageList(&m_imageList, TVSIL_NORMAL);

	//注册拖拽到树上时调用的回调函数，如果不设置，则使用默认处理
	m_treeSubSystem.SetCBAddItems(CB_AddItems);
	m_treeDeviceType.SetCBAddItems(CB_AddItems);
}

void CDlgDeviceTypeConfig::InitDeviceTypeTree()
{
	//删除树上所有的设备类型
	FreeTreeNode(&m_treeDeviceType, m_treeDeviceType.GetRootItem());

	//获取设备类型不带父节点的
	std::vector<T_ASS_DEVICE_TYPE> vecDeviceType = GetDeviceTypeFromDB();
	
	//添加到树上
	AddDeviceTypeToTree(vecDeviceType);
} 

void CDlgDeviceTypeConfig::AddDeviceTypeToTree(std::vector<T_ASS_DEVICE_TYPE> vecDeviceType)
{
	HTREEITEM hRootItem = NULL;
	HTREEITEM hChildItem = NULL;
	hRootItem = m_treeDeviceType.InsertItem("全部类型", 8, 8, TVI_ROOT, NULL);
	SetTreeItemData(&m_treeDeviceType, hRootItem, TREE_NODE_TYPE::ENUM_ROOT_NODE, NULL, 0);
	for (size_t t = 0; t < vecDeviceType.size(); t++)
	{
		if (vecDeviceType.at(t).nParentId == 0)
		{
			hChildItem = m_treeDeviceType.InsertItem(vecDeviceType.at(t).szDeviceTypeName, 8, 8, hRootItem);
			SetTreeItemData(&m_treeDeviceType, hChildItem, TREE_NODE_TYPE::ENUM_DEVICE_TYPE_NODE, (char*)&vecDeviceType.at(t), sizeof(T_ASS_DEVICE_TYPE));
		}
	}
}

void CDlgDeviceTypeConfig::InitSubSystemTree()
{
	//删除树上所有的设备类型
	FreeTreeNode(&m_treeSubSystem, m_treeSubSystem.GetRootItem());

	//获取子系统
	std::vector<T_ASS_SUBSYSTEM> vecSubSystem = GetSubSystemFromDB();

	//添加到树上
	AddSubSystemToTree(vecSubSystem);

	//全部展开
	ExpandAllItems(&m_treeSubSystem, NULL);
}

void CDlgDeviceTypeConfig::AddSubSystemToTree(std::vector<T_ASS_SUBSYSTEM> vecSubSystem)
{
	//获取子系统下的设备类型
	std::vector<T_ASS_DEVICE_TYPE> vecDeviceType = GetDeviceTypeFromDB();

	HTREEITEM hRootItem = NULL;
	HTREEITEM hChildItem = NULL;
	HTREEITEM hDeviceTypeItem = NULL;
	hRootItem = m_treeSubSystem.InsertItem("全部子系统", 8, 8, TVI_ROOT, NULL);
	SetTreeItemData(&m_treeSubSystem, hRootItem, TREE_NODE_TYPE::ENUM_ROOT_NODE, NULL, 0);
	for (size_t t = 0; t < vecSubSystem.size(); t++)
	{
		hChildItem = m_treeSubSystem.InsertItem(vecSubSystem.at(t).szSubSystemName, 8, 8, hRootItem);
		SetTreeItemData(&m_treeSubSystem, hChildItem, TREE_NODE_TYPE::ENUM_SUBSYSTEM_NODE, (char*)&vecSubSystem.at(t), sizeof(T_ASS_SUBSYSTEM));
		
		for (int i=0; i<vecDeviceType.size(); i++)
		{
			if (vecDeviceType.at(i).nParentId == vecSubSystem.at(t).nId)
			{
				hDeviceTypeItem = m_treeSubSystem.InsertItem(vecDeviceType.at(i).szDeviceTypeName, 10, 10, hChildItem);
				SetTreeItemData(&m_treeSubSystem, hDeviceTypeItem, TREE_NODE_TYPE::ENUM_DEVICE_TYPE_NODE, (char*)&vecDeviceType.at(t), sizeof(T_ASS_DEVICE_TYPE));
			}
		}
	}
}

void CDlgDeviceTypeConfig::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect static_rect,winrc,tree_rect;
	GetClientRect(&winrc);
	int nStaticWidth = cx * 0.1;
	int nStaticHeight = 30;

	int nTreeWidth = cx * 0.2;
	int nTreeHeight = cy - nStaticHeight;

	CWnd * pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_DEVICE_TYPE);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//pWnd = GetDlgItem(IDC_TREE_DEVICE_TYPE);
	if (m_treeDeviceType)
	{
		static_rect.top = static_rect.bottom;
		static_rect.bottom = static_rect.top + nTreeHeight;
		static_rect.left = winrc.left;
		static_rect.right = static_rect.left + nTreeWidth;
		m_treeDeviceType.MoveWindow(static_rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_SUB_SYSTEM);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = static_rect.right + 30;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//pWnd = GetDlgItem(IDC_TREE_SUB_SYSTEM);
	if (m_treeSubSystem)
	{
		static_rect.top = static_rect.bottom;
		static_rect.bottom = static_rect.top + nTreeHeight;
		static_rect.left = static_rect.left;
		static_rect.right = static_rect.left + nTreeWidth;
		m_treeSubSystem.MoveWindow(static_rect);
	}
}

BOOL CDlgDeviceTypeConfig::OnEraseBkgnd(CDC* pDC)
{
	//设置对话框背景色
	CBrush backBrush(RGB(218,255,251));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CDC srcDC;
	//srcDC.CreateCompatibleDC(pDC);
	//CBitmap* pOldBmp;
	//BITMAP bmp;
	//CRect winrc;

	//GetClientRect(&winrc);

	//pOldBmp = srcDC.SelectObject(m_pBackBmp);
	//m_pBackBmp->GetBitmap(&bmp);
	//pDC->StretchBlt(0,0,winrc.Width(),winrc.Height(),
	//	&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	//srcDC.SelectObject(pOldBmp);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgDeviceTypeConfig::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDeviceTypeConfig::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_treeSubSystem.OnMouseMove(nFlags, point);
	//m_treeDeviceType.OnMouseMove(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgDeviceTypeConfig::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_treeSubSystem.OnLButtonUp(nFlags, point);
	//m_treeDeviceType.OnLButtonUp(nFlags, point);

	CDialog::OnLButtonUp(nFlags, point);
}

HBRUSH CDlgDeviceTypeConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
