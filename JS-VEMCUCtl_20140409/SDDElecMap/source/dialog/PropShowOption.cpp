// PropShowOption.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropShowOption.h"
#include "dialog/FontProp.h"
#include "dialog/BmpSelectPage.h"
#include "dialog/SelectVectorDlg.h"


// CPropShowOption 对话框

IMPLEMENT_DYNAMIC(CPropShowOption, CPropertyPage)
CPropShowOption::CPropShowOption()
	: CPropertyPage(CPropShowOption::IDD)
	, m_strBmp1(_T(""))
	, m_strBmp2(_T(""))
	, m_strVect1(_T(""))
	, m_strVect2(_T(""))
	, m_strText1(_T(""))
	, m_strText2(_T(""))
	, m_display(0)
{
}

CPropShowOption::~CPropShowOption()
{
}

void CPropShowOption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_display);
	DDX_Text(pDX, IDC_EDIT2, m_strBmp1);
	DDV_MaxChars(pDX, m_strBmp1, 32);
	DDX_Text(pDX, IDC_EDIT3, m_strBmp2);
	DDV_MaxChars(pDX, m_strBmp2, 32);
	DDX_Text(pDX, IDC_EDIT4, m_strVect1);
	DDV_MaxChars(pDX, m_strVect1, 32);
	DDX_Text(pDX, IDC_EDIT5, m_strVect2);
	DDV_MaxChars(pDX, m_strVect2, 32);
	DDX_Text(pDX, IDC_EDIT11, m_strText1);
	DDV_MaxChars(pDX, m_strText1, 32);
	DDX_Text(pDX, IDC_EDIT12, m_strText2);
	DDV_MaxChars(pDX, m_strText2, 32);
}


BEGIN_MESSAGE_MAP(CPropShowOption, CPropertyPage)
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
END_MESSAGE_MAP()


// CPropShowOption 消息处理程序

BOOL CPropShowOption::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化

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

	UpdateUI();		

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CPropShowOption::SetProp(int display,
														 char str1[], char str2[], COLORREF color1, 
														 COLORREF color2, BOOL bFangXiang, 
														 float fTextHeight, float fTextWidth, 
														 float fJqWidth,char TextFont[],BOOL bTranslate1,
														 BOOL bTranslate2)
{
	m_display=display;
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

	default:
		display=2;

	}
}

void CPropShowOption::GetProp(int& display,
														 char* str1, char* str2, COLORREF &color1, 
														 COLORREF &color2, BOOL &bFangXiang,float& fTextHeight,  
														 float& fTextWidth, float& fJqWidth,char* TextFont,
														 BOOL& bTranslate1,BOOL& bTranslate2)
														 
{
	display=m_display;
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


void CPropShowOption::OnTextColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_TextColor1==m_ctrlTextColor1.m_Color) return;
 	m_TextColor1=m_ctrlTextColor1.m_Color;
			
}

void CPropShowOption::OnTextColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_TextColor2==m_ctrlTextColor2.m_Color) return;
 	m_TextColor2=m_ctrlTextColor2.m_Color;
	
}

void CPropShowOption::OnVectColor1() 
{
	// TODO: Add your control notification handler code here
	if (m_VectColor1==m_ctrlVectColor1.m_Color) return;
 	m_VectColor1=m_ctrlVectColor1.m_Color;
	
}

void CPropShowOption::OnVectColor2() 
{
	// TODO: Add your control notification handler code here
	if (m_VectColor2==m_ctrlVectColor2.m_Color) return;
 	m_VectColor2=m_ctrlVectColor2.m_Color;
	
}

void CPropShowOption::OnFont() 
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

void CPropShowOption::OnBmp1() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("画面代理.."));
	CBmpSelectPage dlg;
	dlg.SetContent(m_strBmp1,m_bTranslate1,m_TranslateColor1);
	sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		m_strBmp1=dlg.GetContent(m_bTranslate1,m_TranslateColor1);
		UpdateData(FALSE);
	}
	
}

void CPropShowOption::OnBmp2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("画面代理.."));
	CBmpSelectPage dlg;
	dlg.SetContent(m_strBmp2,m_bTranslate2,m_TranslateColor2);
	sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		m_strBmp2=dlg.GetContent(m_bTranslate2,m_TranslateColor2);
 		UpdateData(FALSE);
	}
	
}

void CPropShowOption::UpdateUI()
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

	UpdateWindow();

}

void CPropShowOption::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();		
}

void CPropShowOption::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();		
}

void CPropShowOption::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateUI();	
}


void CPropShowOption::OnVect1() 
{
	// TODO: Add your control notification handler code here

	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;
 
	dlg.SetContent(m_strVect1,m_bTranslate1,m_VectColor1);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetContent(m_strVect1,m_bTranslate1,m_VectColor1);
		m_ctrlVectColor1.m_Color=m_VectColor1;
		m_ctrlVectColor1.Invalidate();

 		UpdateData(FALSE);
	}
	 
}

void CPropShowOption::OnVect2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;
 
	dlg.SetContent(m_strVect2,m_bTranslate2,m_VectColor2);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		dlg.GetContent(m_strVect2,m_bTranslate2,m_VectColor2);
		m_ctrlVectColor2.m_Color=m_VectColor2;
		m_ctrlVectColor2.Invalidate();
 	
		UpdateData(FALSE);

	}

}

