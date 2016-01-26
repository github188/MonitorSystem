/*************************************************
Use at your own risk!
文件名称： MultiTreeCtrl.h
模块设计： 卢业恭
模块编写： 卢业恭
功能： 带复选框的可拖拽的树形控件类．
版本号： V2013.3.13
***************************************************/
#include <vector>
#include "TreeDropTarget.h"
#if !defined(__AN_MULTITREECTRL_H)
#define __AN_MULTITREECTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// 复选框状态
enum CHECK_STATE
{
STATE_INI   = 0,//未设定
STATE_UNSEL = 1,//未选中
STATE_SEL   = 2,//全选中
STATE_PSEL  = 3,//部分选中
};

// 控件主要消息事件ID定义
const int  DRAG_DELAY   = 60;
const UINT TVN_ENDDRAG  = WM_USER+1001; // 拖动事件消息ID
const UINT TVN_UPSTATE  = WM_USER+1002; // 更新状态消息ID
// TVN_ENDLABELEDIT // 标签编辑事件消息ID

/////////////////////////////////////////////////////////////////////////////
// CMultiTreeCtrl —— 带有复选框的树型控件
class CMultiTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CMultiTreeCtrl();

	HTREEITEM     m_hItemClick ;     //被点击的节点
	HTREEITEM     m_hItemDrag  ;     //被拖动的节点
	HTREEITEM     m_hReceved   ;     //接受拖动的节点
	HTREEITEM     m_hNewParent ;     //接受插入的父节点
	HTREEITEM     m_hNewSibling;     //接受插入的节点

    //光标资源，须在类对象外部获取！！！
	HCURSOR m_hDragLeft  ; // 左箭头
	HCURSOR m_hDragRight ; // 右箭头
	HCURSOR m_hDragAbove ; // 向上箭头
	HCURSOR m_hDragBelow ; // 向下箭头
	HCURSOR m_hDragAppend; // 到末尾箭头
	HCURSOR m_hDragCancel; // 取消箭头

// Attributes
protected:
	UINT          m_TimerTicks    ;  //处理滚动的定时器所经过的时间
	UINT          m_nScrollTimerID;  //处理滚动的定时器
	CPoint        m_HoverPoint    ;  //鼠标位置
	UINT          m_nHoverTimerID ;  //鼠标敏感定时器
	DWORD         m_dwDragStart   ;  //按下鼠标左键那一刻的时间
	BOOL          m_bDragging     ;  //标识是否正在拖动过程中
	CImageList*   m_pDragImage    ;  //拖动时显示的图象列表

	UINT          m_uClickFlags;     //鼠标点击状态
	UINT          m_uDragFlags ;     //节点拖动状态
	CTreeDropTarget m_CTreeDropTarget;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	HTREEITEM FindNextItem(HTREEITEM hItem);
	HTREEITEM FindPrevItem(HTREEITEM hItem);
	void      ResetItemSelState(int nState);
	BOOL      SetItemStateEx(HTREEITEM hItem, UINT nState, BOOL bSearch);
    BOOL      OnMakeDrag(BOOL bRemove); // == UpdateDrag(...);
	void GetCheckItems(std::vector<HTREEITEM>& vecTreeItems, HTREEITEM hTreeItem);
	HTREEITEM AddItem( HTREEITEM hParent, CString csItemName, HTREEITEM hInsAfter, int iSelImage , int iNonSelImage, long lParam );
	virtual   ~CMultiTreeCtrl();
	void SetCBAddItems(CB_ADDITEMS cbAddItems);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMultiTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem, int nState);
	HTREEITEM CopyBranch(HTREEITEM htiBranch,HTREEITEM htiNewParent,HTREEITEM htiAfter);
	HTREEITEM CopyItem(HTREEITEM hItem,HTREEITEM htiNewParent,HTREEITEM htiAfter);
}; 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__AN_MULTITREECTRL_H)
