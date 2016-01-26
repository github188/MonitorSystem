// PropertyFilePage.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/PropertyFilePage.h"


// CPropertyFilePage 对话框

IMPLEMENT_DYNAMIC(CPropertyFilePage, CPropertyPage)
CPropertyFilePage::CPropertyFilePage()
	: CPropertyPage(CPropertyFilePage::IDD)
	, m_filename(_T(""))
	, m_filespecification(_T(""))
{
}

CPropertyFilePage::~CPropertyFilePage()
{
}

void CPropertyFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
	//DDX_Control(pDX, IDC_EDIT2, m_filename);
	//DDX_Control(pDX, IDC_EDIT1, m_filespecificaton);
	DDX_Text(pDX, IDC_EDIT2, m_filename);
	DDX_Text(pDX, IDC_EDIT1, m_filespecification);
}


BEGIN_MESSAGE_MAP(CPropertyFilePage, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()

void CPropertyFilePage::FillListBox(void)
{
	
	 CFileFind  filedir;
	 CString file;//=theApp.GetDirectory();
	 CString filename;
     file="C:\\sunpac\\par\\global\\FileLink\\*.*";
	 //file.Format("%s%s\\*.%s",theApp.GetDirectory(),m_path,m_ext);

	 m_listbox.ResetContent();
	 if (filedir.FindFile(file))
	 {
	   while (filedir.FindNextFile())
		{
			 filename=filedir.GetFileName();
			 m_listbox.AddString(filename);
		}
	  //filename=filedir.GetFileTitle();
	  filename =filedir.GetFileName ();
	  m_listbox.AddString(filename);
		filedir.Close();
	}
}

// CPropertyFilePage 消息处理程序

void CPropertyFilePage::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos=m_listbox.GetCurSel();
	CString filename;
	if (pos>=0)
	{
		m_listbox.GetText(pos,filename);
		m_filename=filename;
	}
	UpdateData(FALSE);
}

void CPropertyFilePage::Getfile(char* filename,char* ename)
{
	ASSERT(filename!=NULL);
	ASSERT(ename!=NULL);
	if (filename==NULL || ename==NULL) return;
	filename[0]=0x00;
	ename[0]=0x00;
	if (m_filename.IsEmpty()) return;
	strcpy(filename,m_filename);
	strcpy (ename ,m_filespecification );

}

BOOL CPropertyFilePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

    // TODO:  在此添加额外的初始化
  FillListBox ();
   UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}



void CPropertyFilePage::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData (TRUE);

	// TODO:  在此添加控件通知处理程序代码
}
void CPropertyFilePage::Setfile(char *filename,char*ename)
{
   
	ASSERT(filename !=NULL);
	ASSERT(ename!=NULL);
	 if(filename ==NULL ||ename ==NULL)
		 return ;
  //if(m_filename =="" || m_filespecification =="")
  //  return ;
  m_filename =filename;
  m_filespecification =ename;
 //  if(m_filename =="" || m_filespecification =="")
 //	  return ;

   //UpdateData(FALSE);


}





void CPropertyFilePage::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
    UpdateData (TRUE);
	// TODO:  在此添加控件通知处理程序代码
}
