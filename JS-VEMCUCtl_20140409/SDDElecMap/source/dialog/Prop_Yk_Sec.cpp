// Prop_Yk_Sec.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/Prop_Yk_Sec.h"


// CProp_Yk_Sec 对话框

IMPLEMENT_DYNAMIC(CProp_Yk_Sec, CPropertyPage)
CProp_Yk_Sec::CProp_Yk_Sec()
	: CPropertyPage(CProp_Yk_Sec::IDD)
	, m_bWatchMan(FALSE)
	, m_bGroup(FALSE)
	,	m_dwFlag(0)
{
}

CProp_Yk_Sec::~CProp_Yk_Sec()
{
}

void CProp_Yk_Sec::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bWatchMan);
	DDX_Check(pDX, IDC_CHECK2, m_bGroup);
	DDX_Control(pDX, IDC_LIST1, m_CtrlGroup);
	ASSERT(pDX!=NULL);
	if ((pDX!=NULL)&&(pDX->m_bSaveAndValidate))
	{
		Save_Group_Flag();
	}
}


BEGIN_MESSAGE_MAP(CProp_Yk_Sec, CPropertyPage)
END_MESSAGE_MAP()


// CProp_Yk_Sec 消息处理程序

BOOL CProp_Yk_Sec::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_CtrlGroup.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT); 
	m_CtrlGroup.InsertColumn(0,"组号",LVCFMT_CENTER,40);
	m_CtrlGroup.InsertColumn(1,"控制组名称",LVCFMT_CENTER,200);
	m_CtrlGroup.SetItemCount(32);
  Init_Ctrl_Group();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

// 初始化控制组
void CProp_Yk_Sec::Init_Ctrl_Group(void)
{
	CString sect,groupno;
	int validate,num=0;
	char buf[256]; 
	for (int i=0;i<=31;i++ )
	{
		sect.Format("group%d",i);
		validate=GetPrivateProfileInt(sect,"validate",0,"c:\\sunpac\\par\\global\\yk_group.ini");
		if (validate==0) continue;
		if (GetPrivateProfileString(sect,"name","",buf,256,"c:\\sunpac\\par\\global\\yk_group.ini")>256)
			continue;
		buf[32]=0;
		//groupno.Format("%s[%d]",buf,i);
		groupno.Format("%d",i);
		m_CtrlGroup.InsertItem(num,groupno);
		m_CtrlGroup.SetItemText(num,1,buf);
		if ((m_dwFlag &( 1<<i))==0)
			m_CtrlGroup.SetCheck(num,FALSE);
		else
		  m_CtrlGroup.SetCheck(num,TRUE);
		m_CtrlGroup.SetItemData(num,(DWORD)i);
		num++;
	}
}

void CProp_Yk_Sec::Save_Group_Flag(void)
{
	int loop=m_CtrlGroup.GetItemCount();
	m_dwFlag=0;
	DWORD bit;
	for (int i=0;i<loop;i++)
	{
		if (m_CtrlGroup.GetCheck(i))
		{
			bit=m_CtrlGroup.GetItemData(i);
			m_dwFlag=(m_dwFlag| 1<<bit); 
		}
	}
}

void CProp_Yk_Sec::SetData(bool bWatchMan, bool bGroup, DWORD dwFlag)
{
   m_bWatchMan=bWatchMan;
	 m_bGroup=bGroup;
	 m_dwFlag=dwFlag;
}

void CProp_Yk_Sec::GetData(bool& bWatchMan, bool& bGroup, DWORD& dwFlag)
{
	 if (m_bWatchMan)
			bWatchMan=true;
	 else 
			bWatchMan=false;
	 if (m_bGroup)
			bGroup=true;
	 else
			bGroup=false;
	 dwFlag=m_dwFlag;
}
