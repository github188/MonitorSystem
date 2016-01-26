// DragDropTreeCtrl.h
// For OLE Drag and Drop between tree controls
// Designed and developed by Vinayak Tadas
// vinayakt@aditi.com
// 

#if !defined(AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_)
#define AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragDropTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl window
#include "TreeDropTarget.h"

// *****************************************************************
// CDragDropTreeCtrl control
// *****************************************************************

class CDragDropTreeCtrl : public CTreeCtrl
{
// Construction
public:
	
	CDragDropTreeCtrl();

// Attributes
public:
	// Adds an item to the tree control
	HTREEITEM AddItem( HTREEITEM hParent, CString csItemName, 
	HTREEITEM hInsAfter, int iSelImage , int iNonSelImage, 
	long lParam);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragDropTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragDropTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CTreeDropTarget m_CTreeDropTarget;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_)
