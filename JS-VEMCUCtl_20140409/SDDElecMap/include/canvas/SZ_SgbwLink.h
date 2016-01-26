// SZ_SgbwLink.h: interface for the CSZ_SgbwLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SZ_SGBWLINK_H__DF32DB81_3C0A_11D6_A5ED_0080C8F60823__INCLUDED_)
#define AFX_SZ_SGBWLINK_H__DF32DB81_3C0A_11D6_A5ED_0080C8F60823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkObjectBase.h"
#define SZ_SGBW_LINENUM								32			//事故报文曲线条数

#pragma pack(push,before_SZ_S_SGBWLINEBASE,1)
typedef struct
{
	BYTE TdNo;										//通道号
	char DotName[17];								//点名
	float Prop;										//比例
	BYTE Enum;										//小数位数
	COLORREF Color;									//颜色
	BYTE Zbgt;										//自闭还是贯通：1自闭，2贯通
	BYTE UorI;										//电流还是电压：1电压，2电流
}SZ_S_SGBWLINEBASE;									//事故报文线条基本结构
#pragma pack(pop,before_SZ_S_SGBWLINEBASE,8)

#pragma pack(push,before_SZ_S_SGBWMAINBASE,1)
typedef struct
{
	BOOL Zbgt;										//是否是自闭贯通方式
	BOOL HaveUandI;									//有无电流电压
	char RtuName[17];								//单元名称
	float MaxU;										//电压最大值
	float MinU;										//电压最小值
	float MaxI;										//电流最大值
	float MinI;										//电流最小值
}SZ_S_SGBWMAININFO;									//事故报文主信息结构
#pragma pack(pop,before_SZ_S_SGBWMAINBASE,8)

class CSZ_SgbwLink : public CLinkObjectBase  
{
	DECLARE_SERIAL(CSZ_SgbwLink);
public:
	CSZ_SgbwLink();
public:
	CSZ_SgbwLink(CString name);
	virtual void Serialize_Ty(CArchive &ar);
	virtual BOOL OnEditProperties();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual ~CSZ_SgbwLink();
	virtual void ParamReplace_Ty(CStringList& ruleList);

private:
	SZ_S_SGBWMAININFO m_sMainInfo;
	SZ_S_SGBWLINEBASE m_sLineInfo[SZ_SGBW_LINENUM];
	int m_nLineCount;
};

#endif // !defined(AFX_SZ_SGBWLINK_H__DF32DB81_3C0A_11D6_A5ED_0080C8F60823__INCLUDED_)
