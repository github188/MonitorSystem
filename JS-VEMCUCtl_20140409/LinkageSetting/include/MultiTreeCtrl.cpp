/*************************************************
Copyright undefined.
文件名称： MultiTreeCtrl.cpp
模块设计： 卢业恭
模块编写： 卢业恭
功能： 带复选框可拖拽的树形控件类．
版本号： V2013.3.13
主要函数： 
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem,int nState);
***************************************************/

#include "stdafx.h"
#include "..\resource.h"
#include "MultiTreeCtrl.h"
//#include "../../ShapeUp/ShapeUp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMultiTreeCtrl

CMultiTreeCtrl::CMultiTreeCtrl()
{
	m_bDragging    = FALSE;
	m_uClickFlags  = 0;
    m_uDragFlags   = 0;
	m_nHoverTimerID = 0;
	m_nScrollTimerID = 0;
	m_hDragLeft    = AfxGetApp()->LoadCursor(IDB_BITMAP_BG  );
	m_hDragRight   = AfxGetApp()->LoadCursor(IDB_BITMAP_BG );
	m_hDragAbove   = AfxGetApp()->LoadCursor(IDB_BITMAP_BG );
	m_hDragBelow   = AfxGetApp()->LoadCursor(IDB_BITMAP_BG );
	m_hDragAppend  = AfxGetApp()->LoadCursor(IDB_BITMAP_BG);
 	m_hDragCancel  = AfxGetApp()->LoadCursor(IDB_BITMAP_BG);
}

CMultiTreeCtrl::~CMultiTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMultiTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMultiTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
//	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiTreeCtrl message handlers

void CMultiTreeCtrl::OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_uClickFlags&TVHT_ONITEMSTATEICON)
	{
		CWnd* pWnd = GetParent();
		pWnd->SendMessage(TVN_UPSTATE);
		*pResult = 1;
	}
	else
	{
		*pResult = 0; 
	}
}

HTREEITEM CMultiTreeCtrl::AddItem( 
									 HTREEITEM hParent, 
									 CString csItemName, 
									 HTREEITEM hInsAfter, 
									 int iSelImage , 
									 int iNonSelImage, 
									 long lParam
									 )
{
	HTREEITEM hItem;
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvIns;

	tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	int nLength = csItemName.GetLength() ;
	tvItem.pszText = csItemName.GetBuffer(nLength);
	tvItem.cchTextMax = nLength;
	tvItem.lParam = lParam;

	tvItem.iImage = iNonSelImage;
	tvItem.iSelectedImage = iSelImage;

	tvIns.item = tvItem;
	tvIns.hInsertAfter = hInsAfter;
	tvIns.hParent = hParent;

	hItem = InsertItem(&tvIns);

	return (hItem);
}

void CMultiTreeCtrl::GetCheckItems(std::vector<HTREEITEM>& vecTreeItems, HTREEITEM hTreeItem)
{
	if (hTreeItem == NULL)
	{
		hTreeItem = GetRootItem();
	}

	HTREEITEM hNextItem = GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		GetCheckItems(vecTreeItems, hNextItem);
		hNextItem = GetNextItem(hNextItem,TVGN_NEXT);
	}

	if (hTreeItem!=NULL)
	{
		//如果该节点复选框被选中
		if (GetCheck(hTreeItem))
		{
			vecTreeItems.push_back(hTreeItem);
		}
	}
}

/************************************************************
功能：处理鼠标左键消息，更新选中状态。
说明：更新相关的项目的选中状态。
注：记录点击时间，以便处理无意拖拽。
*************************************************************/
void CMultiTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_hItemClick = HitTest(point, &m_uClickFlags);

	if ( (m_uClickFlags & TVHT_ONITEMSTATEICON ))
	{
		UINT nState = GetItemState(m_hItemClick, TVIS_STATEIMAGEMASK ) >> 12;
        switch(nState)
        {
        case  STATE_UNSEL: nState = STATE_SEL  ; break;
        case  STATE_PSEL : nState = STATE_SEL  ; break;
        case  STATE_SEL  : nState = STATE_UNSEL; break;
        default:           nState = STATE_UNSEL; break;
        }
		SetItemStateEx(m_hItemClick, nState, TRUE);
	}

	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);

}


