// PropZK.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropZK.h"

// CPropZK 对话框

IMPLEMENT_DYNAMIC(CPropZK, CPropertyPage)
CPropZK::CPropZK()
	: CPropertyPage(CPropZK::IDD)
	, m_zkename(_T(""))
	, m_node(0)
	, m_line(0)
	, m_rtu(0)
{
}

CPropZK::~CPropZK()
{
}

void CPropZK::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FK1, m_zkename);
	DDX_Text(pDX, IDC_FK2, m_node);
	DDX_Text(pDX, IDC_FK3, m_line);
	DDX_Text(pDX, IDC_FK4, m_rtu);
}

BEGIN_MESSAGE_MAP(CPropZK, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CPropZK 消息处理程序

void CPropZK::OnBnClickedButton2()
{
	UpdateData(TRUE);

}


void CPropZK::SetYkParam(char Ykname[],int node,int line,int rtu)
{
	m_zkename=Ykname;
	m_node=node;
	m_line=line;
	m_rtu=rtu;
}

void CPropZK::GetYkParam(char Ykname[],int* node,int* line,int* rtu)

{
	strcpy(Ykname,m_zkename);
    *node=m_node;
	*line=m_line;
	*rtu=m_rtu;
}
