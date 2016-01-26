// TreeDropTarget.cpp : implementation file
// For OLE Drag and Drop between tree controls
// Designed and developed by Vinayak Tadas
// vinayakt@aditi.com
// 

#include "stdafx.h"
#include "TreeDroptarget.h"
#include "DragDropTreeCtrl.h"

#define RECT_BORDER	10

HWND CTreeDropTarget::m_shWndTreeCtrl = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTreeDropTarget

/********************************************************************
OnDragEnter()
	Called when the user drags the object in Tree control.
********************************************************************/
DROPEFFECT CTreeDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point 
)
{
	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;
	return dropeffectRet;
}

/********************************************************************
OnDragOver()
	Called when the user drags the object over Tree control.
********************************************************************/
		
DROPEFFECT CTreeDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
	DWORD dwKeyState, CPoint point )
{
	
	DROPEFFECT dropeffectRet = DROPEFFECT_COPY;
	if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropeffectRet = DROPEFFECT_MOVE;
	// Expand and highlight the item under the mouse and 
	m_pDestTreeCtrl = (CMultiTreeCtrl *)pWnd;
	HTREEITEM hTItem = m_pDestTreeCtrl->HitTest(point);
	if ( hTItem != NULL ) 
	{
		m_pDestTreeCtrl->Expand(hTItem, TVE_EXPAND);
		m_pDestTreeCtrl->SelectDropTarget(hTItem);
	}	
	
	// Scroll Tree control depending on mouse position
	CRect rectClient;
	pWnd->GetClientRect(&rectClient);
	pWnd->ClientToScreen(rectClient);
	pWnd->ClientToScreen(&point);
	int nScrollDir = -1;
	if ( point.y >= rectClient.bottom - RECT_BORDER)
		nScrollDir = SB_LINEDOWN;
	else
	if ( (point.y <= rectClient.top + RECT_BORDER) )
		nScrollDir = SB_LINEUP;

	
	if ( nScrollDir != -1 ) 
	{
		int nScrollPos = pWnd->GetScrollPos(SB_VERT);
		WPARAM wParam = MAKELONG(nScrollDir, nScrollPos);
		pWnd->SendMessage(WM_VSCROLL, wParam);
	}
	
	nScrollDir = -1;
	if ( point.x <= rectClient.left + RECT_BORDER )
		nScrollDir = SB_LINELEFT;
	else
	if ( point.x >= rectClient.right - RECT_BORDER)
		nScrollDir = SB_LINERIGHT;
	
	if ( nScrollDir != -1 ) 
	{
		int nScrollPos = pWnd->GetScrollPos(SB_VERT);
		WPARAM wParam = MAKELONG(nScrollDir, nScrollPos);
		pWnd->SendMessage(WM_HSCROLL, wParam);
	}
	
	return dropeffectRet;
	
	
}

/********************************************************************
OnDragLeave()
	Called when the user drags the object out of Tree control.
********************************************************************/
void CTreeDropTarget::OnDragLeave( CWnd* pWnd )
{
	// Remove Highlighting 
	m_pDestTreeCtrl = (CMultiTreeCtrl *)pWnd;
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	
}

/********************************************************************
OnDrop()
	Called when the user drops the object in the  Tree control.
********************************************************************/
BOOL CTreeDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point)
{
	//Get the selected item from Source and destination Tree controls 
	m_pSourceTreeCtrl = (CMultiTreeCtrl *)CWnd::FromHandlePermanent(m_shWndTreeCtrl);
	HTREEITEM hSelectedItem = m_pSourceTreeCtrl->GetSelectedItem();
	HTREEITEM hTDropItem =m_pDestTreeCtrl->GetDropHilightItem();

	// Get the image indexes from ImageList, if one exists
	int nImage = 0;
	int nSelImage = 0;
	GetItemImages(hSelectedItem, nImage, nSelImage);

	//如果设置了回调函数,则调用回调函数
	if (m_pCBAddItems != NULL)
	{
		m_pCBAddItems(m_pSourceTreeCtrl, m_pDestTreeCtrl, hSelectedItem, hTDropItem, nImage, nSelImage);
		return TRUE;
	}
	
	// Add the items to destination tree control
	m_hDestItem = m_pDestTreeCtrl->AddItem(hTDropItem, m_pSourceTreeCtrl->GetItemText(hSelectedItem), TVI_LAST, nImage, nSelImage, 0);
	AddItem(m_pSourceTreeCtrl->GetChildItem(hSelectedItem),m_hDestItem);
	
	//Remove highlighting
	m_pDestTreeCtrl->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	return TRUE;
	
	
}

/********************************************************************
AddItem()
	Recursive function for adding items from selected tree control to
	destination tree control
********************************************************************/
void CTreeDropTarget::AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem )
{
	int nImage;
	int nSelImage;
	while (	(hSrcTItem != NULL) && (hSrcTItem != m_hDestItem)) 
	{
		// Get the item text and Image indexes for source tree control
		CString csItem = m_pSourceTreeCtrl->GetItemText(hSrcTItem);
		GetItemImages(hSrcTItem, nImage, nSelImage);
		
		// Add item to destination tree control
		HTREEITEM hDestChildItem = m_pDestTreeCtrl->AddItem(hDestTItem, csItem, TVI_LAST, nImage, nSelImage, 0);
		HTREEITEM hSrcChildItem = m_pSourceTreeCtrl->GetChildItem(hSrcTItem);
		
		if(hSrcChildItem != NULL) 
			AddItem(hSrcChildItem, hDestChildItem);
		
		hSrcTItem = m_pSourceTreeCtrl->GetNextSiblingItem(hSrcTItem);
		hDestTItem = m_pDestTreeCtrl->GetParentItem(hDestChildItem);
	}
	
}
/********************************************************************
GetItemImages()
	Gets image indexes associated with a tree item
********************************************************************/
void CTreeDropTarget::GetItemImages(HTREEITEM hSrcTItem, int &nSelItemImage, int &nNonSelItemImage)
{
	
	CImageList *pImageList = m_pSourceTreeCtrl->GetImageList(TVSIL_NORMAL);
	//If no image list is associated with the tree control, return
	if (pImageList == NULL )
	{
		nSelItemImage =0;
		nNonSelItemImage = 0;
	}
	else
	{
		// If no image list is associated with Destination tree control
		// Set the image list of source tree control
		if ( m_pDestTreeCtrl->GetImageList(TVSIL_NORMAL) == NULL )
		{
			m_pDestTreeCtrl->SetImageList(pImageList, TVSIL_NORMAL);
		}
		// Get the image indexes
		m_pSourceTreeCtrl->GetItemImage(hSrcTItem, nSelItemImage, nNonSelItemImage);
	}
}

CTreeDropTarget::CTreeDropTarget()
{
	m_pCBAddItems = NULL;
}

void CTreeDropTarget::SetCBAddItems( CB_ADDITEMS cbAddItems )
{
	m_pCBAddItems = cbAddItems;
}