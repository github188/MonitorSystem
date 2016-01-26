#pragma once


// CImageView

class CImageView : public CWnd
{
	DECLARE_DYNAMIC(CImageView)

public:
	CImageView();
	virtual ~CImageView();

	void CreateWnd(CRect& WndRect,CWnd* pParentWnd);
	void LoadBitmapFile(CString strFileName);

public:
	BOOL m_bFileExsit;

public:
	CRect m_WndRect;

	CBitmap m_bitmap;

	CString m_strFileName;
	bool m_video_opened;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCloseVideo();
};