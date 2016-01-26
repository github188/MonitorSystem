// DlgAlarmStatistics.cpp : 实现文件
//

#include "stdafx.h"
#include "VEMCUCtl.h"
#include "DlgAlarmStatistics.h"
#include "AssDBOperate.h"


// CDlgAlarmStatistics 对话框

IMPLEMENT_DYNAMIC(CDlgAlarmStatistics, CDialog)

CDlgAlarmStatistics::CDlgAlarmStatistics(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmStatistics::IDD, pParent)
{

}

CDlgAlarmStatistics::~CDlgAlarmStatistics()
{
}

void CDlgAlarmStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARM_STATISTICS, m_lstAlarmStatistics);
}


BEGIN_MESSAGE_MAP(CDlgAlarmStatistics, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgAlarmStatistics::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgAlarmStatistics 消息处理程序

BOOL CDlgAlarmStatistics::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	InitAlarmStatisticsList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAlarmStatistics::InitControl()
{
	DWORD dwStyle;
	dwStyle = m_lstAlarmStatistics.GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	m_lstAlarmStatistics.SetExtendedStyle(dwStyle);
	m_lstAlarmStatistics.SetBkColor(RGB(239,246,253));
	m_lstAlarmStatistics.SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	//初始化列表的字段
	CRect winrc;
	GetClientRect(winrc);

	int nWidth = winrc.Width();

	m_lstAlarmStatistics.DeleteAllItems();
	while(m_lstAlarmStatistics.DeleteColumn(0));

	m_lstAlarmStatistics.InsertColumn(0,"序号");
	m_lstAlarmStatistics.SetColumnWidth(0,nWidth*0.3);
	m_lstAlarmStatistics.InsertColumn(1,"变电站名称");
	m_lstAlarmStatistics.SetColumnWidth(1,nWidth*0.3);
	m_lstAlarmStatistics.InsertColumn(2,"告警发生次数");
	m_lstAlarmStatistics.SetColumnWidth(2,nWidth*0.3);
}

void CDlgAlarmStatistics::InitAlarmStatisticsList()
{
	std::vector<ALARM_STATISTICS> vecAlarmStatistics = GetAlarmStatisticsInfo();

	InsertAlarmStatisticsToList(vecAlarmStatistics);

}

void CDlgAlarmStatistics::InsertAlarmStatisticsToList(std::vector<ALARM_STATISTICS> vecAlarmStatistics)
{
	int nIndex = 0;
	for (size_t t=0; t<vecAlarmStatistics.size(); t++)
	{
		CString strIndex = "";
		strIndex.Format("%d",nIndex);
		m_lstAlarmStatistics.InsertItem(nIndex,strIndex);
		m_lstAlarmStatistics.SetItemText(nIndex,1,vecAlarmStatistics.at(t).szStationName);
		m_lstAlarmStatistics.SetItemText(nIndex,2,IntToCString(vecAlarmStatistics.at(t).nAlarmCount));

		nIndex++;
	}
}

void CDlgAlarmStatistics::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
