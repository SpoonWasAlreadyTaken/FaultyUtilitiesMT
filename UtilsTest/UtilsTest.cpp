#include <iostream>
#include <chrono>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void GenericFunction(int nr);

template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max);

TaskSystem mt(30);

int taskCount = 100000;
int taskSize = 10000;

int main()
{
	std::cout << "Tasks: " << taskCount << " Task Size: " << taskSize << "\n";


	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		RandomNumber(-500, 500);
	}


	auto t2 = high_resolution_clock::now();
	auto ms_int = duration_cast<milliseconds>(t2 - t1);
	std::cout << "Single Thread Time: " << ms_int.count() << " milliseconds\n";

	t1 = high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		mt.AddTask(RandomNumber<int, int> , -500, 500);
	}

	mt.WaitForComplete();

	t2 = high_resolution_clock::now();
	ms_int = duration_cast<milliseconds>(t2 - t1);
	std::cout << "Multi Thread Time: " << ms_int.count() << " milliseconds\n" << "Thread Count: " << (int)mt.ActiveThreads() << "/" << (int)mt.MaxThreads() << "\n";
}

void GenericFunction(int nr)
{
	nr = nr * nr;
}

template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max) // generates a random number based on input parameters
{
	for (int i = 0; i < taskSize; i++)
	{
		std::uniform_int_distribution<> distr(min, max);
	}
}