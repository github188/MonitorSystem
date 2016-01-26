/////////////////////////////////////////////////////////////////////
// TyBase.h: interface for the CTyBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYBASE_H__72E869AE_6422_44B5_9640_628F3F263EC2__INCLUDED_)
#define AFX_TYBASE_H__72E869AE_6422_44B5_9640_628F3F263EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "canvas/LinkObjectBase.h"
#include <math.h>

#define pi 3.1415926
#define MAXPOINTNUM 500
#define OFFSETX 4
#define OFFSETY 4

#define NET_RENDER 0x80000000


/*---------------------------------------------------------------------------------
	*								DRAW_TY 工具条的汇总
	*--------------------------------------------------------------------------------
	tySelect				选择,
	tyZoomOut			放大
	tyZoomIn				缩小
	tyLine					直线,
	tyXX						虚线,
	tyHX						弧线,
	tyZX						折线
	tyRect					矩形,
	tyRoundRect		圆角矩形,
	tyEllipse				椭圆,
	tyDBX					多边形,
	tyArc					扇形,
	tyText					文本,
	tyCL						潮流
	tyYDX                   双遥信 
	tyBmp					位图
	lnkYT					遥调
	lnkYK					遥控
	lnkBHTQ				保护投切
	lnkYZTY				有载调压
	tyYC                     遥测
	tyYX						遥信
	tyYM					电度
	lnkCur					曲线
	lnkTicket              操作票
	lnkLB					录波曲线
	lnk_SZ_SGBW		事故报文
	lnkNetColorPower  动态着色层电源 
	lnkNetColorVect     动态着色层矢量图元
	lnkNetColorBmp     动态着色层位图
	lnkNetColorYx         动态着色层开关
	lnkClub					棒图
	tyAnimate            位图动画 
    lnk_SZ_PHOTO     图片链接
	lnkUnit					单元链接
	tyVector				矢量图元
	lnkTxt					文本文件链接
	tyMonNiYK		    模拟操作图元
	lnkWebShot			web图像监控通道链接
	lnk_SZ_SBLOOK		设备管理程序链接
	lnkMNP_SHXG         上海新光模拟屏
	lnkMNP_ZJNH         浙江宁海模拟屏
	lnkNode					节点链接
	lnkLine					    线路链接
	tymccurve               毛刺曲线
	lnkFile					    普通文件链接

-----------------------------------------------------------------------------------
*/


/*---------------------------------------------------------------------------------
	*								链接对象的类型
	*--------------------------------------------------------------------------------
	
	
*/

typedef enum
{
    tyNone=0,
	tySelect=1,
	tyZoomOut=2,
	tyZoomIn=3,
	tyLine=4,
	tyXX=5,
	tyHX=6,
	tyZX=7,
	tyRect=8,
	tyRoundRect=9,
	tyEllipse=10,
	tyDBX=11,
	tyArc=12,
	tyText=13,
	tyCL=14,
	tyDateTime=15,
	tyYDX=16,
	tyBmp=17,
	lnkYK=18,
	lnkYT=19,
	lnkBHTQ=20,
	lnkYZTY=21,
	tyYC=22,
	tyYX=23,
	tyYM=24,
	tyHmProxy=25,
	lnkCur=26,
	lnkLB=27,
	lnk_SZ_SGBW=28,
	lnkNetColorPower=29,
	lnkNetColorVect=30,
	lnkNetColorBmp=31,
	lnkNetColorYx=32,
	lnkClub=33,
	tyAnimate=34,
	lnk_SZ_PHOTO=35,
	lnkUnit=36,
	tyVector=37,
	lnkTxt=38,
	tyMonNiYK=39,
	lnkWebShot=40,
	lnk_SZ_SBLOOK=41,
	lnkMNP_SHXG=42,
	lnkMNP_ZJNH=43,
	lnkTicket=44,
    lnkNode=45,
	lnkLine=46,
	tymccurve=47,
	lnkFile=48,
	tyYcYw=49,
	tyNewYC = 50,
	lnk_SZ_TIMEYC=51,
	tyYmccurve = 62 ,
	//拨号图元
	lnkModem = 63 ,
	lnkYT2 = 64,

	//遥信绑定图元
	lnkYXBind = 65 ,

	//监控模块绑定图元----add by 20151208
	lnkYSHBind = 106,

	lnkQK = 66 ,
	//后台检修牌
	lnkSoftRepair = 67 ,
	//单元扩展
	lnkUnitExtend = 68 ,
	//馈线开关链接
	lnkKX = 69 ,
	//馈线区段链接
	lnkKXSection = 70 ,
	//水行业控制链接
	lnkDoubleCtrl = 71 ,
    tyyccurve=72,
	lnkSupervise = 73,
	lnkPowerGzTj=74,
	lnkLineGzTj=75,
	lnkZK=76,
	tyMove=80,
	tyStationAssistPng=100,

	tyTransSubstation = 101,
	tyCamera =102,// 摄像机
	tyLight  =103,// 灯光
	tyGate   =104,// 门禁
	tyTemp   =105 // 温度
} DRAW_TY;


