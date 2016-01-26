// HMProject.h: interface for the CHMProject class.
//
//	void Save(void);
//		函数：	保存画面工程定义
//	void Load(void);
//		函数：	装载画面工程定义
//	LPCSTR GetMainHMName(void);	
//		函数：	获取主画面名称
//	int SetMainHMName(LPCSTR strname);
//		函数：	设置主画面名称
//		返回		>=0 主画面名称在画面队列中的序号	<0 该画面不存在
//	BOOL DeleteHM(LPCSTR strname);
//		函数：	删除画面
//	BOOL SaveHM(LPCSTR hmname);
//		函数：	保存画面 hmname
//	BOOL OpenHM(LPCSTR strname,LPCSTR layername=NULL);
//		函数：	打开画面strname的画面层layername
//	void UpdateAllHMLayer(void);
//		函数：	更新树形控件中所有最新的画面层信息
//	void ActiveHM(LPCSTR strname);
//		函数：	激活画面strname
//	BOOL ModifyHMprop(LPCSTR strname,tagHMID* pHMID);
//		函数：	修改画面strname属性
// 	void AddHMLayer(LPCSTR strName,tagHMLayerID*pLayer);
//		函数：	增加画面strname的画面层
//	void DeleteHMLayer(LPCSTR pHM,LPCSTR pLayer);
//		函数：	删除画面strname的画面层pLayer
//	int FindHMLayerID(LPCSTR pHM,LPCSTR pLayer);
//		函数：	查找画面strname中的画面层pLayer并返回在该画面中的位置
//	void SetHMLayer(LPCSTR pHM,LPCSTR pLayer, tagHMLayerID *pLayerID);
//		函数：	修改画面strname的画面层pLayer的内容
//	tagHMLayerID * GetHMLayerID(LPCSTR pHM,LPCSTR pLayer);
//		函数：	获取画面strname中的画面层pLayer的内容
//  void MovePre(LPCSTR hmname,LPCSTR layername=NULL)
//		函数：	画面hmname或画面层layername向上移动一行
//  void MoveNext(LPCSTR hmname,LPCSTR layername=NULL)
//		函数：	画面hmname或画面层layername向下移动一行
//	BOOL Get_HmLayer_Names(LPCSTR hmname,CStringList& layerList);
//		函数：	获取画面hmname的所有画面层名称


//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMPROJECT_H__1F81F7E1_0762_43B4_879B_0DA4F3621537__INCLUDED_)
#define AFX_HMPROJECT_H__1F81F7E1_0762_43B4_879B_0DA4F3621537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <hash_map>
#include <map>
using namespace std;
using namespace stdext;
#include "gloable/DataStyle.h"
class CHuaMian;
class CWorkspaceBar;
class CTyHMProxyProp;

class CHMProject : public CObject  
{
private:
	//测试字符串是否是字段strName
	BOOL HitTestSectionName(LPCSTR strSection,LPCSTR strName);
	//清空画面信息集
	void InitMemory(void);
	//测试str是否是段落名
	BOOL HitTestSectionID(LPCSTR str);
	//读取主画面信息
	BOOL ReadMainHuaMian(CStdioFile* pfile);
	//读取strSection段络中strKey关健词包含的文本信息
	BOOL ReadString(CStdioFile *pfile,LPCSTR strSection,LPCSTR strKey, LPSTR buffer,int bufsize);
	//把读指针移到strSection段落起始处
	BOOL SeekSection(CStdioFile *pfile,LPCSTR strSection);
	//读取系统中包含的所有画面信息
	BOOL ReadHuaMianList(CStdioFile* pfile);
	//保存系统中包含的所有画面信息
	BOOL WriteHuaMianList(CStdioFile* pfile);
	//保存主画面信息
	BOOL WriteProjectSection(CStdioFile *pfile);

protected:
	BOOL m_modifyflag;
	BOOL ReadHuaMianFileHead(LPCSTR filepath,tagHMID* pHMID);
	BOOL CloseHM(LPCSTR strname);
	//读取各个画面的画面层信息
	BOOL ReadHMLayer(void);
	
	BOOL SaveFile(LPCSTR filepath);
	BOOL LoadFile(LPCSTR filepath);
	tagHMIDList* GetHuaMianNameList(void);
	tagHMIDList m_HMIDList;				//系统包含的画面信息集
	CString  m_strMainHM;					//主画面名称
	
public:
	friend class CWorkspaceBar;
	friend class CTyHMProxyProp;

public:
	//测试该画面是否已经打开
	BOOL HitTestOnline(LPCSTR strname);
	BOOL Get_HmLayer_Names(LPCSTR hmname,CStringArray& layerList);
	void SaveHM(LPCSTR hmname);
	void MoveNext(LPCSTR hmname,LPCSTR layername=NULL);
	void MovePre(LPCSTR hmname,LPCSTR layername=NULL);
	tagHMLayerID * GetHMLayerID(LPCSTR pHM,LPCSTR pLayer);
	void DeleteHMLayer(LPCSTR pHM,LPCSTR pLayer);
	int FindHMLayerID(LPCSTR pHM,LPCSTR pLayer);
	void SetHMLayer(LPCSTR pHM,LPCSTR pLayer, tagHMLayerID *pLayerID);
	void AddHMLayer(LPCSTR strName,tagHMLayerID*pLayer);
	BOOL IsModified();

	//修改画面strname属性
	BOOL ModifyHMprop(LPCSTR strname,tagHMID* pHMID);
	tagHMID * FindHMID(LPCSTR strname);
	BOOL AddHuaMian(tagHMID *pHMID);
	void SetModifiedFlag(BOOL bModified=TRUE);
	void UpdateAllHMLayer(void);
	void ActiveHM(LPCSTR strname);
	BOOL DeleteHM(LPCSTR strname);
	BOOL OpenHM(BOOL changeScale,LPCSTR strname,LPCSTR layername=NULL);
	CHuaMian * GetHMObject(LPCSTR strname);
	CHuaMian*  GetHMObjectByID(tagHMID* pHMID);
	int SetMainHMName(LPCSTR strname);
	LPCSTR GetMainHMName(void);
	void Save(void);
	void Load(void);

	// 获取系统正在编辑的画面的数目
	int GetOpenHmCount(void);
	CString m_ComputerName;
	CString GetActiveComputer();


private:
	CHMProject();
	CHMProject(const CHMProject&);
	CHMProject& operator= (const CHMProject&);
	virtual ~CHMProject();
	static CHMProject* m_pInstance;
	class CReleaseObj
	{
	public:
		~CReleaseObj()
		{
			if (CHMProject::getInstance())
				delete CHMProject::m_pInstance;
		}
	};
	static CReleaseObj obj;
	std::map<CString,CHuaMian*> map_IDHuamian;
	CHuaMian* m_pCurActiveHM;
public:
	// 获取工程实例
	static CHMProject* getInstance();
	CHuaMian* getCurActiveHuamianInstance();
	void      setCurActiveHuamianInstance(CHuaMian* pHMInstance);

	//@param:  pRealData   struct pointer
	//		   nCount      data count
	BOOL	updateEntityInAHuamian( char *pRealData, int nCount );
};

//extern CHMProject G_HMProject;		//画面工程对象

#endif // !defined(AFX_HMPROJECT_H__1F81F7E1_0762_43B4_879B_0DA4F3621537__INCLUDED_)
