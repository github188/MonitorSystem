#pragma once
#include "canvas/linkobjectbase.h"

class CSZ_SbLookLink :
	public CLinkObjectBase
{
	DECLARE_SERIAL(CSZ_SbLookLink);
public:
	CSZ_SbLookLink(void);
	~CSZ_SbLookLink(void);
	CSZ_SbLookLink(CString strname);
	virtual void Save_Ty(CFile* file, BOOL Yn);
	virtual void Serialize_Ty(CArchive& ar);
	virtual BOOL OnEditProperties(void);
	virtual void ParamReplace_Ty(CStringList& ruleList);
private:
	char m_sbbh[17];
};
