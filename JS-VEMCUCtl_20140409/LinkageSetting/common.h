#ifndef _COMMON_H_
#define _COMMON_H_

const CRegexpT<TCHAR> g_regexpIP("([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}");
void FreeTreeNode(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem);
void SetTreeItemData(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem, TREE_NODE_TYPE enumNodeType, char *pData, int nDataLen);
void FreeOneTreeNode(CTreeCtrl* pTreeCtrl, HTREEITEM hTreeItem);
void ExpandAllItems(CTreeCtrl* pTreeList,HTREEITEM hTreeItem);
HTREEITEM FindItem(CTreeCtrl* pTreeCtrl, HTREEITEM item, CString strText);
#endif