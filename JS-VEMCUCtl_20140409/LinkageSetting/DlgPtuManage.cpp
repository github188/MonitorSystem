// DlgPtuManage.cpp : 实现文件
//

#include "stdafx.h"
#include "LinkageSetting.h"
#include "DlgPtuManage.h"
#include "PtuDlg.h"
#include "DBExcute.h"
#include "RVUServerParse.h"


// CDlgPtuManage 对话框
HANDLE g_hGetDeviceEvent = NULL;

IMPLEMENT_DYNAMIC(CDlgPtuManage, CDialog)

CDlgPtuManage::CDlgPtuManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPtuManage::IDD, pParent)
{
	m_nRvuId = 0;
	m_nStationId = 0;
	m_pStationNode = NULL;
	m_pBackBmp = NULL;
}

CDlgPtuManage::~CDlgPtuManage()
{
}

void CDlgPtuManage::SetRvuId(int nRvuId)
{
	m_nRvuId = nRvuId;
}

void CDlgPtuManage::GetGuid(char* szGuid)
{
	GUID guid;
	CoCreateGuid(&guid);
	sprintf_s(
		szGuid,
		64,
		"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return ;
}

void CDlgPtuManage::SetStationNode(StationNode* pStationNode)
{
	m_pStationNode = pStationNode;
}

void CDlgPtuManage::SetStationId(int nStationId)
{
	m_nStationId = nStationId;
}

void CDlgPtuManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PTU_IP, m_ctrPtuIP);
	DDX_Control(pDX, IDC_EDIT_PTU_PORT, m_ctrPtuPort);
	DDX_Control(pDX, IDC_EDIT_PTU_USERNAME, m_ctrPtuUserName);
	DDX_Control(pDX, IDC_EDIT_PTU_PASSWORD, m_ctrPtuPassword);
	DDX_Control(pDX, IDC_STATIC_PTU_STATUS_VALUE, m_ctrStaticPtuStationValue);
	DDX_Control(pDX, IDC_TREE_RVU_DEVICE_LIST, m_treeRvuDeviceList);
	DDX_Control(pDX, IDC_TREE_ROUTE_RVU_DEVICE_LIST, m_treeRouteRvuDeviceList);
	DDX_Control(pDX, IDC_EDIT_PTU_NAME, m_ctrPtuName);
	DDX_Control(pDX, IDC_TREE_SUB_SYSTEM_LIST, m_treeSubSystemList);
	DDX_Control(pDX, IDC_COMBO_RVU_TYPE, m_comboRvuType);
}


BEGIN_MESSAGE_MAP(CDlgPtuManage, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_INFO, &CDlgPtuManage::OnBnClickedButtonChangeInfo)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_TEST, &CDlgPtuManage::OnBnClickedButtonConnectTest)
	ON_BN_CLICKED(IDC_BUTTON_GET_DEVICE_INFO, &CDlgPtuManage::OnBnClickedButtonGetDeviceInfo)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TO_LOCAL_DATABASE, &CDlgPtuManage::OnBnClickedButtonAddToLocalDatabase)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DEL_RVU, &CDlgPtuManage::OnBnClickedButtonDelRvu)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_RVU, &CDlgPtuManage::OnBnClickedButtonEditRvu)
END_MESSAGE_MAP()


// CDlgPtuManage 消息处理程序

BOOL CDlgPtuManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	g_hGetDeviceEvent = CreateEvent(NULL,TRUE,NULL,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPtuManage::InitControl()
{
	m_pBackBmp = new CBitmap;
	m_pBackBmp->LoadBitmap(IDB_BITMAP_BACK);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_TREE);
	m_ilTreeIcons.Create(16,16,ILC_MASK|ILC_COLOR24,0,1);
	m_ilTreeIcons.Add(&bmp,RGB(255,0,255));

	m_treeRvuDeviceList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);
	m_treeRouteRvuDeviceList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);
	m_treeSubSystemList.SetImageList(&m_ilTreeIcons,TVSIL_NORMAL);

	//初始化设备类型
	InitComboRvuTypeData();
}

void CDlgPtuManage::InitComboRvuTypeData()
{
	std::vector<T_ASS_RVU_TYPE> vecRvuType;
	vecRvuType = GetRvuTypeFromDB();
	
	AddRvuTypeToCombo(vecRvuType);
}

void CDlgPtuManage::AddRvuTypeToCombo(std::vector<T_ASS_RVU_TYPE> vecRvuType)
{
	for (int i=0; i<vecRvuType.size(); i++)
	{
		m_comboRvuType.AddString(vecRvuType.at(i).szRvuType);
	}
	m_comboRvuType.SetCurSel(0);
}

void CDlgPtuManage::SetEditControlText(CString strPtuIP,CString strPtuPort,CString strPtuUserName,CString strPtuPassword,int connect_ptu,CString strPtuName,CString strRvuType, CString strRvuCode)
{
	m_ctrPtuIP.SetWindowText(strPtuIP);
	m_ctrPtuPort.SetWindowText(strPtuPort);
	m_ctrPtuUserName.SetWindowText(strPtuUserName);
	m_ctrPtuPassword.SetWindowText(strPtuPassword);
	m_ctrPtuName.SetWindowText(strPtuName);
	m_comboRvuType.SetWindowText(strRvuType);
	GetDlgItem(IDC_EDIT_RVU_CODE)->SetWindowText(strRvuCode);
	if (connect_ptu == 0)
	{
		m_ctrStaticPtuStationValue.SetWindowText("未连接");
	}
	if (connect_ptu == 1)
	{
		m_ctrStaticPtuStationValue.SetWindowText("已连接");
	}
}

