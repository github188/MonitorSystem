#include "StdAfx.h"
#include "Rvu104Recv.h"
#include "DataBaseExcute.h"

RECV_PACKET g_recv_104packet[MAX_SM_NUM_PER_STATION];
int recvflag=0;


extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;
extern COMMONTHREADPOOL g_parse_rvu104_thread ;
extern COMMONTHREADPOOL g_db_datathread;
extern COMMONTHREADPOOL g_db_smthread;
extern T_RVU_DATA g_sRVUData[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int rvu_num;


extern _T_DEV_ID g_devID[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int devnum;


T_104_SM_LOAD t_104_sminfo[MAX_SM_NUM_PER_STATION];
int t_104_num=0;

extern int getMaxnum;//获取最大数据
extern int getNnum;//当前获取数据数
extern bool g_bRecvComplete;//是否接收完一个rcu的数据

//接收回调函数
int recv_104_parse(NETHANDLE handle,char* buf,int size,void* user)
{
	//NETINFO info;
	if ((buf == NULL) || size == 0)
	{
		return 1;
	}
//	printf("\n handlenum is %d",handlenum104);
	int handleflag=0;
	int num=0;
	int i=0;
// 	for (i=0;i<handlenum104;i++)
// 	{
// 		if (handle==g_recv_104packet[i].handle)
// 		{
// 			handleflag=1;
// 			num=i;
// 			break;
// 		}
// 	}
// 	if (handleflag==0)
// 	{
// 		g_recv_104packet[handlenum104].handle=handle;
// 		num=handlenum104;
// 		handlenum104++;
// 	}
// 	//	ComCloseNet(handle,1);
// 	if (g_recv_104packet[num].size + size >RECVBUFSIZE )
// 	{
// 		memset(&g_recv_104packet[num],0,sizeof(RECV_PACKET));
// 		handlenum104--;
// 		return 1;
// 	}
// 	for (i=0;i<size;i++)
// 	{
// 		printf("\n %02x",buf[i]);
// 	}




	bool bFindhead=false;
	int istart=0;
	for (istart=0;istart<size;istart++)
	{
		if (0x68==buf[istart])
		{
			bFindhead=true;
			break;
		}
	}
	if (false==bFindhead)
	{
		return size;
	}
	//判断踢出主动上传数据
	if (0x06==buf[istart+4])
	{
		return size;
	}

	FILE* f;
	if((f = fopen("recv.txt", "a+")) == (FILE*)NULL)
	{
		printf("\n 文件打开失败");
		
	}
	
	for(i=0;i<size;i++)
	{
		fprintf(f,"%02x ",buf[i]);
		
	}
	fprintf(f,"\n");
	fclose(f);
	if ((unsigned char)buf[size-1]==0x16)
	{
		printf("\n send is 111111 ");
// 		ComInitThreadPool(&g_parse_rvu104_thread,SINGLEFIFOTHREADPOOL|COPYARG,-1,1,100);
// 		g_recv_104packet.handle = handle;
// 		ComDispatchTask(&g_parse_rvu104_thread,Parse104RVURecvBuf,&g_recv_104packet,sizeof(RECV_PACKET),0);
// 		memset(&g_recv_104packet,0,sizeof(RECV_PACKET));
		recvflag=1;

		for(i=0;i<MAX_SM_NUM_PER_STATION;i++)
		{
			printf("\n size is %d",g_recv_104packet[i].size);
			if(g_recv_104packet[i].size==0)
			{
				memset(g_recv_104packet[i].recvbuf,0,RECVBUFSIZE);
				memcpy(g_recv_104packet[i].recvbuf,&buf[istart],size-istart);
				g_recv_104packet[i].size = size-istart;
				g_recv_104packet[i].handle=handle;
				printf("\n g_recvbuff 1 is %02x size is %d",g_recv_104packet[i].recvbuf[0],g_recv_104packet[i].size);
				break;
			}
			
		}


	}	
	return size;
	
}
/*
BYTE[]转INT函数
b:BYTE来源
num:字节数
*/
int bytes2Int(BYTE* b,int num)
{  
	int temp = 0;  
	int res = 0; 
	int i;
	for (i = 0; i < num; i++) {  
		res <<= 8;  
		temp = b[i]&0xFF;  
		res |= temp;  
	}  
	return res;  
}  
/*
CP56Time2a转time函数
data:BYTE来源
st:time类型
*/
void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st)
{
 
	unsigned int mili = data[0] | data[1]<<8;
	st->wSecond= mili / 1000;
	st->wMilliseconds = mili - st->wSecond*1000;
 

	st->wMinute= data[2] & 0x3F;;
	st->wHour= data[3] & 0x1F;


	st->wDay= data[4] & 0x1F;
	st->wDayOfWeek = (data[4] & 0xE0 ) >> 5;
	if(st->wDayOfWeek)// if zero day of week not used.
	st->wDayOfWeek = (st->wDayOfWeek + 1)%7;
	st->wMonth= data[5] & 0x0F;
	st->wYear= 2000 + (data[6] & 0x7F);

}
/*
//单点遥信
void Parse104RVUYX1(int handle,unsigned char* buff,int rvuid)
{
	int i=0,k=0;

	int reserved_index = 0;
	int count = 0;
	//char data[64*1024]={0x0};
	BYTE address[3];
	
	count=buff[7];
	T_DATA_ *pTdata=new T_DATA_[count];
	k=12;
	for(i=0;i<count;i++)
	{
		memset(address,0,3);
		address[2]=buff[k];
		k++;
		address[1]=buff[k];
		k++;
		address[0]=buff[k];
		k++;
		
		
		pTdata[i].type=2;
		pTdata[i].id.SmID=bytes2Int(address,3);
		pTdata[i].id.NodeID=0;
		pTdata[i].t_value=buff[k];
		pTdata[i].size=0;
		pTdata[i].enumstate=0;

		k++;

	}
	//  存共享内存
	int j=0;
	int changeflag=1;
	//遍历RVU 
	for (j=0;j<rvu_num;i++);
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
					if ((g_sRVUData[j].t_datas[i].id.SmID==pTdata[n].id.SmID))
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
		
		for (i=0;i<count;i++)
		{
			g_sRVUData[rvu_num].t_datas[i].type=pTdata[i].type;
			g_sRVUData[rvu_num].t_datas[i].id.SmID=pTdata[i].id.SmID;
			g_sRVUData[rvu_num].t_datas[i].id.NodeID=0;
			g_sRVUData[rvu_num].t_datas[i].t_value=pTdata[i].t_value;
			g_sRVUData[rvu_num].t_datas[i].enumstate=0;
			g_sRVUData[rvu_num].t_datas[i].size=0;
			strcpy(g_sRVUData[rvu_num].t_datas[i].desc,"");
			//更新标志置零
			g_sRVUData[rvu_num].t_datas[i].changeflag=0;
			g_sRVUData[rvu_num].t_datas[i].alarmflag=1;
			
		}
		//上传标志置零
		g_sRVUData[rvu_num].uploadflag=0;
		//接受到实时信息rvu数量+1
		rvu_num++;
	}


	T_data_LOAD *t_data_load=new T_data_LOAD;
	t_data_load->count=count;
	t_data_load->rvuid=rvuid;
	
	
	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
	t_data_load->t_data=new T_DATA_[count];
	memset(t_data_load->t_data,0,sizeof(T_DATA_)*count);
	memcpy(t_data_load->t_data,pTdata,sizeof(T_DATA_)*count);
	
	ComDispatchTask(&g_db_datathread,SaveRvuData,t_data_load,sizeof(T_data_LOAD),0);
	
	//   资源回收
	if (pTdata != NULL)
	{
		delete pTdata;
		pTdata = NULL;
	}

	if (t_data_load->t_data != NULL)
	{
		delete (t_data_load->t_data);
		t_data_load->t_data = NULL;
		delete t_data_load;
		
	}
}
//带段时间格式的单点遥信
void Parse104RVUYX2(int handle,unsigned char* buff,int rvuid)
{
	int i=0,k=0;
	
	int reserved_index = 0;
	int count = 0;
	//char data[64*1024]={0x0};
	BYTE address[3];
	BYTE second[2];
	int sec=0;
	int min=0;
	int mils=0;
	
	count=buff[7];
	_T_104_YX2 *pYX2=new _T_104_YX2[count];
	T_DATA_ *pTdata=new T_DATA_[count];
	k=12;
	for(i=0;i<count;i++)
	{
		memset(address,0,3);
		address[2]=buff[k];
		k++;
		address[1]=buff[k];
		k++;
		address[0]=buff[k];
		k++;
		
		pYX2[i].addressID=bytes2Int(address,3);
		pYX2[i].status=buff[k];
		k++;
		memset(second,0,2);
		second[1]=buff[k];
		k++;
		second[0]=buff[k];
		k++;
		sec=bytes2Int(second,2);
		mils=sec%1000;
		sec=sec/1000;

		min=buff[k];
		k++;
		sprintf(pYX2[i].s_time,"%d:%d.%d",min,sec,mils);

		pTdata[i].type=2;
		pTdata[i].id.SmID=pYX2[i].addressID;
		pTdata[i].id.NodeID=0;
		pTdata[i].t_value=pYX2[i].status;
		pTdata[i].size=0;
		pTdata[i].enumstate=1;

	}
	//  存共享内存
	int j=0;
	int changeflag=1;
	//遍历RVU 
	for (j=0;j<rvu_num;i++);
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
					if ((g_sRVUData[j].t_datas[i].id.SmID==pTdata[n].id.SmID))
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

	
	//存数据库
	T_data_LOAD *t_data_load=new T_data_LOAD;
	t_data_load->count=count;
	t_data_load->rvuid=rvuid;
	
	
	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
	t_data_load->t_data=new T_DATA_[count];
	memset(t_data_load->t_data,0,sizeof(T_DATA_)*count);
	memcpy(t_data_load->t_data,pTdata,sizeof(T_DATA_)*count);
	
	ComDispatchTask(&g_db_datathread,SaveRvuData,t_data_load,sizeof(T_data_LOAD),0);
	
	//   资源回收
	if (pYX2 != NULL)
	{
		delete pYX2;
		pYX2 = NULL;
	}
	if (pTdata != NULL)
	{
		delete pTdata;
		pTdata = NULL;
	}
	
	if (t_data_load->t_data != NULL)
	{
		delete (t_data_load->t_data);
		t_data_load->t_data = NULL;
		delete t_data_load;
		
	}
}
//CP56Time2a的单点遥信
void Parse104RVUYX1e(int handle,unsigned char* buff,int rvuid)
{
	int i=0,k=0,t=0;
	
	int reserved_index = 0;
	int count = 0;
	//char data[64*1024]={0x0};
	BYTE address[3];
	BYTE Cp_Time[7];
	SYSTEMTIME st;
	count=buff[7];
	_T_104_YX1E *pYX1e=new _T_104_YX1E[count];
	T_DATA_ *pTdata=new T_DATA_[count];
	k=12;
	for(i=0;i<count;i++)
	{
		memset(address,0,3);
		address[2]=buff[k];
		k++;
		address[1]=buff[k];
		k++;
		address[0]=buff[k];
		k++;
		
		pYX1e[i].addressID=bytes2Int(address,3);
		pYX1e[i].status=buff[k];
		k++;
		
		for(t=0;t<7;t++)
		{
			Cp_Time[t]=buff[k];
			k++;
		}
		CP56Time2aToTime(Cp_Time,&st);

		sprintf(pYX1e[i].cp_time,"%d-%d-%d %d:%d:%d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

		pTdata[i].type=2;
		pTdata[i].id.SmID=pYX1e[i].addressID;
		pTdata[i].id.NodeID=0;
		pTdata[i].t_value=pYX1e[i].status;
		pTdata[i].size=0;
		pTdata[i].enumstate=1;
		

	}
	//  存共享内存
	int j=0;
	int changeflag=1;
	//遍历RVU 
	for (j=0;j<rvu_num;i++);
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
					if ((g_sRVUData[j].t_datas[i].id.SmID==pTdata[n].id.SmID))
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
	
	
	//存数据库
	T_data_LOAD *t_data_load=new T_data_LOAD;
	t_data_load->count=count;
	t_data_load->rvuid=rvuid;
	
	
	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
	t_data_load->t_data=new T_DATA_[count];
	memset(t_data_load->t_data,0,sizeof(T_DATA_)*count);
	memcpy(t_data_load->t_data,pTdata,sizeof(T_DATA_)*count);
	
	ComDispatchTask(&g_db_datathread,SaveRvuData,t_data_load,sizeof(T_data_LOAD),0);
	
	//   资源回收
	if (pYX1e != NULL)
	{
		delete pYX1e;
		pYX1e = NULL;
	}
	if (pTdata != NULL)
	{
		delete pTdata;
		pTdata = NULL;
	}
	
	if (t_data_load->t_data != NULL)
	{
		delete (t_data_load->t_data);
		t_data_load->t_data = NULL;
		delete t_data_load;
		
	}
}
*/
//系统结构信息解析SM
int Parse104RVUSendSM(int handle,unsigned char* buff,int rvuid)
{
	//	TRACE("-----size=%d------\n\n",size);
	
	int i=0,k=0,j=0;

	int type = 0;
	int count = 0;

	int num=0;
	int changeflag=0;

	
//	T_SM* pTsm=NULL;
	T_104_SM *pTsm=NULL;

		
	count = buff[42] | (buff[43]<<8);


	
	

	pTsm = new T_104_SM[count];
	memset(pTsm,0,sizeof(T_104_SM)*count);
	memcpy(pTsm,&buff[44],sizeof(T_104_SM)*count);
		

			
	


	for (j=0; j<count; j++)
	{
		printf("\n node name is %s",pTsm[j].nodename);
		
		/*if (pTSM[j].device_type==12 && pTSM[j].sub_device_type==1)
		{
			continue ;
		}*/

	}
	//   数据存储数据库

	
//	int result=SaveRvuSM(pTsm,count,rvuid);

	for(i=0;i<t_104_num;i++)
	{
		if (t_104_sminfo[i].rvuid==rvuid)
		{
			num=t_104_sminfo[i].count;
	

			memcpy(t_104_sminfo[i].t_sm+num,pTsm,sizeof(T_104_SM)*count);
		
			t_104_sminfo[i].count+=count;

			changeflag=1;
			break;

		}
	}
	//新增RVU设备
	if (changeflag==0)
	{
		num=t_104_num;
		t_104_sminfo[num].rvuid=rvuid;
		t_104_sminfo[num].count=count;
		memcpy(t_104_sminfo[num].t_sm,pTsm,sizeof(T_104_SM)*count);
		t_104_num++;

	}


//	

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
	

	return 1;
}
/*实时数据信息解析
handle 连接句柄
buff 数据内容
rvuid RVU编号
enumstate 数据类型 0为正常数据 大于1为变更告警数据
*/
int Parse104RVUSendREAL(int handle,unsigned char* buff,int rvuid,int enumstate)
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
	int changeflag=0;
	int num=0;
	//char data[64*1024]={0x0};

	T_104_REAL *pReal=NULL;
	
	getMaxnum = buff[40] | (buff[41]<<8);
	count = buff[42] | (buff[43]<<8);
	getNnum+=count;

	printf("\n 当前总条数 %d,累计接收条数 %d ,当前接收条数 %d",getMaxnum,getNnum,count);
	
	//接收完一个rcu数据
	if (getMaxnum == getNnum)
	{
		g_bRecvComplete = true;
		getMaxnum = 0;
		getNnum = 0;
	}

	pReal = new T_104_REAL[count];
	memset(pReal,0,sizeof(T_104_REAL)*count);
	memcpy(pReal,&buff[44],sizeof(T_104_REAL)*count);

	T_DATA* pTdata=NULL;

	
	printf("\n 实时记录条数__________ %d",count);

	pTdata=new T_DATA[count];
	for (i=0;i<count;i++)
	{

		pTdata[i].id.SmID=pReal[i].smid[0]|(pReal[i].smid[1]<<8);
		pTdata[i].id.NodeID=pReal[i].nodeid[0]|(pReal[i].nodeid[1]<<8);
		pTdata[i].t_value=atof(pReal[i].t_value);
		pTdata[i].size=0;
		if (atoi(pReal[i].t_value)==1)
		{
			enumstate=1;
		}
		else
			enumstate=0;
		pTdata[i].enumstate=enumstate;

		for (k=0;k<devnum;k++)
		{
			if ((g_devID[k].rvuid==rvuid)&&(g_devID[k].SmID==pTdata[i].id.SmID)&&(g_devID[k].NodeID==pTdata[i].id.NodeID))
			{
				pTdata[i].type=g_devID[k].enumtype;
			}
		}
		printf("\n pTdata smid %d node id %d t_value %f",pTdata[i].id.SmID,pTdata[i].id.NodeID,pTdata[i].t_value);

	}


			

//  存共享内存
	int j=0;
// 判断是正常数据还是变更数据 

	//遍历RVU 
	for (j=0;j<MAX_SM_NUM_PER_STATION;j++)
	{
		if (g_sRVUData[j].rvu_id==rvuid)//数据变更
		{
	
			printf("\n 数据更新 count %d",count);
		
			//上传标志置1
			g_sRVUData[j].uploadflag=1;
			for (i=0;i<g_sRVUData[j].count;i++)
			{
				int n=0;
				for(n=0;n<count;n++)
				{
					if ((g_sRVUData[j].t_datas[i].id.SmID==pTdata[n].id.SmID)&&(g_sRVUData[j].t_datas[i].id.NodeID==pTdata[n].id.NodeID)&&(pTdata[n].type==2))
					{
						printf("\n 数据变化");
						if(g_sRVUData[j].t_datas[i].t_value!=pTdata[n].t_value)
						{
							g_sRVUData[j].t_datas[i].changeflag=1;							
							g_sRVUData[j].t_datas[i].alarmflag=1;
							g_sRVUData[j].t_datas[i].updateflag=1;
						}
					
						g_sRVUData[j].t_datas[i].t_value=pTdata[n].t_value;
						g_sRVUData[j].t_datas[i].enumstate=pTdata[n].enumstate;

						changeflag=1;
					}
				}
				
			}
			printf("\n changeflag is %d",changeflag);
			if(changeflag==0)//数据新增
			{
				changeflag=2;
				printf("\n 数据新增 count %d",count);
				num=g_sRVUData[j].count;
				for (k=0;k<count;k++)
				{
					if(pTdata[k].type==2)
					{
						g_sRVUData[j].t_datas[num].type=pTdata[k].type;
						g_sRVUData[j].t_datas[num].id=pTdata[k].id;
						g_sRVUData[j].t_datas[num].t_value=pTdata[k].t_value;
						g_sRVUData[j].t_datas[num].enumstate=pTdata[k].enumstate;
						g_sRVUData[j].t_datas[num].size=pTdata[k].size;
						//更新标志置零
						g_sRVUData[j].t_datas[num].changeflag=1;
						g_sRVUData[j].t_datas[num].alarmflag=0;
						num++;
					}
				}
				g_sRVUData[j].count=num;
				printf("\n get rvudata  count is %d__________________________________________",num);
			}
			
			break;
		}		
	}
	
	if(changeflag==0)//新增数据
	{
		printf("\n 新增数据 count %d  rvunum %d",count,rvu_num);
		g_sRVUData[rvu_num].rvu_id=rvuid;
		
		g_sRVUData[rvu_num].count=count;
		for(int i=0;i<g_nAssRVUNum;i++)
		{
			if (g_tAssRVU[i].rvu_id==rvuid)
			{
				g_sRVUData[rvu_num].station_id=g_tAssRVU[i].station_id;
			}
		}
		
		for (i=0;i<count;i++)
		{
			if(pTdata[i].type==2)
			{
				g_sRVUData[rvu_num].t_datas[i].type=pTdata[i].type;
				g_sRVUData[rvu_num].t_datas[i].id=pTdata[i].id;
				g_sRVUData[rvu_num].t_datas[i].t_value=pTdata[i].t_value;
				g_sRVUData[rvu_num].t_datas[i].enumstate=pTdata[i].enumstate;
				g_sRVUData[rvu_num].t_datas[i].size=pTdata[i].size;
		//		strcpy(g_sRVUData[rvu_num].t_datas[i].desc,pTdata[i].desc);
				//更新标志置零
				g_sRVUData[rvu_num].t_datas[i].changeflag=1;
				g_sRVUData[rvu_num].t_datas[i].alarmflag=0;	
			}

			
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
	
	
	//	t_sm_load.t_sm=(T_SM_*)malloc(sizeof(T_SM_)*count);
// 	t_data_load->t_data=new T_DATA_[count];
// 	memset(t_data_load->t_data,0,sizeof(T_DATA_)*count);
// 	memcpy(t_data_load->t_data,pTdata,sizeof(T_DATA_)*count);
	
//	ComDispatchTask(&g_db_datathread,SaveRvuData,t_data_load,sizeof(T_data_LOAD),0);
	
// 	if (result1!=0)
// 	{
// 		//存储失败
// 		printf("\n 存储实时信息失败");
// 		return 0;
// 	}
	
	
	//   资源回收
	if (pReal != NULL)
	{
		delete pReal;
		pReal = NULL;
	}

	if (pTdata != NULL)
	{
		delete pTdata;
		pTdata = NULL;
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


void Parse104RVURecvBuf(void* arg)
{
	int total_len = 0;
	int index = 0;
	int reserved_index = 0;
	int type = 0;
	//	unsigned char temp_buf[1024*32] = {0x0};
	int i=0;
	int lenth=0;
	int j=0;
	int handlenum=0;
	int handle=0;
	
	while(1)
	{
		
		for (i=0;i<MAX_SM_NUM_PER_STATION;i++)
		{
			
			if (g_recv_104packet[i].size>0)
			{
				printf("\n recv is 111111111");
				BYTE* pBuff = (BYTE*)g_recv_104packet[i].recvbuf;
				int size = g_recv_104packet[i].size;
				g_recv_104packet[i].size=0;//取得数据后清除
				int rvuid=0;
				char rvuip[16];
				char rvucode[32];
				int rvuport=0;
				int datalen=0;
				handle=g_recv_104packet[i].handle;
				//总招命令
				//	BYTE zzBuff[16]={0x68,0x0e,0x00,0x00,0x00,0x00,0x64,0x01,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x14};
				
				
				// 接收数据处理
				_T_104_BUFF *sendBuff;
				
				int i = 0;
				int temp_index = 0;
				
				printf("\n handle is %d  fun is %02x",g_recv_104packet[i].handle,pBuff[4]);
				//判断是否为接入方发给非接入方的数据
				if (pBuff[3]=0x00)
				{
					return;
				}
				
				memcpy(rvucode,&pBuff[8],32);
				
				printf("\n rvucode is %s",rvucode);
				for (i=0;i<g_nAssRVUNum;i++)
				{
					if (strcmp(g_tAssRVU[i].rvu_code,rvucode)==0)
					{
						printf("\n 获取数据成功");
						rvuid=g_tAssRVU[i].rvu_id;
						memcpy(rvuip,g_tAssRVU[i].rvu_ip,16);
						rvuport=g_tAssRVU[i].rvu_port;
						handlenum=i;
					}
				}
				//报文类型判断
				switch(pBuff[4])
				{
				case 1://接入方注册
					{
						
					}
					break;
				case 2://获取单个YX/TC值
					break;
				case 3://远程遥控
					break;
				case 4://总招
					try
					{
						Parse104RVUSendREAL(handle,pBuff,rvuid,0);
					}
					catch(...)
					{
						printf("\n解析错误");
					}
					break;
				case 5://心跳
					{
					//	g_tAssRVU[handlenum].lost_heatbeat=0;//接收到心跳后清0
					}
					break;
				case 6://主动上送
					{
					//	Parse104RVUSendREAL(handle,pBuff,rvuid,1);
					}
					break;
				case 7://结构信息获取
					try
					{
						Parse104RVUSendSM(handle,pBuff,rvuid);
					
					}
					catch(...)
					{
						printf("\n解析错误");
					}
					break;
				default:
					break;
					
				}
		
			}
			else
			{
			printf("\n 没有数据");
			Sleep(1*1000);
			break;
			}
	
		}
	
			
	}


/*
	//判断是否为激活响应
	if ((pBuff[1]==4)&&(pBuff[2]==0x0b))
	{
		printf("\n 激活响应");
		//发送总招命令
		ComSendNet(pRecvPacket->handle,NULL,0,0,0,0,0,(char *)zzBuff,16,0);
		return;
		
	}
	//判断是否为测试帧
	if ((pBuff[1]==4)&&(pBuff[2]==0x83))
	{
		printf("\n 测试针回应");
		return;
		
	}
	//判断是否为响应总招
	if ((pBuff[6]==0x64)&&(pBuff[8]==0x07))
	{
		printf("\n 总招响应");
		return;
		
	}

	//判断是否为响应总召唤
	if (pBuff[8]==0x14)
	{
		switch(pBuff[6])
		{
		case 0x01://单点遥信 4BYTE
			for (j=0;j<g_nAssRVUNum;j++)
			{
				if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
				{
					rvuid=g_tAssRVU[j].rvu_id;
					memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
					rvuport=g_tAssRVU[j].rvu_port;
				}
			}
			Parse104RVUYX1(pRecvPacket->handle,pBuff,rvuid);
			break;
		default:
			break;
		}
		return;
	}
	//判断是否为结束总召唤
	if ((pBuff[8]==0x0a)&&(pBuff[6]==0x64))
	{
		printf("\n 结束总招");
		return;
	}
	//判断是否为主动上送信息
	if (pBuff[8]==0x03)
	{
		switch(pBuff[6])
		{
		case 0x02://带时标的单点遥信 7BYTE
			for (j=0;j<g_nAssRVUNum;j++)
			{
				if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
				{
					rvuid=g_tAssRVU[j].rvu_id;
					memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
					rvuport=g_tAssRVU[j].rvu_port;
				}
			}
			Parse104RVUYX2(pRecvPacket->handle,pBuff,rvuid);
			break;
		case 0x1e://带cp56Time2a时标的单点遥信 11BYTE
			for (j=0;j<g_nAssRVUNum;j++)
			{
				if (g_tAssRVU[j].commnet_handle==pRecvPacket->handle)
				{
					rvuid=g_tAssRVU[j].rvu_id;
					memcpy(rvuip,g_tAssRVU[j].rvu_ip,16);
					rvuport=g_tAssRVU[j].rvu_port;
				}
			}
			Parse104RVUYX1e(pRecvPacket->handle,pBuff,rvuid);
			break;
		default:
			break;
		}
		return;
	}

*/

//exit(1);
	
}