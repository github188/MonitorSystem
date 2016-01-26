// PicFileException.h: interface for the CPicFileException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICFILEEXCEPTION_H__486700F0_4E50_4AA7_B0F0_4DC793430771__INCLUDED_)
#define AFX_PICFILEEXCEPTION_H__486700F0_4E50_4AA7_B0F0_4DC793430771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#define		UNKNOWN				0
#define		HIGHVERSION		1
#define		LOWVERSION		2
#define		BADFILE				3
*/

class CPicFileException : public CException  
{
	DECLARE_DYNAMIC(CPicFileException)
public:
	enum
	{
		UNKNOWN,
		HIGHVERSION,
		LOWVERSION,
		BADFILE
	};


	CPicFileException();

	virtual BOOL  GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );
	virtual ~CPicFileException();

public:
	int m_errno;
};



#endif // !defined(AFX_PICFILEEXCEPTION_H__486700F0_4E50_4AA7_B0F0_4DC793430771__INCLUDED_)