void CDlgPtuManage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd* pWnd = NULL;
	CRect winrc,static_rect,button_rect,tree_rect;

	int nStaticHeight = 30;
	int nStaticWidth = 200;
	int nButtonHeight = 30;
	int nButtonWidth = 200;
	int nTreeWidth = 300;

	GetClientRect(&winrc);

	//tree_route的文本控件
	pWnd = GetDlgItem(IDC_STATIC_ROUTE_DEVICE);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + 10;
		tree_rect.bottom = tree_rect.top + nStaticHeight;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//tree_route控件
	pWnd = GetDlgItem(IDC_TREE_ROUTE_RVU_DEVICE_LIST);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + nStaticHeight + 10;
		tree_rect.bottom = winrc.bottom - 10;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//添加到本地数据库按钮
	pWnd = GetDlgItem(IDC_BUTTON_ADD_TO_LOCAL_DATABASE);
	if (pWnd != NULL)
	{
		tree_rect.top = (winrc.bottom - winrc.top)/2-100 ;
		tree_rect.bottom = tree_rect.top + nButtonHeight;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20 +nTreeWidth + 10;
		tree_rect.right = tree_rect.left + 30;
		pWnd->MoveWindow(tree_rect);
	}

	//tree文本控件
	pWnd = GetDlgItem(IDC_STATIC_LOCAL_DEVICE);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + 10;
		tree_rect.bottom = tree_rect.top + nStaticHeight;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20;//nStaticWidth + nButtonWidth + 10 + 20 +nTreeWidth + 10 + 30 + 10;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//tree控件
	pWnd = GetDlgItem(IDC_TREE_RVU_DEVICE_LIST);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + nStaticHeight + 10;
		tree_rect.bottom = winrc.bottom - 10;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20;//nStaticWidth + nButtonWidth + 10 + 20 +nTreeWidth+ 10 + 30 + 10;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//sub_system_tree文本控件
	pWnd = GetDlgItem(IDC_STATIC_SUB_SYSTEM);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + 10;
		tree_rect.bottom = tree_rect.top + nStaticHeight;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20 +nTreeWidth + 10 + 30 + 10 + nTreeWidth + 10;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//sub_system_tree控件
	pWnd = GetDlgItem(IDC_TREE_SUB_SYSTEM_LIST);
	if (pWnd != NULL)
	{
		tree_rect.top = winrc.top + nStaticHeight + 10;
		tree_rect.bottom = winrc.bottom - 10;
		tree_rect.left = nStaticWidth + nButtonWidth + 10 + 20 +nTreeWidth+ 10 + 30 + 10 + nTreeWidth + 10;
		tree_rect.right = tree_rect.left + nTreeWidth;
		pWnd->MoveWindow(tree_rect);
	}

	//变电站名称static控件
	pWnd = GetDlgItem(IDC_STATIC_PTU_NAME);
	if (pWnd != NULL)
	{
		static_rect.top = winrc.top+10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

    //变电站名称edit控件
	pWnd = GetDlgItem(IDC_EDIT_PTU_NAME);
	if (pWnd != NULL)
	{
		button_rect.top = winrc.top+10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//IP的Static控件
	pWnd = GetDlgItem(IDC_STATIC_PTU_IP);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//IP的EDIT控件
	pWnd = GetDlgItem(IDC_EDIT_PTU_IP);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top+nButtonHeight+10;;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//PORT的Static控件
	pWnd = GetDlgItem(IDC_STATIC_PTU_PORT);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//PORT的edit控件
	pWnd = GetDlgItem(IDC_EDIT_PTU_PORT);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//用户名的static控件
	pWnd = GetDlgItem(IDC_STATIC_PTU_USERNAME);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//用户名的edit控件
	pWnd = GetDlgItem(IDC_EDIT_PTU_USERNAME);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//密码
	pWnd = GetDlgItem(IDC_STATIC_PTU_PASSWORD);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//密码
	pWnd = GetDlgItem(IDC_EDIT_PTU_PASSWORD);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//RVU协议类型
	pWnd = GetDlgItem(IDC_STATIC_RVU_TYPE);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//RVU协议类型
	pWnd = GetDlgItem(IDC_COMBO_RVU_TYPE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight + 100;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//RVU的ID，ANFANG协议使用
	pWnd = GetDlgItem(IDC_STATIC_RVU_CODE);
	if (pWnd != NULL)
	{
		static_rect.top = static_rect.top + nStaticHeight + 10;
		static_rect.bottom = static_rect.top + nStaticHeight;
		static_rect.left = winrc.left + 10;
		static_rect.right = static_rect.left + nStaticWidth;
		pWnd->MoveWindow(static_rect);
	}

	//RVU的ID，ANFANG协议使用
	pWnd = GetDlgItem(IDC_EDIT_RVU_CODE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = static_rect.right + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}
	
	//添加/删除RVU按钮
	pWnd = GetDlgItem(IDC_BUTTON_CHANGE_INFO);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	//删除RVU信息按钮
	pWnd = GetDlgItem(IDC_BUTTON_DEL_RVU);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10 + nButtonWidth + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}


	pWnd = GetDlgItem(IDC_BUTTON_CONNECT_TEST);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_BUTTON_GET_DEVICE_INFO);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_PTU_STATUS);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top + nButtonHeight + 10;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = winrc.left + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_PTU_STATUS_VALUE);
	if (pWnd != NULL)
	{
		button_rect.top = button_rect.top;
		button_rect.bottom = button_rect.top + nButtonHeight;
		button_rect.left = button_rect.left + nButtonWidth + 10;
		button_rect.right = button_rect.left + nButtonWidth;
		pWnd->MoveWindow(button_rect);
	}


}

