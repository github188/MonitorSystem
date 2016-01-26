// SZ_SgbwMainInfoDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/SZ_SgbwMainInfoDlg.h"
#include "dialog/SZ_SgbwLineInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwMainInfoDlg dialog


CSZ_SgbwMainInfoDlg::CSZ_SgbwMainInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSZ_SgbwMainInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSZ_SgbwMainInfoDlg)
	m_sgbwname = _T("");
	m_rtuname = _T("");
	m_imax = 0.0f;
	m_imin = 0.0f;
	m_umax = 0.0f;
	m_umin = 0.0f;
	//}}AFX_DATA_INIT
}


void CSZ_SgbwMainInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSZ_SgbwMainInfoDlg)
	DDX_Control(pDX, IDC_SZ_IMINE, m_imin_c);
	DDX_Control(pDX, IDC_SZ_IMAXE, m_imax_c);
	DDX_Control(pDX, IDC_SZ_ZBGT, m_zbgt);
	DDX_Control(pDX, IDC_SZ_LINELIST, m_grid);
	DDX_Control(pDX, IDC_SZ_UMIN, m_umin_s);
	DDX_Control(pDX, IDC_SZ_IMIN, m_imin_s);
	DDX_Control(pDX, IDC_SZ_IMAX, m_imax_s);
	DDX_Control(pDX, IDC_SZ_UMAX, m_umax_s);
	DDX_Control(pDX, IDC_SZ_UANDI, m_uandi);
	DDX_Control(pDX, IDC_SZ_I, m_i);
	DDX_Control(pDX, IDC_SZ_U, m_u);
	DDX_Text(pDX, IDC_SZ_SGBW, m_sgbwname);
	DDX_Text(pDX, IDC_SZ_RTUNAME, m_rtuname);
	DDX_Text(pDX, IDC_SZ_IMAXE, m_imax);
	DDV_MinMaxFloat(pDX, m_imax, 0.f, 1.e+007f);
	DDX_Text(pDX, IDC_SZ_IMINE, m_imin);
	DDV_MinMaxFloat(pDX, m_imin, 0.f, 1.e+007f);
	DDX_Text(pDX, IDC_SZ_UMAXE, m_umax);
	DDV_MinMaxFloat(pDX, m_umax, 0.f, 1.e+007f);
	DDX_Text(pDX, IDC_SZ_UMINE, m_umin);
	DDV_MinMaxFloat(pDX, m_umin, 0.f, 1.e+007f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSZ_SgbwMainInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSZ_SgbwMainInfoDlg)
	ON_BN_CLICKED(IDC_SZ_BWADD, OnSzBwadd)
	ON_BN_CLICKED(IDC_SZ_BWDEL, OnSzBwdel)
	ON_BN_CLICKED(IDC_SZ_BWEDIT, OnSzBwedit)
	ON_BN_CLICKED(IDC_SZ_BNRTU, OnSzBnrtu)
	ON_BN_CLICKED(IDC_SZ_UANDI, OnSzUandi)
	ON_BN_CLICKED(IDC_SZ_ZBGT, OnSzZbgt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSZ_SgbwMainInfoDlg message handlers

void CSZ_SgbwMainInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;
	if (m_rtuname.IsEmpty())
	{
		AfxMessageBox("请输入单元名称!");
		return ;
	}

	strcpy(m_sMainInfo.RtuName,m_rtuname);
	m_sMainInfo.MaxU=m_umax;
	m_sMainInfo.MinU=m_umin;
	
	if (m_zbgt.GetCheck()==1)
	{
		m_sMainInfo.Zbgt=true;
		m_sMainInfo.HaveUandI=true;
		m_sMainInfo.MaxI=m_imax;
		m_sMainInfo.MinI=m_imin; 
	}
	else
	{
		m_sMainInfo.Zbgt=false;
		if (m_uandi.GetCheck()==1)
		{
			m_sMainInfo.HaveUandI=true;
			m_sMainInfo.MaxI=m_imax;
			m_sMainInfo.MinI=m_imin; 
		}
		else
		{
			m_sMainInfo.HaveUandI=false;
			m_sMainInfo.MaxI=m_umax;
			m_sMainInfo.MinI=m_umin; 
		}
	}
	
	CheckData();
	
	CDialog::OnOK();
}

