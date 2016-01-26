// ElecMapView.cpp : 实现文件
//

#include "stdafx.h"
#include "SDDElecMap.h"
#include "ElecMapView.h"
#include "ElecMapDoc.h"
#include "SDDElecMapDlg.h"

#include "canvas/TyTool.h"
#include "canvas/HuaMian.h"
#include "canvas/HMLayer.h"
#include "canvas/HMProject.h"
#include "canvas/YX.h"
#include "canvas/YSH.h"

#include "gloable/FileRead.h"
// CElecMapView
CLIPFORMAT CElecMapView::c_clipTy = (CLIPFORMAT)::RegisterClipboardFormat(_T("ElecMap_ty"));


IMPLEMENT_DYNCREATE(CElecMapView, CScrollView)

CElecMapView::CElecMapView():
m_fScale(1)
,m_xStart(0)
,m_yStart(0)
,m_xEnd(100)
,m_yEnd(100)
,m_nScrollMin(16)
,m_nHScrollPos(0)
,m_nVScrollPos(0)
,m_bActive(TRUE)
,m_bSetTimer(FALSE)
,m_bTracking(FALSE)
{
	initial();
}

CElecMapView::~CElecMapView()
{
}

BEGIN_MESSAGE_MAP(CElecMapView, CView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_ACTIVATE()
	ON_WM_TIMER()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_COMMAND_RANGE( MENU_MESSAGE_COMMAND, MENU_MESSAGE_COMMAND+MENU_MESSAGE_MAXCOUNT, &CElecMapView::OnPostMessageRMenu)
END_MESSAGE_MAP()

int CElecMapView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
	{
		return -1;
	}

	CClientDC dc(this);
	int w,h;
	w= GetSystemMetrics(SM_CXVIRTUALSCREEN);
	h= GetSystemMetrics(SM_CYVIRTUALSCREEN);

	m_memDC.create(&dc,w,h);

	//tip message
	m_ctrlTipMsg.Create(this);
	m_ctrlTipMsg.Activate(TRUE);
	m_ctrlTipMsg.SetDelayTime(TTDT_INITIAL,1000);
	m_ctrlTipMsg.SetDelayTime(TTDT_AUTOPOP,1000);
	m_ctrlTipMsg.Update();
	m_rectTipMsgPt = CRect(0,0,w,h);
	m_ctrlTipMsg.AddTool(this, "", m_rectTipMsgPt, IDC_TOOLTIP);

	return 0;
}

BOOL CElecMapView::DestroyWindow() 
{

	return CView::DestroyWindow();
}

BOOL CElecMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style=cs.style|WS_HSCROLL|WS_VSCROLL;

	return CView::PreCreateWindow(cs);
}

void CElecMapView::OnInitialUpdate()
{
	InitHScroll();
	InitVScroll();
}

BOOL CElecMapView::initial()
{
	m_bitmap = new CBitmap;
	m_bitmap->LoadBitmap(IDB_BITMAP_JSPROVINCE);

	//initial menu.
	m_pMainMenu.CreatePopupMenu();
	m_pSubMenu.CreatePopupMenu();

	return TRUE;
}

void CElecMapView::OnTimer(UINT_PTR nIDEvent)
{
	stopUpdateView();
	if (nIDEvent ==WM_TIMER_UPDATEVIEW_ID){
		Invalidate();
	}
}

// CElecMapView 绘图

void CElecMapView::updateViewWindow()
{
	Invalidate();
}

void CElecMapView::startUpdateView()
{
	SetTimer(WM_TIMER_UPDATEVIEW_ID,WM_TIMER_UPDATEVIEW_VALUE,NULL);
}

void CElecMapView::stopUpdateView()
{
	KillTimer(WM_TIMER_UPDATEVIEW_ID);
}

void CElecMapView::OnDraw(CDC* pDC)
{
	CElecMapDoc* pDoc = GetDocument();
	ASSERT(pDoc!=NULL);

	CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
	if (!pHuamian)
		return;
	ASSERT(pHuamian!=NULL);

	CSize sz = pHuamian->GetHMSize();
	ASSERT(m_fScale != 0);

	m_xEnd = __min((float)sz.cx,(m_xStart+m_wScreen/m_fScale));
	m_yEnd = __min((float)sz.cy,m_yStart+m_hScreen/m_fScale);

	DrawMemDC(pDC);
	pHuamian->DrawDC(&m_memDC,this);
	m_memDC.draw(pDC);

	startUpdateView();
}


