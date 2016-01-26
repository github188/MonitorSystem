// SkinStatic.cpp : implementation file
//

#include "stdafx.h"
#include "SkinStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic

CSkinStatic::CSkinStatic()
{
	m_nX=0;
	m_nY=0;
	m_strText="";
}

CSkinStatic::~CSkinStatic()
{

}

BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	//{{AFX_MSG_MAP(CSkinStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic message handlers
void CSkinStatic::LoadBitmap(LPCTSTR szImagePath)
{
	m_bmpStatic.LoadImage(szImagePath); 
}

void CSkinStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);
	CEnBitmap bmp;

	if (m_bmpStatic.m_hObject !=0)
		m_bmpStatic.ExtendDraw(&dc,rcClient,m_nX,m_nY); 

	CRect rc=rcClient;
	rc.left = m_nTextStart;
	DrawText(&dc,rc,m_strText,DT_WORDBREAK);
}

BOOL CSkinStatic::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	GetParent()->SendMessage(WM_COMMAND,wParam,lParam); 	
	return CStatic::OnCommand(wParam, lParam);
}

void CSkinStatic::SetText(CString strText, int nStart,COLORREF cr)
{
	m_strText=strText;
	m_nTextStart=nStart;
	m_crText=cr;
}

void CSkinStatic::SetText(char * pszText,int nStart,COLORREF cr)
{
	m_strText = pszText;
	m_nTextStart = nStart;
	m_crText = cr;
}

CString& CSkinStatic::GetText()
{
	return m_strText;
}

BOOL CSkinStatic::DrawText(CDC *pDC,CRect rc ,CString strText,UINT uFormat)
{
	CRect r;
	CString str;
	CRect rcButton;
	if (strText.GetLength() )
	{
		CFont *ofont;
		ofont = pDC->SelectObject( GetParent()->GetFont() );

		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText( strText, rc,uFormat);
		pDC->SelectObject(ofont);
	}

	return TRUE;
}