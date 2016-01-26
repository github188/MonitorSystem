// PropertyList.cpp : implementation file
//

#include "stdafx.h"
//#include "PropListBox.h"
#include "canvas/PropertyList.h"
#include "dialog/FontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyList

CPropertyList::CPropertyList()
{
}

CPropertyList::~CPropertyList()
{
}


BEGIN_MESSAGE_MAP(CPropertyList, CListBox)
	//{{AFX_MSG_MAP(CPropertyList)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(IDC_PROPCMBBOX, OnKillfocusCmbBox)
	ON_CBN_SELCHANGE(IDC_PROPCMBBOX, OnSelchangeCmbBox)
	ON_EN_KILLFOCUS(IDC_PROPEDITBOX, OnKillfocusEditBox)
	ON_EN_CHANGE(IDC_PROPEDITBOX, OnChangeEditBox)
	ON_BN_CLICKED(IDC_PROPBTNCTRL, OnButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyList message handlers

BOOL CPropertyList::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CListBox::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style |= LBS_OWNERDRAWFIXED;

	m_bTracking = FALSE;
	m_nDivider = 0;
	m_bDivIsSet = FALSE;

	return TRUE;
}

void CPropertyList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 20; //pixels
}


void CPropertyList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rectFull = lpDIS->rcItem;
	CRect rect = rectFull;
	if (m_nDivider==0)
		m_nDivider = rect.Width() / 2;
	rect.left = m_nDivider;
	CRect rect2 = rectFull;
	rect2.right = rect.left - 1;
	UINT nIndex = lpDIS->itemID;

	if (nIndex != (UINT) -1)
	{
		//draw two rectangles, one for each row column
		dc.FillSolidRect(rect2,RGB(192,192,192));
		dc.DrawEdge(rect2,EDGE_SUNKEN,BF_BOTTOMRIGHT);
		dc.DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);

		//get the CPropertyItem for the current row
		STRUCT_PROPERTYITEM* pItem = (STRUCT_PROPERTYITEM*) GetItemDataPtr(nIndex);

		//write the property name in the first rectangle
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(pItem->hint,CRect(rect2.left+3,rect2.top+3,
											rect2.right-3,rect2.bottom+3),
					DT_LEFT | DT_SINGLELINE);

		//write the initial property value in the second rectangle
		//dc.FillSolidRect(rect,RGB(0,0,255));
		if (pItem->type==PIT_FONT)
		{
			COLORREF oldcolor=dc.GetTextColor();
			dc.SetTextColor(pItem->color);
			dc.DrawText(pItem->font.lfFaceName,CRect(rect.left+3,rect.top+3,
											rect.right+3,rect.bottom+3),
					DT_LEFT | DT_SINGLELINE);
			dc.SetTextColor(oldcolor);
		}
		else
		{
			if (pItem->type==PIT_COLOR)
			{
				dc.FillSolidRect(CRect(rect.left+3,rect.top+3,
											rect.right-3,rect.bottom-3),pItem->color);
			}
			else
			{
				dc.DrawText(pItem->value,CRect(rect.left+3,rect.top+3,
											rect.right+3,rect.bottom+3),
					DT_LEFT | DT_SINGLELINE);
			}
		}
	}
	dc.Detach();
}

int CPropertyList::AddItem(CString txt)
{
	int nIndex = AddString(txt);
	return nIndex;
}

int CPropertyList::AddPropItem(STRUCT_PROPERTYITEM* pItem)
{
	int nIndex = AddString(_T(""));
	SetItemDataPtr(nIndex,pItem);
	return nIndex;
}

int CPropertyList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bDivIsSet = FALSE;
	m_nDivider = 0;
	m_bTracking = FALSE;

	m_hCursorSize = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_SSerif8Font.CreatePointFont(80,_T("MS Sans Serif"));

	return 0;
}