// CElecMapView 诊断
CElecMapDoc* CElecMapView::GetDocument()
{	
	if (m_pDocument ==NULL)
	{
		TRACE0("doc initial failed.");
		return 0;
	}
	BOOL ok = m_pDocument->IsKindOf(RUNTIME_CLASS(CElecMapDoc));
	
	if(!ok)
	{
		TRACE0("get doc class failed.");
		return 0;
	}

	return (CElecMapDoc*)m_pDocument;
}

//===============================================================
//class-self method------down
BOOL CElecMapView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE){	
		m_ctrlTipMsg.RelayEvent(pMsg);
	}

	return CView::PreTranslateMessage(pMsg);
}

void CElecMapView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (m_bActive != bActivate)
	{
		if (bActivate)
			m_bActive = bActivate;

		if (!m_selection.IsEmpty())
		{
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		}

		m_bActive = bActivate;
	}
}

void CElecMapView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	m_wScreen = cx;
	m_hScreen = cy;
	m_nXPage = m_wScreen/m_nScrollMin;
	m_nYPage = m_hScreen/m_nScrollMin;

	OnHMSizeChanged();
}

BOOL CElecMapView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CElecMapView::DrawMemDC(CDC *pDC)
{
	CElecMapDoc* pDoc = GetDocument();
	CHuaMian* pHuaMian =CHMProject::getInstance()->getCurActiveHuamianInstance();
	ASSERT(pHuaMian != NULL);
	if (pHuaMian == NULL)
	{
		return;
	}

	CRgn rgn;
	CBrush brush;
	CPen pen;

	CRect rt;
	pDC->GetClipBox(rt);
	if (rt.IsRectEmpty()) return;

	BOOL bl = rgn.CreateRectRgn(rt.left,rt.top,rt.right,rt.bottom);
	ASSERT(bl == TRUE);
	if (bl)
	{
		m_memDC.SelectClipRgn(&rgn);
	}

	if (!(pHuaMian->GetBmpBackGroundMode()))
	{
		CSize sz = m_memDC.GetSize();
		CBitmap bmp;
		bmp.LoadBitmap(IDB_BACK);
		brush.CreatePatternBrush(&bmp);

		CRect rt;
		rt.SetRect(0,0,sz.cx,sz.cy);
		m_memDC.FillRect(&rt,&brush);
	}	
}

void CElecMapView::OnPostMessageRMenu( UINT nID )
{
	if ( nID > (MENU_MESSAGE_COMMAND+MENU_MESSAGE_MAXCOUNT) || nID < MENU_MESSAGE_COMMAND ){
		return;
	}

	//get camera id.
	CString strCameraID;
	
	UINT error(200);
	UINT index = nID - MENU_MESSAGE_COMMAND - error;

	if (index == MENU_MESSAGE_MAXCOUNT){
		strCameraID = m_vecCameraID.at(0);
	}
	else{
		if (index > m_vecCameraID.size()){
			TRACE0("get selected camera index error.\n");
			return;
		}
		strCameraID = m_vecCameraID.at(index);
	}

	//post message to open video.
	g_pMainDlg->postMessageToOpenVideo(strCameraID);
}

void CElecMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (CTyTool::c_drawTy!=tySelect) return;

	if ((point.x<0)||(point.y<0)) return;

	BOOL bFind(FALSE);
	OnUpdataPopMenu( point, bFind );

	if (bFind){
		GetCursorPos(&point);
		m_pMainMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

		m_pMainMenu.DestroyMenu();
		m_pSubMenu.DestroyMenu();
	}
}

void CElecMapView::OnContextMenu( CWnd* pWnd, CPoint point )
{
	
}

