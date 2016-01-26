#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/ty_ycyw.h"
//#include "sp_drawview.h"
#include "../../ElecMapView.h"
#include "dialog/SZ_YcYwDlg.h"

IMPLEMENT_SERIAL(CTy_YcYw, CTyBase, 0)

CTy_YcYw::CTy_YcYw(void)
{
	m_para.x0=0;
	m_para.y0=0;
	m_para.x1=1;
	m_para.y1=1;
	strcpy(m_para.Ename,"");
	m_para.Color=RGB(0,0,255);
	m_para.DisableColor=RGB(128,128,128);
	m_para.LwLevelColor=RGB(0,255,0);
	m_para.HiLevelColor=RGB(255,0,0);
	m_para.max=0;
	m_para.min=0;
	m_para.drawtype=YW_DRAW_DOWNUP;
}

CTy_YcYw::~CTy_YcYw(void)
{
}

CTy_YcYw::CTy_YcYw(float x0,float y0,float x1,float y1,char* Ename)
{
	m_para.x0=x0;
	m_para.y0=y0;
	m_para.x1=x1;
	m_para.y1=y1;
	strcpy(m_para.Ename,Ename);
	m_para.Color=RGB(0,0,255);
	m_para.DisableColor=RGB(128,128,128);
	m_para.LwLevelColor=RGB(0,255,0);
	m_para.HiLevelColor=RGB(255,0,0);
	m_para.max=0;
	m_para.min=0;
	m_para.drawtype=YW_DRAW_DOWNUP;
}

CTy_YcYw::CTy_YcYw(S_TYYCYW newVal)
{
	m_para=newVal;
}

void CTy_YcYw::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet sheet( _T("液位显示对象..") );

    CSZ_YcYwDlg dlg;
	dlg.SetPara(m_para.Color,m_para.DisableColor,m_para.HiLevelColor,m_para.LwLevelColor,
		m_para.Ename,m_para.max,m_para.min,m_para.drawtype);
	sheet.AddPage(&dlg);

	if (sheet.DoModal() != IDOK)
		return;
	dlg.GetPara(&m_para.Color,&m_para.DisableColor,&m_para.HiLevelColor,&m_para.LwLevelColor,
		m_para.Ename,&m_para.max,&m_para.min,&m_para.drawtype);
	SetModifiedFlag();
	Invalidate();
}

DRAW_TY CTy_YcYw::GetTyType()
{
	return tyYcYw;
}

void CTy_YcYw::Ty_Serialize(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write(&m_para,sizeof(S_TYYCYW));
	}
	else
	{
		ar.Read(&m_para,sizeof(S_TYYCYW));
	}
}

void CTy_YcYw::Ty_Save(CFile *file, BOOL Yn)
{
	if (Yn)
	{
		file->Write((unsigned char *)&m_para,sizeof(S_TYYCYW));
	}
	else
	{
		file->Read((unsigned char *)&m_para,sizeof(S_TYYCYW));
	}
}

void CTy_YcYw::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	ASSERT(pView!=NULL);

	pView->InvalidateTy(this);

	switch (nHandle)
	{
		default:
			ASSERT(FALSE);
			break;
		case 1:
			if ((nFlags&MK_SHIFT)!=0)
			{
				if (fabs(point.x-m_para.x1)>fabs(point.y-m_para.y1)) 
				{
					m_para.y0=point.y;
					if (point.x<m_para.x1)
						m_para.x0=m_para.x1-(float)fabs(m_para.y1-m_para.y0);
					else
						m_para.x0=m_para.x1+(float)fabs(m_para.y1-m_para.y0);
				} 
				else
				{
					m_para.x0=point.x;
					if (point.y<m_para.y1)
						m_para.y0=m_para.y1-(float)fabs(m_para.x1-m_para.x0);
					else
						m_para.y0=m_para.y1+(float)fabs(m_para.x1-m_para.x0);
				}
			}
			else 
			{
				m_para.x0=point.x;
				m_para.y0=point.y;
			}
			break;
		case 2:
			m_para.y0=point.y;
			break;
		case 3:
			if ((nFlags&MK_SHIFT)!=0)
			{
				if (fabs(point.x-m_para.x0)>fabs(point.y-m_para.y1)) 
				{
					m_para.y0=point.y;
					if (point.x<m_para.x0)
						m_para.x1=m_para.x0-(float)fabs(m_para.y1-m_para.y0);
					else
						m_para.x1=m_para.x0+(float)fabs(m_para.y1-m_para.y0);
				} 
				else
				{
					m_para.x1=point.x;
					if (point.y<m_para.y1)
						m_para.y0=m_para.y1-(float)fabs(m_para.x1-m_para.x0);
					else
						m_para.y0=m_para.y1+(float)fabs(m_para.x1-m_para.x0);
				}
			}
			else 
			{
				m_para.x1=point.x;
				m_para.y0=point.y;
			}
			break;
		case 4:
			m_para.x1=point.x;
			break;
		case 5:
			if ((nFlags&MK_SHIFT)!=0)
			{
				if (fabs(point.x-m_para.x0)>fabs(point.y-m_para.y0)) 
				{
					m_para.y1=point.y;
					if (point.x<m_para.x0)
						m_para.x1=m_para.x0-(float)fabs(m_para.y1-m_para.y0);
					else
						m_para.x1=m_para.x0+(float)fabs(m_para.y1-m_para.y0);
				} 
				else
				{
					m_para.x1=point.x;
					if (point.y<m_para.y0)
						m_para.y1=m_para.y0-(float)fabs(m_para.x1-m_para.x0);
					else
						m_para.y1=m_para.y0+(float)fabs(m_para.x1-m_para.x0);
				}
			}
			else 
			{
  				m_para.x1=point.x;
	  			m_para.y1=point.y;
			}
			break;
		case 6:
			m_para.y1=point.y;
			break;
		case 7:
			if ((nFlags&MK_SHIFT)!=0)
			{
				if (fabs(point.x-m_para.x1)>fabs(point.y-m_para.y0)) 
				{
					m_para.y1=point.y;
					if (point.x<m_para.x1)
						m_para.x0=m_para.x1-(float)fabs(m_para.y1-m_para.y0);
					else
						m_para.x0=m_para.x1+(float)fabs(m_para.y1-m_para.y0);
				} 
				else
				{
					m_para.x0=point.x;
					if (point.y<m_para.y0)
						m_para.y1=m_para.y0-(float)fabs(m_para.x1-m_para.x0);
					else
						m_para.y1=m_para.y0+(float)fabs(m_para.x1-m_para.x0);
				}
			}
			else 
			{
				m_para.x0=point.x;
				m_para.y1=point.y;
			}
			break;
		case 8:
			m_para.x0=point.x;
			break;
	}

	pView->InvalidateTy(this);
}

