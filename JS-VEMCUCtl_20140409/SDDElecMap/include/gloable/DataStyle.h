#if !defined(DATASTYLE_H)
#define DATASTYLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*---------------------------------------------------------------------------------
	*								DRAWTYPE 工具箱的汇总
	*--------------------------------------------------------------------------------

  drwNULL		空箱,
	drwJH			简单几何图形工具箱,
	drwDH			动画工具箱,
	drwTK			矢量图/位图工具箱,
	drwSZ			数值图元工具箱,
	drwSB			设备图元工具箱,
	drwQX			曲线工具箱,
	drwKZ			控制图元工具箱,
	drwZZ			装置图元工具箱,
	drwQT			系统其它类工具箱
	drwNetColor网络动态着色工具箱
	drwINTF   应用接口工具箱
-----------------------------------------------------------------------------------
*/
typedef enum
{
  drwNULL=0,drwJH, drwDH,drwTK,drwSZ,drwSB,drwQX,drwKZ,drwZZ,drwQT,drwNetColor,drwINTF,drwSTAS
} DRAWTYPE;

#define HMC_MAXNUM		8		//画面层最大数目
#define MAX_QGMROW		50
#define MAX_QGMCOL		50

#pragma pack(push,before_tagHuaMianFileHead,1)

//画面层标识
struct tagHMLayerID
{
	int       iLayerNo;						//层序号
	char    strLayerName[33];		//画面层中文名
	float    fScale;							//默认显示比例
	float	   fStartScale;				    //显示起始范围
	float    fEndScale;					    //显示终止范围
	int       iOffset;						    //画面层距离文件起始处的偏移量
	int	   iStartNumber;				//图元最后面的位置编号
	int	   iEndNumber;					//图元最前面的位置编号	
	int	   IDDef;							//图元标识的起始值
	BOOL  bHide;							//是否隐藏显示

	const tagHMLayerID& operator =( const tagHMLayerID& srcHMLayerID);

};

typedef struct tagHMLayerID  tagHMLayerBase;

//画面文件头简要信息
typedef struct 
{
	WORD	   wPicFlag;									//文件特定标志
	WORD	   wMVersion;								//文件高版本号
	WORD     wEVersion;								//文件低版本号
	DWORD  dwPICStyle;								//画面风格
	WORD	   wPicWidth;								//图幅宽度
	WORD	   wPicHeight;								//图幅高度
	WORD	   wResolution;							    //屏幕分辨率的宽度
	WORD	   hResolution;							    //屏幕分辨率的高度
	float        fDefaultScale;						    //默认的画面显示比例
	COLORREF dwBackColor;						//背景颜色
	DWORD  dwBackImageNo;						//背景位图编号
}tagHuaMianBase;

//画面文件头信息
typedef struct 
{
	tagHuaMianBase recBase;					//画面文件头简要信息
	BYTE                 	bLayerNum;				//画面层实际数目
	int                        iLayerIdDef;				//画面层的起始预定义号
	tagHMLayerBase arrLayer[16];		    //画面层简要信息组
	DWORD	            dwEpdYxListOffset;	//报警遥信距离文件起始处的偏移量
	DWORD               dwEpdYcListOffset;	//报警遥测距离文件起始处的偏移量	
}tagHuaMianFileHead;	

#pragma pack(pop,before_tagHuaMianFileHead,8)

//画面标识
struct tagHMID
{
	char        strHMName[33];		//画面中文名
	DWORD dwflag;						//画面标志
	DWORD dwStyle;						//画面风格
	WORD   wWidth;						//画面宽度
	WORD   wHeight;						//画面高度
	int          iLayerIdDef;				//画面层的起始预定义号
	WORD   wLayerNum;				//包含的画面层数目
	float      fScale;							//画面默认显示比例
	tagHMLayerID ArrHMLayer[HMC_MAXNUM];//包含的画面层信息

public:
	void Exchange(int i,int j);
	int    FindHMLayerID(LPCSTR strname);
	void DeleteHMLayer(LPCSTR strname);
	void SetHMLayer(LPCSTR strname,tagHMLayerID* pLayer);
	void AddHMLayer(tagHMLayerID*pLayer);
	const tagHMID& operator =( const tagHMID& srcHMID);
};


//画面标识信息集
typedef CTypedPtrList<CPtrList, tagHMID*> tagHMIDList;


#endif 
