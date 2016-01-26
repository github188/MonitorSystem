// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/Vector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD CVector::m_version=0x1;
CVectorList g_VectorList;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector(LPCSTR filepath)
{
	ClearAll();
	Load(filepath);
}

CVector::~CVector()
{
	ClearAll();
}

void CVector::Load(LPCSTR filepath)
{
	ClearAll();

	CFile f;
	CFileException e;
	if (strlen(filepath)<=0) return;


	if(!f.Open(filepath,CFile::modeRead|CFile::typeBinary,&e))
	{
		#ifdef _DEBUG
			afxDump << "文件打开失败! " << e.m_cause << "\n";
		#endif
			return;
	}

	try
	{
		LoadFromStream(&f);
		Calculate_Bound_Rect();
	}
	catch (CFileException* err)
	{
		CString errstr;
		errstr.Format("文件读过程中发生异常!,%s,异常码:%d",filepath,err->m_cause);
		#ifdef _DEBUG
			afxDump << errstr<< "\n";
		#endif
		err->Delete();
		return;
	}
}

void CVector::LoadFromStream(CFile *file)
{
	DWORD version;
	int count;
	ASSERT(file!=NULL);
	if (file==NULL) return;

	file->Read((unsigned char *)&version,sizeof(version));
	ASSERT(version<=m_version);
	if (version>m_version) return;

	file->Read((unsigned char *)&m_replaceColor,sizeof(m_replaceColor));
	
	file->Read((unsigned char *)&m_x0,sizeof(float));
	file->Read((unsigned char *)&m_y0,sizeof(float));
	file->Read((unsigned char *)&m_x1,sizeof(float));
	file->Read((unsigned char *)&m_y1,sizeof(float));

	file->Read((unsigned char *)&count,sizeof(count));
	
	DRAW_TY tyType;
	CTyBase* pTy;
  
	float x0,x1,y0,y1;
	for (int i=0;i<count;i++)
	{
		file->Read((unsigned char *)&tyType,sizeof(tyType));
		ASSERT(tyType>0);
		if (tyType<=0) break;
		pTy=CTyBase::CreateTy(tyType);
		ASSERT(pTy!=NULL);
		if (pTy==NULL) break;
		pTy->Save(file,FALSE);
		
		//删除超出范围的图元
		pTy->GetRect(&x0,&y0,&x1,&y1);
		if ((x0<m_x0)||(y0<m_y0)||(x1>m_x1)||(y1>m_y1))
			delete pTy;
		else
			m_list.AddTail(pTy);		
	}
}

void CVector::Draw(CDC *pDC, CRect viewRect, CRect clipRect,COLORREF color,BOOL blReplace)
{
	CSize sz=GetSize();
	if ((sz.cx==0)||(sz.cy==0)) return;
	if ((viewRect.Width()==0)||(viewRect.Height()==0)) return;
	float scaleX=(float)(((double)viewRect.Width())/(double)(m_x1-m_x0));
	float scaleY=(float)(((double)viewRect.Height())/(double)(m_y1-m_y0));

	CTyBase * pTy;
	POSITION pos=m_list.GetTailPosition();
	while (pos!=NULL)
	{
	  pTy=m_list.GetPrev(pos);
		ASSERT(pTy!=NULL);
		if (pTy==NULL) break;
		pTy->DrawRect(pDC,viewRect.left,viewRect.top,m_x0,m_y0,scaleX,scaleY,m_replaceColor,color,blReplace);
	}
}

void CVector::ClearAll()
{
	m_x0=0;
	m_y0=0;
	m_x1=0;
	m_y1=0;

	CTyBase * pTy;
	POSITION pos=m_list.GetHeadPosition();
	while (pos!=NULL)
	{
		pTy=m_list.GetNext(pos);
		ASSERT(pTy!=NULL);
		if (pTy==NULL) break;
		delete pTy;
	}
	m_list.RemoveAll();
}

void CVector::Calculate_Bound_Rect()
{

}

