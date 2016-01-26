#include "stdafx.h"

#include "SDDElecMap.h"
#include "ElecMapDoc.h"
#include "ElecMapView.h"

#include "canvas/YSH.h"
#include "canvas/RtValueTool.h"

//#include "dialog/YX_PROP_INFO.h"
//#include "dialog/DlgYSHShow.h"
//#include "dialog/DlgCameraEntity.h"

IMPLEMENT_SERIAL(CYSH, CTyBase, 0)

CYSH::CYSH(float x0,float y0,float w,float h)
{
	sprintf(m_stuYSH.ename,"DEFAULTENAME");
	m_stuYSH.rect.x0=x0;
	m_stuYSH.rect.y0=y0;
	m_stuYSH.rect.x1=x0+32;
	m_stuYSH.rect.y1=y0+32;
}

CYSH::~CYSH()
{

}

void CYSH::DrawDC( CDC *pDC, CElecMapView *pView )
{
	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;

	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2,pt3,pt4;
	pt1=pView->UPtoLP(m_stuYSH.rect.x0,m_stuYSH.rect.y0);
	pt2=pView->UPtoLP(m_stuYSH.rect.x0,m_stuYSH.rect.y1);
	pt3=pView->UPtoLP(m_stuYSH.rect.x1,m_stuYSH.rect.y1);
	pt4=pView->UPtoLP(m_stuYSH.rect.x1,m_stuYSH.rect.y0);

	if ((pt3.x-pt1.x<=2)&&(pt3.y-pt1.y<=2)) return;

	DrawBmp(pDC,pView,pt1,pt3);
}

void CYSH::DrawBmp( CDC *pDC, CElecMapView *pView, CPoint pt1, CPoint pt3 )
{
	if ((pt3.x-pt1.x<1)||(pt3.y-pt1.y<1)) return;
	BOOL trans=false;

	DrawImage(m_stuYSH.strBmpName_normal.c_str(),pDC,pt1.x+1,pt1.y+1,pt3.x-pt1.x-2,pt3.y-pt1.y-2,trans,RGB(0,0,0));
}

void CYSH::Ty_Serialize( CArchive &ar )
{

}

void CYSH::Ty_Save( CFile *file, BOOL Yn )
{
	if(Yn)	//如果是在进行保存
	{
 		file->Write((unsigned char *)&m_stuYSH.rect,sizeof(m_stuYSH.rect));
		file->Write((unsigned char *)m_stuYSH.ename,sizeof(char)*17);
		file->Write((LPTSTR)(LPCTSTR)m_stuYSH.strBmpName_normal.c_str(),sizeof(m_stuYSH.strBmpName_normal));
		file->Write((LPTSTR)(LPCTSTR)m_stuYSH.strBmpName_warining.c_str(),sizeof(m_stuYSH.strBmpName_warining));
		file->Write((LPTSTR)(LPCTSTR)m_stuYSH.strBmpName_none.c_str(),sizeof(m_stuYSH.strBmpName_none));
	} 
	else
	{
		char readstr[64];

		file->Read((unsigned char *)&m_stuYSH.rect,sizeof(m_stuYSH.rect));
		file->Read((unsigned char *)m_stuYSH.ename,sizeof(char)*17);

		int norsize;
		file->Read((int*)(&norsize), sizeof(int));
		file->Read(readstr,norsize);
		m_stuYSH.strBmpName_normal.assign(readstr,norsize);

		int warnSize;
		file->Read((int*)(&warnSize),sizeof(int));
		file->Read(readstr,warnSize);
		m_stuYSH.strBmpName_warining.assign(readstr,warnSize);

		int closeSize;
		file->Read((int*)(&closeSize),sizeof(int));
		file->Read(readstr,closeSize);
		m_stuYSH.strBmpName_none.assign(readstr,closeSize);

		RegisterImage(m_stuYSH.strBmpName_normal.c_str());
		RegisterImage(m_stuYSH.strBmpName_warining.c_str());
		RegisterImage(m_stuYSH.strBmpName_none.c_str());
	}
}

void CYSH::OnEditProperties( CElecMapView * pView /*= NULL*/ )
{

}

