// TyAnimate_Prop.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/TyAnimate_Prop.h"
#include "dialog/BmpSelectPage.h"
#include "canvas/yc.h"
#include "dialog/SelectVectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTyAnimate_Prop property page

IMPLEMENT_DYNCREATE(CTyAnimate_Prop, CPropertyPage)

typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

CTyAnimate_Prop::CTyAnimate_Prop() : CPropertyPage(CTyAnimate_Prop::IDD)
{
	//{{AFX_DATA_INIT(CTyAnimate_Prop)
	m_ename = _T("");
	m_high = 0.0f;
	m_width = 0.0f;
	m_x0 = 0.0f;
	m_y0 = 0.0f;
	m_diskind = -1;
	m_pickind = -1;
	//}}AFX_DATA_INIT
}

CTyAnimate_Prop::~CTyAnimate_Prop()
{
}

void CTyAnimate_Prop::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTyAnimate_Prop)
	DDX_Control(pDX, IDC_LISTBMP2, m_listvec);
	DDX_Control(pDX, IDC_LISTBMP, m_listbmp);
	DDX_Text(pDX, CZ_IDC_EDIT_ENAME, m_ename);
	DDV_MaxChars(pDX, m_ename, 16);
	DDX_Text(pDX, CZ_IDC_EDITFH, m_high);
	DDV_MinMaxFloat(pDX, m_high, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITFW, m_width);
	DDV_MinMaxFloat(pDX, m_width, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITX0, m_x0);
	DDV_MinMaxFloat(pDX, m_x0, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITY0, m_y0);
	DDV_MinMaxFloat(pDX, m_y0, 0.f, 32000.f);
	DDX_Radio(pDX, IDC_RADIO1, m_diskind);
	DDX_Radio(pDX, IDC_RADIO3, m_pickind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTyAnimate_Prop, CPropertyPage)
	//{{AFX_MSG_MAP(CTyAnimate_Prop)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(CZ_IDC_BUTTON_FINDDOT, OnIdcButtonFinddot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTyAnimate_Prop message handlers

void CTyAnimate_Prop::SetPara(Stu_TyAnimate *animate)
{
	memcpy(&myanimate,animate,sizeof(Stu_TyAnimate));
	m_ename=myanimate.ename;
	m_x0=myanimate.rect.x0;
	m_y0=myanimate.rect.y0;
	m_width=myanimate.rect.x1-myanimate.rect.x0;
	m_high=myanimate.rect.y1-myanimate.rect.y0;

	m_diskind=myanimate.disOR0Kind;
	m_pickind=myanimate.picskind;
}

void CTyAnimate_Prop::GetPara(Stu_TyAnimate *animate)
{
	memcpy(animate,&myanimate,sizeof(Stu_TyAnimate));
}	

