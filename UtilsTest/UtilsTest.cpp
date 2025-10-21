#include <iostream>
#include <chrono>
#include <atomic>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;


template <typename MIN, typename MAX>
void RandomNumber(MIN min, MAX max);
void ClearGrid(uint32_t start, uint32_t span, uint32_t leftOver);
void GenericFunction(int num, int start, int length);

TaskSystem mt(10);

uint16_t const testCases = 100;

int const taskCount = 30;
int const taskSize = 3000000;

uint32_t const gridY = 216;
uint32_t const gridX = 384;

std::vector<int> grids[gridY][gridX];

int64_t lNumber = 0;


std::chrono::high_resolution_clock::time_point t1;
std::chrono::high_resolution_clock::time_point t2;
std::chrono::microseconds singleMS;
std::chrono::microseconds multiMS;

int main()
{
	std::cout << "Tasks: " << taskCount << " Task Size: " << taskSize << " Test Cases: " << testCases <<"\n";


	uint32_t count = gridY;
	uint32_t span = count / mt.ActiveThreads();
	uint32_t leftOver = count - (span * mt.ActiveThreads());

	for (uint16_t tests = 0; tests < testCases; tests++)
	{
		t1 = std::chrono::high_resolution_clock::now();
		for (uint8_t i = 0; i < mt.ActiveThreads(); i++)
		{
			//RandomNumber(-50000, 50000);
			ClearGrid(i * span, span, leftOver * (i == mt.ActiveThreads() - 1));
		}


		t2 = std::chrono::high_resolution_clock::now();
		singleMS += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);








		t1 = std::chrono::high_resolution_clock::now();
		for (uint8_t i = 0; i < mt.ActiveThreads(); i++)
		{
			//mt.AddTask(RandomNumber<int,int>, -50000, 50000);
			mt.AddTask(ClearGrid, i * span, span, leftOver * (i == mt.ActiveThreads() - 1));
		}

		mt.WaitForComplete();
		t2 = std::chrono::high_resolution_clock::now();
		multiMS += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	}






	std::cout << "Single Thread Time: " << singleMS.count() / testCases << " ms\n";
	std::cout << "Multi Thread Time: " << multiMS.count() / testCases << " ms " << "\n";
	std::cout << "Thread Count: " << (int)mt.ActiveThreads() << "/" << (int)mt.MaxThreads() << "\n";


	mt.~TaskSystem();

	while (std::cin.get() != '\n');
}

void GenericFunction(int num, int start, int length)
{

}

void ClearGrid(uint32_t start, uint32_t span, uint32_t leftOver)
{
	uint32_t end = start + span + leftOver;
	for (uint32_t y = start; y < end; y++) for (uint32_t x = 0; x < gridX; x++) grids[y][x].clear();
}

template <typename MIN, typename MAX>
 void RandomNumber(MIN min, MAX max) // generates a random number based on input parameters
{
	for (int i = 0; i < taskSize; i++)
	{
		std::uniform_int_distribution<> distr(min, max);
	}
}