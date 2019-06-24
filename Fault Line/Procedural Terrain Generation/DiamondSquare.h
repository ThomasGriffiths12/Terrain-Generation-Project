#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <directxcolors.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdlib.h>

class DiamondSquare
{
private:
	std::vector<std::vector<float>> heightMap;
	int size;
	int resolution;
	int reach;
	int min;
	int max;
public:

	void SquareStep(int length, int halfLength);
	void DiamondStep(int length, int halfLength);
	void SetInitialValues();
	std::vector<std::vector<float>> DiamondStepProcess();
	DiamondSquare(int size);
	~DiamondSquare();
	double r = rand();
	float randF() { return r / RAND_MAX; }
	float randM() { return (min + randF() * (max - min)); }
};

