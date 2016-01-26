// CZWavSelectPage.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "dialog/CZWavSelectPage.h"
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCZWavSelectPage property page

IMPLEMENT_DYNCREATE(CCZWavSelectPage, CPropertyPage)

CCZWavSelectPage::CCZWavSelectPage() : CPropertyPage(CCZWavSelectPage::IDD)
{
	//{{AFX_DATA_INIT(CCZWavSelectPage)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}

CCZWavSelectPage::~CCZWavSelectPage()
{
}

void CCZWavSelectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCZWavSelectPage)
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDV_MaxChars(pDX, m_edit, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCZWavSelectPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCZWavSelectPage)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCZWavSelectPage message handlers

BOOL CCZWavSelectPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////////////
	FillListBox();
	
	if(!m_edit.IsEmpty())
     m_list.SelectString(0,m_edit);

	OnSelchangeList1();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCZWavSelectPage::FillListBox()
{
     CFileFind  wavdir;
	 char chpath[MAX_PATH]; 
	 GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
	 CString file;// = theApp.GetDirectory();
	 file.Format("%s",chpath);
	 CString wavname;
	 file += "global\\wav\\*.wav";

	 m_list.ResetContent();

	 if (wavdir.FindFile(file))
	 {
		 while (wavdir.FindNextFile())
		 {
			 wavname = wavdir.GetFileTitle();
			 m_list.AddString(wavname);
		 }
		 
		 wavname = wavdir.GetFileTitle();
		 m_list.AddString(wavname);
		 
		 wavdir.Close();
	 }
}

CString CCZWavSelectPage::GetContent()
{
	return m_edit.Left(32);	
}

void CCZWavSelectPage::SetContent(LPCSTR wavename)
{
    m_edit = wavename;
}

void CCZWavSelectPage::OnSelchangeList1() 
{
	int pos = m_list.GetCurSel();
	CString wavname;

	if (pos >= 0)
	{
		m_list.GetText(pos,wavname);
		m_edit = wavname;
	}
	
	UpdateData(FALSE);
}

void CCZWavSelectPage::OnButton1() 
{
	m_button.EnableWindow(false);

	char chpath[MAX_PATH]; 
	GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
	CString wavefilepath;
	wavefilepath.Format("%sglobal\\wav\\%s.wav", chpath ,m_edit);//theApp.GetDirectory()

	//if (sndPlaySound(wavefilepath,SND_SYNC|SND_NODEFAULT))
	//{
	//    sndPlaySound(NULL,SND_SYNC|SND_NODEFAULT);
	//}
	//else
	//{
	//	CString hh=wavefilepath+"≤•∑≈ ß∞‹!";
	//	AfxMessageBox(hh);
	//}

	m_button.EnableWindow(true);
}
