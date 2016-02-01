
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include <afxdisp.h>        // MFC 自动化类

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC90.CRT' version='9.0.21022.8' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"") 

#define TOTALCYCLEMETHODNUM	16
#define TOTALCYCLENAMENUM	100

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

//屏蔽警告
#pragma warning(disable:4005 4018 4244 4627 4800 4995 4996)

#include <afxdb.h> 
#include <odbcinst.h>

#include "VMDefine.h"

//系统库
#pragma comment(lib,"Wsock32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"version.lib")

//rtsip库
#include "SipUtil.h"
#pragma comment(lib,"lib\\rtsip.lib")

//rtsp库
#include ".\\include\\rtspClientAPI.h"
#pragma comment(lib,"lib\\RtspLib.lib")

//CommonLib
#include "include\\CommonLib.h"
#pragma comment(lib,"lib\\CommonLib.lib")

// 自定义变量
#include "xappany.h"

// 数据库连接
#pragma comment(lib,"lib\\libmySQL.lib")

// http 部分接口
#include ".\\include\\HttpClient.h"

#ifdef DEBUG
#pragma comment(lib,"lib\\HttpClient_d.lib")
#else
#pragma comment(lib,"lib\\HttpClient.lib")
#endif

//气象接口
#include ".\\include\\SearchWeatherInfoApi.h"
#pragma comment(lib,"lib\\SearchWeatherInfo.lib")

// 配置
#include "include\\SystemLinkageSettingAPI.h"
#ifdef DEBUG
#pragma comment(lib,"lib\\SystemLinkageSetting_d.lib")
#else
#pragma comment(lib,"lib\\SystemLinkageSetting.lib")
#endif

// 系统拓扑图
#include ".\\include\\SysNetTuopuAPI.h"
#pragma comment(lib, "lib\\SysNetTuopu.lib")

// xml解析
#include ".\\tinyxml\\xmlApi.h"

// 文件日志
#include "VMLog.h"

//VideoError信息
#include "VideoErrorInfo.h"

//ClientVideo信息
#include "ClientVideoInfo.h"

//CameraStatus信息
#include "CameraStatusInfo.h"

#include "include\\DevPlayVideoSdk.h"
#pragma comment(lib, "lib\\DevPlayVideoSdk.lib")

//辅助系统
#include "include\\ACSSystemAPI.h"
#ifdef DEBUG
#pragma comment(lib,"lib\\ACSSystemD.lib")
#else
#pragma comment(lib,"lib\\ACSSystem.lib")
#endif

//辅助系统全局变量和结构体定义
#include "AssSystemDefine.h"
#include "AssDBOperate.h"
#include "AssCommonFunction.h"

//辅助联动配置管理
#include "include\\LinkageSettingAPI.h"
#ifdef DEBUG
#pragma comment(lib,"lib\\LinkageSettingD.lib")
#else
#pragma comment(lib,"lib\\LinkageSetting.lib")
#endif

#include "AssCameraOperate.h"

//电子地图 add by wx 20151102
#ifdef  _DEBUG
#pragma comment(lib,"lib\\SDDElecMapD.lib")
#else
#pragma comment(lib,"lib\\SDDElecMap.lib")
#endif
#include "SDDElecMap\DLLInterface.h"

#include "include\\deelx.h"
#include "AssGlobal.h"

#include "include\\MyTreeCtrl.h"
#include "include\\MultiTreeCtrl.h"

#include "LogManager.h"


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif