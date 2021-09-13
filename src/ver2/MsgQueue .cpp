#include "MsgQueue.h"
#include <unordered_map>

#define msgQueueImpl ((MsgQueueImpl *)this->p)

struct MsgPayload
{
	MsgEnum msgEnum;
	Msg msg;
};

class MsgQueueImpl
{
public:
	MsgQueueImpl();
	~MsgQueueImpl();
	MsgQueueImpl(int containerSize);
	MsgQueueImpl(int containerSize, int consumerNum);

	void RegisterMsg(MsgEnum msgEnum, MsgCallBack cb);
	void SetConsumerNum(int num);

protected:
	MsgPayload **container;
	std::unordered_map<MsgEnum, MsgCallBack> *callbackMap;

	int containerSize;
	int consumerNum;
};

void MsgQueue::SetConsumerNum(int num)
{
	msgQueueImpl->SetConsumerNum(num);
}

void MsgQueueImpl::SetConsumerNum(int num)
{
	this->consumerNum = num;
}

void MsgQueueImpl::RegisterMsg(MsgEnum msgEnum, MsgCallBack cb)
{
	(*this->callbackMap)[msgEnum] = cb;
}

void MsgQueue::RegisterMsg(MsgEnum msgEnum, MsgCallBack cb)
{
	msgQueueImpl->RegisterMsg(msgEnum, cb);
}

MsgQueueImpl::MsgQueueImpl()
{
	this->containerSize = MsgQueueDefaultSize;
	this->consumerNum = MsgQueueDefaultConsumerNum;

	this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
	this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize)
{
	this->containerSize = containerSize;
	this->consumerNum = MsgQueueDefaultConsumerNum;

	this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
	this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize, int consumerNum)
{
	this->containerSize = containerSize;
	this->consumerNum = consumerNum;

	this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
	this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::~MsgQueueImpl()
{
	delete[] this->container;
	delete this->callbackMap;
}

MsgQueue::MsgQueue()
{
	this->p = new MsgQueueImpl();
}

MsgQueue::MsgQueue(int containerSize)
{
	this->p = new MsgQueueImpl(containerSize);
}

MsgQueue::MsgQueue(int containerSize, int consumerNum)
{
	this->p = new MsgQueueImpl(containerSize, consumerNum);
}