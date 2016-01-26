// FontDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dialog/FontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontDlg dialog


CFontDlg::CFontDlg()
	:CPropertyPage(CFontDlg::IDD)
{
	//{{AFX_DATA_INIT(CFontDlg)
	m_fontedit = _T("");
	m_styleedit = _T("");
	m_sizeedit = _T("");
	m_font = _T("");
	m_style = _T("");
	m_size = _T("");
	m_strikeout = FALSE;
	m_underline = FALSE;
	//}}AFX_DATA_INIT
	m_sizeenable=FALSE;
	p_logfont=&m_logfont;
}

void CFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontDlg)
	DDX_Control(pDX, IDC_STYLEEDIT, m_styleeditctr);
	DDX_Control(pDX, IDC_SIZEEDIT, m_sizeeditctr);
	DDX_Control(pDX, IDC_FONTEDIT, m_fonteditctr);
	DDX_Control(pDX, IDC_COLORCOMBO, m_colorcombo);
	DDX_Control(pDX, IDC_SIZELIST, m_sizelist);
	DDX_Control(pDX, IDC_STYLELIST, m_stylelist);
	DDX_Control(pDX, IDC_FONTLIST, m_fontlist);
	DDX_Text(pDX, IDC_FONTEDIT, m_fontedit);
	DDX_Text(pDX, IDC_STYLEEDIT, m_styleedit);
	DDX_Text(pDX, IDC_SIZEEDIT, m_sizeedit);
	DDX_LBString(pDX, IDC_FONTLIST, m_font);
	DDX_LBString(pDX, IDC_STYLELIST, m_style);
	DDX_LBString(pDX, IDC_SIZELIST, m_size);
	DDX_Check(pDX, IDC_STRIKEOUT, m_strikeout);
	DDX_Check(pDX, IDC_UNDERLINE, m_underline);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFontDlg)
	ON_LBN_SELCHANGE(IDC_FONTLIST, OnSelchangeFontlist)
	ON_LBN_SELCHANGE(IDC_SIZELIST, OnSelchangeSizelist)
	ON_LBN_SELCHANGE(IDC_STYLELIST, OnSelchangeStylelist)
	ON_BN_CLICKED(IDC_UNDERLINE, OnUnderline)
	ON_BN_CLICKED(IDC_STRIKEOUT, OnStrikeout)
	ON_CBN_SELCHANGE(IDC_COLORCOMBO, OnSelchangeColorcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontDlg message handlers

BOOL CFontDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//初始化字体列表
	m_fontlist.InsertString(0,"宋体");
	m_fontlist.InsertString(0,"仿宋_GB2312");
	m_fontlist.InsertString(0,"黑体");
	m_fontlist.InsertString(0,"楷体_GB2312");

	//初始化字体编辑框
	m_fontedit=p_logfont->lfFaceName;

	//初始化字体风格列表
	m_stylelist.InsertString(0,"粗斜体");
	m_stylelist.InsertString(0,"粗体");
	m_stylelist.InsertString(0,"斜体");
	m_stylelist.InsertString(0,"规则");
	
	//初始化字体风格编辑框
	if((p_logfont->lfItalic)&&(p_logfont->lfWeight == 700))
	{
		m_styleedit = "粗斜体";
	}
	else if(p_logfont->lfItalic)
	{
	    m_styleedit = "斜体";
	}
	else if(p_logfont->lfWeight == 700)
	{
		m_styleedit = "粗体";
	}
	else
	{
		m_styleedit = "规则";
	}

	//初始化字体尺寸列表
    m_sizelist.InsertString(0,"八号");
    m_sizelist.InsertString(0,"七号");
    m_sizelist.InsertString(0,"小六");
    m_sizelist.InsertString(0,"六号");
    m_sizelist.InsertString(0,"小五");
    m_sizelist.InsertString(0,"五号");
    m_sizelist.InsertString(0,"小四");
    m_sizelist.InsertString(0,"四号");
    m_sizelist.InsertString(0,"小三");
    m_sizelist.InsertString(0,"三号");
    m_sizelist.InsertString(0,"小二");
    m_sizelist.InsertString(0,"二号");
	m_sizelist.InsertString(0,"小一");
	m_sizelist.InsertString(0,"一号");
	m_sizelist.InsertString(0,"小初");
    m_sizelist.InsertString(0,"初号");
	m_sizelist.InsertString(0,"72");
	m_sizelist.InsertString(0,"48");
	m_sizelist.InsertString(0,"36");
	m_sizelist.InsertString(0,"28");
	m_sizelist.InsertString(0,"26");
	m_sizelist.InsertString(0,"24");
	m_sizelist.InsertString(0,"22");
	m_sizelist.InsertString(0,"20");
	m_sizelist.InsertString(0,"18");
	m_sizelist.InsertString(0,"16");
	m_sizelist.InsertString(0,"14");
	m_sizelist.InsertString(0,"12");
	m_sizelist.InsertString(0,"11");
	m_sizelist.InsertString(0,"10");
	m_sizelist.InsertString(0,"9");
	m_sizelist.InsertString(0,"8");

	//初始化字体尺寸
	if(p_logfont->lfHeight == 7)
	{
		m_sizeedit = "七号";
		m_sizelist.SetCurSel(29);
	}
	else if(p_logfont->lfHeight == 9)
	{
		m_sizeedit = "小六";
		m_sizelist.SetCurSel(28);
	}
	else if(p_logfont->lfHeight == 10)
	{
		m_sizeedit = "六号";
		m_sizelist.SetCurSel(27);
	}
	else if(p_logfont->lfHeight == 11)
	{
		m_sizeedit = "8";
		m_sizelist.SetCurSel(0);
	}
	else if(p_logfont->lfHeight == 12)
	{
		m_sizeedit = "小五";
		m_sizelist.SetCurSel(26);
	}
	else if(p_logfont->lfHeight == 13)
	{
		m_sizeedit = "10";
		m_sizelist.SetCurSel(2);
	}
	else if(p_logfont->lfHeight == 14)
	{
		m_sizeedit = "五号";
		m_sizelist.SetCurSel(25);
	}
	else if(p_logfont->lfHeight == 15)
	{
		m_sizeedit = "11";
		m_sizelist.SetCurSel(3);
	}
	else if(p_logfont->lfHeight == 16)
	{
		m_sizeedit = "小四";
		m_sizelist.SetCurSel(24);
	}
	else if(p_logfont->lfHeight == 19)
	{
		m_sizeedit = "四号";
		m_sizelist.SetCurSel(23);
	}
	else if(p_logfont->lfHeight == 20)
	{
		m_sizeedit="小三";
		m_sizelist.SetCurSel(22);
	}else
	if(p_logfont->lfHeight == 21)
	{
		m_sizeedit = "三号";
		m_sizelist.SetCurSel(21);
	}
	else if(p_logfont->lfHeight == 24)
	{
		m_sizeedit = "小二";
		m_sizelist.SetCurSel(20);
	}
	else if(p_logfont->lfHeight == 27)
	{
		m_sizeedit = "20";
		m_sizelist.SetCurSel(7);
	}
	else if(p_logfont->lfHeight == 29)
	{
		m_sizeedit="二号";
		m_sizelist.SetCurSel(19);
	}
	else if(p_logfont->lfHeight == 32)
	{
		m_sizeedit = "小一";
		m_sizelist.SetCurSel(18);
	}
	else if(p_logfont->lfHeight == 35)
	{
		m_sizeedit = "一号";
		m_sizelist.SetCurSel(17);
	}
	else if(p_logfont->lfHeight == 37)
	{
		m_sizeedit="28";
		m_sizelist.SetCurSel(11);
	}
	else if(p_logfont->lfHeight == 48)
	{
		m_sizeedit = "小初";
		m_sizelist.SetCurSel(16);
	}
	else if(p_logfont->lfHeight == 56)
	{
		m_sizeedit = "初号";
		m_sizelist.SetCurSel(15);
	}
	else if(p_logfont->lfHeight == 64)
	{
		m_sizeedit = "48";
		m_sizelist.SetCurSel(13);
	}
	else if(p_logfont->lfHeight == 96)
	{
		m_sizeedit = "72";
		m_sizelist.SetCurSel(14);
	}

	if(!m_sizeenable)
	{
		m_sizelist.EnableWindow(FALSE);
		m_sizeeditctr.EnableWindow(FALSE);
	}

	//初始化字体颜色列表
	m_colorcombo.InsertString(0,"白色");
	m_colorcombo.InsertString(0,"蓝绿色");
	m_colorcombo.InsertString(0,"紫红色");
	m_colorcombo.InsertString(0,"蓝色");
	m_colorcombo.InsertString(0,"黄色");
	m_colorcombo.InsertString(0,"浅绿色");
	m_colorcombo.InsertString(0,"红色");
	m_colorcombo.InsertString(0,"银色");
	m_colorcombo.InsertString(0,"灰色");
	m_colorcombo.InsertString(0,"深青色");
	m_colorcombo.InsertString(0,"紫色");
	m_colorcombo.InsertString(0,"深蓝色");
	m_colorcombo.InsertString(0,"黄褐色");
	m_colorcombo.InsertString(0,"绿色");
	m_colorcombo.InsertString(0,"褐红色");
	m_colorcombo.InsertString(0,"黑色");

	//初始化字体颜色
	if(!m_colorenable)
	{
		m_colorcombo.EnableWindow(FALSE);
	}
	else
	{
    	if(m_textcolor == RGB(0,0,0))            //黑色
		{
    		m_colorcombo.SetCurSel(0);
		}
		else if(m_textcolor == RGB(128,0,0))          //褐红色
		{
    		m_colorcombo.SetCurSel(1);
		}
		else if(m_textcolor == RGB(0,128,0))          //绿色
		{
		    m_colorcombo.SetCurSel(2);
		}
		else if(m_textcolor == RGB(128,128,0))        //黄褐色
		{
    		m_colorcombo.SetCurSel(3);
		}
		else if(m_textcolor == RGB(0,0,128))        //深蓝色
		{
    		m_colorcombo.SetCurSel(4);
		}
		else if(m_textcolor == RGB(128,0,128))        //紫色
		{
    		m_colorcombo.SetCurSel(5);
		}
		else if(m_textcolor == RGB(0,128,128))      //深青色
		{
    		m_colorcombo.SetCurSel(6);
		}
		else if(m_textcolor == RGB(128,128,128))    //灰色
		{
    		m_colorcombo.SetCurSel(7);
		}
		else if(m_textcolor == RGB(192,192,192))    //银色  
		{
    		m_colorcombo.SetCurSel(8);
		}
		else if(m_textcolor == RGB(255,0,0))         //红色
		{
    		m_colorcombo.SetCurSel(9);
		}
		else if(m_textcolor == RGB(0,255,0))         //浅绿色
		{
    		m_colorcombo.SetCurSel(10);
		}
		else if(m_textcolor == RGB(255,255,0))        //黄色
		{
    		m_colorcombo.SetCurSel(11);
		}
		else if(m_textcolor == RGB(0,0,255))       //蓝色
		{
    		m_colorcombo.SetCurSel(12);
		}
		else if(m_textcolor == RGB(255,0,255))         //紫红色
		{
    		m_colorcombo.SetCurSel(13);
		}
		else if(m_textcolor == RGB(0,255,255))       //蓝绿色
		{
    		m_colorcombo.SetCurSel(14);
		}
       else if(m_textcolor == RGB(255,255,255))       //白色
		{
    		m_colorcombo.SetCurSel(15);
		}
	}

	//初始化下划线
	m_underline=p_logfont->lfUnderline;

	//初始化删除线
	m_strikeout=p_logfont->lfStrikeOut;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFontDlg::OnSelchangeFontlist() 
{
	int index;
	index = m_fontlist.GetCurSel();
	m_fontlist.GetText(index,m_fontedit);
	strcpy(p_logfont->lfFaceName,m_fontedit);	
	UpdateData(FALSE);
}

void CFontDlg::OnSelchangeSizelist() 
{
	int index;
	index = m_sizelist.GetCurSel();
	m_sizelist.GetText(index,m_sizeedit);

	switch(index)
	{
	case 0:
		p_logfont->lfHeight = 11;
		break;

	case 1:
		p_logfont->lfHeight = 12;
		break;

	case 2:
		p_logfont->lfHeight = 13;
		break;

	case 3:
		p_logfont->lfHeight = 15;
		break;

	case 4:
		p_logfont->lfHeight = 16;
		break;

	case 5:
		p_logfont->lfHeight = 19;
		break;

	case 6:
		p_logfont->lfHeight = 24;
		break;

	case 7:
		p_logfont->lfHeight = 27;
		break;

	case 8:
		p_logfont->lfHeight = 29;
		break;

	case 9:
		p_logfont->lfHeight=32;
		break;

	case 10:
		p_logfont->lfHeight=35;
		break;

	case 11:
		p_logfont->lfHeight=37;
		break;

	case 12:
		p_logfont->lfHeight=48;
		break;

	case 13:
		p_logfont->lfHeight=64;
		break;

	case 14:
		p_logfont->lfHeight=96;
		break;

	case 15:
		p_logfont->lfHeight=56;
		break;

	case 16:
		p_logfont->lfHeight=48;
		break;

	case 17:
		p_logfont->lfHeight=35;
		break;

	case 18:
		p_logfont->lfHeight=32;
		break;

	case 19:
		p_logfont->lfHeight=29;
		break;

	case 20:
		p_logfont->lfHeight=24;
		break;

	case 21:
		p_logfont->lfHeight=21;
		break;

	case 22:
		p_logfont->lfHeight=20;
		break;

	case 23:
		p_logfont->lfHeight=19;
		break;

	case 24:
		p_logfont->lfHeight=16;
		break;

	case 25:
		p_logfont->lfHeight=14;
		break;

	case 26:
		p_logfont->lfHeight=12;
		break;

	case 27:
		p_logfont->lfHeight=10;
		break;

	case 28:
		p_logfont->lfHeight=9;
		break;

	case 29:
		p_logfont->lfHeight=7;
		break;

	case 30:
		p_logfont->lfHeight=7;
		break;
	}

	UpdateData(FALSE);
}

void CFontDlg::OnSelchangeStylelist() 
{
	int index;
	index = m_stylelist.GetCurSel();
	m_stylelist.GetText(index,m_styleedit);

	if(m_styleedit == "粗斜体")
	{
    	p_logfont->lfItalic = 1;
		p_logfont->lfWeight = 700;
	}
	else if(m_styleedit == "粗体")
	{
		p_logfont->lfWeight = 700;
	}
	else if(m_styleedit == "斜体")
	{
		p_logfont->lfItalic = 1;
	}
	else
	{
		p_logfont->lfItalic = 0;
		p_logfont->lfWeight = 0;
	}

	UpdateData(FALSE);
}

void CFontDlg::SetFontDlg(LOGFONT *plogfont, BOOL sizeenable,BOOL colorenable,COLORREF textcolor)
{
	ASSERT(plogfont != NULL);

	memcpy(&m_logfont,plogfont,sizeof(LOGFONT));
	m_sizeenable = sizeenable;
	m_colorenable = colorenable;
	m_textcolor = textcolor;
}

void CFontDlg::GetCurLogfont(LOGFONT *logfont)
{
	ASSERT(logfont != NULL);

	memcpy(logfont,&m_logfont,sizeof(LOGFONT));
}

COLORREF CFontDlg::GetTextColor()
{
	return m_textcolor;
}

void CFontDlg::OnUnderline() 
{	
	m_underline = !m_underline;
	p_logfont->lfUnderline = m_underline;

	UpdateData(FALSE);
}

void CFontDlg::OnStrikeout() 
{	
	m_strikeout = !m_strikeout;
	p_logfont->lfStrikeOut = m_strikeout;

	UpdateData(FALSE);
}

void CFontDlg::OnSelchangeColorcombo() 
{	
	int index;
	index = m_colorcombo.GetCurSel();

	switch(index)
	{
	case 0:
		m_textcolor=RGB(0,0,0);
		break;
    case 1:
		m_textcolor=RGB(128,0,0);
		break;
	case 2:
		m_textcolor=RGB(0,128,0);
		break;
	case 3:
		m_textcolor=RGB(128,128,0);
		break;
	case 4:
		m_textcolor=RGB(0,0,128);
		break;
	case 5:
		m_textcolor=RGB(128,0,128);
		break;
	case 6:
		m_textcolor=RGB(0,128,128);
		break;
	case 7:
		m_textcolor=RGB(128,128,128);
		break;
	case 8:
		m_textcolor=RGB(192,192,192);
		break;
	case 9:
		m_textcolor=RGB(255,0,0);
		break;
	case 10:
		m_textcolor=RGB(0,255,0);
		break;
	case 11:
		m_textcolor=RGB(255,255,0);
		break;
	case 12:
		m_textcolor=RGB(0,0,255);
		break;
	case 13:
		m_textcolor=RGB(255,0,255);
		break;
	case 14:
		m_textcolor=RGB(0,255,255);
		break;
	case 15:
		m_textcolor=RGB(255,255,255);
		break;
	}
}
