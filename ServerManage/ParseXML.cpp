#include "stdafx.h"
#include "ParseXML.h"
#include "DataBaseExcute.h"

_C_LOGIN_USER c_login_user[MAX_RVU_DEVICE_NUM_PER_STATION];
int usernum=0;
extern COMMONTHREADPOOL g_parsexmlthread;
extern COMMONTHREADPOOL g_sql_insert;
extern COMMONTHREADPOOL g_user_login;
int g_nLostPacketNum=0;

extern T_RVU_DATA g_sRVUData[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int rvu_num;

//////////////////////////////////////////////////////////////////////////
extern int alarmupdate;
extern int dateupdate;
//////////////////////////////////////////////////////////////////////////



void ArrayArrange(int *p,int num)
{
	int *p1=new int(num);
	int i=0;
	int j=0;
	for (i=0;i<num;i++)
	{
		p1[i]=p[i];
		p[i]=0;
	}
	
	for (i=0;i<num;i++)
	{
		if (p1[i]>0)
		{
			p[j]=p1[i];
			j++;
		}
	}
	delete p1;
}

int RecvPTUServerCallBack(NETHANDLE handle,char* buf,int size,void* user)
{
	if ((buf==NULL)||(size==0))
	{
		return -1;
	}

	ComDispatchTask(&g_parsexmlthread,ParseXML,buf,size,0);

	return size;
}

int GetParseXMLType(char* buf,int size)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	const char		*pResult = NULL;
	int nResult = -1;

	try
	{
		resXML.Parse(buf);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return nResult;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return nResult;

		if (strcmp(pResult, "Request_Login") == 0) 
			nResult = ASS_REQUEST_LOGIN;
		else if (strcmp(pResult,"Request_Subscribe_Station")==0)
			nResult = ASS_REQUEST_STATION;
		else if (strcmp(pResult,"ASS_KeepAlive")==0)
			nResult = ASS_KEEPALIVE;
		else if (strcmp(pResult,"ASS_Response_YKControl")==0)
			nResult = ASS_REQUEST_YK_CONTROL;
		else if(strcmp(pResult,"Request_Subscribe_Alarm")==0)
			nResult = ASS_REQUEST_ALARM;
		else if(strcmp(pResult,"Request_Hide_Dev")==0)
			nResult = ASS_REQUEST_HIDE;
		else if(strcmp(pResult,"Request_Clock")==0)
			nResult = ASS_REQUEST_TIME;
		
		return nResult;
	}
	catch(...)
	{

	}

	return nResult;
}
char * gettime()
{
	return "";
}
void ParseXML(void* arg)
{
	int nType = ASS_REQUEST_LOGIN;
	int nResult = -1;


	C_RECV_PACKET* pRecvPacket = (C_RECV_PACKET*)arg;
	
	if (pRecvPacket == NULL)
	{
		return;
	}
	char *strXml = (char*)pRecvPacket->recvbuf;
	int handle=pRecvPacket->handle;
	char *sendXml=NULL;


	_T_ASS_LOGIN_USER_INFO_ g_login_user;
	_T_ASS_ALARM_SET_ _g_alarm;
	_T_ASS_CONTROL_ _g_control;
	_T_ASS_HIDE_SET_ _g_hide;
	_T_ASS_STATION_INFO_ _g_station;


	TRACE("----XMLCallBackHandle-----\n");

	if (strXml == NULL||strlen(strXml) == 0||strlen(strXml) > 64*1024)
		return;

	char sendtime[64] = {0x0};
	char szReason[64] = {0x0};
	int nCommandResult  =0;
	try
	{
		//丢包判断
		if (g_nLostPacketNum > 100)
		{
			TRACE("-----丢包数:%d\n-----",g_nLostPacketNum);
			return ;
		}

		nType = GetParseXMLType(strXml,strlen(strXml));

		switch(nType)
		{
		case ASS_REQUEST_LOGIN:
			{
				memset(&g_login_user,0,sizeof(_T_ASS_LOGIN_USER_INFO_));
				nResult = ParseLoginInfo(&g_login_user,strXml);
				int user_id=0;
				int userflag=1;
				//登陆
				if (nResult == 0)
				{
//////////////////////////////////////////////////////////////////////////
// 					g_sRVUData[0].rvu_id=14;
// 					g_sRVUData[0].station_id=1;
// 					g_sRVUData[0].count=2;
// 					g_sRVUData[0].t_datas[0].id.SmID=1;
// 					g_sRVUData[0].t_datas[0].id.NodeID=1;
// 					g_sRVUData[0].t_datas[0].type=1;
// 					g_sRVUData[0].t_datas[0].t_value=0.000000;
// 					g_sRVUData[0].t_datas[0].enumstate=0;
// 					g_sRVUData[0].t_datas[1].id.SmID=1;
// 					g_sRVUData[0].t_datas[1].id.NodeID=2;
// 					g_sRVUData[0].t_datas[1].type=1;
// 					g_sRVUData[0].t_datas[1].t_value=1.000000;
// 					g_sRVUData[0].t_datas[1].enumstate=0;
// 					rvu_num=1;
					
//////////////////////////////////////////////////////////////////////////
					user_id = cmpLogin(&g_login_user);
					//登陆成功
					if (user_id>0)
					{
						for(int i=0;i<usernum;i++)
						{
							if (c_login_user[i].userid==user_id)
							{
								c_login_user[i].handle=handle;
								c_login_user[i].login_flag=1;
								c_login_user[i].stationID=0;
								c_login_user[i].alarmstationNum=0;
								c_login_user[i].devidNum=0;
								userflag=0;
							}
							
						}
						if (userflag==1)
						{
							c_login_user[usernum].userid=user_id;
							c_login_user[usernum].handle=handle;
							c_login_user[usernum].login_flag=1;
							usernum++;
						}
						ComDispatchTask(&g_user_login,GetUserInfo,&user_id,sizeof(int),0);
						
					}
					
				}
				//登出
				else if (nResult==1)
				{
					user_id = cmpLogin(&g_login_user);
					if (user_id>0)
					{
						for(int i=0;i<usernum;i++)
						{
							if (c_login_user[i].userid==user_id)
							{
								c_login_user[i].handle=handle;
								c_login_user[i].login_flag=0;
							}
							
						}
					}
				}
				
				if (user_id>0)
				{
					//操作成功
					sendXml=new char[256];
					sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
						"<HTTP_XML EventType=\"Response_Login\">"
						"<Item Result=\"0\" />"
						"</HTTP_XML>");
					
				}
				else
				{
					//操作失败
					sendXml=new char[256];
					sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
						"<HTTP_XML EventType=\"Response_Login\">"
						"<Item Result=\"1\" />"
						"</HTTP_XML>");
					
				}
				
				ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
				delete sendXml;
				sendXml=NULL;
			}
			break;
	
		case ASS_KEEPALIVE:
			nResult = ParseKeepAlive(strXml,sendtime);
			sendXml=new char[256];
			sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
				"<HTTP_XML EventType=\"Response_BeatHeart\">"
						"</HTTP_XML>");
			ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
			delete sendXml;
			sendXml=NULL;
			g_nLostPacketNum = 0;
			break;
		case ASS_REQUEST_YK_CONTROL:
			memset(&_g_control,0,sizeof(_T_ASS_CONTROL_));
			nResult = ParseYKControl(&_g_control,strXml);
			break;
		case ASS_REQUEST_STATION:
			{
				memset(&_g_station,0,sizeof(_T_ASS_STATION_INFO_));
				nResult = ParseStationInfo(&_g_station,strXml);

				int loginflag=1;
				for (int i=0;i<usernum;i++)
				{
					if ((nResult==0)&&(c_login_user[i].userid==_g_station.user_id)&&(c_login_user[i].login_flag==1))
					{
						printf("\n c_log is %d  %d",c_login_user[i].userid,_g_station.user_id);
						loginflag=0;
						c_login_user[i].stationID=_g_station.station_id;
						for (int j=0;j<rvu_num;j++)
						{
							if (g_sRVUData[j].station_id==_g_station.station_id)
							{

								sendXml=new char[g_sRVUData[j].count*128+128];
								sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
									"<HTTP_XML EventType=\"Response_Push_Data\">");
								char add_XML[128];
								
								for(int k=0;k<g_sRVUData[j].count;k++)
								{
									sprintf(add_XML,"<Item RvuID=\"%d\" SmID=\"%d\" NodeID=\"%d\" Value=\"%f\" Type=\"%d\" Status=\"%d\" />",
										g_sRVUData[j].rvu_id,g_sRVUData[j].t_datas[k].id.SmID,g_sRVUData[j].t_datas[k].id.NodeID,
										g_sRVUData[j].t_datas[k].t_value,g_sRVUData[j].t_datas[k].type,g_sRVUData[j].t_datas[k].enumstate);
									strcat(sendXml,add_XML);
								}
								strcat(sendXml,"</HTTP_XML>");
						//		printf("\n sendXml is %s",sendXml);
								ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
								delete sendXml;
								sendXml=NULL;
							}
						}
					}
				}
				if ((loginflag==1)||(nResult!=0))
				{
					sendXml=new char[256];
					sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
						"<HTTP_XML EventType=\"Response_Subscribe_Station\">"
						"<Item Result=\"1\" />"
						"</HTTP_XML>");
					ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
					delete sendXml;
					sendXml=NULL;
				}
			}
			break;
		case ASS_REQUEST_ALARM:
			{
				memset(&_g_alarm,0,sizeof(_T_ASS_ALARM_SET_));
				nResult = ParseAlarm(&_g_alarm,strXml);
				int alarmflag=1;
				char szSQL[256]={0x0};
				//增加
				if (nResult==1)
				{
					int alarmnum=0;
					for (int i=0;i<usernum;i++)
					{
						if (c_login_user[i].userid==_g_alarm.user_id)
						{
							alarmnum=c_login_user[i].alarmstationNum;//获取当前告警订阅的站点数量
							alarmflag=0;
							for (int j=0;j<c_login_user[i].alarmstationNum;j++)
							{
								if (c_login_user[i].AlarmStation[j]==_g_alarm.station_id[0])
								{
									alarmflag=1;
								}
							}
							if (alarmflag==1)
							{
								break;
							}
	//						memset(c_login_user[i].AlarmStation,0,MAX_RVU_DEVICE_NUM_PER_STATION*sizeof(int));
							for (int j=0;j<_g_alarm.num;j++)
							{
								if (_g_alarm.station_id[j]>0)
								{
									c_login_user[i].AlarmStation[alarmnum]=_g_alarm.station_id[j];
									alarmnum++;
									sprintf(szSQL,"insert into ass_alarm_subscribe (user_id,station_id) values (%d,%d)",c_login_user[i].userid,_g_alarm.station_id[j]);
									ComDispatchTask(&g_sql_insert,SqlInsert,&szSQL,sizeof(szSQL),0);
								}

							}
							c_login_user[i].alarmstationNum=alarmnum;
							sendXml=new char[256];
							sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<HTTP_XML EventType=\"Response_Subscribe_Alarm\">"
								"<Item Result=\"0\" />"
								"</HTTP_XML>");
							ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
							delete sendXml;
							sendXml=NULL;
						}
					}
				
				}
				//删除
				else if (nResult==0)
				{
					int alarmnum=0;
					for (int i=0;i<usernum;i++)
					{
						if (c_login_user[i].userid==_g_alarm.user_id)
						{
							alarmnum=c_login_user[i].alarmstationNum;//获取当前告警订阅的站点数量
							alarmflag=0;
	//						memset(c_login_user[i].AlarmStation,0,MAX_RVU_DEVICE_NUM_PER_STATION*sizeof(int));
							for (int j=0;j<c_login_user[i].alarmstationNum;j++)
							{
								if (c_login_user[i].AlarmStation[j]==_g_alarm.station_id[0])
								{
									c_login_user[i].AlarmStation[j]=0;
									alarmnum--;
									sprintf(szSQL,"delete from ass_alarm_subscribe where user_id=%d and station_id=%d",c_login_user[i].userid,_g_alarm.station_id[0]);
									ComDispatchTask(&g_sql_insert,SqlInsert,&szSQL,sizeof(szSQL),0);
								}
							}

							ArrayArrange(c_login_user[i].AlarmStation,c_login_user[i].alarmstationNum);
							c_login_user[i].alarmstationNum=alarmnum;
							sendXml=new char[256];
							sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<HTTP_XML EventType=\"Response_Subscribe_Alarm\">"
								"<Item Result=\"0\" />"
								"</HTTP_XML>");
							ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
							delete sendXml;
							sendXml=NULL;
						}
					}
				}

				if ((alarmflag==1)||(nResult<0))
				{
					sendXml=new char[256];
					sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
						"<HTTP_XML EventType=\"Response_Subscribe_Alarm\">"
						"<Item Result=\"1\" />"
						"</HTTP_XML>");
					ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
					delete sendXml;
					sendXml=NULL;
				}
				
			}
			break;
		case ASS_REQUEST_HIDE:
			//////////////////////////////////////////////////////////////////////////
			alarmupdate=1;
			dateupdate=1;
			//////////////////////////////////////////////////////////////////////////
			memset(&_g_hide,0,sizeof(_T_ASS_HIDE_SET_));
			nResult = ParseHide(&_g_hide,strXml);
			{
				int hideflag=1;
				char szSQL[256]={0x0};
				printf("\n result is %d",nResult);
				//增加
				if (nResult==1)
				{
					int hidenum=0;
					for (int i=0;i<usernum;i++)
					{
						if (c_login_user[i].userid==_g_hide.user_id)
						{
							hidenum=c_login_user[i].devidNum;
							hideflag=0;
							for (int j=0;j<c_login_user[i].devidNum;j++)
							{
								if (c_login_user[i].devID[j]==_g_hide.devID[0])
								{
									hideflag=1;
								}
							}
							if (hideflag==1)
							{
								break;
							}
//							memset(c_login_user[i].devID,0,MAX_RVU_DEVICE_NUM_PER_STATION*sizeof(int));
							for (int j=0;j<_g_hide.num;j++)
							{
								if (_g_hide.devID[j]>0)
								{
									c_login_user[i].devID[hidenum]=_g_hide.devID[j];
									hidenum++;
									sprintf(szSQL,"insert into ass_alarm_hide (user_id,dev_id) values (%d,%d)",c_login_user[i].userid,_g_hide.devID[j]);
									ComDispatchTask(&g_sql_insert,SqlInsert,&szSQL,sizeof(szSQL),0);
								}
							}
							c_login_user[i].devidNum=hidenum;
							sendXml=new char[256];
							sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<HTTP_XML EventType=\"Response_Hide_Dev\">"
								"<Item Result=\"0\" />"
								"</HTTP_XML>");
							ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
							delete sendXml;
							sendXml=NULL;
						}
					}
				}
				//删除
				else if (nResult==0)
				{
					int hidenum=0;
					for (int i=0;i<usernum;i++)
					{
						if ((c_login_user[i].userid==_g_hide.user_id))
						{
							hidenum=c_login_user[i].devidNum;
							hideflag=0;
//							memset(c_login_user[i].devID,0,MAX_RVU_DEVICE_NUM_PER_STATION*sizeof(int));
							for (int j=0;j<c_login_user[i].devidNum;j++)
							{
								if (c_login_user[i].devID[j]==_g_hide.devID[0])
								{
									c_login_user[i].devID[j]=0;
									hidenum--;
									sprintf(szSQL,"delete from ass_alarm_hide where user_id=%d and dev_id=%d",c_login_user[i].userid,_g_hide.devID[0]);
									ComDispatchTask(&g_sql_insert,SqlInsert,&szSQL,sizeof(szSQL),0);
								}
							
							}
							ArrayArrange(c_login_user[i].devID,c_login_user[i].devidNum);
							c_login_user[i].devidNum=hidenum;
							sendXml=new char[256];
							sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
								"<HTTP_XML EventType=\"Response_Hide_Dev\">"
								"<Item Result=\"0\" />"
								"</HTTP_XML>");
							ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
							delete sendXml;
							sendXml=NULL;
						}
					}
				}
			
				if ((hideflag==1)||(nResult<0))
				{
					sendXml=new char[256];
					sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
						"<HTTP_XML EventType=\"Response_Hide_Dev\">"
						"<Item Result=\"1\" />"
						"</HTTP_XML>");
					ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
					delete sendXml;
					sendXml=NULL;
				}
			}
			break;
		case ASS_REQUEST_TIME:
			nResult = ParseTimeSet(strXml,sendtime);
			{
				time_t t;
				struct tm * timeinfo;
				time(&t);
				timeinfo=localtime(&t);
				char t_time[21];
				sprintf(t_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
					timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
				
				sendXml=new char[256];
				sprintf(sendXml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
					"<HTTP_XML EventType=\"Response_Clock\">"
					"<Item time=\"%s\" />"
					"</HTTP_XML>",t_time);
				ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,sendXml,strlen(sendXml),0);
				delete sendXml;
				sendXml=NULL;
			}
			break;
		default:
			break;
		}
	}
	catch(...)
	{

	}
}

