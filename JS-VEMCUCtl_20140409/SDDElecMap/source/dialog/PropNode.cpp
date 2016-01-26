// PropNode.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropNode.h"


// CPropNode 对话框

IMPLEMENT_DYNAMIC(CPropNode, CPropertyPage)
CPropNode::CPropNode()
	: CPropertyPage(CPropNode::IDD)
	
	, m_KZCaption(_T(""))
	, m_NodeNameCaption(_T(""))

	, m_NodeYC(FALSE)
	, m_NodeYX(FALSE)
	, m_NodeDiandu(FALSE)
	, m_NodeSoe(FALSE)
	, m_NodeEpd(FALSE)
{
}

CPropNode::~CPropNode()
{
}

void CPropNode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);	
	DDX_Text(pDX, IDC_NODEKZ_CAPTION, m_KZCaption);
	DDX_Text(pDX, IDC_NODENAME_CAPTION, m_NodeNameCaption);

	DDX_Check(pDX, IDC_NODE_YC, m_NodeYC);
	DDX_Check(pDX, IDC_NODE_YX, m_NodeYX);
	DDX_Check(pDX, IDC_NODE_DIANDU, m_NodeDiandu);
	DDX_Check(pDX, IDC_NODESOE, m_NodeSoe);
	DDX_Check(pDX, IDC_NODEEPD, m_NodeEpd);
}


BEGIN_MESSAGE_MAP(CPropNode, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropNode 消息处理程序
void CPropNode::GetNodeParam(char text[],char NodeCaption[],BOOL& bNodesoe,BOOL& bNodeyc,BOOL& bNodeycspecial, BOOL& bNodediandu,BOOL& bNodeEpd)
{
	strcpy(text ,m_KZCaption) ;
	strcpy(NodeCaption ,m_NodeNameCaption) ;
	bNodesoe = (m_NodeSoe == 1) ? true : false ;
	bNodeEpd = (m_NodeEpd == 1) ? true : false;
	bNodeyc = (m_NodeYC == 1) ? true : false ;
	bNodeycspecial = (m_NodeYX == 1) ? true : false ;
	bNodediandu = (m_NodeDiandu == 1) ? true : false ;

}

void CPropNode::SetNodeParam(char text[],char NodeCaption[],BOOL bNodesoe,BOOL bNodeyc,BOOL bNodeycspecial, BOOL bNodediandu,BOOL bNodeEpd)
{
	m_KZCaption = text ;
	m_NodeNameCaption = NodeCaption ;
	m_NodeSoe = (bNodesoe) ? 1 : 0 ;
	m_NodeEpd = (bNodeEpd) ? 1 : 0;
	m_NodeYC = (bNodeyc) ? 1 : 0 ;
	m_NodeYX = (bNodeycspecial) ? 1 : 0 ;
	m_NodeDiandu = (bNodediandu) ? 1 : 0 ;
}

void CPropNode::OnBnClickedButton1()
{
	
}
