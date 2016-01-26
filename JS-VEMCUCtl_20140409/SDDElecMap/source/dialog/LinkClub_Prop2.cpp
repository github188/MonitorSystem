// LinkClub_Prop2.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/LinkClub_Prop2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop2 property page

IMPLEMENT_DYNCREATE(CLinkClub_Prop2, CPropertyPage)

CLinkClub_Prop2::CLinkClub_Prop2() : CPropertyPage(CLinkClub_Prop2::IDD)
{
	//{{AFX_DATA_INIT(CLinkClub_Prop2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLinkClub_Prop2::~CLinkClub_Prop2()
{
}

void CLinkClub_Prop2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkClub_Prop2)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkClub_Prop2, CPropertyPage)
	//{{AFX_MSG_MAP(CLinkClub_Prop2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop2 message handlers

BOOL CLinkClub_Prop2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_list.SetExtendedStyle (LVS_EX_GRIDLINES);

    m_list.InsertColumn (0, "序号");
    m_list.InsertColumn (1, "英文名");
    m_list.InsertColumn (2, "简称");
    m_list.SetColumnWidth (0, 40);
    m_list.SetColumnWidth (1, 150);
    m_list.SetColumnWidth (2, 100);

	char s[16];
	for (int i=0;i<mycount;i++)
	{
		sprintf(s,"%d",i);
		m_list.InsertItem(i,s);
		m_list.SetItemText(i,1,myinfo[i].ename);
		m_list.SetItemText(i,2,myinfo[i].cname);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinkClub_Prop2::SetPara(BYTE count, STRUCT_SINGLEBAR info[])
{
	mycount=count;
	memcpy(myinfo,info,sizeof(STRUCT_SINGLEBAR)*64);
}

void CLinkClub_Prop2::GetPara(BYTE *count, STRUCT_SINGLEBAR *info)
{
    *count=mycount;
	memcpy(info,myinfo,sizeof(STRUCT_SINGLEBAR)*64);
}

void CLinkClub_Prop2::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char s[8];
	if (mycount>=64) return;
	sprintf(s,"%d",mycount);
	m_list.InsertItem(mycount,s);
	m_list.SetItemText(mycount,1,"");
	m_list.SetItemText(mycount,2,"");
    m_list.SetItemState(mycount, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);

	mycount++;
}

void CLinkClub_Prop2::OnButton2() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if (pos==NULL) return;
	int item=m_list.GetNextSelectedItem(pos);

	for (int i=item;i<mycount-1;i++)
	{
		CString str1,str2,str3;
		str1=m_list.GetItemText(i+1,0);
		str2=m_list.GetItemText(i+1,1);
		str3=m_list.GetItemText(i+1,2);
		m_list.SetItemText(i,0,str1);
		m_list.SetItemText(i,1,str2);
		m_list.SetItemText(i,2,str3);
	}
	m_list.DeleteItem(mycount-1);
	mycount--;
	if (item>mycount-1) item=mycount-1;
    m_list.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);

}

void CLinkClub_Prop2::OnButton3() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if (pos==NULL) return;
	int item=m_list.GetNextSelectedItem(pos);
	if (item==0)return;
	CString str1,str2,str3;
	CString str4,str5,str6;
//	str1=m_list.GetItemText(item-1,0);
	str2=m_list.GetItemText(item-1,1);
	str3=m_list.GetItemText(item-1,2);
//	str4=m_list.GetItemText(item,0);
	str5=m_list.GetItemText(item,1);
	str6=m_list.GetItemText(item,2);
//	m_list.SetItemText(item,0,str1);
	m_list.SetItemText(item,1,str2);
	m_list.SetItemText(item,2,str3);
//	m_list.SetItemText(item-1,0,str4);
	m_list.SetItemText(item-1,1,str5);
	m_list.SetItemText(item-1,2,str6);
    m_list.SetItemState(item-1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);

	
}

void CLinkClub_Prop2::OnButton4() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if (pos==NULL) return;
	int item=m_list.GetNextSelectedItem(pos);
	if (item==mycount-1)return;
	CString str1,str2,str3;
	CString str4,str5,str6;
//	str1=m_list.GetItemText(item+1,0);
	str2=m_list.GetItemText(item+1,1);
	str3=m_list.GetItemText(item+1,2);
//	str4=m_list.GetItemText(item,0);
	str5=m_list.GetItemText(item,1);
	str6=m_list.GetItemText(item,2);
//	m_list.SetItemText(item,0,str1);
	m_list.SetItemText(item,1,str2);
	m_list.SetItemText(item,2,str3);
//	m_list.SetItemText(item+1,0,str4);
	m_list.SetItemText(item+1,1,str5);
	m_list.SetItemText(item+1,2,str6);
    m_list.SetItemState(item+1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);

}
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

void CLinkClub_Prop2::OnButton5() 
{
	// TODO: Add your control notification handler code here
 //===========
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if (pos==NULL) return;
	int item=m_list.GetNextSelectedItem(pos);

	char dllf[256],s[256],myename[17],mycname[33];
 	HINSTANCE hmydll;
 	sprintf(dllf,"c:\\sunpac\\dll\\Finddot.dll");
 	hmydll=LoadLibrary(dllf);
 	if (hmydll==NULL){
 		sprintf(s,"%s 调入失败！",dllf);
 		AfxMessageBox(s);
 		return;
 	}
 	ShowFindDotDlg * pf;
 	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
 	if (pf==NULL){
 		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
 		AfxMessageBox(s);
 		FreeLibrary(hmydll);
 		hmydll=NULL;
 		return;
 	}
 	CWnd * pp=this->GetParent();
	BYTE dotkind=1;
 	if ((*pf)(dotkind,myename,mycname))
 	{
		m_list.SetItemText(item,1,myename);
 	}
 
 	FreeLibrary(hmydll);
	
}

void CLinkClub_Prop2::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tmp1,tmp2;
	for (int i=0;i<mycount;i++)
	{
		tmp1=m_list.GetItemText(i,1);
		tmp2=m_list.GetItemText(i,2);
		sprintf(myinfo[i].ename,"%s",tmp1);
		sprintf(myinfo[i].cname,"%s",tmp2);
	}
	
	CPropertyPage::OnOK();
}
