#pragma once

#include "../../resource.h"

// CYMcCurvePage1 对话框

class CYMcCurvePage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CYMcCurvePage1)

public:
	CYMcCurvePage1();
	virtual ~CYMcCurvePage1();

// 对话框数据
	enum { IDD = IDD_YMCURVEPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 名称
	CString m_strmcename;
	float m_fLeftcoord;
	float m_fTopcoord;
	float m_fXlen;
	float m_fYlen;
	// 小时曲线
	DWORD m_hMin;
	DWORD m_hMax;
	DWORD m_hScale;
	// 日曲线
	DWORD m_dMin;
	DWORD m_dMax;
	DWORD m_dScale;
	// 月曲线
	DWORD m_mMin;
	DWORD m_mMax;
	DWORD m_mScale;
	// 年曲线
	DWORD m_yMin;
	DWORD m_yMax;
	DWORD m_yScale;
	// 坐标轴单位
	CString m_unit;
};
