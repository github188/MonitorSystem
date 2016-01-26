#include "stdafx.h"
#include "canvas/linkfile.h"
#include "canvas/TyBase.h"
#include "dialog/PropertyFilePage.h"

IMPLEMENT_SERIAL(CLinkFile,CLinkObjectBase,0)

CLinkFile::CLinkFile(void)
:CLinkObjectBase(lnkFile,"")
{
	InitMemory();  
}

CLinkFile::~CLinkFile(void)
{
 
}

CLinkFile::CLinkFile( CString strname)
:CLinkObjectBase(lnkFile,strname)
{
	InitMemory();  
}

void CLinkFile::InitMemory ()
{
	ZeroMemory (m_Filename ,sizeof(char)*33);
	ZeroMemory (m_name ,sizeof(char)*33);
}

void CLinkFile::Save_Ty(CFile *file, BOOL Yn)
{
	if(Yn)	//如果是在进行保存
	{
		//file->Write((int *)&m_Fun,sizeof(m_Fun));
		file->Write((unsigned char *)m_Filename ,sizeof(char)*33);
	}
	else
	{
		//file->Read((int *)&m_Fun,sizeof(m_Fun));
		file->Read((unsigned char *)m_Filename ,sizeof(char)*33);
	}
}

void CLinkFile::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
	//	ar<<m_Fun;
		ar.Write((unsigned char *)m_Filename,sizeof(char)*33);
	}
	else
	{
	//	ar>>m_Fun;
		ar.Read((unsigned char *)m_Filename,sizeof(char)*33);
	}
}

BOOL CLinkFile::OnEditProperties()
{
    CPropertySheet sheet( _T("选择文件..") );
	CPropertyFilePage  dlg;

	sheet.AddPage( (CPropertyPage*)&dlg );
    dlg.Setfile (m_Filename ,m_name );
	
	if (sheet.DoModal() == IDOK)
	{
		dlg.Getfile(m_Filename,m_name ); 
		return TRUE;
  }

	return false;
}



