#pragma once

#define OPEN_CAMERA_BY_PLATFORM 0//通过南瑞平台打开摄像头
#define OPEN_CAMERA_BY_DIRECT_DEVICE 1//直连设备打开摄像头

class CCameraControl
{
public:
	CCameraControl(void);
	~CCameraControl(void);

public:
	int OpenCameraByDirectDevice(CString strDvrIP, int nDvrPort, CString strUsername, CString strPassword, int nDvrType, int nChannel, HWND playwnd);
	int OpenCamera( CString strDvrIP, int nDvrPort, int nChannel, int nDvrType, CString strUsername, CString strPassword, int nOpenType, HWND playwnd );
};
