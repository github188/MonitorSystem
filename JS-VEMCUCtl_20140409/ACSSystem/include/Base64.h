#ifndef _BASE_64_INCLUDE_
#define _BASE_64_INCLUDE_

BOOL Base64Encode(unsigned char * src,int srclen,unsigned char * dst,int &dstlen);
BOOL Base64Decode(unsigned char * src,int srclen,unsigned char * dst,int &dstlen);

#endif