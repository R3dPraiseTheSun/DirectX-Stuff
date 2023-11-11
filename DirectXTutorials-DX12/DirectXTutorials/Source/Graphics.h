#pragma once

#include "pch.h"
#include "GraphicsUtils.h"

class Graphics
{
public:
	Graphics(HWND handle, uint32_t width, uint32_t height, bool useWarp);
	~Graphics();

	void EnableDebugLayer();

	void Initialize();

	uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue);


public:
	void Update();
	void Render();
	void Resize(uint32_t width, uint32_t height);
	void Flush();
	void UpdateRenderTargetViews();
	void WaitForFenceValue();
	  
	bool g_UseWarp = false;
	bool g_IsInitialized = false;
	bool g_VSync = true;
	bool g_Fullscreen = false;

	RECT g_WindowRect;

	HANDLE g_FenceEvent;
protected:
	static const uint8_t g_NumFrames = 3;

	bool g_TearingSupported = false;

	uint32_t g_Width;
	uint32_t g_Height;

	// DirectX 12 Objects
	ComPtr<ID3D12Device2> g_Device;
	ComPtr<ID3D12CommandQueue> g_CommandQueue;
	ComPtr<IDXGISwapChain4> g_SwapChain;
	ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];
	ComPtr<ID3D12GraphicsCommandList> g_CommandList;
	ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];
	ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
	UINT g_RTVDescriptorSize;
	UINT g_CurrentBackBufferIndex;

	// Synchronization objects
	ComPtr<ID3D12Fence> g_Fence;
	uint64_t g_FenceValue = 0;
	uint64_t g_FrameFenceValues[g_NumFrames] = {};

private:
	HWND m_Handle;
};

