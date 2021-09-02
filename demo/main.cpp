#include "../src/msg_queue.h"
#include <iostream>
#include <thread>
#pragma comment(lib, "../releases/x64/msg_queue.lib")

void printInt(void *msg)
{
	int *pInt;
	pInt = (int *)msg;
	std::cout << "this is int " << *pInt << std::endl;
	delete pInt;
}

void printString(void *msg)
{
	char *ptrChar;
	ptrChar = (char *)msg;
	std::cout << "this is char " << ptrChar << std::endl;
	delete[] ptrChar;
}

enum MsgEnum
{
	PrintInt = 0,
	PrintString = 1,
};

int main(int argc, char)
{
	msg_queue *mq = new msg_queue();
	mq->set_max_thread_num(1);

	mq->register_msg(PrintInt, printInt);
	mq->register_msg(PrintString, printString);
	mq->start();

	for (int i = 0; i < 10; i++)
	{
		int j = i;
		std::thread t([mq](int i) -> void
					  {
						  int *tempInt = new int;
						  *tempInt = i;

						  char *tempChar = new char[20];
						  sprintf(tempChar, "%d", i);

						  mq->put_msg(PrintInt, tempInt);
						  mq->put_msg(PrintString, tempChar);
					  },
					  j);
		t.detach();
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
}