#pragma once

//////////////////////////////////////////////////////////////////////////
#define  RECV_BUF_MAX_COUNT (64*1024)
#define  RECV_BUF_NODE_COUNT (200)
//////////////////////////////////////////////////////////////////////////

typedef struct _b_recv_buf_node_type
{
	int DataLen;
	unsigned char Buf[RECV_BUF_MAX_COUNT];
}Recv_Buf_Node_Type;

//构造一个接受缓冲队列
typedef struct _recv_buf_queue_
{
	int front;                                        //头指针
	int rear;                                         //尾指针
	int number;                                   //当前队列的结点数
	Recv_Buf_Node_Type Node[RECV_BUF_NODE_COUNT];     //存储区
}Recv_Buf_Queue;

void InitRecvQueue(Recv_Buf_Queue &RecvQueue);//初始化队列
void ClearRecvQueue(Recv_Buf_Queue &RecvQueue);//清空队列
BOOL RecvQueueEmpty(Recv_Buf_Queue &RecvQueue);//判断队列是否为空
BOOL RecvQueueFull(Recv_Buf_Queue &RecvQueue);//判断队列是否已满
int  GetRecvHead(Recv_Buf_Queue &RecvQueue,Recv_Buf_Node_Type &Node);//得到队列头
int  GetRecvNodeNumber(Recv_Buf_Queue &RecvQueue);//得到当前队列的结点数
int  RecvEnQueue(Recv_Buf_Queue &RecvQueue,Recv_Buf_Node_Type &Node);//入队列
int  RecvDeQueue(Recv_Buf_Queue &RecvQueue,Recv_Buf_Node_Type &Node);//出队列
