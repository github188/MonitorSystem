/*
MFC List-control树控件的操作
*/
#ifndef _ASSLISTCONTROLOPERATE_H_
#define _ASSLISTCONTROLOPERATE_H_

//添加列,自动设置列宽
void AddColumnToList(CListCtrl *pList, std::vector<CString> vecText);

#endif