/**********************************************************************
处理空格键消息,修改节点的选中状态
**********************************************************************/
void CMultiTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	if(nChar == VK_SPACE/*0x20*/)
 	{
 		HTREEITEM hItem = GetSelectedItem();
 		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
 		if(nState != STATE_INI)
 		{
		    nState = (nState == STATE_UNSEL) ? STATE_SEL : STATE_UNSEL;
 			SetItemStateEx( hItem, nState, TRUE);
 		}
 	}
	else CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


/************************************************************************
功能：设置hItem项以及关联数据的状态为nState。
说明：可搜索并更新相关的项的选中状态。
注：需要确保每个Item在初始化时关联了有效的指针或NULL值。
************************************************************************/
BOOL CMultiTreeCtrl::SetItemStateEx(HTREEITEM hItem, UINT nState, BOOL bSearch)
{
	DWORD data = (GetItemData(hItem));
	CTreeCtrl::SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
	if(bSearch)
	{
		TravelChild(hItem, nState);
		TravelSiblingAndParent(hItem, nState);
	}
	return TRUE;
}




/**************************************************************
功能：设置孩子节点的选中状态
设置要求：保持孩子节点的状态与nState一致
注：使用递归处理所有的孩子节点以及孩子的孩子
**************************************************************/
void CMultiTreeCtrl::TravelChild(HTREEITEM hItem, int nState)
{
	HTREEITEM hChildItem = GetChildItem(hItem);

	if(hChildItem != NULL)
	{
		//设置子节点的状态与当前节点的状态一致
        SetItemStateEx( hChildItem, nState, FALSE);

		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, nState);
		
		//处理子节点的兄弟节点和其子节点
		HTREEITEM hBrotherItem = GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//设置子节点状态与当前节点的状态一致
            SetItemStateEx( hBrotherItem, nState, FALSE);

			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, nState);
            
			hBrotherItem = GetNextSiblingItem(hBrotherItem);
		}
	}
}

/************************************************************************
功能：设置父亲节点的选中状态
要求：检查兄弟节点的状态，并相应的设置父亲节点的状态。
注：使用递归处理父亲的父亲
************************************************************************/
void CMultiTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, int nState)
{
	HTREEITEM hNextSiblingItem, hPrevSiblingItem, hParentItem;
	
	//查找父节点，没有就结束
	hParentItem = GetParentItem(hItem);
	if(hParentItem != NULL)
	{
		int nStateSibling = nState;//设初始值，防止没有兄弟节点时出错
		
		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem = GetNextSiblingItem(hItem);
		while(hNextSiblingItem != NULL)
		{
			nStateSibling = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nStateSibling != nState)
            {
                break;
            }
			else
            {
                hNextSiblingItem = GetNextSiblingItem(hNextSiblingItem);
            }
		}
		
		if(nStateSibling == nState)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem = GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem != NULL)
			{
				nStateSibling = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nStateSibling != nState)
                {
                    break;
                }
				else
                {
                    hPrevSiblingItem = GetPrevSiblingItem(hPrevSiblingItem);
                }
			}
		}
	
		if( nStateSibling != nState )
		{
            nState = STATE_PSEL;
        }

		//再递归处理父节点的兄弟节点和其父节点
        SetItemStateEx( hParentItem, nState, FALSE);
		TravelSiblingAndParent(hParentItem, nState);
	}	
}



/*************************************************
功能：重设所有项目的选中状态
**************************************************/
void CMultiTreeCtrl::ResetItemSelState(int nState)
{
    HTREEITEM hItem = GetRootItem();

	while(NULL != hItem)
	{
		SetItemStateEx(hItem, nState, TRUE);
		hItem = GetNextSiblingItem(hItem);
	}

    return; 
}



void CMultiTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;
	// TODO: Add your control notification handler code here
	if (pTVDispInfo->item.pszText != NULL)
	{
		int nData = GetItemData(pTVDispInfo->item.hItem);
		pTVDispInfo->item.pszText;
	}
}

