#pragma once

#include "stdafx.h"
#include <string.h>
#include <iostream>

#ifdef _UNICODE
	using tstring = std::wstring;
	#define tcout  wcout
#else // _UNICODE
	using tstring = std::string;
	#define tcout  cout
#endif // _UNICODE

#ifdef DX12_MIGRATION
#define _XTT TEXT
class CGlobalvariable : public CSingleTonBase<CGlobalvariable>
{
protected:
	tstring								m_CommandLine;

	ComPtr<ID3D12Device>				m_pID3D12Device;
	ComPtr<ID3D12CommandAllocator>		m_pd3dCommandAllocator;
	ComPtr<ID3D12CommandQueue>			m_pd3dCommandQueue;
	ComPtr<ID3D12GraphicsCommandList>	m_pd3dCommandList;
	ComPtr<ID3D12Fence>					m_pd3dFence;

	unsigned int				m_nCbvSrvDescriptorIncrementSize;

public:

	CGlobalvariable() : CSingleTonBase()
	{
		Initialize();
	}

	~CGlobalvariable()
	{
		ReleseInstance();
	}

	void ReleseInstance()
	{
	}

public:

	WARP_RESULT_ENUM Initialize()
	{
		Release();
		return  WARP_RESULT_ENUM::OK;
	}

	void SetDevice(ID3D12Device* pID3D12Device){ m_pID3D12Device = pID3D12Device; }
	ID3D12Device*	GetDevice() { return m_pID3D12Device.Get(); }

	void SeCommandAllocator(ComPtr<ID3D12CommandAllocator> pCommandAllocator)			 { m_pd3dCommandAllocator = pCommandAllocator; }
	void SetCommandQueue(ComPtr<ID3D12CommandQueue> pCommandQueue)						 { m_pd3dCommandQueue = pCommandQueue; }
	void SetGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> pGrahpicsCommandList) { m_pd3dCommandList = pGrahpicsCommandList; }
	void SetCommandLine(LPWSTR commandLine) { m_CommandLine = commandLine; }

	ID3D12CommandAllocator*		GetCommandAllocator()									 { return m_pd3dCommandAllocator.Get(); }
	ID3D12CommandQueue*			GetCommandQueue()										 { return m_pd3dCommandQueue.Get(); }
	ID3D12GraphicsCommandList*	GetGraphicsCommandList() { return m_pd3dCommandList.Get(); }
	const tstring				GetCommandLineMsg() { return m_CommandLine; }

	void						SetCBSrvDescriptorIncrementSize(const unsigned int size) { m_nCbvSrvDescriptorIncrementSize = size; }
	unsigned int				GetCBSrvDescriptorIncrementSize() const { return m_nCbvSrvDescriptorIncrementSize;}

};	
#endif //DX12_MIGRATION