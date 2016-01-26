// SkinButton.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SkinButton.h"
#include "EnBitmap.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CToolTipCtrl g_ToolTip;
/////////////////////////////////////////////////////////////////////////////
// CSkinButton

CSkinButton::CSkinButton()
{
	m_bMouseIn = m_bDown = FALSE;
	m_nStyle=STYLE_BITMAP;
}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers

void CSkinButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_bFocus = GetFocus()->GetSafeHwnd() == m_hWnd;
	m_bEnable = !(GetWindowLong(m_hWnd,GWL_STYLE) & WS_DISABLED);
	CEnBitmap bmp;

	if (m_nStyle & STYLE_BITMAP)
	{
		if ( !m_bEnable )
		{
			//disable
			m_bmpButton[m_index].DrawImage(bmp,4,1,4,1);	
		}
		else if ( m_bMouseIn &&m_bDown )
		{
			//down
			m_bmpButton[m_index].DrawImage(bmp,3,1,4,1);	
		}
		else if ( m_bMouseIn )
		{
			//hover
			m_bmpButton[m_index].DrawImage(bmp,2,1,4,1);		
		}
		else if ( m_bFocus )
		{
			//normal with focus
			m_bmpButton[m_index].DrawImage(bmp,1,1,4,1);	
		}
		else
		{
			//normal
			m_bmpButton[m_index].DrawImage(bmp,1,1,4,1);	
		}

		//设置区域
		SetWindowRgn(bmp.BitmapToRegion(RGB(255,0,255)), TRUE );	
		HRGN hRgn = CreateRectRgn( 0, 0, 0, 0);
		GetWindowRgn(hRgn);	

		//选入位图绘画区域
		::SelectClipRgn(dc.GetSafeHdc(), hRgn);
		bmp.Draw( &dc, CRect(0,0,bmp.GetWidth(),bmp.GetHeight()));	
		dc.SelectClipRgn(NULL);
		DeleteObject(hRgn);

		DrawText( &dc ,bmp.GetRect() ,RGB(0,0,0));

	}

	else if (m_nStyle & STYLE_TOOLBAR)
	{
		CRect rcButton;
		GetClientRect(&rcButton);
		CRect rcImage,rcText;
		rcImage=m_bmpButton[m_index].GetRect(); 
		int nMaigin=(rcButton.Height()- rcImage.Height())/2; 
		rcImage.OffsetRect(nMaigin,nMaigin); 
		rcText=rcButton;
		rcText.left =rcImage.right ;


		//先更新背景
		CRect rc=rcButton;		
		ClientToScreen(&rcButton);
		GetParent()->ScreenToClient(&rcButton); 
		GetParent()->RedrawWindow(&rcButton,NULL,RDW_ERASE|RDW_UPDATENOW |RDW_INVALIDATE|RDW_NOCHILDREN   );		
		rc.DeflateRect(1,1);
		
		if ( !m_bEnable )
		{
			//disable	
		}
		else if ( m_bMouseIn &&m_bDown )
		{
			//down
			dc.Draw3dRect(&rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));	
		}
		else if ( m_bMouseIn )
		{
			dc.Draw3dRect(&rc,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_3DDKSHADOW));		
		}
		else if ( m_bFocus )
		{
		}
		else
		{
		}

		m_bmpButton[m_index].TransparentBlt(dc, &rcImage,RGB(255,0,255));	
		DrawText( &dc ,rcText,m_crText);
	}
}

void CSkinButton::LoadBitmap(LPCTSTR szImagePath,int index)
{
	m_index = index;
	m_bmpButton[m_index].LoadImage(szImagePath); 
}

BOOL CSkinButton::LoadBitmap(UINT uIDRes,int index,COLORREF crBack)
{
	m_index = index;
	return m_bmpButton[m_index].LoadBitmap(uIDRes);
}

