#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <directxcolors.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include "DDSTextureLoader.h"
#include "DiamondSquare.h"
#include "HillAndCircle.h"
#include "Fault.h"
#include "resource.h"

using namespace DirectX;

#define ReleaseCOM( x ){ if( x != NULL) x->Release(); x = NULL;}


class Terrain
{
	struct SimpleVertex
	{
		XMFLOAT3 PosL;
		XMFLOAT3 NormL;
		XMFLOAT2 Tex;
	};

	struct InitInfo
	{
		std::wstring heightMapFileName;

		float heightScale;

		UINT numVertices;
		UINT numCells;

		UINT heightMapWidth;
		UINT heightMapHeight;

		UINT indicesCount;

		UINT rows;
		UINT columns;

		float cellSpacing;

	};

public:
	Terrain();
	~Terrain();
	void LoadHeightMap();
	bool InBounds(int i, int j);
	float Average(int i, int j);
	void Smooth();
	void BuildHeightMapSRV(ID3D11Device* device);
	float GetDepth() const;
	float GetWidth() const;
	void BuildVertexBuffer(ID3D11Device * device);
	void BuildIndexBuffer(ID3D11Device* device);
	void BuildStaticVertexBuffer(ID3D11Device * device);
	void BuildStaticIndexBuffer(ID3D11Device * device);

	UINT ReturnIndicesCount();
	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
private:
	DiamondSquare * diamond;
	HillAndCircle * hillCircle;
	Fault * fault;
	std::vector<float> mStaticHeightMap;
	std::vector<std::vector<float>> mHeightMap;
	ID3D11ShaderResourceView * mHeightMapSRV;
	InitInfo info;

};