void CElecMapView::OnUpdataPopMenu( CPoint Point, BOOL& bFind )
{
	BOOL bIsCamera(FALSE);
	//get entity ename  &  scene name
	CString strEname;

	CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
	if (pHuamian==NULL){
		return;
	}

	CString strSceneName = pHuamian->GetHMName();
	CString strSceneID;
	if (!CFileRead::getSceneIDBySceneName( strSceneName, strSceneID )){
		TRACE0("get scene id failed.\n");
		return;
	}

	PointStruct local = LPtoUP(Point);
	CHMLayer* pLayer =NULL;
	POSITION pos= pHuamian->m_HMLayerList.GetHeadPosition();
	while (pos!=NULL){
		pLayer = pHuamian->m_HMLayerList.GetNext(pos);
		if (pLayer != NULL){
			CTyBase* pObj = pLayer->ObjectAt(local,LLtoUL(OFFSETX));
			if (!pObj){
				continue;
			}
			//yx
			CYX* pYX = dynamic_cast<CYX*>(pObj);
			if (pYX!=NULL){
				strEname.Format("%s",pYX->m_yx.ename);
				bFind = TRUE;
				break;
			}
			//ysh
			CYSH* pYSH = dynamic_cast<CYSH*>(pObj);
			if (pYSH!=NULL){
				strEname.Format("%s",pYSH->m_stuYSH.ename);
				bFind = TRUE;
				bIsCamera = TRUE;
				break;
			}
		}
	}
	
	// get rvu_id sm_id
	if (!bIsCamera){	
		int rvuID, smID;
		if (!CFileRead::getRvuIDAndSmIDBySceneAndEname( strSceneID, strEname, rvuID, smID )){
			TRACE0("get rvu_id, sm_id failed.\n");
			return;
		}

		//get dev_id
		int devID;
		if (!CFileRead::getDevIDByRvuIDAndsmID( rvuID, smID, devID )){
			TRACE0("get dev_id failed.");
			return;
		}

		//get vector camera id
		m_vecCameraID.clear();
		if (!CFileRead::getCameraIDSByDevID( devID, m_vecCameraID )){
			TRACE0("get camera ids failed.\n");
			return;
		}

		//get vector camera name
		m_vecCameraName.clear();
		for (size_t t=0; t<m_vecCameraID.size(); t++){
			CString strCamName;
			if (!CFileRead::getCameraNameByCameraID(m_vecCameraID.at(t), strCamName)) {
				TRACE0("get camera name failed.");
				continue;
			}
			m_vecCameraName.push_back(strCamName);
		}
		//pop menu.
		if (bFind){
			if (m_pMainMenu == NULL){
				m_pMainMenu.CreatePopupMenu();
			}

			if (m_pSubMenu == NULL){
				m_pSubMenu.CreatePopupMenu();
			}
			for (int t=0; t<(int)m_vecCameraName.size(); t++){
				m_pSubMenu.AppendMenu(MF_ENABLED|MF_STRING, MENU_MESSAGE_COMMAND+t, m_vecCameraName.at(t));
			}

			m_pMainMenu.AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT)m_pSubMenu.m_hMenu,_T("关联视频"));
		}
	}
	else{
		CString strCameraID;
		m_vecCameraID.clear();
		if (!CFileRead::getCameraIDBySceneIDAndEname(strSceneID,strEname,strCameraID)){
			TRACE0("get camera id failed.\n");
			return;
		}
		m_vecCameraID.push_back(strCameraID);

		if (m_pMainMenu == NULL){
			m_pMainMenu.CreatePopupMenu();
		}
		if (m_pSubMenu == NULL){
			m_pSubMenu.CreatePopupMenu();
		}

		m_pMainMenu.AppendMenu(MF_STRING|MF_ENABLED, MENU_MESSAGE_COMMAND+MENU_MESSAGE_MAXCOUNT,_T("打开视频"));
	}
	
}

