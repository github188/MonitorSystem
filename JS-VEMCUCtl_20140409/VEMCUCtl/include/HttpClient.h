/*  
 * $Id: HttpClient.h,v 1.00 2010/07/01 09:53:18 henixon Exp $
 *
 * Copyright (C) 2008,2009,2010 http team
 *
 * Copyright 2010 Nanjing Yinshi Software Co.,Ltd.
 *
 * See the file COPYRIGHT for the respective terms and conditions.
 * If the file is missing contact me at mount_zijin@sohu.com
 * and I'll send you a copy.
 *
 */

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#define POWER_LEN		256
#define NOTES_LEN		64
#define NAME_LEN		256
#define URL_LEN			256
#define CODE_LEN		20
#define TIME_LEN		22
#define IP_LEN			16

#define HTTP_DEFAULT			0

// 获取资源信息
#define HTTP_REQUEST_RESOURCE		1
#define HTTP_RESPONSE_RESOURCE		2
// 获取历史告警信息
#define HTTP_REQUEST_HISTORY_ALARM	3
#define HTTP_RESPONSE_HISTORY_ALARM	4
// 获取历史录像信息
#define HTTP_REQUEST_HISTORY_VIDEO	5
#define HTTP_RESPONSE_HISTORY_VIDEO	6

// 更新所有子节点的记录
#define HTTP_REQUEST_UPDATE_PU_RESOURCE		10
#define HTTP_RESPONSE_UPDATE_PU_RESOURCE	11

// DB return result
#define HTTP_RESPONSE_DB			30
// sm_system
#define HTTP_UPDATE_SM_SYSTEM		40
#define HTTP_INSERT_SM_SYSTEM		41
#define HTTP_DELETE_SM_SYSTEM		42
// sm_router
#define HTTP_UPDATE_SM_ROUTER		43
#define HTTP_INSERT_SM_ROUTER		44
#define HTTP_DELETE_SM_ROUTER		45
// sm_vtdu
#define HTTP_UPDATE_SM_VTDU			46
#define HTTP_INSERT_SM_VTDU			47
#define HTTP_DELETE_SM_VTDU			48
// sm_server
#define HTTP_UPDATE_SM_SERVER		49
#define HTTP_INSERT_SM_SERVER		50
#define HTTP_DELETE_SM_SERVER		51
// sm_user
#define HTTP_UPDATE_SM_USER			52
#define HTTP_INSERT_SM_USER			53
#define HTTP_DELETE_SM_USER			54
// ob_node_relation
#define HTTP_UPDATE_OB_NODE_RELATION	55
#define HTTP_INSERT_OB_NODE_RELATION	56
#define HTTP_DELETE_OB_NODE_RELATION	57
// ob_camera
#define HTTP_UPDATE_OB_CAMERA		58
#define HTTP_INSERT_OB_CAMERA		59
#define HTTP_DELETE_OB_CAMERA		60
// ob_yx
#define HTTP_UPDATE_OB_YX			61
#define HTTP_INSERT_OB_YX			62
#define HTTP_DELETE_OB_YX			63
// ob_yc
#define HTTP_UPDATE_OB_YC			64
#define HTTP_INSERT_OB_YC			65
#define HTTP_DELETE_OB_YC			66

// sm_system
typedef struct SM_SYSTEM_ITEM{
	char	system_id[CODE_LEN];
	char	system_name[NAME_LEN];
	char	cms_ip[IP_LEN];
	int		cms_port;
	char	http_ip[IP_LEN];
	int		http_port;
	int		system_status;
	char	up_system_id[CODE_LEN];
	int		local_domain;

	struct SM_SYSTEM_ITEM *item;
}_t_sm_system_item,*_pt_sm_system_item;