void CDlgPtuManage::OnBnClickedButtonChangeInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CPtuDlg* pParent = (CPtuDlg*)(this->GetParent()->GetParent());
	HTREEITEM hItem = (pParent->m_treeList).GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	StationNode* pStationNode = (StationNode*)pParent->m_treeList.GetItemData(hItem);
	if (pStationNode == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	//node_type为2时表示变电站节点
	if (pStationNode->node_type != 2)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	CString strPtuName = "";
	CString strPtuIP = "";
	CString strPtuPort = "";
	CString strPtuUserName = "";
	CString strPtuPassword = "";
	CString strRvuType = "";
	CString strRvuCode = "";
	m_ctrPtuName.GetWindowText(strPtuName);
	m_ctrPtuIP.GetWindowText(strPtuIP);
	m_ctrPtuPort.GetWindowText(strPtuPort);
	m_ctrPtuUserName.GetWindowText(strPtuUserName);
	m_ctrPtuPassword.GetWindowText(strPtuPassword);
	m_comboRvuType.GetWindowText(strRvuType);
	GetDlgItem(IDC_EDIT_RVU_CODE)->GetWindowText(strRvuCode);

	if(!CheckRvuParams(strPtuIP, strPtuPort, strPtuUserName, strPtuPassword, strRvuType, strRvuCode))
		return;

	char sql_buf[1024]={0x0};
	MYSQL_RES	* res;

	//如果rvu表中存在这个变电站id，则更新为空
	sprintf_s(sql_buf,"UPDATE ass_rvu set station_id='' where station_id=%d",pStationNode->station_id);
	mysql_query(g_LinkageSettingMysql, sql_buf);

	//通过IP,port,rvu_code判断rvu是否添加过，如果添加过，则更新，没有则插入
	int rnum = 0;
	sprintf_s(sql_buf,"select rvu_id from ass_rvu where rvu_ip='%s' and rvu_port='%s' and rvu_code='%s'",strPtuIP.GetBuffer(),strPtuPort.GetBuffer(),strRvuCode.GetBuffer());
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnum = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	//绑定过则更新
	int ret = 0;
	if (rnum > 0)
	{
		sprintf_s(sql_buf,"UPDATE ass_rvu set rvu_name='%s',rvu_username='%s',rvu_password='%s',rvu_protocol='%s',station_id='%d' where rvu_ip='%s' and rvu_port='%s' and rvu_code='%s'",
			strPtuName.GetBuffer(),strPtuUserName.GetBuffer(),strPtuPassword.GetBuffer(),strRvuType.GetBuffer(),pStationNode->station_id,
			strPtuIP.GetBuffer(),strPtuPort.GetBuffer(),strRvuCode.GetBuffer());
		ret = mysql_query(g_LinkageSettingMysql, sql_buf);
	}
	//没绑定过则插入
	else
	{
		CString strText = "";
		strText.Format("确定要将%s变电站绑定到RVU：%s上吗？",pStationNode->station_name,strPtuIP);
		if (MessageBox(strText,"辅助系统",MB_ICONEXCLAMATION|MB_OKCANCEL)==IDCANCEL)
		{
			return;
		}
		sprintf_s(sql_buf,"INSERT INTO ass_rvu (station_id,rvu_ip,rvu_port,rvu_username,rvu_password,rvu_name,rvu_protocol,rvu_code) VALUES (%d,'%s','%s','%s','%s','%s','%s','%s')",
			pStationNode->station_id,strPtuIP.GetBuffer(),strPtuPort.GetBuffer(),strPtuUserName.GetBuffer(),strPtuPassword.GetBuffer(),strPtuName.GetBuffer(),strRvuType.GetBuffer(),strRvuCode.GetBuffer());
		ret = mysql_query(g_LinkageSettingMysql, sql_buf);
	}
	//插入执行成功
	if (!ret)
	{
		MessageBox("RVU配置信息保存成功","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("RVU配置信息保存成功：%s", strPtuIP);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}
	//插入执行失败
	else
	{
		MessageBox("RVU配置信息保存失败,请检查数据库连接","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("RVU配置信息保存失败：%s", strPtuIP);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}

	//重载全局变量
	ReadRVUInfo();

	//更新列表
	//InialRouteRvuDeviceList(m_pStationNode);
	InialRvuDeviceList(m_pStationNode);
	//InialSubSystemList(m_pStationNode);
}

bool CDlgPtuManage::CheckRvuParams(CString strPtuIP, CString strPtuPort, CString strPtuUserName, CString strPtuPassword, CString strRvuType, CString strRvuCode)
{
	//判断信息是否格式是否正确
	if (strPtuIP.IsEmpty()||strPtuPort.IsEmpty()||strPtuUserName.IsEmpty()||strPtuPassword.IsEmpty()||strRvuType.IsEmpty())
	{
		MessageBox("PTU信息不全","辅助系统");
		return false;
	}

	if (strRvuType == "ANFANG" && strRvuCode == "")
	{
		MessageBox("安防平台规范必须填写rvu的ID!", "辅助系统");
		return false;
	}

	if (atoi(strPtuPort.GetBuffer())>65535||atoi(strPtuPort.GetBuffer())<0)
	{
		MessageBox("端口信息有误，请重新输入！");
		return false;
	}

	MatchResult result = g_regexpIP.Match(strPtuIP.GetBuffer());
	if (!result.IsMatched())
	{
		MessageBox("IP地址格式有误！");
		return false;
	}
}

BOOL CDlgPtuManage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//解决输入一个字符输入四遍的问题
	//return TRUE;

	return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPtuManage::OnBnClickedButtonConnectTest()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CPtuDlg* pParent = (CPtuDlg*)(this->GetParent()->GetParent());
	HTREEITEM hItem = pParent->m_treeList.GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	StationNode* pStationNode = (StationNode*)pParent->m_treeList.GetItemData(hItem);
	if (pStationNode == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	//node_type为2时表示变电站节点
	if (pStationNode->node_type != 2)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	CString strPtuIP = "";
	CString strPtuPort = "";
	CString strPtuUserName = "";
	CString strPtuPassword = "";
	m_ctrPtuIP.GetWindowText(strPtuIP);
	m_ctrPtuPort.GetWindowText(strPtuPort);
	m_ctrPtuUserName.GetWindowText(strPtuUserName);
	m_ctrPtuPassword.GetWindowText(strPtuPassword);

	//测试连接PTU功能
	int nRet = ConnectTest(strPtuIP,strPtuPort);

	if (nRet != 0)
	{
		MessageBox("连接失败","辅助系统");
		//连接成功，更改状态
		pStationNode->connect_ptu = 0;

		m_ctrStaticPtuStationValue.SetWindowText("未连接");
	}
	else
	{
		//连接成功，更改状态
		pStationNode->connect_ptu = 1;

		m_ctrStaticPtuStationValue.SetWindowText("已连接");
	}

	CRect rect;
	m_ctrStaticPtuStationValue.GetWindowRect(rect);
	GetParent()->GetParent()->ScreenToClient(rect);
	GetParent()->GetParent()->InvalidateRect(&rect);
	pParent->m_treeList.SetItemData(hItem,(DWORD)pStationNode);

}

int CDlgPtuManage::ConnectTest(CString strPtuIP,CString strPtuPort)
{
	int ret = -1 , len = 36 , i = 0,j = 0;

	for (j=0; j<g_nAssRVUNum; j++)
	{
		if (strcmp(g_tAssRVU[j].rvu_ip,strPtuIP.GetBuffer())==0)
		{
			ret = g_tAssRVU[j].commnet_handle;
			return 0 ;
		}
	}

	if (ret <= 0)
	{
		ret =  ComOpenNet("172.17.36.61",0,strPtuIP.GetBuffer(),atoi(strPtuPort.GetBuffer()),NETCLIENT,TCP,RECVBUFSIZE,SOCKETRECVBUFSIZE,SOCKETSENDBUFSIZE,10,-1,10,recv_parse,NULL,0);
	}

	if (ret == -1)
	{
		return 3;
	}

	return 0;
}

void CDlgPtuManage::InialRouteRvuDeviceList(StationNode *pNodeInfo)
{
	//FreeAllTreeItem(&m_treeRouteRvuDeviceList,NULL);
	//HTREEITEM hMainItem = NULL;
	//HTREEITEM hItemRvu = NULL,hItemDevice = NULL;
	////添加远端rvu设备tree的节点
	//hMainItem = m_treeRouteRvuDeviceList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	//SetRouteRvuTreeNodeInfo(hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0);
	//for(int i=0;i<g_nAssRVUNum;i++)
	//{
	//	//添加rvu
	//	if (g_tAssRVU[i].station_id == pNodeInfo->station_id)
	//	{
	//		hItemRvu = m_treeRouteRvuDeviceList.InsertItem(g_tAssRVU[i].rvu_name,8,8,hMainItem);
	//		SetRouteRvuTreeNodeInfo(hItemRvu,g_tAssRVU[i].rvu_id,RVU_NODE,g_tAssRVU[i].rvu_name,g_tAssRVU[i].station_id);
	//	}
	//}

	//ExpandAllItem(&m_treeRouteRvuDeviceList,NULL);
}

void CDlgPtuManage::SetRouteRvuTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId)
{
	NodeInfo* pNodeInfo = NULL;
	pNodeInfo = new NodeInfo;
	memset(pNodeInfo,0,sizeof(NodeInfo));
	
	pNodeInfo->node_id = nNodeId;
	pNodeInfo->node_type = nNodeType;
	sprintf_s(pNodeInfo->node_name,"%s",szNodeName);
	pNodeInfo->parent_id = nParentId;

	m_treeRouteRvuDeviceList.SetItemData(hItem,(DWORD)pNodeInfo);
}

void CDlgPtuManage::SetSusSystemTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId)
{
	NodeInfo* pNodeInfo = NULL;
	pNodeInfo = new NodeInfo;
	memset(pNodeInfo,0,sizeof(NodeInfo));

	pNodeInfo->node_id = nNodeId;
	pNodeInfo->node_type = nNodeType;
	sprintf_s(pNodeInfo->node_name,"%s",szNodeName);
	pNodeInfo->parent_id = nParentId;

	m_treeSubSystemList.SetItemData(hItem,(DWORD)pNodeInfo);
}

void CDlgPtuManage::SetLocalRvuTreeNodeInfo(HTREEITEM hItem,int nNodeId,int nNodeType,char* szNodeName,int nParentId)
{
	NodeInfo* pNodeInfo = NULL;
	pNodeInfo = new NodeInfo;
	memset(pNodeInfo,0,sizeof(NodeInfo));

	pNodeInfo->node_id = nNodeId;
	pNodeInfo->node_type = nNodeType;
	sprintf_s(pNodeInfo->node_name,"%s",szNodeName);
	pNodeInfo->parent_id = nParentId;

	m_treeRvuDeviceList.SetItemData(hItem,(DWORD)pNodeInfo);
}

void CDlgPtuManage::InialRvuDeviceList(StationNode *pNodeInfo)
{
	//清空列表
	FreeAllTreeItem(&m_treeRvuDeviceList,NULL);
	//添加变电站根结点
	HTREEITEM hMainItem = NULL;
	HTREEITEM hItemRvu = NULL,hItemDevice = NULL;
	hMainItem = m_treeRvuDeviceList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	SetLocalRvuTreeNodeInfo(hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0);
	//寻找该变电站绑定的rvu
	for(int i=0;i<g_nAssRVUNum;i++)
	{
		//添加rvu
		if (g_tAssRVU[i].station_id == pNodeInfo->station_id)
		{
			hItemRvu = m_treeRvuDeviceList.InsertItem(g_tAssRVU[i].rvu_name,7,7,hMainItem);
			SetLocalRvuTreeNodeInfo(hItemRvu,g_tAssRVU[i].rvu_id,RVU_NODE,g_tAssRVU[i].rvu_name,g_tAssRVU[i].station_id);
			//添加rvu下的设备
			for (int j = 0;j<g_nAssRVUDeviceNum;j++)
			{
				//过滤掉视频设备
				if (g_tAssRVUDevice[j].device_type == 12)
				{
					continue;
				}
				if (g_tAssRVUDevice[j].rvu_id == g_tAssRVU[i].rvu_id)
				{
					hItemDevice = m_treeRvuDeviceList.InsertItem(g_tAssRVUDevice[j].node_name,10,10,hItemRvu);
					SetLocalRvuTreeNodeInfo(hItemDevice,g_tAssRVUDevice[j].sm_id,RVU_DEVICE_NODE,g_tAssRVUDevice[j].node_name,g_tAssRVUDevice[j].rvu_id);
				}	
			}
		}
	}

	ExpandAllItem(&m_treeRvuDeviceList,NULL);
}

void CDlgPtuManage::InialSubSystemList(StationNode *pNodeInfo)
{
	////清空列表
	//FreeAllTreeItem(&m_treeSubSystemList,NULL);
	////添加变电站根结点
	//HTREEITEM hMainItem = NULL;
	//HTREEITEM hItemSubSystem = NULL,hItemDevice = NULL;
	//hMainItem = m_treeSubSystemList.InsertItem(pNodeInfo->station_name, 8, 8, TVI_ROOT, NULL);
	//SetSusSystemTreeNodeInfo(hMainItem,pNodeInfo->station_id,STATION_NODE,pNodeInfo->station_name,0);

	////获取变电站下的所有RVU
	//int nRvuId[100]={0};
	//int nRvuNum = 0;
	//for (int m=0;m<g_nAssRVUNum;m++)
	//{
	//	if (g_tAssRVU[m].station_id == pNodeInfo->station_id)
	//	{
	//		nRvuId[nRvuNum] = g_tAssRVU[m].rvu_id;
	//		nRvuNum++;
	//	}
	//}
	////添加子系统节点
	//for (int i=0;i<g_nAssStationObNodeNum;i++)
	//{
	//	hItemSubSystem = m_treeSubSystemList.InsertItem(g_tAssStationObNode[i].node_name,11,11,hMainItem);
	//	SetSusSystemTreeNodeInfo(hItemSubSystem,g_tAssStationObNode[i].id,SUBSYSTEM_NODE,g_tAssStationObNode[i].node_name,g_tAssStationObNode[i].parent_id);
	//	for (int j = 0;j<g_nAssRVUDeviceNum;j++)
	//	{
	//		for(int k=0;k<nRvuNum;k++)
	//		{
	//			if (g_tAssRVUDevice[j].sub_sys_id == g_tAssStationObNode[i].id && g_tAssRVUDevice[j].rvu_id == nRvuId[k])
	//			{
	//				hItemDevice = m_treeSubSystemList.InsertItem(g_tAssRVUDevice[j].node_name,10,10,hItemSubSystem);
	//				SetSusSystemTreeNodeInfo(hItemDevice,g_tAssRVUDevice[j].sm_id,RVU_DEVICE_NODE,g_tAssRVUDevice[j].node_name,g_tAssRVUDevice[j].sub_sys_id);
	//			}	
	//		}

	//	}
	//}

	//ExpandAllItem(&m_treeSubSystemList,NULL);
}

void CDlgPtuManage::OnBnClickedButtonGetDeviceInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hDeviceItem = NULL;
	NodeInfo* pNodeInfo = NULL;
	HTREEITEM hItem = m_treeRouteRvuDeviceList.GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("当前没有选中RVU设备资源的RVU节点","辅助系统");
		return;
	}

	NodeInfo* pNodeinfo = (NodeInfo*)m_treeRouteRvuDeviceList.GetItemData(hItem);
	if (pNodeinfo->node_type != RVU_NODE)
	{
		MessageBox("当前没有选中RVU设备资源的RVU节点","辅助系统");
		return;
	}

	CString strPtuIP = "";
	CString strPtuPort = "";
	CString strPtuUserName = "";
	CString strPtuPassword = "";
	m_ctrPtuIP.GetWindowText(strPtuIP);
	m_ctrPtuPort.GetWindowText(strPtuPort);
	m_ctrPtuUserName.GetWindowText(strPtuUserName);
	m_ctrPtuPassword.GetWindowText(strPtuPassword);
	
	//获取监控模块信息
	int ret = GetRVUServerDevice(strPtuIP.GetBuffer(),atoi(strPtuPort.GetBuffer()),strPtuUserName.GetBuffer(),strPtuPassword.GetBuffer());
	if (ret == -1)
	{
		MessageBox("网络连接失败！","辅助系统");
		return;
	}

	//等待获取成功
	DWORD dwReturnValue = 0;
	dwReturnValue = WaitForSingleObject(g_hGetDeviceEvent,2*1000);

	//获取成功后,更新列表
	if (dwReturnValue == WAIT_TIMEOUT)
	{
		MessageBox("获取设备信息超时,请重试!","辅助系统");
		return;
	}
	if (dwReturnValue == WAIT_OBJECT_0)
	{
		MessageBox("获取设备信息成功!","辅助系统");
		HTREEITEM hSel = m_treeRouteRvuDeviceList.GetSelectedItem();
		if (hSel != NULL)
		{
			//删除一下RVU下的设备节点
			hDeviceItem = m_treeRouteRvuDeviceList.GetChildItem(hSel);
			while(hDeviceItem!=NULL)
			{
				HTREEITEM hDeleteItem = hDeviceItem;
				pNodeInfo = (NodeInfo*)m_treeRouteRvuDeviceList.GetItemData(hDeviceItem);
				if (pNodeInfo!=NULL)
				{
					delete pNodeInfo;
					pNodeInfo = NULL;
					m_treeRouteRvuDeviceList.SetItemData(hDeviceItem,NULL);
				}
				hDeviceItem = m_treeRouteRvuDeviceList.GetNextSiblingItem(hDeviceItem);
				m_treeRouteRvuDeviceList.DeleteItem(hDeleteItem);
			}

			pNodeInfo = (NodeInfo*)m_treeRouteRvuDeviceList.GetItemData(hSel);

			for (int j=0; j<g_nTSM; j++)
			{
				//视频设备不从rvu这里获取
				if (g_t_TSM[j].device_type == 12)
				{
					continue;
				}
				g_t_TSM[j].rvu_id = pNodeInfo->node_id;
				hItem = m_treeRouteRvuDeviceList.InsertItem(g_t_TSM[j].sm_name,5,5,hSel);
				SetRouteRvuTreeNodeInfo(hItem,g_t_TSM[j].id.sm_id,RVU_DEVICE_NODE,g_t_TSM[j].sm_name,g_t_TSM[j].rvu_id);
			}
			m_treeRouteRvuDeviceList.Expand(hSel,TVE_EXPAND);
		}
	}

	//获取监控节点信息
}

