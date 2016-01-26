#include "stdafx.h"
#include "RVUServerParse.h"
#include "PtuDlg.h"
#include "DlgPtuManage.h"


void ParseRVUResponseSetPointAck(unsigned char* buff,int size)
{
	int nResult = buff[0];

	if (nResult == 1)
	{
		TRACE("----云台控制成功----\n");
	}
	else
	{
		TRACE("----云台控制失败----\n");
	}

	TRACE("---ParseRVUResponseSetPointAck:size=%d----\n\n",size);

	for (int i=4; i<size; i++)
	{
		TRACE("%x",buff[i]);
	}
}

void ParseRVUResponseSetNodes(unsigned char* buff,int size)
{
	TRACE("-----size=%d------\n\n",size);

	int i=0;
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	int count = 0;
	int data_index = 0;
	int first_len = 0;
	int data_len = 0;
	//char data[512*1024]={0x0};
	char * pData = NULL;
	pData = new char[size];
	memset(pData,0,sizeof(char)*size);

	TNODE* pTNode  = NULL;
	while (i<=size)
	{
		if (buff[i]==0x78 && buff[i+1]==0x87)
		{
			index = buff[i+4];
			reserved_index = buff[i+5];

			if (index == 1)
			{
				total_len = (buff[i+10] | (buff[i+11]<<8) | (buff[i+12]<<16) | (buff[i+13]<<24));
				type = buff[i+22] | (buff[i+23]<<8) | (buff[i+24]<<16) | (buff[i+25]<<24);
				count = buff[i+26] | (buff[i+27]<<8) | (buff[i+28]<<16) | (buff[i+29]<<24);
				if (count == -2)
				{
					TRACE("----查不到响应ID号------\n");
					return ;
				}
				else if (count == -1)
				{
					TRACE("----报文内容过长------\n");
				}

				//first_len = total_len%MAX_FRAME_LEN;
				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}

				data_len = first_len - SET_SM_ACK_DATA_OFFSET;

				if (reserved_index == 0)
				{
					pTNode = new TNODE[count];
					memset(pTNode,0,sizeof(TNODE)*count);
					memcpy(pTNode,&buff[i+SET_SM_ACK_DATA_OFFSET],sizeof(TNODE)*count);
					break;
				}
				else
				{
					memcpy(pData,&buff[i+SET_SM_ACK_DATA_OFFSET],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}

					pTNode = new TNODE[count];
					memset(pTNode,0,sizeof(TNODE)*count);
					memcpy(pTNode,pData,sizeof(TNODE)*count);
				}
			}
			break;
		}

		i++;
	}

	CTime time = CTime::GetCurrentTime();
	CString strInfo,strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d COMMAND->GET_NODES Cnt:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond(),count);

	//g_pMainDlg->m_ListInfo.AddString(strTime);

	for (int j=0; j<count; j++)
	{
		TRACE("----%d: name=%s-----\n",j,pTNode[j].node_name);
		/*if (pTNode[j].)
		{
		}*/

		strInfo.Format("Type:%d ID.SmID:%d.NodeID:%d VirtualID.SmID:%d VirtualID.NodeID:%d  Name:%s Desc:%s MaxVal:%f MinVal:%f unit:%s",
			pTNode[j].type,pTNode[j].id.sm_id,pTNode[j].id.node_id,pTNode[j].virtual_id.sm_id,pTNode[j].virtual_id.node_id,pTNode[j].node_name,pTNode[j].desc,pTNode[j].max_val,pTNode[j].min_val,pTNode[j].unit);
		//g_pMainDlg->m_ListInfo.AddString(strInfo);
	}

	if (pTNode != NULL)
	{
		delete pTNode;
		pTNode = NULL;
	}

	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
}

