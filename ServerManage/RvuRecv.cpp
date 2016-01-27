#include "StdAfx.h"

#include "RvuRecv.h"
#include "DataBaseExcute.h"
#include "Rvu104Recv.h"

extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;

extern COMMONLIST	g_rvu_sendlist;
extern COMMONTHREADPOOL g_parse_rvu_thread;

extern COMMONTHREADPOOL g_db_smthread;
extern COMMONTHREADPOOL g_db_nodethread;
extern COMMONTHREADPOOL g_db_datathread;
extern COMMONTHREADPOOL g_db_alarmthread;

RECV_PACKET g_recv_packet[MAX_ONTHREAD_NUM];
int handlenum=0;
extern DBConfigParam g_DBConfigParam;

SEND_RVU_INFO g_sRVUInfo[MAX_RVU_NUM_PER_STATION];
RVU_NODE_INFO g_sNODEInfo[MAX_RVU_DEVICE_NUM_PER_STATION];
int nodenum=0;

T_RVU_DATA g_sRVUData[MAX_RVU_DEVICE_NUM_PER_STATION];
int rvu_num=0;

int dateupdate=0;
int alarmupdate=0;

int anfanghandle=0;//安防协议连接句柄

int getMaxnum=0;//获取最大数据
int getNnum=0;//当前获取数据数

bool g_bRecvComplete = false;//接收完一个rcu数据标志

//T_RVU_ALARM g_sRVUalarm[MAX_RVU_DEVICE_NUM_PER_STATION];


