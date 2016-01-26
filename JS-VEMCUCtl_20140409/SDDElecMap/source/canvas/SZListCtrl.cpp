// SZListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/SZListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSZListCtrl

CSZListCtrl::CSZListCtrl()
{
	m_nListnum=1;
}

CSZListCtrl::~CSZListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSZListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSZListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSZListCtrl message handlers

void CSZListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CHeaderCtrl* pHeader=GetHeaderCtrl();
	int m_nColnum=pHeader->GetItemCount();

	if (lpDrawItemStruct==NULL) return ;

	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem;
		
	if ((lpDrawItemStruct->itemState&ODS_SELECTED)==ODS_SELECTED)
	{
		rcItem=GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
		pDC->FillSolidRect(rcItem, ::GetSysColor(COLOR_ACTIVECAPTION));
		COLORREF oldColor=pDC->GetTextColor();
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		for (int x=0; x<m_nColnum; x++)
		{
			rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
			pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		pDC->SetTextColor(oldColor);
	}
	else
	{
		/**************Ð´ÎÄ×Ö*************/
		for (int x=0; x<m_nColnum; x++)
		{
			rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, TRUE);
			pDC->FillSolidRect(rcItem, RGB(255,255,255));
			pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		/**************Ð´ÎÄ×Ö*************/
	//	CListCtrl::DrawItem(lpDrawItemStruct);
		/**************»­ÑÕÉ«*************/
		if ((int)lpDrawItemStruct->itemID<m_nColorArrayLen)
		{
			rcItem=GetCellRect(lpDrawItemStruct->itemID, m_nListnum, TRUE);
			pDC->FillSolidRect(rcItem, m_aColorArray[lpDrawItemStruct->itemID]);
		}
		/**************»­ÑÕÉ«*************/
	}
}

CRect CSZListCtrl::GetCellRect(int nItem, int nColumn, BOOL outside)
{
	static CRect rectCell;

	// Get the height
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    LV_COLUMN column;
	column.mask = LVCF_WIDTH;        // Set the mask to only return the width

    for (int x=0; x<nColumn; x++)
    {
        column.iSubItem=1;
        CListCtrl::GetColumn(x, &column); // Get rect-edge (left/top)
        rectCell.left+=column.cx;
    }
    column.iSubItem=1;
    CListCtrl::GetColumn(nColumn, &column); // Get rect-edge (right/bottom)
    rectCell.right = rectCell.left + column.cx;

    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}

void CSZListCtrl::SetColor(COLORREF *ColorArray, int ColorArrayLen)
{
	int i;

	if ((ColorArray==NULL)||(ColorArrayLen<=0)||(ColorArrayLen>32)) return;
	for(i=0;i<ColorArrayLen;i++)
		m_aColorArray[i]=ColorArray[i];

	m_nColorArrayLen=ColorArrayLen;
}

void CSZListCtrl::SetColorList(int Listnum)
{
	m_nListnum=Listnum;
}

CRect CSZListCtrl::GetFullCellRect(int nItem, BOOL outside)
{
	static CRect rectCell;
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    if (!outside)
    {   //decrease 1 pixel
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }
	return rectCell;
}
