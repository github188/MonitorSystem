// TWTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

CMyTreeCtrl::CMyTreeCtrl()
{
	m_uFlags=0;
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers

void CMyTreeCtrl::OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}

void CMyTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
	//	UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK );// >> 12;
		BOOL checked = GetCheck(hItem);
		//nState=(nState==3)?1:3;
		SetCheck( hItem, !checked);
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMyTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	//处理空格键
 /*	if(nChar==0x20)
 	{
 		HTREEITEM hItem =GetSelectedItem();
 		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) ;//>> 12;
 		if(nState!=0)
 		{
 			//nState=(nState==3)?1:3;
 			SetItemState( hItem, nState, TVIS_STATEIMAGEMASK );
 		}
 	}
	else */
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	*pResult = 0;
}

BOOL CMyTreeCtrl::SetCheck(HTREEITEM hItem, BOOL fCheck)
{
	BOOL bReturn=CTreeCtrl::SetCheck( hItem, fCheck );

	TravelChild(hItem, fCheck);
	TravelSiblingAndParent(hItem,fCheck);
	return bReturn;
}

void CMyTreeCtrl::TravelChild(HTREEITEM hItem, BOOL fCheck)
{
	HTREEITEM hChildItem,hBrotherItem;
	
	//查找子节点，没有就结束
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
		//设置子节点的状态与当前节点的状态一致
		CTreeCtrl::SetCheck( hChildItem, fCheck );
		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, fCheck);
		
		//处理子节点的兄弟节点和其子节点
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//设置子节点的兄弟节点状态与当前节点的状态一致
			CTreeCtrl::SetCheck( hBrotherItem, fCheck );
			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, fCheck);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}

void CMyTreeCtrl::GetCheckItems(std::vector<HTREEITEM>& vecTreeItems, HTREEITEM hTreeItem)
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

void CMyTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, BOOL fCheck)
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;
	
	//查找父节点，没有就结束
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		BOOL nChecked1=fCheck;//设初始值，防止没有兄弟节点时出错
		
		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nChecked1 = GetCheck( hNextSiblingItem );
			if(nChecked1!=fCheck) 
				break;
			else 
				hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}
		
		if(nChecked1==fCheck)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nChecked1 = GetCheck( hPrevSiblingItem );
				if(nChecked1!=fCheck)
					break;
				else 
					hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}
		
		if(nChecked1==fCheck )
		{
			//如果状态一致，则父节点的状态与当前节点的状态一致
			CTreeCtrl::SetCheck( hParentItem, fCheck);
			//再递归处理父节点的兄弟节点和其父节点
			TravelSiblingAndParent(hParentItem,fCheck);
		}
		else
		{
			//状态不一致，则当前节点的父节点、父节点的父节点……状态均为第三态
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nChecked1 = GetCheck( hParentItem) ;
				if(nChecked1!=0)
				{
					CTreeCtrl::SetCheck( hParentItem, fCheck);
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

