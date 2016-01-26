#include "stdafx.h"
#include "canvas/ImagePng.h"
#include "canvas/png/png.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////
#define  pi  3.1415926

//////////////////////////////////////////////////////////////////////////
CImagePngMapX g_ImagePngMapX;

//////////////////////////////////////////////////////////////////////////
CImagePng::CImagePng(void)
{
	memset(m_szPngName,0,sizeof(m_szPngName));
	m_hBmp = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nCount = 0;
}

CImagePng::~CImagePng(void)
{
	memset(m_szPngName,0,sizeof(m_szPngName));

	if (m_hBmp != NULL)
	{
		DeleteObject(m_hBmp);
		m_hBmp = NULL;
	}

	m_nWidth = 0;
	m_nHeight = 0;
	m_nCount = 0;
}

BOOL CImagePng::LoadImagePng(char *szPngName)
{
	try
	{
		if (szPngName == NULL||strlen(szPngName) >= sizeof(m_szPngName))
			return FALSE;

		FILE* file = NULL;
		HDC hDC  = NULL;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;

		do 
		{
			file = fopen(szPngName,"rb");
			if (file == NULL)
				break;

			hDC =  ::CreateCompatibleDC(NULL);
			if (hDC == NULL)
				break;

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
			if (png_ptr == NULL)
				break;

			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL)
				break;

			setjmp(png_jmpbuf(png_ptr));

			png_init_io(png_ptr, file);

			// 读文件
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

			// 得到文件的宽高色深
			int width = png_get_image_width(png_ptr,info_ptr);
			int height = png_get_image_height(png_ptr, info_ptr);

			int color_type = png_get_color_type(png_ptr, info_ptr);

			// 申请内存
			int size = height*width*4;

			// row_pointers里边就是rgba数据
			png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
			if (row_pointers == NULL)
				break;

			BITMAPINFO bmi;

			memset(&bmi, 0, sizeof(bmi));

			bmi.bmiHeader.biSize = sizeof(BITMAPINFO);

			bmi.bmiHeader.biWidth = width;

			bmi.bmiHeader.biHeight = height;

			bmi.bmiHeader.biPlanes = 1;

			bmi.bmiHeader.biBitCount = 32;

			bmi.bmiHeader.biCompression = BI_RGB;

			bmi.bmiHeader.biSizeImage = size;

			unsigned char* bgra = NULL;

			m_hBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,(void**)&bgra,NULL,0);
			if (m_hBmp == NULL)
				break;

			int pos = 0;
			int nAlpha = 0;

			for(int i = 0; i < height; i++)
			{
				for(int j = 0; j < (4 * width); j += 4)
				{
					nAlpha = row_pointers[height-i-1][j + 3]; 
					bgra[pos++] = (row_pointers[height-i-1][j + 2]*nAlpha+127)/255; // blue
					bgra[pos++] = (row_pointers[height-i-1][j + 1]*nAlpha+127)/255; // green
					bgra[pos++] = (row_pointers[height-i-1][j]*nAlpha+127)/255;        // red
					bgra[pos++] = row_pointers[height-i-1][j + 3];              // alpha
				}
			}

			::DeleteDC(hDC);

			png_destroy_read_struct(&png_ptr, &info_ptr, 0);

			fclose(file);

			m_nWidth = width;
			m_nHeight = height;

			return TRUE;

		} while (FALSE);

		if (file != NULL)
		{
			fclose(file);
			file = NULL;
		}

		if (hDC != NULL)
		{
			::DeleteDC(hDC);
			hDC = NULL;
		}

		if (m_hBmp != NULL)
		{
			::DeleteObject(m_hBmp);
			m_hBmp = NULL;
		}

		if (png_ptr != NULL||info_ptr != NULL)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			png_ptr = NULL;
			info_ptr = NULL;
		}

		return FALSE;
	}
	catch(...)
	{

	}
	return FALSE;
}

int CImagePng::GetImagePngWidth()
{
	if (m_hBmp == NULL)
		return -1;

	return m_nWidth;
}

int CImagePng::GetImagePngHeight()
{
	if (m_hBmp == NULL)
		return -1;

	return m_nHeight;
}

