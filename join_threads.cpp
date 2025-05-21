#include "join_threads.h"

JoinThreads::JoinThreads(std::vector<std::thread>& threads)
	: threads{ threads }
{
};

JoinThreads::~JoinThreads()
{
	for (auto& t: threads)
	{
		if (t.joinable()) t.join();
	}
}