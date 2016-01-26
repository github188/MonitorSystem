#if !defined(AFX_SELECTVECTORDLG_H__2DCC8C3D_D17D_4A65_B1A0_BA8A5DF09907__INCLUDED_)
#define AFX_SELECTVECTORDLG_H__2DCC8C3D_D17D_4A65_B1A0_BA8A5DF09907__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SelectVectorDlg.h : header file
//
#include "../../resource.h"
#include "canvas/LineProp.h"
#include "canvas/Vector.h"

/////////////////////////////////////////////////////////////////////////////
// CVectorStatic window

class CVectorStatic : public CStatic
{
// Construction
public:
	CVectorStatic();

// Attributes
public:
  CVector m_vector;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVectorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetVectFile(LPCSTR filepath);
	virtual ~CVectorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVectorStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CSelectVectorDlg dialog

class CSelectVectorDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CSelectVectorDlg)
private:
  CStaticColor  m_ctrl_ReplaceColor;
  CStaticColor  m_ctrl_ToColor;
	CVectorStatic m_ctrlStatic;

	COLORREF m_ReplaceColor;
	COLORREF m_ToColor;


// Construction
public:
	void GetContent(CString& vectorname, BOOL& bFill, COLORREF& color);
	void SetContent(LPCSTR vectorname,BOOL bFill,COLORREF color);
	CSelectVectorDlg();
	~CSelectVectorDlg();

// Dialog Data
	//{{AFX_DATA(CSelectVectorDlg)
	enum { IDD = IDD_VECTOR };
	CListBox	m_list;
	BOOL	m_bFill;
	CString	m_vector;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSelectVectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillBox();
	// Generated message map functions
	//{{AFX_MSG(CSelectVectorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTVECTORDLG_H__2DCC8C3D_D17D_4A65_B1A0_BA8A5DF09907__INCLUDED_)
