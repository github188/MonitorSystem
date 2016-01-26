// YX_PROP_DIS.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/YX_PROP_DIS.h"
#include "dialog/FontDlg.h"
#include "dialog/BmpSelectPage.h"
#include "dialog/SelectVectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_DIS property page

IMPLEMENT_DYNCREATE(CYX_PROP_DIS, CPropertyPage)

CYX_PROP_DIS::CYX_PROP_DIS(CString text0,CString text1,CString textdis,BOOL canseefla) 
: CPropertyPage(CYX_PROP_DIS::IDD)
{

	m_distext0=text0;
	m_distext1=text1;
	m_distextdis=textdis;

	m_veccolor0=0x000000;
	m_veccolor1=0x000000;
	m_veccolor2=0x000000;
	m_txtcolor0=0x000000;
	m_txtcolor1=0x000000;
	m_txtcolor2=0x000000;
	m_bmpname0[0]='\0';
	m_bmpname1[0]='\0';
	m_bmpname2[0]='\0';
	m_vecname0[0]='\0';
	m_vecname1[0]='\0';
	m_vecname2[0]='\0';
	m_strname0[0]='\0';
	m_strname1[0]='\0';
	m_strname2[0]='\0';
	m_pickind0=-1;
	m_pickind1=-1;
	m_pickind2=-1;

	canseeflash=canseefla;
	//{{AFX_DATA_INIT(CYX_PROP_DIS)
	m_editfh = 0.0f;
	m_editfw = 0.0f;
	m_editx = 0.0f;
	m_edity = 0.0f;
	m_editjj = 0.0f;
	m_checkvaluefc0 = FALSE;
	m_checkvaluefc1 = FALSE;
	m_checkvaluefc2 = FALSE;
	m_checkvalueflash0 = FALSE;
	m_checkvalueflash1 = FALSE;
	m_checkvalueflash2 = FALSE;
	m_radio0 = -1;
	m_radio1 = -1;
	m_radio2 = -1;
	//}}AFX_DATA_INIT
}

CYX_PROP_DIS::~CYX_PROP_DIS()
{
}