int CDlgPtuManage::GetRVUServerDevice(char* ip,int port,char* user,char* password)
{
	int ret = -1 , len = 36 , i = 0,j = 0;

	for (j=0; j<g_nAssRVUNum; j++)
	{
		if (strcmp(g_tAssRVU[j].rvu_ip,ip)==0)
		{
			ret = g_tAssRVU[j].commnet_handle;
			break;
		}
	}

	if (ret <= 0)
	{
		ret =  ComOpenNet("172.17.36.61",0,ip,port,NETCLIENT,TCP,RECVBUFSIZE,SOCKETRECVBUFSIZE,SOCKETSENDBUFSIZE,10,-1,10,recv_parse,NULL,0);
	}

	DWORD xyh = 0;
	BYTE buf[SOCKETSENDBUFMAXSIZE];
	memset(buf,0,SOCKETSENDBUFMAXSIZE);

	if( ret > 0 )
	{
		buf[0] = 0x78;
		buf[1] = 0x87;
		buf[2] = 0x1c;//数据内容长度（不是整个帧长度）
		buf[3] = 0x20;//请求应答标志
		buf[4] = 0x1; //报文序号
		buf[5] = 0;
		buf[6] = 0;
		for(i = 0 ; i < 6 ; i ++)
		{
			buf[6] += buf[i];
		}
		buf[6] = buf[6] & 0xff;
		buf[6] = ~buf[6];
		buf[7] = 0;
		buf[8] = 0xc7;
		buf[9] = 0xc7;
		buf[10] = 0x08;
		buf[11] = 0;
		buf[12] = 0;
		buf[13] = 0;
		buf[14] = 0;
		buf[15] = 0;
		buf[16] = 0;
		buf[17] = 0;
		buf[18] = 0;
		buf[19] = 0;
		buf[20] = 0;
		buf[21] = 0;
		buf[22] = 0xc9;//报文类型 0xc9 = 201表示请求RVU监控模块的属性
		buf[23] = 0;
		buf[24] = 0;
		buf[25] = 0;
		buf[26] = 0x1;
		buf[27] = 0;
		buf[28] = 0;
		buf[29] = 0;
		buf[30] = 0xff;	//数据开始,SM编号65535
		buf[31] = 0xff;
		buf[32] = 0x00;//节点编号0
		buf[33] = 0x00;
		buf[34] = 0xd8;
		buf[35] = 0xd8;
		for ( i = 26 ; i <= 33 ; i ++)
		{
			xyh += buf[i];
		}
		buf[14] = xyh & 0xff;
		buf[15] = ( xyh >> 8 ) & 0xff;
		for ( i = 8 ; i <= 35 ; i ++)
		{
			buf[7] += buf[i];
		}
		buf[7] = ~buf[7];
		ComSendNet(ret,ip,port,0,0,0,0,(char*)buf,len,0);
	}
	else
	{
		TRACE("----ip=%s,ComOpenNet : ret = %d----\n\n",ip,ret);
	}

	return ret;
}

