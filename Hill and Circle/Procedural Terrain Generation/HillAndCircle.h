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
#include "resource.h"


class HillAndCircle
{
private:
	std::vector<std::vector<float>> heightMap;
	float iterations;
	float displacement;
	int size;
public:
	HillAndCircle(int size);
	~HillAndCircle();
	void HillAndCircleAlgorithm();
	std::vector<std::vector<float>> HillAndCircleProcess();
};