void RvuConnect(void* arg)
{
	int listindex=0;
	int i=0;
	// 查询g_AccessDevList，得到当前有几个设备是通过串口1进行获取数据的
	T_PRO t_pro;
	BYTE l_byte[1452];
	int lenth=0;
	int result=0;
	int size=0;
	int datalen=0;
	int losttime=0;

	JS_SYS js_sys;



	while(1)
	{
		
//		ComGetListNode(&g_AccessDevList, TraversingListNetInit, (void*)nSerialPort, (void**)(&tAccessDev), &listindex);
		for(i=0;i<g_nAssRVUNum;i++)
		{
			if (g_tAssRVU[i].commnet_handle<=0)
			{
				
			
				if (strcmp(g_tAssRVU[i].rvu_type,"JIANGSU")==0)//江苏协议
				{
// 					printf("\n JIANGSU");
// 					g_tAssRVU[i].commnet_handle  = ComOpenNet(g_DBConfigParam.LocalIP,0,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,NETCLIENT,TCP,DEFAULT_TCP_CLIENT_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE,10,-1,1,recv_parse,NULL,10,0);
// 					//登陆命令发送
// 					if (g_tAssRVU[i].commnet_handle>0)
// 					{
// 						JS_LOGIN js_login;
// 						memcpy(js_login.username,g_tAssRVU[i].rvu_username,NAMELENGTH);
// 						memcpy(js_login.password,g_tAssRVU[i].rvu_password,PASSWORDLEN);
// 						//  					strcpy(js_login.username,"admin                                   ");
// 						//  					strcpy(js_login.password,"123                 ");
// 						js_login.memberID=0;
// 						js_login.enumlog=1;//登陆
// 						memset(l_byte,0,1452);
// 						lenth=0;
// 						RVU_SEND_JsLogin(js_login,l_byte,&lenth);
// 						
// 						TxProtocol(&t_pro,REQUEST_RVU_LOGIN,l_byte,lenth);
// 						size=lenth+28;
// 						BYTE * s_buf=(BYTE*)&t_pro;
// 						result=ComSendNet(g_tAssRVU[i].commnet_handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)s_buf,size,0);
// 						
// 						memset(l_byte,0,1452);
// 						lenth=0;
// 						//发送系统结构获取
// 						g_tAssRVU[i].login_flag=1;
// 						js_sys.cnt=1;
// 						js_sys.tids[0].SmID=(short)65535;
// 						js_sys.tids[0].NodeID=0;
// 						RVU_SEND_JsSys(js_sys,l_byte,&lenth);
// 						TxProtocol(&t_pro,REQUEST_RVU_GET_SM,l_byte,lenth);				
// 						size=lenth+28;
// 						BYTE * s_buf1=(BYTE*)&t_pro;
// 						ComSendNet(g_tAssRVU[i].commnet_handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)s_buf1,size,0);
// 						memset(l_byte,0,1452);
// 						
						
//					}
				}
				else if (strcmp(g_tAssRVU[i].rvu_type,"HIK")==0)//海康协议
				{
					printf("\n HIK");
				}
				else if (strcmp(g_tAssRVU[i].rvu_type,"ANFANG")==0)//安防协议
				{
					printf("\n ANFANG");
					if (anfanghandle==0)
					{
						anfanghandle=ComOpenNet(g_DBConfigParam.LocalIP,0,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,NETCLIENT,TCP,DEFAULT_TCP_CLIENT_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE,10,-1,1,recv_104_parse,NULL,10,0);
			
					}
					//连接成功
					if (anfanghandle>0)
					{
						printf("\n 安防服务器连接成功");
						g_tAssRVU[i].commnet_handle=anfanghandle;
						//结构获得
// 						_T_104_BUFF *sendBuff=new _T_104_BUFF;
// 						datalen=32;
// 						lenth=9+datalen;
// 						sendBuff->head[0]=0x68;//报头
// 						sendBuff->bufflen[0]=lenth;
// 						sendBuff->bufflen[1]=lenth>>8;//报文长度
// 						sendBuff->sendflag[0]=0;//接入方发给被接入方
// 						sendBuff->funcode[0]=0x07;//功能码 结构
// 						sendBuff->funcode[1]=0x00;//功能码
// 						sendBuff->datalen[0]=datalen;//数据内容长度
// 						sendBuff->datalen[1]=datalen>>8;
// 						
// 						memcpy(sendBuff->data,g_tAssRVU[i].rvu_code,datalen);
// 						sendBuff->end[0]=0x16;
// 						
// 						result=ComSendNet(anfanghandle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)sendBuff,lenth,0);
// 						printf("\n result is %d",result);
// 						
// 						delete sendBuff;
			
						
//						Sleep(1000);

					}

				
				}
			

			}
			else
			{
				printf("\n rvu is %d,handle is %d",i,g_tAssRVU[i].commnet_handle);
				if (strcmp(g_tAssRVU[i].rvu_type,"JIANGSU")==0)//江苏协议
				{
// 					printf("\n JIANGSU");
// 					//心跳检测
// 					memset(l_byte,0,1452);
// 					lenth=0;
// 					RVU_SEND_CONN(l_byte);
// 					TxProtocol(&t_pro,HEART_BEAT,l_byte,lenth);				
// 					size=lenth+28;
// 					BYTE * s_buf=(BYTE*)&t_pro;
// 					result=ComSendNet(g_tAssRVU[i].commnet_handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)s_buf,size,0);
// 					if (result<0)
// 					{
// 						//					g_tAssRVU[i].commnet_handle  = ComOpenNet("127.0.0.1",0,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,NETCLIENT,TCP,DEFAULT_TCP_CLIENT_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_RECV_BUF_SIZE,DEFAULT_TCP_CLIENT_SOCKET_SEND_BUF_SIZE,10,-1,10,recv_parse,NULL,10,0);		
// 						g_tAssRVU[i].commnet_handle=0;
// 					}
				}
				else if (strcmp(g_tAssRVU[i].rvu_type,"HIK")==0)//海康协议
				{
					printf("\n HIK");
				}
				else if (strcmp(g_tAssRVU[i].rvu_type,"ANFANG")==0)//安防协议
				{
					printf("\n ANFANG");
// 					//心跳检测
// 					_T_104_BUFF *t_buff=new _T_104_BUFF;
// 					datalen=32;
// 					lenth=9+datalen;
// 					t_buff->head[0]=0x68;//报头
// 					t_buff->bufflen[0]=lenth;
// 					t_buff->bufflen[1]=lenth>>8;//报文长度
// 					t_buff->sendflag[0]=0;//接入方发给被接入方
// 					t_buff->funcode[0]=0x05;//功能码 心跳
// 					t_buff->funcode[1]=0x00;//功能码
// 					t_buff->datalen[0]=datalen;//数据内容长度
// 					t_buff->datalen[1]=datalen>>8;
// 					t_buff->data=new BYTE[datalen];
// 					memcpy(t_buff->data,g_tAssRVU[i].rvu_code,datalen);
// 					t_buff->end[0]=0x16;
// 					
// 					ComSendNet(g_tAssRVU[i].commnet_handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)t_buff,lenth,0);
// 					
// 					g_tAssRVU[i].lost_heatbeat++;
// 
// 					//超时自动断开
// 					if (g_tAssRVU[i].lost_heatbeat>2)
// 					{
// 						g_tAssRVU[i].commnet_handle=0;
// 						ComCloseNet(g_tAssRVU[i].commnet_handle,1);
// 					}
// 
// 					delete t_buff->data;
// 					delete t_buff;

						//总招发送
					_T_104_BUFF *sendBuff=new _T_104_BUFF;
					datalen=32;
					lenth=9+datalen;
					sendBuff->head[0]=0x68;//报头
					sendBuff->bufflen[0]=lenth;
					sendBuff->bufflen[1]=lenth>>8;//报文长度
					sendBuff->sendflag[0]=0;//接入方发给被接入方
					sendBuff->funcode[0]=0x04;//功能码 总招
					sendBuff->funcode[1]=0x00;//功能码
					sendBuff->datalen[0]=datalen;//数据内容长度
					sendBuff->datalen[1]=datalen>>8;
					
					memcpy(sendBuff->data,g_tAssRVU[i].rvu_code,datalen);
					sendBuff->end[0]=0x16;
					
					ComSendNet(g_tAssRVU[i].commnet_handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)sendBuff,lenth,0);
					
					delete sendBuff;
								//判断是否采集完一个RCU的数据
					while(1)
					{
						//判断是否接收完一个rcu数据，接收完，发送获取下一个rcu数据
						if (g_bRecvComplete)
						{
							g_bRecvComplete = false;
							break;
						}
						/*
						if((getMaxnum==getNnum)&&(getMaxnum>0))
						{
							getMaxnum=0;
							getNnum=0;
							Sleep(1000);
							break;
						}
						*/
						Sleep(1000);
						
						losttime++;
						if((getMaxnum==0)&&(losttime>5))
						{
							losttime=0;
							break;
						}
						
					}

				}
			
			}
		
		}
		Sleep(3*1000);
	}

}


