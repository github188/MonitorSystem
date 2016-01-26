#include "stdafx.h"
#include "Base64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const unsigned char base64alphabet[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const unsigned char base64table[128] =
{
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
	 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,  0,255,255,
	255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,12, 13, 14,
	 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
	255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255
};

//Base64±àÂë
BOOL Base64Encode(unsigned char * src,int srclen,unsigned char * dst,int &dstlen)
{
	unsigned char FirstChar,SecondChar,ThirdChar;
	dstlen = 0;

	while (srclen >= 3)
	{
		FirstChar  = *src++;
		SecondChar = *src++;
		ThirdChar  = *src++;
		*dst++ = base64alphabet[FirstChar>>2];
		*dst++ = base64alphabet[FirstChar<<4&0x3f|SecondChar>>4];
		*dst++ = base64alphabet[SecondChar<<2&0x3f|ThirdChar>>6];
		*dst++ = base64alphabet[ThirdChar&0x3f];
		srclen -= 3;
		dstlen += 4;
	}

	if (srclen == 2)
	{
		FirstChar  = *src++;
		SecondChar = *src;
		*dst++ = base64alphabet[FirstChar>>2];
		*dst++ = base64alphabet[FirstChar<<4&0x3f|SecondChar>>4];
		*dst++ = base64alphabet[SecondChar<<2&0x3f];
		*dst   = '=';
		dstlen += 4;
	}
	else if (srclen ==1)
	{
		*dst++ = base64alphabet[*src>>2];
		*dst++ = base64alphabet[*src<<4&0x3f];
		*dst++ = '=';
		*dst   = '=';
		dstlen += 4;
	}

	return TRUE;
}

//Base64½âÂë
BOOL Base64Decode(unsigned char * src,int srclen,unsigned char * dst,int &dstlen)
{
	unsigned char FirstChar,SecondChar,ThirdChar,FourthChar;
	dstlen = 0;

	while(srclen > 4)
	{
		FirstChar  = base64table[*src++];
		SecondChar = base64table[*src++];
		ThirdChar  = base64table[*src++];
		FourthChar = base64table[*src++];
		*dst++ = FirstChar<<2|SecondChar>>4;
		*dst++ = SecondChar<<4|ThirdChar>>2;
		*dst++ = ThirdChar<<6|FourthChar;
		srclen -= 4;
		dstlen += 3;
	}

	FirstChar  = base64table[*src++];
	SecondChar = base64table[*src++];
	ThirdChar  = base64table[*src++];
	FourthChar = base64table[*src];

	if (*src-- == '=')
	{
		if (*src == '=')
		{
			*dst++ = FirstChar<<2|SecondChar>>4;
			dstlen++;
		}
		else 
		{
			*dst++ = FirstChar<<2|SecondChar>>4;
			*dst++ = SecondChar<<4|ThirdChar>>2;
			dstlen += 2;
		}
	}
	else
	{
		*dst++ = FirstChar<<2|SecondChar>>4;
		*dst++ = SecondChar<<4|ThirdChar>>2;
		*dst++ = ThirdChar<<6|FourthChar;
		dstlen += 3;
	}

	return TRUE;
}