BOOL CImagePng::DrawImagePng(HDC hDC,int nXDst,int nYDst,int nWidthDst,int nHeightDst,int nXSrc,int nYSrc,int nWidthSrc,int nHeightSrc,double dbRotateAngle,DWORD dwRop)
{
	try
	{
		if (m_nWidth <= 0||m_nHeight <= 0||m_hBmp == NULL)
			return FALSE;

		BOOL bResult = FALSE;

		BLENDFUNCTION bf;

		bf.BlendOp =  AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 255;
		bf.AlphaFormat = AC_SRC_ALPHA;

		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_hBmp);

		HDC hRotateMemDC = CreateCompatibleDC(hDC);
		HBITMAP hRotateBmp = CreateCompatibleBitmap(hDC,nWidthDst,nHeightDst);
		HBITMAP hOldBmp2 = (HBITMAP)SelectObject(hRotateMemDC, hRotateBmp);

		BITMAP bm;
		::GetObject(m_hBmp,sizeof(bm),&bm);

		POINT CenterPoint;
		CenterPoint.x = bm.bmWidth/2;
		CenterPoint.y = bm.bmHeight/2;

		float sin_angle = (float)sin(dbRotateAngle);
		float cos_angle = (float)cos(dbRotateAngle);

		//旋转世界坐标系统
		XFORM xform;
		memset(&xform,0,sizeof(xform));
		xform.eM11 = cos_angle;
		xform.eM12 = -sin_angle;
		xform.eM21 = sin_angle;
		xform.eM22 = cos_angle;
		xform.eDx = (float)(CenterPoint.x - (CenterPoint.x*cos_angle+CenterPoint.y*sin_angle));
		xform.eDy = (float)(CenterPoint.y - (-CenterPoint.x*sin_angle+CenterPoint.y*cos_angle));

		int nOldMode = SetGraphicsMode(hRotateMemDC,GM_ADVANCED);
		bResult = SetWorldTransform(hRotateMemDC,&xform);

		float fRateX = (float)nWidthDst/m_nWidth;
		float fRateY = (float)nHeightDst/m_nHeight;

		memset(&xform,0,sizeof(xform));
		xform.eM11 = fRateX;
		xform.eM12 = 0;
		xform.eM21 = 0;
		xform.eM22 = fRateY;
		xform.eDx = 0;
		xform.eDy = 0;

		//缩放世界坐标系统
		bResult = ModifyWorldTransform(hRotateMemDC,&xform,MWT_RIGHTMULTIPLY);

		//旋转图像
		bResult = AlphaBlend(hRotateMemDC,0,0,m_nWidth,m_nHeight,hMemDC,0,0,m_nWidth,m_nHeight,bf);

		//还原世界坐标系统
		xform.eM11 = (float)1.0f;
		xform.eM12 = (float)0.0f;
		xform.eM21 = (float)0.0f;
		xform.eM22 = (float)1.0f;
		xform.eDx = (float)0.0f;
		xform.eDy = (float)0.0f;

		bResult = SetWorldTransform(hRotateMemDC,&xform);
		
		//还原图形模式
		SetGraphicsMode(hDC,nOldMode);

		//复制图像
		AlphaBlend(hDC,nXDst,nYDst,nWidthDst,nHeightDst,hRotateMemDC,0,0,nWidthDst,nHeightDst,bf);

		SelectObject(hRotateMemDC,hOldBmp2);
		DeleteObject(hRotateBmp);
		DeleteDC(hRotateMemDC);

		SelectObject(hMemDC,hOldBmp);
		DeleteDC(hMemDC);

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
CImagePngMapX::CImagePngMapX()
{

}

CImagePngMapX::~CImagePngMapX()
{

}

BOOL CImagePngMapX::RegisterImagePng(char *szPngName)
{
	CImagePng* pImagePng = NULL;

	try
	{
		if (m_MapPngList.Lookup(szPngName,pImagePng)) 
		{
			pImagePng->m_nCount++;
		}
		else 
		{
			pImagePng = AddImagePng(szPngName);
			if (pImagePng == NULL) 
				return FALSE;

			pImagePng->m_nCount = 1;
		}

		return TRUE;
	}
	catch(...)
	{

	}

	return FALSE;
}

void CImagePngMapX::UnRegisterImagePng(char *szPngName)
{
	CImagePng* pImagePng = NULL;

	try
	{
		if (!m_MapPngList.Lookup(szPngName,pImagePng))  
			return;

		if (pImagePng == NULL)
			return;

		pImagePng->m_nCount--;

		if (pImagePng->m_nCount <= 0)
		{
			m_MapPngList.RemoveKey(szPngName);
			delete pImagePng;
			pImagePng = NULL;
		}
	}
	catch(...)
	{

	}
}

CImagePng* CImagePngMapX::AddImagePng(char *szPngName)
{
	try
	{
		if (szPngName == NULL)
			return NULL;

		CImagePng* pImagePng = new CImagePng();
		if(pImagePng == NULL)
			return NULL;

		BOOL bResult = pImagePng->LoadImagePng(szPngName);
		if (bResult == FALSE)
		{
			delete pImagePng;
			pImagePng = NULL;
			return NULL;
		}

		m_MapPngList[szPngName] = pImagePng;
	}
	catch(...)
	{

	}

	return NULL;
}

BOOL CImagePngMapX::DrawImagePng(char *szPngName,HDC hDC,int nXDst,int nYDst,int nWidthDst,int nHeightDst,int nXSrc,int nYSrc,int nWidthSrc,int nHeightSrc,double dbRotateAngle,DWORD dwRop)
{
	CImagePng* pImagePng = NULL;
	BOOL bResult = FALSE;

	try
	{
		if (!m_MapPngList.Lookup(szPngName,pImagePng))  
			return FALSE;

		if (pImagePng == NULL)
			return FALSE;

		bResult = pImagePng->DrawImagePng(hDC,nXDst,nYDst,nWidthDst,nHeightDst,nXSrc,nYSrc,nWidthSrc,nHeightSrc,dbRotateAngle,dwRop);

		return bResult;
	}
	catch(...)
	{

	}

	return FALSE;
}