void ParseRVURecvBuf(void* arg)
{
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
//	unsigned char temp_buf[1024*32] = {0x0};

	JS_SYS js_sys;
	T_PRO t_pro;
	int lenth=0;
	BYTE l_byte[1452];
	
	RECV_PACKET* pRecvPacket = (RECV_PACKET*)arg;
	
	if (pRecvPacket == NULL)
	{
		return;
	}
	BYTE* pBuff = (BYTE*)pRecvPacket->recvbuf;
	int size = pRecvPacket->size;
	int rvuid=0;
	char rvuip[16];
	int rvuport=0;

// 接收数据处理
	int i = 0;
	int j = 0;
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
			case RESPONSE_RVU_LOGIN://登陆信息接收
				if (ParseRVUSendLogin(pRecvPacket->handle,pBuff,size)>0)
				{
// 					for (i=0;i<g_nAssRVUNum;i++)
// 					{
// 						if (g_tAssRVU[i].commnet_handle==pRecvPacket->handle)
// 						{
// 							//发送系统结构获取
// 							g_tAssRVU[i].login_flag=1;
// 							js_sys.cnt=1;
// 							js_sys.tids[0].SmID=(short)0xffff;
// 							js_sys.tids[0].SmID=0;
// 							RVU_SEND_JsSys(js_sys,l_byte,&lenth);
// 							TxProtocol(&t_pro,REQUEST_RVU_GET_SM,l_byte,lenth);				
// 							size=lenth+28;
// 							BYTE * s_buf=(BYTE*)&t_pro;
// 							ComSendNet(pRecvPacket->handle,g_tAssRVU[i].rvu_ip,g_tAssRVU[i].rvu_port,0,0,0,0,(char *)s_buf,size,0);
// 							memset(l_byte,0,1452);
// 
// 						}
// 					}
				}
				break;
			case RESPONSE_RVU_SET_SM://系统结构接收 SM
				printf("\n 系统结构接收");
				for (j=0;j<g_nAssRVUNum;j++)
				{
					if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
					{
						rvuid=g_tAssRVU[j].rvu_id;
						memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
						rvuport=g_tAssRVU[j].rvu_port;
					}
				}
				if (ParseRVUSendSM(pRecvPacket->handle,pBuff,size,rvuid)>0)
				{
					
					js_sys.cnt=1;
					js_sys.tids[0].SmID=(short)65535;
					js_sys.tids[0].NodeID=(short)65535;
					RVU_SEND_JsSys(js_sys,l_byte,&lenth);
					TxProtocol(&t_pro,REQUEST_RVU_GET_NODES,l_byte,lenth);				
					size=lenth+28;
					BYTE * s_buf=(BYTE*)&t_pro;
					ComSendNet(pRecvPacket->handle,rvuip,rvuport,0,0,0,0,(char *)s_buf,size,0);
					memset(l_byte,0,1452);
			
				}
				
				break;
			case RESPONSE_RVU_SET_NODES://系统结构接收 NODE
				printf("\n 系统节点接收");
				for(j=0;j<g_nAssRVUNum;j++)
				{
					if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
					{
						rvuid=g_tAssRVU[j].rvu_id;
						memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
						rvuport=g_tAssRVU[j].rvu_port;
					}
				}
			
				if (ParseRVUSendNODE(pRecvPacket->handle,pBuff,size,rvuid)>0)
				{
					JS_REAL js_real;
					for (j=0;j<nodenum;j++)
					{
						if (g_sNODEInfo[i].rvu_handle==pRecvPacket->handle)
						{
// 							js_real.cnt=g_sNODEInfo[i].count;
// 							js_real.mode=1;
// 							js_real.pollingtime=1;
// 							int j=0;
// 							for (j=0;j<js_real.cnt;j++)
// 							{
// 								js_real.tids[j].SmID=g_sNODEInfo[i].tids[j].sm_id;
// 								js_real.tids[j].NodeID=g_sNODEInfo[i].tids[j].node_id;
// 							}

							js_real.cnt=1;
							js_real.mode=1;
							js_real.pollingtime=1;
						
							js_real.tids[0].SmID=(short)65535;
							js_real.tids[0].NodeID=(short)65535;
					
						}
					}
					//发送实时信息统招指令
					RVU_SEND_JsReal(js_real,l_byte,&lenth);
					TxProtocol(&t_pro,REQUEST_RVU_SET_DYN_ACCESS_MODE,l_byte,lenth);				
					size=lenth+28;
					BYTE * s_buf=(BYTE*)&t_pro;
					ComSendNet(pRecvPacket->handle,rvuip,rvuport,0,0,0,0,(char *)s_buf,size,0);
					memset(l_byte,0,1452);

				}
				break;
			case RESPONSE_RVU_DYN_ACCESS_MODE_ACK://接收实时信息
				dateupdate=1;
				alarmupdate=1;
				printf("\n 实时信息接收");
				for(j=0;j<g_nAssRVUNum;j++)
				{
					if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
					{
						rvuid=g_tAssRVU[j].rvu_id;
						memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
						rvuport=g_tAssRVU[j].rvu_port;
					}
				}
				ParseRVUSendREAL(pRecvPacket->handle,pBuff,size,rvuid);
				break;

// 			case SEND_REALTIME_ALARM://接收告警信息
// 				for(i=0;i<g_nAssRVUNum;i++)
// 				{
// 					if (g_tAssRVU[i].commnet_handle==pRecvPacket->handle)
// 					{
// 						rvuid=g_tAssRVU[i].rvu_id;
// 						memcpy(rvuip,g_tAssRVU[i].rvu_ip,16);
// 						rvuport=g_tAssRVU[i].rvu_port;
// 					}
// 				}
// 				ParseRVUSendRealtimeAlarm(pRecvPacket->handle,pBuff,size,rvuid);
// 				break;
		
			default:
				break;
			}
			break;
		}
		i++;
	}


 //   ComInsertListNode(&g_rvu_sendlist, (void*)(tAccessDev), &listindex);

	
	
