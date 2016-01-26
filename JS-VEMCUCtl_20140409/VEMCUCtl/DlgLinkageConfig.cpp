// DlgLinkageConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgLinkageConfig.h"
#include "AssDBOperate.h"


// CDlgLinkageConfig 对话框

IMPLEMENT_DYNAMIC(CDlgLinkageConfig, CDialog)

CDlgLinkageConfig::CDlgLinkageConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkageConfig::IDD, pParent)
{

}

CDlgLinkageConfig::~CDlgLinkageConfig()
{
}

void CDlgLinkageConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINKAGE_SHOW_TYPE, m_comboLinkageShowType);
	DDX_Control(pDX, IDC_COMBO_SHOW_VIDEO, m_comboShowVideo);
	DDX_Control(pDX, IDC_COMBO_CHANGE_TIME, m_comboChangeTime);
	DDX_Control(pDX, IDC_CHECK_SECURITY, m_checkSecurity);
	DDX_Control(pDX, IDC_CHECK_FIRE, m_checkFire);
	DDX_Control(pDX, IDC_CHECK_DOOR_ALARM, m_checkDoorAlarm);
	DDX_Control(pDX, IDC_CHECK_DOOR_ACTION, m_checkDoorAction);
}


BEGIN_MESSAGE_MAP(CDlgLinkageConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLinkageConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLinkageConfig 消息处理程序

void CDlgLinkageConfig::OnBnClickedOk()
{
	//获取控件信息
	T_ASS_LINKAGE_CONFIG tAssLinkageConfig;
	SetControlInfoToLinkageConfig(&tAssLinkageConfig);

	//入库
	InsertLinkageConfigInfoToDB(&tAssLinkageConfig);

	OnOK();
}

void CDlgLinkageConfig::SetControlInfoToLinkageConfig(T_ASS_LINKAGE_CONFIG* pAssLinkageConfig)
{
	//获取控件的值
	CString strShowType = "";
	m_comboLinkageShowType.GetLBText(m_comboLinkageShowType.GetCurSel(),strShowType);

	CString strShowVideo = "";
	m_comboShowVideo.GetLBText(m_comboShowVideo.GetCurSel(),strShowVideo);

	CString strChangeTime = "";
	m_comboChangeTime.GetLBText(m_comboChangeTime.GetCurSel(),strChangeTime);

	int nSecurity = m_checkSecurity.GetCheck();
	int nFire = m_checkFire.GetCheck();
	int nDoorAlarm = m_checkDoorAlarm.GetCheck();
	int nDoorAction = m_checkDoorAction.GetCheck();

	pAssLinkageConfig->nUserId = g_userpower.userID;
	pAssLinkageConfig->nShowType = GetShowTypeByShowTypeText(strShowType.GetBuffer());
	pAssLinkageConfig->nShowVideo = GetShowVideoByShowVideoTetxt(strShowVideo.GetBuffer());
	pAssLinkageConfig->nChangeTime = atoi(strChangeTime);
	pAssLinkageConfig->nSecurity = nSecurity;
	pAssLinkageConfig->nFire = nFire;
	pAssLinkageConfig->nDoorAlarm = nDoorAlarm;
	pAssLinkageConfig->nDoorAction = nDoorAction;
}

int CDlgLinkageConfig::GetShowVideoByShowVideoTetxt(char *pShowVideoText)
{
	if (strcmp(pShowVideoText,"否") == 0)
	{
		return 0;
	}
	if (strcmp(pShowVideoText,"是") == 0)
	{
		return 1;
	}
}

int CDlgLinkageConfig::GetShowTypeByShowTypeText(char *pShowTypeText)
{
	if (strcmp(pShowTypeText,"多画面展示") == 0)
	{
		return 0;
	}
	if (strcmp(pShowTypeText,"电子地图展示") == 0)
	{
		return 1;
	}
}

BOOL CDlgLinkageConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化展示方式
	InitComboShowType();

	//初始化是否显示视频
	InitComboShowVideo();

	//初始化切换时间
	InitComboChangeTime();

	//初始化数据源，默认为全部勾选
	InitAllCheckBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLinkageConfig::InitComboShowType()
{
	m_comboLinkageShowType.ResetContent();
	m_comboLinkageShowType.AddString("多画面展示");
	m_comboLinkageShowType.AddString("电子地图展示");
	m_comboLinkageShowType.SetCurSel(0);
}

void CDlgLinkageConfig::InitComboShowVideo()
{
	m_comboShowVideo.ResetContent();
	m_comboShowVideo.AddString("是");
	m_comboShowVideo.AddString("否");
	m_comboShowVideo.SetCurSel(0);
}

void CDlgLinkageConfig::InitComboChangeTime()
{
	m_comboChangeTime.ResetContent();
	for (int i=5;i<=60;i+=5)
	{
		CString strText = "";
		strText.Format("%d",i);
		m_comboChangeTime.AddString(strText);
	}
	m_comboChangeTime.SetCurSel(0);
}

void CDlgLinkageConfig::InitAllCheckBox()
{
	m_checkSecurity.SetCheck(TRUE);
	m_checkFire.SetCheck(TRUE);
	m_checkDoorAlarm.SetCheck(TRUE);
	m_checkDoorAction.SetCheck(TRUE);
}