void CDlgPtuManage::OnBnClickedButtonAddToLocalDatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_treeRouteRvuDeviceList.GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("当前没有选中RVU节点","辅助系统");
		return;
	}

	NodeInfo* pNodeInfo = (NodeInfo*)m_treeRouteRvuDeviceList.GetItemData(hItem);
	if (pNodeInfo->node_type != RVU_NODE)
	{
		MessageBox("当前没有选中RVU节点","辅助系统");
		return;
	}

	char sql_buf[1024] = {0x0};
	int rnums = 0;
	MYSQL_RES* res;
	//提示框,是否更新本地数据库
	INT_PTR nRes;
	nRes = MessageBox("是否将RVU设备资源更新到本地数据库","辅助系统",MB_OKCANCEL);
	if (nRes == IDCANCEL)
	{
		return;
	}
	//将rvu设备资源更新到本地数据库
	for (int i=0;i<g_nTSM;i++)
	{
		//视频设备不从rvu这里获取
		if (g_t_TSM[i].device_type == 12)
		{
			continue;
		}
		//判断是否已经存在于数据表ass_rvu_device中
		sprintf_s(sql_buf,1024,"select * from ass_rvu_sm where rvu_id=%d and sm_id=%d and node_id=%d",g_t_TSM[i].rvu_id,
			g_t_TSM[i].id.sm_id,g_t_TSM[i].id.node_id);
		if (!mysql_query(g_LinkageSettingMysql,sql_buf))
		{
			res = mysql_store_result(g_LinkageSettingMysql);
			rnums = mysql_num_rows(res);
			mysql_free_result(res);
		}

		if (rnums > 0)
		{
			continue;
		}
		
		int sub_sys_type = 0;
		//char szTableName[32] = {0};
		//GetTableNameByType(szTableName,g_t_TSM[i].type);
		sub_sys_type = GetSubSysByDeviceType(g_t_TSM[i].device_type);
		//char szDeviceCode[64]={0};
		//GetGuid(szDeviceCode);

		////插入到设备基本信息表
		sprintf_s(sql_buf,1024,"INSERT INTO ass_rvu_sm(sub_sys_id,rvu_id,sm_id,sm_name,node_id,device_type,sub_device_type,type) " 
			" values(%d,%d,%d,'%s',%d,%d,%d,%d)",sub_sys_type,g_t_TSM[i].rvu_id,g_t_TSM[i].id.sm_id,g_t_TSM[i].sm_name,
			g_t_TSM[i].id.node_id,g_t_TSM[i].device_type,g_t_TSM[i].sub_device_type,g_t_TSM[i].type);
		mysql_query(g_LinkageSettingMysql,sql_buf);

		//根据设备类型和节点编号，判断属于yc,yx,yk哪种类型设备
		//char szSMName[32] = {0};//监控点名称
		//GetSMNameByDeviceTypeAndNodeNum(szSMName,g_t_TSM[i].device_type,g_t_TSM[i].id.node_id);
		////然后插入监控点名称属性表
		//sprintf_s(sql_buf,1024,"INSERT INTO ass_rvu_%s(device_code,sub_sys_id,rvu_id,sm_id,sm_name,node_id,device_type,sub_device_type,type) " 
		//	" values('%s',%d,%d,%d,'%s',%d,%d,%d,%d)",szSMName,szDeviceCode,sub_sys_type,g_t_TSM[i].rvu_id,g_t_TSM[i].id.sm_id,g_t_TSM[i].sm_name,
		//	g_t_TSM[i].id.node_id,g_t_TSM[i].device_type,g_t_TSM[i].sub_device_type,g_t_TSM[i].type);
		//mysql_query(g_LinkageSettingMysql,sql_buf);
	}

	//重新加载全局变量
	ReadRVUDevice();

	//刷新本地设备资源列表
	InialRvuDeviceList(m_pStationNode);
	//刷新子系统列表
	InialSubSystemList(m_pStationNode);
}

