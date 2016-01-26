#include "stdafx.h"
#include "ListControlOperate.h"

void InitDeviceLedgerListColumn(CListCtrl *list)
{
	//联动关系列表
	DWORD dwStyle;
	dwStyle = list->GetStyle();
	dwStyle = LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT ;

	list->SetExtendedStyle(dwStyle);
	list->SetBkColor(RGB(239,246,253));
	list->SetTextBkColor(RGB(0xfe,0xFF,0xc6));

	list->DeleteAllItems();
	while(list->DeleteColumn(0));

	CRect winrc;
	list->GetClientRect(winrc);

	int nWidth = winrc.right - winrc.left;

	int nColumn = 0;
	list->InsertColumn(nColumn,"ID");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"变电站名称");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"设备名称");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"设备厂商");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"设备地址");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"设备端口");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"登陆用户名");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"登陆密码");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"协议类型");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"设备型号");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"硬盘容量");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"模拟视频接入数量");
	list->SetColumnWidth(nColumn,nWidth*0.2);
	nColumn++;
	list->InsertColumn(nColumn,"网络视频接入数量");
	list->SetColumnWidth(nColumn,nWidth*0.2);
}

void AddItemToDeviceLedgerList(CListCtrl *list, int nIndex, _T_ASS_DEVICE_LEDGER_* pDeviceLedger)
{
	int nColumn = 0;

	CString strIndex;
	strIndex.Format("%d", nIndex);
	list->InsertItem(nIndex, strIndex);

	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->szStationName);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->device_name);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->device_manufacturer);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->device_ip);

	CString strPort;
	strPort.Format("%d", pDeviceLedger->device_port);
	list->SetItemText(nIndex, ++nColumn, strPort);

	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->login_username);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->login_password);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->protocol_type);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->device_type);
	list->SetItemText(nIndex, ++nColumn, pDeviceLedger->harddisk_capacity);

	CString strAnalogVideoNum;
	strAnalogVideoNum.Format("%d", pDeviceLedger->analog_video_num);
	list->SetItemText(nIndex, ++nColumn, strAnalogVideoNum);

	CString strNetVideoNum;
	strNetVideoNum.Format("%d", pDeviceLedger->net_video_num);
	list->SetItemText(nIndex, ++nColumn, strNetVideoNum);

	list->SetItemData(nIndex, (DWORD_PTR)pDeviceLedger);
}

void FreeDeviceLedgerList(CListCtrl *list)
{
	int nNum = list->GetItemCount();
	for (int i=0; i<nNum; i++)
	{
		_T_ASS_DEVICE_LEDGER_* pDeviceInfo = (_T_ASS_DEVICE_LEDGER_*)list->GetItemData(i);
		delete pDeviceInfo;
		pDeviceInfo = NULL;
		list->SetItemData(i,(DWORD_PTR)pDeviceInfo);
	}
	list->DeleteAllItems();
}