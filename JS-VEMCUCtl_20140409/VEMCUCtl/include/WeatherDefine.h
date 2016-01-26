#ifndef  _WEATHER_DEFINE_H_
#define _WEATHER_DEFINE_H_

//////////////////////////////////////////////////////////////////////////
#define WEATHER_TIME_LEN		64		// 时间的长度
#define WEATHER_SEND_LEN		4096	// 发送数据长度
#define WEATHER_RECV_LEN		(256*1024)	// 接收数据长度
#define WARN_TYPE_LEN             64     //告警类型    

// ------------------------------- 气象 -----------------------------------------------------
//
typedef struct _t_get_weather_response_head 
{
	int success;
	int result;
	int recordcnt;
	void *list;
} T_GET_WEATHER_RESPONSE_HEAD, *PT_GET_WEATHER_RESPONSE_HEAD;

//当前站点气象信息
typedef struct _t_get_weather_station_cur_data_list
{
	int id;
	char data_time[WEATHER_TIME_LEN];
	int weather_station_id;
	float air_pressure;
	float sea_pressure;
	float temperature;
	float max_temperature;
	float min_temperature;
	float humidity;
	float ave_ws_2min;
	float ave_wd_2min;
	float ave_ws_10min;
	float ave_wd_10min;
	float max_ws_10min;
	float max_ws_wd_10min;
	float max_ws_time_10min;
	float rain_10min;
	float visibility;

	struct _t_get_weather_station_cur_data_list *next;
} T_GET_WEATHER_STATTION_CUR_DATA_LIST, *PT_GET_WEATHER_STATTION_CUR_DATA_LIST;


//历史站点气象信息
typedef struct _t_get_weather_station_history_data_list
{
	int id;
	char data_time[WEATHER_TIME_LEN];
	int weather_station_id;
	float air_pressure;
	float sea_pressure;
	float temperature;
	float max_temperature;
	float min_temperature;
	float humidity;
	float ave_ws_2min;
	float ave_wd_2min;
	float ave_ws_10min;
	float ave_wd_10min;
	float max_ws_10min;
	float max_ws_wd_10min;
	float max_ws_time_10min;
	float rain_10min;
	float visibility;

	struct _t_get_weather_station_history_data_list *next;
} T_GET_WEATHER_STATTION_HISTORY_DATA_LIST, *PT_GET_WEATHER_STATTION_HISTORY_DATA_LIST;

//历史告警气象信息
typedef struct _t_get_weather_warning_history_data_list
{
	int   weather_station_id;
	char data_time[WEATHER_TIME_LEN];
	char weather_warn_type[WARN_TYPE_LEN];
	float weather_warn_value;

	struct _t_get_weather_warning_history_data_list *next;
} T_GET_WEATHER_WARNING_HISTORY_DATA_LIST, *PT_GET_WEATHER_WARNING_HISTORY_DATA_LIST;


//台风信息
typedef struct _t_get_weather_typhoon_data_list
{
	int id;
	int tf_id;
	char tf_time[WEATHER_TIME_LEN];
	float tf_longitude;
	float tf_latitude;
	float air_pressure;
	float wind_speed;
	float tf_50fq;
	float tf_30fq;
	float tf_24forelong;
	float tf_24forelati;
	float tf_24foreqy;
	float tf_24forefs;
	float tf_48forelong;
	float tf_48forelati;
	float tf_48foreqy;
	float tf_48forefs;
	float tf_72forelong;
	float tf_72forelati;
	float tf_72foreqy;
	float tf_72forefs;
	float tf_12yx;
	float tf_12ys;

	struct _t_get_weather_typhoon_data_list *next;
} T_GET_WEATHER_TYPHOON_DATA_LIST, *PT_GET_WEATHER_TYPHOON_DATA_LIST;


//当前站点微气象信息
typedef struct _t_get_micro_weather_station_cur_data_list
{
	char county_id[64];
	char county_name[256];
	char city_id[64];
	char city_name[256];
	char vov_level[64];
	char sub_station_id[64];
	char sub_station_name[256];
	char date_time[WEATHER_TIME_LEN];
	float wind_speed;//风速
	float temperature;//温度
	float humidity;//湿度
	float wind_direction;//风向
	float air_pressure;//气压
	float precipitation;//降雨
	float radiation;//辐射
	char precipitation_type[64];//降雨类型

	struct _t_get_micro_weather_station_cur_data_list *next;
}T_GET_MICRO_WEATHER_STATTION_CUR_DATA_LIST, *PT_GET_MICRO_WEATHER_STATTION_CUR_DATA_LIST;


//站点历史微气象信息
typedef struct _t_get_micro_weather_station_history_data_list
{
	char county_id[64];
	char county_name[256];
	char city_id[64];
	char city_name[256];
	char vov_level[64];
	char sub_station_id[64];
	char sub_station_name[256];
	char date_time[WEATHER_TIME_LEN];
	float wind_speed;//风速
	float temperature;//温度
	float humidity;//湿度
	float wind_direction;//风向
	float air_pressure;//气压
	float precipitation;//降雨
	float radiation;//辐射
	char precipitation_type[64];//降雨类型

	struct _t_get_micro_weather_station_history_data_list *next;
}T_GET_MICRO_WEATHER_STATTION_HISTORY_DATA_LIST, *PT_GET_MICRO_WEATHER_STATTION_HISTORY_DATA_LIST;


#endif