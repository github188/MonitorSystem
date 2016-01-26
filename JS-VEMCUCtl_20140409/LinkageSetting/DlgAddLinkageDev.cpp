// DlgAddLinkageDev.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgAddLinkageDev.h"
#include "DBExcute.h"


// CDlgAddLinkageDev 对话框


T_DOOR_INFO g_tDoorInfo[1024] = {0};
int g_nDoorInfoCount = 0;

T_CAMERA_INFO g_CameraInfo[1024] = {0};
int g_nCameraInfoCount = 0;

IMPLEMENT_DYNAMIC(CDlgAddLinkageDev, CDialog)

CDlgAddLinkageDev::CDlgAddLinkageDev(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddLinkageDev::IDD, pParent)
{

}

CDlgAddLinkageDev::CDlgAddLinkageDev(CWnd* pParent, _T_DEV_INFO_ *pDevInfo)
	: CDialog(CDlgAddLinkageDev::IDD, pParent)
{
	m_pDevInfo = pDevInfo;
}

CDlgAddLinkageDev::~CDlgAddLinkageDev()
{
	memset(&g_tDoorInfo,0,sizeof(T_DOOR_INFO)*1024);
	memset(&g_CameraInfo,0,sizeof(T_CAMERA_INFO)*1024);
}

void CDlgAddLinkageDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINKAGE_TYPE, m_comboLinkageType);
	DDX_Control(pDX, IDC_COMBO_LINKAGE_DEV_NAME, m_comboLinkageDevName);
	DDX_Control(pDX, IDC_COMBO_LINKAGE_ACTION, m_comboLinkageAction);
	DDX_Control(pDX, IDC_COMBO_PRESET_NUM, m_comboPresetNum);
}


BEGIN_MESSAGE_MAP(CDlgAddLinkageDev, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKAGE_TYPE, &CDlgAddLinkageDev::OnCbnSelchangeComboLinkageType)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKAGE_ACTION, &CDlgAddLinkageDev::OnCbnSelchangeComboLinkageAction)
	ON_BN_CLICKED(IDOK, &CDlgAddLinkageDev::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKAGE_DEV_NAME, &CDlgAddLinkageDev::OnCbnSelchangeComboLinkageDevName)
END_MESSAGE_MAP()


// CDlgAddLinkageDev 消息处理程序

BOOL CDlgAddLinkageDev::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCameraInfo(m_pDevInfo->nStationId);

	InitDoorInfo(m_pDevInfo->nStationId);

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAddLinkageDev::InitDoorInfo(int nStationId)
{
	//取出该变电站的门禁设备
}

void CDlgAddLinkageDev::InitCameraInfo(int nStationId)
{
	char sql_buf[1024] = {0};
	MYSQL_RES *res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"SELECT a.camera_id,a.name FROM video_camera AS a WHERE "  
		" a.dvr_id in (select dvr_id from video_dvr where station_group in (select station_code_videoplant from ob_d5000_station where station_id=%d))", m_pDevInfo->nStationId);

	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			sprintf_s(g_CameraInfo[g_nCameraInfoCount].szCameraCode,"%s",row[0]);
			sprintf_s(g_CameraInfo[g_nCameraInfoCount].szCameraName,"%s",row[1]);
			g_nCameraInfoCount++;
		}
	}
}

void CDlgAddLinkageDev::InitCtrl()
{
	m_comboPresetNum.EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_DEV_NAME)->SetWindowText(m_pDevInfo->szDevName);

	InitComboLinkageType();

	char szLinkageTypeText[32] = {0};
	sprintf_s(szLinkageTypeText,"摄像头");
	InitComboLinkageDev(szLinkageTypeText);

	InitComboLinkageAction(szLinkageTypeText);
}

void CDlgAddLinkageDev::InitComboLinkageType()
{
	m_comboLinkageType.ResetContent();
	m_comboLinkageType.AddString("摄像头");
	//m_comboLinkageType.AddString("门禁");
	m_comboLinkageType.SetCurSel(0);
}

