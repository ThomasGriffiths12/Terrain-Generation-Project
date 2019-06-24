#include "DiamondSquare.h"


DiamondSquare::DiamondSquare(int size)
{
	this->size = size;
	this->resolution = size * size;
	this->heightMap.resize(size);

	for (int i = 0; i < heightMap.size(); i++)
	{
		heightMap[i].resize(size);
	}

	this->reach = 100;
	this->min = -this->reach;
	this->max = this->reach;
}

DiamondSquare::~DiamondSquare()
{
}

void DiamondSquare::SquareStep(int length, int halfLength)
{
	for (int x = 0; x < heightMap.size() - 1; x += length)
	{
		for (int y = 0; y < heightMap[x].size() - 1; y += length)
		{
			float a = heightMap[x][y];
			float b = heightMap[x + length][y];
			float c = heightMap[x][y + length];
			float d = heightMap[(x + length)][y + length];
			float avg = (a + b + c + d) / 4;

			heightMap[x + halfLength][y + halfLength] = avg + randM();
		}
	}
}

void DiamondSquare::DiamondStep(int length, int halfLength)
{
	for (int x = 0; x < heightMap.size() - 1; x += halfLength)
	{
		for (int y = (x + halfLength) % length; y < heightMap[x].size() - 1; y += length)
		{
			float a = heightMap[(x - halfLength + (size - 1)) % (size - 1)][y];
			float b = heightMap[x + halfLength][y];
			float c = heightMap[x][(y + halfLength)];
			float d = heightMap[x][(y - halfLength + (size - 1)) % (size - 1)];

			float avg = (a + b + c + d) / 4;
			heightMap[x + halfLength][y + halfLength] = avg + randM();
		}
	}
}

std::vector<std::vector<float>> DiamondSquare::DiamondStepProcess()
{
	srand(time(NULL));
	rand();

	SetInitialValues();

	for (int length = size - 1; length >= 2; length /= 2, reach /= 2)
	{
		min = -reach;
		max = reach;
		int halfLength = length / 2;
		SquareStep(length, halfLength);
		DiamondStep(length, halfLength);
	}

	return this->heightMap;
}

void DiamondSquare::SetInitialValues()
{
	//Defines the corner values of the height map 
	heightMap[0][0] = heightMap[size - 1][size - 1] = heightMap[0][size - 1] = heightMap[size - 1][0] = 100;

}