void CYX_PROP_DIS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYX_PROP_DIS)
	DDX_Control(pDX, CZ_IDC_STATIC_TEXTDIS, m_main2);
	DDX_Control(pDX, CZ_IDC_STATIC_TEXT1, m_main1);
	DDX_Control(pDX, CZ_IDC_STATIC_TEXT0, m_main0);
	DDX_Control(pDX, CZ_IDC_EDITZJ, m_editcontrolzj);
	DDX_Control(pDX, CZ_IDC_BUTTONFONT, m_buttonfont);
	DDX_Control(pDX, CZ_IDC_STATICZJ, m_staticzj);
	DDX_Control(pDX, CZ_IDC_STATICW, m_staticw);
	DDX_Control(pDX, CZ_IDC_STATICH, m_statich);
	DDX_Control(pDX, CZ_IDC_EDITBMP2, m_editbmp2);
	DDX_Control(pDX, CZ_IDC_EDITBMP1, m_editbmp1);
	DDX_Control(pDX, CZ_IDC_STATICVEC2, m_staticvec2);
	DDX_Control(pDX, CZ_IDC_STATICVEC1, m_staticvec1);
	DDX_Control(pDX, CZ_IDC_STATICVEC0, m_staticvec0);
	DDX_Control(pDX, CZ_IDC_STATICTXTC1, m_statictxtc1);
	DDX_Control(pDX, CZ_IDC_STATICTXTC2, m_statictxtc2);
	DDX_Control(pDX, CZ_IDC_STATICTXTC0, m_statictxtc0);
	DDX_Control(pDX, CZ_IDC_STATICTXT2, m_statictxt2);
	DDX_Control(pDX, CZ_IDC_STATICTXT1, m_statictxt1);
	DDX_Control(pDX, CZ_IDC_STATICTXT0, m_statictxt0);
	DDX_Control(pDX, CZ_IDC_STATICBMP2, m_staticbmp2);
	DDX_Control(pDX, CZ_IDC_STATICBMP1, m_staticbmp1);
	DDX_Control(pDX, CZ_IDC_STATICBMP0, m_staticbmp0);
	DDX_Control(pDX, CZ_IDC_EDITVEC2, m_editvec2);
	DDX_Control(pDX, CZ_IDC_EDITVEC1, m_editvec1);
	DDX_Control(pDX, CZ_IDC_EDITVEC0, m_editvec0);
	DDX_Control(pDX, CZ_IDC_EDITTXT2, m_edittxt2);
	DDX_Control(pDX, CZ_IDC_EDITTXT1, m_edittxt1);
	DDX_Control(pDX, CZ_IDC_EDITTXT0, m_edittxt0);
	DDX_Control(pDX, CZ_IDC_EDITBMP0, m_editbmp0);
	DDX_Control(pDX, CZ_IDC_CHECKFLASH2, m_checkflash2);
	DDX_Control(pDX, CZ_IDC_CHECKFLASH1, m_checkflash1);
	DDX_Control(pDX, CZ_IDC_CHECKFLASH0, m_checkflash0);
	DDX_Control(pDX, CZ_IDC_CHECKFC2, m_checkfc2);
	DDX_Control(pDX, CZ_IDC_CHECKFC1, m_checkfc1);
	DDX_Control(pDX, CZ_IDC_CHECKFC0, m_checkfc0);
	DDX_Control(pDX, CZ_IDC_BUTTONVECS2, m_buttonvecs2);
	DDX_Control(pDX, CZ_IDC_BUTTONVECS1, m_buttonvecs1);
	DDX_Control(pDX, CZ_IDC_BUTTONVECS0, m_buttonvecs0);
	DDX_Control(pDX, CZ_IDC_BUTTONBMPS2, m_buttonbmps2);
	DDX_Control(pDX, CZ_IDC_BUTTONBMPS1, m_buttonbmps1);
	DDX_Control(pDX, CZ_IDC_BUTTONBMPS0, m_buttonbmps0);
	DDX_Text(pDX, CZ_IDC_EDITH, m_editfh);
	DDV_MinMaxFloat(pDX, m_editfh, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITW, m_editfw);
	DDV_MinMaxFloat(pDX, m_editfw, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITX, m_editx);
	DDV_MinMaxFloat(pDX, m_editx, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITY, m_edity);
	DDV_MinMaxFloat(pDX, m_edity, 0.f, 32000.f);
	DDX_Text(pDX, CZ_IDC_EDITZJ, m_editjj);
	DDV_MinMaxFloat(pDX, m_editjj, 0.f, 32000.f);
	DDX_Check(pDX, CZ_IDC_CHECKFC0, m_checkvaluefc0);
	DDX_Check(pDX, CZ_IDC_CHECKFC1, m_checkvaluefc1);
	DDX_Check(pDX, CZ_IDC_CHECKFC2, m_checkvaluefc2);
	DDX_Check(pDX, CZ_IDC_CHECKFLASH0, m_checkvalueflash0);
	DDX_Check(pDX, CZ_IDC_CHECKFLASH1, m_checkvalueflash1);
	DDX_Check(pDX, CZ_IDC_CHECKFLASH2, m_checkvalueflash2);
	DDX_Radio(pDX, CZ_IDC_RADIOTXT0, m_radio0);
	DDX_Radio(pDX, CZ_IDC_RADIOTXT1, m_radio1);
	DDX_Radio(pDX, CZ_IDC_RADIOTXT2, m_radio2);
	//}}AFX_DATA_MAP
}

void CYX_PROP_DIS::GetPara(LOGFONT * lfont,COLORREF * color0,COLORREF * color1,COLORREF * colordis,
		char * picname0,char * picname1,char * picnamedis,BYTE * pickind0,BYTE * pickind1,
		BYTE * pickinddis,int * diskind0,int * diskind1,int * diskinddis,float *xx0,float *yy0,float *fontw,float *fonth,float *fontj)
{
	*lfont=m_lfont;

	if (m_pickind0==0) *color0=m_transcolor0;
	if (m_pickind0==1) *color0=m_veccolor0;
	if (m_pickind0==2) *color0=m_txtcolor0;
	if (m_pickind1==0) *color1=m_transcolor1;
	if (m_pickind1==1) *color1=m_veccolor1;
	if (m_pickind1==2) *color1=m_txtcolor1;
	if (m_pickind2==0) *colordis=m_transcolor2;
	if (m_pickind2==1) *colordis=m_veccolor2;
	if (m_pickind2==2) *colordis=m_txtcolor2;

	if ((m_pickind0)==0) memcpy(picname0,m_bmpname0,33*sizeof(char));
	if ((m_pickind0)==1) memcpy(picname0,m_vecname0,33*sizeof(char));
	if (m_pickind0==2) memcpy(picname0,m_strname0,33*sizeof(char));
	if ((m_pickind1)==0) memcpy(picname1,m_bmpname1,33*sizeof(char));
	if ((m_pickind1)==1) memcpy(picname1,m_vecname1,33*sizeof(char));
	if (m_pickind1==2) memcpy(picname1,m_strname1,33*sizeof(char));
	if ((m_pickind2)==0) memcpy(picnamedis,m_bmpname2,33*sizeof(char));
	if ((m_pickind2)==1) memcpy(picnamedis,m_vecname2,33*sizeof(char));
	if (m_pickind2==2) memcpy(picnamedis,m_strname2,33*sizeof(char));

	*pickind0=m_pickind0;
	*pickind1=m_pickind1;
	*pickinddis=m_pickind2;
	*diskind0=m_diskind0;
	*diskind1=m_diskind1;
	*diskinddis=m_diskind2;

	*xx0=x0;
	*yy0=y0;
	*fontw=fontwidth;
	*fonth=fontheight;
	*fontj=fontjj;
}

