#pragma once
#include "linkobjectbase.h"

class CLinkFile :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CLinkFile);
public:
	CLinkFile(void);
	
private:
//	int m_Fun;									
	void InitMemory();
               
public:
	char m_Filename[33];				//该单元点英文名称
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual BOOL OnEditProperties();
	virtual void Serialize_Ty(CArchive &ar);

	CLinkFile (CString strname);
	virtual ~CLinkFile();
};