/*---------------------------------------------------------------------------------
	*								
	*--------------------------------------------------------------------------------

*/

typedef struct
{
	float x;
	float y;
}PointStruct;

typedef struct
{
	float x0;
	float y0;
	float x1;
	float y1;

	void InflateRect(float x,float y)
	{
		x0-=x;
		y0-=y;
		x1+=x;
		y1+=y;
	}; 

	float Width()
	{
		return (float)fabs(x1-x0);	
	}

	float Height()
	{
		return (float)fabs(y1-y0);	
	}
}RectStruct;


class CElecMapView;

/*
--------
算术运算:
--------
	float PointLine(float xx,float yy,float x1,float y1,float x2,float y2)
			函数：计算点（xx,yy)到线段（x1,y1)(x2,y2)的距离，返回计算的距离值

	float CalDisp(float x1,float y1,float x2,float y2)
			函数：计算点(x1,y1)与点(x2,y2)间的距离

	BOOL PointRgn(float x,float y,int Numble,PointStruct *PointList,float blc)
			函数：判断一个点是否在任意多边形区域内
			参数：Numble-边界点数，x,y是点的坐标，多边形顶点的坐标在结构数组PointList中
			返回：1-点在多边形区域0-不在

	BOOL PointInObj(float x,float y,float errRange);
			函数：判断一个点是否在对象区域内
			参数：
					x,y-点的用户坐标
					errRange-误差范围(用户坐标)
			返回：TRUE-点在对象区域内	FALSE-不在


--------
图元操作
--------
	void Move(float xLen,float yLen);
			函数：移动图元
			参数：
					xLen-x方向的距离
					yLen-y方向的距离

--------
事件
--------
	void OnEditProperties()
			函数：编辑图元属性

--------
把手
--------
	int GetHandleCount()
			函数：返回图元的把手数目
	
	PointStruct GetHandle(int nHandle);
			函数：返回特定标识把手nHandle的中心位置(用户坐标)

	CRect GetHandleRect(int nHandleID, CElecMapView* pView);
			函数：返回特定标识把手nHandle的矩形范围（逻辑坐标）

	void MoveHandleTo(int nHandle, PointStruct point, CSp_drawView* pView,UINT nFlags);
			函数：移动特定标识把手nHandle到点point(用户坐标）


---------
链接对象
---------
	BOOL OnEditLinkObjectProperties(int pos)
			函数：编辑第pos个链接对象的属性
			参数:
					pos 链接对象的位置
			返回值:是否修改 

	BOOL ExecuteLinkObject(int pos1,int pos2);
	函数：触发第pos1个链接对象的第pos2个动作的执行
			参数:
					pos1 链接对象的位置
					pos2-链接对象的动作序号
			返回值:跳转是否成功 

  void AddLinkObject(CLinkObjectBase *pLink)
			函数：加入一个链接对象pLink

	void DeleteLinkObject(int pos);
			函数：删除第pos位置的链接对象

	int Get_LinkTy_Count()
		函数：获取图元现在所拥有的链接对象的数目


---------
图象管理
---------
	void UnRegisterImage(LPCSTR bmpname);
	     函数: 注销图象bmpname

	void RegisterImage(LPCSTR bmpname);
	     函数: 登记图象bmpname

	void DrawImage(LPCSTR bmpname,CDC* pDC, int x0, int y0, int nWidth, int nHeight,BOOL bTranslate,COLORREF color,BOOL blHScale=TRUE,BOOL blVScale=TRUE);
	     函数: 显示图象
			 参数：
					bmpname-图象名称
					x0，y0-显示起始坐标位置（逻辑坐标）
          nWidth-显示宽度
					nHeight-显示高度
					bTranslate-背景是否透明
					color-透明颜色
					blHScale-是否水平方向拉伸
					blVScale-是否垂直方向拉伸

------------
矢量图管理
------------
	void UnRegisterVector(LPCSTR vectorname);
	     函数: 注销矢量图vectorname

	void RegisterVector(LPCSTR vectorname);
	     函数: 登记矢量图vectorname

	void DrawVector(LPCSTR vectorname,CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace=TRUE);
	     函数: 显示矢量图
			 参数：
					vectorname-矢量图名称
					viewRect-矢量图显示范围（逻辑坐标）
          clipRect-矢量图显示剪切范围（逻辑坐标）
					color-动态颜色

  CSize Get_Size_Vector(LPCSTR vectorname)
	     函数: 获取矢量图的大小
			 参数：
					vectorname-矢量图名称

	void DrawRect(CDC* pDC,int offsetX,int offsetY,float scaleX,float scaleY,COLORREF ReplaceColor,COLORREF ToColor,BOOL bReplace=TRUE)
	     函数: 图元本身作为矢量图的一部分进行绘制
			 参数：
				 pDC-DC
				 offsetX-图元绘制时的偏移量x
				 offsetY-图元绘制时的偏移量y
				 scaleX-X方向上的比例
				 scaleY-Y方向上的比例
				 ReplaceColor-替代前颜色
				 ToColor-替代后颜色
				 bReplace-是否替代
------------------
网络动态着色层支持
------------------
   BOOL Is_Net_Color_Vector()
	     函数: 图元可以作为着色层矢量图元吗？
   BOOL Is_Net_Color_Bmp()
	     函数: 图元可以作为着色层位图吗？
	 BOOL Is_Net_Color_Power()
	     函数: 图元可以作为着色层电源吗？
	 BOOL Is_Net_Color_Yx();
	     函数: 图元可以作为着色层开关吗？

	 BOOL Is_In_Net_Color_Power();
			 函数: 判断该图元是否已是网络层电源
	 BOOL Is_In_Net_Color_Yx();
	     函数: 判断该图元是否已是着色层开关？
   BOOL Is_In_Net_Color_Bmp()
	     函数: 判断该图元是否已是着色层位图吗？
   BOOL Is_In_Net_Color_Vector()
	     函数: 判断该图元是否已是着色层矢量图吗？


------------------------
多个图元编辑的支持
------------------------
	virtual void Set_Ty_Color(COLORREF color,CSp_drawView * pView=NULL);
	     函数:设置图元的颜色

	virtual void Set_Line_Width(int width,CSp_drawView * pView=NULL);
	     函数:设置线条的宽度

  void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	     函数:设置字体信息

--------------------------
画面按摸板生成
--------------------------
	virtual void ParamReplace(CStringList& ruleList);
	函数:图元参数按规则替换
	替换方法 第一个字符，'0'-普通字符替换，1'-按英文名,第一个字符起, '2'-节点,线路,单元,点号

	int GetEname_From_ReplacRule(CString vDotName,CString& vDstDotName, CStringList& ruleList);
	函数:把英文名代入替换规则获取新的英文名
	int GetDot_From_ReplaceRule(BYTE srcNode, BYTE srcLine, BYTE srcRtu, WORD srcDot, BYTE& dstNode, BYTE& dstLine, BYTE& dstRtu, WORD& dstDot, CStringList& ruleList);
	函数:把点号代入替换规则获取新的点号
	int GetStr_From_ReplacRule(CString srcStr,CString& dstStr, CStringList& ruleList);
	函数:把字符串代入替换规则获取新的字符串


*/



