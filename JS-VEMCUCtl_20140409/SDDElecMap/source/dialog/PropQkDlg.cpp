// PropQkDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropQkDlg.h"


// CPropQkDlg 对话框

IMPLEMENT_DYNAMIC(CPropQkDlg, CPropertyPage)
CPropQkDlg::CPropQkDlg()
	: CPropertyPage(CPropQkDlg::IDD)
	, m_strDes(_T(""))
	, m_strName(_T(""))
{
}

CPropQkDlg::~CPropQkDlg()
{
}

void CPropQkDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strDes);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 17);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
}


BEGIN_MESSAGE_MAP(CPropQkDlg, CPropertyPage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
END_MESSAGE_MAP()

BOOL CPropQkDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitColum() ;
	FillList() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPropQkDlg::InitColum(void)
{
	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;

	lvcol.pszText="群控组英文名";
	lvcol.iSubItem=0;
	lvcol.cx=150;
	m_ctlList.InsertColumn(0,&lvcol);

	lvcol.pszText="群控组中文名";
	lvcol.iSubItem=1;
	lvcol.cx=150;
	m_ctlList.InsertColumn(1,&lvcol);

	m_ctlList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);
}

void CPropQkDlg::FillList(void)
{
	char head[3] ;
	int nversion ;
	int ncount = 0 ;
	CString strPath = "c:\\sunpac\\par\\global\\qk.data" ;
	CFile fe ;

	bool bSuccess = fe.Open(strPath ,CFile::modeReadWrite | CFile::typeBinary) ;
	if(bSuccess == true)
	{
		char ename[17] ;
		char cname[33] ;
		
		fe.Read((unsigned char*)head ,sizeof(char)*3) ;
		fe.Read((unsigned char*)&nversion ,sizeof(int)) ;
		fe.Read((unsigned char*)&ncount ,sizeof(int)) ;
		
		int nNodeCount ;
        for(int i = 0 ;i < ncount ;i++)
		{
			memset(ename ,0 ,17) ;
			memset(cname ,0 ,33) ;

			fe.Read((unsigned char*)ename ,sizeof(char)*17) ;
			fe.Read((unsigned char*)cname ,sizeof(char)*33) ;
			fe.Read((unsigned char*)&nNodeCount ,sizeof(int)) ;

			m_ctlList.InsertItem(i ,ename) ;
			m_ctlList.SetItemText(i ,1 ,cname) ;
		}
	}
}

void CPropQkDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nindex = (int)m_ctlList.GetFirstSelectedItemPosition() - 1 ;
	//UpdateData(true) ;	
	//if(m_strDes == "")
	//	m_strDes = m_ctlList.GetItemText(nindex ,1) ;

	m_strName = m_ctlList.GetItemText(nindex ,0) ;
	m_strDes = m_ctlList.GetItemText(nindex ,1) ;
	UpdateData(false) ;
	*pResult = 0;
}

void CPropQkDlg::GetParam(char* ename, char* des)
{
	strcpy(ename ,m_strName) ;
	strcpy(des ,m_strDes) ;
}

void CPropQkDlg::SetParam(char* ename, char* des)
{
	m_strName = ename ;
	m_strDes = des ;
}