//---------------------------------------------------------------
void CElecMapView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	short nScrollInc;
	int nNewPos;
	CRect r1;

	switch(nSBCode)
	{
	case SB_TOP:
		nScrollInc = -m_nHScrollPos;
		break;
	case SB_BOTTOM:
		nScrollInc = m_nHScrollMax-m_nHScrollPos;
		break;
	case SB_LINEUP:
		nScrollInc = -m_nXLine;
		break;
	case SB_LINEDOWN:
		nScrollInc = m_nXLine;
		break;
	case SB_PAGEUP:
		nScrollInc = -m_nXPage;
		break;
	case SB_PAGEDOWN:
		nScrollInc = m_nXPage;
		break;
	case SB_THUMBPOSITION:
		nScrollInc = nPos-m_nHScrollPos;
		break;
	default:
		nScrollInc = 0;
		break;
	}

	nNewPos  = max(0,min(m_nHScrollPos+nScrollInc,m_nHScrollMax));
	nScrollInc = nNewPos-m_nHScrollPos;
	if(nScrollInc) //如果产生了滚动
	{
		m_nHScrollPos = nNewPos;  //设定新的滚动位置
		SetScrollPos(SB_HORZ,m_nHScrollPos);

		ASSERT(m_fScale != 0);
		m_xStart = m_nHScrollPos*m_nScrollMin/m_fScale;

		GetClientRect(&r1);  //得到客户区的矩形边界
		Invalidate();
	}

	UpdateWindow();//使滚动条的位置改动在屏幕上实现

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CElecMapView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	short nScrollInc;
	int nNewPos;
	CRect r1;

	switch(nSBCode)
	{
	case SB_TOP:
		nScrollInc = -m_nVScrollPos;
		break;
	case SB_BOTTOM:
		nScrollInc = m_nVScrollMax-m_nVScrollPos;
		break;
	case SB_LINEUP:
		nScrollInc = -m_nYLine;
		break;
	case SB_LINEDOWN:
		nScrollInc = m_nYLine;
		break;
	case SB_PAGEUP:
		nScrollInc = -m_nYPage;
		break;
	case SB_PAGEDOWN:
		nScrollInc = m_nYPage;
		break;
	case SB_THUMBPOSITION:
		nScrollInc = nPos-m_nVScrollPos;
		break;
	default:
		nScrollInc = 0;
		break;
	}

	nNewPos  = max(0,min(m_nVScrollPos+nScrollInc,m_nVScrollMax));
	nScrollInc = nNewPos-m_nVScrollPos;
	if(nScrollInc) //如果产生了滚动
	{
		m_nVScrollPos = nNewPos;  //设定新的滚动位置
		SetScrollPos(SB_VERT,m_nVScrollPos);

		ASSERT(m_fScale != 0);
		m_yStart = m_nVScrollPos*m_nScrollMin/m_fScale;

		GetClientRect(&r1);  //得到客户区的矩形边界
		Invalidate();
	}

	UpdateWindow();//使滚动条的位置改动在屏幕上实现

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CElecMapView::InitHScroll()
{
	CSize szUser = GetHMSize();
	ASSERT(m_fScale != 0);

	//横向滚动条的最大范围=(画面宽度(用户坐标)x当前比例-当前屏幕宽度)/最小滚动范围(像素)
	m_nHScrollMax = (int)(((float)szUser.cx*m_fScale-m_wScreen)/(float)m_nScrollMin);

	//当前的横向滚动条位置=起始x坐标(用户坐标)x当前比例/最小滚动范围(像素)
	int pos = int((float)m_xStart*m_fScale/(float)m_nScrollMin);
	m_nHScrollPos = max(0,min(pos,m_nHScrollMax));

	m_xStart = m_nHScrollPos*m_nScrollMin/m_fScale;

	//设置横向滚动条最大范围和当前位置
	SetScrollRange(SB_HORZ,0,m_nHScrollMax,TRUE);
	SetScrollPos(SB_HORZ,m_nHScrollPos);
}

void CElecMapView::InitVScroll()
{
	CSize szUser = GetHMSize();
	ASSERT(m_fScale != 0);

	//竖向滚动条的最大范围=(画面高度(用户坐标)x当前比例-当前屏幕高度)/最小滚动范围(像素)
	m_nVScrollMax=(int)(((float)szUser.cy*m_fScale-m_hScreen)/(float)m_nScrollMin);

	//当前的竖向滚动条位置=起始y坐标(用户坐标)x当前比例/最小滚动范围(像素)
	int pos=(int)((float)m_yStart*m_fScale/(float)m_nScrollMin);
	m_nVScrollPos=max(0,min(pos,m_nVScrollMax));

	m_yStart=m_nVScrollPos*m_nScrollMin/m_fScale;


	//设置竖向滚动条最大范围和当前位置
	SetScrollRange(SB_VERT,0,m_nVScrollMax,TRUE);
	SetScrollPos(SB_VERT,m_nVScrollPos);//rrr.top=0;
}

//---------------------------------------------------------------

void CElecMapView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!m_bActive)
		return;

	CHMLayer* pLayer = GetActiveHMLayer();

	if ((pLayer==NULL)|| (pLayer->GetDocument()!=GetDocument())) 
		return;

	CTyTool* pTool = CTyTool::FindTool(CTyTool::c_drawTy);
	if (pTool != NULL)
	{
		pTool->OnLButtonDblClk(this, nFlags, point);
	}
}

void CElecMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;

	CHMLayer* pLayer = GetActiveHMLayer();

	switch (CTyTool::c_drawTy)
	{
	case	tyZoomIn:
	case	tyZoomOut:
		break;

	default:
		break;//if ((pLayer==NULL)|| (pLayer->GetDocument() != GetDocument())) return
	}

	CTyTool* pTool = CTyTool::FindTool(CTyTool::c_drawTy);

	if (pTool != NULL)
	{
		pTool->OnLButtonDown(this, nFlags, point);
	}
}

