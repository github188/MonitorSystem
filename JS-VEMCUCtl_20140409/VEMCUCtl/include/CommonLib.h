/**
 * @file    CommonLib.h
 * @brief   Include all data structs and interfaces what can be used in CommonLib.
 * @version	1.0
 * @author  Y.J
 * @date    2010-10-1
 */
#ifndef _COMMONLIB_H_
#define _COMMINLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <asm/types.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <semaphore.h>
#include <pthread.h>
#endif
#endif

#ifdef __KERNEL__
#define KMALLOC_MODE 0
#define VMALLOC_MODE 1
#define OSMALLOC_MODE 2
#endif

typedef struct _RWLock
{
#ifdef __KERNEL__
	rwlock_t lock;
#elif defined WIN32
	CRITICAL_SECTION	hReaderLock;    
	CRITICAL_SECTION	hWriterLock;
	int		nReaderCount;
#else
	pthread_rwlock_t lock;
#endif
} COMMONRWLOCK,*PCOMMONRWLOCK;

#ifdef __KERNEL__
typedef struct list_head COMMONLISTHEAD,*PCOMMONLISTHEAD;
#else
typedef struct list_head {
	struct list_head *next, *prev;
}COMMONLISTHEAD,*PCOMMONLISTHEAD;
#endif

typedef struct _list{
	unsigned char free;
	int count;
	unsigned long currentindex;
	int maxkey;
	int handle;
	int index;
	COMMONRWLOCK lock;
	COMMONLISTHEAD list;
}COMMONLIST,*PCOMMONLIST;

typedef struct _list_node{
	COMMONLISTHEAD list;
	int key;
	int index;
	void *data;
}COMMONLISTNODE,*PCOMMONLISTNODE;

typedef struct _array{
	COMMONRWLOCK lock;
	int handle;
	int maxcount;
	int count;
	int nodesize;
	unsigned char *used;
	int *index;
	unsigned char free;
	int *key;
	char* array;
}COMMONARRAY,*PCOMMONARRAY;

#define NOTIFYFIFO 1
#define POLLINGFIFO 2
#define CACHEFIFO 4

typedef struct _fifo_node{
	int len;
	char *buf;
}COMMONFIFONODE,*PCOMMONFIFONODE;

typedef struct _fifo{
	int handle;
	int write;
	int read;
	int num;
	int nodesize;
	int free;
	unsigned char stop:1;
	unsigned char type:4;
	unsigned char padding:3;
	int waiter;
#ifdef WIN32
	CRITICAL_SECTION lock;
	HANDLE notempty;
	HANDLE notfull;
#elif defined __KERNEL__
	spinlock_t lock;
	struct semaphore notempty;
	struct semaphore notfull;
#else
	pthread_mutex_t lock;
	sem_t notempty;
	sem_t notfull;
#endif
	char* pnode;
}COMMONFIFO,*PCOMMONFIFO;

#define NORMALTHREADPOOL 1
#define SINGLEFIFOTHREADPOOL 2
#define COPYARG 4

typedef struct _thread_info{
#ifdef WIN32
	HANDLE handle;
	unsigned int id;
#elif defined __KERNEL__
	struct task_struct * task;
#else
	pthread_t id;
#endif
	int index;
	struct _thread_pool *pool;
}COMMONTHREADINFO,*PCOMMONTHREADINFO;

typedef struct _thread_pool{
	unsigned char shutdown:1;
	unsigned char type:3;
	unsigned char padding:4;
	int threadsnum;
	int fifonum;
	int handle;
	PCOMMONTHREADINFO threads;
	PCOMMONFIFO fifo;
}COMMONTHREADPOOL,*PCOMMONTHREADPOOL;

#define LISTHASHTABLE 1
#define ARRAYHASHTABLE 2
#define BUCKETSORT 4

typedef struct _hash_table{
	int bucketsize;
	int handle;
	unsigned char type:3;
	unsigned char padding:5;
	PCOMMONLIST list;
	PCOMMONARRAY array;
}COMMONHASHTABLE,*PCOMMONHASHTABLE;

#define SINGLESHOTTIMER		0
#define REPEATINGTIMER		1

#define LZW 0

#define DFSLDR 0
#define DFSDLR 1
#define DFSLRD 2
#define BFS    3

