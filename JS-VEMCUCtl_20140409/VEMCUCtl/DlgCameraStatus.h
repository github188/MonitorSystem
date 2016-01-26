#pragma once


// CDlgCameraStatus 对话框

class CDlgCameraStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgCameraStatus)
private:
	CString m_strCameraStatusValue;

public:
	CDlgCameraStatus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCameraStatus();
	CDlgCameraStatus(char *szCameraCode, CWnd* pParent = NULL);

// 对话框数据
	enum { IDD = IDD_DIALOG_CAMERA_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
public:
	char m_szCameraCode[20];
	virtual BOOL OnInitDialog();
	_T_DIAGNOSIS_INFO *GetDiagnosisInfoByCameraCode(char *szCameraCode);
	void SpellDiagnosisText(_T_DIAGNOSIS_INFO* pDiagnosisInfo, CString &strDiagnosisText1, CString &strDiagnosisText2);
	_T_DIAGNOSIS_INFO *m_pDiagnosisInfo;
	CString m_strDiagnosisText1;
	CString m_strDiagnosisText2;
};