//	memset(&g_recv_packet,0,sizeof(RECV_PACKET));
}
// 发送处理
int SendMsgPatch(void *s1,void *s2)
{
	int i=0;
	for (i=0;i<g_nAssRVUNum;i++)
	{
		if (g_tAssRVU[i].commnet_handle==g_sRVUInfo->rvu_handle)
		{
			
			ComSendNet(g_sRVUInfo->rvu_handle,g_sRVUInfo->rvu_ip,g_sRVUInfo->rvu_port,0,0,0,0,(char *)g_sRVUInfo->send_buff,g_sRVUInfo->send_size,0);	
			
		}
	}
	
	return 1;
}
// 信息发送线程
void SendRVUBuf(void* arg)
{

	SEND_RVU_INFO *Send_rvu_info=NULL;
	int listindex=0;
	while(1)
	{
		ComGetListNode(&g_rvu_sendlist, SendMsgPatch, (void*)g_tAssRVU, (void**)(Send_rvu_info), &listindex);
	}
}
//登陆信息解析
int ParseRVUSendLogin(int handle,unsigned char* buff,int size)
{
	//	TRACE("-----size=%d------\n\n",size);
	
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
	JS_RELOGIN *js_relogin=NULL;
	BYTE yh_buff[RECVBUFSIZE];
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
			
		
				js_relogin = new JS_RELOGIN;
				memset(js_relogin,0,sizeof(JS_RELOGIN));
				memcpy(yh_buff,&buff[i+SET_SM_ACK_DATA_OFFSET-4],sizeof(JS_RELOGIN));
				RVU_RECV_JsLogin(yh_buff,js_relogin);
			
			}
			break;
		}
		
		i++;
	}

	
	//   资源回收
	if (js_relogin != NULL)
	{
		delete js_relogin;
		js_relogin = NULL;
	}
	
	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
	if (js_relogin->MemberLevel>0)
	{
		return 1;		
	}
	return 0;
}
//系统结构信息解析SM
int ParseRVUSendSM(int handle,unsigned char* buff,int size,int rvuid)
{
	//	TRACE("-----size=%d------\n\n",size);
	
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
	T_SM* pTsm=NULL;
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
					//	TRACE("----查不到响应ID号------\n");
					return 0;
				}
				else if (count == -1)
				{
					//	TRACE("----报文内容过长------\n");
					return 0;
				}
				
				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}
				
				data_len = first_len - SET_SM_ACK_DATA_OFFSET;
				
				if (reserved_index == 0)
				{
					pTsm = new T_SM[count];
					memset(pTsm,0,sizeof(T_SM)*count);
					memcpy(pTsm,&buff[i+SET_SM_ACK_DATA_OFFSET],sizeof(T_SM)*count);
					break;
				}
				else
				{
					memcpy(/*&data*/pData,&buff[i+SET_SM_ACK_DATA_OFFSET],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}
					
					pTsm = new T_SM[count];
					memset(pTsm,0,sizeof(T_SM)*count);
					memcpy(pTsm,pData,sizeof(T_SM)*count);
				}
			}
			break;
		}
		
		i++;
	}


	for (int j=0; j<count; j++)
	{
		TRACE("----%d: name=%s-----\n",j,pTsm[j].Name);
		
		/*if (pTSM[j].device_type==12 && pTSM[j].sub_device_type==1)
		{
			continue ;
		}*/

		printf("\n Size:%d;Type:%d;SmID:%d;NodeID:%d;MemberGroupID:%d;PortID:%d;PortMode:%d;BaudRate:%d;Address:%d;Longitude:%f;Latitude:%f;Name:%s;Desc:%s;DeviceType:%d;DeviceSubType:%d;Productor:%s;Version:%s;Reserver1:%d;Reserver2:%s;\n",
			pTsm[j].Size,pTsm[j].Type,pTsm[j].ID.SmID,pTsm[j].ID.NodeID,pTsm[j].MemberGropID,pTsm[j].PortID,pTsm[j].PortMode,pTsm[j].BaudRate,pTsm[j].Address,pTsm[j].Longitude,pTsm[j].Latitude,pTsm[j].Name,pTsm[j].Desc,pTsm[j].DeviceType,
			pTsm[j].DeviceSubType,pTsm[j].Productor,pTsm[j].Version,pTsm[j].Reserver1,pTsm[j].Reserver2);
	}
	//   数据存储数据库

	
