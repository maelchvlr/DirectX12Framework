#include "pch.h"
#include "RenderAPI.h"

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

		Vertex vertexData;
		vertexData.position = { 1.0f, 5.0f, 3.0f };
		vertexData.color = { 0.0f, 1.0f, 0.0f, 1.0f };

		void* destination = nullptr;

		mDynamicVertexBuffer->Map(0, 0, &destination);
		memcpy(destination, &vertexData, sizeof(Vertex));
		mDynamicVertexBuffer->Unmap(0, 0);

		


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


		/*
		Next steps:

		bind render targer to every part of the output merger [X]
		
		bind the root signature and pipeline to the GPU / draw process []
		
		bind a datastorage to the "pipeline"/input assembler and give it a view []
		Tell how to interpret data -> primitive topology []

		Draw call []
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



		/*
		
		do drawing stuff there
		
		
		
		*/


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