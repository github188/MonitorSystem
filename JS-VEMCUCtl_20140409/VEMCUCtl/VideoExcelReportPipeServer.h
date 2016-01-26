#ifndef _VIEDEO_EXCEL_REPORT_PIPE_SERVER_H_
#define _VIEDEO_EXCEL_REPORT_PIPE_SERVER_H_

#include "VideoExcelReportParseDef.h"

BOOL StartVideoExcelReportPipe();
void StopVideoExcelReportPipe();
unsigned _stdcall VideoExcelReportPipeInstanceProc(void * arg);
int VideoExcelReportPipeWriteData(void *buffer, DWORD dwLen );



#endif

