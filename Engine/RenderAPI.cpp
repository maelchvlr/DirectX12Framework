#include "pch.h"
#include "RenderAPI.h"

#include <vector>

#include "DXGIFactory.h"
#include "DXGIAdapter.h"

#include "D12Debug.h"

#include "Utils.h"


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

#define G_BOX_VERTICES 18

		Vertex verticesBox[G_BOX_VERTICES];

		verticesBox[0].position = { -1.0f, -1.0f, -1.0f };
		verticesBox[0].color = { 0.0f, 0.0f, 0.3f, 1.0f };
		verticesBox[1].position = { -1.0f, 1.0f, -1.0f };
		verticesBox[1].color = { 0.0f, 0.0f, 0.5f, 1.0f };
		verticesBox[2].position = { 1.0f, -1.0f, -1.0f };
		verticesBox[2].color = { 0.0f, 0.0f, 0.5f, 1.0f };

		verticesBox[3].position = { -1.0f, 1.0f, -1.0f };
		verticesBox[3].color = { 0.0f, 0.0f, 0.5f, 1.0f };
		verticesBox[4].position = { 1.0f, 1.0f, -1.0f };
		verticesBox[4].color = { 0.0f, 0.0f, 0.8f, 1.0f };
		verticesBox[5].position = { 1.0f, -1.0f, -1.0f };
		verticesBox[5].color = { 0.0f, 0.0f, 0.5f, 1.0f };

		verticesBox[6].position = { -1.0f, 1.0f, 1.0f };
		verticesBox[6].color = { 0.3f, 0.0f, 0.3f, 1.0f };
		verticesBox[7].position = { -1.0f, -1.0f, 1.0f };
		verticesBox[7].color = { 0.3f, 0.0f, 0.3f, 1.0f };
		verticesBox[8].position = { 1.0f, 1.0f, 1.0f };
		verticesBox[8].color = { 0.3f, 0.0f, 0.3f, 1.0f };

		verticesBox[9].position = { -1.0f, -1.0f, 1.0f };
		verticesBox[9].color = { 0.3f, 0.0f, 0.3f, 1.0f };
		verticesBox[10].position = { 1.0f, -1.0f, 1.0f };
		verticesBox[10].color = { 0.3f, 0.0f, 0.3f, 1.0f };
		verticesBox[11].position = { 1.0f, 1.0f, 1.0f };
		verticesBox[11].color = { 0.3f, 0.0f, 0.3f, 1.0f };

		verticesBox[12].position = { -1.0f,-1.0f, 1.0f };
		verticesBox[12].color = { 0.3f, 0.3f, 0.3f, 1.0f };
		verticesBox[13].position = { -1.0f, 1.0f, 1.0f };
		verticesBox[13].color = { 0.3f, 0.3f, 0.3f, 1.0f };
		verticesBox[14].position = { -1.0f, -1.0f, -1.0f };
		verticesBox[14].color = { 0.3f, 0.3f, 0.3f, 1.0f };

		verticesBox[15].position = { -1.0f,1.0f, 1.0f };
		verticesBox[15].color = { 0.3f, 0.3f, 0.3f, 1.0f };
		verticesBox[16].position = { -1.0f, 1.0f, -1.0f };
		verticesBox[16].color = { 0.3f, 0.3f, 0.3f, 1.0f };
		verticesBox[17].position = { -1.0f, -1.0f, -1.0f };
		verticesBox[17].color = { 0.3f, 0.3f, 0.3f, 1.0f };





		/*for (int i = 0; i < 3; i++)
		{
			Vertex vertexData;
			vertexData.color = { 0.0f, 1.0f, 0.0f, 1.0f };

			switch (i)
			{
				case 0:
					vertexData.position = { -0.5f, -0.5f, 0.0f };
					vertexData.color = { 1.0f, 0.0f, 0.0f, 1.0f };
					break;
				case 1:
					vertexData.position = { 0.0f, 0.5f, 0.0f };
					vertexData.color = { 0.0f, 1.0f, 0.0f, 1.0f };
					break;
				case 2:
					vertexData.position = { 0.5f, -0.5f, 0.0f };
					vertexData.color = { 0.0f, 0.0f, 1.0f, 1.0f };
					break;

				default:
					break;
			}

			vertices.push_back(vertexData);
		}*/
		


		// Screen Space coordinates only have X and Y cooodinates mapped to (-1, 1) -> (1,-1)

		
		memcpy(mDynamicVertexBuffer.GetCPUMemory(), verticesBox, sizeof(Vertex) * G_BOX_VERTICES);


		mDynamicVertexBufferView.BufferLocation = mDynamicVertexBuffer.Get()->GetGPUVirtualAddress();
		mDynamicVertexBufferView.StrideInBytes = sizeof(Vertex);
		mDynamicVertexBufferView.SizeInBytes = KBs(16);
		

		mBasePipeline.Initialize(mDevice.Get());

		mDepthBuffer.InitializeAsDepthBuffer(mDevice.Get(), mWidth, mHeight);

		mDepthDescHeap.InitializeDepthHeap(mDevice.Get());

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		mDevice->CreateDepthStencilView(mDepthBuffer.Get(), &dsvDesc, mDepthDescHeap.Get()->GetCPUDescriptorHandleForHeapStart());


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
		
		DirectX::XMMATRIX viewMatrix;

		viewMatrix = DirectX::XMMatrixLookAtLH({-2.0f, 3.0f, -3.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f});

		DirectX::XMMATRIX projectionMatrix;

		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(70.0f), 16.0f / 9.0f, 1.0f, 50.0f);

		mViewProjectionMatrix = viewMatrix * projectionMatrix;

		mCBPassData.Initialize(mDevice.Get(), Utils::CalculateCOnstantBufferAlignment(sizeof(PassData)), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);



	}

	void RenderAPI::UpdateDraw()
	{
		memcpy(mCBPassData.GetCPUMemory(), &mViewProjectionMatrix, sizeof(PassData));



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
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthDescHeap.Get()->GetCPUDescriptorHandleForHeapStart();


		mCommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);
		mCommandList.GFXCmd()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, 0);
		mCommandList.GFXCmd()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		mCommandList.GFXCmd()->RSSetViewports(1, &mViewport);
		mCommandList.GFXCmd()->RSSetScissorRects(1, &mScissorRect);


		mCommandList.GFXCmd()->SetGraphicsRootSignature(mBasePipeline.GetRootSignature());
		mCommandList.GFXCmd()->SetPipelineState(mBasePipeline.Get());
		mCommandList.GFXCmd()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList.GFXCmd()->IASetVertexBuffers(0, 1, &mDynamicVertexBufferView);


		mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(0, mCBPassData.Get()->GetGPUVirtualAddress());


		
		


		// do drawing stuff there

		mCommandList.GFXCmd()->DrawInstanced(G_BOX_VERTICES, 1, 0, 0);

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