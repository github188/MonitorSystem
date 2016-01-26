#include "stdafx.h"
#include "canvas/ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl

CColorListCtrl::CColorListCtrl(int colSize)
{
    m_columnSize = colSize;
    m_fullColumnLines = TRUE;
    m_withSingleRect = FALSE;

#ifdef _DEBUG
    m_BigFocusRect = TRUE;
#else
    m_BigFocusRect=FALSE;
#endif

    IsCreated=FALSE;
}

//***************************************************************
CColorListCtrl::~CColorListCtrl()
{

}

//***************************************************************
BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CColorListCtrl::OnDestroy() 
{
	DeleteAllItems();	
	CListCtrl::OnDestroy();
}

//***************************************************************
void CColorListCtrl::SetColumnType(BOOL bNew)
{
    m_fullColumnLines = bNew;
    if (IsCreated) 
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetRectType(BOOL bNew)
{
    m_withSingleRect = bNew;
    if (IsCreated)
    {
        RedrawWindow();
    }
}
void CColorListCtrl::SetFocusType(BOOL bNew)
{
    m_BigFocusRect = bNew;
    if (IsCreated)
    {
        RedrawWindow();
    }
}

//***************************************************************
CMyLParam*	CColorListCtrl::GetMyLParam(int Index)
{
    LVITEM	Item;
	Item.mask		= LVIF_PARAM;
	Item.iItem		= Index;
	Item.iSubItem	= 0;
	CListCtrl::GetItem(&Item);
	return (CMyLParam*)Item.lParam;
}

//***************************************************************
CRect CColorListCtrl::GetFullCellRect(int nItem, BOOL outside)
{
	static CRect rectCell;
    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);

    if (!outside)
    {  
        rectCell.left++;    rectCell.right--;
        rectCell.top++;     rectCell.bottom--;
    }

	return rectCell;
}

//***************************************************************
CRect CColorListCtrl::GetCellRect(int nItem, int nColumn, BOOL outside)
{
	static CRect rectCell;

    CListCtrl::GetItemRect(nItem, rectCell, LVIR_BOUNDS);
    LV_COLUMN column;
	column.mask = LVCF_WIDTH;

    for (int x = 0;x < nColumn;x++)
    {
        column.iSubItem = 1;
        CListCtrl::GetColumn(x, &column);
        rectCell.left += column.cx;
    }

    column.iSubItem = 1;
    CListCtrl::GetColumn(nColumn, &column);
    rectCell.right = rectCell.left + column.cx;

    if (!outside)
    { 
        rectCell.left++;
		rectCell.right--;
        rectCell.top++;
		rectCell.bottom--;
    }

	return rectCell;
}

//***************************************************************
void CColorListCtrl::InvalidateFullItem(int nItem, BOOL bErase)
{
    CRect rcBounds = GetFullCellRect(nItem, TRUE);
	InvalidateRect(rcBounds, bErase);
}

//***************************************************************
// create ListCtrl-head
int CColorListCtrl::InitCtrl(CStringArray *phead, CByteArray *pProcent)
{
    if ( (!phead)||(!pProcent)) 
		return 1;

	LV_COLUMN lvc;
	int cx = 0;
    int count = phead->GetSize(); 
    if (count != m_columnSize) 
		return 2;

    if (pProcent->GetSize() != count) 
		return 3;
	int x;
    for ( x=0; x<count; x++) 
	{
		cx+=pProcent->GetAt(x);
	}

    if (cx!=100) 
		return 4;

	CRect rect_listbox;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	GetWindowRect(rect_listbox);
	cx = int((rect_listbox.right - rect_listbox.left) - 20); 

    //set head-columns in ListBox
	lvc.iSubItem = 0;
	lvc.fmt = LVCFMT_LEFT ;		
	
    for (x = 0;x < count;x++)
    {
        lvc.cx = int(cx*pProcent->GetAt(x)/100);
	    lvc.pszText = (char*)LPCSTR(phead->GetAt(x));
	    CListCtrl::InsertColumn(x,&lvc);
    }

    return 0;
}


