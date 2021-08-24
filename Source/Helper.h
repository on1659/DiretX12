#pragma once

#include "stdafx.h"

#include <fstream>
#include <ostream>
#include <io.h>
#include <conio.h>
#include <queue>
#include <deque>

#include "FontManager.h"

#define MAX_BUFFER 253
#define MAX_ALL_BUFFER 1500
#define MAR_RANGE 40


//RECT
RECT operator*(RECT a, float factor)
{
	a.left = a.left * factor;
	a.right = a.right * factor;
	return a;
}

RECT operator-(RECT a, RECT b)
{
	RECT c;
	c.left = a.left - b.left;
	c.right = a.right - b.right;
	c.top = a.top;
	c.bottom = a.bottom;
	return c;
}

RECT operator-(RECT a, POINT b)
{
	return RECT{ (a.left - b.x),(a.right - b.x),(a.top - b.y),(a.bottom - b.y) };
}

RECT operator+(RECT a, POINT b)
{
	return RECT{ (a.left + b.x),(a.right + b.x),(a.top + b.y),(a.bottom + b.y) };
}


//POINT
POINT operator-(POINT a, POINT b)
{
	return POINT{ (a.x - b.x), (a.y - b.y) };
}

POINT operator*(POINT a, float factor)
{
	return POINT{ (LONG)(a.x * factor),  (LONG)(a.y * factor) };
}

/*
Helper
*/
class Helper
{


public:
	static float RANDOMCOLOR(float a, float b)
	{
		return(a + (float)(rand() / (float)RAND_MAX) * (b - a));
	}

	static int GMessageBox(std::string _title, std::string _message)
	{
		const char* msgChar = _message.c_str();
		int msgLen = 1 + strlen(msgChar);
		wchar_t* message = new wchar_t[msgLen];
		mbstowcs(message, msgChar, msgLen);

		const char* titleChar = _title.c_str();
		int titleLen = 1 + strlen(titleChar);
		wchar_t* title = new wchar_t[titleLen];
		mbstowcs(title, titleChar, titleLen);

		return MessageBox(nullptr, message, title, MB_OK);
	}

	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	static float RandF(float a, float b)
	{
		return a + RandF()*(b - a);
	}

	static float R(float a, float b)
	{
		return(a + (float)(rand() / (float)RAND_MAX) * (b - a));
	}


	//랜덤 텍스쳐 만들기

