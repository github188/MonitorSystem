#include "StdAfx.h"
#include "CameraControl.h"

CCameraControl::CCameraControl(void)
{
}

CCameraControl::~CCameraControl(void)
{
}

int CCameraControl::OpenCamera( CString strDvrIP, int nDvrPort, int nChannel, int nDvrType, CString strUsername, CString strPassword, int nOpenType, HWND playwnd )
{
	int ret = 0;
	if (nOpenType == 1)
	{
		ret = OpenCameraByDirectDevice(strDvrIP, nDvrPort, strUsername, strPassword, nDvrType, nChannel, playwnd);
	}
	return ret;
}

//直连设备打开摄像头
int CCameraControl::OpenCameraByDirectDevice(CString strDvrIP, int nDvrPort, CString strUsername, CString strPassword, int nDvrType, int nChannel, HWND playwnd)
{
	int ret = 0;
	ret = OpenVideo_DevSdk(strDvrIP.GetBuffer(), nDvrPort, strUsername.GetBuffer(), strPassword.GetBuffer(), nDvrType, 1, nChannel, 1, playwnd);
	return ret;
}