void CYSH::GetRect( float *minX,float *minY,float *maxX,float *maxY )
{
	*minX=m_stuYSH.rect.x0;
	*minY=m_stuYSH.rect.y0;
	*maxX=m_stuYSH.rect.x1;
	*maxY=m_stuYSH.rect.y1;
}

DRAW_TY CYSH::GetTyType()
{
	return tyCamera;
}

void CYSH::MoveHandleTo( int nHandle, PointStruct point, CElecMapView* pView,UINT nFlags /*=0*/ )
{
	ASSERT(pView != NULL);

	pView->InvalidateTy(this);


	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
		break;
	case 1:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_stuYSH.rect.x1)>fabs(point.y-m_stuYSH.rect.y1)) 
			{
				m_stuYSH.rect.x0 = point.y;
				if (point.x < m_stuYSH.rect.x1)
					m_stuYSH.rect.x0 = m_stuYSH.rect.x1-(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
				else
					m_stuYSH.rect.x0 = m_stuYSH.rect.x1+(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
			} 
			else
			{
				m_stuYSH.rect.x0 = point.x;
				if (point.y < m_stuYSH.rect.y1)
					m_stuYSH.rect.y0 = m_stuYSH.rect.y1-(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
				else
					m_stuYSH.rect.y0 = m_stuYSH.rect.y1+(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
			}
		}
		else 
		{
			m_stuYSH.rect.x0 = point.x;
			m_stuYSH.rect.y0 = point.y;
		}
		break;

	case 2:
		m_stuYSH.rect.y0 = point.y;
		break;

	case 3:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_stuYSH.rect.x0) > fabs(point.y-m_stuYSH.rect.y1)) 
			{
				m_stuYSH.rect.y0 = point.y;
				if (point.x < m_stuYSH.rect.x0)
					m_stuYSH.rect.x1 = m_stuYSH.rect.x0-(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
				else
					m_stuYSH.rect.x1 = m_stuYSH.rect.x0+(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
			} 
			else
			{
				m_stuYSH.rect.x1 = point.x;
				if (point.y < m_stuYSH.rect.y1)
					m_stuYSH.rect.y0 = m_stuYSH.rect.y1-(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
				else
					m_stuYSH.rect.y0 = m_stuYSH.rect.y1+(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
			}
		}
		else 
		{
			m_stuYSH.rect.x1 = point.x;
			m_stuYSH.rect.y0 = point.y;
		}
		break;

	case 4:
		m_stuYSH.rect.x1 = point.x;
		break;

	case 5:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_stuYSH.rect.x0) > fabs(point.y-m_stuYSH.rect.y0)) 
			{
				m_stuYSH.rect.y1 = point.y;
				if (point.x < m_stuYSH.rect.x0)
					m_stuYSH.rect.x1 = m_stuYSH.rect.x0-(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
				else
					m_stuYSH.rect.x1 = m_stuYSH.rect.x0+(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
			} 
			else
			{
				m_stuYSH.rect.x1 = point.x;
				if (point.y < m_stuYSH.rect.y0)
					m_stuYSH.rect.y1 = m_stuYSH.rect.y0-(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
				else
					m_stuYSH.rect.y1 = m_stuYSH.rect.y0+(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
			}
		}
		else 
		{
			m_stuYSH.rect.x1 = point.x;
			m_stuYSH.rect.y1 = point.y;
		}
		break;

	case 6:
		m_stuYSH.rect.y1 = point.y;
		break;

	case 7:
		if ((nFlags&MK_SHIFT) != 0)
		{
			if (fabs(point.x-m_stuYSH.rect.x1) > fabs(point.y-m_stuYSH.rect.y0)) 
			{
				m_stuYSH.rect.y1 = point.y;
				if (point.x < m_stuYSH.rect.x1)
					m_stuYSH.rect.x0 = m_stuYSH.rect.x1-(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
				else
					m_stuYSH.rect.x0 = m_stuYSH.rect.x1+(float)fabs(m_stuYSH.rect.y1-m_stuYSH.rect.y0);
			} 
			else
			{
				m_stuYSH.rect.x0 = point.x;
				if (point.y < m_stuYSH.rect.y0)
					m_stuYSH.rect.y1 = m_stuYSH.rect.y0-(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
				else
					m_stuYSH.rect.y1 = m_stuYSH.rect.y0+(float)fabs(m_stuYSH.rect.x1-m_stuYSH.rect.x0);
			}
		}
		else 
		{
			m_stuYSH.rect.x0 = point.x;
			m_stuYSH.rect.y1 = point.y;
		}
		break;

	case 8:
		m_stuYSH.rect.x0 = point.x;
		break;
	}

	pView->InvalidateTy(this);
}

void CYSH::Ty_Move( float xLen, float yLen,CElecMapView * pView/*=NULL*/ )
{
	if (pView!=NULL) pView->InvalidateTy(this);
	if (m_stuYSH.rect.x0+xLen>=0) 
	{
		m_stuYSH.rect.x0+=xLen;
		m_stuYSH.rect.x1+=xLen;
	}
	if (m_stuYSH.rect.y0+yLen>=0)
	{
		m_stuYSH.rect.y0+=yLen;
		m_stuYSH.rect.y1+=yLen;
	}
	if (pView!=NULL) pView->InvalidateTy(this);
}

PointStruct CYSH::GetHandle( int nHandle )
{
	ASSERT_VALID(this);
	nHandle--;
	PointStruct pt;
	pt.x=0;pt.y=0;
	if (nHandle==0)
	{
		pt.x=m_stuYSH.rect.x0;
		pt.y=m_stuYSH.rect.y0;
	}
	if (nHandle==4)
	{
		pt.x=m_stuYSH.rect.x0;
		pt.y=m_stuYSH.rect.y0+(m_stuYSH.rect.y1-m_stuYSH.rect.y0)/2;
	}
	if (nHandle==2)
	{
		pt.x=m_stuYSH.rect.x0;
		pt.y=m_stuYSH.rect.y1;
	}
	if (nHandle==3)
	{
		pt.x=m_stuYSH.rect.x0+(m_stuYSH.rect.x1-m_stuYSH.rect.x0)/2;
		pt.y=m_stuYSH.rect.y1;
	}
	if (nHandle==1)
	{
		pt.x=m_stuYSH.rect.x1;
		pt.y=m_stuYSH.rect.y1;
	}
	if (nHandle==5)
	{
		pt.x=m_stuYSH.rect.x1;
		pt.y=m_stuYSH.rect.y0+(m_stuYSH.rect.y1-m_stuYSH.rect.y0)/2;
	}
	if (nHandle==6)
	{
		pt.x=m_stuYSH.rect.x1;
		pt.y=m_stuYSH.rect.y0;
	}
	if (nHandle==7)
	{
		pt.x=m_stuYSH.rect.x0+(m_stuYSH.rect.x1-m_stuYSH.rect.x0)/2;
		pt.y=m_stuYSH.rect.y0;
	}
	return pt;
}

int CYSH::GetHandleCount()
{
	return (int)(8);
}

HCURSOR CYSH::GetHandleCursor( int nHandle )
{
	ASSERT_VALID(this);
	nHandle--;
	LPCTSTR id;
	switch (nHandle)
	{
	case 0:
		id = IDC_SIZENWSE;
		break;
	case 4:
		id = IDC_SIZEWE;
		break;
	case 2:
		id = IDC_SIZENESW;
		break;
	case 3:
		id = IDC_SIZENS;
		break;
	case 1:
		id = IDC_SIZENWSE;
		break;
	case 5:
		id = IDC_SIZEWE;
		break;
	case 6:
		id = IDC_SIZENESW;
		break;
	case 7:
		id = IDC_SIZENS;
		break;
	case 8:
		id = IDC_SIZEALL;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

BOOL CYSH::IntersectRect( float x0, float y0, float x1, float y1 )
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

BOOL CYSH::PointInObj( float x,float y,float errRange )
{
	ASSERT(errRange>=0);
	if (m_bDelete) {return false;}
	if ((x>=max(0,m_stuYSH.rect.x0-errRange))&&
		(y>=max(0,m_stuYSH.rect.y0-errRange))&&
		(x<=m_stuYSH.rect.x1+errRange)&&
		(y<=m_stuYSH.rect.y1+errRange))
	{
		return true;
	}

	return false;
}

CTyBase* CYSH::Clone()
{
	CYSH * pClone = new CYSH();
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}




