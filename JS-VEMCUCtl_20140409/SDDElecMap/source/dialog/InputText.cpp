// InputText.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/InputText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputText property page

IMPLEMENT_DYNCREATE(CInputText, CPropertyPage)

CInputText::CInputText() : CPropertyPage(CInputText::IDD)
{
	//{{AFX_DATA_INIT(CInputText)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}

CInputText::~CInputText()
{
}

void CInputText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputText)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_Text);
	DDV_MaxChars(pDX, m_Text, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputText, CPropertyPage)
	//{{AFX_MSG_MAP(CInputText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputText message handlers

void CInputText::SetText(LPCSTR str)
{
	m_Text=str;
}

void CInputText::GetText(CString& str)
{
	str=m_Text;
}