void CSZ_SgbwMainInfoDlg::OnSzBwadd() 
{
	// TODO: Add your control notification handler code here
	if (m_nLineCount>=SZ_SGBW_LINENUM) 
	{
		AfxMessageBox("事故报文曲线条数已经达到系统最大数目!");
		return ;
	}

	SZ_S_SGBWLINEBASE LineInfo;
	strcpy(LineInfo.DotName,"");
	LineInfo.Color=RGB(0,0,0);
	LineInfo.Enum=0;
	LineInfo.Prop=0.00;
	LineInfo.TdNo=0;
	LineInfo.UorI=1;
	LineInfo.Zbgt=1;
	
	bool bZbgt,bUandI;

	if (m_zbgt.GetCheck()==1) 
	{
		bZbgt=true;
		bUandI=true;
	}
	else
	{
		bZbgt=false;
		if (m_uandi.GetCheck()==1) bUandI=true;
		else bUandI=false;
	}

	CSZ_SgbwLineInfoDlg Dlg;
	Dlg.SetInfo(LineInfo.DotName,LineInfo.TdNo,LineInfo.Prop,LineInfo.Enum,LineInfo.Zbgt,
		LineInfo.UorI,LineInfo.Color,bZbgt,bUandI);

	if (Dlg.DoModal()==IDOK)
	{
		char name[17];
		int TdNo,Enum;
		float Prop;
		BYTE Zbgt,UorI;
		COLORREF Color;

		Dlg.GetInfo(name,&TdNo,&Prop,&Enum,&Zbgt,&UorI,&Color);

		strcpy(LineInfo.DotName,name);
		LineInfo.TdNo=TdNo;
		LineInfo.Prop=Prop;
		LineInfo.Enum=Enum;
		LineInfo.Zbgt=Zbgt;
		LineInfo.UorI=UorI;
		LineInfo.Color=Color;
		
		AddLine(LineInfo);
		
		SetColor();
		WriteGridData();
	}
}

void CSZ_SgbwMainInfoDlg::OnSzBwdel() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_grid.GetFirstSelectedItemPosition();
	if (pos==NULL) return ;
	int Index=m_grid.GetNextSelectedItem(pos);

	if ((Index<0)||(Index>m_nLineCount-1)) 
	{
		AfxMessageBox("请选择一条要删除的曲线!");
		return ;
	}

	DelLine(Index);
	SetColor();
	WriteGridData();
}

void CSZ_SgbwMainInfoDlg::OnSzBwedit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_grid.GetFirstSelectedItemPosition();
	if (pos==NULL) return ;
	int Index=m_grid.GetNextSelectedItem(pos);

	if ((Index<0)||(Index>m_nLineCount-1)) 
	{
		AfxMessageBox("请选择一条要编辑的曲线!");
		return ;
	}
	
	SZ_S_SGBWLINEBASE LineInfo=m_sLineInfo[Index];
	
	bool bZbgt,bUandI;

	if (m_zbgt.GetCheck()==1) 
	{
		bZbgt=true;
		bUandI=true;
	}
	else
	{
		bZbgt=false;
		if (m_uandi.GetCheck()==1) bUandI=true;
		else bUandI=false;
	}

	CSZ_SgbwLineInfoDlg Dlg;
	Dlg.SetInfo(LineInfo.DotName,LineInfo.TdNo,LineInfo.Prop,LineInfo.Enum,LineInfo.Zbgt,
		LineInfo.UorI,LineInfo.Color,bZbgt,bUandI);

	if (Dlg.DoModal()==IDOK)
	{
		char name[17];
		int TdNo,Enum;
		float Prop;
		BYTE Zbgt,UorI;
		COLORREF Color;

		Dlg.GetInfo(name,&TdNo,&Prop,&Enum,&Zbgt,&UorI,&Color);

		strcpy(LineInfo.DotName,name);
		LineInfo.TdNo=TdNo;
		LineInfo.Prop=Prop;
		LineInfo.Enum=Enum;
		LineInfo.Zbgt=Zbgt;
		LineInfo.UorI=UorI;
		LineInfo.Color=Color;
		
		EditLine(Index,LineInfo);

		SetColor();
		WriteGridData();
	}
}

void CSZ_SgbwMainInfoDlg::OnSzBnrtu() 
{
	UpdateData(TRUE);

}

void CSZ_SgbwMainInfoDlg::OnSzUandi() 
{
	ControlUandI();
}

