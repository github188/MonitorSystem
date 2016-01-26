// ElecMapDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "SDDElecMap.h"
#include "ElecMapDoc.h"
#include "ElecMapView.h"
#include "SDDElecMapDlg.h"

#include "canvas/HuaMian.h"
#include "canvas/HMProject.h"
// CElecMapDoc

IMPLEMENT_DYNCREATE(CElecMapDoc, CDocument)

CElecMapDoc::CElecMapDoc()
{

}

BOOL CElecMapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

CElecMapDoc::~CElecMapDoc()
{

}


BEGIN_MESSAGE_MAP(CElecMapDoc, CDocument)
END_MESSAGE_MAP()


// CElecMapDoc 诊断

void CElecMapDoc::Serialize(CArchive& ar)
{
	CFile *file;
	BOOL bSave;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	file = ar.GetFile();
	bSave = ar.IsStoring();

	CHuaMian* pHuamian = NULL;
	if (bSave == FALSE)
	{
		ASSERT(pHuamian == NULL);
		_splitpath(file->GetFilePath(), drive, dir, fname, ext);

		pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
		if (pHuamian==NULL){
			TRACE0("set huamian instance failed.");
			return;
		}
		pHuamian->Save(file,bSave);
		SetModifiedFlag(FALSE);
	} 
	else
	{
		ASSERT(pHuamian != NULL);
		ASSERT_VALID(pHuamian);
	}
}

BOOL CElecMapDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName)){
		return FALSE;
	}

	return TRUE;
}

BOOL  CElecMapDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return 	 CDocument::OnSaveDocument(lpszPathName);
}

void CElecMapDoc::OnFileSave(LPCTSTR lpszPathName)
{
	OnSaveDocument(lpszPathName);
}

// CElecMapDoc 命令

void CElecMapDoc::DrawDC(CDC *pDC, CElecMapView *pView)
{
	CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
	if (!pHuamian)
		return;

	pHuamian->DrawDC(pDC,pView);
}

void CElecMapDoc::UpdateScale(float scale)
{
	POSITION pos = GetFirstViewPosition();
	if (pos==NULL) 
		return;
}

void CElecMapDoc::setCurrentView(CElecMapView* view)
{
	m_pElecMapView = view;
}

CElecMapView* CElecMapDoc::getCurrentView() const
{
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL) 
		return NULL;

	return (CElecMapView*)GetNextView(pos);
}

CHuaMian* CElecMapDoc::getHuamianInstance( const CString& strHMName, CElecMapDoc* pDoc )
{
	return new CHuaMian(strHMName,pDoc);
}

BOOL CElecMapDoc::addHMLayerTest()
{
	tagHMLayerID hmid;

	ZeroMemory(&hmid,sizeof(hmid));
	hmid.fScale=1;
	hmid.fStartScale=1;
	hmid.fEndScale=1;
	strcpy(hmid.strLayerName,"1");
	hmid.bHide = FALSE;

	CHMProject::getInstance()->AddHMLayer("test", &hmid);

	return TRUE;
}