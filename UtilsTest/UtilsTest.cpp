#include <iostream>
#include <chrono>
#include <atomic>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;


template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max);
void GenericFunction(int num, int start, int length);

TaskSystem mt(30);

int const taskCount = 30;
int const taskSize = 3000000;

double array[taskCount * taskSize];

int64_t lNumber = 0;


std::chrono::steady_clock::time_point t1;
std::chrono::steady_clock::time_point t2;
std::chrono::milliseconds singleMS;
std::chrono::milliseconds multiMS;

int main()
{
	std::cout << "Tasks: " << taskCount << " Task Size: " << taskSize << "\n";


	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		//RandomNumber(-50000, 50000);
		GenericFunction(i, taskSize * i, taskSize);
	}


	t2 = std::chrono::high_resolution_clock::now();
	singleMS = duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::cout << "Single Thread Time: " << singleMS.count() << " milliseconds\n";

	for (int i = 0; i < taskSize * taskCount; i++)
	{
		lNumber += array[i];
	}

	std::cout << "Long Number: " << lNumber << "\n";
	lNumber = 0;

	
	for (int i = 0; i < taskSize * taskCount; i++)
	{
		array[i] = 0;
	}
	

	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < taskCount; i++)
	{
		//mt.AddTask(RandomNumber<int,int>, -50000, 50000);
		mt.AddTask(GenericFunction, i, taskSize * i, taskSize);
	}

	mt.WaitForComplete();
	t2 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < taskSize * taskCount; i++)
	{
		lNumber += array[i];
	}

	multiMS = duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::cout << "Multi Thread Time: " << multiMS.count() << " milliseconds " << "\n";
	std::cout << "Long Number: " << lNumber << "\n";
	lNumber = 0;
	std::cout << "Thread Count: " << (int)mt.ActiveThreads() << "/" << (int)mt.MaxThreads() << "\n";


	mt.~TaskSystem();

	while (std::cin.get() != '\n');
}

void GenericFunction(int num, int start, int length)
{
	
	for (int i = start; i < start + length; i++)
	{
		array[i] = num;
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