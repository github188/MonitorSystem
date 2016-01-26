// PropKXDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropKXDlg.h"

// CPropKXDlg 对话框

IMPLEMENT_DYNAMIC(CPropKXDlg, CPropertyPage)
CPropKXDlg::CPropKXDlg()
	: CPropertyPage(CPropKXDlg::IDD)
	, m_strKXKG(_T(""))
	, m_strDes(_T(""))
	, m_nCur(-1)
	, m_nCur2(-1)
{
	memset(m_biaoshi ,0 ,4) ;
	memset(m_kgename ,0 ,17) ;
}

CPropKXDlg::~CPropKXDlg()
{
	if(m_KXArr.GetCount() > 0)
		m_KXArr.RemoveAll();

	if(m_KGArr.GetCount() > 0)
		m_KGArr.RemoveAll();
}

void CPropKXDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO2, m_strKXKG);
	DDX_Control(pDX, IDC_COMBO1, m_ctlCombox);
	DDX_Text(pDX, IDC_EDIT1, m_strDes);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_Control(pDX, IDC_COMBO2, m_KGComBox);
}


BEGIN_MESSAGE_MAP(CPropKXDlg, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CPropKXDlg 消息处理程序

void CPropKXDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCur = m_ctlCombox.GetCurSel() ;
	
	while(m_KGComBox.DeleteString(0) != CB_ERR)
		m_KGComBox.DeleteString(0) ;

	m_KGComBox.SetCurSel(-1) ;

	if(m_nCur < 0)
		return ;
	
	/************连接动态连接库****************/
	typedef void _stdcall TGetKGParam(CArray<TY_KG,TY_KG&>& KGArr,char* KXID);
	TGetKGParam *pGetKGParam = NULL;
	HINSTANCE dllhandle;
	dllhandle = LoadLibrary("C:\\sunpac\\dll\\ReadFAParam.dll");
	if(dllhandle==0) 
	{
		return;
	}
	pGetKGParam=(TGetKGParam*)::GetProcAddress(dllhandle,"GetKGParam");
	if(pGetKGParam==NULL)
	{
		FreeLibrary(dllhandle);
		return;
	}
	/************连接动态连接库****************/

	if(m_KGArr.GetCount() > 0)
		m_KGArr.RemoveAll();

	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	(*pGetKGParam)(m_KGArr,KXData.KXID);

	int KGCount = m_KGArr.GetCount();
	for(int i = 0; i < KGCount; i++)
	{
		TY_KG KGData;
		memset(&KGData,0,sizeof(TY_KG));

		KGData = m_KGArr.GetAt(i);

		m_KGComBox.AddString(KGData.KGEName);
	}
}

BOOL CPropKXDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetKXName();

	GetCurSel1(m_biaoshi) ;
	m_ctlCombox.SetCurSel(m_nCur) ;
	OnCbnSelchangeCombo1() ;
	GetCurSel2(m_kgename);
	m_KGComBox.SetCurSel(m_nCur2) ;
	OnCbnSelchangeCombo2() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPropKXDlg::GetKXName(void)
{
	/************连接动态连接库****************/
	typedef void _stdcall TGetKXParam(CArray<TY_KX,TY_KX&>& KXArr);
	TGetKXParam *pGetKXParam = NULL;
	HINSTANCE dllhandle;
	dllhandle = LoadLibrary("C:\\sunpac\\dll\\ReadFAParam.dll");
	if(dllhandle==0) 
	{
		return;
	}
	pGetKXParam=(TGetKXParam*)::GetProcAddress(dllhandle,"GetKXParam");
	if(pGetKXParam==NULL)
	{
		FreeLibrary(dllhandle);
		return;
	}
	/************连接动态连接库****************/

	if(m_KXArr.GetCount() > 0)
		m_KXArr.RemoveAll();

	(*pGetKXParam)(m_KXArr);

	int KXCount = m_KXArr.GetCount();
	for(int i = 0; i < KXCount; i++)
	{
		TY_KX Data;
		memset(&Data,0,sizeof(TY_KX));

		Data = m_KXArr.GetAt(i);

		m_ctlCombox.AddString(Data.KXCName);
	}
}

void CPropKXDlg::GetParam(char* ename, char* cname ,char* kg ,char* des)
{
	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	TY_KG KGData;
	memset(&KGData,0,sizeof(TY_KG));
	KGData = m_KGArr.GetAt(m_nCur2);

	strcpy(ename ,KXData.KXID) ;
	strcpy(cname ,KXData.KXCName) ;
	strcpy(kg ,KGData.KGEName) ;
	strcpy(des ,m_strDes) ;
}

void CPropKXDlg::SetParam(char* ename ,char* kgename ,char* des)
{
	m_strDes = des ;

	strcpy(m_biaoshi ,ename) ;
	strcpy(m_kgename ,kgename) ;
}


void CPropKXDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCur < 0)
		return ;

	m_nCur2 = m_KGComBox.GetCurSel() ;
	if(m_nCur2 < 0)
		return ;

	while(m_ctlList.DeleteString(0) !=  LB_ERR)
		m_ctlList.DeleteString(0) ;
	
	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	TY_KG KGData;
	memset(&KGData,0,sizeof(TY_KG));
	KGData = m_KGArr.GetAt(m_nCur2);

	CString str ;
	str.Format("馈线段标示:%s" ,KXData.KXID) ;
	m_ctlList.AddString(str) ;

	str.Format("馈线段中文名:%s" ,KXData.KXCName) ;
	m_ctlList.AddString(str) ;

	str.Format("馈线开关:%s" ,KGData.KGEName) ;
	m_ctlList.AddString(str) ;
}

void CPropKXDlg::GetCurSel1(char* ename)
{
	if(ename == NULL)
	{
		m_nCur = 0;
		return;
	}

	int KXCount = m_KXArr.GetCount() ;
	for(int i = 0 ;i < KXCount ;i++)
	{
		TY_KX KXData ;
		memset(&KXData,0,sizeof(TY_KX));

		KXData = m_KXArr.GetAt(i);

		if(strcmp(ename ,KXData.KXID) == 0)
		{
			m_nCur = i ;
			return;
		}
	}

	m_nCur = 0;
}

void CPropKXDlg::GetCurSel2(char* kgename)
{
	if(kgename == NULL)
	{
		m_nCur2 = 0;
		return;
	}

	int KGCount = m_KGArr.GetCount();
	for(int i = 0; i < KGCount; i++)
	{
		TY_KG KGData;
		memset(&KGData,0,sizeof(TY_KG));

		KGData = m_KGArr.GetAt(i);

		if(strcmp(kgename,KGData.KGEName) == 0)
		{
			m_nCur2 = i;
			return;
		}
	}

	m_nCur2 = 0;
}
