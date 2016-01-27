#include "StdAfx.h"
#include "UpClient.h"
#include "ParseXML.h"
#include "DataBaseExcute.h"
#include "DVRPtz.h"

C_RECV_PACKET g_client_packet[MAX_STATION_NUM_PER_USER];
extern COMMONTHREADPOOL g_parse_client_thread;
extern COMMONTHREADPOOL g_c_linkthread;
extern COMMONTHREADPOOL g_db_linkthread;

int Client_Num=0;

extern _C_LOGIN_USER c_login_user[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int usernum;

extern T_RVU_DATA g_sRVUData[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int rvu_num;
//extern T_RVU_ALARM g_sRVUalarm[MAX_RVU_DEVICE_NUM_PER_STATION];

extern _T_DEV_ID g_devID[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int devnum;
extern int dateupdate;
extern int alarmupdate;

int send_parse(NETHANDLE handle,char* buf,int size,void* user)
{
	//NETINFO info;
	
	if ((buf == NULL) || size == 0)
	{
		return 1;
	}
	
	return size;
	
}
//实时数据推送线程
void PushData(void *arg)
{
	char *sendXml=NULL;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////////
// 		g_sRVUData[0].uploadflag=1;
// 		g_sRVUData[0].t_datas[0].changeflag=1;
// 		g_sRVUData[0].t_datas[0].enumstate=0;
// 		g_sRVUData[0].t_datas[1].enumstate=1;
// 		g_sRVUData[0].t_datas[1].alarmflag=1;
		///////////////////////////////////////////////////////////////////////////////

		if (dateupdate==0)
		{
			//间隔1分钟遍历一次
			Sleep(60*1000);
		
			continue;
		}
		for (int i=0;i<usernum;i++)
		{	//遍历已登录用户
			if (c_login_user[i].login_flag==1)
			{
				for (int j=0;j<rvu_num;j++)
				{	//遍历查找未更新站点
					if (c_login_user[i].stationID==g_sRVUData[j].station_id)
					{
						sendXml=new char[g_sRVUData[j].count*128+128];
						sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
							"<HTTP_XML EventType=\"Response_Push_Data\">");
						char add_XML[128];
						for(int k=0;k<g_sRVUData[j].count;k++)
						{	
							//遍历查找未更新数据
							if (g_sRVUData[j].t_datas[k].updateflag==1)
							{														
								sprintf(add_XML,"<Item RvuID=\"%d\" SmID=\"%d\" NodeID=\"%d\" Value=\"%f\" Type=\"%d\" Status=\"%d\" />",
									g_sRVUData[j].rvu_id,g_sRVUData[j].t_datas[k].id.SmID,g_sRVUData[j].t_datas[k].id.NodeID,
									g_sRVUData[j].t_datas[k].t_value,g_sRVUData[j].t_datas[k].type,g_sRVUData[j].t_datas[k].enumstate);
								strcat(sendXml,add_XML);						
							
							}
						}
						strcat(sendXml,"</HTTP_XML>");
						int result=0;
						result=ComSendNet(c_login_user[i].handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
						if (result<0)
						{
							ComCloseNet(c_login_user[i].handle,0);
							c_login_user[i].login_flag=0;
						}

						delete sendXml;
						sendXml=NULL;
					}
				}
			}
		}
		//用户遍历结束 实时信息列表置0
		for(int i=0;i<rvu_num;i++)
		{
			for(int j=0;j<g_sRVUData[i].count;j++)
			{
				g_sRVUData[i].t_datas[j].updateflag=0;
			}
		}
		dateupdate=0;

	}
}
//告警信息推送线程
// void PushAlarm(void *arg)
// {
// 	char *sendXml=NULL;
// 	int devtype=0;
// 	while(1)
// 	{
// 		for (int i=0;i<usernum;i++)
// 		{	
// 			///////////////////////////////////////////////////////////////////////////////////
// 			g_sRVUalarm[0].count=1;
// 			g_sRVUalarm[0].rvu_id=14;
// 			g_sRVUalarm[0].station_id=1;
// 			g_sRVUalarm[0].uploadflag=1;
// 			g_sRVUalarm[0].t_alarms[0].id.sm_id=1;
// 			g_sRVUalarm[0].t_alarms[0].id.node_id=1;
// 			g_sRVUalarm[0].t_alarms[0].alarm_level=1;
// 			g_sRVUalarm[0].t_alarms[0].alarm_status=0;
// 			g_sRVUalarm[0].t_alarms[0].type=0;
// 			g_sRVUalarm[0].t_alarms[0].changeflag=1;
// 			///////////////////////////////////////////////////////////////////////////////////
// 			//遍历已登录用户
// 			if (c_login_user[i].login_flag==1)
// 			{
// 				for (int j=0;j<rvu_num;j++)
// 				{	//遍历查找未更新站点
// 					for (int l=0;l<c_login_user[i].alarmstationNum;l++)
// 					{
// 						if ((c_login_user[i].AlarmStation[l]==g_sRVUalarm[j].station_id)&&(g_sRVUalarm[j].uploadflag==1))
// 						{
// 							char *s_XML="<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
// 								"<HTTP_XML EventType=\"Response_Push_Alarm\">";
// 							char add_XML[128];
// 							sendXml=new char[strlen(s_XML)+g_sRVUData[j].count*128+12];
// 							strcat(sendXml,s_XML);
// 							for(int k=0;k<g_sRVUalarm[j].count;k++)
// 							{	
// 								int devid=0;
// 								int hideflag=1;
// 								//获取该设备ID
// 								for (int t=0;t<devnum;t++)
// 								{
// 									if ((g_sRVUalarm[j].rvu_id==g_devID[t].rvuid)&&(g_sRVUalarm[j].t_alarms[k].id.sm_id==g_devID[t].id.SmID)&&(g_sRVUalarm[j].t_alarms[k].id.node_id==g_devID[t].id.NodeID))
// 									{
// 										devid=g_devID[t].devID;
// 										t=devnum;
// 									}
// 								}
// 								//查看该设备是否被用户屏蔽
// 								for (int p=0;p<c_login_user[i].devidNum;p++)
// 								{
// 									if (devid==c_login_user[i].devID[p])
// 									{
// 										hideflag=0;
// 										p=c_login_user[i].devidNum;
// 									}
// 								}
// 
// 								//遍历查找未更新数据
// 								if ((g_sRVUalarm[j].t_alarms[k].changeflag==1)&&(hideflag==1))
// 								{	
// 									devtype=GetDEVType(g_sRVUalarm[j].rvu_id,g_sRVUalarm[j].t_alarms[k].id.sm_id);
// 									char t_time[21];
// 									sprintf(t_time,"%d-%d-%d %d:%d:%d:%d",g_sRVUalarm[j].t_alarms[k].starttime.years,g_sRVUalarm[j].t_alarms[k].starttime.month,g_sRVUalarm[j].t_alarms[k].starttime.day,g_sRVUalarm[j].t_alarms[k].starttime.hour,g_sRVUalarm[j].t_alarms[k].starttime.minute,g_sRVUalarm[j].t_alarms[k].starttime.second,g_sRVUalarm[j].t_alarms[k].starttime.milsecond);
// 									sprintf(add_XML,"<Item DevID=\"%d\" StationID=\"%d\" AlarmType=\"%d\" AlarmStartTime=\"%s\" PriorLevel=\"%d\" />",
// 										devid,g_sRVUalarm[j].station_id,devtype,
// 										t_time,g_sRVUalarm[j].t_alarms[k].alarm_level);
// 									strcat(sendXml,add_XML);									
// 								
// 								}
// 							}
// 							strcat(sendXml,"</HTTP_XML>");
// 							ComSendNet(c_login_user[i].handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
// 							delete sendXml;
// 							sendXml=NULL;
// 						}
// 					}
// 					
// 				}
// 			}
// 		}
// 		//用户遍历结束 实时信息列表置0
// 		for(i=0;i<rvu_num;i++)
// 		{
// 			g_sRVUData[i].uploadflag=0;
// 			for(int j=0;j<g_sRVUData[i].count;j++)
// 			{
// 				g_sRVUData[i].t_datas[j].changeflag=0;
// 			}
// 		}
// 		//间隔1秒遍历一次
// 		Sleep(1*1000);
// 
// 	}
// }





//告警信息推送线程
void PushAlarm(void *arg)
{
	char *sendXml=NULL;

	int devtype=0;
	int linknum=0;
	time_t t;
	struct tm * timeinfo;
	char t_time[21];

	while(1)
	{
		if (alarmupdate==0)
		{
			//间隔1分钟遍历一次
			Sleep(60*1000);
			continue;
		}
		time(&t);
		timeinfo=localtime(&t);
		sprintf(t_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
		for (int i=0;i<usernum;i++)
		{	
			///////////////////////////////////////////////////////////////////////////////////
			
			///////////////////////////////////////////////////////////////////////////////////
			//遍历已登录用户
			if (c_login_user[i].login_flag==1)
			{
				for (int j=0;j<rvu_num;j++)
				{	//遍历查找未更新站点
					for (int l=0;l<c_login_user[i].alarmstationNum;l++)
					{
						if ((c_login_user[i].AlarmStation[l]==g_sRVUData[j].station_id))
						{
							sendXml=new char[g_sRVUData[j].count*128+128];

							sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<HTTP_XML EventType=\"Response_Push_Alarm\">");

							char add_XML[128];
	
							int sendflag=0;
							for(int k=0;k<g_sRVUData[j].count;k++)
							{	
								int devid=0;
								int hideflag=1;
								//获取该设备ID
								for (int t=0;t<devnum;t++)
								{
									if ((g_sRVUData[j].rvu_id==g_devID[t].rvuid)&&(g_sRVUData[j].t_datas[k].id.SmID==g_devID[t].SmID)&&(g_sRVUData[j].t_datas[k].id.NodeID==g_devID[t].NodeID))
									{
										devid=g_devID[t].devID;
										break;
									}
								}
								//查看该设备是否被用户屏蔽
								for (int p=0;p<c_login_user[i].devidNum;p++)
								{
									if (devid==c_login_user[i].devID[p])
									{
										hideflag=0;
										p=c_login_user[i].devidNum;
									}
								}
								
								//遍历查找告警数据
								if ((g_sRVUData[j].t_datas[k].enumstate>0)&&(g_sRVUData[j].t_datas[k].enumstate<4)&&(hideflag==1)&&(g_sRVUData[j].t_datas[k].alarmflag==1))
								{	
									sendflag=1;
									C_LINKINFO c_linkinfo;
									devtype=GetDEVType(g_sRVUData[j].rvu_id,g_sRVUData[j].t_datas[k].id.SmID,&c_linkinfo);
									
									sprintf(add_XML,"<Item DevID=\"%d\" StationID=\"%d\" AlarmType=\"%d\" AlarmStartTime=\"%s\" PriorLevel=\"%d\" />",
										devid,g_sRVUData[j].station_id,devtype,
										t_time,g_sRVUData[j].t_datas[k].enumstate);
									strcat(sendXml,add_XML);

									if (c_linkinfo.linknum>0)//有联动
									{
										T_LINKPUSH t_linkpush;
										t_linkpush.devid=devid;
										t_linkpush.handle=c_login_user[i].handle;
										t_linkpush.c_linkinfo=c_linkinfo;
										ComDispatchTask(&g_c_linkthread,LinkPush,&t_linkpush,sizeof(T_LINKPUSH),0);
									}
						
									
								}
							}
							strcat(sendXml,"</HTTP_XML>");
							int result=0;
							if (sendflag==1)
							{
								result=ComSendNet(c_login_user[i].handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
								if (result<0)
								{
									ComCloseNet(c_login_user[i].handle,0);
									c_login_user[i].login_flag=0;
								}	
							}
							
							delete sendXml;
							sendXml=NULL;
						}
					}
					
				}
			}
		}
		//用户遍历结束 实时信息列表告警标志置0
		for(int i=0;i<rvu_num;i++)
		{
			g_sRVUData[i].uploadflag=0;
			for(int j=0;j<g_sRVUData[i].count;j++)
			{
				g_sRVUData[i].t_datas[j].alarmflag=0;
			}
		}
		alarmupdate=0;
		
	}
}
//联动推送线程
void LinkPush(void *arg)
{
	T_LINKPUSH* t_linkpush = (T_LINKPUSH*)arg;
	time_t t;
	struct tm * timeinfo;
	time(&t);
	timeinfo=localtime(&t);
	char t_time[21];
	sprintf(t_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
	timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

	char *linkXml=NULL;
	linkXml=new char[t_linkpush->c_linkinfo.linknum*64+128];
	sprintf(linkXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<HTTP_XML EventType=\"Response_LinkageAction\">");
	char ladd_XML[64];
	int i=0;
	for(i=0;i<t_linkpush->c_linkinfo.linknum;i++)
	{
		sprintf(ladd_XML,"<Item DevID=\"%d\" CameraCode=\"%s\" PresetNum=\"%d\" tTime=\"%s\"/>",
			t_linkpush->devid,t_linkpush->c_linkinfo.c_link[i].c_code,t_linkpush->c_linkinfo.c_link[i].c_pset,
				t_time);
		strcat(linkXml,ladd_XML);
		DVR_Contrl(t_linkpush->c_linkinfo.c_link[i]);
	}
	strcat(linkXml,"</HTTP_XML>");

	int result=0;


	result=ComSendNet(t_linkpush->handle,NULL,0,0,0,0,0,linkXml,strlen(linkXml),0);
	if (result<0)
	{
		ComCloseNet(t_linkpush->handle,0);
		t_linkpush->handle=0;
	}
	else
	{
		ComDispatchTask(&g_db_linkthread,SaveRelation,t_linkpush,sizeof(T_LINKPUSH),0);
	}
	
	delete linkXml;
	linkXml=NULL;
}
//client端接收回调函数
int tcp_server_cb(NETHANDLE handle,char* buf,int size,void* user)
{
	if (buf == NULL && size == 0)
	{
		printf("有新的客户端连接！%d\n",handle);

		g_client_packet[Client_Num].handle=handle;
		g_client_packet[Client_Num].Client_Num=Client_Num;
		Client_Num++;
	}
	if (size>0)
	{
		printf("收到客户端传来的数据：%s\n",buf);


		int i=0;
		for(i=0;i<Client_Num;i++)
		{
	
			if (g_client_packet[i].handle==handle)
			{
				memcpy(g_client_packet[i].recvbuf,buf,size);
				ComDispatchTask(&g_parse_client_thread,ParseXML,&g_client_packet[i],sizeof(RECV_PACKET),0);
				memset(g_client_packet[i].recvbuf,0,RECVBUFSIZE);
				g_client_packet[i].size=0;
			
			}
		}
	
	}
	
			

	
	return size;
}