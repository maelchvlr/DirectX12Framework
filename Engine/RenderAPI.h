#pragma once
#include "EngineMin.h"
#include <Windows.h>

#include "RenderDataTypes.h"

#include "D12Device.h"
#include "D12CommandQueue.h"
#include "D12CommandList.h"
#include "DXGISwapChain.h"

#include "D12Ressource.h"

#include "D12PipelineState.h"

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

		D12Ressource mDynamicVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW mDynamicVertexBufferView;


		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;

	private:
		UINT mWidth = 0;
		UINT mHeight = 0;

	};
}