void ParseRVUResponseSetSM(unsigned char* buff,int size)
{
	TRACE("-----size=%d------\n\n",size);

	int i=0;
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	int count = 0;
	int data_index = 0;
	int first_len = 0;
	int data_len = 0;
	//char data[64*1024]={0x0};

	char * pData = NULL;
	pData = new char[size];
	memset(pData,0,sizeof(char)*size);
	TSM* pTSM  = NULL;
	while (i<=size)
	{
		if (buff[i]==0x78 && buff[i+1]==0x87)
		{
			index = buff[i+4];
			reserved_index = buff[i+5];

			if (index == 1)
			{
				total_len = (buff[i+10] | (buff[i+11]<<8) | (buff[i+12]<<16) | (buff[i+13]<<24));
				type = buff[i+22] | (buff[i+23]<<8) | (buff[i+24]<<16) | (buff[i+25]<<24);
				count = buff[i+26] | (buff[i+27]<<8) | (buff[i+28]<<16) | (buff[i+29]<<24);
				if (count == -2)
				{
					TRACE("----查不到响应ID号------\n");
					return ;
				}
				else if (count == -1)
				{
					TRACE("----报文内容过长------\n");
				}

				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}

				data_len = first_len - SET_SM_ACK_DATA_OFFSET;

				if (reserved_index == 0)
				{
					pTSM = new TSM[count];
					memset(pTSM,0,sizeof(TSM)*count);
					memcpy(pTSM,&buff[i+SET_SM_ACK_DATA_OFFSET],sizeof(TSM)*count);
					break;
				}
				else
				{
					memcpy(/*&data*/pData,&buff[i+SET_SM_ACK_DATA_OFFSET],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}

					pTSM = new TSM[count];
					memset(pTSM,0,sizeof(TSM)*count);
					memcpy(pTSM,pData,sizeof(TSM)*count);
				}
			}
			break;
		}

		i++;
	}

	memset(&g_t_TSM,0,sizeof(TSM)*MAX_SM_NUM_PER_STATION);

	g_nTSM = count;
	memcpy(&g_t_TSM,pTSM,sizeof(TSM)*count);
	
	if (pTSM != NULL)
	{
		delete[] pTSM;
		pTSM = NULL;
	}

	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}

	SetEvent(g_hGetDeviceEvent);
}

