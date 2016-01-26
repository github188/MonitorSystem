// HMProxyProp.cpp : implementation file
//

#include "stdafx.h"
#include "dialog/TyHMProxyProp.h"
#include "canvas/HMProject.h"
#include "dialog/FontProp.h"
#include "dialog/BmpSelectPage.h"
#include "canvas/HMProject.h"
#include "dialog/SelectVectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTyHMProxyProp property page

IMPLEMENT_DYNCREATE(CTyHMProxyProp, CPropertyPage)

CTyHMProxyProp::CTyHMProxyProp() : CPropertyPage(CTyHMProxyProp::IDD)
{
	//{{AFX_DATA_INIT(CTyHMProxyProp)
	m_hmname = _T("");
	m_display = -1;
	m_bFlash = FALSE;
	m_strBmp1 = _T("");
	m_strBmp2 = _T("");
	m_strVect1 = _T("");
	m_strText1 = _T("");
	m_strText2 = _T("");
	m_strVect2 = _T("");
	//}}AFX_DATA_INIT
	ZeroMemory(m_TextFont,sizeof(char)*16);
}

CTyHMProxyProp::~CTyHMProxyProp()
{
}

void CTyHMProxyProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTyHMProxyProp)
	DDX_CBString(pDX, IDC_COMBO1, m_hmname);
	DDV_MaxChars(pDX, m_hmname, 32);
	DDX_Radio(pDX, IDC_RADIO1, m_display);
	DDX_Check(pDX, IDC_CHECK1, m_bFlash);
	DDX_Text(pDX, IDC_EDIT2, m_strBmp1);
	DDV_MaxChars(pDX, m_strBmp1, 32);
	DDX_Text(pDX, IDC_EDIT3, m_strBmp2);
	DDV_MaxChars(pDX, m_strBmp2, 32);
	DDX_Text(pDX, IDC_EDIT4, m_strVect1);
	DDV_MaxChars(pDX, m_strVect1, 32);
	DDX_Text(pDX, IDC_EDIT11, m_strText1);
	DDV_MaxChars(pDX, m_strText1, 32);
	DDX_Text(pDX, IDC_EDIT12, m_strText2);
	DDV_MaxChars(pDX, m_strText2, 32);
	DDX_Text(pDX, IDC_EDIT5, m_strVect2);
	DDV_MaxChars(pDX, m_strVect2, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTyHMProxyProp, CPropertyPage)
	//{{AFX_MSG_MAP(CTyHMProxyProp)
	ON_BN_CLICKED(IDC_TEXT_COLOR1, OnTextColor1)
	ON_BN_CLICKED(IDC_TEXT_COLOR2, OnTextColor2)
	ON_BN_CLICKED(IDC_VECT_COLOR1, OnVectColor1)
	ON_BN_CLICKED(IDC_VECT_COLOR2, OnVectColor2)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_BN_CLICKED(IDC_BMP1, OnBmp1)
	ON_BN_CLICKED(IDC_BMP2, OnBmp2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_VECT1, OnVect1)
	ON_BN_CLICKED(IDC_VECT2, OnVect2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTyHMProxyProp message handlers

void CTyHMProxyProp::SetProp(char hmname[], int display, BOOL bFlash, 
														 char str1[], char str2[], COLORREF color1, 
														 COLORREF color2, BOOL bFangXiang, 
														 float fTextHeight, float fTextWidth, 
														 float fJqWidth,char TextFont[],BOOL bTranslate1,
														 BOOL bTranslate2)
{
	m_hmname=hmname;
	m_display=display;
	m_bFlash=bFlash;

	switch (display)
	{
	case 0:
	case 1:
	case 2:
		{//位图
			m_strBmp1=str1;
			m_strBmp2=str2;
			m_bTranslate1=bTranslate1;
			m_bTranslate2=bTranslate2;
			m_TranslateColor1=color1;
			m_TranslateColor2=color2;
		}
	//break;

		{//矢量图
			m_strVect1=str1;
			m_strVect2=str2;
			m_VectColor1=color1;
			m_VectColor2=color2;
			m_bTranslate1=bTranslate1;
			m_bTranslate2=bTranslate2;
		}
	//    break;
		{//文本
			m_strText1=str1;
			m_strText2=str2;
			m_TextColor1=color1;
			m_TextColor2=color2;
			m_bFangXiang=bFangXiang;
			m_TextHeight=fTextHeight;			
			m_TextWidth=fTextWidth;
			m_JqWidth=fJqWidth;
			memcpy(m_TextFont,TextFont,sizeof(char)*16);
			m_TextFont[15]=0x00;
		}
		break;
	}
}

void CTyHMProxyProp::GetProp(char* hmname, int& display, BOOL &bFlash,
														 char* str1, char* str2, COLORREF &color1, 
														 COLORREF &color2, BOOL &bFangXiang,float& fTextHeight,  
														 float& fTextWidth, float& fJqWidth,char* TextFont,
														 BOOL& bTranslate1,BOOL& bTranslate2)
														 
{
	strcpy(hmname,m_hmname);
	display=m_display;
	bFlash=m_bFlash;
	switch (m_display)
	{
	case 0:
		{//位图
			strcpy(str1,m_strBmp1);
			strcpy(str2,m_strBmp2);
			bTranslate1=m_bTranslate1;
			bTranslate2=m_bTranslate2;
			color1=m_TranslateColor1;
			color2=m_TranslateColor2;
		}
	break;
	case 1:
		{//矢量图
			strcpy(str1,m_strVect1);
			strcpy(str2,m_strVect2);
			color1=m_VectColor1;
			color2=m_VectColor2;
			bTranslate1=m_bTranslate1;
			bTranslate2=m_bTranslate2;
		}
	break;
	case 2:
		{//文本
			strcpy(str1,m_strText1);
			strcpy(str2,m_strText2);
			color1=m_TextColor1;
			color2=m_TextColor2;
			bFangXiang=m_bFangXiang;
			fTextHeight=m_TextHeight;			
			fTextWidth=m_TextWidth;
			fJqWidth=m_JqWidth;
			strcpy(TextFont,m_TextFont);
		}
	break;
	}
}

BOOL CTyHMProxyProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

/*
	pCtrl=(CSpinButtonCtrl *)GetDlgItem(IDC_COMBO1);
	ASSERT(pCtrl!=NULL);
	pCtrl->SetRange(8,100);
  pCtrl->SetPos(m_len);

*/
  //m_HMIDList

	m_ctrlVectColor1.SubclassDlgItem(IDC_VECT_COLOR1,this);
	m_ctrlVectColor2.SubclassDlgItem(IDC_VECT_COLOR2,this);
 	
	if (m_ctrlVectColor1.m_Color!=m_VectColor1)
	{
		m_ctrlVectColor1.m_Color=m_VectColor1;
		m_ctrlVectColor1.Invalidate();
	}
	if (m_ctrlVectColor2.m_Color!=m_VectColor2)
	{
		m_ctrlVectColor2.m_Color=m_VectColor2;
		m_ctrlVectColor2.Invalidate();
	}

	m_ctrlTextColor1.SubclassDlgItem(IDC_TEXT_COLOR1,this);
	m_ctrlTextColor2.SubclassDlgItem(IDC_TEXT_COLOR2,this);

	if (m_ctrlTextColor1.m_Color!=m_TextColor1)
	{
		m_ctrlTextColor1.m_Color=m_TextColor1;
		m_ctrlTextColor1.Invalidate();
	}
	if (m_ctrlTextColor2.m_Color!=m_TextColor2)
	{
		m_ctrlTextColor2.m_Color=m_TextColor2;
		m_ctrlTextColor2.Invalidate();
	}

	InitHMList();
	UpdateUI();		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTyHMProxyProp::OnTextColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_TextColor1==m_ctrlTextColor1.m_Color) return;
 	m_TextColor1=m_ctrlTextColor1.m_Color;
			
}

