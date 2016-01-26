// McCurveSheet.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/McCurveDialog.h"
#include "dialog/mccurveimportdialog.h"
#include "canvas/jdjh.h"

// CMcCurveSheet

IMPLEMENT_DYNAMIC(CMcCurveSheet, CPropertySheet)
CMcCurveSheet::CMcCurveSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMcCurveSheet::CMcCurveSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&mccurvepage1);
	AddPage(&mccurvepage2);
}

CMcCurveSheet::~CMcCurveSheet()
{
}


BEGIN_MESSAGE_MAP(CMcCurveSheet, CPropertySheet)
END_MESSAGE_MAP()


// CMcCurveSheet 消息处理程序
// D:\Documents and Settings\Administrator\My Documents\sp_draw\McCurveSheet.cpp : 实现文件
//

#include "../../stdafx.h"
//#include "sp_draw.h"
#include "dialog/mccurvedialog.h"


// CMcCurvePage1 对话框

IMPLEMENT_DYNAMIC(CMcCurvePage1, CPropertyPage)
CMcCurvePage1::CMcCurvePage1()
	: CPropertyPage(CMcCurvePage1::IDD)
	, m_fLeftcoord(0)
	, m_fTopcoord(0)
	, m_fXlen(0)
	, m_fYlen(0)
	, m_fImin(0)
	, m_fImax(0)
	, m_fVmin(0)
	, m_fVmax(0)
	, m_nNode(0)
	, m_nLine(0)
	, m_nRtu(0)
	, m_bGt(TRUE)
	, m_nIprecision(0)
	, m_nVprecision(0)
	, m_strmcename(_T(""))
	, m_bonlyrealline(FALSE)
	, m_UpName(_T(""))
	, m_DownName(_T(""))
	, m_realtime_style(FALSE)
{

}

CMcCurvePage1::~CMcCurvePage1()
{

}

void CMcCurvePage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fLeftcoord);
	DDX_Text(pDX, IDC_EDIT2, m_fTopcoord);
	DDX_Text(pDX, IDC_EDIT13, m_fXlen);
	DDX_Text(pDX, IDC_EDIT14, m_fYlen);
	DDX_Text(pDX, IDC_EDIT15, m_fImin);
	DDX_Text(pDX, IDC_EDIT16, m_fImax);
	DDX_Text(pDX, IDC_EDIT17, m_fVmin);
	DDX_Text(pDX, IDC_EDIT18, m_fVmax);
	DDX_Text(pDX, IDC_EDIT5, m_nNode);
	DDV_MinMaxByte(pDX, m_nNode, 0, 127);
	DDX_Text(pDX, IDC_EDIT21, m_nLine);
	DDV_MinMaxByte(pDX, m_nLine, 0, 127);
	DDX_Text(pDX, IDC_EDIT22, m_nRtu);
	DDV_MinMaxInt(pDX, m_nRtu, 0, 127);
	DDX_Check(pDX, IDC_CHECK1, m_bGt);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nIprecision);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nVprecision);
	DDX_Control(pDX, IDC_EDIT15, m_IminEdit);
	DDX_Control(pDX, IDC_EDIT16, m_ImaxEdit);
	DDX_Control(pDX, IDC_EDIT17, m_VminEdit);
	DDX_Control(pDX, IDC_EDIT18, m_VmaxEdit);
	DDX_Text(pDX, IDC_EDIT6, m_strmcename);
	DDV_MaxChars(pDX, m_strmcename, 11);
	DDX_Check(pDX, IDC_CHECK2, m_bonlyrealline);
	DDX_Text(pDX, IDC_UPNAMEEDIT, m_UpName);
	DDX_Text(pDX, IDC_DOWNNAMEEDIT, m_DownName);
	DDX_Check(pDX, IDC_CHECK3, m_realtime_style);
}


