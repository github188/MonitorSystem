// PropYT.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BS_COUNT 32
/////////////////////////////////////////////////////////////////////////////
// CPropYT property page

IMPLEMENT_DYNCREATE(CPropYT, CPropertyPage)

CPropYT::CPropYT() : CPropertyPage(CPropYT::IDD)
{
	//{{AFX_DATA_INIT(CPropYT)
	m_BS_name = _T("");
	m_YK_name = _T("");
	m_FK_name = _T("");
	m_value_type = -1;
	m_BS_VALUE = -1;
	m_YK_Caption = _T("");
	//}}AFX_DATA_INIT
}

CPropYT::~CPropYT()
{
}

void CPropYT::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropYT)
	DDX_Control(pDX, IDC_BTN_MOD, m_mod);
	DDX_Control(pDX, IDC_BTN_DEL, m_del);
	DDX_Control(pDX, IDC_BTN_ADD, m_add);
	DDX_Control(pDX, IDC_LIST1, m_list_bs);
	DDX_Text(pDX, IDC_BS, m_BS_name);
	DDX_Text(pDX, IDC_YK, m_YK_name);
	DDX_Text(pDX, IDC_FK, m_FK_name);
	DDX_Radio(pDX, IDC_value_type, m_value_type);
	DDX_Radio(pDX, IDC_BS_VALUE, m_BS_VALUE);
	DDX_Text(pDX, IDC_YK_Caption, m_YK_Caption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropYT, CPropertyPage)
	//{{AFX_MSG_MAP(CPropYT)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MOD, OnBtnMod)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropYT message handlers

void CPropYT::Copy_BS_Array(tagBS_Array &Src_BS_Array, tagBS_Array &Dst_BS_Array)
{
	tagBSItem* pItem,*pItem2;
	int i;

	for (i=0;i<Dst_BS_Array.GetSize();i++)
	{
		pItem=Dst_BS_Array[i];
		ASSERT(pItem!=NULL);
		delete pItem;
	}

	Dst_BS_Array.RemoveAll();

	for (i=0;i<Src_BS_Array.GetSize();i++)
	{
		pItem=Src_BS_Array[i];
		ASSERT(pItem!=NULL);
		pItem2=new tagBSItem;
		ASSERT(pItem2!=NULL);
		strcpy(pItem2->strYxName,pItem->strYxName);
		pItem2->bValue=pItem->bValue;
		Dst_BS_Array.Add(pItem2);
	}
}

void CPropYT::GetYTParam(char text[], char Ykname[], int &bVT, char Yxname_FK[], tagBS_Array &BS_Array)
{
	strcpy(text,m_YK_Caption);
	strcpy(Ykname,m_YK_name);
	bVT=(m_value_type==0)?TRUE:FALSE;
	strcpy(Yxname_FK,m_FK_name);

	BS_Array.RemoveAll();
	Copy_BS_Array(m_BS_Array,BS_Array);
}

void CPropYT::SetYTParam(char text[], char Ykname[], int bVT, char Yxname_FK[], tagBS_Array &BS_Array)
{
	m_YK_Caption=text;
	m_YK_name=Ykname;
	m_value_type=(bVT==TRUE)?0:1;
	m_FK_name=Yxname_FK;

	Copy_BS_Array(BS_Array,m_BS_Array);
}

void CPropYT::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	tagBSItem* pItem;
	pItem=new tagBSItem;
	ASSERT(pItem!=NULL);
	if (pItem!=NULL)
	{
		strcpy(pItem->strYxName,m_BS_name);
		pItem->bValue=(BYTE)(m_BS_VALUE==0)?1:0;
		m_BS_Array.Add(pItem);
		CString str;
		str.Format("%-16s,%d",m_BS_name,pItem->bValue);
		m_list_bs.AddString(str);
	}

	Update_BS_UI();		
}

void CPropYT::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	int id=m_list_bs.GetCurSel();
	ASSERT(id!=LB_ERR);

	tagBSItem* pItem;

	CString str;
	pItem=m_BS_Array[id];
	ASSERT(pItem!=NULL);
	str.Format("%-16s,%d",pItem->strYxName,pItem->bValue);
	if (::MessageBox(m_hWnd,str,"É¾³ý±ÕËøÌõ¼þÏî",MB_OKCANCEL)==IDOK)  
	{
		m_BS_Array.RemoveAt(id);
		delete pItem;
		m_list_bs.DeleteString(id);
	}

	Update_BS_UI();	
}

void CPropYT::OnBtnMod() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int id=m_list_bs.GetCurSel();
	ASSERT(id!=LB_ERR);

	tagBSItem* pItem;

	CString str;
	pItem=m_BS_Array[id];
	ASSERT(pItem!=NULL);
	strcpy(pItem->strYxName,m_BS_name);
	pItem->bValue=(BYTE)(m_BS_VALUE==0)?1:0;

	str.Format("%-16s,%d",m_BS_name,pItem->bValue);
	m_list_bs.DeleteString(id);
	m_list_bs.InsertString(id,str);

	Update_BS_UI();	
}

BOOL CPropYT::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list_bs.ResetContent();

	tagBSItem* pItem;
	CString str;
	for (int i=0;i<m_BS_Array.GetSize();i++)
	{
		pItem=m_BS_Array[i];
		ASSERT(pItem!=NULL);
		str.Format("%-16s,%d",pItem->strYxName,pItem->bValue);
		m_list_bs.AddString(str);
	}

	Update_BS_UI();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropYT::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	Update_BS_UI();
	UpdateData(FALSE);	
}

void CPropYT::Update_BS_UI()
{
	if (m_BS_Array.GetSize()<MAX_BS_COUNT)
	  m_add.EnableWindow(TRUE);
	else 
		m_add.EnableWindow(FALSE);
	int id=m_list_bs.GetCurSel();

	if (id!=LB_ERR) 
	{
		m_del.EnableWindow(TRUE);
		m_mod.EnableWindow(TRUE);
		tagBSItem* pItem=m_BS_Array[id];
		ASSERT(pItem!=NULL);
		m_BS_name=pItem->strYxName;
		m_BS_VALUE=(pItem->bValue==0)?0:1;
	} 
	else
	{
		m_del.EnableWindow(FALSE);
		m_mod.EnableWindow(FALSE);
	}
}

void CPropYT::OnButton4() 
{
	
}

void CPropYT::OnButton5() 
{
	
}

void CPropYT::OnButton6() 
{
	
}
