// PropYT2.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropYT2.h"

#define MAX_BS_COUNT 32
// CPropYT2 对话框

IMPLEMENT_DYNCREATE(CPropYT2, CPropertyPage)

CPropYT2::CPropYT2() : CPropertyPage(CPropYT2::IDD)
, m_sPianCha(_T(""))
, m_sXiaoShu(_T(""))
, m_sBiLi(_T(""))
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

CPropYT2::~CPropYT2()
{
}

void CPropYT2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropYT2)
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
	DDX_Control(pDX, IDC_PIANCHA, m_editPianCha);
	DDX_Control(pDX, IDC_XIAOSHU, m_editXiaoShu);
	DDX_Control(pDX, IDC_BILI, m_editBiLi);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_PIANCHA, m_sPianCha);
	DDX_Text(pDX, IDC_XIAOSHU, m_sXiaoShu);
	DDX_Text(pDX, IDC_BILI, m_sBiLi);
}


BEGIN_MESSAGE_MAP(CPropYT2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropYT2)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MOD, OnBtnMod)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_value_type, OnBnClickedvaluetype)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropYT message handlers

void CPropYT2::Copy_BS_Array(tagBS_Array &Src_BS_Array, tagBS_Array &Dst_BS_Array)
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

void CPropYT2::GetYTParam(char text[], char Ykname[], int &bVT, char Yxname_FK[], tagBS_Array &BS_Array, float& fPianCha, int& nXiaoShu, int& nBiLi)
{
	strcpy(text,m_YK_Caption);
	strcpy(Ykname,m_YK_name);
	bVT=(m_value_type==0)?TRUE:FALSE;
	strcpy(Yxname_FK,m_FK_name);

	BS_Array.RemoveAll();
	Copy_BS_Array(m_BS_Array,BS_Array);

	fPianCha = atof((LPSTR)(LPCSTR)m_sPianCha);
	nXiaoShu = atoi((LPSTR)(LPCSTR)m_sXiaoShu);
	nBiLi = atoi((LPSTR)(LPCSTR)m_sBiLi);
	//fPianCha = m_fPianCha;
	//nXiaoShu = m_nXiaoShu;
	//nBiLi = m_nBiLi;

}

void CPropYT2::SetYTParam(char text[], char Ykname[], int bVT, char Yxname_FK[], tagBS_Array &BS_Array, float fPianCha, int nXiaoShu, int nBiLi)
{
	m_YK_Caption=text;
	m_YK_name=Ykname;
	m_value_type=(bVT==TRUE)?0:1;
	m_FK_name=Yxname_FK;

	Copy_BS_Array(BS_Array,m_BS_Array);

	
	if((fPianCha == 0)&&(nXiaoShu == 0)&&(nBiLi == 0))
	{
		m_sPianCha = "";
		m_sXiaoShu = "";
		m_sBiLi = "";
	}
	else
	{
		m_sPianCha.Format("%.3f", fPianCha);
		m_sXiaoShu.Format("%d", nXiaoShu);
		m_sBiLi.Format("%d", nBiLi);
	}
	
	//m_fPianCha = fPianCha;
	//m_nXiaoShu = nXiaoShu;
	//m_nBiLi = nBiLi;
}

void CPropYT2::OnBtnAdd() 
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

void CPropYT2::OnBtnDel() 
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

void CPropYT2::OnBtnMod() 
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

BOOL CPropYT2::OnInitDialog() 
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

	if(m_value_type == 0)
	{
		m_editPianCha.SetWindowText("");
		m_editXiaoShu.SetWindowText("");
		m_editBiLi.SetWindowText("");
		m_editPianCha.EnableWindow(false);
		m_editXiaoShu.EnableWindow(false);
		m_editBiLi.EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropYT2::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	Update_BS_UI();
	UpdateData(FALSE);	
}

void CPropYT2::Update_BS_UI()
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

void CPropYT2::OnButton4() 
{
	
}

void CPropYT2::OnButton5() 
{
		
}

void CPropYT2::OnButton6() 
{
	
}

void CPropYT2::OnBnClickedvaluetype()
{
	m_editPianCha.SetWindowText("");
	m_editXiaoShu.SetWindowText("");
	m_editBiLi.SetWindowText("");
	m_editPianCha.EnableWindow(false);
	m_editXiaoShu.EnableWindow(false);
	m_editBiLi.EnableWindow(false);
}

void CPropYT2::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_editPianCha.EnableWindow(true);
	m_editXiaoShu.EnableWindow(true);
	m_editBiLi.EnableWindow(true);
}


BOOL CPropYT2::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	if((m_value_type == 1)&&(m_sPianCha == ""))
	{
		AfxMessageBox("偏差值未填！");
		return false;
	}
	else if((m_value_type == 1)&&(m_sXiaoShu == ""))
	{
		AfxMessageBox("小数位未填！");
		return false;
	}
	else if((m_value_type == 1)&&(m_sBiLi == ""))
	{
		AfxMessageBox("比例未填！");
		return false;
	}
	else
        return CPropertyPage::OnApply();
}