BEGIN_MESSAGE_MAP(CMcCurvePage1, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
END_MESSAGE_MAP()


// CMcCurvePage1 消息处理程序
// D:\Documents and Settings\Administrator\My Documents\sp_draw\McCurveDialog.cpp : 实现文

// CMcCurvePage2 对话框
#include "../../stdafx.h"
//#include "sp_draw.h"
#include "dialog/mccurvedialog.h"
#include "canvas/colorlist.h"

IMPLEMENT_DYNAMIC(CMcCurvePage2, CPropertyPage)
CMcCurvePage2::CMcCurvePage2()
	: CPropertyPage(CMcCurvePage2::IDD)
	, nCurveSel(0)
	, m_BchannelNO(0)
	, m_BDot(0)
	, m_BPrecision(0)
	, m_fRatio(0)
	, m_nColor(0)
	, editindex(-1)
	, m_nlinenode(0)
	, m_nlineline(0)
	, m_nlinertu(0)
	, m_unit(_T(""))
{

}

CMcCurvePage2::~CMcCurvePage2()
{

}

void CMcCurvePage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO1, nCurveSel);
	DDX_Text(pDX, IDC_EDIT19, m_BchannelNO);
	DDV_MinMaxByte(pDX, m_BchannelNO, 0, 255);
	DDX_Text(pDX, IDC_EDIT20, m_BDot);
	DDV_MinMaxByte(pDX, m_BDot, 0, 255);
	DDX_CBIndex(pDX, IDC_COMBO2, m_BPrecision);
	DDX_Text(pDX, IDC_EDIT21, m_fRatio);
	DDX_Control(pDX, IDC_LIST2, m_CurveList);

	DDX_Control(pDX, IDC_COMBO1,ModeBox);
	DDX_Control(pDX, IDC_COLORSTATIC, m_staticcolor);
	DDX_Text(pDX, IDC_EDIT22, m_nlinenode);
	DDV_MinMaxInt(pDX, m_nlinenode, 0, 127);
	DDX_Text(pDX, IDC_EDIT24, m_nlineline);
	DDV_MinMaxInt(pDX, m_nlineline, 0, 127);
	DDX_Text(pDX, IDC_EDIT25, m_nlinertu);
	DDV_MinMaxInt(pDX, m_nlinertu, 0, 127);
	DDX_Text(pDX, IDC_EDIT30, m_unit);
	DDV_MaxChars(pDX, m_unit, 7);
}


BEGIN_MESSAGE_MAP(CMcCurvePage2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnNMClickList2)
	ON_STN_CLICKED(IDC_COLORSTATIC, OnStnClickedColorstatic)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CMcCurvePage2 消息处理程序
void CMcCurvePage2::DrawListTitle()
{
	m_CurveList.InsertColumn(0,"序号",LVCFMT_CENTER,30,0);
	m_CurveList.InsertColumn(1,"通道号",LVCFMT_CENTER,50,1);
	m_CurveList.InsertColumn(2,"点号",LVCFMT_CENTER,40,2);
	m_CurveList.InsertColumn(3,"比例",LVCFMT_CENTER,40,3);
	m_CurveList.InsertColumn(4,"小数精度",LVCFMT_CENTER,70,4);
	m_CurveList.InsertColumn(5,"颜色",LVCFMT_CENTER,50,5);
	m_CurveList.InsertColumn(6,"节点号",LVCFMT_CENTER,50,6);
	m_CurveList.InsertColumn(7,"线路号",LVCFMT_CENTER,50,7);
	m_CurveList.InsertColumn(8,"单元号",LVCFMT_CENTER,50,8);
	m_CurveList.InsertColumn(9,"简称",LVCFMT_CENTER,60,8);
	m_CurveList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}
