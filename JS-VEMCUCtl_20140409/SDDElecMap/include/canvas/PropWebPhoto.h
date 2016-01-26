#pragma once

#include "afxwin.h"
//#include "sp_draw.h"
#include "../../resource.h"
// CPropWebPhoto 对话框

class CPropWebPhoto : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropWebPhoto)

public:
	CPropWebPhoto();
	virtual ~CPropWebPhoto();

// 对话框数据
	enum { IDD = IDD_WEB_SHOT };
private:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetProp(LPCSTR discription,DWORD webIP,DWORD serverIP,int serverPort,DWORD groupIP,int groupPort,int unitID,LPCSTR unitName,int shotID, LPCSTR shotName);
	void GetProp(char* discription,DWORD& webIP,DWORD& serverIP,int& serverPort,DWORD& groupIP,int& groupPort,int& unitID, char* unitName,int& shotID, char* shotName);
	CString m_discription;
	DWORD m_webIP;
	DWORD m_serverIP;
	int m_serverPort;
	DWORD m_groupIP;
	int m_groupPort;
	int m_unitID;
	CString m_unitName;
	int m_shotID;
	CString m_shotName;
};
