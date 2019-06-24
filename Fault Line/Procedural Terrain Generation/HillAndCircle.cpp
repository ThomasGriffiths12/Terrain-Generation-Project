#include "HillAndCircle.h"



HillAndCircle::HillAndCircle(int size)
{
	iterations = 500;
	this->size = size;
	heightMap.resize(size);
	for (int i = 0; i < heightMap.size(); i++)
	{
		heightMap[i].resize(size);
	}
	displacement = 0.05f;

}


HillAndCircle::~HillAndCircle()
{
}

void HillAndCircle::HillAndCircleAlgorithm()
{
	float circleSize;
	float rCirc = rand();
	float rCircX = rand();
	float rCircY = rand();

	circleSize = (rCirc / RAND_MAX) * 40;
	int circleCentreX = (rCircX / RAND_MAX) * size;
	int circleCentreY = (rCircY / RAND_MAX) * size;


	for (int x = 0; x < heightMap.size() - 1; x++)
	{
		for (int y = 0; y < heightMap[x].size() - 1 ; y++)
		{
			float pointDistance = pow((x - circleCentreX),2) + pow((y - circleCentreY),2);
			pointDistance = pow(circleSize, 2) - pointDistance;
			if (pointDistance > 0)
			heightMap[x][y] += pointDistance * displacement;
		}
	}
}

std::vector<std::vector<float>> HillAndCircle::HillAndCircleProcess()
{
	srand(time(NULL));
	rand();

	for (int i = 0; i < iterations; i++)
	{
		HillAndCircleAlgorithm();
	}
	return heightMap;
}
