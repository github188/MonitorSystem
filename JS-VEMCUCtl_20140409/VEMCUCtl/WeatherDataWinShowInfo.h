#ifndef _WEATHER_DATA_WIN_SHOW_INFO_H_
#define _WEATHER_DATA_WIN_SHOW_INFO_H_

//当前气象显示信息
typedef struct _t_current_wearther_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	int    nStationID;
	int    nTimeRange;
	float temperature;
	float humidity;
	float air_pressure;
	float ave_ws_2min;
	float ave_wd_2min;
	float rain_10min;
}T_CURRENT_WEATHER_DATA_WIN_INFO;

//历史气象记录信息
typedef struct _t_history_wearther_data_record_info_
{
	float temperature;
	float humidity;
	float air_pressure;
	float ave_ws_2min;
	float ave_wd_2min;
	float rain_10min;
}T_HISTORY_WEATHER_DATA_RECORD_INFO;

//历史气象显示信息
typedef struct _t_history_wearther_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	int    nStationID;
	char szStartTime[64];
	char szStopTime[64];
	char szSortColumn[64];
	int    nPageRecords;
	int    nPageIndex;
	int    nRecordCount;
	T_HISTORY_WEATHER_DATA_RECORD_INFO DataRecordInfo[20];
}T_HISTORY_WEATHER_DATA_WIN_INFO;

//历史气象告警记录信息
typedef struct _t_history_weather_warning_data_record_info_
{
	char data_time[WEATHER_TIME_LEN];
	char weather_warn_type[WARN_TYPE_LEN];
	float weather_warn_value;
}T_HISTORY_WEATHER_WARNIGN_DATA_RECORD_INFO;

//历史气象告警显示信息
typedef struct _t_history_weather_warning_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	int    nStationID;
	char szStartTime[64];
	char szStopTime[64];
	char szSortColumn[64];
	int    nPageRecords;
	int    nPageIndex;
	int    nRecordCount;
	T_HISTORY_WEATHER_WARNIGN_DATA_RECORD_INFO DataRecordInfo[20];
}T_HISTORY_WEATHER_WARNIGN_DATA_WIN_INFO;

//台风记录信息
typedef struct _t_typhoon_data_record_info_
{
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
}T_HISTORY_TYPHOON_DATA_RECORD_INFO;

//台风显示信息
typedef struct _t_typhoon_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	int    nTFID;
	char szStartTime[64];
	char szStopTime[64];
	char szSortColumn[64];
	int    nPageRecords;
	int    nPageIndex;
	int    nRecordCount;
	T_HISTORY_TYPHOON_DATA_RECORD_INFO DataRecordInfo[20];
}T_HISTORY_TYPHOON_DATA_WIN_INFO;

//当前微气象显示信息
typedef struct _t_current_micro_wearther_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	char szComType[64];
	int    nComCode;
	int    nTimeRange;
	float wind_speed;//风速
	float temperature;//温度
	float humidity;//湿度
	float wind_direction;//风向
	float air_pressure;//气压
	float precipitation;//降雨
	float radiation;//辐射
	char precipitation_type[64];//降雨类型
}T_CURRENT_MICRO_WEATHER_DATA_WIN_INFO;

//历史微气象显示信息
typedef struct _t_history_micro_wearther_data_record_info_
{
	float wind_speed;//风速
	float temperature;//温度
	float humidity;//湿度
	float wind_direction;//风向
	float air_pressure;//气压
	float precipitation;//降雨
	float radiation;//辐射
	char precipitation_type[64];//降雨类型
}T_HISTORY_MICRO_WEATHER_DATA_RECORD_INFO;


//历史微气象显示信息
typedef struct _t_history_micro_wearther_data_win_show_info_
{
	int nIndex;
	int nFlag;
	char szUserKey[64];
	char szComType[64];
	int    nComCode;
	char szStartTime[64];
	char szStopTime[64];
	char szSortColumn[64];
	int    nPageRecords;
	int    nPageIndex;
	int    nRecordCount;
	T_HISTORY_MICRO_WEATHER_DATA_RECORD_INFO DataRecordInfo[20];
}T_HISTORY_MICRO_WEATHER_DATA_WIN_INFO;


#endif