// PropLine.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropLinkLine.h"


// CPropLinkLine 对话框

IMPLEMENT_DYNAMIC(CPropLinkLine, CPropertyPage)
CPropLinkLine::CPropLinkLine()
	: CPropertyPage(CPropLinkLine::IDD)
	
	, m_KZCaption(_T(""))
	, m_LineNameCaption(_T(""))

	, m_LineYC(FALSE)
	, m_LineYX(FALSE)
	, m_LineDiandu(FALSE)
	, m_LineSoe(FALSE)
	, m_LineEpd(FALSE)
{
}

CPropLinkLine::~CPropLinkLine()
{
}

void CPropLinkLine::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);	
	DDX_Text(pDX, IDC_LINEKZ_CAPTION, m_KZCaption);
	DDX_Text(pDX, IDC_LINENAME_CAPTION, m_LineNameCaption);

	DDX_Check(pDX, IDC_LINE_YC, m_LineYC);
	DDX_Check(pDX, IDC_LINE_YX, m_LineYX);
	DDX_Check(pDX, IDC_LINE_DIANDU, m_LineDiandu);
	DDX_Check(pDX, IDC_LINESOE, m_LineSoe);
	DDX_Check(pDX, IDC_LINEEPD, m_LineEpd);
}


BEGIN_MESSAGE_MAP(CPropLinkLine, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropLinkLine 消息处理程序
void CPropLinkLine::GetLineParam(char text[],char LineCaption[],BOOL& bLinesoe,BOOL& bLineyc,BOOL& bLineycspecial, BOOL& bLinediandu,BOOL& bLineEpd)
{
	strcpy(text ,m_KZCaption) ;
	strcpy(LineCaption ,m_LineNameCaption) ;
	bLinesoe = (m_LineSoe == 1) ? true : false ;
	bLineEpd = (m_LineEpd == 1) ? true : false ;
	bLineyc = (m_LineYC == 1) ? true : false ;
	bLineycspecial = (m_LineYX == 1) ? true : false ;
	bLinediandu = (m_LineDiandu == 1) ? true : false ;

}

void CPropLinkLine::SetLineParam(char text[],char LineCaption[],BOOL bLinesoe,BOOL bLineyc,BOOL bLineycspecial, BOOL bLinediandu,BOOL bLineEpd)
{
	m_KZCaption = text ;
	m_LineNameCaption = LineCaption ;
	m_LineSoe = (bLinesoe) ? 1 : 0 ;
	m_LineEpd = (bLineEpd) ? 1 : 0 ;
	m_LineYC = (bLineyc) ? 1 : 0 ;
	m_LineYX = (bLineycspecial) ? 1 : 0 ;
	m_LineDiandu = (bLinediandu) ? 1 : 0 ;
}

void CPropLinkLine::OnBnClickedButton1()
{
	
}
