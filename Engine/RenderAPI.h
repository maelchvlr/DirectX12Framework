#pragma once
#include "pch.h"

#include "RenderDataTypes.h"

#include "D12Device.h"
#include "D12CommandQueue.h"
#include "D12CommandList.h"
#include "DXGISwapChain.h"

#include "D12Ressource.h"

#include "D12PipelineState.h"
#include "D12DescriptorHeap.h"
#include "BufferUploader.h"

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
		D12PipelineState mBlurPipeline;
		D12Ressource mDepthBuffer;

		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;

		D12DescriptorHeap mDepthDescHeap;
		BufferUploader mBufferUploader;

		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

		DirectX::XMMATRIX mViewProjectionMatrix;

		//scene resources

		D12Ressource mVertexBuffer;
		D12Ressource mIndexBuffer;
		std::vector<D12Ressource> mMaterialsBuffer;
		std::vector<D12Ressource> mObjTransforms;

		D12Ressource mCBPassData;
		Render::Light mLights[8];

		D12Ressource mSceneRenderTarget;


	private:
		UINT mWidth = 0;
		UINT mHeight = 0;

	};
}

