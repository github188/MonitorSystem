// YcCurveDialog.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YcCurveDialog.h"

// CYcCurvePage 对话框

IMPLEMENT_DYNAMIC(CYcCurvePage, CPropertyPage)
CYcCurvePage::CYcCurvePage()
	: CPropertyPage(CYcCurvePage::IDD)
	, m_fMin(0)
	, m_fMax(0)
	, m_Ename(_T(""))
	, m_Cname(_T(""))
	
{
	m_nEditIndex = -1 ;
}

CYcCurvePage::~CYcCurvePage()
{
}

void CYcCurvePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CList);
	DDX_Text(pDX, IDC_EDIT1, m_Ename);
	DDX_Text(pDX, IDC_EDIT26, m_Cname);
	DDX_Control(pDX, IDC_STATIC_COLOR1, m_StaticColor);
	//DDX_Control(pDX, IDC_CHECK2, m_haveplan);
	//DDX_Control(pDX, IDC_CHECK1, m_showplan);
	
}


BEGIN_MESSAGE_MAP(CYcCurvePage, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList2)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_STN_CLICKED(IDC_STATIC_COLOR1, OnStnClickedStaticColor1)
	ON_BN_CLICKED(IDC_ADDP, OnBnClickedAddp)
	ON_BN_CLICKED(IDC_DELETEP, OnBnClickedDeletep)
END_MESSAGE_MAP()


BOOL CYcCurvePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_CList.SetTitle();
	DrawListTitle();
	SetList(&curvearray);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CYcCurvePage::DrawListTitle()
{
    m_CList.InsertColumn(0,"序号",LVCFMT_CENTER,50,0);
	m_CList.InsertColumn(1,"英文名",LVCFMT_CENTER,80,1);
	m_CList.InsertColumn(2,"中文名",LVCFMT_CENTER,80,2);
	m_CList.InsertColumn(3,"颜色",LVCFMT_CENTER,60,3);
	m_CList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}

void CYcCurvePage::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iItem=pNMListView->iItem;
	if (iItem==-1)
	{
		return;
	}
	m_nEditIndex=iItem ;
	CString str;
	str.Format("第%d条曲线参数",iItem+1);
	GetDlgItem(IDC_STATIC)->SetWindowText(str);
	ycurve_info *pSingleCurve=GetCurSingleLine(iItem);
	m_Ename = pSingleCurve->ename ;
	m_Cname = pSingleCurve->cname ;
	m_nColor = pSingleCurve->color ;
	UpdateData(FALSE);
	
	m_CList.RedrawWindow();
	m_StaticColor.SetColor(m_nColor) ;
	*pResult = 0;
}

void CYcCurvePage::SetList(YcCurveInfo *pCurvearray)
{
	m_CList.DeleteAllItems();
	ycurve_info *pTempCurve;
	int i;
	CString strtext;
	int curvecount=pCurvearray->GetCount();
	for (i=0; i<curvecount; i++)
	{
		pTempCurve=&pCurvearray->GetAt(i);
		strtext.Format("%d",i+1);
		m_CList.InsertItem(i,strtext);

		strtext.Format("%s",pTempCurve->ename);
		m_CList.SetItemText(i,1,strtext);

		strtext.Format("%s",pTempCurve->cname);
		m_CList.SetItemText(i,2,strtext);
	}
}

ycurve_info* CYcCurvePage::GetCurSingleLine(int singleline)
{
	ycurve_info *pTempCurve;
	pTempCurve = &curvearray.GetAt(singleline) ;
	return pTempCurve;
}

void CYcCurvePage::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_CList.GetItemCount()>=8)
	{
		AfxMessageBox("不能超过8条");
		return;
	}
	CString str;
	str.Format("%d",m_CList.GetItemCount()+1);
	m_CList.InsertItem(m_CList.GetItemCount(),str);
	ycurve_info newcurve;
	strcpy(newcurve.ename ,"") ;
	strcpy(newcurve.cname ,"") ;
	newcurve.color = RGB(0,0,0) ;
	curvearray.Add(newcurve);
	
}

void CYcCurvePage::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
   int delindex =(int) m_CList.GetFirstSelectedItemPosition()-1;
	if (delindex==-1)
	{
		AfxMessageBox("请选择要删除的曲线");
		return;
	}
	curvearray.RemoveAt(delindex) ;
	int i = curvearray.GetCount() ;
	SetList(&curvearray) ;
}

void CYcCurvePage::OnStnClickedStaticColor1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CYcCurvePage::OnBnClickedAddp()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nEditIndex-1<0)
	{
		AfxMessageBox("曲线的优先级已经最高!");
		return;
	}
	
	ycurve_info newcurve;
    ycurve_info newcurve3;
	ycurve_info *newcurve1=GetCurSingleLine(m_nEditIndex);
    ycurve_info *newcurve2=GetCurSingleLine(m_nEditIndex-1);
    strcpy(newcurve3.ename ,newcurve2->ename);
	strcpy(newcurve3.cname ,newcurve2->cname);
	 newcurve3.color=newcurve2->color;
	strcpy(newcurve.ename ,newcurve1->ename);
	strcpy(newcurve.cname ,newcurve1->cname);
	//UpdateData();
    newcurve.color=newcurve1->color;
	curvearray.SetAt(m_nEditIndex-1,newcurve);
    //UpdateData();
	SetList(&curvearray) ;
	curvearray.SetAt(m_nEditIndex,newcurve3);
	SetList(&curvearray) ;
	
}

