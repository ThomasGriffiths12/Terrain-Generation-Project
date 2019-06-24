#include "Fault.h"



Fault::Fault(int size)
{

	this->size = size;
	heightMap.resize(size);

	for (int i = 0; i < heightMap.size(); i++)
	{
		heightMap[i].resize(size);
	}

	displacement = 0.6;
	iterations = 400;
}

std::vector<std::vector<float>> Fault::FaultProcess()
{
	srand(time(NULL));
	rand();

	for (int i = 0; i < iterations; i++)
	{
		FaultAlgorithm();
	}
	return heightMap;
}

void Fault::FaultAlgorithm()
{

	float v = rand();
	float a = sin(v);
	float b = cos(v);
	float d = sqrt(size * size + size * size);
	float r = rand();
	float c = (r / RAND_MAX) * d - d / 2;
	float dist = 0;


	for (int x = 0; x < heightMap.size() - 1; x++)
	{
		for (int y = 0; y < heightMap[x].size() - 1; y++)
		{
			dist = a * x + b * y - c;
			if (dist > 0)
			{
				heightMap[x][y] += displacement;
			}
			else
			{
				heightMap[x][y] -= displacement;
			}
		}
	}
}


Fault::~Fault()
{
}
