// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__93382E4F_611A_481B_8337_C0372E4067DC__INCLUDED_)
#define AFX_STDAFX_H__93382E4F_611A_481B_8337_C0372E4067DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>


#include "time.h"

#include <afx.h>



//CommonLib
#include "include\\CommonLib.h"
#pragma comment(lib,"lib\\CommonLib.lib")


#pragma comment(lib,"lib\\libmySQL.lib")
#include <winsock2.h>
#include "include/mysql.h"

#include "include\\LibJSIc.h"
#pragma comment(lib,"lib\\LibJsIc.lib") 

#include "DevPlayVideoSdk.h"
#pragma comment(lib,"lib\\DevPlayVideoSdk.lib")

#pragma comment(lib,"lib\\libcd.lib")

// #include "dvr/dhnetsdk.h"
// #include "dvr/HCNetSDK.h"
// #include "dvr/NetSDK.h"
// #include "dvr/MOBOTIXExport.h"
// #pragma comment(lib,"dvr\\dhnetsdk.lib")
// #pragma comment(lib,"dvr\\HCNetSDK.lib")
// #pragma comment(lib,"dvr\\NetSDK.lib")

#include "tinyxml\\tinyxml.h"

#include "define.h"

#pragma warning(disable:4996) 


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__93382E4F_611A_481B_8337_C0372E4067DC__INCLUDED_)