void CDlgPtuManage::GetSMNameByDeviceTypeAndNodeNum(char* szSMName,int nDeviceType,int nNodeId)
{
	//由于监控系统技术规范中，节点编号没有0，所以得到的0一概以1处理
	if (nNodeId == 0)
	{
		nNodeId == 1;
	}

	//遥测
	if ((nDeviceType==1)||
		(nDeviceType==8&&nNodeId==1)||
		(nDeviceType==8&&nNodeId==3)||
		(nDeviceType==14&&nNodeId==3)||
		(nDeviceType==19&&nNodeId>=3&&nNodeId<=50))
	{
		sprintf_s(szSMName,32,"yc");
		return;
	}

	//遥信
	if ((nDeviceType==2)||
		(nDeviceType==4)||
		(nDeviceType==5)||
		(nDeviceType==6)||
		(nDeviceType==13)||
		(nDeviceType==18)||
		(nDeviceType==17)||
		(nDeviceType==20)||
		(nDeviceType==3&&nNodeId==1)||
		(nDeviceType==11&&nNodeId==4)||
		(nDeviceType==11&&nNodeId==9)||
		(nDeviceType==11&&nNodeId==14)||
		(nDeviceType==11&&nNodeId==19)||
		(nDeviceType==11&&nNodeId==24)||
		(nDeviceType==11&&nNodeId==29)||
		(nDeviceType==14&&nNodeId==1)||
		(nDeviceType==19&&nNodeId==2))
	{
		sprintf_s(szSMName,32,"yx");
		return;
	}

	//遥控
	if ((nDeviceType==7)||
		(nDeviceType==3&&nNodeId==2)||
		(nDeviceType==8&&nNodeId==2)||
		(nDeviceType==9)||
		(nDeviceType==10)||
		(nDeviceType==11&&nNodeId==5)||
		(nDeviceType==11&&nNodeId==10)||
		(nDeviceType==11&&nNodeId==15)||
		(nDeviceType==11&&nNodeId==20)||
		(nDeviceType==11&&nNodeId==25)||
		(nDeviceType==11&&nNodeId==30)||
		(nDeviceType==12&&nNodeId>=11&&nNodeId<=13)||
		(nDeviceType==12&&nNodeId>=21&&nNodeId<=22)||
		(nDeviceType==14&&nNodeId==2)||
		(nDeviceType==14&&nNodeId>=4&&nNodeId<=6)||
		(nDeviceType==15&&nNodeId==2)||
		(nDeviceType==16&&nNodeId>=1&&nNodeId<=6)||
		(nDeviceType==16&&nNodeId>=11&&nNodeId<=15)||
		(nDeviceType==21))
	{
		sprintf_s(szSMName,32,"yk");
		return;
	}

	//遥调
	if ((nDeviceType==11&&nNodeId==3)||
		(nDeviceType==12&&nNodeId>=1&&nNodeId<=10)||
		(nDeviceType==12&&nNodeId>=14&&nNodeId<=20)||
		(nDeviceType==16&&nNodeId>=7&&nNodeId<=10))
	{
		sprintf_s(szSMName,32,"yt");
		return;
	}

	sprintf_s(szSMName,32,"other");
	return;
}

