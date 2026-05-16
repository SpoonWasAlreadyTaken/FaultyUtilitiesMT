#include <iostream>
#include <chrono>
#include <atomic>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;


template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max);
void GenericFunction(size_t start, size_t end, int num);

TaskSystem mt(10);

constexpr float testCases = 100;

constexpr int taskCount = 30;
constexpr int taskSize = 300000;

int testArray[taskSize];


std::chrono::high_resolution_clock::time_point t1;
std::chrono::high_resolution_clock::time_point t2;
std::chrono::microseconds singleMS;
std::chrono::microseconds multiMS;

int main()
{
	std::cout << "Tasks: " << taskCount << " Task Size: " << taskSize << " Test Cases: " << testCases <<"\n";

    size_t count = taskSize;
    size_t span = count / taskCount;

    int singleTest = 0;
    int multiTest = 0;

	for (uint16_t tests = 0; tests < testCases; tests++)
	{
		t1 = std::chrono::high_resolution_clock::now();
		for (uint8_t i = 0; i < taskCount; i++)
		{
			//RandomNumber(-50000, 50000);
			GenericFunction(i * span, span, i + 1);
		}


		t2 = std::chrono::high_resolution_clock::now();
		singleMS += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);



        for (size_t i = 0; i < taskSize; ++i) singleTest += testArray[i];
        for (size_t i = 0; i < taskSize; ++i) testArray[i] = 0;



		t1 = std::chrono::high_resolution_clock::now();
		for (uint8_t i = 0; i < taskCount; i++)
		{
			//mt.AddTask(RandomNumber<int,int>, -50000, 50000);
			mt.AddTask(GenericFunction, i * span, span, i + 1);
		}

		mt.WaitForComplete();
		t2 = std::chrono::high_resolution_clock::now();
		multiMS += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

        for (size_t i = 0; i < taskSize; ++i) multiTest += testArray[i];
        for (size_t i = 0; i < taskSize; ++i) testArray[i] = 0;
	}






	std::cout << "Single Thread Time: " << singleMS.count() / testCases << " ms\n";
    std::cout << "Single Thread Result: " << singleTest << "\n";
	std::cout << "Multi Thread Time: " << multiMS.count() / testCases << " ms " << "\n";
    std::cout << "Multi Thread Result: " << multiTest << "\n";
	std::cout << "Thread Count: " << (int)mt.ActiveThreads() << "/" << (int)mt.MaxThreads() << "\n";
}

void GenericFunction(size_t start, size_t span, int num)
{
    size_t end = start + span;
    for (size_t i = start; i < end; ++i) testArray[i] = num;
}


template <typename MIN, typename MAX>
 void RandomNumber(MIN min, MAX max) // generates a random number based on input parameters
{
	for (int i = 0; i < taskSize; i++)
	{
		std::uniform_int_distribution<> distr(min, max);
	}
}
