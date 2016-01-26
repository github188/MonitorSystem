// SelectVectorDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SelectVectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorStatic

CVectorStatic::CVectorStatic()
{
}

CVectorStatic::~CVectorStatic()
{
}


BEGIN_MESSAGE_MAP(CVectorStatic, CStatic)
	//{{AFX_MSG_MAP(CVectorStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorStatic message handlers

void CVectorStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt1;
	GetClientRect(&rt1);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));
	CBrush* pOldBrush=dc.SelectObject(&brush);
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(192,192,192));
	CPen *poldpen=dc.SelectObject(&pen);

	dc.Rectangle(&rt1);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(poldpen);

	CSize sz=m_vector.GetSize();
	if ((sz.cx==0)||(sz.cy==0)) return;
	float scaleX=1;
	float scaleY=1;

	if((rt1.Width()<sz.cx)||(rt1.Height()<sz.cy))
	{
		if (rt1.Width()<sz.cx)
			scaleX=((float)rt1.Width())/(float)(sz.cx+1);
		if (rt1.Height()<sz.cy)
			scaleY=((float)rt1.Height())/(float)(sz.cy+1);
		float scale=__max(scaleX,scaleY);
		sz.cx=__min(rt1.Width()-10,(int)((float)(sz.cx-1)*scale));
		sz.cy=__min(rt1.Height()-10,(int)((float)(sz.cy-1)*scale));

	}
	else
	{
		if (rt1.Width()>sz.cx)
			scaleX=(int)((float)rt1.Width())/(float)(sz.cx+1);
		if (rt1.Height()>sz.cy)
			scaleY=(int)((float)rt1.Height())/(float)(sz.cy+1);
		float scale=__min(scaleX,scaleY);
		sz.cx=__min(rt1.Width()-10,(int)((float)(sz.cx-1)*scale));
		sz.cy=__min(rt1.Height()-10,(int)((float)(sz.cy-1)*scale));

	}
	rt1.SetRect(5,5,sz.cx,sz.cy);
	m_vector.Draw(&dc,rt1,rt1,0,FALSE);

	// Do not call CStatic::OnPaint() for painting messages
}


void CVectorStatic::SetVectFile(LPCSTR filepath)
{
  m_vector.Load(filepath);
	Invalidate(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CSelectVectorDlg property page

IMPLEMENT_DYNCREATE(CSelectVectorDlg, CPropertyPage)

CSelectVectorDlg::CSelectVectorDlg() : CPropertyPage(CSelectVectorDlg::IDD)
{
	//{{AFX_DATA_INIT(CSelectVectorDlg)
	m_bFill = FALSE;
	m_vector = _T("");
	//}}AFX_DATA_INIT
}

CSelectVectorDlg::~CSelectVectorDlg()
{
}

void CSelectVectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectVectorDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Check(pDX, IDC_FILL, m_bFill);
	DDX_Text(pDX, IDC_EDIT1, m_vector);
	DDV_MaxChars(pDX, m_vector, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectVectorDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CSelectVectorDlg)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectVectorDlg message handlers

BOOL CSelectVectorDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_ctrl_ToColor.SubclassDlgItem(IDC_COLOR,this);
	m_ctrlStatic.SubclassDlgItem(IDC_VECTOR,this);

	FillBox();
	if(!m_vector.IsEmpty())
     m_list.SelectString(0,m_vector);

	OnSelchangeList1();

	m_ctrl_ToColor.m_Color=m_ToColor;
	m_ctrl_ToColor.Invalidate();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectVectorDlg::SetContent(LPCSTR vectorname, BOOL bFill, COLORREF color)
{
  m_ToColor=color;
	m_bFill=bFill;
	m_vector=vectorname;
}

void CSelectVectorDlg::GetContent(CString &vectorname, BOOL &bFill, COLORREF &color)
{
  color=m_ToColor;
	bFill=m_bFill;
	vectorname=m_vector;
}


void CSelectVectorDlg::FillBox()
{
	 CFileFind  vectordir;
	 char chpath[MAX_PATH];  
	 GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));  
	 CString file;//=theApp.GetDirectory();
	 file.Format("%s",chpath);
	 CString vectorname;
	 file+="global\\vec\\*.vec";

	 m_list.ResetContent();
	 if (vectordir.FindFile(file))
	 {
	   while (vectordir.FindNextFile())
		{
			 vectorname=vectordir.GetFileTitle();
			 m_list.AddString(vectorname);
		}
	  vectorname=vectordir.GetFileTitle();
	  m_list.AddString(vectorname);

		vectordir.Close();
	}
}

void CSelectVectorDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	int pos=m_list.GetCurSel();
	CString vectorname;
	if (pos>=0)
	{
		m_list.GetText(pos,vectorname);
		m_vector=vectorname;
		CString file;
		char chpath[MAX_PATH];  
		GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));  
		file.Format("%sglobal\\vec\\%s.vec",chpath,vectorname);//theApp.GetDirectory()
		m_ctrlStatic.SetVectFile(file);

	}
	UpdateData(FALSE);
}


void CSelectVectorDlg::OnColor() 
{
	// TODO: Add your control notification handler code here
	if (m_ToColor==m_ctrl_ToColor.m_Color) return;
  
 	m_ToColor=m_ctrl_ToColor.m_Color;
}
