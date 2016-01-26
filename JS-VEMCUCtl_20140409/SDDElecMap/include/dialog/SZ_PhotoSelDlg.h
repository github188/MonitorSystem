#if !defined(AFX_SZ_PHOTOSELDLG_H__6FA3D7E4_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_PHOTOSELDLG_H__6FA3D7E4_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SZ_PhotoSelDlg.h : header file
//
#include "../../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSZ_PhotoSelDlg dialog

class CSZ_PhotoSelDlg : public CDialog
{
// Construction
public:
	void GetInfo(char* PhotoInfo,char* PhotoName);
	void SetInfo(char* PhotoInfo,char* PhotoName);
	CSZ_PhotoSelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSZ_PhotoSelDlg)
	enum { IDD = IDD_SZ_PHOTO };
	CString	m_photoname;
	CString	m_photoinfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSZ_PhotoSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSZ_PhotoSelDlg)
	virtual void OnOK();
	afx_msg void OnSzBnbmp();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_sPhotoInfo[64];
	char m_sPhotoName[32];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SZ_PHOTOSELDLG_H__6FA3D7E4_3D8D_11D6_A5ED_0080C8F60823__INCLUDED_)
