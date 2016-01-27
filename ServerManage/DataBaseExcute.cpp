#include "StdAfx.h"
#include "DataBaseExcute.h"



MYSQL mysql_syslist;

MYSQL mysql_rvulist;

MYSQL mysql_rvudata;

MYSQL mysql_cameralist;

MYSQL mysql_nodelist;

MYSQL mysql_login;

MYSQL mysql_historylog;

MYSQL mysql_devtype;

MYSQL mysql_insert;

MYSQL mysql_create;

extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;

extern _T_SQL_INFO a_tSqlInfo[10];
extern DBConfigParam g_DBConfigParam;

_T_DEV_ID g_devID[MAX_RVU_DEVICE_NUM_PER_STATION];
int devnum=0;

extern _C_LOGIN_USER c_login_user[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int usernum;


extern T_104_SM_LOAD t_104_sminfo[MAX_SM_NUM_PER_STATION];
extern int t_104_num;

extern T_RVU_DATA g_sRVUData[MAX_RVU_DEVICE_NUM_PER_STATION];
extern int rvu_num;

void GetRVUInfo()
{

	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	int i=0;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	
	// 读取ass_rvu表
	sprintf(szSQL,"SELECT rvu_id, station_id,rvu_code,rvu_name,rvu_ip,rvu_port,rvu_username,rvu_password,rvu_protocol FROM ass_rvu where rvu_protocol = 'ANFANG' ORDER BY rvu_id");
	
	nResult = mysql_query(&mysql_rvulist, szSQL);
	if (nResult != 0)
	{
		
		printf("\n rvu info can't get");
		return;
	}
	
	res = mysql_store_result(&mysql_rvulist);
	printf("\n res is %d",res);
	
	while ((row = mysql_fetch_row(res)))
	{	
		
		g_tAssRVU[i].rvu_id = atoi(row[0]);
		g_tAssRVU[i].station_id = atoi(row[1]);
		strcpy(g_tAssRVU[i].rvu_code,row[2]);
		strcpy(g_tAssRVU[i].rvu_name,row[3]);
		strcpy(g_tAssRVU[i].rvu_ip,row[4]);
		g_tAssRVU[i].rvu_port=atoi(row[5]);
		strcpy(g_tAssRVU[i].rvu_username,row[6]);
		strcpy(g_tAssRVU[i].rvu_password,row[7]);
		strcpy(g_tAssRVU[i].rvu_type,row[8]);
		g_tAssRVU[i].login_flag=0;
		//	printf("\nget local is %s\n",tAccessDev->local_addr);		
		i++;		
		
	}
	g_nAssRVUNum = i;
	mysql_free_result(res) ;
// 	return 0;
}
void GetDEVInfo()
{
	
	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	int i=0;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	
	// 读取ass_rvu表
	sprintf(szSQL,"SELECT id,rvu_id,sm_id,node_id,enum_type FROM ass_rvu_sm ORDER BY id");
	
	nResult = mysql_query(&mysql_rvulist, szSQL);
	if (nResult != 0)
	{
		
		printf("\n dev info can't get");
		return;
	}
	
	res = mysql_store_result(&mysql_rvulist);
	printf("\n res is %d",res);
	
	while ((row = mysql_fetch_row(res)))
	{	
		
		g_devID[i].devID = atoi(row[0]);
		g_devID[i].rvuid = atoi(row[1]);
		g_devID[i].SmID = atoi(row[2]);
		g_devID[i].NodeID=atoi(row[3]);
		g_devID[i].enumtype=atoi(row[4]);
		
		//	printf("\nget local is %s\n",tAccessDev->local_addr);		
		i++;		
		
	}
	devnum=i;
	mysql_free_result(res) ;
	// 	return 0;
}
//设备类型获取 联动信息组合
int GetDEVType(int rvuid,int smid,C_LINKINFO *linkinfo)
{
	
	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	int devid=0;
	int devtype=0;
	int i=0;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	
	// 读取ass_rvu表
	sprintf(szSQL,"SELECT id,device_type FROM ass_rvu_sm where rvu_id=%d and sm_id=%d",rvuid,smid);
	try
	{
		nResult = mysql_query(&mysql_devtype, szSQL);
		if (nResult != 0)
		{
			
			printf("\n dev type can't get");
			return 0;
		}
		
		res = mysql_store_result(&mysql_devtype);
		if (res>0)
		{
			while ((row = mysql_fetch_row(res)))
			{	
				devid = atoi(row[0]);
				devtype = atoi(row[1]);
				
			}
		}
		
		mysql_free_result(res) ;

	}
	catch(...)
	{
		return 0;
	}

	// 读取ass_camerapreset表
	sprintf(szSQL,"SELECT camera_code,camera_preset_id FROM ass_camerapreset where dev_id=%d",devid);
	try
	{
		nResult = mysql_query(&mysql_devtype, szSQL);
		if (nResult != 0)
		{
			
			printf("\n camera_code can't get");
			return 0;
		}
		i=0;
		res = mysql_store_result(&mysql_devtype);
		if (res>0)
		{
			while ((row = mysql_fetch_row(res)))
			{	
				strcpy(linkinfo->c_link[i].c_code,row[0]);
				linkinfo->c_link[i].c_pset = atoi(row[1]);
				i++;				
			}
		}
		linkinfo->linknum=i;
		mysql_free_result(res) ;
		
	}
	catch(...)
	{
		return 0;
	}

 	return devtype;
}
//摄像头信息获取
int GetCameraInfo(char *camera_code,C_CAMERAINFO *camerainfo)
{
	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	sprintf(szSQL,"SELECT a.dvr_ip,a.link_port,a.adminuser,a.adminpasswd,a.dvr_type,b.channel FROM video_dvr as a,video_camera as b where b.camera_id='%s' and a.dvr_id=b.dvr_id",camera_code);
	try	
	{
		nResult = mysql_query(&mysql_cameralist, szSQL);
		
		if (nResult != 0)
		{
			return 0;
		}
		
		res = mysql_store_result(&mysql_cameralist);
		if (res>0)
		{
			while ((row = mysql_fetch_row(res)))
			{	
				
				strcpy(camerainfo->ip,row[0]);	
				camerainfo->port=atoi(row[1]);
				strcpy(camerainfo->username,row[2]);
				strcpy(camerainfo->password,row[3]);
				strcpy(camerainfo->dvrtype,row[4]);
				camerainfo->channel=atoi(row[5]);
				
			}
		}
		
		mysql_free_result(res) ;

	}
	catch(...)
	{
		return 0;
	}
	return 1;

}

void GetUserInfo(void *arg)
{
	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	int devtype=0;
	int i=0;
	int u=0;
	int stationnum=0;
	int devnum=0;
	int user_id=*(int *)arg;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;

	for(i=0;i<usernum;i++)
	{
		if (c_login_user[i].userid==user_id)
		{
			u=i;
			break;
		}
	}
	// 读取ass_alarm_subscribe表
	sprintf(szSQL,"SELECT station_id FROM ass_alarm_subscribe where user_id=%d",user_id);
	
	nResult = mysql_query(&mysql_login, szSQL);

	if (nResult != 0)
	{
		return;
	}
	
	res = mysql_store_result(&mysql_login);
	if (res>0)
	{
		while ((row = mysql_fetch_row(res)))
		{	
			
			c_login_user[u].AlarmStation[stationnum] = atoi(row[0]);
			stationnum++;

			
		}
	}
	

	mysql_free_result(res) ;
	c_login_user[u].alarmstationNum=stationnum;

//	memcpy(szSQL,NULL,strlen(szSQL));
	// 读取ass_alarm_hide表
	sprintf(szSQL,"SELECT dev_id FROM ass_alarm_hide where user_id=%d",user_id);
	nResult = mysql_query(&mysql_login, szSQL);
	if (nResult != 0)
	{
		return;
	}
	res = mysql_store_result(&mysql_login);
	if (res>0)
	{
		while ((row = mysql_fetch_row(res)))
		{	
			
			c_login_user[u].devID[devnum] = atoi(row[0]);
			devnum++;

			
		}
	}

	mysql_free_result(res) ;
	c_login_user[u].devidNum=devnum;


}
int cmpLogin(_T_ASS_LOGIN_USER_INFO_ *login_user)
{
	
	char	szSQL[MAX_SQL_LENTH] = {0x0};
	int nResult=0;
	int count=0;
	int id=0;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;

	try
	{
		// 读取ct_user表
		sprintf(szSQL,"SELECT count(*),user_id from ct_user where user_name='%s' and pass_word='%s'",login_user->rvu_username,login_user->rvu_password);
		
		nResult = mysql_query(&mysql_login, szSQL);
		if (nResult != 0)
		{
			
			return 0;
		}
		
		res = mysql_store_result(&mysql_login);
		
		while ((row = mysql_fetch_row(res)))
		{	
			
			count = atoi(row[0]);
			id = atoi(row[1]);
			
		}
		mysql_free_result(res) ;
		if (count==1)
		{
			return id;
		}
		return 0;
	}
	catch (...)
	{
		return 0;
	}

		
}



//int SaveRvuSM(T_SM *t_sm,int count,int rvuid)
void SaveRvuSM(void *arg)
{

	char    adSQL[STR_SQL_LENTH*2] = {0x0};
	T_SM_LOAD* t_sm_load = (T_SM_LOAD*)arg;
	int nResult=0;
	int i=0;
	int count=t_sm_load->count;
	int rvuid=t_sm_load->rvuid;

	printf("\ncount is %d",count);


	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;

	// 将数据插入ASS_RVU_SM表
	for (i=0;i<count;i++)
	{
		if(t_sm_load->t_sm[i].Type==7)
		{

		sprintf(adSQL,"select * from ass_rvu_sm where rvu_id=%d and sm_id=%d",rvuid,t_sm_load->t_sm[i].ID.SmID);
		
		nResult = mysql_query(&mysql_syslist, adSQL);		
		if (nResult!=0)
		{
			return ;
		}
		res = mysql_store_result(&mysql_syslist);
		row = mysql_fetch_row(res);
		mysql_free_result(res) ;

		if (row==0)
		{
			sprintf(adSQL,"insert into ass_rvu_sm (rvu_id,enum_type,sm_id,node_id,member_group_id,port_id,port_mode,baud_rate,address,longtitude,latitude,name,f_desc,device_type,device_sub_type,productor,version,camera_channel,ruv_id_ex,reserver1,reserver2,relation_video_status) "
				"values(%d,%d,%d,%d,%d,%d,%d,%d,%d,"
				"'%f','%f','%s','%s',%d,%d,'%s','%s',%d,%d,%d,'%s',0);\n",rvuid,t_sm_load->t_sm[i].Type,
				t_sm_load->t_sm[i].ID.SmID,t_sm_load->t_sm[i].ID.NodeID,t_sm_load->t_sm[i].MemberGropID,t_sm_load->t_sm[i].PortID,t_sm_load->t_sm[i].PortMode,t_sm_load->t_sm[i].BaudRate,
				t_sm_load->t_sm[i].Address,t_sm_load->t_sm[i].Longitude,t_sm_load->t_sm[i].Latitude,t_sm_load->t_sm[i].Name,t_sm_load->t_sm[i].Desc,t_sm_load->t_sm[i].DeviceType,t_sm_load->t_sm[i].DeviceSubType,
				t_sm_load->t_sm[i].Productor,t_sm_load->t_sm[i].Version,t_sm_load->t_sm[i].CameraChannel,t_sm_load->t_sm[i].RVUID,t_sm_load->t_sm[i].Reserver1,t_sm_load->t_sm[i].Reserver2);
 	
		}
		else
		{
			sprintf(adSQL,"update ass_rvu_sm set enum_type=%d,member_group_id=%d,port_id=%d,port_mode=%d,baud_rate=%d,"
				"address=%d,longtitude='%f',latitude='%f',name='%s',f_desc='%s',device_type=%d,device_sub_type=%d,productor='%s',version='%s',camera_channel=%d,ruv_id_ex=%d,reserver1=%d,reserver2='%s' "
				"where rvu_id=%d and sm_id=%d;",t_sm_load->t_sm[i].Type,t_sm_load->t_sm[i].MemberGropID,t_sm_load->t_sm[i].PortID,t_sm_load->t_sm[i].PortMode,t_sm_load->t_sm[i].BaudRate,
				t_sm_load->t_sm[i].Address,t_sm_load->t_sm[i].Longitude,t_sm_load->t_sm[i].Latitude,t_sm_load->t_sm[i].Name,t_sm_load->t_sm[i].Desc,t_sm_load->t_sm[i].DeviceType,t_sm_load->t_sm[i].DeviceSubType,
				t_sm_load->t_sm[i].Productor,t_sm_load->t_sm[i].Version,t_sm_load->t_sm[i].CameraChannel,t_sm_load->t_sm[i].RVUID,t_sm_load->t_sm[i].Reserver1,t_sm_load->t_sm[i].Reserver2,rvuid,t_sm_load->t_sm[i].ID.SmID);
		}
		
		
	
		nResult = mysql_query(&mysql_syslist, adSQL);
	//	strcat(szSQL,adSQL);
		}
	
	}
//	printf("\n sql is %s",szSQL);
	

	if (nResult != 0)
	{
		printf("\n erro DB result is %d",nResult);
		return ;
	}
	
	//更新设备表	
	void GetDEVInfo();

	memset(adSQL,0,sizeof(adSQL));



	if (t_sm_load != NULL)
	{
		delete t_sm_load;
		t_sm_load = NULL;
	}



}

void Save104RvuSM(void *arg)
{
	
	char    adSQL[STR_SQL_LENTH*2] = {0x0};
	int nResult=0;
	int i=0,j=0;
	int count=0;
	int rvuid=0;

	int nodetype=0;
	int dev_type=0;

	int smid=0;
	int nodeid=0;

	
	printf("\ncount is %d",count);
	
	
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	while(1)
	{
		Sleep(30*1000);

		for (j=0;j<t_104_num;j++)
		{
			Sleep(5*1000);
			if (t_104_num>0)
			{
				count=t_104_sminfo[j].count;
				rvuid=t_104_sminfo[j].rvuid;
				// 将数据插入ASS_RVU_SM表
				for (i=0;i<count;i++)
				{
//					printf("\n t104num is %d",t_104_num);
//					printf("\n 插入设备表i %d",i);
					smid=t_104_sminfo[j].t_sm[i].smid[0]|((t_104_sminfo[j].t_sm[i].smid[1])<<8);
					nodeid=t_104_sminfo[j].t_sm[i].nodeid[0]|((t_104_sminfo[j].t_sm[i].nodeid[1])<<8);
					if (strcmp(t_104_sminfo[j].t_sm[i].node_type,"YC")==0)
					{
						nodetype=1;
					}
					else if (strcmp(t_104_sminfo[j].t_sm[i].node_type,"YX")==0)
					{
						nodetype=2;
					}
					else if (strcmp(t_104_sminfo[j].t_sm[i].node_type,"YK")==0)
					{
						nodetype=3;
					}
					else if (strcmp(t_104_sminfo[j].t_sm[i].node_type,"YT")==0)
					{
						nodetype=4;
					}
	
					if (nodetype==2)
					{
						dev_type = t_104_sminfo[j].t_sm[i].dev_type[0] | (t_104_sminfo[j].t_sm[i].dev_type[1]<<8) | (t_104_sminfo[j].t_sm[i].dev_type[2]<<16) | (t_104_sminfo[j].t_sm[i].dev_type[3]<<24);
						sprintf(adSQL,"select * from ass_rvu_sm where rvu_id=%d and sm_id=%d and node_id=%d",rvuid,smid,nodeid);
						
						nResult = mysql_query(&mysql_syslist, adSQL);		
						if (nResult!=0)
						{
							return ;
						}
						res = mysql_store_result(&mysql_syslist);
						row = mysql_fetch_row(res);
						mysql_free_result(res) ;
						
						if (row==0)
						{
							sprintf(adSQL,"insert into ass_rvu_sm (rvu_id,enum_type,sm_id,node_id,name,device_type) "
								"values(%d,%d,%d,%d,'%s',%d);",rvuid,nodetype,
								smid,nodeid,t_104_sminfo[j].t_sm[i].nodename,dev_type);
							
						}
						else
						{
							sprintf(adSQL,"update ass_rvu_sm set enum_type=%d,"
								"name='%s',device_type=%d "
								"where rvu_id=%d and sm_id=%d and node_id=%d;",nodetype,t_104_sminfo[j].t_sm[i].nodename,dev_type,rvuid,smid,nodeid);
						}
						
						
						
						nResult = mysql_query(&mysql_syslist, adSQL);
					}
					
// 				
// 					memset(t_104_sminfo[j].t_sm+i,0,sizeof(T_104_SM));
// 					t_104_sminfo[j].count--;//数据清0
					//	strcat(szSQL,adSQL);
					
					
				}
				//	printf("\n sql is %s",szSQL);
				
				
				if (nResult != 0)
				{
					printf("\n erro DB result is %d",nResult);
					return ;
				}
				memset(adSQL,0,sizeof(adSQL));
 			}
		

		}

		//更新设备表	
		void GetDEVInfo();
	}
		
	
}
void SaveRvuNODE(void *arg)
{

	char    adSQL[STR_SQL_LENTH*2] = {0x0};
	T_NODE_LOAD* t_node_load = (T_NODE_LOAD*)arg;
	int nResult=0;
	int i=0;
	int count=t_node_load->count;
	int rvuid=t_node_load->rvuid;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	
	// 将数据插入ASS_RVU_NODE表
	for (i=0;i<count;i++)
	{
		if (t_node_load->t_node[i].Type!=3)
		{
			sprintf(adSQL,"select * from ass_rvu_node where rvu_id=%d and sm_id=%d and node_id=%d",rvuid,t_node_load->t_node[i].ID.SmID,t_node_load->t_node[i].ID.NodeID);
			
			nResult = mysql_query(&mysql_syslist, adSQL);		
			if (nResult!=0)
			{
				return ;
			}
			res = mysql_store_result(&mysql_syslist);
			row = mysql_fetch_row(res);
			mysql_free_result(res) ;

			if (row==0)
			{
				sprintf(adSQL,"insert into ass_rvu_node (rvu_id,enum_type,sm_id,node_id,virtual_sm_id,virtual_node_id,name,f_desc,maxval,minval,alarm_enable,hi_limit1,lo_limit1,hi_limit2,lo_limit2,hi_limit3,lo_limit3,percision,saved,unit,phy_id,reserver1,reserver2,reserver3,reserver4) "
					"values(%d,%d,%d,%d,%d,%d,'%s','%s','%f','%f',%d,'%f','%f','%f','%f','%f','%f',"
					"'%f',%d,'%s',%d,%d,%d,%d,%d);",rvuid,t_node_load->t_node[i].Type,
					t_node_load->t_node[i].ID.SmID,t_node_load->t_node[i].ID.NodeID,t_node_load->t_node[i].VirtuaID.SmID,t_node_load->t_node[i].VirtuaID.NodeID,t_node_load->t_node[i].Name,t_node_load->t_node[i].Desc,t_node_load->t_node[i].MaxVal,t_node_load->t_node[i].MinVal,
					t_node_load->t_node[i].AlarmEnable,t_node_load->t_node[i].HiLimit1,t_node_load->t_node[i].LoLimit1,t_node_load->t_node[i].HiLimit2,t_node_load->t_node[i].LoLimit2,t_node_load->t_node[i].HiLimit3,t_node_load->t_node[i].LoLimit3,t_node_load->t_node[i].Percision,
					t_node_load->t_node[i].Saved,t_node_load->t_node[i].Unit,t_node_load->t_node[i].PHYID,t_node_load->t_node[i].Reserver1,t_node_load->t_node[i].Reserver2,t_node_load->t_node[i].Reserver3,t_node_load->t_node[i].Reserver4);
			}
			else
			{
				sprintf(adSQL,"update ass_rvu_node set enum_type=%d,virtual_sm_id=%d,virtual_node_id=%d,name='%s',f_desc='%s',maxval='%f',minval='%f',alarm_enable=%d,hi_limit1='%f',lo_limit1='%f',hi_limit2='%f',lo_limit2='%f',hi_limit3='%f',lo_limit3='%f',percision='%f',saved=%d,unit='%s',phy_id=%d,reserver1=%d,reserver2=%d,reserver3=%d,reserver4=%d "
					"where rvu_id=%d and sm_id=%d and node_id=%d;",t_node_load->t_node[i].Type,t_node_load->t_node[i].VirtuaID.SmID,t_node_load->t_node[i].VirtuaID.NodeID,t_node_load->t_node[i].Name,t_node_load->t_node[i].Desc,t_node_load->t_node[i].MaxVal,t_node_load->t_node[i].MinVal,
					t_node_load->t_node[i].AlarmEnable,t_node_load->t_node[i].HiLimit1,t_node_load->t_node[i].LoLimit1,t_node_load->t_node[i].HiLimit2,t_node_load->t_node[i].LoLimit2,t_node_load->t_node[i].HiLimit3,t_node_load->t_node[i].LoLimit3,t_node_load->t_node[i].Percision,
					t_node_load->t_node[i].Saved,t_node_load->t_node[i].Unit,t_node_load->t_node[i].PHYID,t_node_load->t_node[i].Reserver1,t_node_load->t_node[i].Reserver2,t_node_load->t_node[i].Reserver3,t_node_load->t_node[i].Reserver4,rvuid,t_node_load->t_node[i].ID.SmID,t_node_load->t_node[i].ID.NodeID);
			
			}
		
			nResult = mysql_query(&mysql_syslist, adSQL);

	
//			strcat(szSQL,adSQL);
		}
	
	}
	
//	nResult = mysql_query(&mysql_syslist, szSQL);
	if (nResult != 0)
	{
		
		return ;
	}
	
//	res = mysql_store_result(&mysql_syslist);


	memset(adSQL,0,sizeof(adSQL));	
	if (t_node_load != NULL)
	{
		delete t_node_load;
		t_node_load = NULL;
	}

	
}
void SaveRelation(void *arg)
{
	
	char    adSQL[STR_SQL_LENTH] = {0x0};
	time_t t;
	struct tm * timeinfo;
	time(&t);
	timeinfo=localtime(&t);
	char c_time[21];
	char t_year[5];
	sprintf(c_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(t_year,"%4d",1900+timeinfo->tm_year);
	T_LINKPUSH* t_linkpush = (T_LINKPUSH*)arg;
	int nResult=0;
	int i=0;
	int count=t_linkpush->c_linkinfo.linknum;
	printf("\n 1111111111111111111111");
	// 将数据插入t_ass_linkage_relation表
	for (i=0;i<count;i++)
	{
		
		sprintf(adSQL,"insert into ass_linkage_relation_%s (dev_id,camera_code,preset_num,start_time) "
			"values(%d,'%s',%d,'%s');",t_year,t_linkpush->devid,t_linkpush->c_linkinfo.c_link[i].c_code,t_linkpush->c_linkinfo.c_link[i].c_pset,c_time);
			
		nResult = mysql_query(&mysql_rvudata, adSQL);
		//		strcat(szSQL,adSQL);
	}
	
	
	
	
	memset(adSQL,0,sizeof(adSQL));
	
	
	if (t_linkpush != NULL)
	{
		delete t_linkpush;
		t_linkpush = NULL;
	}
	
	
}
void SaveRvuData(void *arg)
{

	char    adSQL[STR_SQL_LENTH] = {0x0};
	time_t t;
	struct tm * timeinfo;
	time(&t);
	timeinfo=localtime(&t);
	char c_time[21];
	char t_year[5];
	char t_ym[7];
	sprintf(c_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
	timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(t_year,"%4d",1900+timeinfo->tm_year);
	sprintf(t_ym,"%4d%02d",1900+timeinfo->tm_year,1+timeinfo->tm_mon);
	T_data_LOAD* t_data_load = (T_data_LOAD*)arg;
	int nResult=0;
	int i=0;
	int k=0;
	int count=t_data_load->count;
	int rvuid=t_data_load->rvuid;
	int devid=0;
	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	printf("\n 1111111111111111111111");
	// 将数据插入ass_rvu_sm_realdata表,ass_rvu_sm_history_data_表
	for (i=0;i<count;i++)
	{
		sprintf(adSQL,"select * from ass_rvu_sm_realdata where rvu_id=%d and sm_id=%d and node_id=%d and enum_type=%d",rvuid,t_data_load->t_data[i].id.SmID,t_data_load->t_data[i].id.NodeID,t_data_load->t_data[i].type);
		
		nResult = mysql_query(&mysql_rvudata, adSQL);		
		if (nResult!=0)
		{
			return;
		}
		res = mysql_store_result(&mysql_rvudata);
		row = mysql_fetch_row(res);
		mysql_free_result(res) ;
		
		if (row==0)
		{
			sprintf(adSQL,"insert into ass_rvu_sm_realdata (rvu_id,enum_type,sm_id,node_id,value,status,size "
				"values(%d,%d,%d,%d,%f,%d,%d);",rvuid,t_data_load->t_data[i].type,
			t_data_load->t_data[i].id.SmID,t_data_load->t_data[i].id.NodeID,t_data_load->t_data[i].t_value,t_data_load->t_data[i].enumstate,t_data_load->t_data[i].size);
		}
		else
		{
			sprintf(adSQL,"update ass_rvu_sm_realdata set value='%l',status=%d,size=%d "
				"where rvu_id=%d and sm_id=%d and node_id=%d and enum_type=%d",t_data_load->t_data[i].t_value,t_data_load->t_data[i].enumstate,t_data_load->t_data[i].size,
				rvuid,t_data_load->t_data[i].id.SmID,t_data_load->t_data[i].id.NodeID,t_data_load->t_data[i].type);
		}

		nResult = mysql_query(&mysql_rvudata, adSQL);
		
		//获取该设备ID
		for (k=0;k<devnum;k++)
		{
			if ((rvuid==g_devID[k].rvuid)&&(t_data_load->t_data[i].id.SmID==g_devID[k].SmID)&&(t_data_load->t_data[i].id.NodeID==g_devID[k].NodeID))
			{
				devid=g_devID[k].devID;
				break;
			}
		}

		sprintf(adSQL,"insert into ass_rvu_sm_history_data_%s (dev_id,rvu_id,enum_type,sm_id,node_id,value,status,time) "
			"values(%d,%d,%d,%d,%d,%f,%d,'%s');",devid,rvuid,t_data_load->t_data[i].type,
			t_data_load->t_data[i].id.SmID,t_data_load->t_data[i].id.NodeID,t_data_load->t_data[i].t_value,t_data_load->t_data[i].enumstate,c_time);
		nResult = mysql_query(&mysql_rvudata, adSQL);
		if (nResult!=0)
		{
			CreateTables();//如果插入失败 建表
		}
		//		strcat(szSQL,adSQL);
	}

	// 将数据插入ass_alarm表
	for (i=0;i<count;i++)
	{
		if ((t_data_load->t_data[i].enumstate>0)&&(t_data_load->t_data[i].enumstate<4))
		{
			sprintf(adSQL,"insert into ass_alarm_%s (rvu_id,sm_id,node_id,type,start_time,alarm_level,alarm_value) "
				"values(%d,%d,%d,%d,'%s',%d,'%f');",t_year,rvuid,t_data_load->t_data[i].id.SmID,t_data_load->t_data[i].id.NodeID,t_data_load->t_data[i].type,c_time,
				t_data_load->t_data[i].enumstate,t_data_load->t_data[i].t_value);
			nResult = mysql_query(&mysql_rvudata, adSQL);
		}
	}
	
//	nResult = mysql_query(&mysql_rvudata, szSQL);
	if (nResult != 0)
	{
		
		return ;
	}
	

	
	memset(adSQL,0,sizeof(adSQL));
		
	
	if (t_data_load != NULL)
	{
		delete t_data_load;
		t_data_load = NULL;
	}

	
}
void Save104RvuData(void *arg)
{

	char    adSQL[STR_SQL_LENTH] = {0x0};
	int i=0,j=0,k=0;
	int count=0;
	int rvuid=0;
	int devid=0;
	int nResult=0;
	time_t t;
	struct tm * timeinfo;

	MYSQL_RES * res = NULL ;
	MYSQL_ROW	row ;
	Sleep(10*1000);
	while(1)
	{
		Sleep(30*1000);
		time(&t);
		timeinfo=localtime(&t);
		char c_time[21];
		char t_year[5];
		char t_ym[7];
		sprintf(c_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
		sprintf(t_year,"%4d",1900+timeinfo->tm_year);
		sprintf(t_ym,"%4d%02d",1900+timeinfo->tm_year,1+timeinfo->tm_mon);
		printf("\n rvunum is %d",rvu_num);

		for(j=0;j<rvu_num;j++)
		{
			count=g_sRVUData[j].count;
			rvuid=g_sRVUData[j].rvu_id;
			devid=0;

			printf("\n save rvu data count is %d",count);
			// 将数据插入ass_rvu_sm_realdata表,ass_rvu_sm_history_data_表
			for (i=0;i<count;i++)
			{
				sprintf(adSQL,"select * from ass_rvu_sm_realdata where rvu_id=%d and sm_id=%d and node_id=%d and enum_type=%d",rvuid,g_sRVUData[j].t_datas[i].id.SmID,g_sRVUData[j].t_datas[i].id.NodeID,g_sRVUData[j].t_datas[i].type);
				
				nResult = mysql_query(&mysql_rvudata, adSQL);		
				if (nResult!=0)
				{
					return;
				}
				res = mysql_store_result(&mysql_rvudata);
				row = mysql_fetch_row(res);
				mysql_free_result(res) ;

				//获取该设备ID
				for (k=0;k<devnum;k++)
				{
					if ((rvuid==g_devID[k].rvuid)&&(g_sRVUData[j].t_datas[i].id.SmID==g_devID[k].SmID)&&(g_sRVUData[j].t_datas[i].id.NodeID==g_devID[k].NodeID))
					{
						devid=g_devID[k].devID;
						break;
					}
				}
				if(g_sRVUData[j].t_datas[i].id.SmID==0)
				{
					printf("\n 存储数据为空_________________________");
					continue;
				}
				if (row==0)
				{
					sprintf(adSQL,"insert into ass_rvu_sm_realdata (dev_id,rvu_id,enum_type,sm_id,node_id,value,status,size) "
						"values(%d,%d,%d,%d,%d,%f,%d,%d);",devid,rvuid,g_sRVUData[j].t_datas[i].type,
					g_sRVUData[j].t_datas[i].id.SmID,g_sRVUData[j].t_datas[i].id.NodeID,g_sRVUData[j].t_datas[i].t_value,g_sRVUData[j].t_datas[i].enumstate,g_sRVUData[j].t_datas[i].size);
				}
				else
				{
					sprintf(adSQL,"update ass_rvu_sm_realdata set value='%l',status=%d,size=%d "
						"where rvu_id=%d and sm_id=%d and node_id=%d and enum_type=%d",g_sRVUData[j].t_datas[i].t_value,g_sRVUData[j].t_datas[i].enumstate,g_sRVUData[j].t_datas[i].size,
						rvuid,g_sRVUData[j].t_datas[i].id.SmID,g_sRVUData[j].t_datas[i].id.NodeID,g_sRVUData[j].t_datas[i].type);
				}

				nResult = mysql_query(&mysql_rvudata, adSQL);
				printf("\n实时数据存储数据库  changeflag  is %d",g_sRVUData[j].t_datas[i].changeflag);
				
				if(g_sRVUData[j].t_datas[i].changeflag==1)
				{
					sprintf(adSQL,"insert into ass_rvu_sm_history_data_%s (dev_id,rvu_id,enum_type,sm_id,node_id,value,status,time) "
					"values(%d,%d,%d,%d,%d,%f,%d,'%s');",t_ym,devid,rvuid,g_sRVUData[j].t_datas[i].type,
					g_sRVUData[j].t_datas[i].id.SmID,g_sRVUData[j].t_datas[i].id.NodeID,g_sRVUData[j].t_datas[i].t_value,g_sRVUData[j].t_datas[i].enumstate,c_time);
					nResult = mysql_query(&mysql_rvudata, adSQL);
					if (nResult!=0)
					{
						CreateTables();//如果插入失败 建表
					}

						// 将数据插入ass_alarm表
					if ((g_sRVUData[j].t_datas[i].enumstate>0)&&(g_sRVUData[j].t_datas[i].enumstate<4))
					{
						sprintf(adSQL,"insert into ass_alarm_%s (rvu_id,dev_id,sm_id,node_id,type,start_time,alarm_level,alarm_value) "
							"values(%d,%d,%d,%d,%d,'%s',%d,'%f');",t_year,rvuid,devid,g_sRVUData[j].t_datas[i].id.SmID,g_sRVUData[j].t_datas[i].id.NodeID,g_sRVUData[j].t_datas[i].type,c_time,
							g_sRVUData[j].t_datas[i].enumstate,g_sRVUData[j].t_datas[i].t_value);
						nResult = mysql_query(&mysql_rvudata, adSQL);
					}
					g_sRVUData[j].t_datas[i].changeflag=0;
				}

			
				//		strcat(szSQL,adSQL);
			}


		
			
		//	nResult = mysql_query(&mysql_rvudata, szSQL);
			if (nResult != 0)
			{
				
				return ;
			}
							
			memset(adSQL,0,sizeof(adSQL));
		
		
		}
	}
	
}
void SqlInsert(void *arg)
{
	char* szSQL = (char*)arg;
	int nResult=0;
	int i=0;


	nResult = mysql_query(&mysql_insert, szSQL);

	if (nResult != 0)
	{
		
		return ;
	}

}
void CreateTables()
{
	char    adSQL[STR_SQL_LENTH*3] = {0x0};
	time_t t;
	struct tm * timeinfo;
	time(&t);
	timeinfo=localtime(&t);
	char c_time[21];
	char t_year[5];
	char t_ym[7];
	sprintf(c_time,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(t_year,"%4d",1900+timeinfo->tm_year);
	sprintf(t_ym,"%4d%02d",1900+timeinfo->tm_year,1+timeinfo->tm_mon);
	int nResult=0;
	//联动记录表
	sprintf(adSQL,"CREATE TABLE IF NOT EXISTS "
		"`ass_linkage_relation_%s` ("
		"`id` int(16) NOT NULL AUTO_INCREMENT,"
		"`dev_id` int(16) NOT NULL DEFAULT '0',"
		"`camera_code` char(64) DEFAULT NULL,"
		"`preset_num` int(8) DEFAULT '0',"
		"`start_time` char(32) DEFAULT NULL,"
		"PRIMARY KEY (`id`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;",t_year);
	nResult = mysql_query(&mysql_create, adSQL);
	//告警记录表
	sprintf(adSQL,"CREATE TABLE IF NOT EXISTS" 
		"`ass_alarm_%s` ("
		"`id` int(16) NOT NULL AUTO_INCREMENT,"
		"`rvu_id` int(16) DEFAULT NULL,"
		"`sm_id` int(16) DEFAULT NULL,"
		"`node_id` int(16) DEFAULT NULL,"
		"`type` int(16) DEFAULT NULL,"
		"`start_time` char(64) DEFAULT NULL,"
		"`end_time` char(64) DEFAULT NULL,"
		"`confirm_time` char(64) DEFAULT NULL,"
		"`cancel_time` char(64) DEFAULT NULL,"
		"`alarm_level` int(16) DEFAULT NULL,"
		"`alarm_value` float(16,3) DEFAULT NULL,"
		"`alarm_status` int(16) DEFAULT NULL,"
		"`desc` char(60) DEFAULT NULL,"
		"`process_status` int(16) DEFAULT NULL,"
		"PRIMARY KEY (`id`)"
		") ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;",t_year);
	nResult = mysql_query(&mysql_create, adSQL);
	//历史记录表
	sprintf(adSQL,"CREATE TABLE IF NOT EXISTS"
		"`ass_rvu_sm_history_data_%s` ("
		"`id` int(16) NOT NULL,"
		"`dev_id` int(16) NOT NULL,"
		"`rvu_id` int(16) DEFAULT NULL,"
		"`enum_type` int(16) DEFAULT NULL,"
		"`sm_id` int(16) DEFAULT NULL,"
		"`node_id` int(16) DEFAULT NULL,"
		"`value` float(16,3) DEFAULT NULL,"
		"`status` int(16) DEFAULT NULL,"
		"`size` int(16) DEFAULT NULL,"
		"`desc` char(80) CHARACTER SET utf8 DEFAULT NULL,"
		"`time` char(64) CHARACTER SET utf8 DEFAULT '1997-01-01 00:00:00',"
		"PRIMARY KEY (`id`)"
		") ENGINE=MyISAM DEFAULT CHARSET=gb2312;",t_ym);
	nResult = mysql_query(&mysql_create, adSQL);



}
void AutoCreat(void *arg)
{
	time_t t;
	struct tm * timeinfo;

	while(1)
	{
		time(&t);
		timeinfo=localtime(&t);


		//每月1号0点0时0分0秒建表
		if ((timeinfo->tm_mday==1)&&(timeinfo->tm_hour==0)&&(timeinfo->tm_min==0)&&(timeinfo->tm_sec<2))
		{
			CreateTables();
		}
		Sleep(1000);//休眠1秒
	}

}
// void SaveRvuAlarm(void *arg)
// {
// 
// 	char    adSQL[STR_SQL_LENTH] = {0x0};
// 
// 	T_ALARM_LOAD* t_alarm_load = (T_ALARM_LOAD*)arg;
// 	int nResult=0;
// 	int i=0;
// 	int count=t_alarm_load->count;
// 	int rvuid=t_alarm_load->rvuid;
// 
// //	MYSQL_RES * res = NULL ;
// 	//	MYSQL_ROW	row ;
// 	
// 	// 将数据插入ass_alarm表
// 	for (i=0;i<count;i++)
// 	{
// 	
// 		sprintf(adSQL,"insert into ass_alarm (rvu_id,sm_id,node_id,type,start_time,end_time,confirm_time,cancel_time,alarm_level,alarm_value,alarm_status,f_desc) "
// 			"values(%d,%d,%d,%d,'%s','%s','%s','%s',%d,'%f',%d,'%s');",rvuid,t_alarm_load->t_alarm[i].id.sm_id,t_alarm_load->t_alarm[i].id.node_id,t_alarm_load->t_alarm[i].type,t_alarm_load->t_alarm[i].starttime,t_alarm_load->t_alarm[i].endtime,t_alarm_load->t_alarm[i].confirmtime,
// 			t_alarm_load->t_alarm[i].canceltime,t_alarm_load->t_alarm[i].alarm_level,t_alarm_load->t_alarm[i].alarm_value,t_alarm_load->t_alarm[i].alarm_status,t_alarm_load->t_alarm[i].description);
// 		nResult = mysql_query(&mysql_rvudata, adSQL);
// 	}
// 	
// 
// 	if (nResult != 0)
// 	{
// 		
// 		return;
// 	}
// 	
// 	memset(adSQL,0,sizeof(adSQL));
// 	
// 	
// 	if (t_alarm_load != NULL)
// 	{
// 		delete t_alarm_load;
// 		t_alarm_load = NULL;
// 	}
// 
// 	
// }
int connectDB()
{
	// 实时会议状态数据库句柄
	CString strError;
	if (mysql_init(&mysql_syslist) == NULL) 
	{
		printf("数据库初始化失败");
		return -1;
	}	



	if (!mysql_real_connect(&mysql_syslist,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
 		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0))
	{
		printf("数据库建立连接失败");
		return -1;
	}
	mysql_query(&mysql_syslist,"set names gbk");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_rvulist) == NULL) 
	{
		printf("rvu数据库初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_rvulist,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
 		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("rvu数据库建立连接失败");
		return -1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_rvudata) == NULL) 
	{
		printf("rvu数据库初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_rvudata,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("rvu数据库建立连接失败");
		return -1;
	}
	mysql_query(&mysql_rvudata,"set names gbk");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_login) == NULL) 
	{
		printf("登陆数据库初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_login,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("登陆数据库建立连接失败");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_devtype) == NULL) 
	{
		printf("设备数据库初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_devtype,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("设备数据库初始化失败");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_insert) == NULL) 
	{
		printf("数据库插入初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_insert,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("数据库插入初始化失败");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_cameralist) == NULL) 
	{
		printf("视频设备初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_cameralist,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("视频设备初始化失败");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	if (mysql_init(&mysql_create) == NULL) 
	{
		printf("建表初始化失败");
		return -1;
	}	
	
	
	if (!mysql_real_connect(&mysql_create,g_DBConfigParam.DBServerIP,g_DBConfigParam.DBUserName,g_DBConfigParam.DBUserPassword,
		g_DBConfigParam.DBName,g_DBConfigParam.DBServerPort,NULL,0)) 
	{
		printf("建表初始化失败");
		return -1;
	}


	return 0;
}

int disconnectDB()
{
	if (&mysql_syslist)
	{
		mysql_close(&mysql_syslist);
	}
	if (&mysql_rvulist)
	{
		mysql_close(&mysql_rvulist);
	}
	if (&mysql_rvudata)
	{
		mysql_close(&mysql_rvudata);
	}
	if (&mysql_login)
	{
		mysql_close(&mysql_login);
	}
	if (&mysql_devtype)
	{
		mysql_close(&mysql_devtype);
	}
	if (&mysql_insert)
	{
		mysql_close(&mysql_insert);
	}
	if (&mysql_cameralist)
	{
		mysql_close(&mysql_cameralist);
	}
	if (&mysql_create)
	{
		mysql_close(&mysql_create);
	}
	return 0;
}

