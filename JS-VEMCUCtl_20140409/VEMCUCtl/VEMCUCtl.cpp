
// VEMCUCtl.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CVEMCUCtlApp

BEGIN_MESSAGE_MAP(CVEMCUCtlApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVEMCUCtlApp 构造

CVEMCUCtlApp::CVEMCUCtlApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVEMCUCtlApp 对象

CVEMCUCtlApp theApp;

HANDLE g_hAppMutex = NULL;

// CVEMCUCtlApp 初始化

BOOL CVEMCUCtlApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 更改用于存储设置的注册表项
	SetRegistryKey(_T("热点监控"));

	g_hAppMutex = CreateMutex(NULL,FALSE,"JS-VEMCUCtl");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (g_hAppMutex != NULL)
		{
			CloseHandle(g_hAppMutex);
			g_hAppMutex = NULL;
		}

		MessageBox(NULL,"客户端已启动过,并已缩小到任务栏位置,可通过单击任务栏图标右键的方式进行显示。若无相关图标时,请在任务管理器中关闭进程\"VEMCUCtl.exe\",再重新启动客户端。","视频监视系统",MB_OK);
		return FALSE;
	}

	CVEMCUCtlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}

	if (g_hAppMutex != NULL)
	{
		CloseHandle(g_hAppMutex);
		g_hAppMutex = NULL;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
