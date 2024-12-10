#include "pch.h"
#include "RenderAPI.h"

#include <vector>

#include "DXGIFactory.h"
#include "DXGIAdapter.h"

#include "D12Debug.h"


namespace Engine
{
	using namespace Render;
	RenderAPI::~RenderAPI()
	{
		Release();
	}


	void RenderAPI::Initialize(HWND hwnd, const UINT width, const UINT height)
	{
		mWidth = width;
		mHeight = height;

		D12Debug::Get().Enable();

		DXGIFactory factory;

		DXGIAdapter adapter = factory.GetAdapter();

		///////////////

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		PRINT_W_N("Selected GPU: " << desc.Description << ".");

		///////////////


		mDevice.Init(adapter.Get());

		mDevice->SetName(L"Main virtual device");


		mCommandQueue.Initialize(mDevice.Get());
		mCommandList.Initialize(mDevice.Get());
		

		mSwapChain.Initialize(mDevice.Get(), factory.Get(), mCommandQueue.Get(), hwnd, mWidth, mHeight);

		mDynamicVertexBuffer.Initialize(mDevice.Get(), KBs(16), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
		mDynamicVertexBuffer.Get()->SetName(L"Dynamic Vertex Buffer");

		std::vector<Vertex> vertices;

		for (int i = 0; i < 3; i++)
		{
			Vertex vertexData;
			vertexData.color = { 0.0f, 1.0f, 0.0f, 1.0f };

			switch (i)
			{
				case 0:
					vertexData.position = { -0.6f, -0.6f, 0.0f };
					vertexData.color = { 1.0f, 0.0f, 0.0f, 1.0f };
					break;
				case 1:
					vertexData.position = { 0.0f, 0.6f, 0.0f };
					vertexData.color = { 0.0f, 1.0f, 0.0f, 1.0f };
					break;
				case 2:
					vertexData.position = { 0.6f, -0.6f, 0.0f };
					vertexData.color = { 0.0f, 0.0f, 1.0f, 1.0f };
					break;

				default:
					break;
			}

			vertices.push_back(vertexData);
		}
		


		// Screen Space coordinates only have X and Y cooodinates mapped to (-1, 1) -> (1,-1)

		void* destination = nullptr;

		mDynamicVertexBuffer->Map(0, 0, &destination);
		memcpy(destination, vertices.data(), sizeof(Vertex) * vertices.size());
		mDynamicVertexBuffer->Unmap(0, 0);

		mDynamicVertexBufferView.BufferLocation = mDynamicVertexBuffer.Get()->GetGPUVirtualAddress();
		mDynamicVertexBufferView.StrideInBytes = sizeof(Vertex);
		mDynamicVertexBufferView.SizeInBytes = KBs(16);
		


		/*
		 PLAN:
		
		Create Shader Programs:
		- wrapper and compilation [X]
		- create the actual shader program [X]


		Setup two input layouts (vertex/index buffers + one for datastructure needed for the pipeline/shader):
		- pipeline input [X]
		-- wrapper ? [X]
		- root signature [X]
		-- wrapper ? [X]


		setup the actual pipeline:
		- wrapper [X]
		-- set parameters [X]
		- create the functionnality that couples everything together into a pipeline [X]

		
		*/
		// DONE



		/*
		// ONLY CPU = default ram / cache
		// ONLY GPU = default heap on GPU (VRAM)
		// Shared CPU and GPU = with read/write for all - it's stored on the GPU
		// Readback memory on GPU(With Read from the CPU)
		//
		*/

		mBasePipeline.Initialize(mDevice.Get());


		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		mViewport.Width = mWidth;
		mViewport.Height = mHeight;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		mScissorRect.left = 0;
		mScissorRect.top = 0;
		mScissorRect.right = mViewport.Width;
		mScissorRect.bottom = mViewport.Height;


		/*
		Next steps:

		bind render targer to every part of the output merger [X]
		
		bind the root signature and pipeline to the GPU / draw process [X]

		Tell how to interpret data -> primitive topology [X]
		
		bind a datastorage to the "pipeline"/input assembler and give it a view [X]

		Draw call [X]

		Viewport []

		Scissor rect []

		*/



	}

	void RenderAPI::UpdateDraw()
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mSwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mCommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		const float clearColor[] = { 0.0f, 0.2f, 0.2f, 1.0f };
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mSwapChain.GetCurrentRTVHandle();
		mCommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);
		mCommandList.GFXCmd()->OMSetRenderTargets(1, &rtvHandle, false, 0);

		mCommandList.GFXCmd()->RSSetViewports(1, &mViewport);
		mCommandList.GFXCmd()->RSSetScissorRects(1, &mScissorRect);


		mCommandList.GFXCmd()->SetGraphicsRootSignature(mBasePipeline.GetRootSignature());
		mCommandList.GFXCmd()->SetPipelineState(mBasePipeline.Get());
		mCommandList.GFXCmd()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList.GFXCmd()->IASetVertexBuffers(0, 1, &mDynamicVertexBufferView);



		/*
		
		do drawing stuff there
		
		
		
		*/


		mCommandList.GFXCmd()->DrawInstanced(3, 1, 0, 0);

		barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mSwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		mCommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		mCommandList.GFXCmd()->Close();
		mCommandQueue.M_ExecuteCommandList(mCommandList.Get());

		mSwapChain.Present();

		while (mCommandQueue.GetFence()->GetCompletedValue() < mCommandQueue.M_GetCurrentFenceValue())
		{
			_mm_pause();
		}

		mCommandList.ResetCommandList();
	}


	void RenderAPI::Release()
	{
		mDynamicVertexBuffer.Release();

		mCommandQueue.FlushQueue();


		mSwapChain.Release();

		mCommandList.Release();
		mCommandQueue.Release();



		if (mDevice.Get())
		{
			mDevice.Reset();
		}
	}
}