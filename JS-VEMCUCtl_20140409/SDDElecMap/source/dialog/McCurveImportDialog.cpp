// McCurveImportDialog.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/McCurveImportDialog.h"
#include <io.h>

// CMcCurveImportDialog 对话框

IMPLEMENT_DYNAMIC(CMcCurveImportDialog, CDialog)
CMcCurveImportDialog::CMcCurveImportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMcCurveImportDialog::IDD, pParent)
	, m_strSelsection(_T(""))
	, m_strinlist(_T(""))
{
}

CMcCurveImportDialog::~CMcCurveImportDialog()
{
}

void CMcCurveImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Sectionlist);
	DDX_Text(pDX, IDC_EDIT1, m_strSelsection);
	DDX_LBString(pDX, IDC_LIST1, m_strinlist);
}


BEGIN_MESSAGE_MAP(CMcCurveImportDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
END_MESSAGE_MAP()


// CMcCurveImportDialog 消息处理程序

BOOL CMcCurveImportDialog::OnInitDialog()
{

	CDialog::OnInitDialog();
	CStdioFile stdiofile(strfilepath,CFile::modeRead|CFile::shareDenyNone);
	CString strline;
	while (1)
	{
		if (stdiofile.ReadString(strline))
		{
			if (strline.Left(1)=="["
				&& strline.Right(1)=="]")
			{
				strline.TrimLeft("[");
				strline.TrimRight("]");
				char strbuf[30];
				GetPrivateProfileSection(strline,strbuf,30,strfilepath);
				if (strlen(strbuf)!=0)
				{
					m_Sectionlist.AddString(strline);
				}
			}
		}
		else
		{
			break;
		}
	}
	stdiofile.Close();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CMcCurveImportDialog::OnLbnSelchangeList1()
{
	UpdateData();
	//m_strSelsection=m_strinlist;
	CEdit * pEdit=(CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText(m_strinlist);
	
}
LPCTSTR CMcCurveImportDialog::GetSelSection()
{
	return m_strSelsection;
}

void CMcCurveImportDialog::OnLbnDblclkList1()
{
	CDialog::OnOK();
}
