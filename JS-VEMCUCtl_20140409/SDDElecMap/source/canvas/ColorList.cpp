// ColorList.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/ColorList.h"
#include "canvas/mccurvedata.h"
#include "dialog/mccurvedialog.h"
#include "dialog/YMcCurvePage2.h"

// CColorList

IMPLEMENT_DYNAMIC(CColorList, CListCtrl)

CColorList::CColorList()
{

}

CColorList::~CColorList()
{

}


BEGIN_MESSAGE_MAP(CColorList, CListCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CColorList 消息处理程序

void CColorList::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CColorList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC= CDC::FromHandle(lpDrawItemStruct->hDC);
	CMcCurvePage2* ppage2 = (CMcCurvePage2*) GetParent();
	CRect drawrect;
	int i;
	CString str;
	TCurve tempcurve;
	BOOL selected[8];

	ZeroMemory(selected,sizeof(selected[0])*8);

	if (lpDrawItemStruct->itemID==0)
	{
		GetClientRect(drawrect);
		pDC ->FillSolidRect(drawrect,RGB(255,255,255));
	}

	UINT selectstate = lpDrawItemStruct->itemState;
	if (selectstate&ODS_SELECTED)
	{
		GetSubItemRect(lpDrawItemStruct->itemID,0,LVIR_BOUNDS,drawrect);
		pDC->SetBkMode(TRANSPARENT);
		pDC ->FillSolidRect(drawrect,RGB(0,0,255));
		selected[lpDrawItemStruct->itemID]=TRUE;
	}
	
	switch (ppage2->nCurveSel)
	{
	case 0:
			for (i = 0;i < ppage2 ->pmccurvedata->gtIcurvearray.GetSize();i++)
			{
				GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
				drawrect.right = 30;
				str = GetItemText(i,0);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,1);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,2);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,3);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,4,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,4);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,5,LVIR_BOUNDS,drawrect);
				drawrect.DeflateRect(2,2,2,2);
				tempcurve = ppage2->pmccurvedata->gtIcurvearray.GetAt(i);
				pDC->FillSolidRect(drawrect,tempcurve.ncurvecolor);

				GetSubItemRect(i,6,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,6);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,7,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,7);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,8,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,8);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,9,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,9);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
			}
			break;
	case 1:
			for (i = 0;i < ppage2 ->pmccurvedata->gtVcurvearray.GetSize();i++)
			{
				GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
				drawrect.right = 30;
				str=GetItemText(i,0);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,1);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,2);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,3);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,4,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,4);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,5,LVIR_BOUNDS,drawrect);
				drawrect.DeflateRect(2,2,2,2);
				tempcurve = ppage2->pmccurvedata->gtVcurvearray.GetAt(i);
				pDC->FillSolidRect(drawrect,tempcurve.ncurvecolor);

				GetSubItemRect(i,6,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,6);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,7,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,7);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,8,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,8);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,9,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,9);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			}
			break;
	case 2:
		for (i = 0;i < ppage2 ->pmccurvedata->zbIcurvearray.GetSize();i++)
			{
				GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
				drawrect.right=30;
				str = GetItemText(i,0);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,1);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,2);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,3);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,4,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,4);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,5,LVIR_BOUNDS,drawrect);
				drawrect.DeflateRect(2,2,2,2);
				tempcurve = ppage2->pmccurvedata->zbIcurvearray.GetAt(i);
				pDC->FillSolidRect(drawrect,tempcurve.ncurvecolor);

				GetSubItemRect(i,6,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,6);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,7,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,7);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,8,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,8);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,9,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,9);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			break;
	case 3:
			for (i = 0;i < ppage2 ->pmccurvedata->zbVcurvearray.GetSize();i++)
			{
				GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
				drawrect.right=30;
				str = GetItemText(i,0);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,1);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,2);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,3);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,4,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,4);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				
				GetSubItemRect(i,5,LVIR_BOUNDS,drawrect);
				drawrect.DeflateRect(2,2,2,2);
				tempcurve = ppage2->pmccurvedata->zbVcurvearray.GetAt(i);
				pDC->FillSolidRect(drawrect,tempcurve.ncurvecolor);

				GetSubItemRect(i,6,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,6);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			
				GetSubItemRect(i,7,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,7);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,8,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,8);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

				GetSubItemRect(i,9,LVIR_BOUNDS,drawrect);
				str = GetItemText(i,9);
				pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			break;
	default:
		break;
	}
}


IMPLEMENT_DYNAMIC(CNewColorList, CListCtrl)
CNewColorList::CNewColorList()
{

}

CNewColorList::~CNewColorList()
{

}


BEGIN_MESSAGE_MAP(CNewColorList, CListCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// CColorList 消息处理程序

void CNewColorList::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CNewColorList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CYMcCurvePage2* ppage2 = (CYMcCurvePage2*) GetParent();
	CRect drawrect;
	int i;
	CString str;
	ymcurve_info tempcurve;
	BOOL selected[8];
	ZeroMemory(selected,sizeof(selected[0])*8);

	if (lpDrawItemStruct->itemID == 0)
	{
		GetClientRect(drawrect);
		pDC ->FillSolidRect(drawrect,RGB(255,255,255));
	}

	UINT selectstate = lpDrawItemStruct->itemState;
	if (selectstate&ODS_SELECTED)
	{
		GetSubItemRect(lpDrawItemStruct->itemID,0,LVIR_BOUNDS,drawrect);
		pDC->SetBkMode(TRANSPARENT);
		pDC ->FillSolidRect(drawrect,RGB(0,0,255));
		selected[lpDrawItemStruct->itemID]=TRUE;
	}

	for (i = 0;i < ppage2 ->curvearray.GetSize();i++)
	{
		GetSubItemRect(i,0,LVIR_BOUNDS,drawrect);
		drawrect.right = 30;
		str = GetItemText(i,0);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,1,LVIR_BOUNDS,drawrect);
		str = GetItemText(i,1);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,2,LVIR_BOUNDS,drawrect);
		str = GetItemText(i,2);
		pDC->DrawText(str,drawrect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		GetSubItemRect(i,3,LVIR_BOUNDS,drawrect);
		drawrect.DeflateRect(2,2,2,2);
		tempcurve = ppage2->curvearray.GetAt(i);
		pDC->FillSolidRect(drawrect,tempcurve.color);
	}
}
