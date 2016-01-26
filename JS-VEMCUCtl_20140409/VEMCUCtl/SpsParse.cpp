#include "stdafx.h"
#include "SpsParse.h"
#include "Base64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define OPEN_READER(name)\
	int name##_index = g_sps_index;\
	int name##_cache = 0;

#define AV_RB32(x) \
	((((unsigned char *)(x))[0] << 24)|\
	(((unsigned char*)(x))[1]   << 16)|\
	(((unsigned char*)(x))[2]   <<  8)|\
	((unsigned char*)(x))[3])

#define UPDATE_CACHE(name, gb) \
	name##_cache = AV_RB32((unsigned char *)(gb)+(name##_index>>3)) << (name##_index&0x07);

#define GET_CACHE(name) ((unsigned int)name##_cache)

#define LAST_SKIP_BITS(name,num) name##_index += (num);

#define CLOSE_READER(name) g_sps_index= name##_index;

#define SHOW_UBITS(name, num) (((unsigned int)(name##_cache))>>(32-(num)))

const unsigned char g_golomb_vlc_len[512]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

const unsigned char g_ue_golomb_vlc_code[512]=
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
	7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char g_log2_tab[256]=
{
	0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

static int g_sps_index = 0;

static inline unsigned int get_bits(unsigned char *s, int n)
{
    register int tmp;
	OPEN_READER(re)
	UPDATE_CACHE(re, s)
	tmp= SHOW_UBITS(re,n);
	LAST_SKIP_BITS(re, n)
	CLOSE_READER(re)
    return tmp;
}

static inline unsigned int get_bits1(unsigned char *s)
{
	int index= g_sps_index;
	unsigned char result= s[index>>3];
	result<<= (index&0x07);
	result>>= 8 - 1;
	index++;
	g_sps_index= index;

	return result;
}

static inline int av_log2(unsigned int v)
{
	int n = 0;
	if (v & 0xffff0000)
	{
		v >>= 16;
		n += 16;
	}
	if (v & 0xff00) 
	{
		v >>= 8;
		n += 8;
	}
	n += g_log2_tab[v];

	return n;
}

static inline int get_ue_golomb(unsigned char *gb)
{
    unsigned int buf;
    int log;

    OPEN_READER(re);
    UPDATE_CACHE(re, gb);
    buf=GET_CACHE(re);

    if(buf >= (1<<27))
	{
        buf >>= 32 - 9;
        LAST_SKIP_BITS(re,g_golomb_vlc_len[buf]);
        CLOSE_READER(re);

        return g_ue_golomb_vlc_code[buf];
    }
	else
	{

        log= 2*av_log2(buf) - 31;
        buf>>= log;
        buf--;
        LAST_SKIP_BITS(re,32-log);
        CLOSE_READER(re);

        return buf;
    }
}

BOOL ParseFmtpInfo(char *szFmtpConfig,int &nWidth,int &nHeight,int &level_id)
{
	unsigned char dstbuffer[64];
	int dstlen;
	char *token;
	SPS sps;

	nWidth = nHeight = level_id = 0;

	char *pSPS = strstr(szFmtpConfig,"sprop-parameter-sets=");

	if (pSPS != NULL)
	{
		pSPS += strlen("sprop-parameter-sets=");
		token = strtok(pSPS,",");
		Base64Decode((unsigned char *)pSPS,strlen(pSPS),dstbuffer,dstlen);
		if (dstlen > 8)
		{
			g_sps_index = 0;

			get_bits(dstbuffer,8);//Forbidden_zero_bit|Nal_ref_idc|Nal_unit_type
			sps.profile_idc = get_bits(dstbuffer,8);//profile_idc

			get_bits1(dstbuffer);   //constraint_set0_flag
			get_bits1(dstbuffer);   //constraint_set1_flag
			get_bits1(dstbuffer);   //constraint_set2_flag
			get_bits1(dstbuffer);   //constraint_set3_flag
			get_bits(dstbuffer, 4); //reserved

			sps.level_idc = level_id= get_bits(dstbuffer,8);//level_id
			sps.sps_id = get_ue_golomb(dstbuffer);//sps_id

			sps.log2_max_frame_num= get_ue_golomb(dstbuffer) + 4;//log2_max_frame_num_minus4+4
			sps.poc_type= get_ue_golomb(dstbuffer);//pic_order_cnt_type

			if(sps.poc_type == 0)
			{
				sps.log2_max_poc_lsb= get_ue_golomb(dstbuffer) + 4;//log2_max_pic_order_cnt_lsb_minus4
			}

			sps.ref_frame_count = get_ue_golomb(dstbuffer);//num_ref_frames
			sps.gaps_in_frame_num_allowed_flag = get_bits1(dstbuffer);//gaps_in_frame_num_allowed_flag
			sps.mb_width = get_ue_golomb(dstbuffer) + 1;//pic_width_in_mbs_minus1 + 1
			sps.mb_height= get_ue_golomb(dstbuffer) + 1;//pic_height_in_map_units_minus1 + 1

			nWidth  = sps.mb_width<<4;
			nHeight = sps.mb_height<<4;
		}
	}

	if (nWidth  < 0||nWidth  > 640)  nWidth  = 0;
	if (nHeight < 0||nHeight > 480)  nHeight = 0;

	return TRUE;
}

BOOL IsStandardH264Info(char *szFmtpConfig)
{
	char *token = NULL;

	char *pSPS = strstr(szFmtpConfig,"fmtp=100");
	if (pSPS == NULL) return FALSE;

	return TRUE;
}

int SwitchSPS(char * strSps,BYTE *sps)
{
	int len = strlen(strSps);
	if(len == 0) return 0;

	CString str = strSps;
	CString tmp = "";
	int bit = 0;

	for(int i = 0;i < len/2;i++)
	{
		tmp = str.Mid(i*2,2);
		sscanf(tmp,"%02X ",&bit);
		sps[i] = bit;
	}
	return len/2;
}