void CYcCurvePage::OnBnClickedDeletep()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nEditIndex+1>m_CList.GetItemCount()-1)
	{
		AfxMessageBox("曲线的优先级已经最低!");
		return;
	}
	ycurve_info newcurve;
    ycurve_info newcurve3;
	ycurve_info *newcurve1=GetCurSingleLine(m_nEditIndex);
    ycurve_info *newcurve2=GetCurSingleLine(m_nEditIndex+1);
    strcpy(newcurve3.ename ,newcurve2->ename);
	strcpy(newcurve3.cname ,newcurve2->cname);
	 newcurve3.color=newcurve2->color;
	strcpy(newcurve.ename ,newcurve1->ename);
	strcpy(newcurve.cname ,newcurve1->cname);
	//UpdateData();
    newcurve.color=newcurve1->color;
	curvearray.SetAt(m_nEditIndex+1,newcurve);
    //UpdateData();
	SetList(&curvearray) ;
	curvearray.SetAt(m_nEditIndex,newcurve3);
	SetList(&curvearray) ;
}

void CYcCurvePage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nEditIndex==-1)
	{
		AfxMessageBox("请选择要提交的曲线");
		return;
	}
	if(curvearray.GetCount() < 1)
	{
		AfxMessageBox("请先添加曲线") ;
		return ;
	}
	UpdateData();
	ycurve_info newcurve ;
	strcpy(newcurve.ename ,m_Ename) ;
	strcpy(newcurve.cname ,m_Cname) ;
	m_StaticColor.GetColor(&m_nColor);
	newcurve.color = m_nColor ;

	curvearray.SetAt(m_nEditIndex,newcurve);
	SetList(&curvearray) ;

}

void CYcCurvePage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	char dllf[256],s[256],myename[17],mycname[33];
 	HINSTANCE hmydll;
 	sprintf(dllf,YC_FINDDOTDLLNAME);
 	hmydll=LoadLibrary(dllf);
 	if (hmydll==NULL)
	{
 		sprintf(s,"%s 调入失败！",dllf);
 		AfxMessageBox(s);
 		return;
 	}

 	ShowFindDotDlg * pf;
 	pf=(ShowFindDotDlg *)::GetProcAddress(hmydll,"ShowFindDotDlg");
 	if (pf==NULL)
	{
 		sprintf(s,"%s 调入函数ShowFindDotDlg 失败！",dllf);
 		AfxMessageBox(s);
 		FreeLibrary(hmydll);
 		hmydll=NULL;
 		return;
 	}

 	CWnd * pp=this->GetParent();
	BYTE dotkind=0;

	dotkind = 1 ;
 	if ((*pf)(dotkind,myename,mycname))
 	{
 		UpdateData(true);
 		m_Ename=myename;
		char tempchar[9] ;
		StrCpyN(tempchar ,mycname ,9) ;
		m_Cname = tempchar ;
		
 		UpdateData(false);
 	}

 	FreeLibrary(hmydll);
}

//#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YcCurveDialog.h"

// CYcCurveSheet

IMPLEMENT_DYNAMIC(CYcCurveSheet, CPropertySheet)
CYcCurveSheet::CYcCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&Page3);
	AddPage(&Page2);
	AddPage(&Page1);
	AddPage(&Page4);
}

CYcCurveSheet::CYcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&Page3);
	AddPage(&Page2);
	AddPage(&Page1);
	AddPage(&Page4);
}

CYcCurveSheet::~CYcCurveSheet()
{
}


BEGIN_MESSAGE_MAP(CYcCurveSheet, CPropertySheet)
END_MESSAGE_MAP()


// CYcCurveSheet 消息处理程序


// D:\制图包\sp_draw\YcCurveDialog.cpp : 实现文件
//

//#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YcCurveDialog.h"


// CYcCurvePage1 对话框

IMPLEMENT_DYNAMIC(CYcCurvePage1, CPropertyPage)
CYcCurvePage1::CYcCurvePage1()
	: CPropertyPage(CYcCurvePage1::IDD)
	, m_xfmin(0)
	, m_yfmin(0)
	, m_fheight(0)
	, m_xfwide(0)
{
}

CYcCurvePage1::~CYcCurvePage1()
{
}

void CYcCurvePage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_xfmin);
	DDX_Text(pDX, IDC_EDIT26, m_yfmin);
	//DDX_Control(pDX, IDC_EDIT5, m_fwide);
	DDX_Text(pDX, IDC_EDIT27, m_fheight);
	DDX_Text(pDX, IDC_EDIT5, m_xfwide);
}


BEGIN_MESSAGE_MAP(CYcCurvePage1, CPropertyPage)
END_MESSAGE_MAP()


// CYcCurvePage1 消息处理程序
// YcCurveDialog.cpp : 实现文件
//

//#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YcCurveDialog.h"


// CYcCurvePage2 对话框

IMPLEMENT_DYNAMIC(CYcCurvePage2, CPropertyPage)
CYcCurvePage2::CYcCurvePage2()
	: CPropertyPage(CYcCurvePage2::IDD)
	, m_tyName(_T(""))
	, m_fMin(0)
	, m_fMax(0)
	, m_ykdu(0)
	, m_ydanwei(_T(""))
{
}

CYcCurvePage2::~CYcCurvePage2()
{
}

void CYcCurvePage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT27, m_tyName);
	DDX_Text(pDX, IDC_EDIT1, m_fMin);
	DDX_Text(pDX, IDC_EDIT26, m_fMax);
	DDX_Text(pDX, IDC_EDIT5, m_ykdu);
	DDX_Text(pDX, IDC_EDIT28, m_ydanwei);
}


BEGIN_MESSAGE_MAP(CYcCurvePage2, CPropertyPage)
END_MESSAGE_MAP()




