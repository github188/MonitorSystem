#include "StdAfx.h"
#include "canvas/TyState.h"
#include "../../ElecMapView.h"


//IMPLEMENT_SERIAL(CTyState, CTyBase, 0)
//
//CTyState::CTyState(float x0,float y0,float x1,float y1,\
//			   LPCSTR bmpname,BOOL blTranslate,\
//			   COLORREF translatecolor, COLORREF textColor,\
//			   BOOL blHScale,BOOL blVScale,\
//			   int vId/*=0*/,CHMLayer* pLayer/*=NULL*/)
//			   :CTyBase(vId,pLayer)
//{
//	m_x0 = x0;
//	m_y0 = y0;
//	m_x1 = x1;
//	m_y1 = y1;
//	m_bTranslate = blTranslate;
//	m_TranslateColor = translatecolor;
//	m_TextColor =textColor;
//	m_blHScale = blHScale;
//	m_blVScale = blVScale;					
//	if (bmpname != NULL)
//	{
//		strcpy(m_bmpName,bmpname);
//		RegisterImage(m_bmpName);
//	}
//	else 
//		ZeroMemory(m_bmpName,sizeof(char)*33);
//}
//
//CTyState::~CTyState(void)
//{
//}
//
//void CTyState::setScaleConst()
//{
//	m_blHScale = FALSE;
//	m_blVScale = FALSE;
//}
//
//void CTyState::drawText(float startX, float startY, float width, float height,\
//						CString  strText,\
//						CDC *pDC, CElecMapView *pView )
//{
//	CBrush brush,*pOldBrush;
//	LOGBRUSH logbrush;
//
//	CFont font; 
//
//	//创建字模  
//	if (font.CreateFont(32,15,0,0,50,0,0,0,GB2312_CHARSET,OUT_TT_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,_T("宋体")) == FALSE)
//		return;
//
//	logbrush.lbStyle = BS_NULL;
//	brush.CreateBrushIndirect(&logbrush);
//	pOldBrush = pDC->SelectObject(&brush);
//	COLORREF oldTextColor = pDC->SetTextColor(m_TextColor);
//	int oldmode = pDC->SetBkMode(TRANSPARENT);
//
//	CFont* pOldFont = pDC->SelectObject(&font);
//
//	CPoint startPoint(startX,startY);
//	const char * p;
//	int len;
//	unsigned char c1,c2;
//	char bz[4];
//	float x1 = startX,y1 = startY;
//
//	p = (const char *)strText;
//	len = strlen(p);
//
//	BOOL bFangXiang(TRUE);
//	float fTextHeight(32);				//字体高度
//	float fTextWidth(15);				//字体宽度
//	float fJqWidth(2);					//间隔宽度
//	while (len > 0)
//	{
//		c1 = *p;
//		c2 = *(p+1);
//		if((c1 > 127)&&(c2 > 127))  //如果是一个汉字
//		{
//			strncpy(bz,p,2);//拷贝一个汉字到bz中
//			bz[2] = 0x00;
//			p = p+2;        //跳过汉字指向下一个字符位置
//			pDC->TextOut(startPoint.x,startPoint.y,bz); //在屏幕上写这个汉字
//			len = len-2;                  //字符数减2
//
//			//下一个字符的显示位置
//			if (bFangXiang) 
//				x1 = x1+fTextWidth*2+fJqWidth;
//			else 
//				y1=y1+fTextHeight+fJqWidth;
//
//			startPoint = pView->UPtoLP(x1,y1);
//		} 
//		else
//		{
//			strncpy(bz,p,1);       //拷贝一个字符到bz中
//			bz[1]=0x00;
//			p++;                   //跳过这一个字符到下一个字符
//			if (bFangXiang)
//				pDC->TextOut(startPoint.x,startPoint.y,bz);//在屏幕上写字符
//			else
//				pDC->TextOut(startPoint.x+(int)(width/2),startPoint.y,bz);//在屏幕上写字符
//
//			len--;                 //字符数减1
//
//			if (bFangXiang) 
//				x1=x1+fTextWidth+fJqWidth;
//			else 
//				y1=y1+fTextHeight+fJqWidth;
//
//			startPoint=pView->UPtoLP(x1,y1);
//		}
//	}
//
//	pDC->SetBkMode(oldmode);
//	pDC->SetTextColor(oldTextColor);
//	pDC->SelectObject(pOldBrush);
//}
//
//void CTyState::DrawDC( CDC *pDC, CElecMapView *pView )
//{
//	ASSERT(pDC != NULL);
//	ASSERT(pView != NULL);
//
//	if(m_bDelete)	//如果已经处于删除状态
//		return;
//
//	float minx,miny,maxx,maxy;
//	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
//	if (!IntersectRect(minx,miny,maxx,maxy))
//		return;
//
//	GetRect(&minx,&miny,&maxx,&maxy);
//
//	CPoint pt1,pt2;
//	pt1 = pView->UPtoLP(minx,miny);
//	pt2 = pView->UPtoLP(maxx,maxy);
//
//	CRect rt;
//	rt.SetRect(pt1,pt2);
//	rt.NormalizeRect();
//
//	if ((rt.Width() < 1)||(rt.Height() < 1))
//		return;
//
//	DrawImage(m_bmpName,pDC,pt1.x,pt1.y,rt.Width()-48,rt.Height(),m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
//	
//	float startX = pt1.x +(rt.Width()-48);
//	float startY = pt1.y;
//	float width =48;
//	float height =32;
//	drawText( startX,startY, width, height, m_bmpName, pDC,pView);
//}
//
//
//void CTyState::GetRect(float *minX, float *minY, float *maxX, float *maxY)
//{
//	*minX  = __min(m_x0,m_x1);
//	*minY  = __min(m_y0,m_y1);
//	*maxX = __max(m_x0,m_x1);
//	*maxY = __max(m_y0,m_y1);
//}
//
//CTyBase * CTyState::Clone()
//{
//	ASSERT_VALID(this);
//
//	CTyState* pClone = new CTyState(m_x0,m_y0,m_x1,m_y1,m_bmpName,m_bTranslate,m_TranslateColor,m_blHScale,m_blVScale);
//	ASSERT_VALID(pClone);
//
//	return (CTyBase *)pClone;
//}
//
//BOOL CTyState::IntersectRect(float x0, float y0, float x1, float y1)
//{
//	return CTyBase::IntersectRect(x0,y0,x1,y1);
//}