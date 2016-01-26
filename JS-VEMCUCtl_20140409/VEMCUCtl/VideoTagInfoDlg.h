#pragma once


// CVideoTagInfoDlg 对话框

class CVideoTagInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoTagInfoDlg)

public:
	CVideoTagInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoTagInfoDlg();

public:
	BOOL SetVideoInfo(char *pszVideoInfo,int nIndex);
	BOOL SetVideoInfoCurrentIndex(int nIndex);
	BOOL SetFontInfo(int nWidth,int nHeight,int nWeight);
	BOOL SetBrushColor(COLORREF color);
	BOOL HideVideoInfo();
	BOOL ShowVideoInfo(int x,int y,bool bFlag=false);

public:
	CFont   m_font; 
	HBRUSH m_hBrush;
	POINT  m_ptWin;
	char     m_szVideoInfo[3][256];
	int        m_nVideoInfoIndex;
	BOOL   m_bVideoInfoShowFlag;

// 对话框数据
	enum { IDD = IDD_VIDEO_TAG_INFO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
