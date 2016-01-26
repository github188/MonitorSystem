#pragma once

#include "afxwin.h"
#include "sp_draw.h"

// CPropLinkFile 对话框

class CPropLinkFile : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropLinkFile)

public:
	CPropLinkFile();
	virtual ~CPropLinkFile();

// 对话框数据
	enum { IDD = IDD_LINK_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_filename;			//文件名
	CString m_ext;					//文件扩展名
	CString m_path;					//文件相对路径
public:
	CListBox m_listbox;
	CButton m_Btn_Advance;
	virtual BOOL OnInitDialog();
private:
	void FillListBox(void);
public:
	void Setfile(LPCSTR path, LPCSTR filename, LPCSTR ext);
	void Getfile(char* filename);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
};