/**********************************************************************
处理节点开始拖放的消息,获取拖拽图像以及拖拽节点句柄
注：通过获取延迟时间处理无意拖拽
**********************************************************************/
void CMultiTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;
	// Highlight selected item
	SelectItem(hTSelItem);
	Select(hTSelItem, TVGN_DROPHILITE);
	
	COleDataSource *poleSourceObj = new COleDataSource ;
	CTreeDropTarget::m_shWndTreeCtrl = m_hWnd;
	// Begin Drag operation
	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();
	// Remove the highlighting
	SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	// If user is moving item by pressing Shift, delete selected item
	if ( dropeffect == DROPEFFECT_MOVE)
		DeleteItem(hTSelItem); 
	delete poleSourceObj;
	*pResult = 0;
}

int CMultiTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	// Register Tree control as a drop target	
	m_CTreeDropTarget.Register(this);
	return 0;
}

/***********************************************************************
实现鼠标拖拽过程中的敏感节点节点展开以及滚动列表
***********************************************************************/
void CMultiTreeCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	//if( nIDEvent == m_nHoverTimerID )
	//{ 
	//	//鼠标敏感节点
	//	KillTimer( m_nHoverTimerID );
	//	m_nHoverTimerID = 0;
	//	HTREEITEM  trItem = 0;
	//	UINT  uFlag = 0;
	//	trItem = HitTest( m_HoverPoint, &uFlag );
	//	if( trItem && m_bDragging )
	//	{
	//		SelectItem( trItem );
	//		Expand( trItem, TVE_EXPAND );
	//	}
	//}
	//else if( nIDEvent == m_nScrollTimerID )
	//{  
	//	//处理拖曳过程中的滚动问题
	//	m_TimerTicks++;
	//	CPoint  pt;
	//	GetCursorPos( &pt );
	//	CRect  rect;
	//	GetClientRect( &rect );
	//	ClientToScreen( &rect );

	//	if (pt.x > rect.left && pt.x < rect.right)
	//	{
	//		HTREEITEM  hItem = GetFirstVisibleItem();
	//		
	//		if( pt.y < rect.top - 10 )
	//		{
	//			//向上滚动
	//			int  slowscroll = 6 - (rect.top + 10 - pt.y )/20;
	//			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
	//			{
	//				CImageList::DragShowNolock ( false );
	//				SendMessage( WM_VSCROLL, SB_LINEUP );
	//				SelectDropTarget( hItem );
	//				m_hNewParent = hItem;
	//				CImageList::DragShowNolock ( true );
	//			}
	//		}
	//		else if( pt.y > rect.bottom + 10 )
	//		{
	//			//向下滚动
	//			int  slowscroll = 6 - (pt.y - rect.bottom + 10)/20;
	//			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
	//			{
	//				CImageList::DragShowNolock ( false );
	//				SendMessage( WM_VSCROLL, SB_LINEDOWN );
	//				int  nCount = GetVisibleCount();
	//				for( int i=0 ; i<nCount-1 ; i++ )
	//					hItem = GetNextVisibleItem( hItem );
	//				if( hItem )
	//					SelectDropTarget( hItem );
	//				m_hNewParent = hItem;
	//				CImageList::DragShowNolock ( true );
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	CTreeCtrl::OnTimer(nIDEvent);
	//}
}

/******************************************************************
处理鼠标点击释放消息
注：若存在拖拽操作，给父窗口发送节点拖拽的消息。
    父窗口收到拖拽消息后，可以通过树的成员获取拖拽的节点句柄。
******************************************************************/
void CMultiTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTreeCtrl::OnLButtonUp(nFlags, point);
	//if( m_bDragging )
	//{
	//	m_bDragging = FALSE;
	//	CImageList::DragLeave( this );
	//	CImageList::EndDrag();
	//	ReleaseCapture();
	//	delete m_pDragImage;

	//	SelectDropTarget( NULL );
	//	
	//	if( m_hItemDrag == m_hNewParent )
	//	{
	//		KillTimer( m_nScrollTimerID );
	//		return;
	//	}
	//	
	//	KillTimer( m_nScrollTimerID );
	//	CWnd* pWnd = GetParent();
	//	pWnd->SendMessage(TVN_ENDDRAG);
	//}
}