void CTy_YcYw::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	if (pView!=NULL) pView->InvalidateTy(this);
	m_para.x0+=xLen;
	m_para.x1+=xLen;
	m_para.y0+=yLen;
	m_para.y1+=yLen;
	if (pView!=NULL) pView->InvalidateTy(this);
}

HCURSOR CTy_YcYw::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
		default:
			ASSERT(FALSE);
		case 1:
		case 5:
			id=IDC_SIZENWSE;
			break;
		case 2:
		case 6:
			id=IDC_SIZENS;
			break;
		case 3:
		case 7:
			id=IDC_SIZENESW;
			break;
		case 4:
		case 8:
			id=IDC_SIZEWE;
			break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

PointStruct CTy_YcYw::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	PointStruct pt;

	float x0,y0,x1,y1;

	GetRect(&x0,&y0,&x1,&y1);
  

	switch (nHandle)
	{
		default:
			ASSERT(FALSE);
		case 1:
			pt.x = x0;
			pt.y = y0;
			break;
		case 2:
			pt.x = (x1+x0)/2;
			pt.y = y0;
			break;
		case 3:
			pt.x = x1;
			pt.y = y0;
			break;
		case 4:
			pt.x = x1;
			pt.y = (y0+y1)/2;
			break;
		case 5:
			pt.x = x1;
			pt.y = y1;
			break;
		case 6:
			pt.x = (x1+x0)/2;
			pt.y = y1;
			break;
		case 7:
			pt.x = x0;
			pt.y = y1;
			break;
		case 8:
			pt.x = x0;
			pt.y = (y0+y1)/2;
			break;
	}

	return pt;
}

int CTy_YcYw::GetHandleCount()
{
	return 8;
}

BOOL CTy_YcYw::PointInObj(float x, float y,float errRange)
{
	ASSERT(errRange>=0);
	if (m_bDelete) return FALSE; 

	BOOL ret=FALSE;
	RectStruct rt,rt2;
	GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

	rt2=rt;
	rt2.InflateRect(errRange,errRange);
	ret=PointInRect(x,y,rt2);

	return ret;
}

BOOL CTy_YcYw::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase * CTy_YcYw::Clone()
{
	ASSERT_VALID(this);

	CTy_YcYw* pClone = new CTy_YcYw(m_para);
	ASSERT_VALID(pClone);

	return (CTyBase *)pClone;
}

void CTy_YcYw::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	*minX=__min(m_para.x0,m_para.x1);
	*minY=__min(m_para.y0,m_para.y1);
	*maxX=__max(m_para.x0,m_para.x1);
	*maxY=__max(m_para.y0,m_para.y1);
}

void CTy_YcYw::DrawDC(CDC *pDC, CElecMapView *pView)
{
	ASSERT(pDC!=NULL);
	ASSERT(pView!=NULL);

	if(m_bDelete)	//如果已经处于删除状态
		return;

	float minx,miny,maxx,maxy;
	pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
	if (!IntersectRect(minx,miny,maxx,maxy)) return;


	GetRect(&minx,&miny,&maxx,&maxy);
	CPoint pt1,pt2;
	pt1=pView->UPtoLP(minx,miny);
	pt2=pView->UPtoLP(maxx,maxy);
	CRect rt;
	rt.SetRect(pt1,pt2);
	rt.NormalizeRect();
	if ((rt.Width()<1)||(rt.Height()<1)) return;

	HBRUSH wBrush = ::CreateSolidBrush(m_para.Color);
    HBRUSH hBrold = (HBRUSH)::SelectObject(pDC->m_hDC, wBrush);
	HPEN hPen=::CreatePen(PS_SOLID, 0, m_para.Color);
	HPEN hOldPen=(HPEN)::SelectObject(pDC->m_hDC, hPen);
	::Rectangle(pDC->m_hDC, rt.left,rt.top,rt.right,rt.bottom);
	
	::SelectObject(pDC->m_hDC,hOldPen);
	::SelectObject(pDC->m_hDC, hBrold);
	::DeleteObject(hPen);
	::DeleteObject(wBrush);
}

void CTy_YcYw::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret=GetEname_From_ReplacRule(m_para.Ename,str,ruleList);
	if (ret>0)
	{
		SetModifiedFlag(true);
		strcpy(m_para.Ename,str);
	}
}