typedef struct _tree{
	COMMONRWLOCK lock;
	int handle;
	struct _tree_node *root;
}COMMONTREE,*PCOMMONTREE;

typedef struct _tree_node{
	int sonindex;
	int sonnum;
	int usesonnum;
	char traversing;
	void* data;
	struct _tree_node *proot;
	struct _tree_node *pparent;
	struct _tree_node **pson;
}COMMONTREENODE,*PCOMMONTREENODE;

#define NETHANDLE int

#define NETSERVER 0
#define NETCLIENT 1

#define TCP 2
#define UDP 4
#define ICMP 8

typedef enum _NET_WORK_MODE{
	NET_SELECT,
	NET_WSASELECT,
	NET_WSAOVERLAPPED,
	NET_IOCP,
	NET_EPOLL
}NET_MODE;

#define ICMP_ECHO_REPLY 			0
#define ICMP_DEST_UNREACH 			3
#define ICMP_SOURCE_QUENCH 			4
#define ICMP_REDIRECT_MESSAGE		5
#define ICMP_ECHO_REQUEST 			8
#define ICMP_TIME_EXCEEDED			11
#define ICMP_PARAMETER_PROBLEM		12
#define ICMP_TIMESTAMP_REQUEST		13
#define ICMP_TIMESTAMP_REPLY		14
#define ICMP_INFORMATION_REQUEST	15
#define ICMP_INFORMATION_REPLY		16
#define ICMP_ADDRESSMASK_REQUEST	17
#define ICMP_ADDRESSMASK_REPLY		18
#define ICMP_MIN 8

#define ICMP_NETWORK_UNREACHABLE													0
#define ICMP_HOST_UNREACHABLE														1
#define ICMP_PROTOCOL_UNREACHABLE													2
#define ICMP_PORT_UNREACHABLE														3
#define ICMP_FRAGMENTATION_NEED_DF_NOT_SET											4
#define ICMP_SOURCE_ROUTE_FAILED													5
#define ICMP_DESTINATION_NETWORK_UNKNOWN											6
#define ICMP_DESTINATION_HOST_UNKNOWN												7
#define ICMP_SOURCE_HOST_UNKNOWN													8
#define ICMP_COMMUNICATION_WITH_DESTINATION_NETWORK_ADMINISTRATIVELY_PROHIBITED		9
#define ICMP_COMMUNICATION_WITH_DESTINATION_HOST__ADMINISTRATIVELY_PROHIBITED		10
#define ICMP_NETWORK_UNREACHABLE_FOR_TYPE_OF_SERVICE								11
#define ICMP_HOST_UNREACHABLE_FOR_TYPE_OF_SERVICE									12
#define ICMP_COMMUNICATION_ADMINISTRATIVELY_PROHIBITED_BY_FILTERING					13

#define ICMP_REDIRECT_DATAGRAMS_FOR_THE_NETWORK										0
#define ICMP_REDIRECT_DATAGRAMS_FOR_THE_HOST										1
#define ICMP_REDIRECT_DATAGRAMS_FOR_THE_TYPE_OF_SERVICE_AND_NETWORK					2
#define ICMP_REDIRECT_DATAGRAMS_FOR_THE_TYPE_OF_SERVICE_AND_HOST					3

typedef struct _net_info{
	unsigned long localip;
	unsigned short localport;
	unsigned long destip;
	unsigned short destport;
	unsigned char protocol;
	unsigned char type;
	unsigned char code;
	unsigned short ttl;
	unsigned long timestamp;
	unsigned short id;
	unsigned short seq;
	unsigned long socketbufsize[2];
	unsigned long bufsize[2];
}NETINFO,*PNETINFO;

#define MAXHTTPHEADER 8
#define MAXHTTPMSG 16

#define HTTPPOST 0
#define HTTPGET 1
#define HTTPRESPONSE 2

#define DEFAULTHTTPREQSIZE 64
#define DEFAULTHTTPREASONSIZE 256
#define DEFAULTHTTPHOSTSIZE 16
#define DEFAULTHTTPCONTENTTYPESIZE 32
#define DEFAULTCONNECTIONSIZE 64
#define DEFAULTHTTPBODYSIZE 1024*1024

enum http_code{
	HTTPOK=0,
	HTTPNOTFOUND,
	HTTPERROR,
};