/***************************************************************
处理鼠标拖动事件，绘制节点拖动效果，记录拖拽接受节点。
注：若拖拽超出边界，接手节点为根结点。
***************************************************************/
void CMultiTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	////检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时
	//if( m_nHoverTimerID )
	//{
	//	KillTimer( m_nHoverTimerID );
	//	m_nHoverTimerID = 0;
	//}

	//m_nHoverTimerID = SetTimer( 1, 600, NULL );  //定时为 0.6 秒则自动展开
	//m_HoverPoint    = point;

	//if( m_bDragging )
	//{
	//	CPoint  pt = point;
	//	CImageList::DragMove( pt );//移动正在拖放操作中被拖动的图像

	//	//鼠标经过时高亮显示
	//	CImageList::DragShowNolock( false );  //避免鼠标经过时留下难看的痕迹
	//	if( (m_hReceved = HitTest(point, &m_uDragFlags)) != NULL )
	//	{
	//		SelectDropTarget( m_hReceved );
	//	}
	//	CImageList::DragShowNolock( true );

 //       
 //       //判断接受拖动的节点与被拖动节点的层次关系
 //       BOOL bCancel = FALSE;
 //       if(m_hReceved  == m_hItemDrag)
 //       {
 //           bCancel = TRUE;
 //       }
 //       else
 //       {
	//        HTREEITEM  htiParent = m_hReceved;
	//        while( (htiParent = GetParentItem(htiParent)) != NULL )
	//        {
 //               //接受拖动的节点是被拖动节点的子节点的情况
	//	        if( htiParent == m_hItemDrag )
	//	        {
 //                   bCancel = TRUE;
 //                   break;
 //               }
 //           }
 //       }

 //       if(bCancel) // 取消拖放
 //       {
 //           m_hReceved     = m_hItemDrag;
 //           m_hNewParent   = m_hItemDrag;
 //           m_hNewSibling  = m_hItemDrag;
 //           ::SetCursor(m_hDragCancel);
 //       }
 //       else if( m_uDragFlags  & TVHT_ONITEMSTATEICON) // 添加接受节点之前
 //       {
 //           m_hNewParent   = GetParentItem(m_hReceved);
 //           m_hNewSibling  = m_hReceved;
 //           ::SetCursor(m_hDragAbove  );
 //       }
 //       else if(m_uDragFlags & TVHT_ONITEMICON) // 添加到接受节点之后
 //       {
	//	    m_hNewParent  = GetParentItem(m_hReceved);
 //           m_hNewSibling = GetNextSiblingItem(m_hReceved);
	//		m_hNewSibling = (m_hNewSibling==NULL)?TVI_LAST:m_hNewSibling;
 //           ::SetCursor(m_hDragBelow);
 //       }
 //       else if(m_uDragFlags & TVHT_ONITEMLABEL) // 添加为接受节点的孩子节点
 //       {
 //           m_hNewParent  = m_hReceved;
 //           m_hNewSibling = TVI_LAST;
 //           ::SetCursor(m_hDragAppend);
 //       }
 //       else if(m_uDragFlags & TVHT_TORIGHT) // 添加到所有节点之后
 //       {
 //           m_hNewParent  = NULL;
 //           m_hNewSibling = TVI_LAST;
 //           ::SetCursor(m_hDragRight);
 //       }
 //       else if(m_uDragFlags & TVHT_TOLEFT) // 添加到所有节点之前
 //       {
 //           m_hNewParent  = NULL;
 //           m_hNewSibling = NULL;
 //           ::SetCursor(m_hDragLeft);
 //       }
 //       else if(m_uDragFlags & TVHT_ABOVE) // 添加到所有节点之前
 //       {
 //           m_hNewParent  = NULL;
 //           m_hNewSibling = NULL;
 //           ::SetCursor(m_hDragAbove);
 //       }
 //       else if(m_uDragFlags & TVHT_BELOW) // 添加到所有节点之后
 //       {
 //           m_hNewParent  = NULL;
 //           m_hNewSibling = TVI_LAST;
 //           ::SetCursor(m_hDragBelow);
 //       }
 //       else // 取消拖放
 //       {
 //           m_hReceved    = m_hItemDrag;
 //           m_hNewParent  = m_hItemDrag;
 //           m_hNewSibling = m_hItemDrag;
 //           ::SetCursor(m_hDragCancel);
 //       }
	//}
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

