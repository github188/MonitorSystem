#include "stdafx.h"
#include "common.h"
#include "ParamDefine.h"

//释放树节点
void FreeTreeNode(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem)
{
	if (pTreeCtrl == NULL)
		return;

	hTreeItem = pTreeCtrl->GetRootItem();
	if (hTreeItem == NULL)
		return;

	HTREEITEM hNextItem = pTreeCtrl->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		FreeTreeNode(pTreeCtrl, hNextItem);
		hNextItem = pTreeCtrl->GetNextItem(hNextItem, TVGN_NEXT);
	}

	if (hTreeItem != NULL)
	{
		T_ASS_TREE_NODE *pNode = (T_ASS_TREE_NODE*)pTreeCtrl->GetItemData(hTreeItem);
		if (pNode != NULL)
		{
			if (pNode->body != NULL)
			{
				delete[] pNode->body;
				pNode->body = NULL;
			}
			delete pNode;
			pNode = NULL;
			pTreeCtrl->SetItemData(hTreeItem,NULL);
		}
		pTreeCtrl->DeleteItem(hTreeItem);
	}
}

void ExpandAllItems(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	if (hTreeItem == NULL)
	{
		hTreeItem = pTreeList->GetRootItem();
	}

	if (!pTreeList->ItemHasChildren(hTreeItem))
	{
		return;
	}

	HTREEITEM hNextItem = pTreeList->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		ExpandAllItems(pTreeList,hNextItem);
		hNextItem = pTreeList->GetNextItem(hNextItem,TVGN_NEXT);
	}

	pTreeList->Expand(hTreeItem,TVE_EXPAND);
}

void FreeOneTreeNode(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem)
{
	T_ASS_TREE_NODE *pNode = (T_ASS_TREE_NODE*)pTreeCtrl->GetItemData(hTreeItem);
	if (pNode != NULL)
	{
		if (pNode->body != NULL)
		{
			delete[] (char*)pNode->body;
			pNode->body = NULL;
		}
		delete pNode;
		pNode = NULL;
		pTreeCtrl->SetItemData(hTreeItem,NULL);
	}
	pTreeCtrl->DeleteItem(hTreeItem);
}

void SetTreeItemData(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem, TREE_NODE_TYPE enumNodeType, char *pData, int nDataLen)
{
	T_ASS_TREE_NODE *pNode =  new T_ASS_TREE_NODE();
	pNode->enumTreeNodeType = enumNodeType;

	char *pNewData = new char[nDataLen + 1];
	memset(pNewData, 0 , nDataLen + 1);
	memcpy(pNewData, pData, nDataLen);

	pNode->body = pNewData;
	pTreeCtrl->SetItemData(hTreeItem, (DWORD)pNode);
}

//按名称查找树节点
HTREEITEM FindItem(CTreeCtrl* pTreeCtrl, HTREEITEM item, CString strText)
{
	HTREEITEM hFind;

	//空树,直接返回
	if (item == NULL)
	{
		return NULL;
	}

	//遍历查找
	while(item != NULL)
	{
		//当前节点即所需查找节点
		if ((pTreeCtrl->GetItemText(item)).Find(strText) >= 0)
			return item;

		//查找当前节点的子节点
		if (pTreeCtrl->ItemHasChildren(item))
		{
			item = pTreeCtrl->GetChildItem(item);

			//递归调用查找子节点下节点
			hFind = FindItem(pTreeCtrl, item, strText);

			if (hFind)
			{
				return hFind;
			}
			else
			{
				item = pTreeCtrl->GetNextSiblingItem(pTreeCtrl->GetParentItem(item));
			}
		}
		else
		{
			item = pTreeCtrl->GetNextSiblingItem(item);
		}
	}
	return item;
}