void ParseRVUSendRealtimeAlarm(int handle,unsigned char* buff,int size)
{
	TRACE("-----size=%d------\n\n",size);

	int i=0,k=0;
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	int count = 0;
	int data_index = 0;
	int first_len = 0;
	int data_len = 0;
	//char data[64*1024]={0x0};

	char * pData = NULL;
	pData = new char[size];
	memset(pData,0,sizeof(char)*size);
	TALARM* pTAlarm  = NULL;
	while (i<=size)
	{
		if (buff[i]==0x78 && buff[i+1]==0x87)
		{
			index = buff[i+4];
			reserved_index = buff[i+5];

			if (index == 1)
			{
				total_len = (buff[i+10] | (buff[i+11]<<8) | (buff[i+12]<<16) | (buff[i+13]<<24));
				type = buff[i+22] | (buff[i+23]<<8) | (buff[i+24]<<16) | (buff[i+25]<<24);
				count = buff[i+26] | (buff[i+27]<<8) | (buff[i+28]<<16) | (buff[i+29]<<24);
				if (count == -2)
				{
					TRACE("----查不到响应ID号------\n");
					return ;
				}
				else if (count == -1)
				{
					TRACE("----报文内容过长------\n");
				}

				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}

				data_len = first_len - SET_SM_ACK_DATA_OFFSET;

				if (reserved_index == 0)
				{
					pTAlarm = new TALARM[count];
					memset(pTAlarm,0,sizeof(TALARM)*count);
					memcpy(pTAlarm,&buff[i+SET_SM_ACK_DATA_OFFSET],sizeof(TALARM)*count);
					break;
				}
				else
				{
					memcpy(/*&data*/pData,&buff[i+SET_SM_ACK_DATA_OFFSET],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}

					pTAlarm = new TALARM[count];
					memset(pTAlarm,0,sizeof(TALARM)*count);
					memcpy(pTAlarm,pData,sizeof(TALARM)*count);
				}
			}
			break;
		}

		i++;
	}

	CTime time = CTime::GetCurrentTime();
	CString strInfo,strTime,strStationName;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());

	//g_pMainDlg->m_ListInfo.AddString(strTime);

	CString strStartTime,strEndTime,strConfirmTime,strCancelTime;
	int nImageIndex = 0;
	for (int j=0; j<count; j++)
	{
		strStartTime.Format("%d-%02d-%02d %02d:%02d:%02d",pTAlarm[j].starttime.years,pTAlarm[j].starttime.month,pTAlarm[j].starttime.day,pTAlarm[j].starttime.hour,pTAlarm[j].starttime.minute,pTAlarm[j].starttime.second);
		strEndTime.Format("%d-%02d-%02d %02d:%02d:%02d",pTAlarm[j].endtime.years,pTAlarm[j].endtime.month,pTAlarm[j].endtime.day,pTAlarm[j].endtime.hour,pTAlarm[j].endtime.minute,pTAlarm[j].endtime.second);
		strConfirmTime.Format("%d-%02d-%02d %02d:%02d:%02d",pTAlarm[j].confirmtime.years,pTAlarm[j].confirmtime.month,pTAlarm[j].confirmtime.day,pTAlarm[j].confirmtime.hour,pTAlarm[j].confirmtime.minute,pTAlarm[j].confirmtime.second);
		strCancelTime.Format("%d-%02d-%02d %02d:%02d:%02d",pTAlarm[j].canceltime.years,pTAlarm[j].canceltime.month,pTAlarm[j].canceltime.day,pTAlarm[j].canceltime.hour,pTAlarm[j].canceltime.minute,pTAlarm[j].canceltime.second);

		for (k=0; k<g_nAssRVUNum; k++)
		{
			if (handle == g_tAssRVU[k].commnet_handle)
			{
				break;
			}
		}

		for (i=0; i<g_nAcsRVUStation; i++)
		{
			if (g_tAcsRVUStationInfo[i].station_id == g_tAssRVU[k].station_id)
			{
				strStationName.Format("%s",g_tAcsRVUStationInfo[i].station_name);
				break;
			}
		}

		//for (int ii=0; ii<g_nAssRVUDeviceNum; ii++)
		//{
		//	if ((g_tAssRVUDevice[ii].sm_id==pTAlarm[j].id.sm_id)
		//		&&(g_tAssRVUDevice[ii].node_id==pTAlarm[j].id.node_id))
		//	{
		//		strInfo.Format("%s  %s%s%s%s",strTime,strStationName,g_tAssRVU[k].rvu_name,g_tAssRVUDevice[ii].node_name,pTAlarm[j].description);

		//		if (pTAlarm[j].alarm_level == 1)
		//		{
		//			nImageIndex = 2;
		//		}
		//		else if (pTAlarm[j].alarm_level == 1)
		//		{
		//			nImageIndex = 1;
		//		}
		//		else
		//		{
		//			nImageIndex = 0;
		//		}
		//		g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_lbxAlarmInfo.InsertString(0,strInfo,nImageIndex);

		//		//联动自动展示
		//		if (g_pMainDlg->m_pMultiVideo->m_bLinkageShow == TRUE)
		//		{
		//			char sql_buf[1024] = {0x0};
		//			MYSQL_RES* res;
		//			MYSQL_ROW row;
		//			char szDvrType[16] = {0x0};
		//			int dvr_type = 0;
		//			CString strInfo ;
		//			sprintf_s(sql_buf,1024,"select t4.camera_name,t1.preset_id,t4.channel_id,t3.dvr_type,t3.dvr_ip,t3.dvr_port,t3.dvr_username,t3.dvr_password  from "
		//				"ass_rvu_camera_preset as t1,ass_rvu_device_linkage as t2,ass_rvu_dvr as t3,ass_rvu_camera as t4  "
		//			" where t2.rvu_device_id=1 and t2.camera_preset_id = t1.id and t1.dvr_id=t3.id and t1.camera_id=t4.id;",g_tAssRVUDevice[ii].id);
		//			if (!mysql_query(&mysql_cameralist,sql_buf))
		//			{
		//				res = mysql_store_result(&mysql_cameralist);
		//				while (row = mysql_fetch_row(res))
		//				{
		//					sprintf_s(szDvrType,16,"%s",row[3]);

		//					if (stricmp(szDvrType,"HIK") == 0)
		//					{
		//						dvr_type = DEVTYPE_HK;
		//					}
		//					else if (stricmp(szDvrType,"DH")==0)
		//					{
		//						dvr_type = DEVTYPE_DH;
		//					}
		//					else if (stricmp(szDvrType,"DL")==0)
		//					{
		//						dvr_type = DEVTYPE_DL;
		//					}
		//					else if (stricmp(szDvrType,"MOBO")==0)
		//					{
		//						dvr_type = DEVTYPE_MOBOTIX;
		//					}
		//					else 
		//					{
		//						strInfo.Format("打开摄像机%s，未能识别DVR类型，打开摄像机失败！",row[0]);
		//						WriteUserOperateLog(g_szLoginName,strInfo.GetBuffer());

		//						WriteRVUAlarmLog(g_tAssRVU[k].rvu_id,g_tAssRVU[k].station_id,strStationName.GetBuffer(),g_tAssRVUDevice[ii].sm_id,g_tAssRVUDevice[ii].node_id,
		//							g_tAssRVUDevice[ii].node_name,strTime.GetBuffer(),pTAlarm[j].alarm_level,pTAlarm[j].description,pTAlarm[j].alarm_value);
		//						break;
		//					}

		//					if (g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_ViewChannel.m_video_opened==TRUE)
		//					{
		//						g_pMainDlg->m_pMultiVideo->CloseVideo(g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_camera_info.dvr_type,0);
		//					}

		//					memset(&g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_camera_info,0,sizeof(CAMERA_INFO));

		//					g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_camera_info.dvr_type = dvr_type;

		//					g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_strNodeName.Format("%s",row[0]);

		//					g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_strCameraName.SetText(row[0]);


		//					BOOL ret = g_pMainDlg->m_pMultiVideo->OpenVideo(row[4],atoi(row[5]),row[6],row[7],dvr_type,atoi(row[2]),0);

		//					if (ret == FALSE)
		//					{
		//						//AfxMessageBox("打开视频失败！");
		//						strInfo.Format("打开摄像机%s,DVR类型%s，IP地址%s,端口%d,通道号%d视频失败！",row[0],row[3],row[4],atoi(row[5]),atoi(row[2]));
		//						WriteUserOperateLog(g_szLoginName,strInfo.GetBuffer());
		//						 
		//						WriteRVUAlarmLog(g_tAssRVU[k].rvu_id,g_tAssRVU[k].station_id,strStationName.GetBuffer(),g_tAssRVUDevice[ii].sm_id,g_tAssRVUDevice[ii].node_id,
		//							g_tAssRVUDevice[ii].node_name,strTime.GetBuffer(),pTAlarm[j].alarm_level,pTAlarm[j].description,pTAlarm[j].alarm_value);
		//						break;
		//					}

		//					strInfo.Format("打开摄像机%s,DVR类型%s，IP地址%s,端口%d,通道号%d视频成功！",row[0],row[3],row[4],atoi(row[5]),atoi(row[2]));
		//					WriteUserOperateLog(g_szLoginName,strInfo.GetBuffer());

		//					g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_ViewChannel.m_video_opened = true;

		//					if (ret == TRUE)
		//					{
		//						ret = g_pMainDlg->m_pMultiVideo->DvrPtzControl(g_pMainDlg->m_pMultiVideo->m_ViewOut[0].m_nLoginID,dvr_type,atoi(row[2]),GOTO_PRESET,atoi(row[1]));
		//					}
		//					
		//				}
		//			}

		//		}

		//		TRACE("----alarm info = %s-----\n\n",strInfo);
		//		WriteRVUAlarmLog(g_tAssRVU[k].rvu_id,g_tAssRVU[k].station_id,strStationName.GetBuffer(),g_tAssRVUDevice[ii].sm_id,g_tAssRVUDevice[ii].node_id,
		//			g_tAssRVUDevice[ii].node_name,strTime.GetBuffer(),pTAlarm[j].alarm_level,pTAlarm[j].description,pTAlarm[j].alarm_value);
		//		break;
		//	}
		//}
		//for (i=0; i<g_nTSM; i++)
		//{
		//	if ((pTAlarm[j].id.sm_id==g_t_TSM[i].id.sm_id)
		//		&&(pTAlarm[j].id.node_id==g_t_TSM[i].id.node_id))
		//	{
		//		strInfo.Format("%s %s%s",strStartTime,g_t_TSM[i].sm_name,pTAlarm[j].description);
		//		int index = g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_AlarmInfoList.GetItemCount();
		//		g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_AlarmInfoList.InsertItem(index,strInfo);
		//		g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_AlarmInfoList.SetItemText(index,2,g_t_TSM[i].sm_name);
		//		if (pTAlarm[j].alarm_level==0)
		//		{
		//			strInfo.Format("%s","无告警判断");
		//		}
		//		else if (pTAlarm[j].alarm_level==1)
		//		{
		//			strInfo.Format("%s","严重告警");
		//		}
		//		else if (pTAlarm[j].alarm_level==2)
		//		{
		//			strInfo.Format("%s","主要告警");
		//		}
		//		else if (pTAlarm[j].alarm_level==3)
		//		{
		//			strInfo.Format("%s","一般告警");
		//		}
		//		g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_AlarmInfoList.SetItemText(index,3,strInfo);
		//		g_pMainDlg->m_pMultiVideo->m_pAlarmInfoDlg->m_AlarmInfoList.SetItemText(index,4,strStartTime);
		//		/*strInfo.Format("Size:%d Type:%d ID.SmID:%d.NodeID:%d EnumAlarmTime:%d StartTime:%s EndTime:%s ConfirmTime:%s CancelTime:%s AlarmLevel:%d AlarmValue:%f Description:%s ",
		//			size,pTAlarm[j].type,pTAlarm[j].id.sm_id,pTAlarm[j].id.node_id,pTAlarm[j].alarm_status,strStartTime,strEndTime,strConfirmTime,strCancelTime,pTAlarm[j].alarm_level,pTAlarm[j].alarm_value,pTAlarm[j].description);*/
		//	}
		//}
		
		//g_pMainDlg->m_ListInfo.AddString(strInfo);
	}

	if (pTAlarm != NULL)
	{
		delete pTAlarm;
		pTAlarm = NULL;
	}

	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
}

