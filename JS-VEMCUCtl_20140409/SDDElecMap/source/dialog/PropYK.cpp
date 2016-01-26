// PropYK.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MAX_BS_COUNT 32

/////////////////////////////////////////////////////////////////////////////
// CPropYK property page

IMPLEMENT_DYNCREATE(CPropYK, CPropertyPage)

CPropYK::CPropYK() : CPropertyPage(CPropYK::IDD)
{
	//{{AFX_DATA_INIT(CPropYK)
	m_BS_Value = -1;
	m_BS_name = _T("");
	m_YK_caption = _T("");
	m_YK_name = _T("");
	m_ZG_name = _T("");
	m_ZK_value = -1;
	m_FK_name = _T("");
	m_FX_open_name = _T("");
	m_FX_close_name = _T("");
	//}}AFX_DATA_INIT
}

CPropYK::~CPropYK()
{
}

void CPropYK::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropYK)
	DDX_Control(pDX, IDC_BUTTON_MOD, m_modify);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_del);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_add);
	DDX_Control(pDX, IDC_LIST3, m_list_bs);
	DDX_Radio(pDX, IDC_BS_VALUE, m_BS_Value);
	DDX_Text(pDX, IDC_BS, m_BS_name);
	DDV_MaxChars(pDX, m_BS_name, 16);
	DDX_Text(pDX, IDC_YK_TEXT, m_YK_caption);
	DDV_MaxChars(pDX, m_YK_caption, 16);
	DDX_Text(pDX, IDC_YK, m_YK_name);
	DDV_MaxChars(pDX, m_YK_name, 16);
	DDX_Text(pDX, IDC_ZG, m_ZG_name);
	DDV_MaxChars(pDX, m_ZG_name, 16);
	DDX_Radio(pDX, IDC_ZK_VALUE, m_ZK_value);
	DDX_Text(pDX, IDC_FK, m_FK_name);
	DDV_MaxChars(pDX, m_FK_name, 16);
	DDX_Text(pDX, IDC_FX_OPEN, m_FX_open_name);
	DDV_MaxChars(pDX, m_FX_open_name, 16);
	DDX_Text(pDX, IDC_FX_CLOSE, m_FX_close_name);
	DDV_MaxChars(pDX, m_FX_close_name, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropYK, CPropertyPage)
	//{{AFX_MSG_MAP(CPropYK)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MOD, OnButtonMod)
	ON_LBN_SELCHANGE(IDC_LIST3, OnSelchangeList3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropYK message handlers

/*
	char m_Ykname[17];					//该遥控点英文名称
	BOOL m_bZK;									//TRUE-直控 FALSE-选控
	char m_Yxname_Hang[17];			//摘/挂牌遥信反馈点
	char m_Yxname_FX[17];				//遥控返校点
	char m_Yxname_Yk[17];				//遥控反馈点
	tagBS_Array m_BS_Array;			//闭锁条件集合
*/

void CPropYK::SetYkParam(char text[],char Ykname[],BOOL bZK,char Yxname_Hang[],
												 char Yxname_FX_Close[],char Yxname_FX_Open[],
												 char Yxname_Yk[],tagBS_Array& BS_Array)
{
	m_YK_caption=text;
	m_YK_name=Ykname;
	m_ZK_value=(bZK==TRUE)?0:1;
	m_ZG_name=Yxname_Hang;
	m_FX_close_name=Yxname_FX_Close;
	m_FX_open_name=Yxname_FX_Open;
	m_FK_name=Yxname_Yk;

	Copy_BS_Array(BS_Array,m_BS_Array);

}

void CPropYK::GetYkParam(char text[],char Ykname[],BOOL& bZK,char Yxname_Hang[],
							 char Yxname_FX_Close[],char Yxname_FX_Open[],char Yxname_Yk[],tagBS_Array& BS_Array)

{
	strcpy(text,m_YK_caption);
	strcpy(Ykname,m_YK_name);
	bZK=(m_ZK_value==0)?TRUE:FALSE;
	strcpy(Yxname_Hang,m_ZG_name);
	strcpy(Yxname_FX_Close,m_FX_close_name);
	strcpy(Yxname_FX_Open,m_FX_open_name);
	strcpy(Yxname_Yk,m_FK_name);

	BS_Array.RemoveAll();
	Copy_BS_Array(m_BS_Array,BS_Array);

}

void CPropYK::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	tagBSItem* pItem;
	pItem=new tagBSItem;
	ASSERT(pItem!=NULL);

	if (pItem!=NULL)
	{
		strcpy(pItem->strYxName,m_BS_name);
		pItem->bValue=(BYTE)(m_BS_Value==0)?1:0;
		m_BS_Array.Add(pItem);
		CString str;
		str.Format("%-16s,%d",m_BS_name,pItem->bValue);
		m_list_bs.AddString(str);
	}

	Update_BS_UI();	
}

void CPropYK::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	int id=m_list_bs.GetCurSel();
	ASSERT(id!=LB_ERR);

	tagBSItem* pItem;

	CString str;
	pItem=m_BS_Array[id];
	ASSERT(pItem!=NULL);
	str.Format("%-16s,%d",pItem->strYxName,pItem->bValue);
	if (::MessageBox(m_hWnd,str,"删除闭锁条件项",MB_OKCANCEL)==IDOK)  
	{
		m_BS_Array.RemoveAt(id);
		delete pItem;
		m_list_bs.DeleteString(id);
	}

  Update_BS_UI();
}

void CPropYK::OnButtonMod() 
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
	pItem->bValue=(BYTE)(m_BS_Value==0)?1:0;

	str.Format("%-16s,%d",m_BS_name,pItem->bValue);
	m_list_bs.DeleteString(id);
	m_list_bs.InsertString(id,str);

	Update_BS_UI();

}

BOOL CPropYK::OnInitDialog() 
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

void CPropYK::Update_BS_UI()
{
	if (m_BS_Array.GetSize()<MAX_BS_COUNT)
	  m_add.EnableWindow(TRUE);
	else 
		m_add.EnableWindow(FALSE);
	int id=m_list_bs.GetCurSel();

	if (id!=LB_ERR) 
	{
		m_del.EnableWindow(TRUE);
		m_modify.EnableWindow(TRUE);
		tagBSItem* pItem=m_BS_Array[id];
		ASSERT(pItem!=NULL);
		m_BS_name=pItem->strYxName;
		m_BS_Value=(pItem->bValue==0)?0:1;
	} 
	else
	{
		m_del.EnableWindow(FALSE);
		m_modify.EnableWindow(FALSE);
	}

}

void CPropYK::OnSelchangeList3() 
{
	// TODO: Add your control notification handler code here
	Update_BS_UI();
	UpdateData(FALSE);
}

void CPropYK::Copy_BS_Array(tagBS_Array &Src_BS_Array, tagBS_Array &Dst_BS_Array)
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

void CPropYK::OnButton1() 
{
	
}

void CPropYK::OnButton2() 
{

}

void CPropYK::OnButton3() 
{
	
}

void CPropYK::OnButton9() 
{
	

}

void CPropYK::OnButton4() 
{

}

void CPropYK::OnButton5() 
{
	
}