//获取上一个相邻的item
HTREEITEM CMultiTreeCtrl::FindPrevItem(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		HTREEITEM hPrev = GetNextItem(hItem, TVGN_PREVIOUS);
		if (hPrev != NULL)
		{
			hItem = GetNextItem(hPrev, TVGN_CHILD);
			while (hItem != NULL)
			{
				while (hItem != NULL)//
				{
					hPrev = hItem;
					hItem = GetNextItem(hPrev, TVGN_NEXT);
				}
				hItem = GetNextItem(hPrev, TVGN_CHILD);
			}
			return hPrev;
		}
		else
		{
			hPrev = GetNextItem(hItem, TVGN_PARENT);

			if (hPrev != NULL)
			{
				return hPrev;
			}
		}
	}
	return NULL;
}


//获取下一个相邻的item
HTREEITEM CMultiTreeCtrl::FindNextItem(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		HTREEITEM hNext = GetNextItem(hItem, TVGN_CHILD);
		if (hNext != NULL)
		{
			return hNext;
		}
		else
		{
			while (hItem != NULL)
			{
				hNext = GetNextItem(hItem, TVGN_NEXT);
				if (hNext != NULL)
				{
					return hNext;
				}
				else
				{
					hItem = GetNextItem(hItem, TVGN_PARENT);
				}
			}
		}
	}

	return NULL;
}


// 拷贝条目
HTREEITEM CMultiTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT  tvstruct;
	HTREEITEM        hNewItem;
	CString          sText;

	// 得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength ();
	tvstruct.item.pszText    = sText.LockBuffer ();

	// 将条目插入到合适的位置
	tvstruct.hParent         = htiNewParent;
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer ();

	// 限制拷贝条目数据和条目状态
	SetItemData ( hNewItem, GetItemData(hItem) );
	SetItemState( hNewItem, GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}


// 拷贝分支
HTREEITEM CMultiTreeCtrl::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM  hChild;
	HTREEITEM  hNewItem = CopyItem( htiBranch, htiNewParent, htiAfter );
	hChild = GetChildItem( htiBranch );

	while( hChild != NULL )
	{
		CopyBranch( hChild,hNewItem,htiAfter );
		hChild = GetNextSiblingItem( hChild );
	}

	return  hNewItem;
}

//处理节点拖动消息
BOOL CMultiTreeCtrl::OnMakeDrag(BOOL bRemove)
{
    //判断接受拖动的节点与被拖动节点是否相同
	if( m_hItemDrag == m_hReceved )
	{
		return FALSE;
	}

    //接受拖动的节点不是被拖动节点的子节点的情况情况
    //直接拷贝被拖动节点分支到接受拖动的节点下
    if(m_hNewSibling == TVI_LAST || m_hNewSibling == TVI_FIRST)
    {
	    HTREEITEM  htiNew   = CopyBranch( m_hItemDrag, m_hNewParent, m_hNewSibling );
	    SelectItem( htiNew );
    }
    else
    {
        HTREEITEM  hitAfter = GetPrevSiblingItem(m_hNewSibling);
        if(hitAfter == NULL) hitAfter = TVI_FIRST;
	    HTREEITEM  htiNew   = CopyBranch( m_hItemDrag, m_hNewParent, hitAfter );
	    SelectItem( htiNew );
    }

    if(bRemove)
    {
        DeleteItem( m_hItemDrag );
    }
    return TRUE;
}

void CMultiTreeCtrl::SetCBAddItems( CB_ADDITEMS cbAddItems )
{
	m_CTreeDropTarget.SetCBAddItems(cbAddItems);
}