	static ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, std::vector<_TCHAR*> ppstrFilePaths)
	{
		int nTextures = ppstrFilePaths.size();

		D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
		d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.FirstMipLevel = 0;
		d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
		d3dxImageLoadInfo.BindFlags = 0;
		d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		d3dxImageLoadInfo.MiscFlags = 0;
		d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
		d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
		d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		d3dxImageLoadInfo.pSrcInfo = 0;

		ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
		for (UINT i = 0; i < nTextures; i++)
		{
			HRESULT hResult = D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);
			if (FAILED(hResult))
			{
				if (ppstrFilePaths[i])
				{
					MessageBox(nullptr, ppstrFilePaths[i], L"여기는 2DArry", MB_OK);
				}
			}
		}

		//
		// Create the texture array.  Each element in the texture 
		// array has the same format/dimensions.
		//
		D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
		ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

		D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
		d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
		d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
		d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
		d3dTexture2DArrayDesc.ArraySize = nTextures;
		d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
		d3dTexture2DArrayDesc.SampleDesc.Count = 1;
		d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
		d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DArrayDesc.CPUAccessFlags = 0;
		d3dTexture2DArrayDesc.MiscFlags = 0;

		ID3D11Texture2D *pd3dTexture2DArray;
		pd3dDevice->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

		ID3D11DeviceContext *pd3dDeviceContext;
		pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

		//
		// Copy individual texture elements into texture array.
		//

		// for each texture element...
		D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
		for (UINT t = 0; t < nTextures; t++)
		{
			for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
			{
				pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
				pd3dDeviceContext->UpdateSubresource(pd3dTexture2DArray,
					D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels),
					0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch,
					d3dMappedTexture2D.DepthPitch);
				pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
			}
		}
		//
		// Create a resource view to the texture array.
		//

		D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
		ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
		d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
		d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
		d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
		d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

		ID3D11ShaderResourceView *pd3dsrvTextureArray;
		pd3dDevice->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

		if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

		for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
		delete[] ppd3dTextures;

		if (pd3dDeviceContext) pd3dDeviceContext->Release();

		return(pd3dsrvTextureArray);
	}

	static ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, std::wstring ppstrFilePaths)
	{
		int nTextures = 1;

		D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
		d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.FirstMipLevel = 0;
		d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
		d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
		d3dxImageLoadInfo.BindFlags = 0;
		d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		d3dxImageLoadInfo.MiscFlags = 0;
		d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
		d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
		d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		d3dxImageLoadInfo.pSrcInfo = 0;

		ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
		for (UINT i = 0; i < nTextures; i++)
		{
			HRESULT hResult = D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths.c_str(), &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

			if (FAILED(hResult))
			{
				if (ppstrFilePaths.size() > 0)
				{
					MessageBox(nullptr, ppstrFilePaths.c_str(), L"아앙", MB_OK);
				}
				else
					MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", L"아앙", MB_OK);
				return nullptr;
			}
		}

		//
		// Create the texture array.  Each element in the texture 
		// array has the same format/dimensions.
		//
		D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
		ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

		D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
		d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
		d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
		d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
		d3dTexture2DArrayDesc.ArraySize = nTextures;
		d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
		d3dTexture2DArrayDesc.SampleDesc.Count = 1;
		d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
		d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DArrayDesc.CPUAccessFlags = 0;
		d3dTexture2DArrayDesc.MiscFlags = 0;

		ID3D11Texture2D *pd3dTexture2DArray;
		pd3dDevice->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

		ID3D11DeviceContext *pd3dDeviceContext;
		pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

		//
		// Copy individual texture elements into texture array.
		//

		// for each texture element...
		D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
		for (UINT t = 0; t < nTextures; t++)
		{
			for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
			{
				pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
				pd3dDeviceContext->UpdateSubresource(pd3dTexture2DArray,
					D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels),
					0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch,
					d3dMappedTexture2D.DepthPitch);
				pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
			}
		}
		//
		// Create a resource view to the texture array.
		//

		D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
		ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
		d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
		d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
		d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
		d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

		ID3D11ShaderResourceView *pd3dsrvTextureArray;
		pd3dDevice->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

		if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

		for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
		delete[] ppd3dTextures;

		if (pd3dDeviceContext) pd3dDeviceContext->Release();

		return(pd3dsrvTextureArray);
	}

	static ID3D11ShaderResourceView* CreateRandomTexture1DSRV(ID3D11Device *pd3dDevice)
	{
		XMFLOAT4 RV[1024];
		for (int i = 0; i < 1024; i++)
			RV[i] = XMFLOAT4(Helper::R(-1.0f, 1.0f), Helper::R(-1.0f, 1.0f), Helper::R(-1.0f, 1.0f), Helper::R(-1.0f, 1.0f));
		D3D11_SUBRESOURCE_DATA d3dSubResourceData;
		d3dSubResourceData.pSysMem = RV;
		d3dSubResourceData.SysMemPitch = sizeof(XMFLOAT4) * 1024;
		d3dSubResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE1D_DESC d3dTextureDesc;
		ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
		d3dTextureDesc.Width = 1024;
		d3dTextureDesc.MipLevels = 1;
		d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d3dTextureDesc.ArraySize = 1;
		ID3D11Texture1D *pd3dTexture;
		pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);
		ID3D11ShaderResourceView *pd3dsrvTexture;
		pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
		pd3dTexture->Release();
		return(pd3dsrvTexture);
	}

	static ID3D11ShaderResourceView* CrseateRandomTexture1DSRV_WaterFall(ID3D11Device *pd3dDevice)
	{
		XMFLOAT4 RV[1024];
		for (int i = 0; i < 1024; i++)
			RV[i] = XMFLOAT4(Helper::R(-1.0f, 1.0f), Helper::R(-1.0f, 0.0f), Helper::R(0.5f, 0.5f), Helper::R(-1.0f, 1.0f));
		D3D11_SUBRESOURCE_DATA d3dSubResourceData;
		d3dSubResourceData.pSysMem = RV;
		d3dSubResourceData.SysMemPitch = sizeof(XMFLOAT4) * 1024;
		d3dSubResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE1D_DESC d3dTextureDesc;
		ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
		d3dTextureDesc.Width = 1024;
		d3dTextureDesc.MipLevels = 1;
		d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d3dTextureDesc.ArraySize = 1;
		ID3D11Texture1D *pd3dTexture;
		pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);
		ID3D11ShaderResourceView *pd3dsrvTexture;
		pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
		pd3dTexture->Release();
		return(pd3dsrvTexture);
	}

	static ID3D11ShaderResourceView* CrseateRandomTexture1DSRV_WaterFall_Bottom(ID3D11Device *pd3dDevice)
	{
		XMFLOAT4 RV[1024];
		for (int i = 0; i < 1024; i++)
			RV[i] = XMFLOAT4(Helper::R(-1.0f, 1.0f), Helper::R(0, 1.0f), Helper::R(0.0f, 0.0f), Helper::R(-1.0f, 1.0f));
		D3D11_SUBRESOURCE_DATA d3dSubResourceData;
		d3dSubResourceData.pSysMem = RV;
		d3dSubResourceData.SysMemPitch = sizeof(XMFLOAT4) * 1024;
		d3dSubResourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE1D_DESC d3dTextureDesc;
		ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
		d3dTextureDesc.Width = 1024;
		d3dTextureDesc.MipLevels = 1;
		d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d3dTextureDesc.ArraySize = 1;
		ID3D11Texture1D *pd3dTexture;
		pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);
		ID3D11ShaderResourceView *pd3dsrvTexture;
		pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
		pd3dTexture->Release();
		return(pd3dsrvTexture);
	}


	//Csv Loader
	static int MergeNumber(int *number, int size)
	{
		int sum = 0;
		for (int i = 1; i <= size; ++i)
		{
			sum += number[size - i] * pow(10.0f, i - 1);
		}
		return sum;
	}

	static std::vector<XMFLOAT3> MapCsvLoader(std::string path, std::string filename)
	{
		path.append(filename.c_str());
		path.append(".csv");

		std::ifstream fin;
		fin.open(path);
		std::vector<XMFLOAT3> vPosition;
		char msg[MAX_BUFFER];

		if (!fin)
			fMessageBox("CSV_ERROR", path);

		while (fin.getline(msg, MAX_BUFFER))
		{
			int nPivot = 2;
			XMFLOAT3 pos;
			for (int i = 0; i < 3; ++i)
			{
				int number[4];
				int nNumber = 0;
				while (nPivot < strlen(msg) && msg[nPivot] != ',')
				{
					number[nNumber++] = msg[nPivot] - 48;
					nPivot++;
				}

				switch (i)
				{
				case 0:
					pos.x = MergeNumber(number, nNumber);
					break;
				case 1:
					pos.y = MergeNumber(number, nNumber);
					break;
				case 2:
					pos.z = MergeNumber(number, nNumber);
					break;
				}

				nPivot++;
			}
			vPosition.push_back(pos);
		}
		fin.close();
		return vPosition;
	}

	static std::vector<XMFLOAT4> MapCsvLoader(XMFLOAT4, std::string path, std::string filename)
	{
		path.append(filename.c_str());
		path.append(".csv");

		std::ifstream fin;
		fin.open(path);
		std::vector<XMFLOAT4> vPosition;
		char msg[MAX_BUFFER];

		while (fin.getline(msg, MAX_BUFFER))
		{
			int nPivot = 2;
			XMFLOAT4 pos;
			for (int i = 0; i < 4; ++i)
			{
				int *number = new int[4];
				int nNumber = 0;
				while (nPivot < strlen(msg) && msg[nPivot] != ',')
				{
					number[nNumber++] = msg[nPivot] - 48;
					nPivot++;
				}

				switch (i)
				{
				case 0:
					pos.x = MergeNumber(number, nNumber);
					break;
				case 1:
					pos.y = MergeNumber(number, nNumber);
					break;
				case 2:
					pos.z = MergeNumber(number, nNumber);
					break;
				case 3:
					pos.w = MergeNumber(number, nNumber);
					break;
				}

				nPivot++;
				number = nullptr;
				delete number;
			}
			vPosition.push_back(pos);
		}
		fin.close();
		return vPosition;
	}

	static void fMessageBox(std::string _title, std::string _message)
	{
		const char* msgChar = _message.c_str();
		int msgLen = 1 + strlen(msgChar);
		wchar_t* message = new wchar_t[msgLen];
		mbstowcs(message, msgChar, msgLen);

		const char* titleChar = _title.c_str();
		int titleLen = 1 + strlen(titleChar);
		wchar_t* title = new wchar_t[titleLen];
		mbstowcs(title, titleChar, titleLen);

		MessageBox(nullptr, message, title, MB_OK);
	}

	static char* IsCsv(char *path, char ch[])
	{
		int n = strlen(ch);

		if (ch[n - 3] == 'c' & ch[n - 2] == 's' & ch[n - 1] == 'v')
		{
			for (int i = 0; i < (n - 4); ++i)
				path[i] = ch[i];
			path[n - 4] = '\0';
			return path;
		}
		return false;
	}

	static std::vector<std::string> GetCsvPaths(char *path)
	{
		std::vector<std::string> vPath;
		vPath.clear();
		_finddata_t fd;
		long handle;
		int result = 1;
		handle = _findfirst(path, &fd);  //현재 폴더 내 모든 파일을 찾는다.

		if (handle == -1)
		{
			fMessageBox("Thre we No Files.", "NO");
			return vPath;
		}

		while (result != -1)
		{
			char* path = new char();
			if (IsCsv(path, fd.name))
				vPath.push_back(path);
			result = _findnext(handle, &fd);
		}
		_findclose(handle);
		return vPath;
	}

	static int** MapData(std::string path = "../Assets/01_MapData/CSV/", std::string filename = "CubeTerrainHeight")
	{
		path.append(filename.c_str());
		path.append(".csv");

		char msg[MAX_ALL_BUFFER];
		std::ifstream fin;
		fin.open(path);

		if (!fin) fMessageBox("CSV_ERROR", path);

		int z = 0;
		int n = 0;
		int maxZ = 0;

		fin.getline(msg, MAX_ALL_BUFFER);
		{
			int number[4];
			int nNumber = 0;
			while (msg[nNumber] != ',')
			{
				number[nNumber] = msg[nNumber] - 48;
				nNumber++;
			}
			n = MergeNumber(number, nNumber);

			memset(&number, NULL, sizeof(number));
			int yCount = 0;
			while (msg[++nNumber] != ',')
			{
				number[yCount++] = msg[nNumber] - 48;
			}
			maxZ = MergeNumber(number, yCount);


		}

		int **RV = nullptr;
		RV = new int*[maxZ];

		for (int i = 0; i < maxZ; ++i)
			RV[i] = new int[n];


		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			int x = 0;
			for (int nLoopCount = 0; nLoopCount < n; ++nLoopCount)
			{
				int number[20] = { NULL };
				int nNumber = 0;
				while (msg[++x] != ',')
				{
					number[nNumber++] = msg[x] - 48;
				}
				RV[z][nLoopCount] = MergeNumber(number, nNumber);
			}
			z++;
		}
		return RV;
	}

	static void MapData(int* map[], std::string path = "../Assets/01_MapData/CSV/", std::string filename = "CubeTerrainHeight")
	{
		path.append(filename.c_str());
		path.append(".csv");

		char msg[MAX_ALL_BUFFER];
		std::ifstream fin;
		fin.open(path);

		if (!fin) fMessageBox("CSV_ERROR", path);

		int z = 0;
		int n = 0;
		int maxZ = 0;

		fin.getline(msg, MAX_ALL_BUFFER);
		{
			int number[4];
			int nNumber = 0;
			while (msg[nNumber] != ',')
			{
				number[nNumber] = msg[nNumber] - 48;
				nNumber++;
			}
			n = MergeNumber(number, nNumber);

			memset(&number, NULL, sizeof(number));
			int yCount = 0;
			while (msg[++nNumber] != ',')
			{
				number[yCount++] = msg[nNumber] - 48;
			}
			maxZ = MergeNumber(number, yCount);


		}

		//int **RV = nullptr;
		//RV = new int*[maxZ];
		//
		//for (int i = 0; i < maxZ; ++i)
		//	RV[i] = new int[n];


		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			int x = 0;
			for (int nLoopCount = 0; nLoopCount < n; ++nLoopCount)
			{
				int number[20] = { NULL };
				int nNumber = 0;
				while (msg[++x] != ',')
				{
					number[nNumber++] = msg[x] - 48;
				}
				map[z][nLoopCount] = MergeNumber(number, nNumber);
			}
			z++;
		}
	}

	/*
	ix, iy =       Input x, y;
	mx, my =   miro X, miro y;
	ax, ay = adjust x, y;
	ex, ey = endposition x, y;
	w , h = Width, Height;
	*/
	static std::deque<XMFLOAT2> BFS(int **gMap, int ix, int iy, int w, int h)
	{

		std::queue<XMFLOAT2> que;
		int visited[MAR_RANGE * 2][MAR_RANGE * 2] = { 0, 0 };

		int ax, ay;
		ax = (MAR_RANGE - 1) - ix; ay = (MAR_RANGE - 1) - iy;

		if (ix < MAR_RANGE) ax = 0;
		if (iy < MAR_RANGE) ay = 0;

		XMFLOAT2   endPosition;
		endPosition.x = -1;
		endPosition.y = -1;
		XMFLOAT2 startPosition;
		startPosition.x = ix;
		startPosition.y = iy;

		int n = 5 + rand() % (MAR_RANGE - 5);
		que.push(startPosition);
		visited[(MAR_RANGE - 1)][(MAR_RANGE - 1)] = 1;

		while (true)
		{
			XMFLOAT2 pos = que.front(); que.pop();
			int mx, my;
			int count;
			mx = pos.x;
			my = pos.y;
			count = visited[my + ay][mx + ax];

			/*
			중복되지않는 4개의 숫자 뽑는 알고리즘을 사용해서 4가지 움직이는 방향을 랜덤으로 한다.
			*/
#pragma region[4가지 방향을 랜덤으로]

			bool bRand[4] = { false };
			int nCount = 3;

			int moveNumber;

			while (1)
			{
				moveNumber = rand() % 4;

				while (bRand[moveNumber])
				{
					moveNumber = rand() % 4;
				}
				bRand[moveNumber] = true;


				if (moveNumber == 0)
				{
					if (my + 1 < h)
					{
						if (gMap[my + 1][mx] == 0 && visited[my + 1 + ay][mx + ax] == 0)
						{
							que.push(XMFLOAT2(mx, my + 1));
							visited[my + 1 + ay][mx + ax] = count + 1;
						}

						if (gMap[my + 1][mx] == 100 && visited[my + 1 + ay][mx + ax] == 100)
						{
							que.push(XMFLOAT2(mx, my + 1));
							visited[my + 1 + ay][mx + ax] = count + 1;
						}
					}
				}

				else if (moveNumber == 1)
				{
					if (my - 1 >= 0)
					{
						if (gMap[my - 1][mx] == 0 && visited[my - 1 + ay][mx + ax] == 0)
						{
							que.push(XMFLOAT2(mx, my - 1));
							visited[my - 1 + ay][mx + ax] = count + 1;
						}

						if (gMap[my - 1][mx] == 100 && visited[my - 1 + ay][mx + ax] == 100)
						{
							que.push(XMFLOAT2(mx, my - 1));
							visited[my - 1 + ay][mx + ax] = count + 1;
						}
					}
				}

				else if (moveNumber == 2)
				{
					if (mx + 1 < w)
					{
						if (gMap[my][mx + 1] == 0 && visited[my + ay][mx + 1 + ax] == 0)
						{
							que.push(XMFLOAT2(mx + 1, my));
							visited[my + ay][mx + 1 + ax] = count + 1;
						}

						if (gMap[my][mx + 1] == 100 && visited[my + ay][mx + 1 + ax] == 100)
						{
							que.push(XMFLOAT2(mx + 1, my));
							visited[my + ay][mx + 1 + ax] = count + 1;
						}
					}

				}

				else
				{
					if (mx - 1 >= 0)
					{
						if (gMap[my][mx - 1] == 0 && visited[my + ay][mx - 1 + ax] == 0)
						{
							que.push(XMFLOAT2(mx - 1, my));
							visited[my + ay][mx - 1 + ax] = count + 1;
						}
						if (gMap[my][mx - 1] == 100 && visited[my + ay][mx - 1 + ax] == 100)
						{
							que.push(XMFLOAT2(mx - 1, my));
							visited[my + ay][mx - 1 + ax] = count + 1;
						}
					}
				}

				bool check = bRand[0];
				for (int i = 0; i < 4; ++i)
					check = bRand[i] & check;
				if (check)break;

			}

#pragma endregion	

			/*
			4가지 방향을 순서대로
			*/

#pragma region[그냥 4가지 방향]
			/*
			if (my + 1 < h)
			{
			if (gMap[my + 1][mx] == 0 && visited[my + 1 + ay][mx + ax] == 0)
			{
			que.push(XMFLOAT2(mx, my + 1));
			visited[my + 1 + ay][mx + ax] = count + 1;
			}
			}

			if (my - 1 >= 0)
			{
			if (gMap[my - 1][mx] == 0 && visited[my - 1 + ay][mx + ax] == 0)
			{
			que.push(XMFLOAT2(mx, my - 1));
			visited[my - 1 + ay][mx + ax] = count + 1;
			}
			}

			if (mx + 1 < w)
			{
			if (gMap[my][mx + 1] == 0 && visited[my + ay][mx + 1 + ax] == 0)
			{
			que.push(XMFLOAT2(mx + 1, my));
			visited[my + ay][mx + 1 + ax] = count + 1;
			}
			}

			if (mx - 1 >= 0)
			{
			if (gMap[my][mx - 1] == 0 && visited[my + ay][mx - 1 + ax] == 0)
			{
			que.push(XMFLOAT2(mx - 1, my));
			visited[my + ay][mx - 1 + ax] = count + 1;
			}
			}
			*/
#pragma endregion

			if (que.empty() || n == count)
			{
				endPosition.x = mx;
				endPosition.y = my;
				break;
			}

		}//while(true);

		int ex, ey;
		ex = endPosition.x + ax; 	ey = endPosition.y + ay;
		std::deque<XMFLOAT2> path;
		int nPathLoop = 0;
		//path.reserve(visited[ey - ay][ex - ax]);
		path.push_back(XMFLOAT2(ex, ey));

		while (1)
		{
			int mx, my;
			int count;
			mx = ex; my = ey;
			count = visited[my][mx];

			if (count == 1 || ++nPathLoop > MAR_RANGE)
				break;

			if (visited[my + 1][mx] == count - 1)
			{
				ex = mx;
				ey = my + 1;
				path.push_back(XMFLOAT2(ex, ey));
			}
			else if (visited[my - 1][mx] == count - 1)
			{
				ex = mx;
				ey = my - 1;
				path.push_back(XMFLOAT2(ex, ey));
			}
			else if (visited[my][mx + 1] == count - 1)
			{
				ex = mx + 1;
				ey = my;
				path.push_back(XMFLOAT2(ex, ey));
			}
			else if (visited[my][mx - 1] == count - 1)
			{
				ex = mx - 1;
				ey = my;
				path.push_back(XMFLOAT2(ex, ey));
			}

		}
		return path;
	};


	static ID3D11ShaderResourceView* MapTexture(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext, std::string path = "../Assets/01_MapData/CSV/", std::string filename = "CubeTerrainHeight")
	{
		path.append(filename.c_str());
		path.append(".csv");

		char msg[MAX_ALL_BUFFER];
		std::ifstream fin;
		fin.open(path);

		if (!fin) fMessageBox("CSV_ERROR", path);

		int z = 0;
		int n = 0;
		int maxZ = 0;

		fin.getline(msg, MAX_ALL_BUFFER);
		{
			int number[4];
			int nNumber = 0;
			while (msg[nNumber] != ',')
			{
				number[nNumber] = msg[nNumber] - 48;
				nNumber++;
			}
			n = MergeNumber(number, nNumber);

			memset(&number, NULL, sizeof(number));
			int yCount = 0;
			while (msg[++nNumber] != ',')
			{
				number[yCount++] = msg[nNumber] - 48;
			}
			maxZ = MergeNumber(number, yCount);


		}

		XMFLOAT4 **RV = nullptr;
		RV = new XMFLOAT4*[maxZ];

		for (int i = 0; i < maxZ; ++i)
			RV[i] = new XMFLOAT4[n];


		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			int x = 0;
			for (int nLoopCount = 0; nLoopCount < n; ++nLoopCount)
			{
				int number[20] = { NULL };
				int nNumber = 0;
				while (msg[++x] != ',')
				{
					number[nNumber++] = msg[x] - 48;
				}
				RV[z][nLoopCount] = XMFLOAT4(MergeNumber(number, nNumber), MergeNumber(number, nNumber), MergeNumber(number, nNumber), 1.0f);
			}
			z++;
		}

		D3D11_SUBRESOURCE_DATA d3dSubresourceData;
		ZeroMemory(&d3dSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		d3dSubresourceData.pSysMem = RV;
		d3dSubresourceData.SysMemPitch = sizeof(XMFLOAT4) * 1024;
		d3dSubresourceData.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC d3dTextureDesc;
		ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTextureDesc.Width = FRAME_BUFFER_WIDTH;
		d3dTextureDesc.Width = FRAME_BUFFER_HEIGHT;
		d3dTextureDesc.MipLevels = 1;
		d3dTextureDesc.ArraySize = 1;
		d3dTextureDesc.SampleDesc.Count = 1;
		d3dTextureDesc.SampleDesc.Quality = 0;

		//포맷을 RGBA
		d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		//
		//----------------------------------------Resource Desc-----------------------------------------//
		//D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		//::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		//d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//d3dSRVDesc.Texture2D.MipLevels = 1;
		////d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		////----------------------------------------Resource Desc-----------------------------------------//
		//
		//d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//

		ID3D11Texture2D *pd3dTexture;


		// Texture1D를 제작
		pd3dDevice->CreateTexture2D(&d3dTextureDesc, &d3dSubresourceData, &pd3dTexture);
		ID3D11ShaderResourceView *pd3dsrvTexture;
		//쉐이더 리소스 뷰 제작
		HRESULT h = pd3dDevice->CreateShaderResourceView(pd3dTexture, nullptr, &pd3dsrvTexture);
		if (FAILED(h))fMessageBox("Save Error Not Texture", "Save Error");

		else
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			_TCHAR name[256];
			_stprintf_s(name, _T("%d_%d_%d_%dh%dm%ds.png"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, 50);

			//스크린샷파일생성
			ID3D11Resource *res;
			pd3dsrvTexture->GetResource(&res);
			HRESULT hResult = D3DX11SaveTextureToFile(pd3dDeviceContext, res, D3DX11_IFF_PNG, name);
			if (SUCCEEDED(hResult)) fMessageBox("Save SUCCEEDED Screen", "SUCCEEDED");
			else if (FAILED(hResult)) fMessageBox("Save FAILED Screen", "FAILED");
			res->Release();
			pd3dTexture->Release();
		}

		return(pd3dsrvTexture);
	}

	//'16.04.03 KYT
	/*
	숫자 반올림
	*/
	static int Rounding(float f)
	{
		float a = f - (int)f;
		if (a >= 0.5f)	return ((int)f + 1);
		return (int)f;
	}

	//KYT '16.05.05
	/*
	어린이날 기념 Runtime중에 데이터 수정하기
	txt가 없으면 자동으로 넣어줌
	*/