void CPropertyList::OnSelchange() 
{
	CRect rect;
	CString lBoxSelText;

	//m_curSel = GetCurSel();

	GetItemRect(m_curSel,rect);
	rect.left = m_nDivider;

	STRUCT_PROPERTYITEM* pItem = (STRUCT_PROPERTYITEM*) GetItemDataPtr(m_curSel);

	if (m_btnCtrl)
		m_btnCtrl.ShowWindow(SW_HIDE);

	if (pItem->type==PIT_COMBO)
	{
		//display the combo box.  If the combo box has already been
		//created then simply move it to the new location, else create it
		m_nLastBox = 0;
		if (m_cmbBox)
			m_cmbBox.MoveWindow(rect);
		else
		{	
			rect.bottom += 100;
			m_cmbBox.Create(CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT | WS_VISIBLE | WS_CHILD | WS_BORDER,
							rect,this,IDC_PROPCMBBOX);
			m_cmbBox.SetFont(&m_SSerif8Font);
		}

		//add the choices for this particular property
		CString cmbItems = pItem->info;
		lBoxSelText = pItem->value;
		
		m_cmbBox.ResetContent();
		//m_cmbBox.AddString("");		
		int i,i2;
		i=0;
		while ((i2=cmbItems.Find('|',i)) != -1)
		{
			m_cmbBox.AddString(cmbItems.Mid(i,i2-i));
			i=i2+1;
		}

		m_cmbBox.ShowWindow(SW_SHOW);
		m_cmbBox.SetFocus();

		//jump to the property's current value in the combo box
		int j = m_cmbBox.FindStringExact(0,lBoxSelText);
		if (j != CB_ERR)
			m_cmbBox.SetCurSel(j);
		else
			m_cmbBox.SetCurSel(0);
	}
	else if (pItem->type==PIT_EDIT)
	{
		//display edit box
		m_nLastBox = 1;
		m_prevSel = m_curSel;
		rect.bottom -= 3;
		if (m_editBox)
			m_editBox.MoveWindow(rect);
		else
		{	
			m_editBox.Create(ES_LEFT | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER,
							rect,this,IDC_PROPEDITBOX);
			m_editBox.SetFont(&m_SSerif8Font);
		}

		lBoxSelText = pItem->value;
		int limited=atoi(pItem->info);
		m_editBox.LimitText(limited);

		m_editBox.ShowWindow(SW_SHOW);
		m_editBox.SetFocus();
		//set the text in the edit box to the property's current value
		m_editBox.SetWindowText(lBoxSelText);
	}
	else
		DisplayButton(rect);
}

void CPropertyList::DisplayButton(CRect region)
{
	//displays a button if the property is a file/color/font chooser
	m_nLastBox = 2;
	m_prevSel = m_curSel;

	if (region.Width() > 25)
		region.left = region.right - 25;
	region.bottom -= 3;

	if (m_btnCtrl)
		m_btnCtrl.MoveWindow(region);
	else
	{	
		m_btnCtrl.Create("...",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
						region,this,IDC_PROPBTNCTRL);
		m_btnCtrl.SetFont(&m_SSerif8Font);
	}

	m_btnCtrl.ShowWindow(SW_SHOW);
	m_btnCtrl.SetFocus();
}

void CPropertyList::OnKillFocus(CWnd* pNewWnd) 
{
	//m_btnCtrl.ShowWindow(SW_HIDE);

	CListBox::OnKillFocus(pNewWnd);
}

void CPropertyList::OnKillfocusCmbBox() 
{
	m_cmbBox.ShowWindow(SW_HIDE);

	Invalidate();
}

void CPropertyList::OnKillfocusEditBox()
{
	CString newStr;
	m_editBox.ShowWindow(SW_HIDE);

	Invalidate();
}

void CPropertyList::OnSelchangeCmbBox()
{
	CString selStr;
	if (m_cmbBox)
	{
		m_cmbBox.GetLBText(m_cmbBox.GetCurSel(),selStr);
		STRUCT_PROPERTYITEM * pItem = (STRUCT_PROPERTYITEM*) GetItemDataPtr(m_curSel);
		sprintf(pItem->value,selStr);
	}
}

void CPropertyList::OnChangeEditBox()
{
	CString newStr;
	m_editBox.GetWindowText(newStr);
	
	STRUCT_PROPERTYITEM* pItem = (STRUCT_PROPERTYITEM*) GetItemDataPtr(m_curSel);
	sprintf(pItem->value , newStr);

}

