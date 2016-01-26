// PicFileException.cpp: implementation of the CPicFileException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/PicFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CPicFileException,CException)

CPicFileException::CPicFileException()
{
	m_errno=BADFILE;
}

CPicFileException::~CPicFileException()
{

}

BOOL CPicFileException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	CString str;
	switch (m_errno)
	{
	case HIGHVERSION:
		//str.LoadString(IDS_ERR_HIGHVERSION);
		break;
	case LOWVERSION:
		//str.LoadString(IDS_ERR_LOWVERSION);
		break;
	case UNKNOWN:
		//str.LoadString(IDS_ERR_UNKNOWN);
		break;
	default:
		//str.LoadString(IDS_ERR_BADFILE);
		break;
	}

	ASSERT(nMaxError>(UINT)str.GetLength());
	strncpy(lpszError,str,nMaxError<(UINT)str.GetLength()? nMaxError:str.GetLength());
	return TRUE;
}



