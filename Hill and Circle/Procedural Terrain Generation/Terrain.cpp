#include "Terrain.h"


Terrain::Terrain()
{

	info.heightMapHeight = 513;
	info.heightMapWidth = 513;
	info.heightScale = 100;
	info.cellSpacing = 1;
	info.heightMapFileName = L"Resources\\coastMountain513.raw";

	info.indicesCount = (info.heightMapHeight - 1) * (info.heightMapWidth - 1) * 6;
	info.rows = info.heightMapHeight;
	info.columns = info.heightMapWidth;

	info.numVertices = (info.rows * info.columns);
	info.numCells = ((info.rows - 1) * (info.columns - 1));

	fault = new Fault(513);
	//mHeightMap = fault->FaultProcess();
	diamond = new DiamondSquare(513);
	//mHeightMap = diamond->DiamondStepProcess();
	hillCircle = new HillAndCircle(513);
	mHeightMap = hillCircle->HillAndCircleProcess();
	Smooth();
}
void Terrain::LoadHeightMap()
{
	//A height for each vertex
	std::vector<unsigned char> in(info.heightMapWidth * info.heightMapHeight);

	//Open the file.
	std::ifstream inFile;
	inFile.open(info.heightMapFileName.c_str(), std::ios_base::binary);

	if (inFile)
	{
		//Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());
		//Done with file
		inFile.close();
	}

	//Copy the array data into a float array and scale it.
	mStaticHeightMap.resize(info.heightMapHeight * info.heightMapWidth, 0);

	for (UINT i = 0; i < info.heightMapHeight * info.heightMapWidth; ++i)
	{
		mStaticHeightMap[i] = (in[i] / 255.0f) * info.heightScale;
	}
}

bool Terrain::InBounds(int i, int j)
{
	return i >= 0 && i < (int)info.heightMapHeight && j >= 0 && j < (int)info.heightMapWidth;
}

float Terrain::Average(int i, int j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += mHeightMap[m][n];
				num += 1.0f;
			}
		}
	}
	return avg / num;
}

void Terrain::Smooth()
{
	std::vector<std::vector<float>> dest(mHeightMap.size());
	for (int i = 0; i < dest.size(); i++)
	{
		dest[i].resize(dest.size());
	}

	for (UINT i = 0; i < info.heightMapHeight - 1; ++i)
	{
		for (UINT j = 0; j < info.heightMapWidth - 1; ++j)
		{
			dest[i][j] = Average(i, j);
		}
	}

	mHeightMap = dest;
}