int CDlgPtuManage::GetSubSysByDeviceType(int nDeviceType)
{
	//目前只分三类，安防，消防，门禁。把灯光控制等其他都归到安防
	int sub_sys_type = 0;
	switch(nDeviceType)
	{
	case 14:
	case 15:
	case 16:
		sub_sys_type = SECURITY_SUB_SYSTEM;//ENVIRONMENT_SUB_SYSTEM;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 8:
	case 9:
	case 10:
	case 13:
		sub_sys_type = SECURITY_SUB_SYSTEM;
		break;
	case 12:
		sub_sys_type = CAMERA_SUB_SYSTEM;
		break;
	case 17:
	case 18:
		sub_sys_type = FIRE_SUB_SYSTEM;
		break;
	case 7:
		sub_sys_type = SECURITY_SUB_SYSTEM;//LIGHT_SUB_SYSTEM;
		break;
	case 11:
		sub_sys_type = DOOR_SUB_SYSTEM;
		break;
	default:
		sub_sys_type = SECURITY_SUB_SYSTEM;//OTHER_SUB_SYSTEM;
		break;
	}
	return sub_sys_type;
}

void CDlgPtuManage::GetTableNameByType(char* szTableName,int nType)
{
	switch(nType)
	{
	case 1:
		sprintf_s(szTableName,32,"ass_rvu_yc");
		break;
	case 2:
		sprintf_s(szTableName,32,"ass_rvu_yx");
		break;
	case 3:
		sprintf_s(szTableName,32,"ass_rvu_yk");
		break;
	case 4:
		sprintf_s(szTableName,32,"ass_rvu_yt");
		break;
	case 7:
		sprintf_s(szTableName,32,"ass_rvu_sm");
		break;
	default:
		sprintf_s(szTableName,32,"ass_rvu_other");
		break;
	}
}