//***************************************************************
// add an item
int CColorListCtrl::AddItem(CStringArray *line, int pos)
{
    int ret_index = LISTCTRL_ERROR;

    if (!line)
		return ret_index;

	LV_ITEM lvi;
    
	lvi.mask = LVIF_TEXT ; 
	lvi.iSubItem = 0;
    lvi.pszText = "";

    int count=GetItemCount();
    int columns=line->GetSize();

    if (m_columnSize!=columns)
		return ret_index;

    if (DWORD(pos)<=DWORD(count)) 
		count=pos;
	
    lvi.iItem = count;
	lvi.mask = LVIF_TEXT ; 
	lvi.iSubItem = 0;
    ret_index = InsertItem(&lvi);

    for (int x = 0;x < columns;x++)
    {
        CListCtrl::SetItemText(count,x,(char*)LPCSTR(line->GetAt(x)) );
    }

	lvi.mask = LVIF_STATE;
	lvi.iSubItem = 1;
	lvi.stateMask = 0xFFFFFFFF ; 
	lvi.state = 0;

	if (CListCtrl::SetItem(&lvi)) 
    {
        CMyLParam *p = new CMyLParam(m_columnSize);

        #ifdef TESTCOLOR
            // only for TEST
            if (ret_index<3)
            {
                p->SetBackColor(ITEM_COLOR(ret_index+BLUE),0);
                p->SetTextColor(ITEM_COLOR(ret_index+RED ),0);
            }
            else
            if (ret_index<=6)
            {
                p->SetBackColor(ITEM_COLOR(ret_index+BLUE),1);
                p->SetTextColor(ITEM_COLOR(ret_index+RED ),1);
            }
            else
            {
                p->SetBackColor(ITEM_COLOR((ret_index+BLUE)&0xF),2);
                p->SetTextColor(ITEM_COLOR((ret_index+RED )&0xF),2);
            }
            // end: only for TEST
        #endif

        CListCtrl::SetItemData(ret_index, DWORD(p));
       
		return ret_index;
    }

    return LISTCTRL_ERROR;
}

//***************************************************************
BOOL CColorListCtrl::DeleteAllItems()
{
	while (CListCtrl::GetItemCount())
	{
        CColorListCtrl::DeleteItem(0);
	}

    return TRUE;
}

//***************************************************************
BOOL CColorListCtrl::DeleteItem(int nItem )
{
    if (CListCtrl::GetItemCount()<=nItem) 
		return FALSE;

    CMyLParam *p = (CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p != NULL)
    {
        CListCtrl::SetItemData(nItem, p->GetData());
        delete p;
        return CListCtrl::DeleteItem( nItem );
    }

    return TRUE;
}


//***************************************************************
DWORD CColorListCtrl::GetItemData( int nItem ) const
{
    CMyLParam *p=(CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p != NULL)
    {
        return p->GetData();
    }

    return 0;
}

//***************************************************************
BOOL  CColorListCtrl::SetItemData( int nItem, DWORD dwData )
{
    CMyLParam *p=(CMyLParam*)CListCtrl::GetItemData(nItem);
    if (p != NULL)
    {
        p->SetData(dwData);
        return TRUE;
    }

    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemBackgndColor(ITEM_COLOR color, int Index, int iSub)	
{
    CMyLParam *p=GetMyLParam(Index);
    if (p == NULL) 
		return FALSE;

    if (p->SetBackColor(color, iSub))
    {
        RedrawItems( Index, Index );
        return TRUE;
    }

    return FALSE;
}

//***************************************************************
BOOL CColorListCtrl::SetItemTextColor(ITEM_COLOR color, int Index, int iSub)	
{
	CRect m_rect;
	int i;
	int h=0;

    for(i = 0;i < GetItemCount();i++)
	{
	    GetItemRect(i, &m_rect, LVIR_BOUNDS);
		h = h+m_rect.Size().cy;
	}

	CSize size(0,h);
	Scroll(size);

    CMyLParam *p = GetMyLParam(Index);
    if (p == NULL) 
		return FALSE;

    if (p->SetTextColor(color, iSub))
    {
        RedrawItems( Index, Index );
        return TRUE;
    }

    return FALSE;
}

//***************************************************************
COLORREF CColorListCtrl::GetItemBackgndColor(int Index, int iSub)
{
    CMyLParam *p=GetMyLParam(Index);
    if (p == NULL)
		return 0;

    return p->GetBackColor(iSub);
}

//***************************************************************
COLORREF CColorListCtrl::GetItemTextColor(int Index, int iSub)
{
    CMyLParam *p=GetMyLParam(Index);
    if (p == NULL)
		return 0;

    return p->GetTextColor(iSub);
}

//***************************************************************
void CColorListCtrl::OnPaint() 
{
	const MSG *msg = GetCurrentMessage();
	DefWindowProc(msg->message, msg->wParam, msg->lParam);

    if (!m_fullColumnLines) 
		return;

	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
	{
		CClientDC dc(this );
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		RECT rect;
		pHeader->GetClientRect( &rect );
		int top = rect.bottom;

		GetClientRect( &rect );

		int borderx = 0 - GetScrollPos( SB_HORZ );

        CPen       *pOldPen;
        CPen        pen;
        CGdiObject *pOldBrush;
        pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_DESELTEXT) );
        pOldPen    = dc.SelectObject(&pen);
        pOldBrush = dc.SelectStockObject(NULL_BRUSH);

		for(int i = 0;i < nColumnCount;i++ )
		{
			borderx += GetColumnWidth( i );

			if( borderx >= rect.right ) 
				break;

			dc.MoveTo( borderx-1, top);
			dc.LineTo( borderx-1, rect.bottom );
		}

        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush);
	}
}

