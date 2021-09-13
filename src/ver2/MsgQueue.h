#pragma once

#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
#endif

#define ExternPointer void *

using MsgEnum = int;
using Msg = void *;
using MsgCallBack = void (*)(Msg msg);

class MsgQueue
{
	MsgQueue();
	MsgQueue(int containerSize);
	MsgQueue(int containerSize, int consumerNum);
	~MsgQueue();
	void RegisterMsg(MsgEnum msgEnum, MsgCallBack cb);
	void PutMsg(MsgEnum msgEnum, Msg m);
	void SetConsumerNum(int num);
	void Start();
	void Stop();

protected:
	ExternPointer p;
};

#undef ExternPointer

#define MsgQueueDefaultSize 100
#define MsgQueueDefaultConsumerNum 3