void CYX_PROP_DIS::SetPara(LOGFONT lfont,COLORREF color0,COLORREF color1,COLORREF colordis,
		char * picname0,char * picname1,char * picnamedis,BYTE pickind0,BYTE pickind1,
		BYTE pickinddis,int diskind0,int diskind1,int diskinddis,float xx0,float yy0,float fontw,float fonth,float fontj)
{
	m_lfont=lfont;
	m_pickind0=pickind0;
	m_pickind1=pickind1;
	m_pickind2=pickinddis;
	m_diskind0=diskind0;
	m_diskind1=diskind1;
	m_diskind2=diskinddis;

	x0=xx0;
	y0=yy0;
	fontwidth=fontw;
	fontheight=fonth;
	fontjj=fontj;

	trans0=false;
	trans1=false;
	trans2=false;
	m_checkvalueflash0=false;
	m_checkvalueflash1=false;
	m_checkvalueflash2=false;
	m_checkvaluefc0=false;
	m_checkvaluefc1=false;
	m_checkvaluefc2=false;
	m_transcolor0=color0;
	m_transcolor1=color1;
	m_transcolor2=colordis;
	m_txtcolor0=color0;
	m_txtcolor1=color1;
	m_txtcolor2=colordis;
	m_veccolor0=color0;
	m_veccolor1=color1;
	m_veccolor2=colordis;
	if ((m_diskind0 & 01)==0x01) {trans0=true;}
	if ((m_diskind1 & 01)==0x01) {trans1=true;}
	if ((m_diskind2 & 01)==0x01) {trans2=true;}

	if ((m_diskind0 & 02)==0x02) {m_checkvaluefc0=true;}
	if ((m_diskind1 & 02)==0x02) {m_checkvaluefc1=true;}
	if ((m_diskind2 & 02)==0x02) {m_checkvaluefc2=true;}

	if ((m_diskind0 & 04)==0x04) {m_checkvalueflash0=true;}
	if ((m_diskind1 & 04)==0x04) {m_checkvalueflash1=true;}
	if ((m_diskind2 & 04)==0x04) {m_checkvalueflash2=true;}

	if (m_pickind0 ==0) memcpy(m_bmpname0,picname0,33*sizeof(char));
	if (m_pickind0 ==1) memcpy(m_vecname0,picname0,33*sizeof(char));
	if (m_pickind0 ==2) memcpy(m_strname0,picname0,33*sizeof(char));
	if (m_pickind1 ==0) memcpy(m_bmpname1,picname1,33*sizeof(char));
	if (m_pickind1 ==1) memcpy(m_vecname1,picname1,33*sizeof(char));
	if (m_pickind1 ==2) memcpy(m_strname1,picname1,33*sizeof(char));
	if (m_pickind2 ==0) memcpy(m_bmpname2,picnamedis,33*sizeof(char));
	if (m_pickind2 ==1) memcpy(m_vecname2,picnamedis,33*sizeof(char));
	if (m_pickind2 ==2) memcpy(m_strname2,picnamedis,33*sizeof(char));
}


BEGIN_MESSAGE_MAP(CYX_PROP_DIS, CPropertyPage)
	//{{AFX_MSG_MAP(CYX_PROP_DIS)
	ON_WM_CREATE()
	ON_BN_CLICKED(CZ_IDC_BUTTONFONT, OnIdcButtonfont)
	ON_BN_CLICKED(CZ_IDC_RADIOTXT0, OnIdcRadiotxt0)
	ON_BN_CLICKED(CZ_IDC_RADIOBMP0, OnIdcRadiobmp0)
	ON_BN_CLICKED(CZ_IDC_RADIOVEC0, OnIdcRadiovec0)
	ON_BN_CLICKED(CZ_IDC_RADIOTXT1, OnIdcRadiotxt1)
	ON_BN_CLICKED(CZ_IDC_RADIOBMP1, OnIdcRadiobmp1)
	ON_BN_CLICKED(CZ_IDC_RADIOVEC1, OnIdcRadiovec1)
	ON_BN_CLICKED(CZ_IDC_RADIOTXT2, OnIdcRadiotxt2)
	ON_BN_CLICKED(CZ_IDC_RADIOBMP2, OnIdcRadiobmp2)
	ON_BN_CLICKED(CZ_IDC_RADIOVEC2, OnIdcRadiovec2)
	ON_BN_CLICKED(CZ_IDC_BUTTONBMPS0, OnIdcButtonbmps0)
	ON_BN_CLICKED(CZ_IDC_BUTTONBMPS1, OnIdcButtonbmps1)
	ON_BN_CLICKED(CZ_IDC_BUTTONBMPS2, OnIdcButtonbmps2)
	ON_BN_CLICKED(CZ_IDC_BUTTONVECS0, OnIdcButtonvecs0)
	ON_BN_CLICKED(CZ_IDC_BUTTONVECS1, OnIdcButtonvecs1)
	ON_BN_CLICKED(CZ_IDC_BUTTONVECS2, OnIdcButtonvecs2)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(CZ_IDC_EDITTXT0, OnEnChangeIdcEdittxt0)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYX_PROP_DIS message handlers

