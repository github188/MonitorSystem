// HMLayer.cpp: implementation of the CHMLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\SDDElecMap.h"
#include "..\..\ElecMapDoc.h"
#include ".\..\ElecMapView.h"

//#include "canvas/TyBase.h"
#include "canvas/QGM.h"
//#include "HMProject.h"
#include "canvas/HuaMian.h"
#include "canvas/HMLayer.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CHMLayer, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHMLayer::CHMLayer(float startBL,float endBL,float defBL, int layerID,LPCSTR pStr,CHuaMian *pHuaMian,int sPos,int ePos,int sTyID)
{
	m_IDDef = sTyID;
	m_iStartNumber = sPos;
	m_iEndNumber = ePos;

	ASSERT(pHuaMian != NULL);
	ASSERT(startBL > 0);
	ASSERT(endBL > 0);

	m_pHuaMian = pHuaMian;
	m_startViewBL = __min(startBL,endBL);
	m_endViewBL = __max(startBL,endBL);
	m_DefaultBL = defBL;
	m_iLayerNo = layerID;
	m_CName = pStr;
	m_uRow = 0;
	m_uCol = 0;

	ZeroMemory(QGMArr,sizeof(QGMArr));

	ASSERT((defBL >= m_startViewBL)&&(defBL <= m_endViewBL));

	OnHMLayerDataChanged();
}

CHMLayer::~CHMLayer()
{
	POSITION pos = m_TyList.GetHeadPosition();
	while (pos != NULL)
	{
		delete m_TyList.GetNext(pos);
	}
}

CRect CHMLayer::GetArrQGMBoundsRect(float x0, float y0, float x1, float y1)
{
	ASSERT(m_pHuaMian != NULL);

	CRect rt;

	rt.left       =  (int)((float)x0*(m_DefaultBL/(float)m_pHuaMian->m_wResolution));
	rt.right     =  (int)(x1*m_DefaultBL/(float)m_pHuaMian->m_wResolution);
	rt.top       =  (int)((float)y0*m_DefaultBL/(float)m_pHuaMian->m_hResolution);
	rt.bottom = (int)(y1*m_DefaultBL/(float)m_pHuaMian->m_hResolution);

	return rt;
}

void CHMLayer::Add(CTyBase *pTy,BOOL blNumberChanged)
{
	ASSERT(pTy != NULL);
	
	if (pTy->m_Id == 0) pTy->m_Id = ++m_IDDef;
	
	if (blNumberChanged)
	{
		++m_iEndNumber;
		pTy->m_iNumber = m_iEndNumber;
	}

	m_TyList.AddTail(pTy);
	pTy->m_pHMLayer = this;
	
	OnAddTy(pTy);
	
	ASSERT(m_pHuaMian!=NULL);

	m_pHuaMian->OnAddTyEvent(pTy);
}

void CHMLayer::Remove(CTyBase *pTy)
{
	ASSERT(pTy != NULL);

	OnRemoveTy(pTy);

	ASSERT(m_pHuaMian != NULL);
	m_pHuaMian->OnRemoveTyEvent(pTy);

	POSITION pos = m_TyList.Find(pTy);
	if (pos != NULL)
	{
		m_TyList.RemoveAt(pos);
	}
}