class CHMLayer;

#define MAXLINKTY_COUNT  8    //每一个图元最多拥有的链接图元的数目


class CTyBase : public CObject  
{
	DECLARE_SERIAL(CTyBase);

public:
	CHMLayer *m_pHMLayer;				//指向自己所在的画面层对象
	int		   m_Id;								    //图形元素唯一的识别号
	int		   m_iNumber;						//图元位置编号
	BOOL	   m_bDelete;						//是否删除
	DWORD  m_dwFlag;						//图元标志
														
	CLinkObjectArray m_LinkArr;          //链接对象列表

public:
	CTyBase (int vId = 0,CHMLayer* pLayer = NULL);
	int GetID();

	virtual void GetRect(float *minX,float *minY,float *maxX,float *maxY);

public:
	virtual void Set_Ty_Is_Fill(BOOL bFill,CElecMapView * pView=NULL);
	virtual void Set_Ty_Fill_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void Set_Ty_Color(COLORREF color,CElecMapView * pView=NULL);
	virtual void Set_Line_Width(int width,CElecMapView * pView=NULL);
	virtual void SetFontProp(float height,float width,float jqwidth,BOOL pl,COLORREF color,char font[]);
	
	//网络动态着色层支持
	BOOL Is_Net_Color_Power();
	virtual BOOL Is_Net_Color_Yx();
	virtual BOOL Is_Net_Color_Bmp();
	virtual BOOL Is_Net_Color_Vector();

	BOOL Is_In_Net_Color_Vector();
	BOOL Is_In_Net_Color_Bmp();
	BOOL Is_In_Net_Color_Yx();
	BOOL Is_In_Net_Color_Power();

