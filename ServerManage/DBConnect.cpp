#include "StdAfx.h"
#include "DBConnect.h"

extern _T_ASS_RVU_INFO_ g_tAssRVU[MAX_RVU_NUM_PER_STATION]; 
extern int g_nAssRVUNum;

void GetRVUInfo()
{
   int i=0;
   g_nAssRVUNum = 1;
   char *ip="127.0.0.1";

   for(i=0;i<g_nAssRVUNum;i++)
   {
	  

	   sprintf(g_tAssRVU[i].rvu_ip,"%s",ip);
	   
	   g_tAssRVU[i].rvu_port=6000;
   }
}