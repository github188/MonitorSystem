#include "stdafx.h"
#include "canvas/TyStationAssistPngTool.h"
#include "canvas/TyStationAssistPng.h"
#include "canvas/ImagePng.h"
//#include "sp_drawView.h"
#include "../../ElecMapView.h"
#include "dialog/DlgStationAssistPng.h"
#include "dialog/PropPositionRect.h"


//////////////////////////////////////////////////////////////////////////
CTyStationAssistPng::CTyStationAssistPng()
{
	m_x0 = 0.0f;
	m_y0 = 0.0f;
	m_x1 = 0.0f;
	m_y1 = 0.0f;

	m_bHScale = TRUE;
	m_bVScale = TRUE;	

	m_fRotateAngle = 0.0f;

	memset(m_szPngName,0,sizeof(m_szPngName));
}

CTyStationAssistPng::CTyStationAssistPng(float x0,float y0,float x1,float y1,
															LPCSTR szPngName,float fRotateAngle,
															BOOL bHScale,BOOL bVScale,int nId,
															CHMLayer* pLayer):CTyBase(nId,pLayer)
{
	m_x0 = x0;
	m_y0 = y0;
	m_x1 = x1;
	m_y1 = y1;

	m_bHScale = bHScale;
	m_bVScale = bVScale;	

	m_fRotateAngle = fRotateAngle;

	if (szPngName != NULL)
	{
		strcpy(m_szPngName,szPngName);
		g_ImagePngMapX.RegisterImagePng(m_szPngName);
	}
	else 
	{
		memset(m_szPngName,0,sizeof(m_szPngName));
	}
}

CTyStationAssistPng::~CTyStationAssistPng(void)
{
	g_ImagePngMapX.UnRegisterImagePng(m_szPngName);
}

IMPLEMENT_SERIAL(CTyStationAssistPng, CTyBase, 0)

//////////////////////////////////////////////////////////////////////////
void CTyStationAssistPng::SetDefaultProp(CElecMapView *pView)
{
	try
	{
		ASSERT(pView != NULL);
		if (pView == NULL) 
			return;

		CTyStationAssistPngTool::m_nWidth  = pView->ULtoLL(m_x1-m_x0);
		CTyStationAssistPngTool::m_nHeight = pView->ULtoLL(m_y1-m_y0);
		strcpy(CTyStationAssistPngTool::m_szPngName,m_szPngName);
		CTyStationAssistPngTool::m_bHScale = m_bHScale;
		CTyStationAssistPngTool::m_bVScale = m_bVScale;
		CTyStationAssistPngTool::m_fRotateAngle = m_fRotateAngle;

	}
	catch(...)
	{

	}
}

BOOL CTyStationAssistPng::Is_Net_Color_Bmp()
{
	return FALSE;
}

void CTyStationAssistPng::OnEditProperties(CElecMapView * pView)
{
	CPropertySheet PngSheet( _T("对象..") );

	CDlgStationAssistPng PngDlg;
	PngDlg.m_strPngName = m_szPngName;
	PngDlg.m_fRotateAngle = (int)(m_fRotateAngle*180/pi+0.5);
	PngDlg.m_bCheckHorizontalStretch = m_bHScale;
	PngDlg.m_bCheckVerticalStretch = m_bVScale;
	PngSheet.AddPage((CPropertyPage*)&PngDlg);

	CPropPositionRect PosDlg;

	float x0,y0,x1,y1;
	GetRect(&x0,&y0,&x1,&y1);
	PosDlg.SetContent(x0,y0,x1,y1);
	PngSheet.AddPage( (CPropertyPage*)&PosDlg );

	if (PngSheet.DoModal() == IDOK)
	{
		Invalidate();

		if (PngDlg.m_strPngName.IsEmpty() == FALSE&&PngDlg.m_strPngName.Compare(m_szPngName) != 0)
		{
			g_ImagePngMapX.UnRegisterImagePng(m_szPngName);
			sprintf_s(m_szPngName,sizeof(m_szPngName),"%s",PngDlg.m_strPngName);
			g_ImagePngMapX.RegisterImagePng(m_szPngName);
		}

		PosDlg.GetContent(m_x0,m_y0,m_x1,m_y1);

		m_fRotateAngle = PngDlg.m_fRotateAngle/180*pi;

		SetModifiedFlag();
		Invalidate();
		SetDefaultProp(pView);
	}
}

DRAW_TY CTyStationAssistPng::GetTyType()
{
	return tyStationAssistPng;
}

void CTyStationAssistPng::Ty_Serialize(CArchive &ar)
{
	try
	{
		if(ar.IsStoring())  
		{
			ar<<m_x0<<m_y0<<m_x1<<m_y1;
			ar.Write(m_szPngName,sizeof(m_szPngName));
			ar<<m_bHScale<<m_bVScale;
			ar<<m_fRotateAngle;
		}
		else
		{
			ar>>m_x0>>m_y0>>m_x1>>m_y1;
			ar.Read(m_szPngName,sizeof(m_szPngName));
			ar>>m_bHScale>>m_bVScale;
			ar>>m_fRotateAngle;

			g_ImagePngMapX.RegisterImagePng(m_szPngName);
		}
	}
	catch(...)
	{

	}
}

