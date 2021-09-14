#include "../MsgQueue.h"
#include <thread>
#include <mutex>
#include <iostream>

std::mutex mu;

template <typename T>
void Println(T t)
{
	using namespace std;
	cout << t << endl;
}

template <typename T>
void Print(T t)
{
	using namespace std;
	cout << t;
}

enum MsgEnumAction
{
	PrintInt,
	PrintString
};

int main(int argc, char **argv)
{
	MsgQueue mq;
	mq.SetConsumerNum(5);

	mq.RegisterMsg(PrintInt, [](Msg msg) -> void
				   {
					   mu.lock();
					   Print("PrintInt ");
					   Println(*(int *)msg);
					   mu.unlock();

					   delete (int *)msg;
				   });

	mq.RegisterMsg(PrintString, [](Msg msg) -> void
				   {
					   mu.lock();
					   Print("PrintString ");
					   Println((char *)msg);
					   mu.unlock();

					   delete[](char *) msg;
				   });

	for (int i = 0; i < 1000; i++)
	{
		int *pInt = new int;
		*pInt = i;
		mq.PutMsg(PrintInt, pInt);

		char *tempChar = new char[20];
		sprintf(tempChar, "%d", i);
		mq.PutMsg(PrintString, tempChar);
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
}