#include <iostream>
#include "CSample.h"

int CSample::count = 0;

CSample::CSample()
{
	std::cout << "ƒƒ‚ƒŠ[‚É’Ç‰Á" << std::endl;
	n_num = new int[2]();

	for (int j = 0; j < 2; j++)
	{
		n_num[j] = 0;
	}
}

CSample::~CSample()
{
	std::cout << "ƒƒ‚ƒŠ[‚©‚çíœ" << std::endl;
	delete[] n_num;
}

void CSample::add(int w_pos, int num)
{
	n_num[w_pos] += num;
	count++;
}

void CSample::set(int w_pos, int num)
{
	n_num[w_pos] = num;
	count++;
}

int CSample::get(int w_pos)
{
	count++;
	return n_num[w_pos];
}

int CSample::Count()
{
	return count;
}
