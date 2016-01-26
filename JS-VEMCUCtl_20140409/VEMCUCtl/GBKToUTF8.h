#ifndef _GBK_TO_UTF8_H_
#define _GBK_TO_UTF8_H_

BOOL ConvertGBKToUTF8(char *szGBK,char *szUTF8,int nUTF8BufSize,int &nRealLen);//将GBK转换成UTF8
BOOL ConvertUTF8ToGBK(char *szUTF8,char *szGBK,int nGBKBufSize,int &nRealLen);//将UTF8转换成GBK

#endif