// ListCtrlCl.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VMListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CVMListCtrl
IMPLEMENT_DYNAMIC(CVMListCtrl, CListCtrl)

CVMListCtrl::CVMListCtrl()
{
	m_nRowHeight = 0;
	m_nFontHeight = 12;
	m_nFontWidth = 0;
	m_color = RGB(0,0,0);
}

CVMListCtrl::~CVMListCtrl()
{
	try
	{
		stColor * pColor = NULL;

		while(!m_ptrListRow.IsEmpty())
		{
			pColor = (stColor *)m_ptrListRow.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}
		
		while(!m_ptrListCol.IsEmpty())
		{
			pColor = (stColor *)m_ptrListCol.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		while(!m_ptrListItem.IsEmpty())
		{
			pColor = (stColor *)m_ptrListItem.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		while(!m_rowTextColor.IsEmpty())
		{
			pColor = (stColor *)m_rowTextColor.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		while(!m_colTextColor.IsEmpty())
		{
			pColor = (stColor *)m_colTextColor.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		while(!m_ItemTextColor.IsEmpty())
		{
			pColor = (stColor *)m_ItemTextColor.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}
	}
	catch(...)
	{
		TRACE("CVMListCtrl::~CVMListCtrl异常");
	}
}

BEGIN_MESSAGE_MAP(CVMListCtrl, CListCtrl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, &CVMListCtrl::OnLvnInsertitem)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CVMListCtrl::OnLvnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, &CVMListCtrl::OnLvnDeleteallitems)
END_MESSAGE_MAP()

// CVMListCtrl 消息处理程序

void CVMListCtrl::PreSubclassWindow()
{
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	
	CListCtrl::PreSubclassWindow();

	CHeaderCtrl *pHeader = GetHeaderCtrl();
	if (pHeader != NULL)
		m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

void CVMListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct == NULL)
		return;

	int ofst = 0;
	UINT uFormat = DT_CENTER ;
	TCHAR Buffer[256] = {0};
	TEXTMETRIC metric;
	COLORREF color;
	CFont NewFont;
	CFont *pOldFont = NULL;
	
	LV_ITEM lvi;
	LV_COLUMN lvc;
	LV_COLUMN lvcprev;

	memset(&metric,0,sizeof(metric));
	memset(&lvi,0, sizeof(lvi));
	memset(&lvc,0,sizeof(lvc));
	memset(&lvcprev,0,sizeof(lvcprev));

	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (pDC == NULL)
		return;
	
	CRect rtClient;
	GetClientRect(&rtClient);

	NewFont.CreateFont(m_nFontHeight,m_nFontWidth,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("宋体"));//创建字体 
	pOldFont = pDC->SelectObject(&NewFont);

	for ( int nCol = 0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			GetColumn(nCol-1, &lvcprev);
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;

		memset(&lvi,0,sizeof(lvi));
		memset(Buffer,0,sizeof(Buffer));

		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = Buffer;
		lvi.cchTextMax = sizeof(Buffer);
		
		if(!GetItem(&lvi))
			continue;

		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol == 0)
		{
			rcTemp.left -= 2;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_HIGHLIGHT)) ;
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			else if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			else if (FindRowColor(lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			else
			{
				color = GetBkColor();
				pDC->FillSolidRect(rcTemp,color);
			}
		}

		uFormat = DT_CENTER ;
		if (m_Header.m_strFormat[nCol] == '0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_strFormat[nCol] == '1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_strFormat[nCol] == '2')
		{
			uFormat = DT_RIGHT;
		}

		pDC->GetTextMetrics(&metric);
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);

		if (FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		else if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		else if (FindRowTextColor(lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		else
		{
			pDC->SetTextColor(m_color);
		}

		DrawText(lpDrawItemStruct->hDC, Buffer, (int)strlen(Buffer), &rcItem, uFormat) ;
	}

	pDC->SelectObject(pOldFont);
	NewFont.DeleteObject();
}

void CVMListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CVMListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight > 0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

int CVMListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_strHeadArray.Add(lpszColumnHeading);

	if (nFormat == LVCFMT_LEFT)
	{
		m_Header.m_strFormat = m_Header.m_strFormat + "0";
	}
	else if (nFormat == LVCFMT_CENTER)
	{
		m_Header.m_strFormat = m_Header.m_strFormat + "1";
	}
	else if (nFormat == LVCFMT_RIGHT)
	{
		m_Header.m_strFormat = m_Header.m_strFormat + "2";
	}
	else
	{
		m_Header.m_strFormat = m_Header.m_strFormat + "1";
	}

	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}

BOOL CVMListCtrl::DeleteColumn(int nCol)
{
	if (nCol >=0 &&nCol < m_Header.m_strHeadArray.GetCount())
		m_Header.m_strHeadArray.RemoveAt(nCol);

	return CListCtrl::DeleteColumn(nCol);
}

// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CVMListCtrl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CVMListCtrl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_fHeight = Height;
}

bool CVMListCtrl::FindRowColor(int row ,COLORREF &color)//查找行颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListRow.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListRow.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				break;
			}
		}

		return bFlag;

	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::FindRowByColor(int &row ,COLORREF color) //查找行颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListRow.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListRow.GetNext(pos);
			if (pColor != NULL&&pColor->rgb == color)
			{
				bFlag = true;
				row = pColor->nRow;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::FindColColor(int col,COLORREF &color) //查找列颜色
{
	bool bFlag = false;
	POSITION pos = m_ptrListCol.GetHeadPosition();

	while(pos != NULL)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor != NULL&&pColor->nCol == col)
		{
			bFlag = true;
			color = pColor->rgb;
			break;
		}
	}

	return bFlag;
}

