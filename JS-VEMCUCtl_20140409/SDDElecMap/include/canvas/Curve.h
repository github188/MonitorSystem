// Curve.h: interface for the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVE_H__F53ED881_D908_11D5_BFC5_0050BABDFD68__INCLUDED_)
#define AFX_CURVE_H__F53ED881_D908_11D5_BFC5_0050BABDFD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkObjectBase.h"

typedef struct
{
	char ename[17];                  //英文名
	char cname[33];                  //中文名
	float scale;                     //比例
	int precission;                  //小数精度
	COLORREF color;                  //曲线颜色
	int width;                       //曲线宽度
	int reference;                   //参照坐标系
	BOOL compare;                    //是否与计划曲线比较
	int swing;                       //差值振幅
}curveinfo;

typedef struct
{
	char curvegroupname[33];           //曲线组名称
	int curvegroupstyle;               //曲线组风格
	int coordinatestyle;               //坐标系风格
	int curstyle;                      //描绘曲线的风格
	int firstcount;                    //第一组曲线条数
	int secondcount;                   //第二组曲线条数
//	char firstname[33];                //第一组曲线名称
	float firstleftmin;                //第一组曲线左纵坐标最小值
	float firstleftmax;                //第一组曲线左纵坐标最大值
	float firstleftinterval;           //第一组曲线左纵坐标间隔
	char firstleftunit[9];             //第一组曲线左纵坐标单位
	float firstrightmin;               //第一组曲线右纵坐标最小值
	float firstrightmax;               //第一组曲线右纵坐标最大值
	float firstrightinterval;          //第一组曲线右纵坐标间隔
	char firstrightunit[9];            //第一组曲线右纵坐标单位
//	char secondname[33];               //第二组曲线名称
	float secondleftmin;               //第二组曲线左纵坐标最小值
	float secondleftmax;               //第二组曲线左纵坐标最大值
	float secondleftinterval;          //第二组曲线左纵坐标间隔
	char secondleftunit[9];            //第二组曲线左纵坐标单位
	float secondrightmin;              //第二组曲线右纵坐标最小值
	float secondrightmax;              //第二组曲线右纵坐标最大值
	float secondrightinterval;         //第二组曲线右纵坐标间隔
	char secondrightunit[9];           //第二组曲线右纵坐标单位
	curveinfo firstinfolist[8];        //第一组曲线信息数组
	curveinfo secondinfolist[8];       //第二组曲线信息数组
} curvegroupinfo;

struct CURVEEXTINFO
{
	BYTE flag;
	float maxValue;
	float minValue;
	int color;//上限颜色
	int mincolor;//下限颜色
	int tpye;
	float width;
};

class CCurve : public CLinkObjectBase  
{
	DECLARE_SERIAL(CCurve);
public:	
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	BOOL OnEditProperties();
	CCurve();
	virtual ~CCurve();
	virtual void ParamReplace_Ty(CStringList& ruleList);


	curvegroupinfo m_curvegroupinfo;

	
protected:	
private:
	CURVEEXTINFO m_CurveExtInfo[2];
	void LoadExt(CString name);
	void SaveExt(CString name, int bFlag);
};

#endif // !defined(AFX_CURVE_H__F53ED881_D908_11D5_BFC5_0050BABDFD68__INCLUDED_)
