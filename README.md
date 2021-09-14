# MsgQueue

a c++ msg queue lib

```c++
// construct a msg_queue object.
MsgQueue *mq = new MsgQueue();
// could also construct by containerSize or consumerNum
// MsgQueue *mq = MsgQueueImpl(int containerSize);
// MsgQueue *mq = MsgQueueImpl(int containerSize, int consumerNum);


// register a msg
// @param msgEnum int
// @param msgcallback void (*msgcb)(void *)
int msgEnum = 0;
mq->RegisterMsg(msgEnum,[](void* msg)->void{
    std::cout << *((int*) msg) <<std::endl;
    delete (int*) msg;
});

// put msg into queue.
int *pInt = new int;
*pInt = 123;
mq->PutMsg(msgEnum,pInt);
// task will work automatically.

// demo/main.cpp for more usage.

```





