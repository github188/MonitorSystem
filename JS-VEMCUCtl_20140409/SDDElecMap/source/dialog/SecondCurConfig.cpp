// SecondCurConfig.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/curve.h"
#include "dialog/CurEditDialog.h"
#include "dialog/SecondCurConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecondCurConfig dialog


CSecondCurConfig::CSecondCurConfig()
	: CPropertyPage(CSecondCurConfig::IDD)
	, m_bChecked(FALSE)
	, m_nIndex(0)
	, m_nMax(0)
	, m_nMin(0)
	, m_nWidth(0)
{
	//{{AFX_DATA_INIT(CSecondCurConfig)
	//}}AFX_DATA_INIT
	m_nMax = 0;
	 m_nMin = 0;
	 m_nWidth = 0;
}


void CSecondCurConfig::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecondCurConfig)
	DDX_Control(pDX, IDC_SHQ_SECONDINFOLIST, m_infolist);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bChecked);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nIndex);
	DDX_Text(pDX, IDC_EDIT1, m_nMax);
	DDX_Text(pDX, IDC_EDIT26, m_nMin);
	DDX_Text(pDX, IDC_EDIT29, m_nWidth);
	DDX_Control(pDX, IDC_COLOR3, m_corl);
	DDX_Control(pDX, IDC_COLOR, m_corls);
}


BEGIN_MESSAGE_MAP(CSecondCurConfig, CPropertyPage)
	//{{AFX_MSG_MAP(CSecondCurConfig)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POP_CUR_ADD, OnPopCurAdd)
	ON_COMMAND(ID_POP_CUR_DEL, OnPopCurDel)
	ON_COMMAND(ID_POP_CUR_EDIT, OnPopCurEdit)
	ON_UPDATE_COMMAND_UI(ID_POP_CUR_ADD, OnUpdatePopCurAdd)
	ON_UPDATE_COMMAND_UI(ID_POP_CUR_DEL, OnUpdatePopCurDel)
	ON_UPDATE_COMMAND_UI(ID_POP_CUR_EDIT, OnUpdatePopCurEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SHQ_SECONDINFOLIST, OnItemchangedShqSecondinfolist)
	ON_NOTIFY(NM_DBLCLK, IDC_SHQ_SECONDINFOLIST, OnDblclkShqSecondinfolist)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_COLOR3, OnColor3)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_STN_CLICKED(IDC_COLOR3, OnStnClickedColor3)
	ON_STN_CLICKED(IDC_COLOR, OnStnClickedColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecondCurConfig message handlers

//DEL void CSecondCurConfig::SetInitData(curvegroupinfo *m_initdata)
//DEL {
//DEL }

BOOL CSecondCurConfig::OnInitDialog()
{
	CPropertyPage::OnInitDialog();	

	LV_COLUMN m_lvcolumn;
	m_lvcolumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	m_lvcolumn.fmt=LVCFMT_CENTER;
	m_lvcolumn.cx=350;

	m_lvcolumn.iSubItem=0;
	m_lvcolumn.pszText="曲线名称";
	m_infolist.InsertColumn(0,&m_lvcolumn);

	for(int i=0;i<m_curcount;i++)
	{
		LV_ITEM lvItem;
		lvItem.mask=LVIF_TEXT|LVIF_STATE;
		lvItem.state=0;
		lvItem.stateMask=0;
		lvItem.iItem=i;
		lvItem.iSubItem=0;
		lvItem.pszText=m_curinfolist[i].cname;
		m_infolist.InsertItem(&lvItem);		
	}

	//m_corls.SubclassDlgItem(IDC_COLOR,this);
	UpdateColor();
	UpdateColor3();

	if( !m_bChecked)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT26)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLOR3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT29)->EnableWindow(FALSE);
	}
     
	// m_color = 0;
	// m_color3 = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSecondCurConfig::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(pWnd->GetDlgCtrlID()==IDC_SHQ_SECONDINFOLIST)
	{
    	CMenu m_menu;
    	CMenu* p_popupmenu;
    	if(m_menu.LoadMenu(IDR_POPUP_CUR))
		{
    		p_popupmenu=m_menu.GetSubMenu(0);
    		p_popupmenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		}
	}

	// TODO: Add your message handler code here
	
}