//	int result=SaveRvuSM(pTsm,count,rvuid);

	T_SM_LOAD *t_sm_load=new T_SM_LOAD;
	t_sm_load->count=count;
	t_sm_load->rvuid=rvuid;


 //	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
	t_sm_load->t_sm=new T_SM_[count];
	memset(t_sm_load->t_sm,0,sizeof(T_SM)*count);
	memcpy(t_sm_load->t_sm,pTsm,sizeof(T_SM)*count);

	ComDispatchTask(&g_db_smthread,SaveRvuSM,t_sm_load,sizeof(T_SM_LOAD),0);

//	delete t_sm_load;

// 	if (result!=0)
// 	{
// 		//存储失败
// 		printf("\n存储监控模块信息失败");
// 		return 0;
// 	}
		
	//   资源回收
	if (pTsm != NULL)
	{
		delete pTsm;
		pTsm = NULL;
	}
	
	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}

	if (t_sm_load->t_sm != NULL)
	{
		delete (t_sm_load->t_sm);
		t_sm_load->t_sm = NULL;
		delete t_sm_load;

	}
	return 1;
}
//系统结构信息解析NODE
int ParseRVUSendNODE(int handle,unsigned char* buff,int size,int rvuid)
{
	//	TRACE("-----size=%d------\n\n",size);
	
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
	T_NODE* pTnode=NULL;
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
					//	TRACE("----查不到响应ID号------\n");
					return 0;
				}
				else if (count == -1)
				{
					//	TRACE("----报文内容过长------\n");
					return 0;
				}
				
				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}
				
				data_len = first_len - SET_SM_ACK_DATA_OFFSET;
				
				if (reserved_index == 0)
				{
					pTnode = new T_NODE[count];
					memset(pTnode,0,sizeof(T_NODE)*count);
					memcpy(pTnode,&buff[i+SET_SM_ACK_DATA_OFFSET],sizeof(T_NODE)*count);
					break;
				}
				else
				{
					memcpy(/*&data*/pData,&buff[i+SET_SM_ACK_DATA_OFFSET],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}
					
					pTnode = new T_NODE[count];
					memset(pTnode,0,sizeof(T_NODE)*count);
					memcpy(pTnode,pData,sizeof(T_NODE)*count);
				}
			}
			break;
		}
		
		i++;
	}
	for (int j=0; j<count; j++)
	{
		TRACE("----%d: name=%s-----\n",j,pTnode[j].Name);
		/*if (pTNode[j].)
		{
		}*/

		printf("\n Type:%d ID.SmID:%d.NodeID:%d VirtualID.SmID:%d VirtualID.NodeID:%d  Name:%s Desc:%s MaxVal:%f MinVal:%f unit:%s h1:%f",
			pTnode[j].Type,pTnode[j].ID.SmID,pTnode[j].ID.NodeID,pTnode[j].VirtuaID.SmID,pTnode[j].VirtuaID.NodeID,pTnode[j].Name,pTnode[j].Desc,pTnode[j].MaxVal,pTnode[j].MinVal,pTnode[j].Unit,pTnode[j].HiLimit1);
	}
	//   数据存储数据库