BOOL CTyAnimate_Prop::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_listbmp.SetRectType(false);
    m_listbmp.SetColumnType(false);
    m_listvec.SetRectType(false);
    m_listvec.SetColumnType(false);

    CStringArray Head;
    CByteArray   Cols;
    Head.Add("序号");
	Cols.Add(20);
	Head.Add("图名");
	Cols.Add(60);
	Head.Add("透明色");
	Cols.Add(20);
	m_listbmp.InitCtrl(&Head, &Cols);

    Head.RemoveAll();
	Cols.RemoveAll();
	Head.Add("序号");
	Cols.Add(20);
	Head.Add("图名");
	Cols.Add(60);
	Head.Add("动态色");
	Cols.Add(20);
	m_listvec.InitCtrl(&Head, &Cols);

    m_listbmp.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT); 
    m_listvec.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT); 
	
	if (myanimate.picskind==0)
	{
		m_listbmp.ShowWindow(SW_SHOW);
		m_listvec.ShowWindow(SW_HIDE);
		for (int i=0;i<myanimate.piccount;i++)
		{
			CString tmp;
			tmp.Format("%d",i);
			Head.RemoveAll();
			Head.Add(tmp);
			Head.Add(myanimate.pics[i].picname);
			if (myanimate.pics[i].pickind==0) 
			{
				Head.Add("无");
			}else{
				Head.Add("有");
			}
		    int pos=m_listbmp.AddItem(&Head);
			m_listbmp.SetItemData(pos,myanimate.pics[i].piccolor);
		}
	//	m_listbmp.SetItemBackgndColor(WHITE,pos,0);
	//	m_listbmp.SetItemTextColor(BLACK,pos,0);
	}
	if (myanimate.picskind==1)
	{
		m_listbmp.ShowWindow(SW_HIDE);
		m_listvec.ShowWindow(SW_SHOW);
		for (int i=0;i<myanimate.piccount;i++)
		{
			CString tmp;
			tmp.Format("%d",i);
			Head.RemoveAll();
			Head.Add(tmp);
			Head.Add(myanimate.pics[i].picname);
			if (myanimate.pics[i].pickind==0) 
			{
				Head.Add("无");
			}else{
				Head.Add("有");
			}
		    int pos=m_listvec.AddItem(&Head);
			m_listvec.SetItemData(pos,myanimate.pics[i].piccolor);
		}
	//	m_listbmp.SetItemBackgndColor(WHITE,pos,0);
	//	m_listbmp.SetItemTextColor(BLACK,pos,0);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTyAnimate_Prop::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    CStringArray Head;
	if (m_pickind==0)//bmp
	{
		if (m_listbmp.GetItemCount()==16)
		{
			AfxMessageBox("警告:配置已满,不能添加!");
			return;
		}
		int pos=m_listbmp.GetItemCount();
		CPropertySheet sheet( _T("对象.."));
		CBmpSelectPage dlg;
		dlg.SetContent("",0,0x000000);
		sheet.AddPage((CPropertyPage*)&dlg);
		if (sheet.DoModal() == IDOK)
		{
			BOOL tmpbool;
			COLORREF color;
			CString tmp1,tmp2;
			tmp2=dlg.GetContent(tmpbool,color);
			Head.RemoveAll();
			tmp1.Format("%d",pos);
			Head.Add(tmp1);
			Head.Add(tmp2.Left(32));
			if (tmpbool) 
			{
				Head.Add("有");
			}else{
				Head.Add("无");
			}
			int pos=m_listbmp.AddItem(&Head);
			m_listbmp.SetItemData(pos,color);
			m_listbmp.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	if (m_pickind==1)//vec
	{

		if (m_listvec.GetItemCount()==16)
		{
			AfxMessageBox("警告:配置已满,不能添加!");
			return;
		}
		int pos=m_listvec.GetItemCount();
		
		CPropertySheet sheet( _T("对象..") );
		CSelectVectorDlg dlg;
 
		dlg.SetContent("",FALSE,0);
		sheet.AddPage( (CPropertyPage*)&dlg );

		if (sheet.DoModal() == IDOK)
		{
			BOOL tmpbool;
			COLORREF color;
			CString tmp1,tmp2;
			dlg.GetContent(tmp2,tmpbool,color);
			Head.RemoveAll();
			tmp1.Format("%d",pos);
			Head.Add(tmp1);
			Head.Add(tmp2.Left(32));
			if (tmpbool) 
			{
				Head.Add("有");
			}else{
				Head.Add("无");
			}
			int pos=m_listvec.AddItem(&Head);
			m_listvec.SetItemData(pos,color);
			m_listvec.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
			
		}
	}
}

void CTyAnimate_Prop::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (m_pickind==0)//bmp
	{
		if (m_listbmp.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listbmp.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listbmp.GetNextSelectedItem(temppos);
		m_listbmp.DeleteItem(pos);
		for (int i=0;i<m_listbmp.GetItemCount();i++)
		{
			CString tmp;
			tmp.Format("%d",i);
			m_listbmp.SetItemText(i,0,tmp);
		}
		if (pos>m_listbmp.GetItemCount()-1)
		{
			pos--;
		}
		if (pos>=0) m_listbmp.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                              LVIS_SELECTED | LVIS_FOCUSED);
	}
	if (m_pickind==1)//vec
	{
		if (m_listvec.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listvec.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listvec.GetNextSelectedItem(temppos);
		m_listvec.DeleteItem(pos);
		for (int i=0;i<m_listvec.GetItemCount();i++)
		{
			CString tmp;
			tmp.Format("%d",i);
			m_listvec.SetItemText(i,0,tmp);
		}
		if (pos>m_listvec.GetItemCount()-1)
		{
			pos--;
		}
		if (pos>=0) m_listvec.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                              LVIS_SELECTED | LVIS_FOCUSED);
	}
	
}

void CTyAnimate_Prop::OnButton3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    CStringArray Head;
	if (m_pickind==0)//bmp
	{
		if (m_listbmp.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listbmp.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listbmp.GetNextSelectedItem(temppos);
		CPropertySheet sheet( _T("选择位图"));
		CBmpSelectPage dlg;
		CString sbmp=m_listbmp.GetItemText(pos,1);
		CString sbool=m_listbmp.GetItemText(pos,2);
		if (strcmp(sbool,"有")==0)
		{
			dlg.SetContent(sbmp,true,m_listbmp.GetItemData(pos));
		}else{
			dlg.SetContent(sbmp,false,m_listbmp.GetItemData(pos));
		}
		sheet.AddPage((CPropertyPage*)&dlg);
		if (sheet.DoModal() == IDOK)
		{
			BOOL tmpbool;
			COLORREF color;
			CString tmp=dlg.GetContent(tmpbool,color);
			if (tmpbool) 
			{
				m_listbmp.SetItemText(pos,2,"有");
			}else{
				m_listbmp.SetItemText(pos,2,"无");
			}
			m_listbmp.SetItemText(pos,1,tmp.Left(32));
			m_listbmp.SetItemData(pos,color);
			m_listbmp.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	if (m_pickind==1)//vec
	{

		if (m_listvec.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listvec.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listvec.GetNextSelectedItem(temppos);
		CPropertySheet sheet( _T("对象.."));

		CSelectVectorDlg dlg;
 
		dlg.SetContent("",FALSE,0);
		
		
		CString svec=m_listvec.GetItemText(pos,1);
		CString sbool=m_listvec.GetItemText(pos,2);
		if (strcmp(sbool,"有")==0)
		{
			dlg.SetContent(svec,true,m_listvec.GetItemData(pos));
		}else{
			dlg.SetContent(svec,false,m_listvec.GetItemData(pos));
		}
		sheet.AddPage((CPropertyPage*)&dlg);
		if (sheet.DoModal() == IDOK)
		{
			BOOL tmpbool;
			COLORREF color;
			CString tmp;
			dlg.GetContent(tmp,tmpbool,color);
			if (tmpbool) 
			{
				m_listvec.SetItemText(pos,2,"有");
			}else{
				m_listvec.SetItemText(pos,2,"无");
			}
			m_listvec.SetItemText(pos,1,tmp.Left(32));
			m_listvec.SetItemData(pos,color);
			m_listvec.SetItemState(pos, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
		}
	
	}
	
}

void CTyAnimate_Prop::OnButton4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    CStringArray Head;
	if (m_pickind==0)//bmp
	{
		if (m_listbmp.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listbmp.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listbmp.GetNextSelectedItem(temppos);
		if (pos==0) return;
		CString bb1,bb2;COLORREF cc1;
		bb1=m_listbmp.GetItemText(pos,1);
		bb2=m_listbmp.GetItemText(pos,2);
		cc1=m_listbmp.GetItemData(pos);
		m_listbmp.SetItemText(pos,1,m_listbmp.GetItemText(pos-1,1));
		m_listbmp.SetItemText(pos,2,m_listbmp.GetItemText(pos-1,2));
		m_listbmp.SetItemData(pos,m_listbmp.GetItemData(pos-1));
		m_listbmp.SetItemText(pos-1,1,bb1);
		m_listbmp.SetItemText(pos-1,2,bb2);
		m_listbmp.SetItemData(pos-1,cc1);
		m_listbmp.SetItemState(pos-1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
	}
	if (m_pickind==1)//vec
	{
		if (m_listvec.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listvec.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listvec.GetNextSelectedItem(temppos);
		if (pos==0) return;
		CString bb1,bb2;COLORREF cc1;
		bb1=m_listvec.GetItemText(pos,1);
		bb2=m_listvec.GetItemText(pos,2);
		cc1=m_listvec.GetItemData(pos);
		m_listvec.SetItemText(pos,1,m_listvec.GetItemText(pos-1,1));
		m_listvec.SetItemText(pos,2,m_listvec.GetItemText(pos-1,2));
		m_listvec.SetItemData(pos,m_listvec.GetItemData(pos-1));
		m_listvec.SetItemText(pos-1,1,bb1);
		m_listvec.SetItemText(pos-1,2,bb2);
		m_listvec.SetItemData(pos-1,cc1);
		m_listvec.SetItemState(pos-1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
	}
	
}

void CTyAnimate_Prop::OnButton5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    CStringArray Head;
	if (m_pickind==0)//bmp
	{
		if (m_listbmp.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listbmp.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listbmp.GetNextSelectedItem(temppos);
		if (pos==m_listbmp.GetItemCount()-1) return;
		CString bb1,bb2;COLORREF cc1;
		bb1=m_listbmp.GetItemText(pos,1);
		bb2=m_listbmp.GetItemText(pos,2);
		cc1=m_listbmp.GetItemData(pos);
		m_listbmp.SetItemText(pos,1,m_listbmp.GetItemText(pos+1,1));
		m_listbmp.SetItemText(pos,2,m_listbmp.GetItemText(pos+1,2));
		m_listbmp.SetItemData(pos,m_listbmp.GetItemData(pos+1));
		m_listbmp.SetItemText(pos+1,1,bb1);
		m_listbmp.SetItemText(pos+1,2,bb2);
		m_listbmp.SetItemData(pos+1,cc1);
		m_listbmp.SetItemState(pos+1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);
	}
	if (m_pickind==1)//vec
	{
		if (m_listvec.GetItemCount()==0)
		{
			return;
		}
		POSITION temppos=m_listvec.GetFirstSelectedItemPosition();
		if (temppos==NULL) return;
		int pos=m_listvec.GetNextSelectedItem(temppos);
		if (pos==m_listvec.GetItemCount()-1) return;
		CString bb1,bb2;COLORREF cc1;
		bb1=m_listvec.GetItemText(pos,1);
		bb2=m_listvec.GetItemText(pos,2);
		cc1=m_listvec.GetItemData(pos);
		m_listvec.SetItemText(pos,1,m_listvec.GetItemText(pos+1,1));
		m_listvec.SetItemText(pos,2,m_listvec.GetItemText(pos+1,2));
		m_listvec.SetItemData(pos,m_listvec.GetItemData(pos+1));
		m_listvec.SetItemText(pos+1,1,bb1);
		m_listvec.SetItemText(pos+1,2,bb2);
		m_listvec.SetItemData(pos+1,cc1);
		m_listvec.SetItemState(pos+1, LVIS_SELECTED | LVIS_FOCUSED , 
                                LVIS_SELECTED | LVIS_FOCUSED);

	}
	
}

void CTyAnimate_Prop::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);
	sprintf(myanimate.ename,"%s",m_ename);
	myanimate.rect.x0=m_x0;
	myanimate.rect.y0=m_y0;
	myanimate.rect.x1=m_x0+m_width;
	myanimate.rect.y1=m_y0+m_high;
	myanimate.disOR0Kind=m_diskind;
	myanimate.picskind=m_pickind;

	if (m_pickind==0)//bmp
	{
		myanimate.piccount=m_listbmp.GetItemCount();
		for (int i=0;i<myanimate.piccount;i++)
		{
			sprintf(myanimate.pics[i].picname,"%s",m_listbmp.GetItemText(i,1).Left(32));
			CString tmp=m_listbmp.GetItemText(i,2);
			if (strcmp(tmp,"有")==0)
			{
				myanimate.pics[i].pickind=1;
			}else{
				myanimate.pics[i].pickind=0;
			}
			myanimate.pics[i].piccolor=m_listbmp.GetItemData(i);
		}
	}
	if (m_pickind==1)//vec
	{
		myanimate.piccount=m_listvec.GetItemCount();
		for (int i=0;i<myanimate.piccount;i++)
		{
			sprintf(myanimate.pics[i].picname,"%s",m_listvec.GetItemText(i,1).Left(32));
			CString tmp=m_listvec.GetItemText(i,2);
			if (strcmp(tmp,"有")==0)
			{
				myanimate.pics[i].pickind=1;
			}else{
				myanimate.pics[i].pickind=0;
			}
			myanimate.pics[i].piccolor=m_listvec.GetItemData(i);
		}
	}
	
	CPropertyPage::OnOK();
}

void CTyAnimate_Prop::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_listbmp.ShowWindow(SW_SHOW);
	m_listvec.ShowWindow(SW_HIDE);
}

void CTyAnimate_Prop::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_listbmp.ShowWindow(SW_HIDE);
	m_listvec.ShowWindow(SW_SHOW);
	
}

void CTyAnimate_Prop::OnIdcButtonFinddot() 
{
	// TODO: Add your control notification handler code here
	
 	char dllf[256],s[256],myename[17],mycname[33];
 	HINSTANCE hmydll;
 	sprintf(dllf,YC_FINDDOTDLLNAME);
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
 		m_ename=myename;
 //		UpdateData(true);
 		UpdateData(false);
 	}
 
 	FreeLibrary(hmydll);
}