void CSecondCurConfig::OnPopCurAdd()
{
	if(m_curcount<8)
	{
		char cname[10];
		sprintf(cname,"新添曲线%d",m_curcount+1);
		strcpy(m_curinfolist[m_curcount].cname,cname);
	    strcpy(m_curinfolist[m_curcount].ename,"");
	    m_curinfolist[m_curcount].color=RGB(0,0,255);
	    m_curinfolist[m_curcount].compare=FALSE;
	    m_curinfolist[m_curcount].precission=2;
	    m_curinfolist[m_curcount].reference=0;
	    m_curinfolist[m_curcount].scale=1;
	    m_curinfolist[m_curcount].swing=3;
	    m_curinfolist[m_curcount].width=1;	
		LV_ITEM lvItem;
		lvItem.mask=LVIF_TEXT|LVIF_STATE;
		lvItem.state=0;
		lvItem.stateMask=0;
		lvItem.iItem=m_curcount;
		lvItem.iSubItem=0;
		lvItem.pszText=cname;
		m_infolist.InsertItem(&lvItem);		
		m_curcount++;
	}
	else
	{
		AfxMessageBox("最多只能支持8条曲线",MB_OK);
	}
}

void CSecondCurConfig::OnPopCurDel()
{
	// TODO: Add your command handler code here	
	if((m_itemindex>=0) && (m_itemindex<=8))
	{    	
		if(AfxMessageBox("您是否真要删除该条曲线",MB_YESNO)==IDYES)
		{
    		if(m_itemindex<m_curcount-1)
			{
    			//将后面的曲线前移一位
        		for(int i=m_itemindex;i<m_curcount-1;i++)
				{
    				for(int j=0;j<33;j++)
					{
                		m_curinfolist[i].cname[j]=m_curinfolist[i+1].cname[j];
                	    m_curinfolist[i].ename[j]=m_curinfolist[i+1].ename[j];
					}
            	    m_curinfolist[i].color=m_curinfolist[i+1].color;
            	    m_curinfolist[i].compare=m_curinfolist[i+1].compare;
            	    m_curinfolist[i].precission=m_curinfolist[i+1].precission;
        	        m_curinfolist[i].reference=m_curinfolist[i+1].reference;
            	    m_curinfolist[i].scale=m_curinfolist[i+1].scale;
            	    m_curinfolist[i].swing=m_curinfolist[i+1].swing;
            	    m_curinfolist[i].width=m_curinfolist[i+1].width;	
				}
			}			
			m_infolist.DeleteItem(m_itemindex);
       		m_curcount--;
		}
	}
}

void CSecondCurConfig::OnPopCurEdit()
{
	// TODO: Add your command handler code here
	if((m_itemindex>=0) && (m_itemindex<=8))
	{
     	CCurEditDialog dlg;
		dlg.m_cname=m_curinfolist[m_itemindex].cname;
		dlg.m_color=m_curinfolist[m_itemindex].color;
		dlg.m_swing=m_curinfolist[m_itemindex].swing;
		dlg.m_ename=m_curinfolist[m_itemindex].ename;
		dlg.m_precission=m_curinfolist[m_itemindex].precission;
		if(m_curinfolist[m_itemindex].reference==0)
    		dlg.m_reference="左坐标系";
		else
			dlg.m_reference="右坐标系";
		dlg.m_scale=m_curinfolist[m_itemindex].scale;
		dlg.m_compare=m_curinfolist[m_itemindex].compare;
		dlg.m_width=m_curinfolist[m_itemindex].width;
        if (dlg.DoModal()==IDOK)
		{
			strcpy(m_curinfolist[m_itemindex].cname,dlg.m_cname);
			m_curinfolist[m_itemindex].color=dlg.m_color;
			m_curinfolist[m_itemindex].compare=dlg.m_compare;
			m_curinfolist[m_itemindex].width=dlg.m_width;
			m_curinfolist[m_itemindex].scale=dlg.m_scale;
			if(dlg.m_reference=="左坐标系")
    			m_curinfolist[m_itemindex].reference=0;
			else
				m_curinfolist[m_itemindex].reference=1;
			m_curinfolist[m_itemindex].precission=dlg.m_precission;
			m_curinfolist[m_itemindex].swing=dlg.m_swing;
			strcpy(m_curinfolist[m_itemindex].ename,dlg.m_ename);
			UpdateItem(m_curinfolist[m_itemindex].cname);
		}
	}
}

