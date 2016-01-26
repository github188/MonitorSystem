typedef bool (*fp_Init_DevSdk)();
typedef bool (*fp_Exit_DevSdk)();
typedef int (*fp_GetID_DevSdk)();
typedef int (*fp_OpenVideo_DevSdk)(char* ip,unsigned short port, char* user, char* password,unsigned short dvrtype,unsigned short dvrid,unsigned short channel,int id ,HWND playwnd);
typedef void (*fp_CloseVideo_DevSdk)(int id);

typedef struct _dev_play_video_sdk_funs
{
	fp_Init_DevSdk fun_Init_DevSdk;
	fp_Exit_DevSdk fun_Exit_DevSdk;
	fp_GetID_DevSdk fun_GetID_DevSdk;
	fp_OpenVideo_DevSdk fun_OpenVideo_DevSdk;
	fp_CloseVideo_DevSdk fun_CloseVideo_DevSdk;
}DEV_PLAY_VIDEO_SDK_FUNS;