//	int result=SaveRvuNODE(pTnode,count,rvuid);

	T_NODE_LOAD *t_node_load=new T_NODE_LOAD;
	t_node_load->count=count;
	t_node_load->rvuid=rvuid;
	
	
	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
	t_node_load->t_node=new T_NODE_[count];
	memset(t_node_load->t_node,0,sizeof(T_NODE)*count);
	memcpy(t_node_load->t_node,pTnode,sizeof(T_NODE)*count);
	
	ComDispatchTask(&g_db_nodethread,SaveRvuNODE,t_node_load,sizeof(T_NODE_LOAD),0);



	int n_flag=1;
	for(i=0;i<nodenum;i++)
	{	//判断是否为数据变更
		if (g_sNODEInfo[i].rvu_id==rvuid)
		{
			n_flag=0;
			g_sNODEInfo[i].rvu_handle=handle;
			g_sNODEInfo[i].count=count;
;
			for (int j=0;j<count;j++)
			{
				int k=0;
				if (pTnode[j].Type!=3)
				{
					g_sNODEInfo[i].tids[k].sm_id=pTnode[j].ID.SmID;
					g_sNODEInfo[i].tids[k].node_id=pTnode[j].ID.NodeID;
					k++;
				}
				
			}

		}
	}
	//判断是否为新增数据
	if (n_flag==1)
	{
		g_sNODEInfo[nodenum].rvu_handle=handle;
		g_sNODEInfo[nodenum].count=count;
		g_sNODEInfo[nodenum].rvu_id=rvuid;
//		g_sRVUalarm[nodenum].rvu_id=rvuid;
		for (i=0;i<count;i++)
		{
			int k=0;
			if (pTnode[i].Type!=3)
			{
				g_sNODEInfo[nodenum].tids[k].sm_id=pTnode[i].ID.SmID;
				g_sNODEInfo[nodenum].tids[k].node_id=pTnode[i].ID.NodeID;
				k++;
			}
			
		}
		nodenum++;
	}