int ParseYKControl(_T_ASS_CONTROL_ *g_asscontrol,const char* strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;

	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;

	int nResult = -1;

	try
	{
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return nResult;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return nResult;

		if (strcmp(pResult, "Request_Control_Dev") != 0) 
			return nResult;

		pItem = pElement->FirstChildElement("Item");
		if (pItem == NULL)
			return nResult;


		pResult = pItem->Attribute("RvuID");
		if (pResult != NULL)
		{
			g_asscontrol->rvuid = atoi(pResult);
		}
		pResult = pItem->Attribute("SmID");
		if (pResult != NULL)
		{
			g_asscontrol->smid = atoi(pResult);
		}
		pResult = pItem->Attribute("NodeID");
		if (pResult != NULL)
		{
			g_asscontrol->nodeid = atoi(pResult);
		}
		pResult = pItem->Attribute("Value");
		if (pResult != NULL)
		{
			g_asscontrol->value = atoi(pResult);
		}

		pResult = pItem->Attribute("Type");
		if (pResult != NULL)
		{
			g_asscontrol->type = atoi(pResult);
		}

		return 0;
	}
	catch (...)
	{

	}

	return nResult; 
}

int ParseKeepAlive(const char* strXML,char* sendtime)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;

	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;

	int nResult = -1;

	try
	{
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return nResult;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return nResult;

		if (strcmp(pResult, "Request_BeatHeart") != 0) 
			return nResult;

		if (pResult != NULL)
		{
			strcpy(sendtime,gettime());
		}

		return 0;
	}
	catch (...)
	{

	}

	return nResult;	
}

