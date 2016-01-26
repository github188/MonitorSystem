// LinkObjectBase.h: interface for the CLinkObjectBase class.
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKOBJECTBASE_H__5A42B2E7_1E6C_416B_B8CF_875F29FF3C96__INCLUDED_)
#define AFX_LINKOBJECTBASE_H__5A42B2E7_1E6C_416B_B8CF_875F29FF3C96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*---------------------------------------------------------------------------------
	*								动态链接
	*--------------------------------------------------------------------------------

------------------
链接对象类型的管理
------------------	

		CLinkObjectBase * FindLinkObject(int linktype)
		函数:找到对应于链接类型linktype的链接类型对象
		参数:
			linktype-链接类型
		返回:指向链接类型对象的指针
	 

------------------
链接类型数据文档
------------------	

		PLINKKEY CLinkObjectBase::MakeOutKey()
		函数:制造链接类型数据文档
		返回一个初始化后的链接类型数据文档

		BOOL OnEditProperties(PLINKKEY pKey)
		函数:编辑一个链接类型数据文档
		参数:
			pKey-指向链接类型数据文档的指针
		返回：链接类型数据文档是否被修改

------------
执行数据链接
------------

		void Jump(HWND hwnd,PLINKKEY pKey)
		函数:跳转到链接文档执行的界面
		参数:
			pKey-指向链接类型数据文档的指针
			hwnd-界面的父窗口句炳


*/

//动态链接对象基类
class CTyBase;
class CLinkObjectBase : public CObject  
{
	DECLARE_SERIAL(CLinkObjectBase);
public:
	CLinkObjectBase(){m_pTy = NULL;}

public:
	CLinkObjectBase(int linktype,CString strname);
	virtual ~CLinkObjectBase();
protected:

public:
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	virtual CLinkObjectBase * Clone();
	void SetModifiedFlag(BOOL bl);
	void Save(CFile *file, BOOL Yn);
	void Serialize(CArchive &ar);
	virtual void Execute(UINT pos);
	virtual BOOL OnEditProperties();
	void ParamReplace(CStringList& ruleList);
	virtual void ParamReplace_Ty(CStringList& ruleList);

	CTyBase* m_pTy;		
	int m_key;				//链接对象的类型
	char m_name[33];	//链接说明  wkey+name=唯一
};


typedef CTypedPtrArray<CObArray, CLinkObjectBase*> CLinkObjectArray;


#endif // !defined(AFX_LINKOBJECTBASE_H__5A42B2E7_1E6C_416B_B8CF_875F29FF3C96__INCLUDED_)