// 	if (result!=0)
// 	{
// 		//存储失败
// 		printf("\n 存储节点信息失败");
// 		return 0;
// 	}
	
	
	//   资源回收
	if (pTnode != NULL)
	{
		delete pTnode;
		pTnode = NULL;
	}
	
	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
	if (t_node_load->t_node != NULL)
	{
		delete (t_node_load->t_node);
		t_node_load->t_node = NULL;
		delete t_node_load;
		
	}
	return 1;
}

//实时数据信息解析
int ParseRVUSendREAL(int handle,unsigned char* buff,int size,int rvuid)
{
	//	TRACE("-----size=%d------\n\n",size);
	
	int i=0,k=0;
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	int count = 0;
	int result=0;
	int data_index = 0;
	int first_len = 0;
	int data_len = 0;
	//char data[64*1024]={0x0};
	
	char * pData = NULL;
	pData = new char[size];
	memset(pData,0,sizeof(char)*size);
	T_DATA* pTdata=NULL;
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
				result = buff[i+26] | (buff[i+27]<<8) | (buff[i+28]<<16) | (buff[i+29]<<24);
				count = buff[i+30] | (buff[i+31]<<8) | (buff[i+32]<<16) | (buff[i+33]<<24);
				printf("\n 实时记录条数 %d",count);
				if (result == 0)
				{
					// 查询数据失败
					return 0;
				}
				if (count == -2)
				{
					//	TRACE("----查不到响应ID号------\n");
					return 0;
				}
				else if (count == -1)
				{
					//	TRACE("----报文内容过长------\n");
					return 0;
				}
				
				first_len = size%MAX_FRAME_LEN;
				if (first_len == 0)
				{
					first_len = MAX_FRAME_LEN;
				}
				
				data_len = first_len - SET_SM_ACK_DATA_OFFSET-4;
				
				if (reserved_index == 0)
				{
					pTdata = new T_DATA[count];
					memset(pTdata,0,sizeof(T_DATA)*count);
					memcpy(pTdata,&buff[i+SET_SM_ACK_DATA_OFFSET+4],sizeof(T_DATA)*count);
					break;
				}
				else
				{
					memcpy(/*&data*/pData,&buff[i+SET_SM_ACK_DATA_OFFSET+4],data_len);
					for (int j=0; j<reserved_index; j++)
					{
						memcpy(/*&data[data_len+j*(MAX_FRAME_LEN-8)]*/pData+data_len+j*(MAX_FRAME_LEN-8),&buff[i+first_len+j*MAX_FRAME_LEN+8],MAX_FRAME_LEN-8);
					}
					
					pTdata = new T_DATA[count];
					memset(pTdata,0,sizeof(T_DATA)*count);
					memcpy(pTdata,pData,sizeof(T_DATA)*count);
				}
			}
			break;
		}
		
		i++;
	}