BOOL CYX_PROP_DIS::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_main0.SetWindowText(m_distext0);
	m_main1.SetWindowText(m_distext1);
	m_main2.SetWindowText(m_distextdis);

	// TODO: Add extra initialization here
//		m_normalc.SubclassWindow(hwnd);
//	m_normalc.SetColor(normalcolor);

	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR0,&hwnd);
	m_txtcolorstatic0.SubclassWindow(hwnd);
	m_txtcolorstatic0.SetColor(m_txtcolor0);
	GetDlgItem(CZ_IDC_STATICVECCOLOR0,&hwnd);
	m_veccolorstatic0.SubclassWindow(hwnd);
	m_veccolorstatic0.SetColor(m_veccolor0);
		
	GetDlgItem(CZ_IDC_STATICVECCOLOR1,&hwnd);
	m_veccolorstatic1.SubclassWindow(hwnd);
	m_veccolorstatic1.SetColor(m_veccolor1);
	GetDlgItem(CZ_IDC_STATICTXTCOLOR1,&hwnd);
	m_txtcolorstatic1.SubclassWindow(hwnd);
	m_txtcolorstatic1.SetColor(m_txtcolor1);
		
	GetDlgItem(CZ_IDC_STATICVECCOLOR2,&hwnd);
	m_veccolorstatic2.SubclassWindow(hwnd);
	m_veccolorstatic2.SetColor(m_veccolor2);
	GetDlgItem(CZ_IDC_STATICTXTCOLOR2,&hwnd);
	m_txtcolorstatic2.SubclassWindow(hwnd);
	m_txtcolorstatic2.SetColor(m_txtcolor2);
		
	m_editx=x0;
	m_edity=y0;
	m_editfw=fontwidth;
	m_editfh=fontheight;
	m_editjj=fontjj;

	UpdateData(false);


	HideAll();	

	if (m_pickind0==0) {m_editbmp0.SetWindowText(m_bmpname0);}
	if (m_pickind1==0) {m_editbmp1.SetWindowText(m_bmpname1);}
	if (m_pickind2==0) {m_editbmp2.SetWindowText(m_bmpname2);}

	if (m_pickind0==1) {m_editvec0.SetWindowText(m_vecname0);}
	if (m_pickind1==1) {m_editvec1.SetWindowText(m_vecname1);}
	if (m_pickind2==1) {m_editvec2.SetWindowText(m_vecname2);}

	if (m_pickind0==2) {m_edittxt0.SetWindowText(m_strname0);}
	if (m_pickind1==2) {m_edittxt1.SetWindowText(m_strname1);}
	if (m_pickind2==2) {m_edittxt2.SetWindowText(m_strname2);}

	if (m_pickind0==0) {m_radio0=1;Show0Bmp();}
	if (m_pickind0==1) {m_radio0=2;Show0vec();}
	if (m_pickind0==2) {m_radio0=0;Show0Txt();}

	if (m_pickind1==0) {m_radio1=1;Show1Bmp();}
	if (m_pickind1==1) {m_radio1=2;Show1vec();}
	if (m_pickind1==2) {m_radio1=0;Show1Txt();}

	if (m_pickind2==0) {m_radio2=1;Show2Bmp();}
	if (m_pickind2==1) {m_radio2=2;Show2vec();}
	if (m_pickind2==2) {m_radio2=0;Show2Txt();}

	UpdateData(false);

	if (canseeflash==false)
	{
		m_checkflash0.ShowWindow(SW_HIDE);
		m_checkflash1.ShowWindow(SW_HIDE);
		m_checkflash2.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CYX_PROP_DIS::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


void CYX_PROP_DIS::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_veccolorstatic0.GetColor(&m_veccolor0);
	m_veccolorstatic1.GetColor(&m_veccolor1);
	m_veccolorstatic2.GetColor(&m_veccolor2);
	m_txtcolorstatic0.GetColor(&m_txtcolor0);
	m_txtcolorstatic1.GetColor(&m_txtcolor1);
	m_txtcolorstatic2.GetColor(&m_txtcolor2);

	m_vecname0[0]='\0';
	m_vecname1[0]='\0';
	m_vecname2[0]='\0';
	m_strname0[0]='\0';
	m_strname1[0]='\0';
	m_strname2[0]='\0';
	m_bmpname0[0]='\0';
	m_bmpname1[0]='\0';
	m_bmpname2[0]='\0';

/*	m_transcolor0=0xc0c0c0;
	m_transcolor1=0xc0c0c0;
	m_transcolor2=0xc0c0c0;
*/
	m_editvec0.GetWindowText(m_vecname0,32);
	m_editvec1.GetWindowText(m_vecname1,32);
	m_editvec2.GetWindowText(m_vecname2,32);
	m_editbmp0.GetWindowText(m_bmpname0,32);
	m_editbmp1.GetWindowText(m_bmpname1,32);
	m_editbmp2.GetWindowText(m_bmpname2,32);
	m_edittxt0.GetWindowText(m_strname0,32);
	m_edittxt1.GetWindowText(m_strname1,32);
	m_edittxt2.GetWindowText(m_strname2,32);

	UpdateData(true);
	x0=m_editx;
	y0=m_edity;
	fontwidth=m_editfw;
	fontheight=m_editfh;
	fontjj=m_editjj;

	m_diskind0=0;
	if (trans0) {m_diskind0 = m_diskind0 | 0x01;}
	if (m_checkvaluefc0) {m_diskind0 = m_diskind0 | 0x02;}
	if (m_checkvalueflash0) {m_diskind0 = m_diskind0 | 0x04;}

	m_diskind1=0;
	if (trans1) {m_diskind1 = m_diskind1 | 0x01;}
	if (m_checkvaluefc1) {m_diskind1 = m_diskind1 | 0x02;}
	if (m_checkvalueflash1) {m_diskind1 = m_diskind1 | 0x04;}

	m_diskind2=0;
	if (trans2) {m_diskind2 = m_diskind2 | 0x01;}
	if (m_checkvaluefc2) {m_diskind2 = m_diskind2 | 0x02;}
	if (m_checkvalueflash2) {m_diskind2 = m_diskind2 | 0x04;}

	if (m_radio0==0) m_pickind0=2;
	if (m_radio0==1) m_pickind0=0;
	if (m_radio0==2) m_pickind0=1;
	if (m_radio1==0) m_pickind1=2;
	if (m_radio1==1) m_pickind1=0;
	if (m_radio1==2) m_pickind1=1;
	if (m_radio2==0) m_pickind2=2;
	if (m_radio2==1) m_pickind2=0;
	if (m_radio2==2) m_pickind2=1;

	if (m_pickind0==2)
	{
		//m_strname0
		 CString tmp1,tmp2,tmp3;
		 int lenmax=1;
		 tmp1=m_strname0;
		 if (tmp1.GetLength()>lenmax) lenmax=tmp1.GetLength();
		 tmp2=m_strname1;
		 if (tmp2.GetLength()>lenmax) lenmax=tmp2.GetLength();
		 tmp3=m_strname2;
		 if (tmp3.GetLength()>lenmax) lenmax=tmp3.GetLength();
		 int jj;
		 for (jj=tmp1.GetLength();jj<lenmax;jj++)
		 {
			 m_strname0[jj]=0x00;
		 }
		 for (jj=tmp2.GetLength();jj<lenmax;jj++)
		 {
			 m_strname1[jj]=0x00;
		 }
		 for (jj=tmp3.GetLength();jj<lenmax;jj++)
		 {
			 m_strname2[jj]=0x00;
		 }
		 m_strname0[lenmax]=0x00;
		 m_strname1[lenmax]=0x00;
		 m_strname2[lenmax]=0x00;
	}	

    CPropertyPage::OnOK();
}