int ParseTimeSet(const char* strXML,char* sendtime)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	
	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;
	
	int nResult = -1;
	
	try
	{
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return nResult;
		
		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return nResult;
		
		if (strcmp(pResult, "Request_Clock") != 0) 
			return nResult;
		
		if (pResult != NULL)
		{
			strcpy(sendtime,gettime());
		}
		
		return 0;
	}
	catch (...)
	{
		
	}
	
	return nResult;	
}
//登陆信息解析
int ParseLoginInfo(_T_ASS_LOGIN_USER_INFO_* pAssLoginInfo,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;

	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;
	int type=0;
	int i=0;

	if (pAssLoginInfo == NULL||strXML == NULL)
		return -1;

	try
	{
		memset(pAssLoginInfo,0,sizeof(_T_ASS_LOGIN_USER_INFO_));

		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;

		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;

		if (strcmp(pResult, "Request_Login") != 0) 
			return -1;

		pItem = pElement->FirstChildElement("Item");
		if (pItem == NULL)
			return -1;

		pResult = pItem->Attribute("UserName");
		if (pResult != NULL)
		{
		
			strcpy(pAssLoginInfo->rvu_username,pResult);
			printf("\n username is %s",pAssLoginInfo->rvu_username);
		}

		pResult = pItem->Attribute("Password");
		if (pResult != NULL)
		{
	
			strcpy(pAssLoginInfo->rvu_password, pResult);
			printf("\n password is %s",pAssLoginInfo->rvu_password);
		}

		pResult = pItem->Attribute("Type");
		if (pResult != NULL)
		{
			type = atoi(pResult);
		}
		
		return type;		
	
	}
	catch (...)
	{

	}

	return -1;	
}
//变电站实时消息订阅
int ParseStationInfo(_T_ASS_STATION_INFO_* pAssstation,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement *pItem = NULL;
	TiXmlElement    *pSubList = NULL;
	const char		*pResult = NULL;
	
	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;
	int type=0;
	int i=0;
	
	if (pAssstation == NULL||strXML == NULL)
		return -1;
	
	try
	{
		memset(pAssstation,0,sizeof(_T_ASS_STATION_INFO_));
		
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;
		
		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;
		
		if (strcmp(pResult, "Request_Subscribe_Station") != 0) 
			return -1;
		
	
		for(pItem = pElement->FirstChildElement("Item"); pItem != NULL; pItem = pItem->NextSiblingElement())
		{
			pResult = pItem->Attribute("UserID");
			if (pResult != NULL)
			{
				
				pAssstation->user_id=atoi(pResult);
				printf("\n userid is %d",pAssstation->user_id);
			}
			
			pResult = pItem->Attribute("StationID");
			if (pResult != NULL)
			{
				
				pAssstation->station_id=atoi(pResult);
				printf("\n station id is %d",pAssstation->station_id);
			}
		}
		
		return 0;
		
		
	}
	catch (...)
	{
		
	}
	
	return -1;	
}
//告警订阅
int ParseAlarm(_T_ASS_ALARM_SET_* pAssalarm,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement    *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;

	
	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;
	int type=0;
	int i=0;
	int num=0;
	
	if (pAssalarm == NULL||strXML == NULL)
		return -1;
	
	try
	{
		memset(pAssalarm,0,sizeof(_T_ASS_ALARM_SET_));
		
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;
		
		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;
		
		if (strcmp(pResult, "Request_Subscribe_Alarm") != 0) 
			return -1;
		
		for(pItem = pElement->FirstChildElement("Item"); pItem != NULL; pItem = pItem->NextSiblingElement())
		{
			pResult = pItem->Attribute("UserID");
			if (pResult != NULL)
			{
				
				pAssalarm->user_id=atoi(pResult);
				printf("\n userid is %d",pAssalarm->user_id);
			}
			
			pResult = pItem->Attribute("StationID");
			if (pResult != NULL)
			{
				
				pAssalarm->station_id[i]=atoi(pResult);
				pAssalarm->num++;
				printf("\n station id is %d",pAssalarm->station_id[i]);
			}

			pResult = pItem->Attribute("Type");
			if (pResult != NULL)
			{
				
				type=pAssalarm->type=atoi(pResult);

				printf("\n station id is %d",pAssalarm->station_id[i]);
			}
			i++;
		}
			return type;
		
	}
	catch (...)
	{
		
	}
	
	return -1;	
}
//设备屏蔽
int ParseHide(_T_ASS_HIDE_SET_* pAssHide,const char *strXML)
{
	TiXmlDocument	resXML;
	TiXmlElement	*pElement = NULL;
	TiXmlElement    *pSubList = NULL;
	TiXmlElement *pItem = NULL;
	const char		*pResult = NULL;
	
	
	char szGB2312Info[256] = {0};
	int    nGB2312InfoLen = 0;
	int type=0;
	int i=0;
	int num=0;
	
	if (pAssHide == NULL||strXML == NULL)
		return -1;
	
	try
	{
		memset(pAssHide,0,sizeof(_T_ASS_HIDE_SET_));
		
		resXML.Parse(strXML);
		pElement = resXML.FirstChildElement("HTTP_XML");
		if (pElement == NULL)
			return -1;
		
		pResult = pElement->Attribute("EventType");
		if (pResult == NULL)
			return -1;
		
		if (strcmp(pResult, "Request_Hide_Dev") != 0) 
			return -1;
		
		for(pItem = pElement->FirstChildElement("Item"); pItem != NULL; pItem = pItem->NextSiblingElement())
		{
			pResult = pItem->Attribute("UserID");
			if (pResult != NULL)
			{
				
				pAssHide->user_id=atoi(pResult);
				printf("\n userid is %d",pAssHide->user_id);
			}
			
			pResult = pItem->Attribute("DevID");
			if (pResult != NULL)
			{
				
				pAssHide->devID[i]=atoi(pResult);
				pAssHide->num++;
				printf("\n station id is %d",pAssHide->devID[i]);
			}
			pResult = pItem->Attribute("Hide");
			if (pResult != NULL)
			{
				
				type=pAssHide->hide=atoi(pResult);
				printf("\n hide is %d",pAssHide->hide);
			}
			i++;
		}
		
		return type;
		
	}
	catch (...)
	{
		
	}
	
	return -1;	
}