public:
	/*
	static float RuntimeData(float, std::string path)
	{
	float f;
	std::ifstream fin(path);
	if (fin.is_open())GMessageBox("업대..", path);
	fin >> f;
	fin.close();
	return (float)f;
	}
	static int RuntimeData(int, std::string path)
	{
	int n;
	std::ifstream fin(path);
	if (fin.is_open())GMessageBox("업대..", path);
	fin >> n;
	fin.close();
	return (int)n;
	}
	static XMFLOAT3 RuntimeData(XMFLOAT3, std::string path)
	{
	std::vector<float> v;
	std::ifstream fin(path);
	if (fin.is_open()) GMessageBox("업대..", path);
	for (int i = 0; i < 3; ++i)
	{
	float f;
	fin >> f;
	v.push_back(f);
	}
	fin.close();
	return XMFLOAT3(v[0], v[1], v[2]);
	}
	static XMFLOAT2 RuntimeData(XMFLOAT2, std::string path)
	{
	std::vector<float> v;
	std::ifstream fin(path);
	if (fin.is_open()) GMessageBox("업대..", path);
	for (int i = 0; i < 2; ++i)
	{
	float f;
	fin >> f;
	v.push_back(f);
	}
	fin.close();
	return XMFLOAT2(v[0], v[1]);
	}

	template <class ReturnType>
	static ReturnType RuntimeDataModify(std::string fileName, std::string extension = ".txt")
	{
	std::string path = "..\Assets\06_DebugFile";
	path.append(fileName);
	if (path.find(extension) >= path.size()) path.append(extension);
	return RuntimeData(ReturnType(), path);
	}

	template <class ReturnType>
	static ReturnType RuntimeDataModify(ReturnType, std::string fileName, std::string extension = ".txt")
	{
	std::string path = "..\Assets\06_DebugFile";
	path.append(fileName);
	if (path.find(extension) >= path.size()) path.append(extension);
	return RuntimeData(ReturnType(), path);
	}
	*/

	static float RuntimeDataModify(float, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::ifstream fin;
		fin.open(path);
		fin.getline(msg, MAX_ALL_BUFFER);

		fin.close();
		float f = static_cast<float>(atof(msg));
		return f;
	}

	static void RuntimeDataModify(float* f, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::ifstream fin;
		fin.open(path);
		fin.getline(msg, MAX_ALL_BUFFER);

		fin.close();
		(*f) = static_cast<float>(atof(msg));
	}

	static XMFLOAT3 RuntimeDataModify(XMFLOAT3, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::vector<float> v;
		std::ifstream fin;
		fin.open(path);
		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			float f = static_cast<float>(atof(msg));
			v.push_back(f);
		}
		fin.close();
		return XMFLOAT3(v[0], v[1], v[2]);
	}

	static void RuntimeDataModify(XMFLOAT3 *f3, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::vector<float> v;
		std::ifstream fin;
		fin.open(path);
		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			float f = static_cast<float>(atof(msg));
			v.push_back(f);
		}
		fin.close();
		(*f3).x = v[0]; (*f3).y = v[1]; (*f3).z = v[2];
	}


	static XMFLOAT4 RuntimeDataModify(XMFLOAT4, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::vector<float> v;
		std::ifstream fin;
		fin.open(path);
		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			float f = static_cast<float>(atof(msg));
			v.push_back(f);
		}
		fin.close();
		return XMFLOAT4(v[0], v[1], v[2], v[3]);
	}

	static void RuntimeDataModify(XMFLOAT4 *f4, std::string fileName)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);

		if (path.find(".txt") >= path.size())
			path.append(".txt");

		std::vector<float> v;
		std::ifstream fin;
		fin.open(path);
		while (fin.getline(msg, MAX_ALL_BUFFER))
		{
			float f = static_cast<float>(atof(msg));
			v.push_back(f);
		}
		fin.close();
		(*f4).x = v[0]; (*f4).y = v[1]; (*f4).z = v[2]; (*f4).w = v[3];
	}


	template<class Type>
	static std::vector<Type> RuntimeDataModify(std::vector<Type>, std::string fileName, std::string format = "txt", int loop = 10000)
	{
		char msg[MAX_ALL_BUFFER];
		std::string path = "../Assets/06_DebugFile/";
		path.append(fileName);
		path.append(".");

		if (path.find(format) >= path.size())
			path.append(format);

		std::vector<Type> v;
		std::ifstream fin;

		fin.open(path);

		if (!fin.is_open()) ::GMessageBox(msg, "파일 없음");

		if (format == "txt") format = '\t';
		else format = ',';

		while (fin.getline(msg, MAX_ALL_BUFFER) && (loop > 0))
		{
			int x = 0;
			int number[20] = { NULL };
			int nNumber = 0;

			while (msg[++x] != format[0]);

			while (msg[++x] != NULL)
			{
				number[nNumber++] = msg[x] - 48;
			}
			v.push_back(MergeNumber(number, nNumber));
			loop--;
		}
		fin.close();

		return v;
	}

	static float getDegree(XMFLOAT3 A, XMFLOAT3 B)
	{
		A.y *= -1;
		XMFLOAT3 xmFloat3;
		XMVECTOR a = XMLoadFloat3(&A);
		XMVECTOR b = XMLoadFloat3(&B);
		a = XMVector3Normalize(a);
		b = XMVector3Normalize(b);
		XMStoreFloat3(&xmFloat3, XMVector3Dot(a, b));
		float fAngle = static_cast<float>(D3DXToDegree(acos(xmFloat3.x)));
		return fabs(fAngle);
	}

	static bool IsRenderCheck(int nLoopNumber, int camPos, int nRoomCount)
	{

		int nMaxCount = SPACE->GetRoomContainer().size() - 1;
		int n;

		switch (nLoopNumber)
		{
		case 0:
		{
			n = nRoomCount - 9;
			return (n >= 0 & n == camPos);
		}
		case 1:
		{
			n = nRoomCount - 8;
			return (n >= 0 & n == camPos);
		}

		case 2:
		{
			n = nRoomCount - 7;
			return (n >= 0 & n == camPos);
		}

		case 3:
		{
			n = nRoomCount - 1;
			return (n >= 0 & n == camPos);
		}

		case 4:
		{
			n = nRoomCount + 1;
			return (n <= nMaxCount & n == camPos);
		}

		case 5:
		{
			n = nRoomCount + 7;
			return (n <= nMaxCount & n == camPos);
		}

		case 6:
		{
			n = nRoomCount + 8;
			return (n <= nMaxCount & n == camPos);
		}

		case 7:
		{
			n = nRoomCount + 9;
			return (n <= nMaxCount & n == camPos);
		}
		}

	}

	template<class Type = int>
	static std::vector<Type> IsRenderCountVector(int camPos)
	{
		std::vector<Type> vRoomNumber;
		int nMaxCount = SPACE->GetRoomContainer().size() - 1;

		//-------------------------------------------------------
		if (camPos - 9 >= 0)
			vRoomNumber.push_back(camPos - 9);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------

		if (camPos - 8 >= 0)
			vRoomNumber.push_back(camPos - 8);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------
		if (camPos - 7 >= 0)
			vRoomNumber.push_back(camPos - 7);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------

		if (camPos - 1 >= 0)
			vRoomNumber.push_back(camPos - 1);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------
		if (camPos + 1 <= nMaxCount)
			vRoomNumber.push_back(camPos + 1);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------
		if (camPos + 7 <= nMaxCount)
			vRoomNumber.push_back(camPos + 7);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------
		if (camPos + 8 <= nMaxCount)
			vRoomNumber.push_back(camPos + 8);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------
		if (camPos + 9 <= nMaxCount)
			vRoomNumber.push_back(camPos + 9);
		else
			vRoomNumber.push_back(-1);
		//-------------------------------------------------------

		return vRoomNumber;
	}

	static void Font_Message(const int& x, const int& y, const XMFLOAT3& f3, const std::wstring& wstr = L"")
	{
		FONTMGR->MessagePush("기본", x, y, 20, YT_Color(0, 0, 0), L"(%1.f, %1.f, %1.f)", f3.x, f3.y, f3.z);
	}

	static void Font_Message(const int& x, const int& y, const XMFLOAT3& f3, const TCHAR* wstr = L"")
	{
		FONTMGR->MessagePush("기본", x, y, 20, YT_Color(0, 0, 0), L"(%1.f, %1.f, %1.f)", f3.x, f3.y, f3.z);
	}

	static BYTE *ReadCompiledEffectFile(WCHAR *pszFileName, int *pnReadBytes)
	{
		FILE *pFile = NULL;
		::_wfopen_s(&pFile, pszFileName, L"rb");
		::fseek(pFile, 0, SEEK_END);
		int nFileSize = ::ftell(pFile);
		BYTE *pByteCode = new BYTE[nFileSize];
		::rewind(pFile);
		*pnReadBytes = ::fread(pByteCode, sizeof(BYTE), nFileSize, pFile);
		::fclose(pFile);
		return(pByteCode);
	}

	static float Round(float f, int range)
	{
		return static_cast<int>(f * range) / range;
	}
};


namespace Radar
{
	inline void DXUT_SetDebugName(IDXGIObject* pObj, const CHAR* pstrName )
	{
		if ( pObj )
		   pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
	}

	inline void DXUT_SetDebugName( ID3D10Device* pObj, const CHAR* pstrName )
	{
		if ( pObj )
			pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
	}

	inline void DXUT_SetDebugName( ID3D10DeviceChild* pObj, const CHAR* pstrName )
	{
		if ( pObj )
			pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
	}

	inline void DXUT_SetDebugName( ID3D11Device* pObj, const CHAR* pstrName )
	{
		if ( pObj )
			pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
	}

	inline void DXUT_SetDebugName( ID3D11DeviceChild* pObj, const CHAR* pstrName )
	{
		if ( pObj )
			pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
	}

}
