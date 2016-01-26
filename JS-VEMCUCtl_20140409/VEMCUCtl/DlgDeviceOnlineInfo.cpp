// DlgDeviceOnlineInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "VEMCUCtlDlg.h"
#include "DlgDeviceOnlineInfo.h"


CDlgDeviceOnlineInfo *g_pDlgDeviceOnlineInfo = NULL;


// CDlgDeviceOnlineInfo 对话框

IMPLEMENT_DYNAMIC(CDlgDeviceOnlineInfo, CDialog)

CDlgDeviceOnlineInfo::CDlgDeviceOnlineInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceOnlineInfo::IDD, pParent)
	, m_strProblemCameraNum(_T(""))
	, m_strAllCameraNum(_T(""))
	, m_strCameraOnlineRate(_T(""))
	, m_strOffStationNum(_T(""))
	, m_strAllStationNum(_T(""))
	, m_strStationOnlineRate(_T(""))
	, m_strProblemCameraNum_500KV(_T(""))
	, m_strAllCameraNum_500KV(_T(""))
	, m_strCameraOnlineRate_500KV(_T(""))
	, m_strOffStationNum_500KV(_T(""))
	, m_strAllStationNum_500KV(_T(""))
	, m_strStationOnlineRate_500KV(_T(""))
	, m_strProblemCameraNum_220KV(_T(""))
	, m_strAllCameraNum_220KV(_T(""))
	, m_strCameraOnlineRate_220KV(_T(""))
	, m_strOffStationNum_220KV(_T(""))
	, m_strAllStationNum_220KV(_T(""))
	, m_strStationOnlineRate_220KV(_T(""))
	, m_strProblemCameraNum_110KV(_T(""))
	, m_strAllCameraNum_110KV(_T(""))
	, m_strCameraOnlineRate_110KV(_T(""))
	, m_strOffStationNum_110KV(_T(""))
	, m_strAllStationNum_110KV(_T(""))
	, m_strStationOnlineRate_110KV(_T(""))
	, m_strProblemCameraNum2(_T(""))
	, m_strAllCameraNum2(_T(""))
	, m_strCameraOnlineRate2(_T(""))
	, m_strOffStationNum2(_T(""))
	, m_strAllStationNum2(_T(""))
	, m_strStationOnlineRate2(_T(""))
{

}

CDlgDeviceOnlineInfo::~CDlgDeviceOnlineInfo()
{

}

void CDlgDeviceOnlineInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROBLEM_CAMERA_NUM, m_strProblemCameraNum);
	DDX_Text(pDX, IDC_EDIT_ALL_CAMERA_NUM, m_strAllCameraNum);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ONLINE_RATE, m_strCameraOnlineRate);
	DDX_Text(pDX, IDC_EDIT_OFF_STATION_NUM, m_strOffStationNum);
	DDX_Text(pDX, IDC_EDIT_ALL_STATION_NUM, m_strAllStationNum);
	DDX_Text(pDX, IDC_EDIT_STATION_ONLINE_RATE, m_strStationOnlineRate);
	DDX_Text(pDX, IDC_EDIT_PROBLEM_CAMERA_NUM_500KV, m_strProblemCameraNum_500KV);
	DDX_Text(pDX, IDC_EDIT_ALL_CAMERA_NUM_500KV, m_strAllCameraNum_500KV);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ONLINE_RATE_500KV, m_strCameraOnlineRate_500KV);
	DDX_Text(pDX, IDC_EDIT_OFF_STATION_NUM_500KV, m_strOffStationNum_500KV);
	DDX_Text(pDX, IDC_EDIT_ALL_STATION_NUM_500KV, m_strAllStationNum_500KV);
	DDX_Text(pDX, IDC_EDIT_STATION_ONLINE_RATE_500KV, m_strStationOnlineRate_500KV);
	DDX_Text(pDX, IDC_EDIT_PROBLEM_CAMERA_NUM_220KV, m_strProblemCameraNum_220KV);
	DDX_Text(pDX, IDC_EDIT_ALL_CAMERA_NUM_220KV, m_strAllCameraNum_220KV);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ONLINE_RATE_220KV, m_strCameraOnlineRate_220KV);
	DDX_Text(pDX, IDC_EDIT_OFF_STATION_NUM_220KV, m_strOffStationNum_220KV);
	DDX_Text(pDX, IDC_EDIT_ALL_STATION_NUM_220KV, m_strAllStationNum_220KV);
	DDX_Text(pDX, IDC_EDIT_STATION_ONLINE_RATE_220KV, m_strStationOnlineRate_220KV);
	DDX_Text(pDX, IDC_EDIT_PROBLEM_CAMERA_NUM_110KV, m_strProblemCameraNum_110KV);
	DDX_Text(pDX, IDC_EDIT_ALL_CAMERA_NUM_110KV, m_strAllCameraNum_110KV);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ONLINE_RATE_110KV, m_strCameraOnlineRate_110KV);
	DDX_Text(pDX, IDC_EDIT_OFF_STATION_NUM_110KV, m_strOffStationNum_110KV);
	DDX_Text(pDX, IDC_EDIT_ALL_STATION_NUM_110KV, m_strAllStationNum_110KV);
	DDX_Text(pDX, IDC_EDIT_STATION_ONLINE_RATE_110KV, m_strStationOnlineRate_110KV);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOk);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Text(pDX, IDC_EDIT_PROBLEM_CAMERA_NUM2, m_strProblemCameraNum2);
	DDX_Text(pDX, IDC_EDIT_ALL_CAMERA_NUM2, m_strAllCameraNum2);
	DDX_Text(pDX, IDC_EDIT_CAMERA_ONLINE_RATE2, m_strCameraOnlineRate2);
	DDX_Text(pDX, IDC_EDIT_OFF_STATION_NUM2, m_strOffStationNum2);
	DDX_Text(pDX, IDC_EDIT_ALL_STATION_NUM2, m_strAllStationNum2);
	DDX_Text(pDX, IDC_EDIT_STATION_ONLINE_RATE2, m_strStationOnlineRate2);
	DDX_Control(pDX, IDC_PROGRESS_RATE_INFO, m_progressRateInfo);
}


