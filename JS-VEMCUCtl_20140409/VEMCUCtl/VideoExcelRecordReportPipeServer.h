#ifndef _VIEDEO_EXCEL_RECORD_REPORT_PIPE_SERVER_H_
#define _VIEDEO_EXCEL_RECORD_REPORT_PIPE_SERVER_H_

#include "VideoExcelRecordReportParseDef.h"

BOOL StartVideoExcelRecordReportPipe();
void StopVideoExcelRecordReportPipe();
unsigned _stdcall VideoExcelRecordReportPipeInstanceProc(void * arg);
int VideoExcelRecordReportPipeWriteData(void *buffer, DWORD dwLen );


#endif

