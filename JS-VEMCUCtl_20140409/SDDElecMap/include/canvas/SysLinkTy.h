// SysLinkTy.h: interface for the CLinkYk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSLINKTY_H__9CCA4309_286E_4981_BEF4_03EBCFAEED79__INCLUDED_)
#define AFX_SYSLINKTY_H__9CCA4309_286E_4981_BEF4_03EBCFAEED79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "canvas/LinkObjectBase.h"

#pragma pack(push,before_tagBSItem,1)

//闭锁条件项
struct tagBSItem
{
  char strYxName[17]; //闭锁遥信名称
	BYTE bValue;				//闭锁条件成立值
};

#pragma pack(pop,before_tagBSItem,8)


typedef CTypedPtrArray<CPtrArray,tagBSItem*> tagBS_Array;

//遥控图元
class CLinkYk : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkYk);
public:
	CLinkYk();
private:
	char m_Ykname[17];					//该遥控点英文名称
	BOOL m_bZK;									//TRUE-直控 FALSE-选控
	char m_Yxname_Hang[17];			//摘/挂牌遥信反馈点
	char m_Yxname_FX_Close[17];	//遥控合返校点
	char m_Yxname_FX_Open[17];	//遥控分返校点
	char m_Yxname_Yk[17];				//遥控反馈点
	tagBS_Array m_BS_Array;			//闭锁条件集合


	bool m_bWatchMan;						//开关是否采用监护人监护控制方式
	bool m_bGroup;							//开关是否采用分组控制方式
	DWORD m_dwFlag;							//开关允许那些组控制

	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkYk(CString strname,bool bWatchMan,bool bGroup,DWORD dwFlag);
	virtual ~CLinkYk();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};


//直控图元
class CLinkZk : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkZk);
public:
	CLinkZk();
private:
	int m_node;
	int m_line;
	int m_rtu;
	char m_Yxname_Hang[17];			//摘/挂牌遥信反馈点
	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkZk(CString strname,bool bWatchMan,bool bGroup,DWORD dwFlag);
	virtual ~CLinkZk();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

//网络动态着色电源
class CNetColorPower : public CLinkObjectBase  
{
	DECLARE_SERIAL(CNetColorPower);
public:
	CNetColorPower();
private:
	char m_Ycname[17];					//该遥控点英文名称
	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	virtual ~CNetColorPower();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

//网络动态着色矢量图元
class CNetColorVector : public CLinkObjectBase  
{
	DECLARE_SERIAL(CNetColorVector);
public:
	CNetColorVector();
private:
	COLORREF m_color;					//失电颜色
	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	virtual ~CNetColorVector();
};


//网络动态着色位图图元
class CNetColorBitmap : public CLinkObjectBase  
{
	DECLARE_SERIAL(CNetColorBitmap);
public:
	CNetColorBitmap();
private:
	char m_BmpName[33];					//失电位图名
	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	virtual ~CNetColorBitmap();
};

//网络动态着色开关图元
class CNetColorYx : public CLinkObjectBase  
{
	DECLARE_SERIAL(CNetColorYx);
public:
	CNetColorYx();
private:

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	virtual ~CNetColorYx();
};


class CLinkYT : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkYT);
public:
	CLinkYT();
private:
	char m_YTname[17];					//该遥调点英文名称
	int  m_dataType;					  //数据类型 0-12位 非0-16位
	char m_Yxname_Yk[17];				//遥控反馈点
	tagBS_Array m_BS_Array;			//闭锁条件集合

	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkYT(CString strname);
	virtual ~CLinkYT();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};


class CLinkYZTY : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkYZTY);
public:
	CLinkYZTY();
private:
	char m_YZTYname_Up[17];			//该有载调压升压点英文名称
	char m_YZTYname_Down[17];		//该有载调压降压点英文名称
	char m_YZTYname_Stop[17];		//该有载调压急停点英文名称
	int m_ActionType_Up;				//该有载调压升压点动作方式
	int m_ActionType_Down;			//该有载调压降压点动作方式
	int m_ActionType_Stop;			//该有载调压急停点动作方式
	char m_Yxname_Yk[17];				//有载调压反馈点

	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkYZTY(CString strname);
	virtual ~CLinkYZTY();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

class CLinkUnit : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkUnit);
public:
	CLinkUnit();
private:
	int m_Fun;									//保护投切(0)、Soe查询(1)、yc查询(2)、yc越限查询(3)、
															//电度量查询(4)、录波曲线(5)、定值整定(6)、参数(7)
															//PT/CT变比(8)
	void InitMemory();

public:
	char m_Unitname[17];				//该单元点英文名称
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkUnit(CString strname);
	virtual ~CLinkUnit();
	virtual void ParamReplace_Ty(CStringList& ruleList);
};


class CLinkNode : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkNode);
public:
	CLinkNode();
private:
	int m_Fun;									//Soe查询(0)、yc查询(1)、yc越限查询(2)、
															//电度量查询(3)
															
	void InitMemory();

public:
	char m_Nodename[17];				//该单元点英文名称
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkNode(CString strname);
	virtual ~CLinkNode();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

class CLinkLine : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkLine);
public:
	CLinkLine();
private:
	int m_Fun;									//Soe查询(0)、yc查询(1)、yc越限查询(2)、
															//电度量查询(3)
															
	void InitMemory();

public:
	char m_Linename[17];				//该单元点英文名称
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkLine(CString strname);
	virtual ~CLinkLine();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};


