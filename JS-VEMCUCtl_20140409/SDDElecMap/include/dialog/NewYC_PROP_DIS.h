#pragma once
#include "canvas/LineProp.h"
#include "../../resource.h"
// CNewYC_PROP_DIS 对话框

class CNewYC_PROP_DIS : public CPropertyPage
{
	DECLARE_DYNAMIC(CNewYC_PROP_DIS)

public:
	CNewYC_PROP_DIS(BYTE kind=1);
	virtual ~CNewYC_PROP_DIS();

	CRTUColorStatic m_normalc;
	CRTUColorStatic m_disablec;
	CRTUColorStatic m_upc;		//上限
	CRTUColorStatic m_downc;	//下限

	CRTUColorStatic m_uupc;		//上上限值
	CRTUColorStatic m_ddownc;	//下下限值

	BYTE itkind;//1 yc 2 ym

	LOGFONT lgfont;//字体
    COLORREF levelupcolor;//越上限颜色
	COLORREF leveluupcolor;//越上上限颜色

    COLORREF leveldowncolor;//越下限颜色
	COLORREF levelddowncolor;//越下下限颜色

    COLORREF normalcolor;//正常颜色
    COLORREF disablecolor;//无效颜色


	float rectx0;
	float recty0;
	float fontwidth;
	float fontheight;
	float fontjj;

	//yc
	void GetPara(COLORREF *disablec,COLORREF *normalc,COLORREF *leveluc,COLORREF *leveluuc,
		COLORREF *leveldc,COLORREF *levelddc ,LOGFONT *ff,float *x0,float *y0,float *fontw,
			float *fonth,float *fontj);

	void SetPara(COLORREF disablec,COLORREF normalc,COLORREF leveluc,COLORREF leveluuc,
		COLORREF leveldc,COLORREF levelddc ,LOGFONT ff,float x0,float y0,float fontw,
			float fonth,float fontj);
	//ym
	void GetPara2(COLORREF *disablec,COLORREF *normalc,
		LOGFONT *ff,float *x0,float *y0,float *fontw,float *fonth,float *fontj);
	void SetPara2(COLORREF disablec,COLORREF normalc,
		LOGFONT ff,float x0,float y0,float fontw,float fonth,float fontj);


// 对话框数据
	enum { IDD = CZ_IDD_NEW_YC_DIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_editx0;
	float m_edity0;
	float m_editfh;
	float m_editfw;
	float m_editjj;
	afx_msg void OnBnClickedButtonYcFont();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
