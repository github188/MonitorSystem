//***************************************************************************

#ifndef _SYSNETTUOPUAPI_H_
#define _SYSNETTUOPUAPI_H_

#ifdef _x_sysnettuopu_dll_		// 动态链接库定义
#define _xdll_ __declspec(dllexport)
#else
#define _xdll_ __declspec(dllimport)
#endif

typedef void (*CALLBACKTRANS) (char* sysid,char* message);

//---------------------------------------------------------------------------
// C语言标准
#ifdef __cplusplus
extern "C" {
#endif
//显示界面
_xdll_ void	ShowNetTuopuDlg(HWND parenthwnd,HWND* redlg);
//连接数据库句柄
_xdll_ void NetTuopuSetConnect(void* hdbc);
//事物发送回调句柄
_xdll_ void NetTuopuSetTransactionAPI(CALLBACKTRANS trans);
//设置显示数据
_xdll_ void NetTuopuSetMessage(char* message);
//设置用户
_xdll_ void NetTuopuSetUserNum(char* usernum);
#ifdef __cplusplus
}
#endif
//---------------------------------------------------------------------------
#undef _xdll_
#endif	
