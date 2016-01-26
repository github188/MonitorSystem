// DlgCameraStatus.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgCameraStatus.h"


// CDlgCameraStatus 对话框

IMPLEMENT_DYNAMIC(CDlgCameraStatus, CDialog)

CDlgCameraStatus::CDlgCameraStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraStatus::IDD, pParent)
{
	
}

CDlgCameraStatus::CDlgCameraStatus( char *szCameraCode, CWnd* pParent /*= NULL*/ )
	: CDialog(CDlgCameraStatus::IDD, pParent)
{
	memcpy(m_szCameraCode,szCameraCode,20);
	m_pDiagnosisInfo = NULL;
	m_strDiagnosisText1 = "";
	m_strDiagnosisText2 = "";
}

CDlgCameraStatus::~CDlgCameraStatus()
{
	if (m_pDiagnosisInfo!=NULL)
	{
		delete m_pDiagnosisInfo;
	}
}

void CDlgCameraStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CAMERA_STATUS_VALUE, m_strCameraStatusValue);
}


BEGIN_MESSAGE_MAP(CDlgCameraStatus, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgCameraStatus::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgCameraStatus 消息处理程序

void CDlgCameraStatus::OnBnClickedButton2()
{
	OnOK();
}

BOOL CDlgCameraStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	//根据CameraCode获取故障信息
	m_pDiagnosisInfo = GetDiagnosisInfoByCameraCode(m_szCameraCode);
	
	//根据故障信息拼出字符串
	if (m_pDiagnosisInfo == NULL)
	{	
		m_strDiagnosisText1.Format("%s","没查到该摄像头的故障诊断信息");
	}
	else
	{
		SpellDiagnosisText(m_pDiagnosisInfo, m_strDiagnosisText1, m_strDiagnosisText2);
	}

	GetDlgItem(IDC_CAMERA_STATUS_VALUE)->SetWindowText(m_strDiagnosisText1);
	GetDlgItem(IDC_STATIC_DIAGNOSIS_INFO)->SetWindowText(m_strDiagnosisText2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCameraStatus::SpellDiagnosisText(_T_DIAGNOSIS_INFO* pDiagnosisInfo, CString &strDiagnosisText1, CString &strDiagnosisText2)
{
	//无故障
	if (pDiagnosisInfo->nTotalResult == 0)
	{
		strDiagnosisText1 += "无故障";
	}
	//有故障
	if (pDiagnosisInfo->nTotalResult == 1)
	{
		strDiagnosisText1 += "有故障";
		if (pDiagnosisInfo->nLoginErr == 1)
		{
			strDiagnosisText2 += "登陆失败,";
		}
		if (pDiagnosisInfo->nOpenVideoErr == 1)
		{
			strDiagnosisText2 += "打开视频失败,";
		}
		if (pDiagnosisInfo->nRecordErr == 1)
		{
			strDiagnosisText2 += "录像失败,";
		}
		if (pDiagnosisInfo->nLose == 1)
		{
			strDiagnosisText2 += "视频丢失,";
		}
		if (pDiagnosisInfo->nImpluse == 1)
		{
			strDiagnosisText2 += "有噪点,";
		}
		if (pDiagnosisInfo->nStripe == 1)
		{
			strDiagnosisText2 += "条纹干扰,";
		}
		if (pDiagnosisInfo->nBright == 1)
		{
			strDiagnosisText2 += "亮度异常,";
		}
		if (pDiagnosisInfo->nVague == 1)
		{
			strDiagnosisText2 += "视频模糊,";
		}
		if (pDiagnosisInfo->nColor == 1)
		{
			strDiagnosisText2 += "视频偏色,";
		}
		if (pDiagnosisInfo->nPTZ == 1)
		{
			strDiagnosisText2 += "云台失控,";
		}
		if (pDiagnosisInfo->nShake == 1)
		{
			strDiagnosisText2 += "抖动,";
		}
	}
}


_T_DIAGNOSIS_INFO* CDlgCameraStatus::GetDiagnosisInfoByCameraCode(char *szCameraCode)
{
	char sql_buf[1024] = {0};
	MYSQL_RES * res = NULL;
	MYSQL_ROW	row ;
    _T_DIAGNOSIS_INFO *pDiagnosisInfo = NULL; 

	// 读取当前视频平台数量及信息
	sprintf_s(sql_buf, "SELECT a.CameraCode,a.TotalResult,a.ResultOfLoginErr,a.ResultOfOpenVideoErr,a.ResultOfRecordErr, "
		" a.ResultOfLose,a.ResultOfImpluse,a.ResultOfStripe,a.ResultOfBright,a.ResultOfVague,a.ResultOfColor, "
	    " a.ResultOfPTZ,a.ResultOfShake FROM vaichannlscheckresults AS a "
	    " where a.CameraCode='%s'", szCameraCode);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while ( row = mysql_fetch_row( res ) )
		{
			pDiagnosisInfo = new _T_DIAGNOSIS_INFO;
			pDiagnosisInfo->nTotalResult = atoi(row[1]);
			pDiagnosisInfo->nLoginErr = atoi(row[2]);
			pDiagnosisInfo->nOpenVideoErr = atoi(row[3]);
			pDiagnosisInfo->nRecordErr = atoi(row[4]);
			pDiagnosisInfo->nLose = atoi(row[5]);
			pDiagnosisInfo->nImpluse = atoi(row[6]);
			pDiagnosisInfo->nStripe = atoi(row[7]);
			pDiagnosisInfo->nBright = atoi(row[8]);
			pDiagnosisInfo->nVague = atoi(row[9]);
			pDiagnosisInfo->nColor = atoi(row[10]);
			pDiagnosisInfo->nPTZ = atoi(row[11]);
			pDiagnosisInfo->nShake = atoi(row[12]);
		}
		mysql_free_result(res) ;
	}

	return pDiagnosisInfo;
}
