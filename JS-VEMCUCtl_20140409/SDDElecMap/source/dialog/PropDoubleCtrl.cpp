// CPropDoubleCtrl.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropDoubleCtrl.h"


// CPropDoubleCtrl 对话框

IMPLEMENT_DYNAMIC(CPropDoubleCtrl, CPropertyPage)
CPropDoubleCtrl::CPropDoubleCtrl()
	: CPropertyPage(CPropDoubleCtrl::IDD)
	, m_sCtrlName(_T(""))
	, m_sOprtName1(_T(""))
	, m_sOprtName2(_T(""))
	, m_sYKName1(_T(""))
	, m_sYKName2(_T(""))
	, m_nActType1(0)
	, m_nActType2(0)
	, m_bOnceQuit(FALSE)
{
}

CPropDoubleCtrl::~CPropDoubleCtrl()
{
}

void CPropDoubleCtrl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_CTRLNAME, m_sCtrlName);
	DDX_Text(pDX, IDC_OPRTNAME1, m_sOprtName1);
	DDV_MaxChars(pDX, m_sOprtName1, 10);
	DDX_Text(pDX, IDC_YKNAME1, m_sYKName1);
	DDV_MaxChars(pDX, m_sYKName1, 16);
	DDX_Text(pDX, IDC_OPRTNAME2, m_sOprtName2);
	DDV_MaxChars(pDX, m_sOprtName2, 10);
	DDX_Text(pDX, IDC_YKNAME2, m_sYKName2);
	DDV_MaxChars(pDX, m_sYKName2, 16);	
	DDX_Radio(pDX, IDC_ACTTYPE1, m_nActType1);
	DDX_Radio(pDX, IDC_ACTTYPE2, m_nActType2);
	DDX_Check(pDX, IDC_ONCEQUIT, m_bOnceQuit);
}


BEGIN_MESSAGE_MAP(CPropDoubleCtrl, CPropertyPage)
	ON_BN_CLICKED(IDC_SELYK1, OnBnClickedSelyk1)
	ON_BN_CLICKED(IDC_SELYK2, OnBnClickedSelyk2)
END_MESSAGE_MAP()


// CPropDoubleCtrl 消息处理程序

void CPropDoubleCtrl::OnBnClickedSelyk1()
{

}

void CPropDoubleCtrl::OnBnClickedSelyk2()
{

}

void CPropDoubleCtrl::SetParam(char ctrlname[], char oprtname1[], char ykname1[], int acttype1, char oprtname2[], char ykname2[], int acttype2, bool oncequit)
{
	m_sCtrlName = ctrlname;
	m_sOprtName1 = oprtname1;
	m_sOprtName2 = oprtname2;
	m_sYKName1 = ykname1;
	m_sYKName2 = ykname2;
	m_nActType1 = acttype1;
	m_nActType2 = acttype2;
	m_bOnceQuit = oncequit;
}

void CPropDoubleCtrl::GetParam(char ctrlname[], char oprtname1[], char ykname1[], int &acttype1, char oprtname2[], char ykname2[], int &acttype2, bool &oncequit)
{
	strcpy(ctrlname, m_sCtrlName);
	strcpy(oprtname1, m_sOprtName1);
	strcpy(oprtname2, m_sOprtName2);
	strcpy(ykname1, m_sYKName1);
	strcpy(ykname2, m_sYKName2);
	acttype1 = m_nActType1;
	acttype2 = m_nActType2;
	oncequit = m_bOnceQuit;
}