enum http_version{
	HTTP10=0,
	HTTP11,
};

typedef struct _http_msg{
	int type;
	char req[DEFAULTHTTPREQSIZE];
	int reqsize;
	char reason[DEFAULTHTTPREASONSIZE];
	int reasonsize;
	int code;
	char content_type[DEFAULTHTTPCONTENTTYPESIZE];
	int content_typesize;
	char host[DEFAULTHTTPHOSTSIZE];
	int connectionsize;
	int hostsize;
	int version;
	char connection[DEFAULTCONNECTIONSIZE];
	char body[DEFAULTHTTPBODYSIZE];
	int bodysize;
}HTTPMSG,*PHTTPMSG;

#define LOGINFO 1
#define LOGWARNING 2
#define LOGERROR 4
#define LOG2FILE 8

#ifdef WIN32
#ifdef COMMONLIB_EXPORTS
#define COMMONLIB_API __declspec(dllexport)
#else
#define COMMONLIB_API __declspec(dllimport)
#endif
COMMONLIB_API int ComGetVersion(int *version,char* info);

COMMONLIB_API int ComMemoryPoolInit(unsigned long start);
COMMONLIB_API void ComMemoryPoolFree(void);
COMMONLIB_API void *ComMemMalloc(int handle,int nBytes);
COMMONLIB_API void ComMemFree(int handle,void *pPrior);
COMMONLIB_API void *ComMemRealloc(int handle,void *pPrior, int nBytes);
COMMONLIB_API int ComMemRoundup(int handle,int n);
COMMONLIB_API int ComMemPoolOpen(int nByte,int nMinLog,unsigned char type);
COMMONLIB_API void ComMemPoolClose(int handle);

COMMONLIB_API int ComInitRWLock(PCOMMONRWLOCK lock);
COMMONLIB_API int ComDestroyRWLock(PCOMMONRWLOCK lock);
COMMONLIB_API int ComAcquireReadLock(PCOMMONRWLOCK lock);
COMMONLIB_API int ComReleaseReadLock(PCOMMONRWLOCK lock);
COMMONLIB_API int ComAcquireWriteLock(PCOMMONRWLOCK lock);
COMMONLIB_API int ComReleaseWriteLock(PCOMMONRWLOCK lock);