//***************************************************************
void CColorListCtrl::RepaintSelectedItems()
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
        InvalidateFullItem(nItem, FALSE);
	}

	if((GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = GetNextItem(-1, LVNI_SELECTED);nItem != -1;nItem = GetNextItem(nItem, LVNI_SELECTED))
		{
            InvalidateFullItem(nItem, FALSE);
		}
	}

	UpdateWindow();
}

//***************************************************************
BOOL CColorListCtrl::SetSel(int pos, BOOL bSelect)
{
    UINT nState = 0;

    if (bSelect != FALSE) 
	{
		nState=LVIS_SELECTED|LVS_EX_FULLROWSELECT;
	}
	else 
	{
		nState=0;
	}

    if (SetItemState(pos, nState, LVIS_SELECTED|LVS_EX_FULLROWSELECT))
    {
        if((GetStyle()&LVS_SHOWSELALWAYS))
        {
            InvalidateFullItem(pos, TRUE);
        }

        return TRUE;
    }

    return FALSE;
}
//***************************************************************
void CColorListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	SendMessage(WM_PAINT);

    CListCtrl::OnKillFocus(pNewWnd);

	if(pNewWnd != NULL&&pNewWnd->GetParent() == this)
		return;

	if((GetStyle()&LVS_TYPEMASK) == LVS_REPORT)
	{
		RepaintSelectedItems();
	}
}