void CheckRVUIsOnLine(int handle)
{
	for (int i=0; i<g_nAssRVUNum; i++)
	{
		if (g_tAssRVU[i].commnet_handle == handle)
		{
			g_tAssRVU[i].lost_heatbeat = 0;
			TRACE("-----recv heart beat,ip=%s,port=%d----\n\n",g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port);
			break;
		}
	}
}

void ParseRVURecvBuf(void* arg)
{
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	unsigned char temp_buf[1024*32] = {0x0};

	RECV_PACKET* pRecvPacket = (RECV_PACKET*)arg;

	if (pRecvPacket == NULL)
	{
		return;
	}

	unsigned char* pBuff = (unsigned char*)pRecvPacket->recvbuf;
	int size = pRecvPacket->size;

	//TRACE("----size = %d-----\n",pRecvPacket->size);
	int i = 0;
	int temp_index = 0;
	while (i<=size)
	{
		if (pBuff[i]==0x78 && pBuff[i+1]==0x87)
		{
			total_len = (pBuff[i+2]) | (pBuff[i+3]<<8);
			index = pBuff[i+4];
			reserved_index = pBuff[i+5];

			type = (pBuff[i+22]) | (pBuff[i+23]<<8);

			switch(type)
			{
			case RESPONSE_RVU_SET_POINT_ACK:
				memcpy(&temp_buf,&pBuff[24],size-26);
				ParseRVUResponseSetPointAck(temp_buf,size-26);
				break;
			case RESPONSE_RVU_SET_SM:
				ParseRVUResponseSetSM(pBuff,size);
				break;
			case RESPONSE_RVU_SET_NODES:
				ParseRVUResponseSetNodes(pBuff,size);
				break;
			case SEND_REALTIME_ALARM:
				ParseRVUSendRealtimeAlarm(pRecvPacket->handle,pBuff,size);
				break;
			case HEART_BEAT_ACK:
				CheckRVUIsOnLine(pRecvPacket->handle);
				break;
			default:
				break;
			}
			break;
		}
		i++;
	}

	
	memset(&g_recv_packet,0,sizeof(RECV_PACKET));
}

int recv_parse(NETHANDLE handle,char* buf,int size,void* user)
{
	NETINFO info;

	if ((buf == NULL) || size == 0)
	{
		return 1;
	}

	if (g_recv_packet.size + size > RECVBUFSIZE)
	{
		memset(&g_recv_packet,0,sizeof(RECV_PACKET));
		return 1;
	}

	memcpy(g_recv_packet.recvbuf+g_recv_packet.size,buf,size);
	g_recv_packet.size += size;

	if (((unsigned char)buf[size-1]==0xd8)&&((unsigned char)buf[size-2]==0xd8))
	{
		g_recv_packet.handle = handle;
		ComDispatchTask(&g_parse_rvu_thread,ParseRVURecvBuf,&g_recv_packet,sizeof(RECV_PACKET),0);
	}

	return size;
	//return 0;

}