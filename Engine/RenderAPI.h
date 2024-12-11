#pragma once
#include "EngineMin.h"
#include <Windows.h>
#include <DirectXMath.h>

#include "RenderDataTypes.h"

#include "D12Device.h"
#include "D12CommandQueue.h"
#include "D12CommandList.h"
#include "DXGISwapChain.h"

#include "D12Ressource.h"

#include "D12PipelineState.h"
#include "D12DescriptorHeap.h"

namespace Engine
{
	class ENGINE_API RenderAPI
	{

	public:
		RenderAPI() = default;
		~RenderAPI();

		void Initialize(HWND hwnd, const UINT width, const UINT height);

		void UpdateDraw();

		void Release();

	private:

		D12Device mDevice;
		D12CommandQueue	mCommandQueue;
		D12CommandList mCommandList;

		DXGISwapChain mSwapChain;

		D12PipelineState mBasePipeline;
		D12Ressource mDepthBuffer;

		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;

		D12DescriptorHeap mDepthDescHeap;

		D12Ressource mDynamicVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW mDynamicVertexBufferView;

		D12Ressource mCBPassData;

		DirectX::XMMATRIX mViewProjectionMatrix;

	private:
		UINT mWidth = 0;
		UINT mHeight = 0;

	};
}