void CSZ_SgbwMainInfoDlg::OnSzZbgt() 
{
	if (m_zbgt.GetCheck()==1) 
	{
		m_uandi.EnableWindow(false);
		if (m_sMainInfo.HaveUandI) m_uandi.SetCheck(1);
		else m_uandi.SetCheck(0);
	}
	else 
		m_uandi.EnableWindow(true);
	ControlUandI();
}

void CSZ_SgbwMainInfoDlg::SetInfo(SZ_S_SGBWMAININFO MainInfo, SZ_S_SGBWLINEBASE *LineInfo, int LineCount,char* name)
{
	m_sMainInfo=MainInfo;
	m_nLineCount=LineCount;
	int i;
	for(i=0;i<LineCount;i++)
		m_sLineInfo[i]=LineInfo[i];
	m_sgbwname=name;
}

void CSZ_SgbwMainInfoDlg::GetInfo(SZ_S_SGBWMAININFO *MainInfo, SZ_S_SGBWLINEBASE *LineInfo, int *LineCount,char* name)
{
	*MainInfo=m_sMainInfo;
	*LineCount=m_nLineCount;
	int i;
	for(i=0;i<m_nLineCount;i++)
		LineInfo[i]=m_sLineInfo[i];
	strcpy(name,m_sgbwname);
}

void CSZ_SgbwMainInfoDlg::WriteGridTitle()
{
	LV_COLUMN lvcol;

	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText="编号";
	lvcol.iSubItem=0;
	lvcol.cx=40;
	m_grid.InsertColumn(0,&lvcol);

	lvcol.pszText="颜色";
	lvcol.iSubItem=1;
	lvcol.cx=40;
	m_grid.InsertColumn(1,&lvcol);

	lvcol.pszText="英文名";
	lvcol.iSubItem=2;
	lvcol.cx=150;
	m_grid.InsertColumn(2,&lvcol);

	lvcol.pszText="通道号";
	lvcol.iSubItem=3;
	lvcol.cx=70;
	m_grid.InsertColumn(3,&lvcol);

	lvcol.pszText="比例";
	lvcol.iSubItem=4;
	lvcol.cx=70;
	m_grid.InsertColumn(4,&lvcol);

	lvcol.pszText="小数位数";
	lvcol.iSubItem=5;
	lvcol.cx=70;
	m_grid.InsertColumn(5,&lvcol);

	lvcol.pszText="自闭贯通";
	lvcol.iSubItem=6;
	lvcol.cx=70;
	m_grid.InsertColumn(6,&lvcol);

	lvcol.pszText="电流电压";
	lvcol.iSubItem=7;
	lvcol.cx=70;
	m_grid.InsertColumn(6,&lvcol);

	m_grid.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
}

void CSZ_SgbwMainInfoDlg::WriteGridData()
{
	int i;

	char str[64];
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;

	m_grid.DeleteAllItems(); 
					
	if (m_nLineCount<=0)
	{
		//无数据
		lvitem.iItem=0;
		lvitem.iSubItem=0;
		lvitem.pszText="";
		m_grid.InsertItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=1;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=2;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=3;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=4;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);
	
		lvitem.iItem=0;
		lvitem.iSubItem=5;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=6;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);

		lvitem.iItem=0;
		lvitem.iSubItem=7;
		lvitem.pszText="";
		m_grid.SetItem(&lvitem);
	}
	else
	{
		//有数据
		for(i=0;i<m_nLineCount;i++)
		{
			sprintf(str,"%d",i);
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.pszText=str;
			m_grid.InsertItem(&lvitem);

			lvitem.iItem=i;
			lvitem.iSubItem=1;
			lvitem.pszText="";
			m_grid.SetItem(&lvitem);

			lvitem.iItem=i;
			lvitem.iSubItem=2;
			lvitem.pszText=m_sLineInfo[i].DotName;
			m_grid.SetItem(&lvitem);

			sprintf(str,"%d",m_sLineInfo[i].TdNo);
			lvitem.iItem=i;
			lvitem.iSubItem=3;
			lvitem.pszText=str;
			m_grid.SetItem(&lvitem);

			sprintf(str,"%.3f",m_sLineInfo[i].Prop);
			lvitem.iItem=i;
			lvitem.iSubItem=4;
			lvitem.pszText=str;
			m_grid.SetItem(&lvitem);

			sprintf(str,"%d",m_sLineInfo[i].Enum);
			lvitem.iItem=i;
			lvitem.iSubItem=5;
			lvitem.pszText=str;
			m_grid.SetItem(&lvitem);

			if (m_zbgt.GetCheck()==1)
			{
				if (m_sLineInfo[i].Zbgt==1)
				{
					lvitem.iItem=i;
					lvitem.iSubItem=6;
					lvitem.pszText="自闭";
					m_grid.SetItem(&lvitem);
				}
				else
				{
					lvitem.iItem=i;
					lvitem.iSubItem=6;
					lvitem.pszText="贯通";
					m_grid.SetItem(&lvitem);
				}
			}
			else
			{
				lvitem.iItem=i;
				lvitem.iSubItem=6;
				lvitem.pszText="";
				m_grid.SetItem(&lvitem);
			}

			if (m_uandi.GetCheck()==1)
			{
				if (m_sLineInfo[i].UorI==1)
				{
					lvitem.iItem=i;
					lvitem.iSubItem=7;
					lvitem.pszText="电压";
					m_grid.SetItem(&lvitem);
				}
				else
				{
					lvitem.iItem=i;
					lvitem.iSubItem=7;
					lvitem.pszText="电流";
					m_grid.SetItem(&lvitem);
				}
			}
			else
			{
				lvitem.iItem=i;
				lvitem.iSubItem=7;
				lvitem.pszText="";
				m_grid.SetItem(&lvitem);
			}
		}
	}
}