bool CVMListCtrl::FindItemColor(int col,int row,COLORREF &color) //查找颜色
{
	bool bFlag = false;
	POSITION pos = m_ptrListItem.GetHeadPosition();

	while(pos != NULL)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor != NULL&&pColor->nCol==col&&pColor->nRow==row)
		{
			bFlag = true;
			color = pColor->rgb;
			break;
		}
	}
	
	return bFlag;
}

void CVMListCtrl::SetRowColor(int row,COLORREF color)//设置行颜色
{
	try
	{
		bool bResult = ModifyRowColor(row,color);
		if (bResult)
			return;
		
		stColor *pColor  = new stColor;
		if (pColor == NULL)
			return;

		pColor->nRow = row;
		pColor->rgb = color;

		m_ptrListRow.AddTail(pColor);
	}
	catch(...)
	{

	}
}

void CVMListCtrl::SetColColor(int col,COLORREF color) //设置列颜色
{
	try
	{
		bool bResult = ModifyColColor(col,color);
		if (bResult)
			return;

		stColor *pColor  = new stColor;
		if (pColor == NULL)
			return;

		pColor->nCol = col;
		pColor->rgb = color;

		m_ptrListCol.AddTail(pColor);
	}
	catch(...)
	{

	}
}

void CVMListCtrl::SetItemColor(int col,int row,COLORREF color) //设置格子颜色
{
	try
	{
		bool bResult = ModifyItemColor(col,row,color);
		if (bResult)
			return;

		stColor *pColor  = new stColor;
		if (pColor == NULL)
			return;

		pColor->nCol = col;
		pColor->nRow = row;
		pColor->rgb = color;

		m_ptrListItem.AddTail(pColor);
	}
	catch(...)
	{

	}
}

void CVMListCtrl::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);

	WINDOWPOS wp;

	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();

	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CVMListCtrl::SetHeaderFontHW(int nHeight,int nWidth) //设置头部字体宽和高
{
	m_Header.m_nFontHeight = nHeight;
	m_Header.m_nFontWith = nWidth;
}

void CVMListCtrl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}

BOOL CVMListCtrl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
	return TRUE;
}

void CVMListCtrl::SetFontHW(int nHeight,int nWidth) //设置字体高和宽
{
	m_nFontHeight = nHeight;
	m_nFontWidth = nWidth;
}

void CVMListCtrl::SetRowTextColor(int row,COLORREF color) //设置行文本颜色
{
	try
	{
		bool bResult = ModifyRowTextColor(row,color);
		if (bResult)
			return;

		stColor *pColor = new stColor;
		if (pColor == NULL)
			return;

		pColor->nRow = row;
		pColor->rgb = color;
		m_rowTextColor.AddTail(pColor);
	}
	catch(...)
	{

	}
}

void CVMListCtrl::SetColTextColor(int col,COLORREF color)
{
	try
	{
		bool bResult = ModifyColTextColor(col,color);
		if (bResult)
			return;

		stColor *pColor = new stColor;
		if (pColor == NULL)
			return;

		pColor->nCol = col;
		pColor->rgb = color;
		m_colTextColor.AddTail(pColor);
	}
	catch(...)
	{

	}
}