void CMcCurvePage2::OnBnClickedButton1()//添加曲线
{
	if (m_CurveList.GetItemCount()>=8)
	{
		AfxMessageBox("不能超过8条");
		return;
	}

	CString str;
	str.Format("%d",m_CurveList.GetItemCount()+1);
	m_CurveList.InsertItem(m_CurveList.GetItemCount(),str);
	TCurve newcurve;
	ZeroMemory(&newcurve,sizeof(TCurve));
	newcurve.Bprecision=2;
	
	switch (nCurveSel)
	{
	case 0:
		newcurve.fcurveratio=6.5;
		pmccurvedata->gtIcurvearray.Add(newcurve);
		break;
	case 1:
		newcurve.fcurveratio=300;
		pmccurvedata->gtVcurvearray.Add(newcurve);
		break;
	case 2:
		newcurve.fcurveratio=6.5;
		pmccurvedata->zbIcurvearray.Add(newcurve);
		break;
	case 3:
		newcurve.fcurveratio=300;
		pmccurvedata->zbVcurvearray.Add(newcurve);
		break;
	}
}

void CMcCurvePage2::OnBnClickedButton11()//删除曲线
{
	int delindex =(int) m_CurveList.GetFirstSelectedItemPosition()-1;
	if (delindex==-1)
	{
		AfxMessageBox("请选择要删除的曲线");
		return;
	}
	
	switch (nCurveSel)
	{
	case 0:
		pmccurvedata->gtIcurvearray.RemoveAt(delindex);
		SetList(&pmccurvedata->gtIcurvearray);
		break;

	case 1:
		pmccurvedata->gtVcurvearray.RemoveAt(delindex);
		SetList(&pmccurvedata->gtVcurvearray);
		break;

	case 2:
		pmccurvedata->zbIcurvearray.RemoveAt(delindex);
		SetList(&pmccurvedata->zbIcurvearray);
		break;

	case 3:
		pmccurvedata->zbVcurvearray.RemoveAt(delindex);
		SetList(&pmccurvedata->zbVcurvearray);
		break;

	default:
		break;
	}
}

void CMcCurvePage2::OnBnClickedButton2()//提交
{
	if (editindex==-1)
	{
		AfxMessageBox("请选择要提交的曲线");
		return;
	}

	UpdateData();

	TCurve newcurve;
	memset(&newcurve,0,sizeof(TCurve));
	newcurve.BcurvechannelNO=m_BchannelNO;
	newcurve.Bdot=m_BDot;
	newcurve.Bprecision=m_BPrecision+1;
	CString fa;
	fa.Format("%0.3f",m_fRatio);
	newcurve.fcurveratio=atof(fa);
	m_staticcolor.GetColor(&m_nColor);
	newcurve.ncurvecolor=m_nColor;
	newcurve.nlinenode=m_nlinenode;
	newcurve.nlineline=m_nlineline;
	newcurve.nlinertu=m_nlinertu;
	if (m_unit.GetLength()>0)
		strcpy(newcurve.unit,m_unit);

	switch (nCurveSel)
	{
	case 0:
		pmccurvedata->gtIcurvearray.SetAt(editindex,newcurve);
		SetList(&pmccurvedata->gtIcurvearray);
		break;

	case 1:
		pmccurvedata->gtVcurvearray.SetAt(editindex,newcurve);
		SetList(&pmccurvedata->gtVcurvearray);
		break;

	case 2:
		pmccurvedata->zbIcurvearray.SetAt(editindex,newcurve);
		SetList(&pmccurvedata->zbIcurvearray);
		break;

	case 3:
		pmccurvedata->zbVcurvearray.SetAt(editindex,newcurve);
		SetList(&pmccurvedata->zbVcurvearray);
		break;
	
	default:
		break;
	}
}