typedef struct 
{              
	//BYTE	udflag ;//方向标志 0:图元->modem  1:modem->图元
	DWORD	adptid ;//适配器id
	char	telno[12] ;//电话号码
	//BYTE	cmdid ;//命令id  0:拨号  1:切换成自动轮询状态
	DWORD	dotime ;//拨号时间（单位秒） 0:代表无时间限制
}ModemManualDailEvent ;
//Modem手动拨号图元
class CLinkModem : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkModem) ;
public:
	virtual BOOL OnEditProperties();
	CLinkModem() ;
	virtual ~CLinkModem() ;
private:
	//拨号图元
	CString m_strname ;//控制说明
	ModemManualDailEvent m_modem ;
	void InitMemory();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	
	virtual void Serialize_Ty(CArchive &ar);

};


class CLinkYT2 : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkYT2);
public:
	CLinkYT2();
private:
	char m_YTname[17];					//该遥调点英文名称
	int  m_dataType;					  //数据类型 0-12位 非0-16位
	char m_Yxname_Yk[17];				//遥控反馈点
	tagBS_Array m_BS_Array;			//闭锁条件集合
	float m_fPianCha;               //偏差值
	int m_nXiaoShu;                 //小数位数
	int m_nBiLi;                    //比例

	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkYT2(CString strname);
	virtual ~CLinkYT2();
	virtual void ParamReplace_Ty(CStringList& ruleList);


};

////////////////////////////////////////////////////
//2004.3.29
//pang增加遥信量邦定连接图元
class CLinkYXBind : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkYXBind) ;
public:
	CLinkYXBind() ;
	virtual ~CLinkYXBind() ;
public:
	virtual BOOL OnEditProperties();
	char m_Ename[17] ;//绑定的英文名
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

////////////////////////////////////////////////////
//增加监控模块连接图元
class CLinkYSHBind : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkYSHBind) ;
public:
	CLinkYSHBind() ;
	virtual ~CLinkYSHBind() ;
public:
	virtual BOOL OnEditProperties();
	char m_Ename[17] ;//绑定的英文名
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	virtual void ParamReplace_Ty(CStringList& ruleList);

};


//////////////////////////////////////////////////////////
//2004.3.30
//pang增加群控链接图元
class CLinkQK : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkQK) ;
public:
	CLinkQK() ;
	virtual ~CLinkQK() ;
public:
	virtual BOOL OnEditProperties();
	char m_Ename[17] ;//绑定的组英文名

	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);

private:
	bool m_bWatchMan;						//开关是否采用监护人监护控制方式
	bool m_bGroup;							//开关是否采用分组控制方式
	DWORD m_dwFlag;							//开关允许那些组控制
};

///////////////////////////////////////////////////////////////
//2004.4.3
//pang增加后台检修牌
class CLinkSoftRepair : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkSoftRepair) ;
public:
	CLinkSoftRepair() ;
	virtual ~CLinkSoftRepair() ;
public:
	virtual BOOL OnEditProperties();
	char m_Ename[17] ;//绑定的英文名
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	virtual void ParamReplace_Ty(CStringList& ruleList);


} ;
///////////////////////////////////////////////////////////////
//2004.4.3
//pang增加单元扩展
class CLinkUnitExtend : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkUnitExtend) ;
public:
	CLinkUnitExtend() ;
private:
	char m_Unitname[17] ;
	int m_nFun ;
	void InitMemory() ;
public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkUnitExtend(CString strname);
	virtual ~CLinkUnitExtend();
	virtual void ParamReplace_Ty(CStringList& ruleList);

} ;

/////////////////////////////////////////////////////////////////
//2004.06.05
//pang增加馈线开关链接图元
class CLinkKX : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkKX) ;
public:
	CLinkKX() ;
	virtual ~CLinkKX() ;
public:
	virtual BOOL OnEditProperties();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
private:
	char m_Ename[4] ;
	char m_Cname[13] ;
	char m_kgEname[17] ;
	virtual void ParamReplace_Ty(CStringList& ruleList);

} ;

/////////////////////////////////////////////////////////////////
//2004.06.05
//pang增加馈线区段链接图元
class CLinkKXSection : public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkKXSection) ;
public:
	CLinkKXSection() ;
	virtual ~CLinkKXSection() ;
public:
	virtual BOOL OnEditProperties();
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
private:
	char m_ename[4] ;
	char m_cname[13] ;
	char m_qdEname[4] ;
	char m_qdCname[13] ;
	COLORREF m_col1;
	COLORREF m_col2;
	char m_bmpName1[33] ;
	char m_bmpName2[33] ;
} ;

////////////////////////////////////////////////////////////////
//2004.08.03
//st增加水行业控制链接图元
class CLinkDoubleCtrl : public CLinkObjectBase  
{
	DECLARE_SERIAL(CLinkDoubleCtrl);
public:
	CLinkDoubleCtrl();
private:
	char m_OprtName1[11];
	char m_OprtName2[11];
	char m_YKName1[18];
	char m_YKName2[18];
	int m_nActType1;
	int m_nActType2;
	bool m_bOnceQuit;

	void InitMemory();

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);
	CLinkDoubleCtrl(CString strname);
	virtual ~CLinkDoubleCtrl();
	virtual void ParamReplace_Ty(CStringList& ruleList);

};

#endif // !defined(AFX_SYSLINKTY_H__9CCA4309_286E_4981_BEF4_03EBCFAEED79__INCLUDED_)