// sm_vtdu
typedef struct SM_VTDU_ITEM{
	int		vtdu_id;
	char	sys_id[CODE_LEN];
	char	vtdu_name[NAME_LEN];
	char	vtdu_ip[IP_LEN];
	int		vtdu_port;
	char	public_ip[IP_LEN];
	int		server_state;
	char	last_time[TIME_LEN];
	char	call_name[CODE_LEN];
	int		max_load;
	int		cur_load;
	int		error_num;
	char	cpu_use[NAME_LEN];
	char	memory_use[NAME_LEN];
	
	struct SM_VTDU_ITEM *item;
}_t_sm_vtdu_item,*_pt_sm_vtdu_item;

// sm_router
typedef struct SM_ROUTER_ITEM{
	char	code_from[CODE_LEN];
	char	code_to[CODE_LEN];
	char	ip[IP_LEN];
	int		port;
	int		local_domain;

	struct SM_ROUTER_ITEM *item;
}_t_sm_router_item,*_pt_sm_router_item;

// sm_server
typedef struct SM_SERVER_ITEM{
	int		server_id;
	char	sys_id[CODE_LEN];
	char	server_name[NAME_LEN];
	char	server_ip[IP_LEN];
	int		server_port;
	char	bk_server_name[NAME_LEN];
	char	bk_server_ip[IP_LEN];
	int		bk_server_port;
	int		server_type;
	int		server_state;
	char	last_time[TIME_LEN];
	char	call_name[CODE_LEN];
	int		max_load;
	char	cpu_use[NAME_LEN];
	char	memory_use[NAME_LEN];
	
	struct SM_SERVER_ITEM *item;
}_t_sm_server_item,*_pt_sm_server_item;

// sm_user
typedef struct SM_USER_ITEM{
	int		user_no;
	char	user_name[NAME_LEN];
	char	pass_word[NAME_LEN];
	char	show_name[NAME_LEN];
	int		dev_type;
	char	ip_addr[IP_LEN];
	int		user_state;
	char	last_time[TIME_LEN];
	int		is_used;
	int		user_degree;
	int		user_id;
	
	struct SM_USER_ITEM *item;
}_t_sm_user_item,*_pt_sm_user_item;

// ob_node_relation
typedef struct OB_NODE_RELATION_ITEM{
	int		node_id;
	int		parent_id;
	char	sys_id[CODE_LEN];
	char	node_name[NAME_LEN];
	char	node_sysid[CODE_LEN];
	char	notes[NOTES_LEN];
	int		order_num;
	int		node_type;
	
	struct OB_NODE_RELATION_ITEM *item;
}_t_ob_node_relation_item,*_pt_ob_node_relation_item;

// ob_camera
typedef struct OB_CAMERA_ITEM{
	int		camera_id;
	char	system_id[CODE_LEN];
	char	camera_name[NAME_LEN];
	char	call_num[CODE_LEN];
	char	pts_callname[CODE_LEN];
	int		dvr_id;
	int		channel_no;
	int		camera_state;
	int		alarm_state;
	char	user_power[POWER_LEN];
	int		data_type;
	int		yt_tag;
	char	notes[NOTES_LEN];
	int		order_num;
	
	struct OB_CAMERA_ITEM *item;
}_t_ob_camera_item,*_pt_ob_camera_item;

// ob_yx
typedef struct OB_YX_ITEM{

	struct OB_YX_ITEM *item;
}_t_ob_yx_item,*_pt_ob_yx_item;

// ob_yc
typedef struct OB_YC_ITEM{

	struct OB_YC_ITEM *item;
}_t_ob_yc_item,*_pt_ob_yc_item;

