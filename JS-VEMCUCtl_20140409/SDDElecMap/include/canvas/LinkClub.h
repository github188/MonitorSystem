#if !defined(AFX_LINKCLUB_H__35726B1A_7F50_481A_8E8A_A3837ACD7DA9__INCLUDED_)
#define AFX_LINKCLUB_H__35726B1A_7F50_481A_8E8A_A3837ACD7DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LinkClub.h : header file
//
#include "LinkObjectBase.h"
#include "TyBase.h"

/////////////////////////////////////////////////////////////////////////////
// CLinkClub window
typedef struct{
	char title[33];//标题
	LOGFONT titlefont;//标题字体
	COLORREF titlecolor;//标题颜色
	char yunit[9];//Y轴单位
	float ymax;//y轴最大值
	float ymin;//y轴最小值
	LOGFONT yunitfont;//y轴单位字体
	COLORREF yunitcolor;//Y轴单位颜色
	LOGFONT coordinatefont;//坐标系字体
	COLORREF coordinatecolor;//坐标系颜色
	COLORREF backcolor;//背景色
	BYTE allmaxmin;//统一越限值0 not 1 yes
	float maxvalue;//统一的最大值
	float minvalue;//统一的最小值
	DWORD clubwidth;//棒宽度
	DWORD clubspace;//棒间距
//	CFONT
}STRUCT_CLUB;

typedef struct{
	char ename[17];
	char cname[9];
}STRUCT_SINGLEBAR;

class CLinkClub : public CLinkObjectBase
{
// Construction
	DECLARE_SERIAL(CLinkClub);
public:
	STRUCT_CLUB clubinfo;
	STRUCT_SINGLEBAR barinfo[64];
	BYTE barcount;

	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	CLinkClub();

// Attributes
public:

// Operations
public:
//	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
//	virtual void Serialize_Ty(CArchive &ar);
//	virtual ~CLinkYk();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkClub)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLinkClub();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKCLUB_H__35726B1A_7F50_481A_8E8A_A3837ACD7DA9__INCLUDED_)