void CDlgPtuManage::ExpandAllItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	if (hTreeItem == NULL)
	{
		hTreeItem = pTreeList->GetRootItem();
	}

	if (!pTreeList->ItemHasChildren(hTreeItem))
	{
		return;
	}

	HTREEITEM hNextItem = pTreeList->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		ExpandAllItem(pTreeList,hNextItem);
		hNextItem = pTreeList->GetNextItem(hNextItem,TVGN_NEXT);
	}

	pTreeList->Expand(hTreeItem,TVE_EXPAND);
}

void CDlgPtuManage::FreeAllTreeItem(CTreeCtrl* pTreeList,HTREEITEM hTreeItem)
{
	NodeInfo* pNodeInfo = NULL;
	if (hTreeItem == NULL)
	{
		hTreeItem = pTreeList->GetRootItem();
	}

	HTREEITEM hNextItem = pTreeList->GetChildItem(hTreeItem);
	while(hNextItem!=NULL)
	{
		FreeAllTreeItem(pTreeList,hNextItem);
		hNextItem = pTreeList->GetNextItem(hNextItem,TVGN_NEXT);
	}

	if (hTreeItem!=NULL)
	{
		pNodeInfo = (NodeInfo*)pTreeList->GetItemData(hTreeItem);
		if (pNodeInfo != NULL)
		{
			delete pNodeInfo;
			pNodeInfo = NULL;
			pTreeList->SetItemData(hTreeItem,NULL);
		}
		pTreeList->DeleteItem(hTreeItem);
	}

}

BOOL CDlgPtuManage::OnEraseBkgnd(CDC* pDC)
{
	//设置对话框背景色
	CBrush backBrush(RGB(218,255,251));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	//CDC srcDC;
	//srcDC.CreateCompatibleDC(pDC);
	//CBitmap* pOldBmp;
	////pOldBmp = srcDC.SelectObject(m_pTitleBmp);
	//BITMAP bmp;
	////m_pTitleBmp->GetBitmap(&bmp);
	////pDC->SetStretchBltMode(COLORONCOLOR);
	////pDC->StretchBlt(0,MAIN_TITLE_HEIGHT + 1,m_WndRect.Width(),MAIN_BUTTON_HIGHT,&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	////srcDC.SelectObject(pOldBmp);

	//CRect winrc;

	//GetClientRect(&winrc);

	//pOldBmp = srcDC.SelectObject(m_pBackBmp);
	//m_pBackBmp->GetBitmap(&bmp);
	//pDC->StretchBlt(0,0,winrc.Width(),winrc.Height(),
	//	&srcDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	//srcDC.SelectObject(pOldBmp);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgPtuManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDlgPtuManage::OnBnClickedButtonDelRvu()
{
	// TODO: 在此添加控件通知处理程序代码
	CPtuDlg* pParent = (CPtuDlg*)(this->GetParent()->GetParent());
	HTREEITEM hItem = (pParent->m_treeList).GetSelectedItem();
	if (hItem == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	StationNode* pStationNode = (StationNode*)pParent->m_treeList.GetItemData(hItem);
	if (pStationNode == NULL)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	//node_type为2时表示变电站节点
	if (pStationNode->node_type != 2)
	{
		MessageBox("当前没有选中变电站节点","辅助系统");
		return;
	}

	CString strPtuName = "";
	CString strPtuIP = "";
	CString strPtuPort = "";
	CString strPtuUserName = "";
	CString strPtuPassword = "";
	m_ctrPtuName.GetWindowText(strPtuName);
	m_ctrPtuIP.GetWindowText(strPtuIP);
	m_ctrPtuPort.GetWindowText(strPtuPort);
	m_ctrPtuUserName.GetWindowText(strPtuUserName);
	m_ctrPtuPassword.GetWindowText(strPtuPassword);

	//判断信息是否格式是否正确
	if (strPtuIP.IsEmpty()||strPtuPort.IsEmpty()||strPtuUserName.IsEmpty()||strPtuPassword.IsEmpty())
	{
		MessageBox("RVU信息不全","辅助系统");
	}

	CString strText = "";
	strText.Format("删除RVU同时也会删除RVU下面的设备！确定要删除RVU：%s吗？",strPtuIP);
	if (MessageBox(strText,"辅助系统",MB_ICONEXCLAMATION|MB_OKCANCEL)==IDCANCEL)
	{
		return;
	}

	char sql_buf[1024]={0x0};
	MYSQL_RES	* res;
	//判断rvu设备是否存在
	int rnum = 0;
	sprintf_s(sql_buf,"select rvu_id from ass_rvu where rvu_ip='%s' and rvu_port='%s' and station_id='%d'",strPtuIP.GetBuffer(),strPtuPort.GetBuffer(),pStationNode->station_id);
	if (!mysql_query(g_LinkageSettingMysql, sql_buf))
	{
		res = mysql_store_result(g_LinkageSettingMysql);
		rnum = mysql_num_rows(res);
		mysql_free_result( res ) ;
	}

	if (rnum <= 0)
	{
		MessageBox("要删除的RVU在数据库中不存在","辅助系统");
		return ;
	}
	//删除rvu
	int ret;
	sprintf_s(sql_buf,"delete from ass_rvu where rvu_ip='%s' and station_id=%d",strPtuIP.GetBuffer(),pStationNode->station_id);
	ret = mysql_query(g_LinkageSettingMysql, sql_buf);
	if (!ret)
	{
		MessageBox("RVU设备删除成功","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("RVU设备删除成功：%s", strPtuIP);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}
	else
	{
		MessageBox("RVU设备删除失败,请检查数据库连接","辅助系统");

		//写操作记录
		CString strText = "";
		strText.Format("RVU设备删除失败：%s", strPtuIP);
		WriteAssUserOperateLogPre(strText.GetBuffer());
	}

	//重载全局变量
	ReadRVUInfo();
	//更新列表
	InialRouteRvuDeviceList(m_pStationNode);
	InialRvuDeviceList(m_pStationNode);
	InialSubSystemList(m_pStationNode);
}

void CDlgPtuManage::OnBnClickedButtonEditRvu()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