CSize CVector::GetSize()
{
	CSize sz;
	sz.cx=(int)(m_x1-m_x0+0.99);
	sz.cy=(int)(m_y1-m_y0+0.99);

	return sz;
}


void CVectorList::ClearAll()
{
	CVector* pVector;
	CString str;
	if (!Lock()) return;
	POSITION pos=m_map_vector.GetStartPosition();
	while (pos!=NULL)
	{
		pVector=NULL;
		m_map_vector.GetNextAssoc(pos,str,pVector);
		ASSERT(pVector!=NULL);
		if (pVector==NULL) continue;
		delete pVector;
	}
	Unlock();
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVectorList::CVectorList()
{
	m_map_vector.RemoveAll();
	m_map_count.RemoveAll();
}

CVectorList::~CVectorList()
{

}

BOOL CVectorList::Lock()
{
	return m_mutex.Lock();
}

void CVectorList::Unlock()
{
	m_mutex.Unlock();
}

void CVectorList::Register_Vector(LPCSTR vecName)
{
	CVector* pVector;
	void* p;

	CString filepath;
	char chpath[MAX_PATH];  
	GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
	filepath.Format("%sglobal\\vec\\%s.vec",chpath,vecName);//theApp.GetDirectory()

	if (!Lock()) return;
	if (m_map_vector.Lookup(vecName,pVector))
	{
		if (m_map_count.Lookup(vecName,p))
		{
			WORD i=(WORD)((DWORD)(p));
			i++;
			m_map_count[vecName]=(void *)i;
		}
		else 
			ASSERT(FALSE);
	}
	else
	{
		pVector=new CVector(filepath);
		ASSERT(pVector!=NULL);
		if (pVector!=NULL) 
		{
			m_map_vector[vecName]=pVector;
			WORD i=1;
			m_map_count[vecName]=(void *)i;
		}
	}
	Unlock();
}

void CVectorList::UnRegister_Vector(LPCSTR vecName)
{

	CVector* pVector;
	void* p;

	if (!Lock()) return;

	if (m_map_vector.Lookup(vecName,pVector))
	{
		if (m_map_count.Lookup(vecName,p))
		{
			DWORD i=(DWORD)p;
			if (i<=1)
			{
			  delete pVector;
				m_map_vector.RemoveKey(vecName);
				m_map_count.RemoveKey(vecName);
			}
			else 
			{
				i--;
				m_map_count[vecName]=(void *)i;
			}
		}
		else
		{
			ASSERT(FALSE);
			delete pVector;
			m_map_vector.RemoveKey(vecName);
		}
	}
	Unlock();
}

CSize CVectorList::Get_Vector_Size(LPCSTR vecName)
{
	CSize ret(0,0);
	CVector* pVector;
	BOOL find=FALSE;

	if (Lock())
	{
		if (m_map_vector.Lookup(vecName,pVector))
		{
			ASSERT(pVector!=NULL);
			if (pVector!=NULL)
			{
				 ret=pVector->GetSize();
				 find=TRUE;
			}
		}
		Unlock();
	}

	if (!find)
	{
		CString filepath;
		char chpath[MAX_PATH];  
		GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
		filepath.Format("%sglobal\\vec\\%s.vec",chpath,vecName);//theApp.GetDirectory()
		pVector=new CVector(filepath);
		ASSERT(pVector!=NULL);
		if (pVector!=NULL)
			ret=pVector->GetSize();
	}

	return ret;
}

void CVectorList::DrawVector(LPCSTR vectorname, CDC *pDC, CRect viewRect, CRect clipRect, COLORREF color, BOOL blReplace)
{
	CSize ret(0,0);
	CVector* pVector;

	if (Lock())
	{
		if (m_map_vector.Lookup(vectorname,pVector))
		{
			ASSERT(pVector!=NULL);
			if (pVector!=NULL)
			  pVector->Draw(pDC,viewRect,clipRect,color,blReplace);
		}
		Unlock();
	}

}