bool CVMListCtrl::FindRowTextColor(int row,COLORREF &color)//查找行字体颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_rowTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_rowTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::FindRowByTextColor(int &row,COLORREF color)//查找行字体颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_rowTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_rowTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->rgb == color)
			{
				bFlag = true;
				row = pColor->nRow;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::FindColTextColor(int col,COLORREF &color)
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_colTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nCol==col)
			{
				bFlag = true;
				color = pColor->rgb;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::FindItemTextColor(int col,int row,COLORREF &color)
{
	try
	{
		bool bFlag = false;
		stColor *pColor  = NULL;

		POSITION pos = m_ItemTextColor.GetHeadPosition();

		while(pos != NULL)
		{
			pColor = (stColor*)m_ItemTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nCol==col&&pColor->nRow==row)
			{
				bFlag = true;
				color = pColor->rgb;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyRowColor(int row,COLORREF color)//修改行颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListRow.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListRow.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyRowColor(int row,int new_row,COLORREF &color)//修改行颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListRow.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListRow.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				pColor->nRow = new_row;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyColColor(int col,COLORREF color)//修改列颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListCol.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
			if (pColor != NULL&&pColor->nCol == col)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyItemColor(int col,int row,COLORREF color)//修改item颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListItem.GetHeadPosition();

		while(pos != NULL)
		{
			stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
			if (pColor != NULL&&pColor->nCol==col&&pColor->nRow==row)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyRowTextColor(int row,COLORREF color)//修改行文字颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_rowTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_rowTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::ModifyRowTextColor(int row,int new_row,COLORREF &color)//修改行文字颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_rowTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_rowTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				pColor->nRow = new_row;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::ModifyColTextColor(int col,COLORREF color)//修改列文字颜色
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_colTextColor.GetHeadPosition();

		while(pos!=NULL)
		{
			stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nCol==col)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::ModifyItemTextColor(int col,int row,COLORREF color)//修改item文字颜色
{
	try
	{
		bool bFlag = false;
		stColor *pColor  = NULL;

		POSITION pos = m_ItemTextColor.GetHeadPosition();

		while(pos != NULL)
		{
			pColor = (stColor*)m_ItemTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nCol == col&&pColor->nRow == row)
			{
				bFlag = true;
				pColor->rgb = color;
				break;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}

	return false;
}

void CVMListCtrl::SetItemTextColor(int col,int row,COLORREF color)
{
	try
	{
		bool bResult = ModifyItemTextColor(col,row,color);
		if (bResult)
			return;

		stColor *pColor = new stColor;
		if (pColor == NULL)
			return;

		pColor->nCol = col;
		pColor->nRow = row;
		pColor->rgb = color;

		m_ItemTextColor.AddTail(pColor);
	}
	catch(...)
	{

	}
}

bool CVMListCtrl::DeleteRowColor(int row,COLORREF &color)
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_ptrListRow.GetHeadPosition();
		POSITION pos1 = NULL;

		while(pos != NULL)
		{
			pos1 = pos;
			stColor *pColor = (stColor*)m_ptrListRow.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				m_ptrListRow.RemoveAt(pos1);
				delete pColor;
				pColor = NULL;
			}
		}

		return bFlag;
	}
	catch(...)
	{

	}
	return false;
}

bool CVMListCtrl::DeleteAllRowColor()
{
	try
	{
		stColor * pColor = NULL;

		while(!m_ptrListRow.IsEmpty())
		{
			pColor = (stColor *)m_ptrListRow.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		return true;
	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::DeleteRowTextColor(int row,COLORREF &color)
{
	try
	{
		bool bFlag = false;
		POSITION pos = m_rowTextColor.GetHeadPosition();
		POSITION pos1;

		while(pos != NULL)
		{
			pos1 = pos;
			stColor *pColor = (stColor*)m_rowTextColor.GetNext(pos);
			if (pColor != NULL&&pColor->nRow == row)
			{
				bFlag = true;
				color = pColor->rgb;
				m_rowTextColor.RemoveAt(pos1);
				delete pColor;
				pColor = NULL;
			}
		}
		
		return bFlag;
	}
	catch(...)
	{

	}

	return false;
}

bool CVMListCtrl::DeleteAllRowTextColor()
{
	try
	{
		stColor * pColor = NULL;

		while(!m_rowTextColor.IsEmpty())
		{
			pColor = (stColor *)m_rowTextColor.RemoveTail();
			if (pColor != NULL)
			{
				delete pColor;
				pColor = NULL;
			}
		}

		return true;
	}
	catch(...)
	{

	}

	return false;
}

void CVMListCtrl::OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//////////////////////////////////////////////////////////////////////////
	if (pNMLV == NULL)
		return;

	try
	{
		COLORREF color = RGB(0,0,0);
		bool bResult = false;

		int nItemIndex = pNMLV->iItem;

		int nCount = GetItemCount();

		for (int i = nCount - 2;i >= nItemIndex;i--)
		{
			bResult = FindRowColor(i,color);
			if (bResult)
			{
				ModifyRowColor(i,i+1,color);
			}

			bResult = FindRowTextColor(i,color);
			if (bResult)
			{
				ModifyRowTextColor(i,i+1,color);
			}
		}
	}
	catch(...)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}


void CVMListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//////////////////////////////////////////////////////////////////////////
	if (pNMLV == NULL)
		return;

	try
	{
		COLORREF color = RGB(0,0,0);
		bool bResult = false;

		int nItemIndex = pNMLV->iItem;

		int nCount = GetItemCount();

		DeleteRowColor(nItemIndex,color);
		DeleteRowTextColor(nItemIndex,color);

		for (int i = nItemIndex+1;i < nCount;i++)
		{
			bResult = FindRowColor(i,color);
			if (bResult)
			{
				ModifyRowColor(i,i-1,color);
			}

			bResult = FindRowTextColor(i,color);
			if (bResult)
			{
				ModifyRowTextColor(i,i-1,color);
			}
		}
	}
	catch(...)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}

void CVMListCtrl::OnLvnDeleteallitems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//////////////////////////////////////////////////////////////////////////

	try
	{
		DeleteAllRowColor();
		DeleteAllRowTextColor();
	}
	catch(...)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}