void CSecondCurConfig::OnUpdatePopCurAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSecondCurConfig::OnUpdatePopCurDel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSecondCurConfig::OnUpdatePopCurEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSecondCurConfig::OnItemchangedShqSecondinfolist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(pNMListView->iItem>=0)
	{
		m_itemindex=pNMListView->iItem;
	}
	
	*pResult = 0;
}

void CSecondCurConfig::OnDblclkShqSecondinfolist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if((m_itemindex>=0) && (m_itemindex<=8))
	{
     	CCurEditDialog dlg;
		dlg.m_cname=m_curinfolist[m_itemindex].cname;
		dlg.m_color=m_curinfolist[m_itemindex].color;
		dlg.m_swing=m_curinfolist[m_itemindex].swing;
		dlg.m_ename=m_curinfolist[m_itemindex].ename;
		dlg.m_precission=m_curinfolist[m_itemindex].precission;
		if(m_curinfolist[m_itemindex].reference==0)
    		dlg.m_reference="左坐标系";
		else
			dlg.m_reference="右坐标系";
		dlg.m_scale=m_curinfolist[m_itemindex].scale;
		dlg.m_compare=m_curinfolist[m_itemindex].compare;
		dlg.m_width=m_curinfolist[m_itemindex].width;
        if (dlg.DoModal()==IDOK)
		{
			strcpy(m_curinfolist[m_itemindex].cname,dlg.m_cname);
			m_curinfolist[m_itemindex].color=dlg.m_color;
			m_curinfolist[m_itemindex].compare=dlg.m_compare;
			m_curinfolist[m_itemindex].width=dlg.m_width;
			m_curinfolist[m_itemindex].scale=dlg.m_scale;
			if(dlg.m_reference=="左坐标系")
    			m_curinfolist[m_itemindex].reference=0;
			else
				m_curinfolist[m_itemindex].reference=1;
			m_curinfolist[m_itemindex].precission=dlg.m_precission;
			m_curinfolist[m_itemindex].swing=dlg.m_swing;
			strcpy(m_curinfolist[m_itemindex].ename,dlg.m_ename);
			UpdateItem(m_curinfolist[m_itemindex].cname);
		}
	}
	
	*pResult = 0;
}

void CSecondCurConfig::UpdateItem(LPTSTR cname)
{
	m_infolist.SetItemText(m_itemindex,0,cname);
}

void CSecondCurConfig::OnColor() 
{
	// TODO: Add your control notification handler code here
	if (m_color==m_corls.m_Color) return;
  
 	m_color=m_corls.m_Color;
	UpdateColor();

}

void CSecondCurConfig::UpdateColor()
{
	if (m_corls.m_Color!=m_color)
	{
		m_corls.m_Color=m_color;
		m_corls.Invalidate();
	}
}

void CSecondCurConfig::OnColor3() 
{
	// TODO: Add your control notification handler code here
	if (m_color3==m_corl.m_Color) return;
  
 	m_color3=m_corl.m_Color;
	UpdateColor3();

}

void CSecondCurConfig::UpdateColor3()
{
	if (m_corl.m_Color!=m_color3)
	{
		m_corl.m_Color=m_color3;
		m_corl.Invalidate();
	}
}

void CSecondCurConfig::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_bChecked)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow();
		GetDlgItem(IDC_EDIT1)->EnableWindow();
		GetDlgItem(IDC_EDIT26)->EnableWindow();
		GetDlgItem(IDC_COLOR)->EnableWindow();
		GetDlgItem(IDC_COLOR3)->EnableWindow();
		GetDlgItem(IDC_EDIT29)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT26)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLOR3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT29)->EnableWindow(FALSE);
	}
}

void CSecondCurConfig::OnStnClickedColor3()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSecondCurConfig::OnStnClickedColor()
{
	// TODO: 在此添加控件通知处理程序代码
}