	//矢量图管理
	void UnRegisterVector(LPCSTR vectorname);
	void RegisterVector(LPCSTR vectorname);
	void DrawVector(LPCSTR vectorname,CDC* pDC,CRect viewRect,CRect clipRect,COLORREF color,BOOL blReplace=TRUE);
	static CSize Get_Size_Vector(LPCSTR vectorname);
	virtual void DrawRect(CDC* pDC,int orgX,int orgY,float offsetX, float offsetY,float scaleX,float scaleY,COLORREF ReplaceColor,COLORREF ToColor,BOOL bReplace=TRUE);

	//图象管理
	void DrawImage(LPCSTR bmpname,CDC* pDC, int x0, int y0, int nWidth, int nHeight,BOOL bTranslate,COLORREF color,BOOL blHScale=TRUE,BOOL blVScale=TRUE);
	void UnRegisterImage(LPCSTR bmpname);
	void RegisterImage(LPCSTR bmpname);

	//链接对象
	BOOL ExecuteLinkObject(int pos1,int pos2);
	BOOL OnEditLinkObjectProperties(int pos);
	void DeleteLinkObject(int pos);
	void AddLinkObject(CLinkObjectBase* pLink);
	int Get_LinkTy_Count();
	CString GetLinkObjectName(int pos);

	//图元修改通知
	void SetModifiedFlag(BOOL bl=TRUE);

	//图元类型
	static CTyBase* CreateTy(DRAW_TY tyStyle);
	virtual DRAW_TY GetTyType();

	//把手
	void MoveHandleTo(int nHandle, PointStruct point, CElecMapView* pView,UINT nFlags=0);

	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags);
	virtual PointStruct GetHandle(int nHandle);
	virtual int GetHandleCount();
	CRect GetHandleLogRect(int nHandleID, CElecMapView* pView);
	
	//图元自身的运动
	void Move(float xLen,float yLen,CElecMapView * pView=NULL);

	virtual void Ty_Move(float xLen, float yLen,CElecMapView * pView);
	virtual void OnEditProperties(CElecMapView * pView=NULL);
	virtual CTyBase* Clone();

	//绘图
	void Invalidate();
	virtual void DrawDC(CDC *pDC, CElecMapView *pView);
	enum TrackerState { normal, selected, active };
	virtual void DrawTracker(CDC* pDC, CElecMapView *pView,TrackerState state);

	//存盘
	void Save(CFile* file,BOOL Yn);
	virtual void Ty_Save(CFile *file, BOOL Yn);
	void LinkObjects_Save(CFile *file, BOOL Yn);
	void Serialize(CArchive& ar);
	virtual void Ty_Serialize(CArchive & ar);
	void LinkObjects_Serialize(CArchive &ar);

	// 画面自动生成
	void ParamReplace(CStringList& ruleList);
	void LinkObjects_ParamReplace(CStringList& ruleList);
	virtual void ParamReplace_Ty(CStringList& ruleList);
	static int GetEname_From_ReplacRule(CString vDotName,CString& vDstDotName, CStringList& ruleList);
	static int GetDot_From_ReplaceRule(BYTE srcNode, BYTE srcLine, BYTE srcRtu, WORD srcDot, BYTE& dstNode, BYTE& dstLine, BYTE& dstRtu, WORD& dstDot, CStringList& ruleList);
	static int GetStr_From_ReplacRule(CString srcStr,CString& dstStr, CStringList& ruleList);

	//判断和计算
	virtual BOOL PointInObj(float x,float y,float errRange);
	int HitTest(PointStruct point, CElecMapView* pView, BOOL bSelected);
	virtual BOOL IntersectRect(float x0, float y0, float x1, float y1);
	static BOOL  PointRgn(float x,float y,int Numble,PointStruct *PointList,float blc);
	static float CalDisp(float x1,float y1,float x2,float y2);
	static float PointLine(float xx,float yy,float x1,float y1,float x2,float y2);
	static BOOL PointInEllipse(float x,float y,RectStruct rt);
	static BOOL PointInRect(float x,float y,RectStruct rt);
	static BOOL IntersectRect2(RectStruct rt1,RectStruct rt2);
	static BOOL URectInURect(RectStruct rt1,RectStruct rt2);

	#ifdef _DEBUG
		void ASSERTValid();
	#endif
	virtual ~CTyBase();

protected:
	friend class CHMLayer;

};

typedef CTypedPtrList<CObList, CTyBase*> CTyBaseList;


#endif // !defined(AFX_TYBASE_H__72E869AE_6422_44B5_9640_628F3F263EC2__INCLUDED_)
