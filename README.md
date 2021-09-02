# msg_queue

```c++
// construct a msg_queue object.
msg_queue *mq = new msg_queue();

// call set_max_thread_num to set max concurrent thread.
// this is useful when you need only one thread to keep
// queue in order.
// default 5.
mq->set_max_thread_num(1);

// register a msg
// @param msg_enum int
// @param msgcb void (*msgcb)(void *)
mq->register_msg(msgEnum,msgCallback);

// start this msg_queue.
mq->start();

// put msg into queue.
mq->put_msg(msgEnum,(void*)anyObjPtr);

// look /demo/main.cpp for more usage.

```





