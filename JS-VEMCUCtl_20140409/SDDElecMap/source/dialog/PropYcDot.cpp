// PropYcDot.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYcDot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropYcDot property page

IMPLEMENT_DYNCREATE(CPropYcDot, CPropertyPage)

CPropYcDot::CPropYcDot() : CPropertyPage(CPropYcDot::IDD)
{
	//{{AFX_DATA_INIT(CPropYcDot)
	m_dotName = _T("");
	//}}AFX_DATA_INIT
}

CPropYcDot::~CPropYcDot()
{
}

void CPropYcDot::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropYcDot)
	DDX_Text(pDX, IDC_EDIT1, m_dotName);
	DDV_MaxChars(pDX, m_dotName, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropYcDot, CPropertyPage)
	//{{AFX_MSG_MAP(CPropYcDot)
	ON_BN_CLICKED(IDC_SELECT_DOT, OnSelectDot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropYcDot message handlers

void CPropYcDot::OnSelectDot() 
{

}

void CPropYcDot::Set_Yc_Dot_Name(LPCSTR buf)
{
	m_dotName=buf;
}

void CPropYcDot::Get_Yc_Dot_Name(char buf[], DWORD bufsize)
{
	int len=m_dotName.GetLength();
	ASSERT((len<=(int)bufsize));
	ASSERT(buf!=NULL);
	if (((int)bufsize>=len)&&(buf!=NULL))
		strcpy(buf,m_dotName);
}