void CElecMapView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bActive)
		return;

	BOOL bIsFind(FALSE);
	//get entity ename  &  scene name
	CString strEname;

	CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
	if (pHuamian==NULL){
		return;
	}

	CString strSceneName = pHuamian->GetHMName();
	CString strSceneID;
	if (!CFileRead::getSceneIDBySceneName( strSceneName, strSceneID )){
		TRACE0("get scene id failed.\n");
		return;
	}

	PointStruct local = LPtoUP(point);
	CHMLayer* pLayer =NULL;
	POSITION pos= pHuamian->m_HMLayerList.GetHeadPosition();
	while (pos!=NULL){
		pLayer = pHuamian->m_HMLayerList.GetNext(pos);
		if (pLayer != NULL){
			CTyBase* pObj = pLayer->ObjectAt(local,LLtoUL(OFFSETX));
			if (!pObj){
				continue;
			}
			//ysh
			CYSH* pYSH = dynamic_cast<CYSH*>(pObj);
			if (pYSH!=NULL){
				strEname.Format("%s",pYSH->m_stuYSH.ename);
				bIsFind = TRUE;
				break;
			}
		}
	}

	CString strCameraID;
	if (!CFileRead::getCameraIDBySceneIDAndEname(strSceneID,strEname,strCameraID)){
		TRACE0("get camera id failed.\n");
		return;
	}
	
	CString strCameraName;
	if (!CFileRead::getCameraNameByCameraID(strCameraID,strCameraName)){
		TRACE0("get camera name failed.\n");
		return;
	}

	m_strTipMsg = strCameraName;

	if (!m_bTracking){
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 500;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	return CView::OnMouseMove(nFlags,point);
}

void CElecMapView::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bTracking = FALSE;
	m_ctrlTipMsg.UpdateTipText(m_strTipMsg,this,IDC_TOOLTIP);
	m_ctrlTipMsg.Update();
}

void CElecMapView::OnMouseLeave()
{
	m_bTracking =FALSE;
}

void CElecMapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bActive)
		return;

	CHMLayer* pLayer = GetActiveHMLayer();

	switch (CTyTool::c_drawTy)
	{
	case tyZoomIn:
	case tyZoomOut:
		break;
	default:
		break;
		//{
		//	if ((pLayer==NULL)|| (pLayer->GetDocument()!=GetDocument()))
		//		return;
		//}
	}

	CTyTool* pTool = CTyTool::FindTool(CTyTool::c_drawTy);
	if (pTool != NULL)
	{
		pTool->OnLButtonUp(this, nFlags, point);
	}
}

//class-self method------up
//===============================================================

//===============================================================
//interface for user------down

CSize CElecMapView::GetHMSize()
{
	CSize sz(2048,2048);
	return sz;
}

void CElecMapView::OnHMSizeChanged()
{
	InitHScroll();
	InitVScroll();
	Invalidate();

	UpdateWindow();
}

void CElecMapView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	RectStruct rt = LRecttoURect(rect);
	CHMLayer* pLayer = GetActiveHMLayer();
	ASSERT(pLayer != NULL);
	pLayer->SelectWithinRect(rt,this);
}

void CElecMapView::Deselect(CTyBase *pTy)
{
	ASSERT(pTy != NULL);

	POSITION pos = m_selection.Find(pTy);
	if (pos != NULL)
	{
		m_selection.RemoveAt(pos);
	}
}

void CElecMapView::Add_Selection(CTyBase *pTy)
{
	ASSERT(pTy != NULL);
	if (pTy == NULL) return;

	CTyBase * pObj;
	POSITION pos,prePos = NULL;

	pos = m_selection.GetTailPosition();
	prePos = pos;

	while (pos != NULL)
	{
		pObj = m_selection.GetPrev(pos);
		ASSERT((pObj != NULL));

		if (pObj->m_iNumber < pTy->m_iNumber)
		{
			m_selection.InsertAfter(prePos,pTy);
			break;	
		} 
		else if (pObj->m_iNumber == pTy->m_iNumber)
		{
			ASSERT((pObj->m_iNumber != pTy->m_iNumber));
			break;
		}	 

		prePos = pos;
	}

	if (prePos == NULL)
	{
		m_selection.AddHead(pTy);
	}
}