void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bDown = TRUE;
	m_bMouseIn = TRUE;
	Invalidate();
	UpdateWindow();
	SetFocus();	

	if (GetDlgCtrlID() == IDC_YT_UP ||
		GetDlgCtrlID() == IDC_YT_DOWN ||
		GetDlgCtrlID() == IDC_YT_LEFT ||
		GetDlgCtrlID() == IDC_YT_RIGHT ||
		GetDlgCtrlID() == IDC_YT_ZOOMIN ||
		GetDlgCtrlID() == IDC_YT_ZOOMOUT ||
		GetDlgCtrlID() == IDC_YT_SAVE ||
		GetDlgCtrlID() == IDC_YT_CALL ||
		GetDlgCtrlID() == IDC_YT_DIAPHRAGM_LARGE ||
		GetDlgCtrlID() == IDC_YT_DIAPHRAGM_SMALL ||
		GetDlgCtrlID() == IDC_BUTTON_HW_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_HW_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_JR_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_JR_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_L_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_L_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_YS_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_YS_CLOSE ||
		GetDlgCtrlID() == IDC_YT_LOCK)
	{
		switch (GetDlgCtrlID())
		{
		case IDC_YT_UP:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 1, 1);
			break;
		case IDC_YT_DOWN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 2, 1);
			break;
		case IDC_YT_LEFT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 3, 1);
			break;
		case IDC_YT_RIGHT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 4, 1);
			break;
		case IDC_YT_ZOOMIN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 5, 1);
			break;
		case IDC_YT_ZOOMOUT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 6, 1);
			break;
		case IDC_YT_SAVE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 7, 1);
			break;
		case IDC_YT_CALL:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 8, 1);
			break;
		case IDC_YT_DIAPHRAGM_LARGE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 9, 1);
			break;
		case IDC_YT_DIAPHRAGM_SMALL:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 10, 1);
			break;
		case IDC_BUTTON_HW_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 11, 1);
			break;
		case IDC_BUTTON_HW_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 12, 1);
			break;
		case IDC_BUTTON_JR_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 13, 1);
			break;
		case IDC_BUTTON_JR_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 14, 1);
			break;
		case IDC_BUTTON_L_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 15, 1);
			break;
		case IDC_BUTTON_L_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 16, 1);
			break;
		case IDC_BUTTON_YS_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 17, 1);
			break;
		case IDC_BUTTON_YS_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 18, 1);
			break;
		case IDC_YT_LOCK:
			{
				AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 19, 1);
				return;

				if (g_pMainDlg != NULL&&g_pMainDlg->m_iFocuseWindowID >= 0&&g_pMainDlg->m_iFocuseWindowID < MAXVIEWCH)
				{
					if (m_index == 0)
					{
						g_DlgVideoView[g_pMainDlg->m_iFocuseWindowID].StartYtLock();
						AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 19, 1);
					}
					else
					{
						g_DlgVideoView[g_pMainDlg->m_iFocuseWindowID].StopYtLock();
						AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 19, 0);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();	
	m_bDown = FALSE;

	if (GetDlgCtrlID() == IDC_YT_UP ||
		GetDlgCtrlID() == IDC_YT_DOWN ||
		GetDlgCtrlID() == IDC_YT_LEFT ||
		GetDlgCtrlID() == IDC_YT_RIGHT ||
		GetDlgCtrlID() == IDC_YT_ZOOMIN ||
		GetDlgCtrlID() == IDC_YT_ZOOMOUT ||
		GetDlgCtrlID() == IDC_YT_SAVE ||
		GetDlgCtrlID() == IDC_YT_CALL ||
		GetDlgCtrlID() == IDC_YT_DIAPHRAGM_LARGE ||
		GetDlgCtrlID() == IDC_YT_DIAPHRAGM_SMALL ||
		GetDlgCtrlID() == IDC_BUTTON_HW_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_HW_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_JR_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_JR_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_L_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_L_CLOSE ||
		GetDlgCtrlID() == IDC_BUTTON_YS_OPEN ||
		GetDlgCtrlID() == IDC_BUTTON_YS_CLOSE ||
		GetDlgCtrlID() == IDC_YT_LOCK)
	{
		switch (GetDlgCtrlID())
		{
		case IDC_YT_UP:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 1, 0);
			break;
		case IDC_YT_DOWN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 2, 0);
			break;
		case IDC_YT_LEFT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 3, 0);
			break;
		case IDC_YT_RIGHT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 4, 0);
			break;
		case IDC_YT_ZOOMIN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 5, 0);
			break;
		case IDC_YT_ZOOMOUT:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 6, 0);
			break;
		case IDC_YT_SAVE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 7, 0);
			break;
		case IDC_YT_CALL:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 8, 0);
			break;
		case IDC_YT_DIAPHRAGM_LARGE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 9, 0);
			break;
		case IDC_YT_DIAPHRAGM_SMALL:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 10, 0);
			break;
		case IDC_BUTTON_HW_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 11, 0);
			break;
		case IDC_BUTTON_HW_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 12, 0);
			break;
		case IDC_BUTTON_JR_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 13, 0);
			break;
		case IDC_BUTTON_JR_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 14, 0);
			break;
		case IDC_BUTTON_L_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 15, 0);
			break;
		case IDC_BUTTON_L_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 16, 0);
			break;
		case IDC_BUTTON_YS_OPEN:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 17, 0);
			break;
		case IDC_BUTTON_YS_CLOSE:
			AfxGetMainWnd()->PostMessage(OM_YTCONTROL, 18, 0);
			break;
		case IDC_YT_LOCK:
			{
				if (m_index == 0)
				{
					SetIndex(1);
					Invalidate(TRUE);
				}
				else
				{
					SetIndex(0);
					Invalidate(TRUE);
				}
			}
			break;
		default:
			break;
		}
	}

	if ( m_bMouseIn )
	{
		m_bMouseIn = FALSE;
		//GetRegion();
		Invalidate();
		UpdateWindow();
		GetParent()->SendMessage( WM_COMMAND, GetDlgCtrlID(), (LPARAM)m_hWnd );
	}
	else
	{
		//GetRegion();
		Invalidate();
		UpdateWindow();
	}	
}

