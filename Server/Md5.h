//Download by http://www.NewXing.com
// Md5.h: interface for the CMd5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MD5_H__7F117010_0179_4DB4_8908_E746E919E0C4__INCLUDED_)
#define AFX_MD5_H__7F117010_0179_4DB4_8908_E746E919E0C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//类型定义////////////////////////////////////////////////////////////
/* POINTER 定义成一个普通的指针类型 */
typedef unsigned char *POINTER;

/* UINT2 定义成两字节的字 */
typedef unsigned short int UINT2;

/* UINT4定一成四字节的字  */
typedef unsigned long int UINT4;
/////////////////////////////////////
/* MD5 context. */
typedef struct 
{
  UINT4 state[4];			/* state (ABCD) */
  UINT4 count[2];			/* 位数量, 模 2^64 (低位在前) */
  unsigned char buffer[64]; /* 输入缓冲器 */
} MD5_CTX;
/////////////////////////////////////
/* Constants for MD5Transform routine.*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


/* F, G, H 和 I 是基本MD5函数 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT 将x循环左移n位 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* 循环从加法中分离出是为了防止重复计算*/
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
//////////////////////////////////////////////////////////////////////

class CMd5
{
public:
	CMd5();
	virtual ~CMd5();

	void GetMD5CheckSum(BYTE* pbyInput,UINT nInputLen,BYTE* pbyCheckSum);

private:
	void MD5Init(MD5_CTX*);
	void MD5Update(MD5_CTX*,unsigned char*,unsigned int);
	void MD5Final(unsigned char [16], MD5_CTX*);

	void MD5Transform(UINT4 [4], unsigned char [64]);
	void Encode(unsigned char *, UINT4 *, unsigned int);
	void Decode(UINT4 *, unsigned char *, unsigned int);
	void MD5_memcpy(POINTER, POINTER, unsigned int);
	void MD5_memset(POINTER, int, unsigned int);

	unsigned char PADDING[64];
};

#endif // !defined(AFX_MD5_H__7F117010_0179_4DB4_8908_E746E919E0C4__INCLUDED_)
