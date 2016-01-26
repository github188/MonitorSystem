#pragma once

#include "SkinButton.h"
#include "Label.h"
#include "BitmapSlider.h"

// CDlgVideoParSet 对话框

class CDlgVideoParSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgVideoParSet)

public:
	CDlgVideoParSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVideoParSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIDEOPARSET };
public:
	CLabel			m_titlebrite;
	CLabel			m_titlebaohe;
	CLabel			m_titleduibi;
	CLabel			m_titlese;
	CBitmapSlider	m_sliderbrite;
	CBitmapSlider	m_sliderbaohe;
	CBitmapSlider	m_slidersedu;
	CBitmapSlider	m_sliderduibi;
	CSkinButton		m_btnSetViewPar;
	CSkinButton		m_btnDefaultViewPar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnControlButton(UINT nID);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
