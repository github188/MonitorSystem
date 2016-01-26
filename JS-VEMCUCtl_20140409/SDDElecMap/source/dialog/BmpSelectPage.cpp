// BmpSelectPage.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "../../resource.h"
#include "dialog/BmpSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpSelectPage property page

IMPLEMENT_DYNCREATE(CBmpSelectPage, CPropertyPage)

CBmpSelectPage::CBmpSelectPage() : CPropertyPage(CBmpSelectPage::IDD)
{
	//{{AFX_DATA_INIT(CBmpSelectPage)
	m_bmpName = _T("");
	m_bTranslate = FALSE;
	m_bl_Scale_Hor = FALSE;
	m_bl_Scale_Ver = FALSE;
	//}}AFX_DATA_INIT
}

CBmpSelectPage::~CBmpSelectPage()
{

}

void CBmpSelectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBmpSelectPage)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_bmpName);
	DDV_MaxChars(pDX, m_bmpName, 32);
	DDX_Check(pDX, IDC_Translate, m_bTranslate);
	DDX_Check(pDX, IDC_CHECK_HOR, m_bl_Scale_Hor);
	DDX_Check(pDX, IDC_CHECK_VER, m_bl_Scale_Ver);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBmpSelectPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBmpSelectPage)
	ON_BN_CLICKED(IDC_BMP, OnBmpClicked)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpSelectPage message handlers

void CBmpSelectPage::SetContent(LPCSTR bmpname, BOOL bTranslate, COLORREF translatecolor,BOOL blHScale,BOOL blVScale)
{
  m_bmpName=bmpname;
	m_bTranslate=bTranslate;
	m_Color=translatecolor;
	m_bl_Scale_Hor=blHScale;
	m_bl_Scale_Ver=blVScale;
}

CString CBmpSelectPage::GetContent(BOOL &bTranslate, COLORREF &translatecolor,BOOL* blHScale,BOOL* blVScale)
{
	bTranslate=m_bTranslate;
	translatecolor=m_ctrlColor.m_Color;
	
	if (blHScale!=NULL) 
	{
		*blHScale=m_bl_Scale_Hor;
	}

	if (blVScale!=NULL)
	{
		*blVScale=m_bl_Scale_Ver;
	}

	return m_bmpName;
}


BOOL CBmpSelectPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////////////
	m_ctrlColor.SubclassDlgItem(IDC_COLOR,this);
	m_ctrlBmp.SubclassDlgItem(IDC_BMP,this);

	FillListBox();

	if(!m_bmpName.IsEmpty())
     m_List.SelectString(0,m_bmpName);

	OnSelchangeList1();
	m_ctrlBmp.m_translatecolor=m_Color;
	OnBmpClicked();
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBmpSelectPage::FillListBox()
{
	CFileFind  bmpdir;
	CString file="";//theApp.GetDirectory();
	CString bmpname;
	file+="global\\bmp\\*.bmp";

	m_List.ResetContent();
	if (bmpdir.FindFile(file))
	{
		while (bmpdir.FindNextFile())
		{
			 bmpname=bmpdir.GetFileTitle();
			 m_List.AddString(bmpname);
		}

		bmpname=bmpdir.GetFileTitle();
		m_List.AddString(bmpname);

		bmpdir.Close();
	}
}

void CBmpSelectPage::OnBmpClicked() 
{
	m_Color=m_ctrlBmp.m_translatecolor;
	m_ctrlColor.m_Color=m_Color;
	m_ctrlColor.Invalidate();
}

void CBmpSelectPage::OnSelchangeList1() 
{
	int pos = m_List.GetCurSel();
	CString bmpname;
	
	if (pos>=0)
	{
		m_List.GetText(pos,bmpname);
		m_bmpName=bmpname;
		CString file;
		//char chpath[MAX_PATH];
		//GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
		file.Format("global\\bmp\\%s.bmp",bmpname);//chpath,theApp.GetDirectory()
		m_ctrlBmp.SetBmpFile(file);
	}
	
	UpdateData(FALSE);
}

void CBmpSelectPage::OnPreView() 
{
	
}
/////////////////////////////////////////////////////////////////////////////
// CStaticBmp

CStaticBmp::CStaticBmp()
{

}

CStaticBmp::~CStaticBmp()
{
}


BEGIN_MESSAGE_MAP(CStaticBmp, CStatic)
	//{{AFX_MSG_MAP(CStaticBmp)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticBmp message handlers

void CStaticBmp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//////////////////////////////////////////////////////////////////////////
	CRect rt;
	GetClientRect(&rt);

	if (HBITMAP(m_bmp) == NULL) return;

	BITMAP bm;
	m_bmp.GetBitmap(&bm);
	int nWidth  = bm.bmWidth;
	int nHeight = bm.bmHeight;

	float scale=__max((float)nWidth/(float)(rt.Width()),(float)nHeight/(float)(rt.Height()));

 	CDC memdc;
	memdc.CreateCompatibleDC(&dc);

	CBitmap *pOldBmp = memdc.SelectObject(&m_bmp);
	dc.FillSolidRect(&rt,RGB(192,192,192));

	if (scale<=1)
	{
		dc.BitBlt(0,0,nWidth,nHeight,&memdc,0,0,SRCCOPY);
	}
	else 
	{
		dc.StretchBlt(0,0,(int)(nWidth/scale),(int)(nHeight/scale),&memdc,0,0,nWidth,nHeight,SRCCOPY);
	}

	memdc.SelectObject(pOldBmp);
}

void CStaticBmp::SetBmpFile(LPCSTR file)
{
	if (stricmp(m_bmpfile,file) == 0) return;
	
	m_bmpfile = file;
	
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,m_bmpfile, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_bmp.DeleteObject();
	m_bmp.Attach(hBmp);

	CDC memdc;
	CClientDC dc(this);
	memdc.CreateCompatibleDC(&dc);

	CBitmap *pOldBmp = memdc.SelectObject(&m_bmp);
	m_translatecolor = memdc.GetPixel(1,1);
	memdc.SelectObject(pOldBmp);

	Invalidate(FALSE);
}


BOOL CStaticBmp::DestroyWindow() 
{
	m_bmp.DeleteObject();
	return CStatic::DestroyWindow();
}

void CStaticBmp::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	
	m_translatecolor = dc.GetPixel(point.x,point.y);

	CStatic::OnLButtonDown(nFlags, point);
}
