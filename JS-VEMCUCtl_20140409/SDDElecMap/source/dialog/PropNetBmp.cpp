// PropNetBmp.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropNetBmp.h"
#include "dialog/BmpSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropNetBmp property page

IMPLEMENT_DYNCREATE(CPropNetBmp, CPropertyPageEx)

CPropNetBmp::CPropNetBmp() : CPropertyPageEx(CPropNetBmp::IDD)
{
	//{{AFX_DATA_INIT(CPropNetBmp)
	m_bmp_name = _T("");
	//}}AFX_DATA_INIT
}

CPropNetBmp::~CPropNetBmp()
{
}

void CPropNetBmp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropNetBmp)
	DDX_Text(pDX, IDC_EDIT1, m_bmp_name);
	DDV_MaxChars(pDX, m_bmp_name, 33);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropNetBmp, CPropertyPageEx)
	//{{AFX_MSG_MAP(CPropNetBmp)
	ON_BN_CLICKED(IDC_BMP_SELECT, OnBmpSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropNetBmp message handlers

void CPropNetBmp::OnBmpSelect() 
{
	// TODO: Add your control notification handler code here

	BOOL bl;
	COLORREF color;

	CPropertySheet sheet( _T("Î»Í¼ä¯ÀÀ..") );
	CBmpSelectPage dlg;
	dlg.SetContent(m_bmp_name,FALSE,RGB(0,0,0));
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		m_bmp_name=dlg.GetContent(bl,color);
		UpdateData(FALSE);
	}
	
}

void CPropNetBmp::Set_Bmp_Name(LPCSTR bmpname)
{
  m_bmp_name=bmpname;
}

void CPropNetBmp::Get_Bmp_Name(char *bmpname, int size)
{
  int len=m_bmp_name.GetLength();
	ASSERT(size>=len);
	ASSERT(bmpname!=NULL);
	if ((size>len)&&(bmpname!=NULL))
		strcpy(bmpname,m_bmp_name);
}
