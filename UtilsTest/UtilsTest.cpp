#include <iostream>
#include <chrono>

#include "FaultyUtilitiesMT.hpp"

#include <random>

std::random_device rd;


void GenericFunction(int nr);

TaskSystem mt(8);

int main()
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;


	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < 2000; i++)
	{
		mt.AddTask(GenericFunction, i);
	}
	mt.~TaskSystem();

	auto t2 = high_resolution_clock::now();
	auto ms_int = duration_cast<milliseconds>(t2 - t1);
	std::cout << "Time to Complete: " << ms_int.count() << " milliseconds\n";
}

void GenericFunction(int nr)
{
	std::cout << "hell yeah +" << nr << "\n";
}