BEGIN_MESSAGE_MAP(CDlgDeviceOnlineInfo, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgDeviceOnlineInfo::OnBnClickedBtnOk)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgDeviceOnlineInfo::OnCbnSelchangeComboType)
	ON_MESSAGE(WM_DEVICE_ONLINE_RATE_INFO_UPDATE_MESSAGE, &CDlgDeviceOnlineInfo::OnDeviceOnlineRateInfoUpdateMessageHandler)
END_MESSAGE_MAP()


// CDlgDeviceOnlineInfo 消息处理程序

BOOL CDlgDeviceOnlineInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	//////////////////////////////////////////////////////////////////////////
	LoadSkin();

	InitControl();

	UpdateData(FALSE);

	CenterWindow();
	//////////////////////////////////////////////////////////////////////////
	return TRUE; 
}

void CDlgDeviceOnlineInfo::OnBnClickedBtnOk()
{
	ShowWindow(SW_HIDE);

	if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageViewInfo == NULL)
		return;

	g_pMainDlg->m_pDlgPageViewInfo->StopDeviceOnlineRateReportInfo();
}

BOOL CDlgDeviceOnlineInfo::LoadSkin()
{
	m_Bmp.LoadBitmap(IDB_BITMAP_ONLINE_RATE_BG);
	m_btnOk.LoadBitmap(IDB_BITMAP_CANCEL_BTN_BK);
	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::InitControl()
{
	m_comboType.InsertString(0, "全部站点");
	m_comboType.InsertString(1, "常州市");
	m_comboType.InsertString(2, "淮安市");
	m_comboType.InsertString(3, "连云港市");
	m_comboType.InsertString(4, "南京市");
	m_comboType.InsertString(5, "南通市");
	m_comboType.InsertString(6, "苏州市");
	m_comboType.InsertString(7, "宿迁市");
	m_comboType.InsertString(8, "泰州市");
	m_comboType.InsertString(9, "无锡市");
	m_comboType.InsertString(10, "徐州市");
	m_comboType.InsertString(11, "盐城市");
	m_comboType.InsertString(12, "扬州市");
	m_comboType.InsertString(13, "镇江市");
	m_comboType.SetCurSel(0);

	m_progressRateInfo.SetRange(0,100);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::ClearDisplayInfo()
{
	m_strProblemCameraNum.Empty();
	m_strAllCameraNum.Empty();
	m_strCameraOnlineRate.Empty();
	m_strOffStationNum.Empty();
	m_strAllStationNum.Empty();
	m_strStationOnlineRate.Empty();

	m_strProblemCameraNum_500KV.Empty();
	m_strAllCameraNum_500KV.Empty();
	m_strCameraOnlineRate_500KV.Empty();
	m_strOffStationNum_500KV.Empty();
	m_strAllStationNum_500KV.Empty();
	m_strStationOnlineRate_500KV.Empty();

	m_strProblemCameraNum_220KV.Empty();
	m_strAllCameraNum_220KV.Empty();
	m_strCameraOnlineRate_220KV.Empty();
	m_strOffStationNum_220KV.Empty();
	m_strAllStationNum_220KV.Empty();
	m_strStationOnlineRate_220KV.Empty();

	m_strProblemCameraNum_110KV.Empty();
	m_strAllCameraNum_110KV.Empty();
	m_strCameraOnlineRate_110KV.Empty();
	m_strOffStationNum_110KV.Empty();
	m_strAllStationNum_110KV.Empty();
	m_strStationOnlineRate_110KV.Empty();

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::OnEraseBkgnd(CDC* pDC)
{
	BITMAP   bm;
	CDC   dcMem;
	CBitmap*   pOldBitmap = NULL;

	CRect   rect;
	GetWindowRect(&rect);

	m_Bmp.GetObject(sizeof(BITMAP),(LPVOID)&bm);

	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(&m_Bmp);

	pDC->StretchBlt(
		0,
		0,
		rect.Width(),
		rect.Height(),
		&dcMem,
		0,
		0,
		bm.bmWidth,
		bm.bmHeight,
		SRCCOPY);

	dcMem.SelectObject(pOldBitmap);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::SetAndShowOnlineInfo(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum)
{
	m_strAllCameraNum.Format("%d",nAllCameraNum);
	m_strProblemCameraNum.Format("%d",nProblemCameraNum);
	m_strAllStationNum.Format("%d",nAllStationNum);
	m_strOffStationNum.Format("%d",nOffStationNum);

	int nCameraOnlineRate = 0;
	int nStationOnlineRate = 0;

	if (nAllCameraNum > 0)
	{
		nCameraOnlineRate = (float)((nAllCameraNum-nProblemCameraNum)*100)/nAllCameraNum+0.5f;
	}

	if (nAllStationNum > 0)
	{
		nStationOnlineRate = (float)((nAllStationNum-nOffStationNum)*100)/nAllStationNum+0.5f;
	}

	if (nCameraOnlineRate < 0)
	{
		nCameraOnlineRate = 0;
	}
	else if (nCameraOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	if (nStationOnlineRate < 0)
	{
		nStationOnlineRate = 0;
	}
	else if (nStationOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	m_strCameraOnlineRate.Format("%d%%",nCameraOnlineRate);
	m_strStationOnlineRate.Format("%d%%",nStationOnlineRate);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::SetAndShowOnlineInfo2(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum)
{
	m_strAllCameraNum2.Format("%d",nAllCameraNum);
	m_strProblemCameraNum2.Format("%d",nProblemCameraNum);
	m_strAllStationNum2.Format("%d",nAllStationNum);
	m_strOffStationNum2.Format("%d",nOffStationNum);

	int nCameraOnlineRate = 0;
	int nStationOnlineRate = 0;

	if (nAllCameraNum > 0)
	{
		nCameraOnlineRate = (float)((nAllCameraNum-nProblemCameraNum)*100)/nAllCameraNum+0.5f;
	}

	if (nAllStationNum > 0)
	{
		nStationOnlineRate = (float)((nAllStationNum-nOffStationNum)*100)/nAllStationNum+0.5f;
	}

	if (nCameraOnlineRate < 0)
	{
		nCameraOnlineRate = 0;
	}
	else if (nCameraOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	if (nStationOnlineRate < 0)
	{
		nStationOnlineRate = 0;
	}
	else if (nStationOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	m_strCameraOnlineRate2.Format("%d%%",nCameraOnlineRate);
	m_strStationOnlineRate2.Format("%d%%",nStationOnlineRate);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::SetAndShowOnlineInfo500KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum)
{
	m_strAllCameraNum_500KV.Format("%d",nAllCameraNum);
	m_strProblemCameraNum_500KV.Format("%d",nProblemCameraNum);
	m_strAllStationNum_500KV.Format("%d",nAllStationNum);
	m_strOffStationNum_500KV.Format("%d",nOffStationNum);

	int nCameraOnlineRate = 0;
	int nStationOnlineRate = 0;

	if (nAllCameraNum > 0)
	{
		nCameraOnlineRate = (float)((nAllCameraNum-nProblemCameraNum)*100)/nAllCameraNum+0.5f;
	}

	if (nAllStationNum > 0)
	{
		nStationOnlineRate = (float)((nAllStationNum-nOffStationNum)*100)/nAllStationNum+0.5f;
	}

	if (nCameraOnlineRate < 0)
	{
		nCameraOnlineRate = 0;
	}
	else if (nCameraOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	if (nStationOnlineRate < 0)
	{
		nStationOnlineRate = 0;
	}
	else if (nStationOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	m_strCameraOnlineRate_500KV.Format("%d%%",nCameraOnlineRate);
	m_strStationOnlineRate_500KV.Format("%d%%",nStationOnlineRate);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::SetAndShowOnlineInfo220KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum)
{
	m_strAllCameraNum_220KV.Format("%d",nAllCameraNum);
	m_strProblemCameraNum_220KV.Format("%d",nProblemCameraNum);
	m_strAllStationNum_220KV.Format("%d",nAllStationNum);
	m_strOffStationNum_220KV.Format("%d",nOffStationNum);

	int nCameraOnlineRate = 0;
	int nStationOnlineRate = 0;

	if (nAllCameraNum > 0)
	{
		nCameraOnlineRate = (float)((nAllCameraNum-nProblemCameraNum)*100)/nAllCameraNum+0.5f;
	}

	if (nAllStationNum > 0)
	{
		nStationOnlineRate = (float)((nAllStationNum-nOffStationNum)*100)/nAllStationNum+0.5f;
	}

	if (nCameraOnlineRate < 0)
	{
		nCameraOnlineRate = 0;
	}
	else if (nCameraOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	if (nStationOnlineRate < 0)
	{
		nStationOnlineRate = 0;
	}
	else if (nStationOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	m_strCameraOnlineRate_220KV.Format("%d%%",nCameraOnlineRate);
	m_strStationOnlineRate_220KV.Format("%d%%",nStationOnlineRate);

	return TRUE;
}

BOOL CDlgDeviceOnlineInfo::SetAndShowOnlineInfo110KV(int nAllCameraNum,int nProblemCameraNum,int nAllStationNum,int nOffStationNum)
{
	m_strAllCameraNum_110KV.Format("%d",nAllCameraNum);
	m_strProblemCameraNum_110KV.Format("%d",nProblemCameraNum);
	m_strAllStationNum_110KV.Format("%d",nAllStationNum);
	m_strOffStationNum_110KV.Format("%d",nOffStationNum);

	int nCameraOnlineRate = 0;
	int nStationOnlineRate = 0;

	if (nAllCameraNum > 0)
	{
		nCameraOnlineRate = (float)((nAllCameraNum-nProblemCameraNum)*100)/nAllCameraNum+0.5f;
	}

	if (nAllStationNum > 0)
	{
		nStationOnlineRate = (float)((nAllStationNum-nOffStationNum)*100)/nAllStationNum+0.5f;
	}

	if (nCameraOnlineRate < 0)
	{
		nCameraOnlineRate = 0;
	}
	else if (nCameraOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	if (nStationOnlineRate < 0)
	{
		nStationOnlineRate = 0;
	}
	else if (nStationOnlineRate > 100)
	{
		nStationOnlineRate = 100;
	}

	m_strCameraOnlineRate_110KV.Format("%d%%",nCameraOnlineRate);
	m_strStationOnlineRate_110KV.Format("%d%%",nStationOnlineRate);

	return TRUE;
}

LRESULT CDlgDeviceOnlineInfo::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}

void CDlgDeviceOnlineInfo::OnCbnSelchangeComboType()
{
	int nIndex = m_comboType.GetCurSel();
	if (nIndex == CB_ERR)
		return;

	char szType[64] = {0};

	int nLen = m_comboType.GetLBTextLen(nIndex);
	if (nLen >= sizeof(szType))
		return;

	nLen = m_comboType.GetLBText(nIndex,szType);
	if (nLen == CB_ERR)
		return;

	if (g_pMainDlg == NULL||g_pMainDlg->m_pDlgPageViewInfo == NULL)
		return;

	g_pMainDlg->m_pDlgPageViewInfo->StopDeviceOnlineRateReportInfo();

	ClearDisplayInfo();

	g_pMainDlg->m_pDlgPageViewInfo->StartDeviceOnlineRateReportInfo(szType);
}

LRESULT CDlgDeviceOnlineInfo::OnDeviceOnlineRateInfoUpdateMessageHandler(WPARAM wParam, LPARAM lParam)
{
	if (lParam != 9999)
		return 1;

	try
	{
		int nPos = (int)wParam;

		if (nPos > 0&&nPos < 100)
		{
			m_progressRateInfo.SetPos(nPos);
		}

		if (nPos == 0||nPos == 100)
		{
			m_progressRateInfo.SetPos(nPos);
			m_progressRateInfo.Invalidate(TRUE);
		}

		if(nPos > 100)
		{
			m_progressRateInfo.SetPos(100);
			m_progressRateInfo.Invalidate(TRUE);
			UpdateData(FALSE);
		}
		
		return 0;
	}
	catch(...)
	{

	}
	return 1;
}
