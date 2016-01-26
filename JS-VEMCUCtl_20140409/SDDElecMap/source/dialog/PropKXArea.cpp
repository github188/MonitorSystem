// PropKXArea.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropKXArea.h"


// CPropKXArea 对话框

IMPLEMENT_DYNAMIC(CPropKXArea, CPropertyPage)
CPropKXArea::CPropKXArea()
	: CPropertyPage(CPropKXArea::IDD)
	, m_strDes(_T(""))
	, m_nCur(-1)
	, m_strQD(_T(""))
	, m_nCur2(-1)
{
	memset(m_biaoshi ,0 ,4) ;
	memset(m_qdename ,0 ,4) ;
}

CPropKXArea::~CPropKXArea()
{
}

void CPropKXArea::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDes);
	DDX_Control(pDX, IDC_COMBO1, m_ctlCombox);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_CBString(pDX, IDC_COMBO2, m_strQD);
	DDX_Control(pDX, IDC_COMBO2, m_QDComBox);
}


BEGIN_MESSAGE_MAP(CPropKXArea, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CPropKXArea 消息处理程序

BOOL CPropKXArea::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitData() ;

	GetCurSel1(m_biaoshi) ;
	m_ctlCombox.SetCurSel(m_nCur) ;
	OnCbnSelchangeCombo1() ;

	GetCurSel2(m_qdename);
	m_QDComBox.SetCurSel(m_nCur2) ;
	OnCbnSelchangeCombo2() ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPropKXArea::InitData(void)
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

void CPropKXArea::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_nCur = m_ctlCombox.GetCurSel() ;
	if(m_nCur < 0)
		return ;

	while(m_QDComBox.DeleteString(0) != CB_ERR)
		m_QDComBox.DeleteString(0) ;

	m_QDComBox.SetCurSel(-1) ;

	/************连接动态连接库****************/
	typedef void _stdcall TGetQDParam(CArray<TY_QD,TY_QD&>& QDArr,char* KXID);
	TGetQDParam *pGetQDParam = NULL;
	HINSTANCE dllhandle;
	dllhandle = LoadLibrary("C:\\sunpac\\dll\\ReadFAParam.dll");
	if(dllhandle==0) 
	{
		return;
	}
	pGetQDParam=(TGetQDParam*)::GetProcAddress(dllhandle,"GetQDParam");
	if(pGetQDParam==NULL)
	{
		FreeLibrary(dllhandle);
		return;
	}
	/************连接动态连接库****************/

	if(m_QDArr.GetCount() > 0)
		m_QDArr.RemoveAll();

	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	(*pGetQDParam)(m_QDArr,KXData.KXID);

	int QDCount = m_QDArr.GetCount();
	for(int i = 0; i < QDCount; i++)
	{
		TY_QD QDData;
		memset(&QDData,0,sizeof(TY_QD));

		QDData = m_QDArr.GetAt(i);

		m_QDComBox.AddString(QDData.QDCName);
	}
}

void CPropKXArea::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCur < 0)
		return ;

	m_nCur2 = m_QDComBox.GetCurSel() ;
	if(m_nCur2 < 0)
		return ;

	while(m_ctlList.DeleteString(0) !=  LB_ERR)
		m_ctlList.DeleteString(0) ;

	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	TY_QD QDData;
	memset(&QDData,0,sizeof(TY_QD));
	QDData = m_QDArr.GetAt(m_nCur2);
	
	CString str ;
	str.Format("馈线段标示:%s" ,KXData.KXID) ;
	m_ctlList.AddString(str) ;

	str.Format("馈线段中文名:%s" ,KXData.KXCName) ;
	m_ctlList.AddString(str) ;

	str.Format("馈线区段标示:%s" ,QDData.QDID) ;
	m_ctlList.AddString(str) ;

	str.Format("馈线区段中文名:%s" ,QDData.QDCName) ;
	m_ctlList.AddString(str) ;
}

void CPropKXArea::GetParam(char* ename ,char* cname ,char* qdename ,char* qdcname ,char* des)
{
	TY_KX KXData;
	memset(&KXData,0,sizeof(TY_KX));
	KXData = m_KXArr.GetAt(m_nCur);

	TY_QD QDData;
	memset(&QDData,0,sizeof(TY_QD));
	QDData = m_QDArr.GetAt(m_nCur2);

	strcpy(ename ,KXData.KXID) ;
	strcpy(cname ,KXData.KXCName) ;
	strcpy(qdename ,QDData.QDID) ;
	strcpy(qdcname,QDData.QDCName);
	strcpy(des ,m_strDes) ;
}

void CPropKXArea::SetParam(char* ename ,char* qdename ,char* des)
{
	m_strDes = des ;

	strcpy(m_biaoshi ,ename) ;
	strcpy(m_qdename ,qdename) ;
}

void CPropKXArea::GetCurSel1(char* ename)
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

void CPropKXArea::GetCurSel2(char* qdename)
{
	if(qdename == NULL)
	{
		m_nCur2 = 0;
		return;
	}

	int QDCount = m_QDArr.GetCount();
	for(int i = 0; i < QDCount; i++)
	{
		TY_QD QDData;
		memset(&QDData,0,sizeof(TY_QD));

		QDData = m_QDArr.GetAt(i);

		if(strcmp(qdename,QDData.QDID) == 0)
		{
			m_nCur2 = i;
			return;
		}
	}

	m_nCur2 = 0;
}