void CElecMapView::InvalidateTy(CTyBase *pTy, BOOL bErase)
{
	float x0,y0,x1,y1;
	CRect rt;
	CPoint pt;

	ASSERT(pTy != NULL);
	pTy->GetRect(&x0,&y0,&x1,&y1);
	pt = UPtoLP(x0,y0);

	rt.left = pt.x;
	rt.top = pt.y;
	pt = UPtoLP(x1,y1);

	rt.right = pt.x;
	rt.bottom = pt.y;
	rt.InflateRect(OFFSETX,OFFSETY);

	InvalidateRect(&rt,bErase);
}

BOOL CElecMapView::GetActiveLayerName(char *pBuf, int bufsize)
{
	CElecMapDoc * pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CHuaMian* pHuamian = CHMProject::getInstance()->getCurActiveHuamianInstance();
	ASSERT(pHuamian);
	if (!pHuamian){
		return FALSE;
	}

	CString hmName = pHuamian->GetHMName();
	hmName.TrimRight("HM");
	CString hmLayerName = hmName+"HMLayer";

	lstrcpy(pBuf,hmLayerName);

	return TRUE;
}

CHMLayer* CElecMapView::GetActiveHMLayer()
{
	CHuaMian* pHuamian =CHMProject::getInstance()->getCurActiveHuamianInstance();
	if (!pHuamian)
		return 0;

	char buf[256];
	BOOL bl = GetActiveLayerName(buf,256);
	if (!bl) return NULL;

	CHMLayer* pLayer = pHuamian->FindLayer(buf);
	return pLayer;
}

void CElecMapView::GetFloatRect(float *x0, float *y0, float *x1, float *y1)
{
	*x0 = m_xStart;
	*y0 = m_yStart;
	*x1 = m_xEnd;
	*y1 = m_yEnd;
}


void CElecMapView::Select(CTyBase *pTy, BOOL bAdd)
{
	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	} 
	else if (m_selection.GetCount() > 0)
	{
		CTyBase* pObj=m_selection.GetHead();
		ASSERT(pObj != NULL);

		if ((pTy != NULL)&&(pObj->m_pHMLayer != pTy->m_pHMLayer))
		{
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
			m_selection.RemoveAll();
		}
	}

	if (pTy == NULL) return;

	if (IsSelected(pTy))
		return;

	Add_Selection(pTy);

	InvalidateTy(pTy);
}

BOOL CElecMapView::IsSelected(CTyBase *pTy) const
{
	return (m_selection.Find(pTy) != NULL);
}

CPoint  CElecMapView::UPtoLP(float x0, float y0)
{
	ASSERT(m_fScale != 0);

	CPoint pt;
	pt.x = (int) ((x0-m_xStart)*m_fScale);
	pt.y = (int)((y0-m_yStart)*m_fScale);

	return pt;
}


void CElecMapView::LPtoUP(CPoint pt, float *x, float *y)
{
	ASSERT(m_fScale != 0);

	*x = (float)pt.x/m_fScale+m_xStart;
	*y = (float)pt.y/m_fScale+m_yStart;
}

float CElecMapView::LLtoUL(int len)
{
	ASSERT(m_fScale != 0);
	float f = (float)len/m_fScale;
	return f;
}

int CElecMapView::ULtoLL(float len)
{
	ASSERT(m_fScale != 0);
	int l = (int)(len*m_fScale);
	return l;
}

PointStruct CElecMapView::LPtoUP(CPoint pt)
{
	PointStruct fpt;
	LPtoUP(pt,&fpt.x,&fpt.y);
	return fpt;
}

RectStruct CElecMapView::LRecttoURect(CRect rt)
{
	RectStruct frt;

	LPtoUP(CPoint(rt.left,rt.top),&frt.x0,&frt.y0);
	LPtoUP(CPoint(rt.right,rt.bottom),&frt.x1,&frt.y1);

	return frt;
}

CRect CElecMapView::URecttoLRect(RectStruct frt)
{
	CPoint pt1,pt2;
	pt1 = UPtoLP(frt.x0,frt.y0);
	pt2 = UPtoLP(frt.x1,frt.y1);

	CRect rt(pt1.x,pt1.y,pt2.x,pt2.y);
	rt.NormalizeRect();

	return rt;
}

CPoint CElecMapView::UPtoLP(PointStruct fpt)
{
	CPoint pt;
	pt = UPtoLP(fpt.x,fpt.y);
	return pt;
}

//interface for user------up
//===============================================================