//  存共享内存
	int j=0;
	int changeflag=1;
	//遍历RVU 
	for (j=0;j<rvu_num;i++)
	{//查询是否为数据变更还是新增数据
		if (g_sRVUData[j].rvu_id==rvuid)//数据变更
		{
			//上传标志置1
			g_sRVUData[j].uploadflag=1;
			for (i=0;i<g_sRVUData[j].count;i++)
			{
				int n=0;
				for(n=0;n<count;n++)
				{
					if ((g_sRVUData[j].t_datas[i].id.SmID==pTdata[n].id.SmID)&&(g_sRVUData[j].t_datas[i].id.NodeID==pTdata[n].id.NodeID))
					{
						g_sRVUData[j].t_datas[i].t_value=pTdata[n].t_value;
						g_sRVUData[j].t_datas[i].enumstate=pTdata[n].enumstate;
						//更新标志置1
						g_sRVUData[j].t_datas[i].changeflag=1;
						g_sRVUData[j].t_datas[i].alarmflag=1;

					}
				}
				
			}
			changeflag=0;
		}
	
	}
	//数据新增
	if (changeflag==1)
	{
		g_sRVUData[rvu_num].rvu_id=rvuid;
		
		g_sRVUData[rvu_num].count=count;
		for(int i=0;i<g_nAssRVUNum;i++)
		{
			if (g_tAssRVU[i].rvu_id==rvuid)
			{
				g_sRVUData[rvu_num].station_id=g_tAssRVU[i].station_id;
			}
		}
		
		for (int i=0;i<count;i++)
		{
			g_sRVUData[rvu_num].t_datas[i].type=pTdata[i].type;
			g_sRVUData[rvu_num].t_datas[i].id=pTdata[i].id;
			g_sRVUData[rvu_num].t_datas[i].t_value=pTdata[i].t_value;
			g_sRVUData[rvu_num].t_datas[i].enumstate=pTdata[i].enumstate;
			g_sRVUData[rvu_num].t_datas[i].size=pTdata[i].size;
			strcpy(g_sRVUData[rvu_num].t_datas[i].desc,pTdata[i].desc);
			//更新标志置零
			g_sRVUData[rvu_num].t_datas[i].changeflag=1;
			g_sRVUData[rvu_num].t_datas[i].alarmflag=1;
			
		}
		//上传标志置零
		g_sRVUData[rvu_num].uploadflag=0;
		//接受到实时信息rvu数量+1
		rvu_num++;
	}


//   数据存储数据库
//	int result1=SaveRvuData(pTdata,count,rvuid);

// 	T_data_LOAD *t_data_load=new T_data_LOAD;
// 	t_data_load->count=count;
// 	t_data_load->rvuid=rvuid;
// 	
// 	
// 	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
// 	t_data_load->t_data=new T_DATA_[count];
// 	memset(t_data_load->t_data,0,sizeof(T_DATA_)*count);
// 	memcpy(t_data_load->t_data,pTdata,sizeof(T_DATA_)*count);
// 	
// 	ComDispatchTask(&g_db_datathread,SaveRvuData,t_data_load,sizeof(T_data_LOAD),0);
	
// 	if (result1!=0)
// 	{
// 		//存储失败
// 		printf("\n 存储实时信息失败");
// 		return 0;
// 	}
	
	
	//   资源回收
	if (pTdata != NULL)
	{
		delete pTdata;
		pTdata = NULL;
	}
	
	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
// 	if (t_data_load->t_data != NULL)
// 	{
// 		delete (t_data_load->t_data);
// 		t_data_load->t_data = NULL;
// 		delete t_data_load;
// 		
// 	}
	return 1;
}

//接收回调函数
int recv_parse(NETHANDLE handle,char* buf,int size,void* user)
{
	//NETINFO info;
	if ((buf == NULL) || size == 0)
	{
		return 1;
	}
	int handleflag=0;
	int num=0;
	int i=0;
	for (i=0;i<handlenum;i++)
	{
		if (handle==g_recv_packet[i].handle)
		{
			handleflag=1;
			num=i;
			break;
		}
	}
	if (handleflag==0)
	{
		g_recv_packet[handlenum].handle=handle;
		num=handlenum;
		handlenum++;
	}
//	ComCloseNet(handle,1);
	if (g_recv_packet[num].size + size > RECVBUFSIZE)
	{
		memset(&g_recv_packet[num],0,sizeof(RECV_PACKET));
		handlenum--;
		return 1;
	}
	
	memcpy(g_recv_packet[num].recvbuf+g_recv_packet[num].size,buf,size);
	g_recv_packet[num].size += size;
	

	if (((unsigned char)buf[size-1]==0xd8)&&((unsigned char)buf[size-2]==0xd8))
	{
		g_recv_packet[num].handle = handle;
		ComDispatchTask(&g_parse_rvu_thread,ParseRVURecvBuf,&g_recv_packet[num],sizeof(RECV_PACKET),0);
		memset(&g_recv_packet[num],0,sizeof(RECV_PACKET));
		handlenum--;
	}

	
	
	return size;
	
}