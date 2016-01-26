// HMLayer.h: interface for the CHMLayer class.
//
/*

----------
构造函数
----------

	CHMLayer(float startBL,float endBL, float defBL,int layerID,LPCSTR pStr,CHuaMian *pHuaMian,int sPos,int ePos,int sTyID);
	参数:
		startBL-	起始显示比例 >=
		endBL-		结束显示比例 <
		defBL-		默认显示比例
		layerID-	画面层标识号
		pStr-			画面层中文名
		sPos-			图元最后端位置标识
		ePos-			图元最前端位置标识
		sTyID-		图元起始标识


------------
图元的管理
------------

	void Add(CTyBase* pTy);
	功能: 向画面层增加一个图元
	参数:
		pTy-指向图元的指针 

	CTyBase * FindTyBase(int id);
	功能: 查找图元
	参数:
		id-图元标识号 
  返回:
      如果找到图元则返回该图元 找不到则返回NULL 

	CTyBase * ObjectAt(PointStruct pt,float errRange)
	功能: 查找在该点投影的图元
	参数:
		pt- 点的用户坐标
		errRange- 误差范围
  返回:
      如果找到图元则返回该图元 找不到则返回NULL 


	void Front(CTyBase* pTy);
	功能: 图元移到前端
	参数:
		pTy-指向图元的指针 

	void Back(CTyBase * pTy);
	功能: 图元移到后端
	参数:
		pTy-指向图元的指针 

	void SelectWithinRect(RectStruct rt, CSp_drawView *pView)
	功能: 选择包括在区域rt内的图元
	参数:
		rt-区域用户坐标 
		pView-指向显示窗的指针


------------
画面层显示
------------
 
	BOOL IsInViewBL(float fScale);
	功能: 测定显示比例是否在该画面层的视野范围内
	参数:
		fScale-要测定的显示比列 
  返回:
			TRUE-在视野范围内		

---------------------
画面层属性处理的方法
---------------------

	void OnHMLayerDataChanged();
	功能:画面的大小或画面层的比例发生变化后的处理事件

	void ModifyProp( tagHMLayerID *pLayerID);
	功能:修改画面层属性
	参数:
		pLayerID-画面层属性数据 

------------
画面层事件:
------------
	void OnAddTy(CTyBase *pTy,BOOL blNumberChanged=TRUE);
	功能:增加一个图元事件
	参数:
		pTy-指向图元的指针 
		blNumberChanged:图元的位置号是否调整

	void OnRemoveTy(CTyBase *pTy);
	功能:删除一个图元事件
	参数:
		pTy-指向图元的指针 

  void OnPositionChangedTy(CTyBase *pTy, RectStruct oldRt, RectStruct newRt)
	功能:图元位置变化事件
	参数:
		pTy-指向图元的指针 
		oldRt-变化前的位置区域
		newRt-变化后的位置区域


----------
切割面:
----------
	
	CRect GetArrQGMBoundsRect(float x0,float y0,float x1,float y1);
	功能:获取一个用户区域在切割面矩阵的覆盖面
	参数:
		x0,y0,x1,y1-用户区域坐标
	返回:
	  返回在切割面矩阵中的覆盖面

		
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMLAYER_H__4C315EA3_ADCC_4A42_BDFE_A15F620693CD__INCLUDED_)
#define AFX_HMLAYER_H__4C315EA3_ADCC_4A42_BDFE_A15F620693CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "canvas/tybase.h"
#include "gloable/DataStyle.h"

class CHuaMian; 
class CElecMapDoc;
class CQGM;
class CElecMapView;
class CZoomInTool;
class CZoomOutTool;

class CHMLayer : public CObject  
{
	DECLARE_SERIAL(CHMLayer);

public:
	CHMLayer(float startBL,float endBL, float defBL,int layerID,LPCSTR pStr,CHuaMian *pHuaMian,int sPos=0,int ePos=0,int sTyID=0);
	virtual ~CHMLayer();

private:
	CRect GetArrQGMBoundsRect(float x0,float y0,float x1,float y1);
	int m_iStartNumber;	//图元最后面的位置编号
	int m_iEndNumber;		//图元最前面的位置编号	
	int m_IDDef;				    //图元标识的起始值

protected:
	CHMLayer(CHuaMian *pHuaMian = NULL)
	{	
		m_pHuaMian = pHuaMian;
		m_uRow = 0;
		m_uCol = 0;
		ZeroMemory(QGMArr,sizeof(QGMArr));
	}

protected:
	CTyBaseList m_TyList;		//该层所有的图元的集合
	float m_startViewBL;		//显示起始比列
	float m_endViewBL;			//显示结束比列
	float m_DefaultBL;			//缺省显示比列
	int		  m_iLayerNo;		//层序号
	CString  m_CName;			//画面层中文名

public:
	void OnPositionChangedTy(CTyBase* pTy,RectStruct oldRt,RectStruct newRt);
	void ZhenLi();
	void SelectWithinRect(RectStruct rt,CElecMapView *pView);
	float GetDefaultScale();
	CTyBase * ObjectAt(PointStruct pt,float errRange);
	
	void Add(CTyBase* pTy,BOOL blNumberChanged = TRUE);
	void Remove(CTyBase * pTy);
	void Front(CTyBase* pTy);
	void Back(CTyBase * pTy);

	CElecMapDoc * GetDocument();
	CTyBase * FindTyBase(int id);
	void ModifyProp( tagHMLayerID *pLayerID);

	void OnAddTy(CTyBase *pTy);
	void OnRemoveTy(CTyBase* pTy);
	void OnHMLayerDataChanged();

	void DrawDC(CDC *pDC, CElecMapView *pView);
	BOOL IsInViewBL(float fScale);
	void Save(CFile *file, BOOL Yn);

	void ParamReplace(CStringList& ruleList);

public:
	CHuaMian*	m_pHuaMian;	//指向该层所在的画面对象	
	CQGM*  QGMArr[MAX_QGMROW][MAX_QGMCOL];//切割面2维数组
	UINT m_uRow;													//数组的行数
	UINT m_uCol;													//数组的列数
	BOOL m_bHide;												//是否隐藏显示	

public:
	friend class CHuaMian;
	friend class CZoomInTool;
	friend class CZoomOutTool;

	#ifdef _DEBUG
		 void ASSERTValid();
	#endif
};

typedef CTypedPtrList<CObList, CHMLayer*> CHMLayerList;


#endif // !defined(AFX_HMLAYER_H__4C315EA3_ADCC_4A42_BDFE_A15F620693CD__INCLUDED_)
