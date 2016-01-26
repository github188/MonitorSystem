#pragma once
#include "../../resource.h"
#include "canvas/lineprop.h"


// CNewYC_PROP_INFO 对话框
typedef BOOL _stdcall ShowFindDotDlg(int kind,char * ename,char * cname);

class CNewYC_PROP_INFO : public CPropertyPage
{
	DECLARE_DYNAMIC(CNewYC_PROP_INFO)

public:
	CNewYC_PROP_INFO(BYTE kind=1);
	virtual ~CNewYC_PROP_INFO();


public:
	CRTUColorStatic m_hintc;
	CRTUColorStatic m_unitc;
	
    COLORREF hintcolor;//提示颜色;
    COLORREF unitcolor;//单位颜色

	BYTE hintenable;
	BYTE itkind;

	void GetPara(char * ename,int *digitlength,char * hint,char * unit,
		COLORREF *unitc,COLORREF *hintc,BYTE * hintena,float *hintjj,float *unitjj ,
				float* UUp ,float* DDown);

	void SetPara(char * ename,int digitlength,char * hint,char * unit,
		COLORREF unitc,COLORREF hintc,BYTE hintena,float hintjj,float unitjj ,
				float UUp ,float DDown);


// 对话框数据
	enum { IDD = CZ_IDD_NEW_YC_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_ename;
	CString m_unit;
	int m_digitlength;
	CString m_hint;

	float m_edithintdis;
	float m_editunitdis;

	float m_fUUp;
	float m_fDDown;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