void CPropertyList::OnButton()
{
	STRUCT_PROPERTYITEM* pItem = (STRUCT_PROPERTYITEM*) GetItemDataPtr(m_curSel);

	//display the appropriate common dialog depending on what type
	//of chooser is associated with the property
	if (pItem->type == PIT_COLOR)
	{
		COLORREF initClr;
		CString currClr = pItem->value;
		//parse the property's current color value
		if (currClr.Find("RGB") > -1)
		{
			int j = currClr.Find(',',3);
			CString bufr = currClr.Mid(4,j-4);
			int RVal = atoi(bufr);
			int j2 = currClr.Find(',',j+1);
			bufr = currClr.Mid(j+1,j2-(j+1));
			int GVal = atoi(bufr);
			int j3 = currClr.Find(')',j2+1);
			bufr = currClr.Mid(j2+1,j3-(j2+1));
			int BVal = atoi(bufr);
			initClr = RGB(RVal,GVal,BVal);
		}
		else
			initClr = 0;
		initClr=pItem->color;
		
		CColorDialog ClrDlg(initClr);
		
		if (IDOK == ClrDlg.DoModal())
		{
			COLORREF selClr = ClrDlg.GetColor();
			CString clrStr;
			clrStr.Format("RGB(%d,%d,%d)",GetRValue(selClr),
						GetGValue(selClr),GetBValue(selClr));
			m_btnCtrl.ShowWindow(SW_HIDE);

			sprintf(pItem->value , clrStr);
			pItem->color=selClr;
			Invalidate();
		}
	}
	else if (pItem->type == PIT_FILE)
	{
		CString SelectedFile; 
		CString Filter("Gif Files (*.gif)|*.gif||");
	
		CFileDialog FileDlg(TRUE, NULL, NULL, NULL,
			Filter);
		
		CString currPath = pItem->value;
		FileDlg.m_ofn.lpstrTitle = "Select file";
		if (currPath.GetLength() > 0)
			FileDlg.m_ofn.lpstrInitialDir = currPath.Left(
				currPath.GetLength() - currPath.ReverseFind('\\'));

		if(IDOK == FileDlg.DoModal())
		{
			SelectedFile = FileDlg.GetPathName();
			
			m_btnCtrl.ShowWindow(SW_HIDE);
			
			sprintf(pItem->value , SelectedFile);
			Invalidate();
		}
	}
	else if (pItem->type == PIT_FONT)
	{	
		CFontDialog FontDlg(&pItem->font,CF_EFFECTS | CF_SCREENFONTS,NULL,this);
		FontDlg.m_cf.lpLogFont=&(pItem->font);
		FontDlg.m_cf.rgbColors=pItem->color;
		
		if(IDOK == FontDlg.DoModal())
		{
			CString faceName = FontDlg.GetFaceName();
			
			m_btnCtrl.ShowWindow(SW_HIDE);
			
			sprintf(pItem->value , faceName);
			pItem->color=FontDlg.GetColor();
			CClientDC dc(this);
			CDC memdc;
			memdc.CreateCompatibleDC(&dc);
			CFont m_font;
			LOGFONT tempfont;
			tempfont=pItem->font;
			m_font.CreateFontIndirect(&tempfont);
			CFont * m_oldfont=memdc.SelectObject(&m_font);
			CSize size;
			size=memdc.GetOutputTextExtent("A",1);//+2);
			memdc.SelectObject(m_oldfont);
			pItem->font.lfWidth=size.cx;
			pItem->font.lfHeight=size.cy;
			Invalidate();
		}
/*		CPropertySheet Sheet("×ÖÌå/ÑÕÉ«ÉèÖÃ");
		CFontDlg FontDlg;
		FontDlg.SetFontDlg(&pItem->font,true,true,pItem->color);
		Sheet.AddPage(&FontDlg);
		
		if(IDOK == Sheet.DoModal())
		{
		
			m_btnCtrl.ShowWindow(SW_HIDE);
			
			pItem->color=FontDlg.GetTextColor();
			FontDlg.GetCurLogfont(&(pItem->font));
			Invalidate();
		}*/
	}
}

void CPropertyList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		//if columns were being resized then this indicates
		//that mouse is up so resizing is done.  Need to redraw
		//columns to reflect their new widths.
		
		m_bTracking = FALSE;
		//if mouse was captured then release it
		if (GetCapture()==this)
			::ReleaseCapture();

		::ClipCursor(NULL);

		CClientDC dc(this);
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		//set the divider position to the new value
		m_nDivider = point.x;

		//redraw
		Invalidate();
	}
	else
	{
		BOOL loc;
		int i = ItemFromPoint(point,loc);
		m_curSel = i;
		CListBox::OnLButtonUp(nFlags, point);
	}
}

void CPropertyList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((point.x>=m_nDivider-5) && (point.x<=m_nDivider+5))
	{
		//if mouse clicked on divider line, then start resizing

		::SetCursor(m_hCursorSize);

		CRect windowRect;
		GetWindowRect(windowRect);
		windowRect.left += 10; windowRect.right -= 10;
		//do not let mouse leave the list box boundary
		::ClipCursor(windowRect);
		
		if (m_cmbBox)
			m_cmbBox.ShowWindow(SW_HIDE);
		if (m_editBox)
			m_editBox.ShowWindow(SW_HIDE);

		CRect clientRect;
		GetClientRect(clientRect);

		m_bTracking = TRUE;
		m_nDivTop = clientRect.top;
		m_nDivBtm = clientRect.bottom;
		m_nOldDivX = point.x;

		CClientDC dc(this);
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));

		//capture the mouse
		SetCapture();
	}
	else
	{
		m_bTracking = FALSE;
		CListBox::OnLButtonDown(nFlags, point);
	}
}

void CPropertyList::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_bTracking)
	{
		//move divider line to the mouse pos. if columns are
		//currently being resized
		CClientDC dc(this);
		//remove old divider line
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));
		//draw new divider line
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		m_nOldDivX = point.x;
	}
	else if ((point.x >= m_nDivider-5) && (point.x <= m_nDivider+5))
		//set the cursor to a sizing cursor if the cursor is over the row divider
		::SetCursor(m_hCursorSize);
	else
		CListBox::OnMouseMove(nFlags, point);
}

void CPropertyList::InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo)
{
	int nOldMode = pDC->SetROP2(R2_NOT);
	
	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo);

	pDC->SetROP2(nOldMode);
}

void CPropertyList::PreSubclassWindow() 
{
	m_bDivIsSet = FALSE;
	m_nDivider = 0;
	m_bTracking = FALSE;
	m_curSel = 1;

	m_hCursorSize = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_SSerif8Font.CreatePointFont(80,_T("MS Sans Serif"));
}

void CPropertyList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

	RedrawWindow(NULL,NULL,RDW_INVALIDATE);

	Invalidate();
}