//***************************************************************
void CColorListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
	
	if(pOldWnd != NULL && pOldWnd->GetParent() == this)
		return;

	if((GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
	{
		RepaintSelectedItems();
	}
}

//***************************************************************
void CColorListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    if (lpDrawItemStruct == NULL)
		return;

    IsCreated = TRUE;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CMyLParam *pMyLParam	= GetMyLParam(lpDrawItemStruct->itemID);
    if (pMyLParam == NULL) 
		return;

	int x = 0;
	CRect rcItem;

    typedef enum 
	{
		R_NOTHINGS = 0,
		R_FOCUS, 
		R_SELECT, 
		R_NORMAL, 
		R_SEL_NOFOCUS
	}redrawmode;

    redrawmode redraw = R_NORMAL;

	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lpDrawItemStruct->itemID;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED)
									||((lvi.state & LVIS_SELECTED)&& ((GetFocus() == this)||(GetStyle() & LVS_SHOWSELALWAYS))));

    BOOL bLostFocus = (GetFocus() != this);

    if ((lpDrawItemStruct->itemAction&ODA_FOCUS) == ODA_FOCUS)
    {
        if (bLostFocus) 
		{
			redraw=R_SEL_NOFOCUS;
		}
        else if ((lvi.state&LVIS_FOCUSED)==LVIS_FOCUSED) 
		{
			redraw=R_FOCUS;
		}
        else 
        {
            if (bHighlight) 
				redraw=R_SELECT;
            else 
				redraw=R_NORMAL;
        }
    }
    else if ((lpDrawItemStruct->itemAction&ODA_SELECT) == ODA_SELECT)
    { 
            if (bHighlight)
            {
                if (bLostFocus)
					redraw=R_SEL_NOFOCUS;
                 else 
					 redraw=R_SELECT;
            }
            else redraw=R_NORMAL;
    }
    else
    { 
        if (bLostFocus)
        {
            if (bHighlight)
				redraw=R_SEL_NOFOCUS;
             else
              redraw=R_NORMAL;
        }
        else 
        {
            if ((lvi.state&LVIS_FOCUSED) == LVIS_FOCUSED)
			{
				redraw=R_FOCUS;
			}
            else 
            {
                if (bHighlight)
					redraw=R_SELECT;
                 else 
					 redraw=R_NORMAL;
            }
        }
    }

    CPen       *pOldPen;
    CPen        pen;
    CGdiObject *pOldBrush;

	if (redraw == R_SEL_NOFOCUS)
		redraw = R_FOCUS;

    switch(redraw)
    {
        case R_FOCUS:
            rcItem = GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_SELBACKGROUND));
            pDC->SetTextColor(GetColorRef(DEF_SELTEXT));

            if (m_BigFocusRect)
            {
                pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_SELTEXT) );
                pOldPen  =pDC->SelectObject(&pen);
                pOldBrush=pDC->SelectStockObject(NULL_BRUSH);
                pDC->Rectangle(rcItem);
                pDC->SelectObject(pOldPen);
                pDC->SelectObject(pOldBrush);
            }
            else 
			{
				pDC->DrawFocusRect(rcItem);
			}

            for (x = 0;x<m_columnSize;x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER );
            }
            break;

        case R_SELECT:
            rcItem = GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_SELBACKGROUND));
            pDC->SetTextColor(GetColorRef(DEF_SELTEXT));

            for (x = 0;x < m_columnSize;x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;

        case R_SEL_NOFOCUS:
            rcItem = GetFullCellRect(lpDrawItemStruct->itemID, TRUE);
            pDC->FillSolidRect(rcItem, GetColorRef(DEF_DESELBACKGROUND));
            pDC->SetTextColor(pMyLParam->GetTextColor(DEF_DESELTEXT));
            pen.CreatePen( PS_DOT, 0, GetColorRef(DEF_DESELTEXT));
            pOldPen = pDC->SelectObject(&pen);
            pOldBrush = pDC->SelectStockObject(NULL_BRUSH);

            if (!m_withSingleRect)
			{
                pDC->Rectangle(rcItem);
			}

            for (x = 0;x < m_columnSize;x++)
            {
                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }

            pDC->SelectObject(pOldPen);
            pDC->SelectObject(pOldBrush);
            break;

        case R_NORMAL:
            for (x = 0;x < m_columnSize;x++)
            {
	            rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, TRUE);
                pDC->FillSolidRect(rcItem, pMyLParam->GetBackColor(x));
                pDC->SetTextColor(pMyLParam->GetTextColor(x));
               
				if (m_withSingleRect)
                {
                    pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
                    pDC->Rectangle(rcItem);
                    pDC->SelectObject(pOldBrush);
                }

                rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, FALSE);
                pDC->DrawText(CListCtrl::GetItemText(lpDrawItemStruct->itemID,x), rcItem, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            }
            break;
    }

	x = 2;
	CString tmp = CListCtrl::GetItemText(lpDrawItemStruct->itemID,x);
	if (strcmp(tmp,"сп") == 0)
	{
		COLORREF color=GetItemData(lpDrawItemStruct->itemID);
		CBrush newbrush(color);
        pOldBrush = pDC->SelectObject(&newbrush);
        rcItem	= GetCellRect(lpDrawItemStruct->itemID, x, TRUE);
        pDC->FillSolidRect(rcItem, color);
        pDC->SelectObject(pOldBrush);
	}
}

void CColorListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);

    if (IsCreated) 
    {
        RedrawWindow(NULL,NULL,RDW_INVALIDATE);
    }
}

void CColorListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);	
}