void Terrain::BuildHeightMapSRV(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = info.heightMapWidth;
	texDesc.Height = info.heightMapHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	std::vector<DirectX::PackedVector::HALF> hMap(mStaticHeightMap.size());
	std::transform(mStaticHeightMap.begin(), mStaticHeightMap.end(), hMap.begin() , DirectX::PackedVector::XMConvertFloatToHalf);

	HRESULT hr;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hMap[0];
	data.SysMemPitch = info.heightMapWidth * sizeof(DirectX::PackedVector::HALF);
	data.SysMemSlicePitch = 0;
	ID3D11Texture2D* hMapTex = 0;
	hr = device->CreateTexture2D(&texDesc, &data, &hMapTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	hr = device->CreateShaderResourceView(hMapTex, &srvDesc, &mHeightMapSRV);
	ReleaseCOM(hMapTex);
}

float Terrain::GetDepth() const
{
	return (info.heightMapHeight - 1) * info.cellSpacing;
}

float Terrain::GetWidth() const
{
	return (info.heightMapWidth - 1) * info.cellSpacing;
}
void Terrain::BuildVertexBuffer(ID3D11Device * device)
{
	std::vector<SimpleVertex> vertices(info.numVertices);
	float halfWidth = 0.5f * GetWidth();
	float halfDepth = 0.5f * GetDepth();

	float cellWidth = GetWidth() / (info.rows - 1);
	float cellDepth = GetDepth() / (info.columns - 1);

	float du = 1.0f / (info.rows - 1);
	float dv = 1.0f / (info.columns - 1);

	int k = 0;
	for (UINT i = 0; i < mHeightMap.size(); i++)
	{
		float z = halfDepth - i * cellDepth;
		for (UINT j = 0; j < mHeightMap[i].size(); j++)
		{
			float x = -halfWidth + j * cellWidth;

			vertices[k].PosL = XMFLOAT3(x, mHeightMap[i][j], z);
			vertices[k].NormL = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[k].Tex.x = j * du;
			vertices[k].Tex.y = i * dv;
			k++;
			//vertices[i * info.columns + j].Pos = XMFLOAT3(x, 0.0f, z);
			//vertices[i * info.columns + j].Tex.x = j * du;
			//vertices[i * info.columns + j].Tex.y = i * dv;
		}
	}
	
	//Code could be useful for bounding box collision implementation later on
	//for (UINT i = 0; i < info.rows - 1; ++i)
	//{
	//	for (UINT j = 0; i < info.columns - 1; ++j)
	//	{
	//		UINT quadID = i * (info.columns - 1) + j;
	//		vertices[i * info.columns + j].BoundsY = 
	//	}
	//}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HRESULT(device->CreateBuffer(&vbd, &vinitData, &_pVertexBuffer));
}

void Terrain::BuildIndexBuffer(ID3D11Device * device)
{
	// Create index buffer
	std::vector<UINT> indices;
	for (int i = 0; i < info.rows - 1; i++)
	{
		for (int j = 0; j < info.columns - 1; j++)
		{
			indices.push_back(i * info.columns + j);
			indices.push_back(i * info.columns + j + 1);
			indices.push_back((i + 1) * info.columns + j);

			indices.push_back((i + 1) * info.columns + j);
			indices.push_back(i * info.columns + j + 1);
			indices.push_back((i + 1) * info.columns + j + 1);
		}
	};
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();     
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices.data();
	HRESULT(device->CreateBuffer(&bd, &InitData, &_pIndexBuffer));
}

void Terrain::BuildStaticVertexBuffer(ID3D11Device * device)
{
	std::vector<SimpleVertex> vertices(info.numVertices);
	float halfWidth = 0.5f * GetWidth();
	float halfDepth = 0.5f * GetDepth();

	float cellWidth = GetWidth() / (info.rows - 1);
	float cellDepth = GetDepth() / (info.columns - 1);

	float du = 1.0f / (info.rows - 1);
	float dv = 1.0f / (info.columns - 1);

	int k = 0;

	for (UINT i = 0; i < info.rows; i++)
	{
		float z = halfDepth - i * cellDepth;
		for (UINT j = 0; j < info.columns; j++)
		{
			float x = -halfWidth + j * cellWidth;

			vertices[k].PosL = XMFLOAT3(x, mStaticHeightMap[k], z);
			vertices[k].NormL = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[k].Tex.x = j * du;
			vertices[k].Tex.y = i * dv;
			k++;
			//vertices[i * info.columns + j].Pos = XMFLOAT3(x, 0.0f, z);
			//vertices[i * info.columns + j].Tex.x = j * du;
			//vertices[i * info.columns + j].Tex.y = i * dv;
		}
	}

	//Code could be useful for bounding box collision implementation later on
	//for (UINT i = 0; i < info.rows - 1; ++i)
	//{
	//	for (UINT j = 0; i < info.columns - 1; ++j)
	//	{
	//		UINT quadID = i * (info.columns - 1) + j;
	//		vertices[i * info.columns + j].BoundsY = 
	//	}
	//}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HRESULT(device->CreateBuffer(&vbd, &vinitData, &_pVertexBuffer));
}

void Terrain::BuildStaticIndexBuffer(ID3D11Device * device)
{
	// Create index buffer
	std::vector<UINT> indices;
	for (int i = 0; i < info.rows - 1; i++)
	{
		for (int j = 0; j < info.columns - 1; j++)
		{
			indices.push_back(i * info.columns + j);
			indices.push_back(i * info.columns + j + 1);
			indices.push_back((i + 1) * info.columns + j);

			indices.push_back((i + 1) * info.columns + j);
			indices.push_back(i * info.columns + j + 1);
			indices.push_back((i + 1) * info.columns + j + 1);
		}
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices.data();
	HRESULT(device->CreateBuffer(&bd, &InitData, &_pIndexBuffer));
}

UINT Terrain::ReturnIndicesCount()
{
	return info.indicesCount;
}


Terrain::~Terrain()
{
	delete diamond;
	delete fault;
}
