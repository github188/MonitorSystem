// DragDropTreeCtrl.cpp : implementation file
// For OLE Drag and Drop between tree controls
// Designed and developed by Vinayak Tadas
// vinayakt@aditi.com
// 


#include "stdafx.h"
#include "DragDropTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl

CDragDropTreeCtrl::CDragDropTreeCtrl()
{
	
}

CDragDropTreeCtrl::~CDragDropTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDragDropTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDragDropTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl message handlers
/*******************************************************************
AddItem()
	Helper Function for adding an item to the tree control
********************************************************************/

HTREEITEM CDragDropTreeCtrl::AddItem( 
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


int CDragDropTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
// Register Tree control as a drop target	
	m_CTreeDropTarget.Register(this);
	return 0;
}

/*******************************************************************
OnBegindrag()
	Called when drag operation begins
********************************************************************/
void CDragDropTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
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
