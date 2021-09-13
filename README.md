# msg_queue

```c++
// construct a msg_queue object.
MsgQueue *mq = new MsgQueue();

// register a msg
// @param msg_enum int
// @param msgcb void (*msgcb)(void *)
mq->RegisterMsg(msgEnum,msgCallback);

// put msg into queue.
mq->PutMsg(msgEnum,(void*)anyObjPtr);

// look /demo/main.cpp for more usage.

```





