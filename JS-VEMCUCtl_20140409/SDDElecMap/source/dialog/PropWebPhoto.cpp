// PropWebPhoto.cpp : 实现文件
//

#include "stdafx.h"
#include "sp_draw.h"
#include "PropWebPhoto.h"


// CPropWebPhoto 对话框

IMPLEMENT_DYNAMIC(CPropWebPhoto, CPropertyPage)
CPropWebPhoto::CPropWebPhoto()
	: CPropertyPage(CPropWebPhoto::IDD)
	, m_discription(_T(""))
	, m_webIP(0)
	, m_serverIP(0)
	, m_serverPort(0)
	, m_groupIP(0)
	, m_groupPort(0)
	, m_unitID(0)
	, m_unitName(_T(""))
	, m_shotID(0)
	, m_shotName(_T(""))
{

}

CPropWebPhoto::~CPropWebPhoto()
{

}

void CPropWebPhoto::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_discription);
	DDV_MaxChars(pDX, m_discription, 32);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_webIP);
	DDX_IPAddress(pDX, IDC_IPADDRESS2, m_serverIP);
	DDX_Text(pDX, IDC_EDIT2, m_serverPort);
	DDX_IPAddress(pDX, IDC_IPADDRESS3, m_groupIP);
	DDX_Text(pDX, IDC_EDIT13, m_groupPort);
	DDX_Text(pDX, IDC_EDIT14, m_unitID);
	DDX_Text(pDX, IDC_EDIT15, m_unitName);
	DDV_MaxChars(pDX, m_unitName, 32);
	DDX_Text(pDX, IDC_EDIT16, m_shotID);
	DDX_Text(pDX, IDC_EDIT17, m_shotName);
	DDV_MaxChars(pDX, m_shotName, 32);
}


BEGIN_MESSAGE_MAP(CPropWebPhoto, CPropertyPage)

END_MESSAGE_MAP()


// CPropWebPhoto 消息处理程序

BOOL CPropWebPhoto::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}






void CPropWebPhoto::SetProp(LPCSTR discription,DWORD webIP,DWORD serverIP,int serverPort,DWORD groupIP,int groupPort,int unitID,LPCSTR unitName,int shotID, LPCSTR shotName)
{
	ASSERT(unitName!=NULL);
	ASSERT(shotName!=NULL);
	ASSERT(NULL!=discription);
	ASSERT(strlen(discription)<=32);
	ASSERT(strlen(unitName)<=32);
	ASSERT(strlen(shotName)<=32);
	if ((NULL==discription)||(NULL==unitName)||(NULL==shotName)||(strlen(discription)>32)||(strlen(unitName)>32)||(strlen(shotName)>32))
		return;
	m_discription=discription;
	m_webIP=webIP;
	m_serverIP=serverIP;
	m_serverPort=serverPort;
	m_groupIP=groupIP;
	m_groupPort=groupPort;
	m_shotID=shotID;
	m_shotName=shotName;
	m_unitID=unitID;
	m_unitName=unitName;

}

void CPropWebPhoto::GetProp(char* discription,DWORD& webIP,DWORD& serverIP,int& serverPort,DWORD& groupIP,int& groupPort,int& unitID, char* unitName,int& shotID, char* shotName)
{
	ASSERT(NULL!=discription);
	ASSERT(unitName!=NULL);
	ASSERT(shotName!=NULL);

	if ((NULL==unitName)||(NULL==shotName)||(NULL==discription))
		return;
	webIP=m_webIP;
	serverIP=m_serverIP;
	serverPort=m_serverPort;
	groupIP=m_groupIP;
	groupPort=m_groupPort;
	unitID=m_unitID;
	shotID=m_shotID;
	if (!m_shotName.IsEmpty())
		strcpy(shotName,m_shotName);
	if (!m_unitName.IsEmpty())
		strcpy(unitName,m_unitName);
	if (!m_discription.IsEmpty())
		strcpy(discription,m_discription);

}