void CYX_PROP_DIS::OnIdcButtonfont() 
{
	// TODO: Add your control notification handler code here
 	CPropertySheet sheet("选择字体");
 	CFontDlg dlg;
 	dlg.SetFontDlg(&m_lfont,false,false,0x000000);
 	sheet.AddPage(&dlg);
 	if (sheet.DoModal()==IDOK)
	{
		dlg.GetCurLogfont(&m_lfont);
	}
}


void CYX_PROP_DIS::HideAll()
{
	    m_editbmp2.ShowWindow(SW_HIDE);
	    m_editbmp1.ShowWindow(SW_HIDE);
	    m_editbmp0.ShowWindow(SW_HIDE);
		HWND hwnd;
		GetDlgItem(CZ_IDC_STATICTXTCOLOR0,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICTXTCOLOR1,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICTXTCOLOR2,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICVECCOLOR0,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICVECCOLOR1,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		GetDlgItem(CZ_IDC_STATICVECCOLOR2,&hwnd);
		CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
		m_staticvec2.ShowWindow(SW_HIDE);
		m_staticvec1.ShowWindow(SW_HIDE);
		m_staticvec0.ShowWindow(SW_HIDE);
		m_statictxtc2.ShowWindow(SW_HIDE);
		m_statictxtc1.ShowWindow(SW_HIDE);
		m_statictxtc0.ShowWindow(SW_HIDE);
		m_statictxt2.ShowWindow(SW_HIDE);
		m_statictxt1.ShowWindow(SW_HIDE);
		m_statictxt0.ShowWindow(SW_HIDE);
		m_staticbmp2.ShowWindow(SW_HIDE);
		m_staticbmp1.ShowWindow(SW_HIDE);
		m_staticbmp0.ShowWindow(SW_HIDE);
		m_editvec2.ShowWindow(SW_HIDE);
		m_editvec1.ShowWindow(SW_HIDE);
		m_editvec0.ShowWindow(SW_HIDE);
		m_edittxt2.ShowWindow(SW_HIDE);
		m_edittxt1.ShowWindow(SW_HIDE);
		m_edittxt0.ShowWindow(SW_HIDE);
		m_checkflash2.ShowWindow(SW_HIDE);
		m_checkflash1.ShowWindow(SW_HIDE);
		m_checkflash0.ShowWindow(SW_HIDE);
		m_checkfc2.ShowWindow(SW_HIDE);
		m_checkfc1.ShowWindow(SW_HIDE);
		m_checkfc0.ShowWindow(SW_HIDE);
		m_buttonvecs2.ShowWindow(SW_HIDE);
		m_buttonvecs1.ShowWindow(SW_HIDE);
		m_buttonvecs0.ShowWindow(SW_HIDE);
		m_buttonbmps2.ShowWindow(SW_HIDE);
		m_buttonbmps1.ShowWindow(SW_HIDE);
		m_buttonbmps0.ShowWindow(SW_HIDE);

}

void CYX_PROP_DIS::Show0Txt()
{
	ShowRect(true);
	Hide0();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR0,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_statictxtc0.ShowWindow(SW_SHOW);
	m_statictxt0.ShowWindow(SW_SHOW);
	m_edittxt0.ShowWindow(SW_SHOW);
	m_checkflash0.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show1Txt()
{
	ShowRect(true);
	Hide1();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR1,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_statictxtc1.ShowWindow(SW_SHOW);
	m_statictxt1.ShowWindow(SW_SHOW);
	m_edittxt1.ShowWindow(SW_SHOW);
	m_checkflash1.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show2Txt()
{
	ShowRect(true);
	Hide2();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR2,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_statictxtc2.ShowWindow(SW_SHOW);
	m_statictxt2.ShowWindow(SW_SHOW);
	m_edittxt2.ShowWindow(SW_SHOW);
	m_checkflash2.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show0Bmp()
{
	ShowRect(false);
	Hide0();
    m_editbmp0.ShowWindow(SW_SHOW);
	m_staticbmp0.ShowWindow(SW_SHOW);
	m_checkflash0.ShowWindow(SW_SHOW);
	m_buttonbmps0.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show1Bmp()
{
	ShowRect(false);
	Hide1();
    m_editbmp1.ShowWindow(SW_SHOW);
	m_staticbmp1.ShowWindow(SW_SHOW);
	m_checkflash1.ShowWindow(SW_SHOW);
	m_buttonbmps1.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show2Bmp()
{
	ShowRect(false);
	Hide2();
    m_editbmp2.ShowWindow(SW_SHOW);
	m_staticbmp2.ShowWindow(SW_SHOW);
	m_checkflash2.ShowWindow(SW_SHOW);
	m_buttonbmps2.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show0vec()
{
	ShowRect(false);
	Hide0();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICVECCOLOR0,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_staticvec0.ShowWindow(SW_SHOW);
	m_editvec0.ShowWindow(SW_SHOW);
	m_checkflash0.ShowWindow(SW_SHOW);
	m_checkfc0.ShowWindow(SW_SHOW);
	m_buttonvecs0.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show1vec()
{
	ShowRect(false);
	Hide1();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICVECCOLOR1,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_staticvec1.ShowWindow(SW_SHOW);
	m_editvec1.ShowWindow(SW_SHOW);
	m_checkflash1.ShowWindow(SW_SHOW);
	m_checkfc1.ShowWindow(SW_SHOW);
	m_buttonvecs1.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::Show2vec()
{
	ShowRect(false);
	Hide2();
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICVECCOLOR2,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW);
	m_staticvec2.ShowWindow(SW_SHOW);
	m_editvec2.ShowWindow(SW_SHOW);
	m_checkflash2.ShowWindow(SW_SHOW);
	m_checkfc2.ShowWindow(SW_SHOW);
	m_buttonvecs2.ShowWindow(SW_SHOW);
}

void CYX_PROP_DIS::OnIdcRadiotxt0() 
{
	// TODO: Add your control notification handler code here
	Show0Txt();
	Show1Txt();
	Show2Txt();
	UpdateData(true);
	m_radio0=0;
	m_radio1=0;
	m_radio2=0;
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiobmp0() 
{
	// TODO: Add your control notification handler code here
	Show0Bmp();
	UpdateData(true);
	if (m_radio1==0) {Show1Bmp();m_radio1=1;}
	if (m_radio2==0) {Show2Bmp();m_radio2=1;}
	UpdateData(false);
	
}

void CYX_PROP_DIS::OnIdcRadiovec0() 
{
	// TODO: Add your control notification handler code here
	Show0vec();
	UpdateData(true);
	if (m_radio1==0) {Show1vec();m_radio1=2;}
	if (m_radio2==0) {Show2vec();m_radio2=2;}
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiotxt1() 
{
	// TODO: Add your control notification handler code here
	Show0Txt();
	Show1Txt();
	Show2Txt();
	UpdateData(true);
	m_radio0=0;
	m_radio1=0;
	m_radio2=0;
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiobmp1() 
{
	// TODO: Add your control notification handler code here
	Show1Bmp();
	UpdateData(true);
	if (m_radio0==0) {Show0Bmp();m_radio0=1;}
	if (m_radio2==0) {Show2Bmp();m_radio2=1;}
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiovec1() 
{
	// TODO: Add your control notification handler code here
	Show1vec();
	UpdateData(true);
	if (m_radio0==0) {Show0vec();m_radio0=2;}
	if (m_radio2==0) {Show2vec();m_radio2=2;}
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiotxt2() 
{
	// TODO: Add your control notification handler code here
	Show0Txt();
	Show1Txt();
	Show2Txt();
	UpdateData(true);
	m_radio0=0;
	m_radio1=0;
	m_radio2=0;
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiobmp2() 
{
	// TODO: Add your control notification handler code here
	Show2Bmp();
	UpdateData(true);
	if (m_radio0==0) {Show0Bmp();m_radio0=1;}
	if (m_radio1==0) {Show1Bmp();m_radio1=1;}
	UpdateData(false);
}

void CYX_PROP_DIS::OnIdcRadiovec2() 
{
	// TODO: Add your control notification handler code here
	Show2vec();
	UpdateData(true);
	if (m_radio0==0) {Show0vec();m_radio0=2;}
	if (m_radio1==0) {Show1vec();m_radio1=2;}
	UpdateData(false);	
}

void CYX_PROP_DIS::Hide0()
{
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR0,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	GetDlgItem(CZ_IDC_STATICVECCOLOR0,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

	m_editbmp0.ShowWindow(SW_HIDE);
	m_staticvec0.ShowWindow(SW_HIDE);

	m_statictxtc0.ShowWindow(SW_HIDE);
	m_statictxt0.ShowWindow(SW_HIDE);
	m_staticbmp0.ShowWindow(SW_HIDE);
	m_editvec0.ShowWindow(SW_HIDE);
	m_edittxt0.ShowWindow(SW_HIDE);
	m_checkflash0.ShowWindow(SW_HIDE);
	m_checkfc0.ShowWindow(SW_HIDE);
	m_buttonvecs0.ShowWindow(SW_HIDE);
	m_buttonbmps0.ShowWindow(SW_HIDE);
}

void CYX_PROP_DIS::Hide1()
{
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR1,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	GetDlgItem(CZ_IDC_STATICVECCOLOR1,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

	m_editbmp1.ShowWindow(SW_HIDE);
	m_staticvec1.ShowWindow(SW_HIDE);
	m_statictxtc1.ShowWindow(SW_HIDE);
	m_statictxt1.ShowWindow(SW_HIDE);
	m_staticbmp1.ShowWindow(SW_HIDE);
	m_editvec1.ShowWindow(SW_HIDE);
	m_edittxt1.ShowWindow(SW_HIDE);
	m_checkflash1.ShowWindow(SW_HIDE);
	m_checkfc1.ShowWindow(SW_HIDE);
	m_buttonvecs1.ShowWindow(SW_HIDE);
	m_buttonbmps1.ShowWindow(SW_HIDE);
}

void CYX_PROP_DIS::Hide2()
{
	HWND hwnd;
	GetDlgItem(CZ_IDC_STATICTXTCOLOR2,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
	GetDlgItem(CZ_IDC_STATICVECCOLOR2,&hwnd);
	CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);

	m_editbmp2.ShowWindow(SW_HIDE);
	m_staticvec2.ShowWindow(SW_HIDE);
	m_statictxtc2.ShowWindow(SW_HIDE);
	m_statictxt2.ShowWindow(SW_HIDE);
	m_staticbmp2.ShowWindow(SW_HIDE);
	m_editvec2.ShowWindow(SW_HIDE);
	m_edittxt2.ShowWindow(SW_HIDE);
	m_checkflash2.ShowWindow(SW_HIDE);
	m_checkfc2.ShowWindow(SW_HIDE);
	m_buttonvecs2.ShowWindow(SW_HIDE);
	m_buttonbmps2.ShowWindow(SW_HIDE);
}

void CYX_PROP_DIS::ShowRect(BOOL txt)
{
	if (txt)
	{
		m_staticw.SetWindowText("字宽:");
		m_statich.SetWindowText("字高:");
		m_staticzj.ShowWindow(SW_SHOW);
		m_editcontrolzj.ShowWindow(SW_SHOW);
		m_buttonfont.ShowWindow(SW_SHOW);
	}
	else
	{
		m_staticw.SetWindowText("宽:");
		m_statich.SetWindowText("高:");
		m_staticzj.ShowWindow(SW_HIDE);
		m_editcontrolzj.ShowWindow(SW_HIDE);
		m_buttonfont.ShowWindow(SW_HIDE);
	}
}

void CYX_PROP_DIS::OnIdcButtonbmps0() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("选择位图"));
	CBmpSelectPage dlg;
	CString tmp;
	m_editbmp0.GetWindowText(tmp);
	dlg.SetContent(tmp,trans0,m_transcolor0);
    sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		CString tmp=dlg.GetContent(trans0,m_transcolor0);
		m_editbmp0.SetWindowText(tmp.Left(32));
	}
}

void CYX_PROP_DIS::OnIdcButtonbmps1() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("选择位图"));
	CBmpSelectPage dlg;
	CString tmp;
	m_editbmp1.GetWindowText(tmp);
	dlg.SetContent(tmp,trans1,m_transcolor1);
    sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		CString tmp=dlg.GetContent(trans1,m_transcolor1);
		m_editbmp1.SetWindowText(tmp.Left(32));
	}
	
}

void CYX_PROP_DIS::OnIdcButtonbmps2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("选择位图"));
	CBmpSelectPage dlg;
	CString tmp;
	m_editbmp2.GetWindowText(tmp);
	dlg.SetContent(tmp,trans2,m_transcolor2);
    sheet.AddPage((CPropertyPage*)&dlg);
	if (sheet.DoModal() == IDOK)
	{
		CString tmp=dlg.GetContent(trans2,m_transcolor2);
		m_editbmp2.SetWindowText(tmp.Left(32));
	}	
}

void CYX_PROP_DIS::OnIdcButtonvecs0() 
{
	// TODO: Add your control notification handler code here

	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;

	CString tmp;
	m_editvec0.GetWindowText(tmp);

	dlg.SetContent(tmp,trans0,m_veccolor0);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		dlg.GetContent(tmp,trans0,m_veccolor0);
		m_editvec0.SetWindowText(tmp.Left(32));
		UpdateData(FALSE);
	}
}

void CYX_PROP_DIS::OnIdcButtonvecs1() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;

	CString tmp;
	m_editvec1.GetWindowText(tmp);

	dlg.SetContent(tmp,trans1,m_veccolor1);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		dlg.GetContent(tmp,trans1,m_veccolor1);
		m_editvec1.SetWindowText(tmp.Left(32));
		UpdateData(FALSE);
	}
	
}

void CYX_PROP_DIS::OnIdcButtonvecs2() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet sheet( _T("对象..") );

	CSelectVectorDlg dlg;

	CString tmp;
	m_editvec2.GetWindowText(tmp);

	dlg.SetContent(tmp,trans2,m_veccolor2);
	sheet.AddPage( (CPropertyPage*)&dlg );

	if (sheet.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		dlg.GetContent(tmp,trans2,m_veccolor2);
		m_editvec2.SetWindowText(tmp.Left(32));
		UpdateData(FALSE);
	}	
}

void CYX_PROP_DIS::OnEnChangeIdcEdittxt0()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