void CDlgAddLinkageDev::InitComboLinkageDev(char *szLinkageTypeText)
{
	m_comboLinkageDevName.ResetContent();

	if (strcmp(szLinkageTypeText,"摄像头") == 0)
	{
		for (int i=0;i<g_nCameraInfoCount;i++)
		{
			m_comboLinkageDevName.AddString(g_CameraInfo[i].szCameraName);
		}
		m_comboLinkageDevName.SetCurSel(0);
	}
	else if (strcmp(szLinkageTypeText,"门禁") == 0)
	{
	}
}

void CDlgAddLinkageDev::OnCbnSelchangeComboLinkageType()
{
	int nIndex = m_comboLinkageType.GetCurSel();
	CString strText;
	m_comboLinkageType.GetLBText(nIndex,strText);

	InitComboLinkageDev(strText.GetBuffer());

	InitComboLinkageAction(strText.GetBuffer());
}

void CDlgAddLinkageDev::InitComboLinkageAction(char *szLinkageTypeText)
{
	m_comboLinkageAction.ResetContent();

	if (strcmp(szLinkageTypeText,"摄像头") == 0)
	{

		//m_comboLinkageAction.AddString("打开");
		//m_comboLinkageAction.AddString("关闭");
		m_comboLinkageAction.AddString("转预置位");

		m_comboLinkageAction.SetCurSel(0);
	}
	else if (strcmp(szLinkageTypeText,"门禁") == 0)
	{
		m_comboLinkageAction.AddString("打开");
		m_comboLinkageAction.AddString("关闭");

		m_comboLinkageAction.SetCurSel(0);
	}
}

void CDlgAddLinkageDev::OnCbnSelchangeComboLinkageAction()
{
	int nIndex = m_comboLinkageAction.GetCurSel();
	CString strText;
	m_comboLinkageAction.GetLBText(nIndex,strText);

	if (strText == "转预置位")
	{
		int nIndex1 = m_comboLinkageDevName.GetCurSel();
		InitComboPresetNum(nIndex1);
		m_comboPresetNum.EnableWindow(TRUE);
	}
	else
	{
		//m_comboPresetNum.EnableWindow(FALSE);
	}
}