void CSZ_SgbwMainInfoDlg::AddLine(SZ_S_SGBWLINEBASE LineInfo)
{
	m_sLineInfo[m_nLineCount++]=LineInfo;
}

void CSZ_SgbwMainInfoDlg::EditLine(int LineNo, SZ_S_SGBWLINEBASE LineInfo)
{
	m_sLineInfo[LineNo]=LineInfo;
}

void CSZ_SgbwMainInfoDlg::DelLine(int LineNo)
{
	int i;
	for(i=0;i<m_nLineCount-1;i++)
		m_sLineInfo[i]=m_sLineInfo[i+1];
	m_nLineCount--;
}

void CSZ_SgbwMainInfoDlg::SetColor()
{
	COLORREF ColorArray[SZ_SGBW_LINENUM];
	int i;
	for(i=0;i<m_nLineCount;i++)
		ColorArray[i]=m_sLineInfo[i].Color;
	
	m_grid.SetColor(ColorArray,m_nLineCount);
}

void CSZ_SgbwMainInfoDlg::CheckData()
{
	int i;
	for(i=0;i<m_nLineCount;i++)
	{
		if (m_zbgt.GetCheck()!=1)
			m_sLineInfo[i].Zbgt=1;

		if (m_uandi.GetCheck()!=1)
			m_sLineInfo[i].UorI=1;
	}
}

void CSZ_SgbwMainInfoDlg::ControlUandI()
{
	if (m_uandi.GetCheck()==1)
	{
		m_u.SetWindowText("电压");
		m_i.ShowWindow(SW_SHOW);
		m_imax_s.ShowWindow(SW_SHOW);
		m_imin_s.ShowWindow(SW_SHOW);
		m_imax_c.ShowWindow(SW_SHOW);
		m_imin_c.ShowWindow(SW_SHOW);
	}
	else
	{
		m_u.SetWindowText("");
		m_i.ShowWindow(SW_HIDE);
		m_imax_s.ShowWindow(SW_HIDE);
		m_imin_s.ShowWindow(SW_HIDE);
		m_imax_c.ShowWindow(SW_HIDE);
		m_imin_c.ShowWindow(SW_HIDE);
	}
}

BOOL CSZ_SgbwMainInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_rtuname=m_sMainInfo.RtuName;
	m_umax=m_sMainInfo.MaxU;
	m_umin=m_sMainInfo.MinU;
	m_imax=m_sMainInfo.MaxI;
	m_imin=m_sMainInfo.MinI;
	
	if (m_sMainInfo.Zbgt) 
	{
		m_zbgt.SetCheck(1);
		m_uandi.SetCheck(1);
		m_uandi.EnableWindow(false);
	}
	else 
	{
		m_zbgt.SetCheck(0);
		if (m_sMainInfo.HaveUandI) m_uandi.SetCheck(1);
		else m_uandi.SetCheck(0);
		m_uandi.EnableWindow(true);
	}

	ControlUandI();
	
	WriteGridTitle();
	
	UpdateData(false);
	SetColor();
	
	WriteGridData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