BOOL CMcCurvePage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	DrawListTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CMcCurvePage2::SetList(CURVEARRAY *pCurvearray)
{
	m_CurveList.DeleteAllItems();
	TCurve *pTempCurve;
	int i;
	CString strtext;
	int curvecount=pCurvearray->GetCount();
	
	for (i=0; i<curvecount; i++)
	{
		pTempCurve=&pCurvearray->GetAt(i);
		strtext.Format("%d",i+1);
		m_CurveList.InsertItem(i,strtext);
		strtext.Format("%d",pTempCurve->BcurvechannelNO);
		m_CurveList.SetItemText(i,1,strtext);
		strtext.Format("%d",pTempCurve->Bdot);
		m_CurveList.SetItemText(i,2,strtext);
		strtext.Format("%.3f",pTempCurve->fcurveratio);
		m_CurveList.SetItemText(i,3,strtext);
		strtext.Format("%d",pTempCurve->Bprecision);
		m_CurveList.SetItemText(i,4,strtext);
		strtext.Format("%d",pTempCurve->nlinenode);
		m_CurveList.SetItemText(i,6,strtext);
		strtext.Format("%d",pTempCurve->nlineline);
		m_CurveList.SetItemText(i,7,strtext);
		strtext.Format("%d",pTempCurve->nlinertu);
		m_CurveList.SetItemText(i,8,strtext);
		strtext.Format("%s",pTempCurve->unit);
		m_CurveList.SetItemText(i,9,strtext);
	}
}

TCurve* CMcCurvePage2::GetCurSingleLine(int singleline)
{
	TCurve *pTempCurve;
	switch (nCurveSel)
	{
	case 0:
		pTempCurve=&pmccurvedata->gtIcurvearray.GetAt(singleline);
		break;
	case 1:
		pTempCurve=&pmccurvedata->gtVcurvearray.GetAt(singleline);
		break;
	case 2:
		pTempCurve=&pmccurvedata->zbIcurvearray.GetAt(singleline);
		break;
	case 3:
		pTempCurve=&pmccurvedata->zbVcurvearray.GetAt(singleline);
		break;
	default:
		break;
	}
	return pTempCurve;
}

void CMcCurvePage2::OnCbnSelchangeCombo1()//切换曲线
{
	UpdateData();
	CURVEARRAY *pcurvearray;
	switch (nCurveSel)
	{
	case 0:
		pcurvearray=&pmccurvedata->gtIcurvearray;
		break;
	case 1:
		pcurvearray=&pmccurvedata->gtVcurvearray;
		break;
	case 2:
		pcurvearray=&pmccurvedata->zbIcurvearray;
		break;
	case 3:
		pcurvearray=&pmccurvedata->zbVcurvearray;
		break;
	}
	SetList(pcurvearray);
	m_CurveList.RedrawWindow();
}

void CMcCurvePage2::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iItem=pNMListView->iItem;
	if (iItem==-1)
	{
		return;
	}
	editindex=iItem;
	CString str;
	str.Format("第%d条曲线参数",iItem+1);
	GetDlgItem(IDC_STATIC1)->SetWindowText(str);
	TCurve *pSingleCurve=GetCurSingleLine(iItem);
	m_BchannelNO=pSingleCurve->BcurvechannelNO;
	m_BDot=pSingleCurve->Bdot;
	m_fRatio=pSingleCurve->fcurveratio;
	m_BPrecision=pSingleCurve->Bprecision-1;
	m_nColor=pSingleCurve->ncurvecolor;
	m_nlineline=pSingleCurve->nlineline;
	m_nlinenode=pSingleCurve->nlinenode;
	m_nlinertu=pSingleCurve->nlinertu;
	m_unit=pSingleCurve->unit;
	UpdateData(FALSE);
	m_CurveList.RedrawWindow();
	m_staticcolor.SetColor(m_nColor);
	*pResult = 0;
}