void CTyHMProxyProp::OnTextColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_TextColor2==m_ctrlTextColor2.m_Color) return;
 	m_TextColor2=m_ctrlTextColor2.m_Color;
	
}

void CTyHMProxyProp::OnVectColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_VectColor1==m_ctrlVectColor1.m_Color) return;
 	m_VectColor1=m_ctrlVectColor1.m_Color;
	
}

void CTyHMProxyProp::OnVectColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_VectColor2==m_ctrlVectColor2.m_Color) return;
 	m_VectColor2=m_ctrlVectColor2.m_Color;
	
}

void CTyHMProxyProp::OnFont() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("画面代理..") );
	CFontProp dlgfont;
	dlgfont.SetFontProp(m_TextHeight,m_TextWidth,m_JqWidth,m_bFangXiang,m_TextColor1,m_TextFont);
	sheet.AddPage((CPropertyPage*)&dlgfont);

	if (sheet.DoModal() == IDOK)
	{
		dlgfont.GetFontProp(m_TextHeight,m_TextWidth,m_JqWidth,m_bFangXiang,m_TextColor1,m_TextFont);
		if (m_TextColor1==m_ctrlTextColor1.m_Color) return;
		m_ctrlTextColor1.m_Color=m_TextColor1;
		m_ctrlTextColor1.Invalidate();
		return;
	}	
}