void CTyStationAssistPng::Ty_Save(CFile *file, BOOL Yn)
{
	try
	{
		if (file == NULL)
			return;

		if(Yn)	//如果是在进行保存
		{
			file->Write((unsigned char *)&m_x0,sizeof(m_x0));
			file->Write((unsigned char *)&m_y0,sizeof(m_y0));
			file->Write((unsigned char *)&m_x1,sizeof(m_x1));
			file->Write((unsigned char *)&m_y1,sizeof(m_y1));

			file->Write((unsigned char *)m_szPngName,sizeof(m_szPngName));
			file->Write((unsigned char *)&m_bHScale,sizeof(m_bHScale));
			file->Write((unsigned char *)&m_bVScale,sizeof(m_bVScale));
			file->Write((unsigned char *)&m_fRotateAngle,sizeof(m_fRotateAngle));
		} 
		else
		{
			file->Read((unsigned char *)&m_x0,sizeof(m_x0));
			file->Read((unsigned char *)&m_y0,sizeof(m_y0));
			file->Read((unsigned char *)&m_x1,sizeof(m_x1));
			file->Read((unsigned char *)&m_y1,sizeof(m_y1));

			file->Read((unsigned char *)m_szPngName,sizeof(m_szPngName));
			file->Read((unsigned char *)&m_bHScale,sizeof(m_bHScale));
			file->Read((unsigned char *)&m_bVScale,sizeof(m_bVScale));
			file->Read((unsigned char *)&m_fRotateAngle,sizeof(m_fRotateAngle));

			g_ImagePngMapX.RegisterImagePng(m_szPngName);	
		}
	}
	catch(...)
	{

	}
}

void CTyStationAssistPng::Ty_MoveHandleTo(int nHandle, PointStruct point, CElecMapView *pView,UINT nFlags)
{
	try
	{
		ASSERT(pView != NULL);

		if (pView == NULL)
			return;

		pView->InvalidateTy(this);

		switch (nHandle)
		{
		case 1:
			if ((nFlags&MK_SHIFT) != 0)
			{
				if (fabs(point.x-m_x1)>fabs(point.y-m_y1)) 
				{
					m_y0 = point.y;

					if (point.x < m_x1)
						m_x0 = m_x1-(float)fabs(m_y1-m_y0);
					else
						m_x0 = m_x1+(float)fabs(m_y1-m_y0);
				} 
				else
				{
					m_x0 = point.x;
					if (point.y < m_y1)
						m_y0 = m_y1-(float)fabs(m_x1-m_x0);
					else
						m_y0 = m_y1+(float)fabs(m_x1-m_x0);
				}
			}
			else 
			{
				m_x0 = point.x;
				m_y0 = point.y;
			}
			break;

		case 2:
			m_y0 = point.y;
			break;

		case 3:
			if ((nFlags&MK_SHIFT) != 0)
			{
				if (fabs(point.x-m_x0) > fabs(point.y-m_y1)) 
				{
					m_y0 = point.y;
					if (point.x < m_x0)
						m_x1 = m_x0-(float)fabs(m_y1-m_y0);
					else
						m_x1 = m_x0+(float)fabs(m_y1-m_y0);
				} 
				else
				{
					m_x1 = point.x;
					if (point.y < m_y1)
						m_y0 = m_y1-(float)fabs(m_x1-m_x0);
					else
						m_y0 = m_y1+(float)fabs(m_x1-m_x0);
				}
			}
			else 
			{
				m_x1 = point.x;
				m_y0 = point.y;
			}
			break;

		case 4:
			m_x1 = point.x;
			break;

		case 5:
			if ((nFlags&MK_SHIFT) != 0)
			{
				if (fabs(point.x-m_x0) > fabs(point.y-m_y0)) 
				{
					m_y1 = point.y;
					if (point.x < m_x0)
						m_x1 = m_x0-(float)fabs(m_y1-m_y0);
					else
						m_x1 = m_x0+(float)fabs(m_y1-m_y0);
				} 
				else
				{
					m_x1 = point.x;
					if (point.y < m_y0)
						m_y1 = m_y0-(float)fabs(m_x1-m_x0);
					else
						m_y1 = m_y0+(float)fabs(m_x1-m_x0);
				}
			}
			else 
			{
				m_x1 = point.x;
				m_y1 = point.y;
			}
			break;

		case 6:
			m_y1 = point.y;
			break;

		case 7:
			if ((nFlags&MK_SHIFT) != 0)
			{
				if (fabs(point.x-m_x1) > fabs(point.y-m_y0)) 
				{
					m_y1 = point.y;
					if (point.x < m_x1)
						m_x0 = m_x1-(float)fabs(m_y1-m_y0);
					else
						m_x0 = m_x1+(float)fabs(m_y1-m_y0);
				} 
				else
				{
					m_x0 = point.x;
					if (point.y < m_y0)
						m_y1 = m_y0-(float)fabs(m_x1-m_x0);
					else
						m_y1 = m_y0+(float)fabs(m_x1-m_x0);
				}
			}
			else 
			{
				m_x0 = point.x;
				m_y1 = point.y;
			}
			break;

		case 8:
			m_x0 = point.x;
			break;

		default:
			break;
		}

		pView->InvalidateTy(this);

		SetDefaultProp(pView);
	}
	catch(...)
	{

	}
}

