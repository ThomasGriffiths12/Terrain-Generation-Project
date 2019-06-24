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

class Fault
{
private:
	std::vector<std::vector<float>> heightMap;
	float size;
	float iterations;
	float displacement;
public:
	Fault(int size);
	std::vector<std::vector<float>> FaultProcess();
	void FaultAlgorithm();
	~Fault();
};

