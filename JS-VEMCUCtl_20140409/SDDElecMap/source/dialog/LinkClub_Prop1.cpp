// LinkClub_Prop1.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/LinkClub_Prop1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop1 property page

IMPLEMENT_DYNCREATE(CLinkClub_Prop1, CPropertyPage)

CLinkClub_Prop1::CLinkClub_Prop1() : CPropertyPage(CLinkClub_Prop1::IDD)
{
	//{{AFX_DATA_INIT(CLinkClub_Prop1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLinkClub_Prop1::~CLinkClub_Prop1()
{
}

void CLinkClub_Prop1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkClub_Prop1)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkClub_Prop1, CPropertyPage)
	//{{AFX_MSG_MAP(CLinkClub_Prop1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkClub_Prop1 message handlers

BOOL CLinkClub_Prop1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	sprintf(myitems[0].hint,"棒图名称");
	sprintf(myitems[0].value,myname);
	myitems[0].type=PIT_EDIT;
	sprintf(myitems[0].info,"32");
	sprintf(myitems[1].hint,"标题");
	sprintf(myitems[1].value,myclub.title);
	myitems[1].type=PIT_EDIT;
	sprintf(myitems[1].info,"32");
	sprintf(myitems[2].hint,"标题字体/颜色");
	myitems[2].font=myclub.titlefont;
	myitems[2].color=myclub.titlecolor;
	myitems[2].type=PIT_FONT;
	sprintf(myitems[2].info,"");
	sprintf(myitems[3].hint,"Y轴坐标最大值");
	sprintf(myitems[3].value,"%f",myclub.ymax);
	myitems[3].type=PIT_EDIT;
	sprintf(myitems[3].info,"10");
	sprintf(myitems[4].hint,"Y轴坐标最小值");
	sprintf(myitems[4].value,"%f",myclub.ymin);
	myitems[4].type=PIT_EDIT;
	sprintf(myitems[4].info,"10");
	sprintf(myitems[5].hint,"Y轴坐标单位");
	sprintf(myitems[5].value,myclub.yunit);
	myitems[5].type=PIT_EDIT;
	sprintf(myitems[5].info,"8");
	sprintf(myitems[6].hint,"Y轴坐标单位字体/颜色");
	myitems[6].font=myclub.yunitfont;
	myitems[6].color=myclub.yunitcolor;
	myitems[6].type=PIT_FONT;
	sprintf(myitems[6].info,"");
	sprintf(myitems[7].hint,"坐标系字体/颜色");
	myitems[7].font=myclub.coordinatefont;
	myitems[7].color=myclub.coordinatecolor;
	myitems[7].type=PIT_FONT;
	sprintf(myitems[7].info,"");
	sprintf(myitems[8].hint,"背景颜色");
	myitems[8].color=myclub.backcolor;
	myitems[8].type=PIT_COLOR;
	sprintf(myitems[8].info,"");
	sprintf(myitems[9].hint,"统一越上限越下限的限值");
	if (myclub.allmaxmin==0) sprintf(myitems[9].value,"No");
	if (myclub.allmaxmin==1) sprintf(myitems[9].value,"Yes");
	myitems[9].type=PIT_COMBO;
	sprintf(myitems[9].info,"Yes|No|");
	sprintf(myitems[10].hint,"    统一的越上限值");
	sprintf(myitems[10].value,"%f",myclub.maxvalue);
	myitems[10].type=PIT_EDIT;
	sprintf(myitems[10].info,"8");
	sprintf(myitems[11].hint,"    统一的越下限值");
	sprintf(myitems[11].value,"%f",myclub.minvalue);
	myitems[11].type=PIT_EDIT;
	sprintf(myitems[11].info,"8");
	sprintf(myitems[12].hint,"棒宽度");
	sprintf(myitems[12].value,"%d",myclub.clubwidth);
	myitems[12].type=PIT_EDIT;
	sprintf(myitems[12].info,"8");
	sprintf(myitems[13].hint,"棒间距");
	sprintf(myitems[13].value,"%d",myclub.clubspace);
	myitems[13].type=PIT_EDIT;
	sprintf(myitems[13].info,"8");
	
	for (int i=0;i<14;i++) m_list.AddPropItem(&myitems[i]);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinkClub_Prop1::SetPara(char *name,STRUCT_CLUB club)
{
	memcpy(myname,name,33);
	myclub=club;
}

void CLinkClub_Prop1::GetPara(char *name,STRUCT_CLUB * club)
{
	memcpy(name,myname,33);
	*club=myclub;
}

void CLinkClub_Prop1::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tmp=myitems[0].value;
	sprintf(myname,"%s",tmp.Left(32));

	sprintf(myclub.title,"%s",myitems[1].value);
	myclub.titlefont=myitems[2].font;
	myclub.titlecolor=myitems[2].color;
	myclub.ymax=(float)atof(myitems[3].value);
	myclub.ymin=(float)atof(myitems[4].value);
	sprintf(myclub.yunit,"%s",myitems[5].value);
	myclub.yunitfont=myitems[6].font;
	myclub.yunitcolor=myitems[6].color;
	myclub.coordinatefont=myitems[7].font;
	myclub.coordinatecolor=myitems[7].color;
	myclub.backcolor=myitems[8].color;
	tmp=myitems[9].value;
	if (strcmp(tmp,"No")==0) 
	{
		myclub.allmaxmin=0;
	}
	else
	{
		myclub.allmaxmin=1;
	}
	myclub.maxvalue=(float)atof(myitems[10].value);
	myclub.minvalue=(float)atof(myitems[11].value);
	int ii=(int)atof(myitems[12].value);
	if (ii<0) ii=0;
	if (ii>=25) myclub.clubwidth=ii;
	ii=(int)atof(myitems[13].value);
	if (ii<0) ii=0;
	myclub.clubspace=ii;

	CPropertyPage::OnOK();
}
