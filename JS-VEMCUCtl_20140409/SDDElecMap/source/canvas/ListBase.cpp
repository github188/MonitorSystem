// ListBase.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/ListBase.h"
#include "dialog/YcCurveDialog.h"

// CListBase

IMPLEMENT_DYNAMIC(CListBase, CListCtrl)
CListBase::CListBase()
{
}

CListBase::~CListBase()
{
}


BEGIN_MESSAGE_MAP(CListBase, CListCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CListBase 消息处理程序

 void CListBase::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
 }

void CListBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
 {
	 CDC* pDC= CDC::FromHandle(lpDrawItemStruct->hDC);
	 CYcCurvePage* ppage2 = (CYcCurvePage*) GetParent();
	 CRect drawrect;
	 int i;
	 CString str;
	 ycurve_info tempcurve;
	 BOOL selected[8];
	
	 ZeroMemory(selected,sizeof(selected[0])*8);
	
	 if (lpDrawItemStruct->itemID==0)
	{
		GetClientRect(drawrect);
		pDC ->FillSolidRect(drawrect,RGB(255,255,255));
	}
	
	UINT selectstate=lpDrawItemStruct->itemState;
	
	if (selectstate&ODS_SELECTED)
	{
		GetSubItemRect(lpDrawItemStruct->itemID,0,LVIR_BOUNDS,drawrect);
		pDC->SetBkMode(TRANSPARENT);
		pDC ->FillSolidRect(drawrect,RGB(0,0,255));
		selected[lpDrawItemStruct->itemID]=TRUE;
	}

	for (i=0; i<ppage2 ->curvearray.GetSize() ; i++)
	{
		GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
		drawrect.right=30;
		str=GetItemText(i,0);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
		str=GetItemText(i,1);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
		str=GetItemText(i,2);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
		drawrect.DeflateRect(2,2,2,2);
		tempcurve=ppage2->curvearray.GetAt(i);
		pDC->FillSolidRect(drawrect,tempcurve.color);
	}
}

	