void CTyHMProxyProp::OnBmp1() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("画面代理.."));
	CBmpSelectPage dlg;
	dlg.SetContent(m_strBmp1,m_bTranslate1,m_TranslateColor1);
	sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		m_strBmp1=dlg.GetContent(m_bTranslate1,m_TranslateColor1);
		UpdateData(FALSE);
	}
}

void CTyHMProxyProp::OnBmp2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("画面代理.."));
	CBmpSelectPage dlg;
	dlg.SetContent(m_strBmp2,m_bTranslate2,m_TranslateColor2);
	sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		m_strBmp2=dlg.GetContent(m_bTranslate2,m_TranslateColor2);
 		UpdateData(FALSE);
	}
}

void CTyHMProxyProp::UpdateUI()
{
	CWnd* pCtrl; 
  //位图
	pCtrl=GetDlgItem(IDC_EDIT2);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==0));
   
	pCtrl=GetDlgItem(IDC_EDIT3);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==0));

	pCtrl=GetDlgItem(IDC_BMP1);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==0));
   
	pCtrl=GetDlgItem(IDC_BMP2);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==0));

  //矢量图
	pCtrl=GetDlgItem(IDC_EDIT4);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));
   
	pCtrl=GetDlgItem(IDC_EDIT5);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));

	pCtrl=GetDlgItem(IDC_VECT1);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));
   
	pCtrl=GetDlgItem(IDC_VECT2);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));

	pCtrl=GetDlgItem(IDC_VECT_COLOR1);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));
   
	pCtrl=GetDlgItem(IDC_VECT_COLOR2);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==1));

	m_ctrlVectColor1.Invalidate();
	m_ctrlVectColor2.Invalidate();

  //文本
	pCtrl=GetDlgItem(IDC_EDIT11);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==2));
   
	pCtrl=GetDlgItem(IDC_EDIT12);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==2));

	pCtrl=GetDlgItem(IDC_FONT);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==2));

	pCtrl=GetDlgItem(IDC_TEXT_COLOR1);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==2));
   
	pCtrl=GetDlgItem(IDC_TEXT_COLOR2);
	ASSERT(pCtrl!=NULL);
	pCtrl->EnableWindow((m_display==2));

	m_ctrlTextColor1.Invalidate();
	m_ctrlTextColor2.Invalidate();
}

void CTyHMProxyProp::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();		
}

void CTyHMProxyProp::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();		
}

void CTyHMProxyProp::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();	
}

void CTyHMProxyProp::InitHMList()
{
	CComboBox *pCtrl;
	pCtrl=(CComboBox *)GetDlgItem(IDC_COMBO1);
	ASSERT(pCtrl!=NULL);
	POSITION pos;
	tagHMID* pRec;
	pos = CHMProject::getInstance()->m_HMIDList.GetHeadPosition();
	while (pos!=NULL)
	{
		pRec=CHMProject::getInstance()->m_HMIDList.GetNext(pos);
		ASSERT(pRec!=NULL);
		if (pRec!=NULL)
			pCtrl->AddString(pRec->strHMName);
	}
}

void CTyHMProxyProp::OnVect1() 
{
	// TODO: Add your control notification handler code here

	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;
 
	dlg.SetContent(m_strVect1,m_bTranslate1,m_VectColor1);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
 		UpdateData(TRUE);
		dlg.GetContent(m_strVect1,m_bTranslate1,m_VectColor1);
		m_ctrlVectColor1.m_Color=m_VectColor1;
		m_ctrlVectColor1.Invalidate();

 		UpdateData(FALSE);
	} 
}

void CTyHMProxyProp::OnVect2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;
 
	dlg.SetContent(m_strVect2,m_bTranslate2,m_VectColor2);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		dlg.GetContent(m_strVect2,m_bTranslate2,m_VectColor2);
		m_ctrlVectColor2.m_Color=m_VectColor2;
		m_ctrlVectColor2.Invalidate();
 	
		UpdateData(FALSE);
	}
}