// =========DB同步部分=========
// sm_system
typedef struct _insert_sm_system{
	int SubNum;
	SM_SYSTEM_ITEM *sm_system_item;
}_t_insert_sm_system,*_pt_insert_sm_system;
typedef struct _update_sm_system{
	int SubNum;
	SM_SYSTEM_ITEM *sm_system_item;
}_t_update_sm_system,*_pt_update_sm_system;
typedef struct _delete_sm_system{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	SM_SYSTEM_ITEM *sm_system_item;
}_t_delete_sm_system,*_pt_delete_sm_system;
// sm_router
typedef struct _insert_sm_router{
	int SubNum;
	SM_ROUTER_ITEM *sm_router_item;
}_t_insert_sm_router,*_pt_insert_sm_router;
typedef struct _update_sm_router{
	int SubNum;
	SM_ROUTER_ITEM *sm_router_item;
}_t_update_sm_router,*_pt_update_sm_router;
typedef struct _delete_sm_router{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	SM_ROUTER_ITEM *sm_router_item;
}_t_delete_sm_router,*_pt_delete_sm_router;
// sm_vtdu
typedef struct _insert_sm_vtdu{
	int SubNum;
	SM_VTDU_ITEM *sm_vtdu_item;
}_t_insert_sm_vtdu,*_pt_insert_sm_vtdu;
typedef struct _update_sm_vtdu{
	int SubNum;
	SM_VTDU_ITEM *sm_vtdu_item;
}_t_update_sm_vtdu,*_pt_update_sm_vtdu;
typedef struct _delete_sm_vtdu{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	SM_VTDU_ITEM *sm_vtdu_item;
}_t_delete_sm_vtdu,*_pt_delete_sm_vtdu;
// sm_server
typedef struct _insert_sm_server{
	int SubNum;
	SM_SERVER_ITEM *sm_server_item;
}_t_insert_sm_server,*_pt_insert_sm_server;
typedef struct _update_sm_server{
	int SubNum;
	SM_SERVER_ITEM *sm_server_item;
}_t_update_sm_server,*_pt_update_sm_server;
typedef struct _delete_sm_server{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	SM_SERVER_ITEM *sm_server_item;
}_t_delete_sm_server,*_pt_delete_sm_server;
// sm_user
typedef struct _insert_sm_user{
	int SubNum;
	SM_USER_ITEM *sm_user_item;
}_t_insert_sm_user,*_pt_insert_sm_user;
typedef struct _update_sm_user{
	int SubNum;
	SM_USER_ITEM *sm_user_item;
}_t_update_sm_user,*_pt_update_sm_user;
typedef struct _delete_sm_user{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	SM_USER_ITEM *sm_user_item;
}_t_delete_sm_user,*_pt_delete_sm_user;
// ob_node_relation
typedef struct _insert_ob_node_relation{
	int SubNum;
	OB_NODE_RELATION_ITEM *ob_node_relation_item;
}_t_insert_ob_node_relation,*_pt_insert_ob_node_relation;
typedef struct _update_ob_node_relation{
	int SubNum;
	OB_NODE_RELATION_ITEM *ob_node_relation_item;
}_t_update_ob_node_relation,*_pt_update_ob_node_relation;
typedef struct _delete_ob_node_relation{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	OB_NODE_RELATION_ITEM *ob_node_relation_item;
}_t_delete_ob_node_relation,*_pt_delete_ob_node_relation;
// ob_camera
typedef struct _insert_ob_camera{
	int SubNum;
	OB_CAMERA_ITEM *ob_camera_item;
}_t_insert_ob_camera,*_pt_insert_ob_camera;
typedef struct _update_ob_camera{
	int SubNum;
	OB_CAMERA_ITEM *ob_camera_item;
}_t_update_ob_camera,*_pt_update_ob_camera;
typedef struct _delete_ob_camera{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	OB_CAMERA_ITEM *ob_camera_item;
}_t_delete_ob_camera,*_pt_delete_ob_camera;
// ob_yx
typedef struct _insert_ob_yx{
	int SubNum;
	OB_YX_ITEM *ob_yx_item;
}_t_insert_ob_yx,*_pt_insert_ob_yx;
typedef struct _update_ob_yx{
	int SubNum;
	OB_YX_ITEM *ob_yx_item;
}_t_update_ob_yx,*_pt_update_ob_yx;
typedef struct _delete_ob_yx{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	OB_YX_ITEM *ob_yx_item;
}_t_delete_ob_yx,*_pt_delete_ob_yx;
// ob_yc
typedef struct _insert_ob_yc{
	int SubNum;
	OB_YC_ITEM *ob_yc_item;
}_t_insert_ob_yc,*_pt_insert_ob_yc;
typedef struct _update_ob_yc{
	int SubNum;
	OB_YC_ITEM *ob_yc_item;
}_t_update_ob_yc,*_pt_update_ob_yc;
typedef struct _delete_ob_yc{
	int SubNum;
	bool all;	//是否全部删除，1表示全部删除
	OB_YC_ITEM *ob_yc_item;
}_t_delete_ob_yc,*_pt_delete_ob_yc;

