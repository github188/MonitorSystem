#pragma once

#include "linkobjectbase.h"

/*
事件内容：
1   亮度调度   
2   复态 恢复到最新保存状态
3   确认 盘上状态元件停闪，不灭灯
4   事故音响
5   预告音响
6   关音响
7   全红置数
8   全绿置数
9   全灭置数
10  全红
11  全绿
12  全橙
13  全灭
14  暗盘
15  亮盘
16  显示器检测
17  显示器地址显示
*/

class CMNP_ZJNH :
	public CLinkObjectBase
{
    DECLARE_SERIAL(CMNP_ZJNH);
public:	
	virtual void Save_Ty(CFile *file, BOOL Yn);
	virtual void Serialize_Ty(CArchive &ar);
	BOOL OnEditProperties();
	CMNP_ZJNH(void);
	~CMNP_ZJNH(void);
protected:
	BOOL Event1_Enable;
	BOOL Event2_Enable;
	BOOL Event3_Enable;
	BOOL Event4_Enable;
	BOOL Event5_Enable;
	BOOL Event6_Enable;
	BOOL Event7_Enable;
	BOOL Event8_Enable;
	BOOL Event9_Enable;
	BOOL Event10_Enable;
	BOOL Event11_Enable;
	BOOL Event12_Enable;
	BOOL Event13_Enable;
	BOOL Event14_Enable;
	BOOL Event15_Enable;
	BOOL Event16_Enable;
	BOOL Event17_Enable;

};