void CTyStationAssistPng::Ty_Move(float xLen, float yLen,CElecMapView * pView)
{
	try
	{
		if (pView != NULL) 
		{
			pView->InvalidateTy(this);
		}

		m_x0 += xLen;
		m_x1 += xLen;
		m_y0 += yLen;
		m_y1 += yLen;

		if (pView != NULL) 
		{
			pView->InvalidateTy(this);
		}
	}
	catch(...)
	{

	}
}

HCURSOR CTyStationAssistPng::GetHandleCursor(int nHandle)
{
	try
	{
		ASSERT_VALID(this);

		LPCTSTR id;

		switch (nHandle)
		{
		case 1:
		case 5:
			id = IDC_SIZENWSE;
			break;

		case 2:
		case 6:
			id = IDC_SIZENS;
			break;

		case 3:
		case 7:
			id = IDC_SIZENESW;
			break;

		case 4:
		case 8:
			id = IDC_SIZEWE;
			break;

		default:
			break;
		}

		return AfxGetApp()->LoadStandardCursor(id);
	}
	catch(...)
	{

	}

	return NULL;
}

PointStruct CTyStationAssistPng::GetHandle(int nHandle)
{
	PointStruct pt;
	
	try
	{
		pt.x = 0.0f;
		pt.y = 0.0f;

		ASSERT_VALID(this);

		float x0,y0,x1,y1;

		GetRect(&x0,&y0,&x1,&y1);

		switch (nHandle)
		{
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

		default:
			break;

		}

		return pt;

	}
	catch(...)
	{

	}

	return pt;
}

int CTyStationAssistPng::GetHandleCount()
{
	return 8;
}

BOOL CTyStationAssistPng::PointInObj(float x, float y,float errRange)
{
	try
	{
		ASSERT(errRange >= 0);

		if (m_bDelete) 
			return FALSE; 

		BOOL ret = FALSE;

		RectStruct rt,rt2;
		GetRect(&rt.x0,&rt.y0,&rt.x1,&rt.y1);

		rt2 = rt;
		rt2.InflateRect(errRange,errRange);

		ret = PointInRect(x,y,rt2);

		return ret;
	}
	catch(...)
	{

	}
	return FALSE;
}

BOOL CTyStationAssistPng::IntersectRect(float x0, float y0, float x1, float y1)
{
	return CTyBase::IntersectRect(x0,y0,x1,y1);
}

CTyBase *CTyStationAssistPng::Clone()
{
	try
	{
		CTyStationAssistPng* pClone = new CTyStationAssistPng(m_x0,m_y0,m_x1,m_y1,m_szPngName,m_fRotateAngle,m_bHScale,m_bVScale,m_Id,m_pHMLayer);
		if (pClone == NULL)
			return NULL;

		return (CTyBase *)pClone;
	}
	catch(...)
	{

	}

	return NULL;
}

void CTyStationAssistPng::GetRect(float *minX, float *minY, float *maxX, float *maxY)
{
	try
	{
		if (minX == NULL||minY == NULL||maxX == NULL||maxY == NULL)
			return;

		*minX  = __min(m_x0,m_x1);
		*minY  = __min(m_y0,m_y1);
		*maxX = __max(m_x0,m_x1);
		*maxY = __max(m_y0,m_y1);
	}
	catch(...)
	{

	}
}

void CTyStationAssistPng::DrawDC(CDC *pDC, CElecMapView *pView)
{
	try
	{
		ASSERT(pDC != NULL);
		ASSERT(pView != NULL);

		if (pDC == NULL||pView == NULL)
			return;

		if(m_bDelete)	//如果已经处于删除状态
			return;

		float minx,miny,maxx,maxy;
		pView->GetFloatRect(&minx,&miny,&maxx,&maxy);
		if (!IntersectRect(minx,miny,maxx,maxy))
			return;

		GetRect(&minx,&miny,&maxx,&maxy);

		CPoint pt1,pt2;
		pt1 = pView->UPtoLP(minx,miny);
		pt2 = pView->UPtoLP(maxx,maxy);

		CRect rt;
		rt.SetRect(pt1,pt2);
		rt.NormalizeRect();

		if ((rt.Width() < 1)||(rt.Height() < 1))
			return;

		g_ImagePngMapX.DrawImagePng(m_szPngName,pDC->m_hDC,pt1.x,pt1.y,rt.Width(),rt.Height(),pt1.x,pt1.y,rt.Width(),rt.Height(),m_fRotateAngle,SRCCOPY);
	}
	catch(...)
	{

	}
}