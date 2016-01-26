#pragma once

#include "EnBitmap.h"


// CDlgShowLinkageServer 对话框

class CDlgShowLinkageServer : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowLinkageServer)

public:
	CDlgShowLinkageServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowLinkageServer();

public:
	CEnBitmap m_Bitmap;
	BITMAP m_Bm;
	HRGN m_hbmpRgn;

public:
	BOOL	m_bMouseIn;
	BOOL	m_bDown;

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOW_LINKAGE_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
