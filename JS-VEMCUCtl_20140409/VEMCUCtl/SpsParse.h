#ifndef _SPS_PARSE_H_
#define _SPS_PARSE_H_

typedef struct SPS
{
	int profile_idc;
	int level_idc;
	int sps_id;
	int log2_max_frame_num;        ///< log2_max_frame_num_minus4 + 4
	int poc_type;                             ///< pic_order_cnt_type
	int log2_max_poc_lsb;              ///< log2_max_pic_order_cnt_lsb_minus4
	int poc_cycle_length;              ///< num_ref_frames_in_pic_order_cnt_cycle
	int ref_frame_count;               ///< num_ref_frames
	int gaps_in_frame_num_allowed_flag;///< gaps_in_frame_num_allowed_flag
	int mb_width;                          ///< pic_width_in_mbs_minus1 + 1
	int mb_height;                        ///< pic_height_in_map_units_minus1 + 1
}SPS;

BOOL ParseFmtpInfo(char *szFmtpConfig,int &nWidth,int &nHeight,int &level_id);
BOOL IsStandardH264Info(char *szFmtpConfig);
int SwitchSPS(char * strSps,BYTE *sps);


#endif