void CDlgAddLinkageDev::InitComboPresetNum(int nIndex)
{
	m_comboPresetNum.ResetContent();
	char sql_buf[1024] = {0};
	MYSQL_RES *res;
	MYSQL_ROW row;

	sprintf_s(sql_buf,"SELECT camera_preset_id	FROM ass_camerapreset WHERE camera_code='%s'", g_CameraInfo[nIndex].szCameraCode);

	if (!mysql_query(g_LinkageSettingMysql,sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		while (row = mysql_fetch_row(res))
		{
			m_comboPresetNum.AddString(row[0]);
		}
	}

	m_comboPresetNum.SetCurSel(0);
}

void CDlgAddLinkageDev::OnBnClickedOk()
{
	//判断输入是否有效
	if (!CheckCondition())
		return;
	
	//拼出表单记录
	_T_LINK_CONFIG_INFO_ tLinkConfigInfo;
	memset(&tLinkConfigInfo,0,sizeof(_T_LINK_CONFIG_INFO_));
	SpellFormInfo(&tLinkConfigInfo);

	//判断是否已经入过库
	CString strCondition = "";
	strCondition.Format("f_dev_id=%d and f_relate_camera_code='%s'", tLinkConfigInfo.nDevId, tLinkConfigInfo.szRelateCameraCode);
	if (CheckExists("ass_linkage_config",strCondition.GetBuffer()))
	{
		//入库
		InsertLinkageInfoToDB(&tLinkConfigInfo);
	}
	else
	{
		MessageBox("已设置过该设备！");
	}

	OnOK();
}

bool CDlgAddLinkageDev::InsertLinkageInfoToDB(_T_LINK_CONFIG_INFO_ *pLinkConfigInfo)
{
	char sql_buf[1024] = {0};
	//拼sql语句
	sprintf_s(sql_buf, "insert t_ass_linkage_relation(f_dev_id,f_relate_dev_id,f_relate_camera_code,f_relate_dev_type,f_relate_dev_action,f_preset_num) "
		" values(%d,%d,'%s',%d,%d,%d)",
		pLinkConfigInfo->nDevId,
		pLinkConfigInfo->nRelateDevId,
		pLinkConfigInfo->szRelateCameraCode,
		pLinkConfigInfo->nRelateDevType,
		pLinkConfigInfo->nRelateDevAction,
		pLinkConfigInfo->nPresetNum);

	if (mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		return false;
	}

	return true;
}

void CDlgAddLinkageDev::SpellFormInfo(_T_LINK_CONFIG_INFO_ *pLinkConfigInfo)
{
	//设备id
	pLinkConfigInfo->nDevId = m_pDevInfo->nDevId;
	
	//联动类型
	CString strText = "";
	int nIndex = m_comboLinkageType.GetCurSel();
	m_comboLinkageType.GetLBText(nIndex,strText);
	if (strText == "摄像头")
	{
		pLinkConfigInfo->nRelateDevType = 2;
	}
	if (strText == "门禁")
	{
		pLinkConfigInfo->nRelateDevType = 1;
	}

	//联动设备
	if (pLinkConfigInfo->nRelateDevType == 1)
	{
		int nIndex1 = m_comboLinkageDevName.GetCurSel();
		pLinkConfigInfo->nRelateDevId = g_tDoorInfo[nIndex1].nDevId;
		sprintf_s(pLinkConfigInfo->szRelateCameraCode,"");
	}
	//联动摄像头
	if (pLinkConfigInfo->nRelateDevType == 2)
	{
		int nIndex1 = m_comboLinkageDevName.GetCurSel();
		sprintf_s(pLinkConfigInfo->szRelateCameraCode,"%s",g_CameraInfo[nIndex1].szCameraCode);
		pLinkConfigInfo->nRelateDevId = 0;
	}

	//联动动作
	int nIndex2 = m_comboLinkageAction.GetCurSel();
	m_comboLinkageAction.GetLBText(nIndex2,strText);
	if (strText == "打开")
	{
		pLinkConfigInfo->nRelateDevAction = 1;
	}
	if (strText == "关闭")
	{
		pLinkConfigInfo->nRelateDevAction = 2;
	}
	if (strText == "转预置位")
	{
		pLinkConfigInfo->nRelateDevAction = 3;
	}

	//预置位编号
	int nIndex3 = m_comboPresetNum.GetCurSel();
	m_comboPresetNum.GetLBText(nIndex3,strText);
	pLinkConfigInfo->nPresetNum = atoi(strText.GetBuffer());
}

bool CDlgAddLinkageDev::CheckCondition()
{
	int nIndex = m_comboLinkageDevName.GetCurSel();
	CString strText="";
	m_comboLinkageDevName.GetLBText(nIndex,strText);

	if (strText == "")
	{
		MessageBox("请选择联动设备！");
		return false;
	}

	CString strLinkageTypeText = "";
	m_comboLinkageType.GetWindowText(strLinkageTypeText);
	if (strLinkageTypeText == "")
	{
		MessageBox("请选择联动类型！");
		return false;
	}

	CString strLinkageActionText = "";
	m_comboLinkageAction.GetWindowText(strLinkageActionText);
	if (strLinkageActionText == "")
	{
		MessageBox("请选择联动动作！");
		return false;
	}

	CString strPresetNumText = "";
	m_comboPresetNum.GetWindowText(strPresetNumText);
	if (strPresetNumText == "")
	{
		MessageBox("请选择预置位编号！");
		return false;
	}

	return true;
}
void CDlgAddLinkageDev::OnCbnSelchangeComboLinkageDevName()
{
	int nIndex1 = m_comboLinkageDevName.GetCurSel();
	InitComboPresetNum(nIndex1);
}