BOOL CMcCurvePage1::OnInitDialog()
{
	m_nIprecision=pmccurvedata->nIprecision;
	m_nVprecision=pmccurvedata->nVprecision;
	m_fLeftcoord = pmccurvedata->fleftcoord ;
	m_fTopcoord= pmccurvedata->ftopcoord;
	m_fXlen= pmccurvedata->frightcoord-pmccurvedata->fleftcoord;
	m_fYlen=pmccurvedata->fbottomcoord-pmccurvedata->ftopcoord;
	m_fImin=pmccurvedata->fImin;
	m_fImax=pmccurvedata->fImax;
	m_fVmin=pmccurvedata->fVmin;
	m_fVmax=pmccurvedata->fVmax;
	m_nNode=pmccurvedata->nNode;
	m_nLine=pmccurvedata->nLine;
	m_nRtu=pmccurvedata->nRtu;
	m_bGt=pmccurvedata->bgtmode;
	m_strmcename=pmccurvedata->strMcename;
	m_bonlyrealline=pmccurvedata->bonlyrealline;
	m_realtime_style=pmccurvedata->m_realtime_style;
	m_UpName=pmccurvedata->UpName;
	m_DownName=pmccurvedata->DownName;
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CMcCurvePage1::OnOK()
{
	if (!UpdateData())
	{
		return;
	}

	pmccurvedata->nIprecision=m_nIprecision;
	pmccurvedata->nVprecision=m_nVprecision;
	pmccurvedata->fleftcoord=m_fLeftcoord ;
	pmccurvedata->frightcoord=m_fTopcoord;
	pmccurvedata->frightcoord=pmccurvedata->fleftcoord+m_fXlen;
	pmccurvedata->fbottomcoord=pmccurvedata->ftopcoord+m_fYlen;
	pmccurvedata->fImin=m_fImin;
	pmccurvedata->fImax=m_fImax;
	pmccurvedata->fVmin=m_fVmin;
	pmccurvedata->fVmax=m_fVmax;
	pmccurvedata->strMcename=m_strmcename;
	pmccurvedata->bonlyrealline=m_bonlyrealline;
	pmccurvedata->m_realtime_style=m_realtime_style;
	pmccurvedata->nNode=m_nNode;
	pmccurvedata->nLine=m_nLine;
	pmccurvedata->nRtu=m_nRtu;
	pmccurvedata->bgtmode=m_bGt;
	pmccurvedata->bNeedsave=TRUE;
	pmccurvedata->UpName=m_UpName;
	pmccurvedata->DownName=m_DownName;
	
	EndDialog(IDOK);
}

BOOL CMcCurveSheet::OnInitDialog()
{
	mccurvepage1.pmccurvedata=pmccurvedata;
	mccurvepage2.pmccurvedata=pmccurvedata;
	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;
}

void CMcCurvePage2::OnStnClickedColorstatic()
{
	
}

void CMcCurvePage1::OnBnClickedCheck1()
{
	UpdateData();
	pmccurvedata->bgtmode=m_bGt;
}

BOOL CMcCurvePage2::OnSetActive()
{
	nCurveSel=0;
	UpdateData(FALSE);
	if (pmccurvedata->gtIcurvearray.GetSize()!=0)
	{
		SetList(&pmccurvedata->gtIcurvearray);
		m_CurveList.RedrawWindow();
	}
	else 
	{
		m_CurveList.DeleteAllItems();
	}

	CMcCurveSheet* pSheet=(CMcCurveSheet*)GetParent();
	CMcCurvePage1* pPage1=(CMcCurvePage1*)pSheet->GetPage(0);
	BOOL isgt=pPage1->m_bGt;
	
	if (isgt)
	{
		ModeBox.ResetContent();
		ModeBox.AddString("贯通电流");
		ModeBox.AddString("贯通电压");
		ModeBox.AddString("自闭电流");
		ModeBox.AddString("自闭电压");
	}
	else
	{
		ModeBox.ResetContent();
		ModeBox.AddString("电流");
		ModeBox.AddString("电压");
	}

	ModeBox.SetCurSel(0);
	
	return CPropertyPage::OnSetActive();
}

void CMcCurvePage2::OnBnClickedButton3()//挑点
{
	/*char myename[17];
	char mycname[33];
	HINSTANCE hinst=LoadLibrary("C:\\SUNPAC\\DLL\\FindDot.dll");
	if (hinst==NULL)
	{
		AfxMessageBox("dll调用失败");
		return;
	}
	pShowFindDot=(ShowFindDotDlg*)GetProcAddress(hinst,"ShowFindDotDlg");

	if (pShowFindDot==NULL)
	{
		AfxMessageBox("调用函数失败");
		FreeLibrary(hinst);
		hinst=NULL;
		return;
	}
		if ((*pShowFindDot)(1,myename,mycname))
		{
			GetDlgItem(IDC_EDIT20)->SetWindowText(mycname);
			GetDlgItem(IDC_EDIT22)->SetWindowText(myename);
		}
	
	FreeLibrary(hinst);*/
}

void CMcCurvePage1::OnBnClickedButton1()//导入
{
	UpdateData();
	CString strfilepath;

	CMcCurveImportDialog importdlg;
	if (::GetServer(strfilepath))
	{
		importdlg.m_strSelsection=m_strmcename;
		importdlg.strfilepath=strfilepath;
	}

	if (importdlg.DoModal()==IDOK)
	{
		pmccurvedata->strMcename=importdlg.GetSelSection();
		pmccurvedata->SetPath(strfilepath);
	
		if (pmccurvedata->ReadCurveData())
		{
			m_strmcename=pmccurvedata->strMcename;
			m_nNode=pmccurvedata->nNode;
			m_nLine=pmccurvedata->nLine;
			m_nRtu=pmccurvedata->nRtu;
			m_fImin=pmccurvedata->fImin;
			m_fImax=pmccurvedata->fImax;
			m_fVmin=pmccurvedata->fImin;
			m_fVmax=pmccurvedata->fVmax;
			m_bGt=pmccurvedata->bgtmode;
			m_bonlyrealline=pmccurvedata->bonlyrealline;
			m_realtime_style=pmccurvedata->m_realtime_style;
			UpdateData(FALSE);
			AfxMessageBox("导入成功");
		}
	//else 
	//{
		//AfxMessageBox("不存在此段值");
	//}
	
	}
	
}

void CMcCurvePage1::OnBnClickedButton13()
{
	UpdateData();
	TCurve temp;
	int i;

	for (i=0; i<pmccurvedata->gtIcurvearray.GetSize(); i++)
	{
		temp=pmccurvedata->gtIcurvearray.GetAt(i);
		temp.nlineline=m_nLine;
		temp.nlinenode=m_nNode;
		temp.nlinertu=m_nRtu;
		pmccurvedata->gtIcurvearray.SetAt(i,temp);
	}

	for (i=0; i<pmccurvedata->gtVcurvearray.GetSize(); i++)
	{
		temp=pmccurvedata->gtVcurvearray.GetAt(i);
		temp.nlineline=m_nLine;
		temp.nlinenode=m_nNode;
		temp.nlinertu=m_nRtu;
		pmccurvedata->gtVcurvearray.SetAt(i,temp);
	}

	for (i=0; i<pmccurvedata->zbIcurvearray.GetSize(); i++)
	{
		temp=pmccurvedata->zbIcurvearray.GetAt(i);
		temp.nlineline=m_nLine;
		temp.nlinenode=m_nNode;
		temp.nlinertu=m_nRtu;
		pmccurvedata->zbIcurvearray.SetAt(i,temp);
	}

	for (i=0; i<pmccurvedata->zbVcurvearray.GetSize(); i++)
	{
		temp=pmccurvedata->zbVcurvearray.GetAt(i);
		temp.nlineline=m_nLine;
		temp.nlinenode=m_nNode;
		temp.nlinertu=m_nRtu;
		pmccurvedata->zbVcurvearray.SetAt(i,temp);
	}
}

void CMcCurvePage2::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CPropertyPage::OnOK();
}

BOOL CMcCurvePage1::OnKillActive()
{
	UpdateData();

	if (m_strmcename.GetLength()==0)
	{
		AfxMessageBox("请添写图元名称");
		CPropertySheet* pSheet = (CPropertySheet*) GetParent();
		//pSheet->SetActivePage(0);
		return FALSE;
	}

	return CPropertyPage::OnKillActive();
}

void CMcCurvePage1::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}