COMMONLIB_API int ComInitList(PCOMMONLIST plist,int handle);
COMMONLIB_API int ComFreeList(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API void ComLockList(PCOMMONLIST plist,int role);
COMMONLIB_API void ComUnLockList(PCOMMONLIST plist,int role);
COMMONLIB_API int ComGetListLength(PCOMMONLIST plist);
COMMONLIB_API int ComGetListNode(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void*s2,void** node,int* listindex);
COMMONLIB_API int ComGetListNodeByIndex(PCOMMONLIST plist,int listindex,void** node);
COMMONLIB_API int ComGetSortListNode(PCOMMONLIST plist,int key,void** node,int* listindex);
COMMONLIB_API int ComInsertListNode(PCOMMONLIST plist,void* data,int* listindex);
COMMONLIB_API int ComInsertSortListNode(PCOMMONLIST plist,void* data,int key,int* listindex);
COMMONLIB_API int ComDeleteListNode(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s1,void* s2);
COMMONLIB_API int ComDeleteListNodeByFunc(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteListNodeByIndex(PCOMMONLIST plist,int listindex,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteSortListNode(PCOMMONLIST plist,int key,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComTraversingList(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComTraversingListByIndex(PCOMMONLIST plist,int start,int end,int (*func)(void* s1,void* s2),void* s2);

COMMONLIB_API int ComInitArray(PCOMMONARRAY parray,int handle,int maxcount,int nodesize);
COMMONLIB_API int ComFreeArray(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API void ComLockArray(PCOMMONARRAY parray,int role);
COMMONLIB_API void ComUnLockArray(PCOMMONARRAY parray,int role);
COMMONLIB_API int ComGetArrayLength(PCOMMONARRAY parray);
COMMONLIB_API int ComGetArrayNode(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void*s2,void** node,int* arrayindex);
COMMONLIB_API int ComGetSortArrayNode(PCOMMONARRAY parray,int key,void** node,int* arrayindex);
COMMONLIB_API int ComGetArrayNodeByIndex(PCOMMONARRAY parray,int arrayindex,void** node);
COMMONLIB_API int ComInsertArrayNode(PCOMMONARRAY parray,void* data,int* arrayindex);
COMMONLIB_API int ComInsertSortArrayNode(PCOMMONARRAY parray,void* data,int key,int* arrayindex);
COMMONLIB_API int ComDeleteArrayNode(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s1,void* s2);
COMMONLIB_API int ComDeleteSortArrayNode(PCOMMONARRAY parray,int key,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteArrayNodeByFunc(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteArrayNodeByIndex(PCOMMONARRAY parray,int index,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComTraversingArray(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);

COMMONLIB_API int ComInitFifo(PCOMMONFIFO fifo,int num,int nodesize,unsigned char type,int handle);
COMMONLIB_API int ComFreeFifo(PCOMMONFIFO fifo,void (*freefunc)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComPopFifo(PCOMMONFIFO fifo,char* data,int* size,int num,int *realnum,int (*pop_cb)(char* data,int size,void* user),void* user);
COMMONLIB_API int ComPushFifo(PCOMMONFIFO fifo,char* data,int size,int num,int *realnum);
COMMONLIB_API int ComGetFifoFree(PCOMMONFIFO fifo);

COMMONLIB_API unsigned int ComHash3(unsigned int a,unsigned int b,unsigned int c,unsigned int bucketsize);
COMMONLIB_API unsigned int ComHashStr(char* str,unsigned int bucketsize);
COMMONLIB_API unsigned int ComGetStrHash(char* str);

COMMONLIB_API int ComInitThreadPool(PCOMMONTHREADPOOL pool,unsigned char type,int handle,int threadsnum,int fifosize);
COMMONLIB_API void ComFreeThreadPool(PCOMMONTHREADPOOL pool,int shutdown);
COMMONLIB_API int ComDispatchTask(PCOMMONTHREADPOOL pool,void (*pFun)(void *), void *arg,int argsize,int key);

COMMONLIB_API int ComInitHashTable(PCOMMONHASHTABLE ptable,unsigned char type,int handle,int bucketsize,int bucketnodenum,int nodesize);
COMMONLIB_API int ComFreeHashTable(PCOMMONHASHTABLE ptable,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API void ComLockHashTable(PCOMMONHASHTABLE ptable,int index,unsigned char role);
COMMONLIB_API void ComUnLockHashTable(PCOMMONHASHTABLE ptable,int index,unsigned char role);
COMMONLIB_API int ComGetHashTableRowLength(PCOMMONHASHTABLE ptable,int index);
COMMONLIB_API int ComGetHashTableNodeRow(PCOMMONHASHTABLE ptable,int index,int key,int (*func)(void* s1,void* s2),void* s2,void** node,int* listindex);
COMMONLIB_API int ComGetHashTableNode(PCOMMONHASHTABLE ptable,int key,int (*func)(void* s1,void* s2),void* s2,void** node,int* index,int* listindex,int role);
COMMONLIB_API int ComGetHashTableNodeByIndex(PCOMMONHASHTABLE ptable,int index,int listindex,void** node);
COMMONLIB_API int ComInsertHashTableNode(PCOMMONHASHTABLE ptable,int index,void* data,int key,int* listindex);
COMMONLIB_API int ComDeleteHashTableNode(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s1,void* s2);
COMMONLIB_API int ComDeleteHashTableNodeByFunc(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteHashTableNodeByIndex(PCOMMONHASHTABLE ptable,int index,int listindex,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComTraversingHashTable(PCOMMONHASHTABLE ptable,int (*func)(void* s1,void* s2),void* s2,int role);
COMMONLIB_API int ComTraversingHashTableRow(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s2);

COMMONLIB_API int ComInitCommpress(int handle);
COMMONLIB_API int ComFreeCommpress(int handle);
COMMONLIB_API int ComInitCommpressNode(int type,int handle,int bits,int (**compress)(int handle,char* in,char* out,int* len),int (**uncompress)(int handle,char* in,char* out,int* len));
COMMONLIB_API int ComFreeCommpressNode(int handle);

COMMONLIB_API int ComSetTimer(int handle,time_t s,time_t ms,unsigned char type,void (*func)(void* data),void* data,int datalen);
COMMONLIB_API int ComModTimer(int timer,time_t s,time_t ms,unsigned char type,void (*func)(void* data),void* data,int datalen);
COMMONLIB_API void ComDelTimer(int timer);

COMMONLIB_API int ComInitTree(PCOMMONTREE ptree,int handle);
COMMONLIB_API void ComFreeTree(PCOMMONTREE ptree,int handle,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API void ComLockTree(PCOMMONTREE ptree,int role);
COMMONLIB_API void ComUnLockTree(PCOMMONTREE ptree,int role);
COMMONLIB_API int GetTreeNodeByIndex(PCOMMONTREE ptree,int level,int brother,int son,void** node);
COMMONLIB_API int GetTreeNodeByFunc(PCOMMONTREE ptree,int type,int queuesize,int (*func)(void* s1,void*s2),void* s2,void** node);
COMMONLIB_API int ComInsertTreeNode(PCOMMONTREE ptree,int level,int brother,int son,int sonnum,void* data);
COMMONLIB_API int ComDeleteTreeNode(PCOMMONTREE ptree,int type,int queuesize,int newparent,int (*func)(void* s1,void* s2),void* s1,void* s2);
COMMONLIB_API int ComDeleteTreeNodeByFunc(PCOMMONTREE ptree,int type,int queuesize,int newparent,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComDeleteTreeNodeByIndex(PCOMMONTREE ptree,int level,int brother,int son,int newparent,int (*func)(void* s1,void* s2),void* s2);
COMMONLIB_API int ComTraversingTree(PCOMMONTREE ptree,int type,int queuesize,int (*func)(void* s1,void* s2),void* s2);

COMMONLIB_API int ComInitNet(int handle,int mode,int maxserver,int maxclient);
COMMONLIB_API NETHANDLE ComOpenNet(char* localip,unsigned short localport,char* destip,unsigned short destport,char type,char protocol,int recvbufsize,int socketrecvbufsize,int socketsendbufsize,int ttl,int handle,int clientnum,int (*read_cb)(NETHANDLE handle,char* buf,int size,void* user),void* user,int tcptimeout,int incb);
COMMONLIB_API int ComSendNet(NETHANDLE handle,char *destip,unsigned short destport,unsigned char icmptype,unsigned char icmpcode,unsigned short icmpseq,unsigned short icmpid,char* buf,int size,int incb);
COMMONLIB_API int ComCloseNet(NETHANDLE handle,int incb);
COMMONLIB_API int ComFreeNet();
COMMONLIB_API int ComGetNetInfo(NETHANDLE handle,PNETINFO info,int incb);

COMMONLIB_API int ComHttpMsgParse(char* buf,int size,PHTTPMSG msg);
COMMONLIB_API int ComHttpMsgBuild(char* buf,PHTTPMSG msg);


COMMONLIB_API void ComLog(int level,const char* formate,...);
COMMONLIB_API int ComInitLog(int handle,char* logname,int logworkernum,int loglevel);
COMMONLIB_API void ComFreeLog();
#else
extern int ComGetVersion(int *version,char* info);

extern int ComMemoryPoolInit(unsigned long start);
extern void ComMemoryPoolFree(void);
extern void *ComMemMalloc(int handle,int nBytes);
extern void ComMemFree(int handle,void *pPrior);
extern void *ComMemRealloc(int handle,void *pPrior, int nBytes);
extern int ComMemRoundup(int handle,int n);
extern int ComMemPoolOpen(int nByte,int nMinLog,unsigned char type);
extern void ComMemPoolClose(int handle);

extern int ComInitRWLock(PCOMMONRWLOCK lock);
extern int ComDestroyRWLock(PCOMMONRWLOCK lock);
extern int ComAcquireReadLock(PCOMMONRWLOCK lock);
extern int ComReleaseReadLock(PCOMMONRWLOCK lock);
extern int ComAcquireWriteLock(PCOMMONRWLOCK lock);
extern int ComReleaseWriteLock(PCOMMONRWLOCK lock);

extern int ComInitList(PCOMMONLIST plist,int handle);
extern int ComFreeList(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
extern void ComLockList(PCOMMONLIST plist,int role);
extern void ComUnLockList(PCOMMONLIST plist,int role);
extern int ComGetListLength(PCOMMONLIST plist);
extern int ComGetListNode(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void*s2,void** node,int* listindex);
extern int ComGetListNodeByIndex(PCOMMONLIST plist,int listindex,void** node);
extern int ComGetSortListNode(PCOMMONLIST plist,int key,void** node,int* listindex);
extern int ComInsertListNode(PCOMMONLIST plist,void* data,int* listindex);
extern int ComInsertSortListNode(PCOMMONLIST plist,void* data,int key,int* listindex);
extern int ComDeleteListNode(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s1,void* s2);
extern int ComDeleteListNodeByFunc(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteListNodeByIndex(PCOMMONLIST plist,int listindex,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteSortListNode(PCOMMONLIST plist,int key,int (*func)(void* s1,void* s2),void* s2);
extern int ComTraversingList(PCOMMONLIST plist,int (*func)(void* s1,void* s2),void* s2);
extern int ComTraversingListByIndex(PCOMMONLIST plist,int start,int end,int (*func)(void* s1,void* s2),void* s2);

extern int ComInitArray(PCOMMONARRAY parray,int handle,int maxcount,int nodesize);
extern int ComFreeArray(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);
extern void ComLockArray(PCOMMONARRAY parray,int role);
extern void ComUnLockArray(PCOMMONARRAY parray,int role);
extern int ComGetArrayLength(PCOMMONARRAY parray);
extern int ComGetArrayNode(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void*s2,void** node,int* arrayindex);
extern int ComGetSortArrayNode(PCOMMONARRAY parray,int key,void** node,int* arrayindex);
extern int ComGetArrayNodeByIndex(PCOMMONARRAY parray,int arrayindex,void** node);
extern int ComInsertArrayNode(PCOMMONARRAY parray,void* data,int* arrayindex);
extern int ComInsertSortArrayNode(PCOMMONARRAY parray,void* data,int key,int* arrayindex);
extern int ComDeleteArrayNode(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s1,void* s2);
extern int ComDeleteSortArrayNode(PCOMMONARRAY parray,int key,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteArrayNodeByFunc(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteArrayNodeByIndex(PCOMMONARRAY parray,int index,int (*func)(void* s1,void* s2),void* s2);
extern int ComTraversingArray(PCOMMONARRAY parray,int (*func)(void* s1,void* s2),void* s2);

extern int ComInitFifo(PCOMMONFIFO fifo,int num,int nodesize,unsigned char type,int handle);
extern int ComFreeFifo(PCOMMONFIFO fifo,void (*freefunc)(void* s1,void* s2),void* s2);
extern int ComPopFifo(PCOMMONFIFO fifo,char* data,int* size,int num,int *realnum,int (*pop_cb)(char* data,int size,void* user),void* user);
extern int ComPushFifo(PCOMMONFIFO fifo,char* data,int size,int num,int *realnum);
extern int ComGetFifoFree(PCOMMONFIFO fifo);

extern unsigned int ComHash3(unsigned int a,unsigned int b,unsigned int c,unsigned int bucketsize);
extern unsigned int ComHashStr(char* str,unsigned int bucketsize);
extern unsigned int ComGetStrHash(char* str);

extern int ComInitThreadPool(PCOMMONTHREADPOOL pool,unsigned char type,int handle,int threadsnum,int fifosize);
extern void ComFreeThreadPool(PCOMMONTHREADPOOL pool,int shutdown);
extern int ComDispatchTask(PCOMMONTHREADPOOL pool,void (*pFun)(void *), void *arg,int argsize,int key);

extern int ComInitHashTable(PCOMMONHASHTABLE ptable,unsigned char type,int handle,int bucketsize,int bucketnodenum,int nodesize);
extern int ComFreeHashTable(PCOMMONHASHTABLE ptable,int (*func)(void* s1,void* s2),void* s2);
extern void ComLockHashTable(PCOMMONHASHTABLE ptable,int index,unsigned char role);
extern void ComUnLockHashTable(PCOMMONHASHTABLE ptable,int index,unsigned char role);
extern int ComGetHashTableRowLength(PCOMMONHASHTABLE ptable,int index);
extern int ComGetHashTableNodeRow(PCOMMONHASHTABLE ptable,int index,int key,int (*func)(void* s1,void* s2),void* s2,void** node,int* listindex);
extern int ComGetHashTableNode(PCOMMONHASHTABLE ptable,int key,int (*func)(void* s1,void* s2),void* s2,void** node,int* index,int* listindex,int role);
extern int ComGetHashTableNodeByIndex(PCOMMONHASHTABLE ptable,int index,int listindex,void** node);
extern int ComInsertHashTableNode(PCOMMONHASHTABLE ptable,int index,void* data,int key,int* listindex);
extern int ComDeleteHashTableNode(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s1,void* s2);
extern int ComDeleteHashTableNodeByFunc(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteHashTableNodeByIndex(PCOMMONHASHTABLE ptable,int index,int listindex,int (*func)(void* s1,void* s2),void* s2);
extern int ComTraversingHashTable(PCOMMONHASHTABLE ptable,int (*func)(void* s1,void* s2),void* s2,int role);
extern int ComTraversingHashTableRow(PCOMMONHASHTABLE ptable,int index,int (*func)(void* s1,void* s2),void* s2);

extern int ComInitCommpress(int handle);
extern int ComFreeCommpress(int handle);
extern int ComInitCommpressNode(int type,int handle,int bits,int (**compress)(int handle,char* in,char* out,int* len),int (**uncompress)(int handle,char* in,char* out,int* len));
extern int ComFreeCommpressNode(int handle);

extern int ComSetTimer(int handle,time_t s,time_t ms,unsigned char type,void (*func)(void* data),void* data,int datalen);
extern int ComModTimer(int timer,time_t s,time_t ms,unsigned char type,void (*func)(void* data),void* data,int datalen);
extern void ComDelTimer(int timer);

extern int ComInitTree(PCOMMONTREE ptree,int handle);
extern void ComFreeTree(PCOMMONTREE ptree,int handle,int (*func)(void* s1,void* s2),void* s2);
extern void ComLockTree(PCOMMONTREE ptree,int role);
extern void ComUnLockTree(PCOMMONTREE ptree,int role);
extern int GetTreeNodeByIndex(PCOMMONTREE ptree,int level,int brother,int son,void** node);
extern int GetTreeNodeByFunc(PCOMMONTREE ptree,int type,int queuesize,int (*func)(void* s1,void*s2),void* s2,void** node);
extern int ComInsertTreeNode(PCOMMONTREE ptree,int level,int brother,int son,int sonnum,void* data);
extern int ComDeleteTreeNode(PCOMMONTREE ptree,int type,int queuesize,int newparent,int (*func)(void* s1,void* s2),void* s1,void* s2);
extern int ComDeleteTreeNodeByFunc(PCOMMONTREE ptree,int type,int queuesize,int newparent,int (*func)(void* s1,void* s2),void* s2);
extern int ComDeleteTreeNodeByIndex(PCOMMONTREE ptree,int level,int brother,int son,int newparent,int (*func)(void* s1,void* s2),void* s2);
extern int ComTraversingTree(PCOMMONTREE ptree,int type,int queuesize,int (*func)(void* s1,void* s2),void* s2);

extern int ComInitNet(int handle,int mode,int maxserver,int maxclient);
extern NETHANDLE ComOpenNet(char* localip,unsigned short localport,char* destip,unsigned short destport,char type,char protocol,int recvbufsize,int socketrecvbufsize,int socketsendbufsize,int ttl,int handle,int clientnum,int (*read_cb)(NETHANDLE handle,char* buf,int size,void* user),void* user,int tcptimeout,int incb);
extern int ComSendNet(NETHANDLE handle,char *destip,unsigned short destport,unsigned char icmptype,unsigned char icmpcode,unsigned short icmpseq,unsigned short icmpid,char* buf,int size,int incb);
extern int ComCloseNet(NETHANDLE handle,int incb);
extern int ComFreeNet();
extern int ComGetNetInfo(NETHANDLE handle,PNETINFO info,int incb);

extern int ComHttpMsgParse(char* buf,int size,PHTTPMSG msg);
extern int ComHttpMsgBuild(char* buf,PHTTPMSG msg);

extern void ComLog(int level,const char* formate...);
extern int ComInitLog(int handle,char* logname,int logworkernum,int loglevel);
extern void ComFreeLog();
#endif
#ifdef __cplusplus
}
#endif
#endif
