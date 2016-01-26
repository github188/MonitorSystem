#include "stdafx.h"
#include "AssListControlOperate.h"
#include <vector>

void AddColumnToList( CListCtrl *pList, std::vector<CString> vecText )
{
	pList->DeleteAllItems();
	while(pList->DeleteColumn(0));

	CRect rect;
	pList->GetWindowRect(rect);
	int nWidth = rect.right - rect.left;

	for (int i=0; i<vecText.size(); i++)
	{
		pList->InsertColumn(i, vecText.at(i));
		pList->SetColumnWidth(i, nWidth/vecText.size());
	}
}