#include <iostream>
#include <chrono>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void GenericFunction(int nr, int const start, int const range);

template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max);

TaskSystem mt(30);

int const taskCount = 1000;
int const taskSize = 1000;

int const arraySize = taskCount * taskSize;
int array[arraySize];

std::chrono::steady_clock::time_point t1;
std::chrono::steady_clock::time_point t2;
std::chrono::milliseconds singleMS;
std::chrono::milliseconds multiMS;

int main()
{
	std::cout << "Tasks: " << taskCount << " Task Size: " << taskSize << "\n";


	t1 = high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		RandomNumber(-50000, 50000);
	}


	t2 = high_resolution_clock::now();
	singleMS = duration_cast<milliseconds>(t2 - t1);
	std::cout << "Single Thread Time: " << singleMS.count() << " milliseconds\n";

	t1 = high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		mt.AddTask(RandomNumber<int,int>, -50000, 50000);
	}

	mt.WaitForComplete();


	t2 = high_resolution_clock::now();
	multiMS = duration_cast<milliseconds>(t2 - t1);
	std::cout << "Multi Thread Time: " << multiMS.count() << " milliseconds " << "\n";
	std::cout << "Thread Count: " << (int)mt.ActiveThreads() << "/" << (int)mt.MaxThreads() << "\n";


	while (std::cin.get() != '\n');
}

void GenericFunction(int nr, int const start, int const range)
{
	int max = start + range;
	for (int i = start; i < max; i++)
	{
		array[i] = nr;
	}
}

template <typename MIN, typename MAX>
 void RandomNumber(MIN min, MAX max) // generates a random number based on input parameters
{
	for (int i = 0; i < taskSize; i++)
	{
		std::uniform_int_distribution<> distr(min, max);
	}
}