BOOL CSkinButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nStyle & STYLE_BITMAP)
	{
		HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);		

		if ( PtInRegion( hRgn, point.x, point.y ))
		{
			::SetCursor(AfxGetApp()->LoadCursor(IDC_MY_HAND));
			if ( !m_bMouseIn)
			{
				m_bMouseIn = TRUE;
				Invalidate();
				UpdateWindow();
				SetCapture();
			}
		}
		else
		{
			if ( m_bMouseIn)
			{
				m_bMouseIn = FALSE;
				//GetRegion();
				Invalidate();
				UpdateWindow();
				if ( !m_bDown )
					ReleaseCapture();
			}

		}
		DeleteObject(hRgn);
	}
	else if (m_nStyle & STYLE_TOOLBAR)
	{
		CRect rc;
		GetClientRect(&rc);
		if ( rc.PtInRect( point ))
		{
			//移入
			if ( !m_bMouseIn)
			{
				m_bMouseIn = TRUE;

				Invalidate();
				UpdateWindow();
				SetCapture();
			}
		}
		else
		{
			//移出
			if ( m_bMouseIn)
			{
				m_bMouseIn = FALSE;
				//GetRegion();
				Invalidate();
				UpdateWindow();
				if ( !m_bDown )
					ReleaseCapture();
			}

		}
	}
}

void CSkinButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
}

void CSkinButton::OnSetFocus(CWnd* pOldWnd) 
{
	
}

void CSkinButton::OnKillFocus(CWnd* pNewWnd) 
{
	
}

BOOL CSkinButton::DrawText(CDC *pDC,CRect rc ,COLORREF crText)
{
	CRect r;
	CString str;
	CRect rcButton;
	CString strText;
	UINT nId=GetDlgCtrlID( ) ;
	if (strText.LoadString(nId))
	{
		CFont *ofont;
		ofont = pDC->SelectObject( GetParent()->GetFont() );

		pDC->SetTextColor(crText);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText( strText, rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

		pDC->SelectObject(ofont);
	}

	return TRUE;
}

HBITMAP CSkinButton::SetBitmap(HBITMAP hBitmap)
{ 
	return m_bmpButton[m_index].SetBitmap(hBitmap) ;	
}

void CSkinButton::SetTextColor(COLORREF cr)
{
	m_crText=cr;
}

int  CSkinButton::SetIndex(int index)
{
	int temp = m_index;
	m_index = index;
	return temp;
}
