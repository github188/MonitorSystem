// HuaMian.h: interface for the CHuaMian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUAMIAN_H__60324C54_3E18_4898_92F6_7F0D36653691__INCLUDED_)
#define AFX_HUAMIAN_H__60324C54_3E18_4898_92F6_7F0D36653691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/tybase.h"
#include "canvas/hmlayer.h"
#include "gloable/DataStyle.h"

//画面风格
#define HMT_TEMPLE		0x1		//画面模板
#define HMT_SCALE			0x2		//画面伸缩
#define HMT_BMP				0x4		//画面背景为位图


class CHMProject;
class CElecMapView;
class CElecMapDoc;
/*-----------------------------------------------------------------------------
	--------------------		对象类：	画面CHuaMian 
 	--------------------		设计者：	高正军
 	--------------------    日期：		2001-9-18		
	-----------------------------------------------------------------------------


 --------画面属性----------------------------------------

	BOOL GetScaleProp()
		函数：画面是否支持伸缩功能

	BOOL GetTempleProp()
		函数：画面是否是画面模板类型

	CString& GetHMName();
		函数：返回画面名称

	void GetScaleRange(float * pMinScale,float * pMaxScale);
		函数：返回画面比例范围


  --------画面层处理----------------------------------------

	CHMLayer * AddHMLayer(tagHMLayerID *pLayer);
		函数：增加画面层
		参数:
			pLayer-画面层属性数据
		返回 画面层对象

	void ModifyLayerProp(LPCSTR pLayer, tagHMLayerID *pLayerID);
		函数：修改画面层属性
		参数:
			pLayer-画面层中文名
			pLayerID-新画面层属性数据

	CHMLayer * FindLayer(LPCSTR strLayer);
		函数：查找画面层对象
		参数:
			strLayer-画面层中文名
		返回 strLayer对应的画面层对象

	CHMLayer * FindLayer(int LayerID);
		函数：查找画面层对象
		参数:
			strLayer-画面层标识号
		返回 strLayer对应的画面层对象

	void ModifyLayerProp(LPCSTR pLayer, tagHMLayerID *pLayerID);
		函数：修改画面层pLayer属性
		参数:
			pLayer-画面层中文名
			pLayerID-画面层属性数据

	void DeleteLayer(LPCSTR strName);
		函数：删除画面层strName
		参数:
			strName-画面层中文名

	void ZhenLi()
		函数：整理  把各个画面层中被删除的图元剔出


	void MoveNext(LPCSTR layername);
		函数：	画面层layername向下移动一层

	void MovePre(LPCSTR layername);
		函数：	画面层layername向上移动一层


---------
  事件
---------

	void OnHMSizeChanged();
		函数：处理画面大小发生变化的事件

	void OnDeleteLayer(CHMLayer *pLayer)
		函数：删除画面层事件
		参数:
			pLayer-画面层中文名

  void CHuaMian::OnRemoveTyEvent(CTyBase *pTy)
		函数：删除图元事件
		参数:
			pTy-指向图元的指针

  void CHuaMian::OnAddTyEvent(CTyBase *pTy)
		函数：增加图元事件
		参数:
			pTy-指向图元的指针


-----------------------------------------------------------------------------
*/


class CHuaMian : public CObject  
{
	DECLARE_SERIAL(CHuaMian);

private:
	int  m_iLayerIdDef;						   //画面层的起始预定义号
	CElecMapDoc * m_pDoc;				   //指向绘图文档的指针

protected:
	DWORD m_dwPICStyle;					//画面风格
	WORD	m_wPicWidth;						//图幅宽度
	WORD	m_wPicHeight;						//图幅高度
	WORD	m_wResolution;					//屏幕分辨率的宽度
	WORD	m_hResolution;					//屏幕分辨率的高度
	float  m_fDefaultScale;				        //默认的画面显示比例
	CString m_strName;						    //画面名称

public:
	CHMLayerList  m_HMLayerList;	//画面所有的层对象
	CHuaMian(CElecMapDoc* pdoc=NULL);
	CHuaMian(LPCSTR strName,CElecMapDoc* pdoc=NULL,DWORD style=HMT_SCALE,WORD picwidth=1024,WORD picheight=768,WORD wresolution=1024,WORD hresolution=768,float scale=1,int preLayerID=0);

public:
	void GetScaleRange(float * pMinScale,float * pMaxScale);
	void MoveNext(LPCSTR layername);
	void MovePre(LPCSTR layername);
	void ZhenLi();
	float GetDefaultScale();
	BOOL GetBmpBackGroundMode();
	COLORREF m_dwBackColor;				//画面背景颜色
	DWORD		m_dwBackImageNo;			//背景位图编号

	void OnRemoveTyEvent(CTyBase *pTy);
	void OnDeleteLayer(CHMLayer* pLayer);
	void DeleteLayer(LPCSTR strName);
	CHMLayer * FindLayer(int LayerID);
	CHMLayer * FindLayer(LPCSTR strLayer);
	void ModifyLayerProp(LPCSTR pLayer, tagHMLayerID *pLayerID);
	CHMLayer * AddHMLayer(tagHMLayerID *pLayer);
	void OnHMSizeChanged();

	void DrawDC(CDC *pDC, CElecMapView *pView);

	CSize GetHMSize();
	void SavePicHead(CFile *file,tagHuaMianFileHead* filehead, BOOL Yn);
	void SetHMID( tagHMID *pHMID);
	CElecMapDoc* GetDocument(){return m_pDoc;}; 
	CString GetHMName(void);
	BOOL GetTempleProp(void);
	void Save(CFile *file, BOOL Yn);
	virtual void Serialize(CArchive& ar);
	BOOL GetScaleProp(void);
	void OnAddTyEvent(CTyBase *pTy);
	void ParamReplace(CStringList& ruleList);

	virtual ~CHuaMian();

	static WORD  m_wPicFlag;				//文件特定标志
	static WORD  m_wMVersion;				//文件高版本号
	static WORD  m_wEVersion;				//文件低版本号
	friend class CHMLayer;

public:
	///update entity status.
	BOOL updateEntityInAHuamian( char *pRealData, int nCount );
};

#endif // !defined(AFX_HUAMIAN_H__60324C54_3E18_4898_92F6_7F0D36653691__INCLUDED_)
