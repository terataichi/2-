#include <iostream>
#include "CSample.h"

int CSample::count = 0;

CSample::CSample()
{
	std::cout << "�������[�ɒǉ�" << std::endl;
	n_num = new int[2]();

	for (int j = 0; j < 2; j++)
	{
		n_num[j] = 0;
	}
}

CSample::~CSample()
{
	std::cout << "�������[����폜" << std::endl;
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
