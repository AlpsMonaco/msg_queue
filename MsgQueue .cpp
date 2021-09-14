#include "MsgQueue.h"
#include <unordered_map>
#include <atomic>
#include <thread>

#define msgQueueImpl ((MsgQueueImpl *)this->p)

struct MsgPayload
{
	MsgEnum msgEnum;
	Msg msg;

	MsgPayload(MsgEnum msgEnum, Msg msg)
	{
		this->msgEnum = msgEnum;
		this->msg = msg;
	}
};

class MsgQueueImpl
{
public:
	MsgQueueImpl();
	~MsgQueueImpl();
	MsgQueueImpl(int containerSize);
	MsgQueueImpl(int containerSize, int consumerNum);
	// void Start();
	void Stop();

	void RegisterMsg(MsgEnum msgEnum, MsgCallBack cb);
	void SetConsumerNum(int num);
	void PutMsg(MsgEnum msgEnum, Msg m);
	int GetQueuePosition(int &num);
	void NewConsumer();

protected:
	MsgPayload **container;
	std::unordered_map<MsgEnum, MsgCallBack> *callbackMap;

	int containerSize;
	int consumerNum;

	std::atomic<int> msgCount;
	std::atomic<int> inQueueMsgCount;
	std::atomic<int> msgDoneCount;
	std::atomic<int> msgId;

	std::atomic<int> runningConsumerNum;

	void ConsumerMethod();
};

void MsgQueueImpl::NewConsumer()
{
	this->ConsumerMethod();
}

void MsgQueueImpl::ConsumerMethod()
{
	int loopNum = 0;
	int totalMsgNum = 0;
	int msgDoneNum = 0;
	int msgId = 0;

	for (;;)
	{
		if (loopNum >= 100)
		{
			break;
		}

		totalMsgNum = msgCount;
		msgDoneNum = msgDoneCount;

		if (totalMsgNum - msgDoneNum > 0)
		{
			msgId = inQueueMsgCount++;
			if (msgId > totalMsgNum - 1)
			{
				inQueueMsgCount--;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				loopNum++;
				continue;
			}

			MsgPayload *mp = container[GetQueuePosition(msgId)];
			// if (mp != nullptr)
			// {
			auto it = this->callbackMap->find(mp->msgEnum);
			if (it != this->callbackMap->end())
			{
				it->second(mp->msg);
			}
			msgDoneCount++;
			delete mp;
			// container[GetQueuePosition(msgId)] = nullptr;
			// }
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			loopNum++;
			continue;
		}
	}
	runningConsumerNum--;
}

void MsgQueue::Stop()
{
	msgQueueImpl->Stop();
}

void MsgQueueImpl::Stop()
{
	int totalMsgNum = 0;
	int msgDoneNum = 0;

	for (;;)
	{
		totalMsgNum = msgCount;
		msgDoneNum = msgDoneCount;

		if (totalMsgNum - msgDoneNum > 0)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else
		{
			break;
		}
	}
}

int MsgQueueImpl::GetQueuePosition(int &num)
{
	return num % containerSize;
}

void MsgQueue::SetConsumerNum(int num)
{
	msgQueueImpl->SetConsumerNum(num);
}

void MsgQueueImpl::SetConsumerNum(int num)
{
	this->consumerNum = num;
}

void MsgQueue::PutMsg(MsgEnum msgEnum, Msg m)
{
	msgQueueImpl->PutMsg(msgEnum, m);
}

void MsgQueueImpl::PutMsg(MsgEnum msgEnum, Msg m)
{
	int msgId = this->msgId++;
	int msgCount = 0;
	int msgDoneNum = 0;

	int consumerNum = this->consumerNum;
	if (this->runningConsumerNum < consumerNum)
	{
		runningConsumerNum++;
		std::thread([this]() -> void
					{ this->NewConsumer(); })
			.detach();
	}

	for (;;)
	{
		msgDoneNum = this->msgDoneCount;
		msgCount = this->msgCount;
		if (msgCount - msgDoneNum >= containerSize)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else
		{
			this->container[GetQueuePosition(msgId)] = new MsgPayload(msgEnum, m);
			this->msgCount++;
			break;
		}
	}
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
	this->msgCount = 0;
	this->inQueueMsgCount = 0;
	this->msgDoneCount = 0;
	this->runningConsumerNum = 0;
	this->msgId = 0;

	this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
	this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize)
{
	this->containerSize = containerSize;
	this->consumerNum = MsgQueueDefaultConsumerNum;
	this->msgCount = 0;
	this->inQueueMsgCount = 0;
	this->msgDoneCount = 0;
	this->runningConsumerNum = 0;
	this->msgId = 0;

	this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
	this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize, int consumerNum)
{
	this->containerSize = containerSize;
	this->consumerNum = consumerNum;
	this->msgCount = 0;
	this->inQueueMsgCount = 0;
	this->msgDoneCount = 0;
	this->runningConsumerNum = 0;
	this->msgId = 0;

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