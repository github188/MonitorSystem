// DlgDeviceRelationCamera.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgDeviceRelationCamera.h"


// CDlgDeviceRelationCamera 对话框

IMPLEMENT_DYNAMIC(CDlgDeviceRelationCamera, CDialog)

CDlgDeviceRelationCamera::CDlgDeviceRelationCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceRelationCamera::IDD, pParent)
{

}

CDlgDeviceRelationCamera::CDlgDeviceRelationCamera(int nDevId, CWnd* pParent /*=NULL*/)
: CDialog(CDlgDeviceRelationCamera::IDD, pParent)
{
	m_nDevId = nDevId;
	memset(m_szCameraName, 0, 64);
}

CDlgDeviceRelationCamera::~CDlgDeviceRelationCamera()
{
}

void CDlgDeviceRelationCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERA, m_comboCamera);
}


BEGIN_MESSAGE_MAP(CDlgDeviceRelationCamera, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDeviceRelationCamera::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDeviceRelationCamera 消息处理程序

BOOL CDlgDeviceRelationCamera::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetRelationCameraInfoByDevId(m_nDevId);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDeviceRelationCamera::GetRelationCameraInfoByDevId(int nDevId)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql_buf[1024] = {0};
	sprintf_s(sql_buf, "SELECT b.`name` FROM ass_camerapreset AS a INNER JOIN  "
		" video_camera AS b ON a.camera_code = b.camera_id "
		" WHERE a.dev_id=%d ", nDevId);

	if (!mysql_query(g_mySqlData, sql_buf))
	{
		res = mysql_store_result(g_mySqlData);

		while ( row = mysql_fetch_row( res ) )
		{
			m_comboCamera.AddString(row[0]);
		}
		mysql_free_result(res) ;
	}
	m_comboCamera.SetCurSel(0);
}

void CDlgDeviceRelationCamera::OnBnClickedOk()
{
	m_comboCamera.GetLBText(m_comboCamera.GetCurSel(), m_szCameraName);
	m_comboCamera.GetLBText(m_comboCamera.GetCurSel(), m_strCameraName);

	OnOK();
}