// 更新节点信息
typedef struct _req_update_pu_resource{
	char node_callnum[CODE_LEN];		// 需要刷新的节点编码
}_t_req_update_pu_resource, *_pt_req_update_pu_resource;

typedef struct _res_update_pu_resource{
	int result;		// 更新结果，0表示请求端不在线，1表示成功，-1表示未知失败
}_t_res_update_pu_resource, *_pt_res_update_pu_resource;

// ======资源信息获取部分======
// 请求资源
typedef struct _request_resource {
	char	code[CODE_LEN];
	char	UserCode[CODE_LEN];
	int		FromIndex;
	int		ToIndex;
}REQ_RESOURCE, *PREQ_RESOURCE;

// 回复资源列表
typedef struct _response_resource_item {
	char	code[CODE_LEN];
	char	name[64];
	int		status;
	int		DecoderTag;
	int		longitude;
	int		latitude;
	int		SubNum;
	
	struct _response_resource_item * next;
}RES_RESOURCE_ITEM, *PRES_RESOURCE_ITEM;

// 回复资源
typedef struct _response_resource {
	char	code[CODE_LEN];
	int		RealNum;
	int		SubNum;
	int		FromIndex;
	int		ToIndex;
	
	PRES_RESOURCE_ITEM item;
}RES_RESOURCE, *PRES_RESOURCE;

// ======历史告警部分======
// 请求历史告警
typedef struct _request_history {
	char	code[CODE_LEN];
	char	UserCode[CODE_LEN];
	int		type;
	char	BeginTime[TIME_LEN];
	char	EndTime[TIME_LEN];
	int		level;
	int		FromIndex;
	int		ToIndex;
}REQ_HISTORY, *PREQ_HISTORY;

// 回复历史告警列表
typedef struct _response_history_item {
	char	code[CODE_LEN];
	char	BeginTime[TIME_LEN];
	int		status;
	int		type;
	
	struct _response_history_item * next;
}RES_HISTORY_ITEM, *PRES_HISTORY_ITEM;

// 回复历史告警
typedef struct _response_history {
	int		RealNum;
	int		SubNum;
	int		FromIndex;
	int		ToIndex;
	
	PRES_HISTORY_ITEM item;
}RES_HISTORY, *PRES_HISTORY;

// ======录像检索部分======
// 请求录像
typedef struct _request_video {
	char	code[CODE_LEN];
	int		type;
	char	UserCode[CODE_LEN];
	char	BeginTime[TIME_LEN];
	char	EndTime[TIME_LEN];
	int		PlantOrPU;
	int		FromIndex;
	int		ToIndex;
}REQ_VIDEO, *PREQ_VIDEO;

// 回复录像列表
typedef struct _response_video_item {
	char	FileName[NAME_LEN];
	char	FileUrl[URL_LEN];
	char	BeginTime[TIME_LEN];
	char	EndTime[TIME_LEN];
	int		size;
	int		DecoderTag;
	int		type;
	
	struct _response_video_item * next;
}RES_VIDEO_ITEM, *PRES_VIDEO_ITEM;

// 回复录像
typedef struct _response_video {
	int		RealNum;
	int		SubNum;
	int		FromIndex;
	int		ToIndex;
	
	PRES_VIDEO_ITEM item;
}RES_VIDEO, *PRES_VIDEO;


int HttpClient(char *ip, unsigned short Port, int type, void *inArg, void **outArg, int timeout);

int initStructArg(void **arg, int type);

void freeStructArg(void **arg, int type);

#endif