// SDDElecMap.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SDDElecMap.h"
#include "SDDElecMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSDDElecMapApp

BEGIN_MESSAGE_MAP(CSDDElecMapApp, CWinApp)
END_MESSAGE_MAP()


// CSDDElecMapApp 构造

CSDDElecMapApp::CSDDElecMapApp()
{
	
}


// 唯一的一个 CSDDElecMapApp 对象

CSDDElecMapApp theApp;


// CSDDElecMapApp 初始化

BOOL CSDDElecMapApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CoInitialize(NULL);

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	return TRUE;
}

CString CSDDElecMapApp::getCurrentAppPath()
{
	CString path; 
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH);

	path.ReleaseBuffer(); 
	int pos = path.ReverseFind('\\'); 
	path = path.Left(pos);

#ifdef _DEBUG
	int pos_Debug = path.ReverseFind('\\');
	path = path.Left(pos_Debug);
	path += "\\SDDElecMap\\";
#else
	path += "\\";
#endif

	return path;
}
