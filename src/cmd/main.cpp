
#include "../util.h"
#include "../msg_queue.h"

#include <map>
#include <thread>

void p(void *s)
{
	println((char *)s);
}

int main(int argc, char **argv)
{
	msg_queue *mq = new msg_queue();
	mq->register_msg(1, p);
	mq->set_max_thread_num(1);
	mq->start();

	void *p;

	for (int i = 0; i < 100; i++)
	{
		char *tmp = new char[20];
		sprintf(tmp, "this is %d", i);
		mq->put_msg(1, (void *)tmp);
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}