void CHMLayer::Front(CTyBase *pTy)
{
	ASSERT(pTy != NULL);

	float x0,y0,x1,y1;
	pTy->GetRect(&x0,&y0,&x1,&y1);
	CRect rt = GetArrQGMBoundsRect(x0,y0,x1,y1);
	
	rt.left = __max(0,rt.left);
	rt.top = __max(0,rt.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt.right = __min((int)m_uCol-1,rt.right);
	rt.bottom = __min((int)m_uRow-1,rt.bottom);

	int i,j;
	for (i = rt.top;i <= rt.bottom;i++)
	{
		for (j = rt.left;j <= rt.right;j++)
		{
			if (QGMArr[i][j] != NULL)
				QGMArr[i][j]->Front(pTy);
		}
	}

	POSITION pos = m_TyList.Find(pTy);
	ASSERT(pos != NULL);
	if (pos == NULL) 
		return;

	m_TyList.RemoveAt(pos);
	m_TyList.AddTail(pTy);
	m_iEndNumber++;
	pTy->m_iNumber = m_iEndNumber;
}

void CHMLayer::Back(CTyBase *pTy)
{
	ASSERT(pTy!=NULL);

	float x0,y0,x1,y1;
	pTy->GetRect(&x0,&y0,&x1,&y1);
	CRect rt = GetArrQGMBoundsRect(x0,y0,x1,y1);

	rt.left = __max(0,rt.left);
	rt.top = __max(0,rt.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt.right = __min((int)m_uCol-1,rt.right);
	rt.bottom = __min((int)m_uRow-1,rt.bottom);

	int i,j;
	for (i = rt.top;i <= rt.bottom;i++)
	{
		for (j = rt.left;j <= rt.right;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			if (QGMArr[i][j] != NULL)
				QGMArr[i][j]->Back(pTy);
		}
	}

	POSITION pos = m_TyList.Find(pTy);
	ASSERT(pos != NULL);
	if (pos == NULL) return;
	m_TyList.RemoveAt(pos);
	m_TyList.AddHead(pTy);
	m_iStartNumber--;
	pTy->m_iNumber = m_iStartNumber;
}

#ifdef _DEBUG
void CHMLayer::ASSERTValid()
{
	ASSERT(!m_CName.IsEmpty());
	ASSERT(m_startViewBL != 0);
	ASSERT(m_endViewBL >= m_startViewBL);
	ASSERT((m_DefaultBL >= m_startViewBL)&&(m_DefaultBL <= m_endViewBL));
	ASSERT(m_iEndNumber >= m_iStartNumber);
}
#endif

void CHMLayer::Save(CFile *file, BOOL Yn)
{
	int num;
	POSITION pos;
	CTyBase * pTy;
	int bufsize=0;
	DRAW_TY tystyle;

	ASSERT(file!=NULL);

	if (Yn)
	{
		AssertValid();
		num=m_TyList.GetCount();
		file->Write((char *)&num,sizeof(num));
		pos=m_TyList.GetHeadPosition();
		while (pos!=NULL)
		{
			pTy=m_TyList.GetNext(pos);
			ASSERT(pTy!=NULL);
			if (!pTy->m_bDelete)
			{
				tystyle=pTy->GetTyType();
				ASSERT(tystyle!=tyNone);
				file->Write((char *)&tystyle,sizeof(tystyle));
				pTy->Save(file,Yn);
			}
		}
	}
	else
	{
		file->Read((char *)&num,sizeof(num));
		for (int i=0;i<num;i++)
		{
			file->Read((char *)&tystyle,sizeof(tystyle));
			pTy=CTyBase::CreateTy(tystyle);
			if (pTy==NULL) return;
			pTy->Save(file,Yn);
			Add(pTy,TRUE);//FALSE);
		}
	}
}

BOOL CHMLayer::IsInViewBL(float fScale)
{
	AssertValid();
	
	if ((fScale<m_startViewBL)|(fScale>m_endViewBL)) return FALSE;
	else return TRUE;
}

void CHMLayer::DrawDC(CDC *pDC, CElecMapView *pView)
{
	float x0,y0,x1,y1;

	if (m_bHide)
		return;
	
	ASSERT(pView != NULL);
	pView->GetFloatRect(&x0,&y0,&x1,&y1);

	CRect rt = GetArrQGMBoundsRect(x0,y0,x1,y1);

	rt.left = __max(0,rt.left);
	rt.top = __max(0,rt.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt.right = __min((int)m_uCol-1,rt.right);
	rt.bottom = __min((int)m_uRow-1,rt.bottom);

	//切割面图元归并显示
	int		preNO  = m_iEndNumber+1;		//前一个显示图元
	int		nextNO = preNO;					//当前显示图元

	CTyBase * arrty[MAX_QGMROW][MAX_QGMCOL];
	ZeroMemory(arrty,sizeof(CTyBase *)*MAX_QGMCOL*MAX_QGMROW);

	POSITION arrpos[MAX_QGMROW][MAX_QGMCOL]; 
	ZeroMemory(arrpos,sizeof(POSITION)*MAX_QGMCOL*MAX_QGMROW);
	 
	int i,j;
	//首先找到数组内第一批在可视区内需要显示的图元
	for (i = rt.top;i <= rt.bottom;i++)
	{
		for (j = rt.left;j <= rt.right;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			if (QGMArr[i][j] == NULL) 
				continue;

			arrpos[i][j] = QGMArr[i][j]->m_TyBaseList.GetHeadPosition();
			while (arrpos[i][j] != NULL)
			{
				arrty[i][j] = QGMArr[i][j]->m_TyBaseList.GetNext(arrpos[i][j]);
				ASSERT(arrty[i][j] != NULL);

				//图元是否已删除，是否在可视区
				if ((!arrty[i][j]->m_bDelete)&&(arrty[i][j]->IntersectRect(x0,y0,x1,y1))) 
					break;

				arrty[i][j] = NULL;
			}
		}
	}

	//切割面图元归并显示
	BOOL loop = TRUE;
	int col,row;
	while (loop)
	{
		//寻找最後端的图元
		for (i = rt.top;i <= rt.bottom;i++)
		{
			for (j = rt.left;j <= rt.right;j++)
			{
				if (arrty[i][j] == NULL) 
					continue;

				if (arrty[i][j]->m_iNumber == preNO)
				{
					arrty[i][j] = NULL;
					if (arrpos[i][j] != NULL)
					{
						while (arrpos[i][j] != NULL)
						{
							arrty[i][j] = QGMArr[i][j]->m_TyBaseList.GetNext(arrpos[i][j]);
							ASSERT(arrty[i][j] != NULL);

							//图元是否已删除，是否在可视区
							if ((!arrty[i][j]->m_bDelete)&&(arrty[i][j]->IntersectRect(x0,y0,x1,y1)))
								break;

							arrty[i][j] = NULL;
						}
					}
				}

				if ((arrty[i][j] != NULL)&&(arrty[i][j]->m_iNumber < nextNO)) 
				{ 
					nextNO = arrty[i][j]->m_iNumber;
					row = i;
					col  = j;
				}
			}
		}
			
		//判断是否显示结束
		if (nextNO == m_iEndNumber+1)
			break;

		//最後端的图元显示
		arrty[row][col]->DrawDC(pDC,pView);

		if (pView->isViewActive() && !pDC->IsPrinting() && pView->IsSelected(arrty[row][col]))
		{
			arrty[row][col]->DrawTracker(pDC, pView,CTyBase::selected);
		}

		preNO = nextNO;
		nextNO = m_iEndNumber+1;
	}	
}

void CHMLayer::OnHMLayerDataChanged()
{
	UINT i,j;
	UINT nRow,nCol;

	AssertValid();
  
	nRow = (UINT)(m_pHuaMian->m_wPicHeight*m_DefaultBL+m_pHuaMian->m_hResolution-1)/m_pHuaMian->m_hResolution;		
	nCol = (UINT)(m_pHuaMian->m_wPicWidth*m_DefaultBL+m_pHuaMian->m_wResolution-1)/m_pHuaMian->m_wResolution;
	ASSERT(nRow > 0);
	ASSERT(nCol > 0);
	
	if (nRow < 1) nRow = 1;
	if (nCol < 1) nCol = 1;
	
	ASSERT(nRow <= MAX_QGMROW);
	ASSERT(nCol <= MAX_QGMCOL);
	
	if (nRow > MAX_QGMROW) nRow = MAX_QGMROW;
	if (nCol > MAX_QGMCOL) nCol = MAX_QGMCOL;

	for (i = 0;i < m_uRow;i++)
	{
		for(j = 0;j < m_uCol;j++)
		{
			if ((QGMArr[i][j] != NULL)&&((i >= nRow)||(j >= nCol)))
			{
				delete QGMArr[i][j];
				QGMArr[i][j] = NULL;
			}
		}
	}
		
	m_uRow = nRow;
	m_uCol = nCol;

	float stepX = (float)m_pHuaMian->m_wPicWidth/(float)m_uCol;
	float stepY = (float)m_pHuaMian->m_wPicHeight/(float)m_uRow;

	for(i = 0;i < (int)m_uRow;i++)
	{
		for(j = 0;j < (int)m_uCol;j++)
		{
			if (QGMArr[i][j] == NULL)
			{
				QGMArr[i][j] = new CQGM(j*stepX,i*stepY,(j+1)*stepX,(i+1)*stepY);
			}
			else
			{
				QGMArr[i][j]->Clear();
				QGMArr[i][j]->SetBounds(j*stepX,i*stepY,(j+1)*stepX,(i+1)*stepY);
			}
		}
	}

	CTyBase* pTy;
	POSITION pos = m_TyList.GetHeadPosition();
	while (pos != NULL)
	{
		pTy = m_TyList.GetNext(pos);
		ASSERT(pTy!=NULL);
		OnAddTy(pTy);
	}
}

void CHMLayer::OnAddTy(CTyBase *pTy)
{
	ASSERT(pTy != NULL);

	float x0,y0,x1,y1;
	pTy->GetRect(&x0,&y0,&x1,&y1);
	CRect rt = GetArrQGMBoundsRect(x0,y0,x1,y1);

	rt.left = __max(0,rt.left);
	rt.top = __max(0,rt.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt.right = __min((int)m_uCol-1,rt.right);
	rt.bottom = __min((int)m_uRow-1,rt.bottom);

	int i,j;
	for (i = rt.top;i <= rt.bottom;i++)
	{
		for (j = rt.left;j <= rt.right;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			if (QGMArr[i][j] == NULL) 
				continue;

			QGMArr[i][j]->m_TyBaseList.AddTail(pTy);
		}
	}
}

void CHMLayer::ModifyProp(tagHMLayerID *pLayerID)
{
	ASSERT(pLayerID != NULL);

	m_CName = pLayerID->strLayerName;
	m_bHide = pLayerID->bHide;
	if ((pLayerID->fStartScale != m_startViewBL)|(pLayerID->fEndScale != m_endViewBL)|(pLayerID->fScale != m_DefaultBL))
	{
		m_startViewBL = pLayerID->fStartScale;
		m_endViewBL = pLayerID->fEndScale;
		m_DefaultBL = pLayerID->fScale;
		OnHMLayerDataChanged();
	}

	AssertValid();
}

CTyBase * CHMLayer::FindTyBase(int id)
{
	CTyBase* pTy;
	POSITION pos = m_TyList.GetHeadPosition();
	while (pos != NULL)
	{
		pTy = m_TyList.GetNext(pos);
		ASSERT(pTy != NULL);

		if (pTy->m_Id == id) 
			return pTy;
	}

	return NULL;
}

CElecMapDoc * CHMLayer::GetDocument()
{
	ASSERT(m_pHuaMian!=NULL);
	return m_pHuaMian->GetDocument();//>m_pDoc;
}

void CHMLayer::OnRemoveTy(CTyBase *pTy)
{
	ASSERT(pTy != NULL);

	float x0,y0,x1,y1;
	pTy->GetRect(&x0,&y0,&x1,&y1);
	CRect rt = GetArrQGMBoundsRect(x0,y0,x1,y1);

	rt.left = __max(0,rt.left);
	rt.top = __max(0,rt.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt.right = __min((int)m_uCol-1,rt.right);
	rt.bottom = __min((int)m_uRow-1,rt.bottom);

	int i,j;
	for (i = rt.top;i <= rt.bottom;i++)
	{
		for (j = rt.left;j <= rt.right;j++)
		{
			if (QGMArr[i][j] != NULL)
			{
					QGMArr[i][j]->RemoveTy(pTy);
			}
		}
	}
}

CTyBase * CHMLayer::ObjectAt(PointStruct pt,float errRange)
{
	CTyBase *pTy = NULL;
	
	if (m_bHide) 
		return pTy;

	CRect rt = GetArrQGMBoundsRect(pt.x,pt.y,pt.x,pt.y);
	if ((rt.left < 0)||(rt.left >= (int)m_uCol)||(rt.top < 0)||(rt.bottom >= (int)m_uRow))
		 return pTy;

	ASSERT(QGMArr[rt.top][rt.left] != NULL);
	if (QGMArr[rt.top][rt.left] != NULL)
	{
		pTy = QGMArr[rt.top][rt.left]->ObjectAt(pt,errRange);
	}

	return pTy;
}

float CHMLayer::GetDefaultScale()
{
	return m_DefaultBL;
}

void CHMLayer::SelectWithinRect(RectStruct rt, CElecMapView *pView)
{
	ASSERT(pView!=NULL);

	CRect rect=GetArrQGMBoundsRect(rt.x0,rt.y0,rt.x1,rt.y1);

	rect.left=__max(0,rect.left);
	rect.top=__max(0,rect.top);
	ASSERT((m_uCol>0)&&(m_uRow>0));
	rect.right=__min((int)m_uCol-1,rect.right);
	rect.bottom=__min((int)m_uRow-1,rect.bottom);

	//切割面图元归并选择
  
	int preNO=m_iEndNumber+1; //前一个显示图元
	int	nextNO=preNO;					//当前显示图元
	CTyBase * arrty[MAX_QGMROW][MAX_QGMCOL];
	ZeroMemory(arrty,sizeof(CTyBase *)*MAX_QGMCOL*MAX_QGMROW);
	POSITION arrpos[MAX_QGMROW][MAX_QGMCOL]; 
	ZeroMemory(arrpos,sizeof(POSITION)*MAX_QGMCOL*MAX_QGMROW);
	 
	RectStruct rtTy;
	int i,j;
	//首先找到数组内第一批在可视区内需要显示的图元
	for (i=rect.top;i<=rect.bottom;i++)
	{
		for (j=rect.left;j<=rect.right;j++)
		{
			ASSERT(QGMArr[i][j]!=NULL);
			if (QGMArr[i][j]==NULL) continue;
			arrpos[i][j]=QGMArr[i][j]->m_TyBaseList.GetHeadPosition();
			while (arrpos[i][j]!=NULL)
			{
				arrty[i][j]=QGMArr[i][j]->m_TyBaseList.GetNext(arrpos[i][j]);
				ASSERT(arrty[i][j]!=NULL);
				//图元是否已删除，是否在可视区
				if (arrty[i][j]->m_bDelete) 
				{
					arrty[i][j]=NULL;
					continue;
				}
				arrty[i][j]->GetRect(&rtTy.x0,&rtTy.y0,&rtTy.x1,&rtTy.y1);
				if (arrty[i][j]->URectInURect(rtTy,rt))
					break; 
				arrty[i][j]=NULL;
			}
		}
	}

	//切割面图元归并显示
	BOOL loop=TRUE;
	int col,row;
	while (loop)
	{
	  //寻找最後端的图元
	  for (i=rect.top;i<=rect.bottom;i++)
	  	for (j=rect.left;j<=rect.right;j++)
			{
				if (arrty[i][j]==NULL) continue;
			  if (arrty[i][j]->m_iNumber==preNO)
			  {
					arrty[i][j]=NULL;
					if (arrpos[i][j]!=NULL)
						while (arrpos[i][j]!=NULL)
						{
								arrty[i][j]=QGMArr[i][j]->m_TyBaseList.GetNext(arrpos[i][j]);
								ASSERT(arrty[i][j]!=NULL);
								//图元是否已删除，是否在选择区域内
								if (arrty[i][j]->m_bDelete)
								{
									arrty[i][j]=NULL;
									continue;
								}

								arrty[i][j]->GetRect(&rtTy.x0,&rtTy.y0,&rtTy.x1,&rtTy.y1);
								if (arrty[i][j]->URectInURect(rtTy,rt)) break; 
								arrty[i][j]=NULL;
						}
				}

				if ((arrty[i][j]!=NULL)&&(arrty[i][j]->m_iNumber<nextNO)) 
				{ 
					nextNO=arrty[i][j]->m_iNumber;
					row=i;
					col=j;
				}
			}
			
		//判断是否选择结束
		if (nextNO==m_iEndNumber+1) break;

		//最後端的图元选择
		pView->Select(arrty[row][col]);


		preNO=nextNO;
		nextNO=m_iEndNumber+1;
	}
}

void CHMLayer::ZhenLi()
{
	UINT i,j;
	for (i = 0;i < m_uRow;i++)
	{
		for (j = 0;j < m_uCol;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			QGMArr[i][j]->ZhenLi();
		}
	}
	
	CTyBase * pTy;
	POSITION pos = m_TyList.GetTailPosition();
	POSITION prePos;
	while (pos != NULL)
	{
		prePos = pos;
		pTy = m_TyList.GetPrev(pos);
		ASSERT(pTy != NULL);
		if (pTy->m_bDelete)
		{
			m_TyList.RemoveAt(prePos);
			delete pTy;
		}
	}
}

void CHMLayer::OnPositionChangedTy(CTyBase *pTy, RectStruct oldRt, RectStruct newRt)
{
	int i,j;

	CRect rt1 = GetArrQGMBoundsRect(oldRt.x0,oldRt.y0,oldRt.x1,oldRt.y1);

	rt1.left = __max(0,rt1.left);
	rt1.top = __max(0,rt1.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt1.right = __min((int)m_uCol-1,rt1.right);
	rt1.bottom = __min((int)m_uRow-1,rt1.bottom);

	for (i = rt1.top;i <= rt1.bottom;i++)
	{
		for (j = rt1.left;j <= rt1.right;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			if (QGMArr[i][j] != NULL)
			{
				QGMArr[i][j]->RemoveTy(pTy);
			}
		}
	}

	CRect rt2 = GetArrQGMBoundsRect(newRt.x0,newRt.y0,newRt.x1,newRt.y1);

	rt2.left  = __max(0,rt2.left);
	rt2.top = __max(0,rt2.top);
	ASSERT((m_uCol > 0)&&(m_uRow > 0));
	rt2.right = __min((int)m_uCol-1,rt2.right);
	rt2.bottom = __min((int)m_uRow-1,rt2.bottom);

	for (i = rt2.top;i <= rt2.bottom;i++)
	{
		for (j = rt2.left;j <= rt2.right;j++)
		{
			ASSERT(QGMArr[i][j] != NULL);
			if (QGMArr[i][j] != NULL)
			{
				QGMArr[i][j]->AddTy(pTy);
			}
		}
	}
}

void CHMLayer::ParamReplace(CStringList& ruleList)
{
	POSITION pos;
	CTyBase * pTy;
	pos = m_TyList.GetHeadPosition();
	while (pos != NULL)
	{
		pTy = m_TyList.GetNext(pos);
		ASSERT(pTy != NULL);
		pTy->ParamReplace(ruleList);
	}
}
