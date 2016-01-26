// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#define MAIN_TITLE_HEIGHT           140

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "include\\CommonLib.h"
#pragma comment(lib,"lib\\CommonLib.lib")

#pragma comment(lib,"lib\\libmySQL.lib")
#include "include/mysql.h"

#include "DevPlayVideoSdk.h"
#pragma comment(lib,"lib\\DevPlayVideoSdk.lib")

#include "dvr/dhnetsdk.h"
#include "dvr/HCNetSDK.h"
#include "dvr/NetSDK.h"
#include "dvr/MOBOTIXExport.h"
#pragma comment(lib,"dvr\\dhnetsdk.lib")
#pragma comment(lib,"dvr\\HCNetSDK.lib")
#pragma comment(lib,"dvr\\NetSDK.lib")

//#pragma comment(lib,"lib\\ysxml.lib")

#include "tinyxml\\tinyxml.h"

#include "./../SDDElecMap/DLLInterface.h"

#ifdef _DEBUG
#pragma comment(lib, "./../VEMCUCtl/lib/SDDElecMapD.lib")
#else
#pragma comment(lib, "./../VEMCUCtl/lib/SDDElecMap.lib")
#endif

#pragma warning(disable:4996)
#include "ParamDefine.h"
#include "AssSystemDefine.h"
#include "Common.h"

