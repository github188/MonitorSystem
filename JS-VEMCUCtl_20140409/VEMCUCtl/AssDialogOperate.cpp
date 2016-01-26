#include "stdafx.h"
#include "resource.h"
#include "AssDialogOperate.h"

//ÉèÖÃ±³¾°Í¼Æ¬,À­Éì
void SetDialogBKPic(CDialog *pDlg, CDC *pDC)
{
	CBitmap bmpBack;
	bmpBack.LoadBitmap(IDB_BITMAP_VIEW_OUT_TOP_BK);

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp;
	BITMAP bmp;
	CRect winrc;

	pDlg->GetClientRect(&winrc);

	pOldBmp = srcDC.SelectObject(&bmpBack);
	bmpBack.GetBitmap(&bmp);
	pDC->StretchBlt(0,0,winrc.Width(),winrc.Height(),
		&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	srcDC.SelectObject(pOldBmp);
}