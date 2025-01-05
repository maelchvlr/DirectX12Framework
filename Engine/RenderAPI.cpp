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

		mBufferUploader.Initialize(mDevice.Get(), KBs(64));



		mVertexBuffer.Initialize(mDevice.Get(), KBs(8), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
		mVertexBuffer.Get()->SetName(L"Vertex Buffer");



		std::vector<Vertex> vertices;

#define G_BOX_VERTICES 24

		Vertex verticesBox[G_BOX_VERTICES];

		verticesBox[0].position = { -1.000000, 1.000000, 1.000000 };
		verticesBox[0].normal = { 0.0000, 1.0000, 0.0000 };

		verticesBox[1].position = { -1.000000, 1.000000, -1.000000 };
		verticesBox[1].normal = { 0.0000, 1.0000, 0.0000 };

		verticesBox[2].position = { 1.000000, 1.000000, 1.000000 };
		verticesBox[2].normal = { 0.0000, 1.0000, 0.0000 };

		verticesBox[3].position = { -1.000000, -1.000000, -1.000000 };
		verticesBox[3].normal = { 0.0000, 0.0000, -1.0000 };

		verticesBox[4].position = { 1.000000, -1.000000, -1.000000 };
		verticesBox[4].normal = { 0.0000, 0.0000, -1.0000 };

		verticesBox[5].position = { -1.000000, 1.000000, -1.000000 };
		verticesBox[5].normal = { 0.0000, 0.0000, -1.0000 };

		verticesBox[6].position = { 1.000000, -1.000000, -1.000000 };
		verticesBox[6].normal = { 1.0000, 0.0000, 0.0000 };

		verticesBox[7].position = { 1.000000, -1.000000, 1.00000 };
		verticesBox[7].normal = { 1.0000, 0.0000 ,0.0000 };

		verticesBox[8].position = { 1.000000, 1.000000, -1.000000 };
		verticesBox[8].normal = { 1.0000, 0.0000, 0.0000 };

		verticesBox[9].position = { 1.000000, -1.000000, 1.000000 };
		verticesBox[9].normal = { 0.0000, -1.0000, 0.0000 };

		verticesBox[10].position = { 1.000000, -1.000000, -1.000000 };
		verticesBox[10].normal = { 0.0000, -1.0000, 0.0000 };

		verticesBox[11].position = { -1.000000, -1.000000, 1.000000 };
		verticesBox[11].normal = { 0.0000, -1.0000, 0.0000 };

		verticesBox[12].position = { -1.000000, -1.000000, 1.000000 };
		verticesBox[12].normal = { -1.0000, 0.0000 ,0.0000 };

		verticesBox[13].position = { -1.000000, -1.000000, -1.000000 };
		verticesBox[13].normal = { -1.0000, 0.0000, 0.0000 };

		verticesBox[14].position = { -1.000000, 1.000000, 1.000000 };
		verticesBox[14].normal = { -1.0000, 0.0000, 0.0000 };

		verticesBox[15].position = { 1.000000, -1.000000, 1.00000 };
		verticesBox[15].normal = { 0.0000, 0.0000 ,1.0000 };

		verticesBox[16].position = { -1.000000, -1.000000, 1.000000 };
		verticesBox[16].normal = { 0.0000, 0.0000, 1.0000 };

		verticesBox[17].position = { 1.000000, 1.000000, 1.000000 };
		verticesBox[17].normal = { 0.0000, 0.0000, 1.0000 };

		//new vertices:

		verticesBox[18].position = { 1.000000, 1.000000, -1.000000 };
		verticesBox[18].normal = { 0.0000, 1.0000, 0.0000 };

		verticesBox[19].position = { 1.000000, 1.000000, -1.000000 };
		verticesBox[19].normal = { 0.0000, 0.0000 ,-1.0000 };

		verticesBox[20].position = { 1.000000, 1.000000, 1.000000 };
		verticesBox[20].normal = { 1.0000, 0.0000, 0.0000 };

		verticesBox[21].position = { -1.000000, -1.000000, -1.000000 };
		verticesBox[21].normal = { 0.0000 ,-1.0000, 0.0000 };

		verticesBox[22].position = { -1.000000, 1.000000, -1.000000 };
		verticesBox[22].normal = { -1.0000, 0.0000, 0.0000 };

		verticesBox[23].position = { -1.000000, 1.000000, 1.000000 };
		verticesBox[23].normal = { 0.0000, 0.0000, 1.0000 };


		mBufferUploader.Upload((D12Ressource*)mVertexBuffer.GetAddressOf(), verticesBox, sizeof(Vertex) * G_BOX_VERTICES, 
			(D12CommandList*)mCommandList.GetAddressOf(), (D12CommandQueue*)mCommandQueue.GetAddressOf(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);


		mVertexBufferView.BufferLocation = mVertexBuffer.Get()->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(Vertex);
		mVertexBufferView.SizeInBytes = KBs(8);

#define G_INDICES 36

		UINT32 indices[G_INDICES];
		
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		indices[6] = 6;
		indices[7] = 7;
		indices[8] = 8;

		indices[9] = 9;
		indices[10] = 10;
		indices[11] = 11;

		indices[12] = 12;
		indices[13] = 13;
		indices[14] = 14;

		indices[15] = 15;
		indices[16] = 16;
		indices[17] = 17;

		indices[18] = 1;
		indices[19] = 18;
		indices[20] = 2;

		indices[21] = 4;
		indices[22] = 19;
		indices[23] = 5;

		indices[24] = 7;
		indices[25] = 20;
		indices[26] = 8;

		indices[27] = 10;
		indices[28] = 21;
		indices[29] = 11;

		indices[30] = 13;
		indices[31] = 22;
		indices[32] = 14;

		indices[33] = 16;
		indices[34] = 23;
		indices[35] = 17;
		
		// Screen Space coordinates only have X and Y cooodinates mapped to (-1, 1) -> (1,-1)

		mIndexBuffer.Initialize(mDevice.Get(), KBs(16), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
		mIndexBuffer.Get()->SetName(L"Index Buffer");

		mBufferUploader.Upload((D12Ressource*)mIndexBuffer.GetAddressOf(), indices, sizeof(UINT32)* G_INDICES,
			(D12CommandList*)mCommandList.GetAddressOf(), (D12CommandQueue*)mCommandQueue.GetAddressOf());


		mIndexBufferView.BufferLocation = mIndexBuffer.Get()->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = KBs(16);

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

		viewMatrix = DirectX::XMMatrixLookAtLH({-8.0f, 3.5f, -8.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f});

		DirectX::XMMATRIX projectionMatrix;

		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(70.0f), 16.0f / 9.0f, 1.0f, 50.0f);

		mViewProjectionMatrix = viewMatrix * projectionMatrix;

		mCBPassData.Initialize(mDevice.Get(), Utils::CalculateCOnstantBufferAlignment(sizeof(PassData)), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);


		// material allocations
		{
			mMaterialsBuffer.reserve(3);


			mMaterialsBuffer.emplace_back(D12Ressource());

			mMaterialsBuffer[0].Initialize(mDevice.Get(), Utils::CalculateCOnstantBufferAlignment(sizeof(MaterialCelShader)), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
			mMaterialsBuffer[0]->SetName(L"Material CB 1");

			MaterialCelShader material;
			material.diffuseAlbedo = { .65f, 0.0f, 0.025f, 1.0f };

			mBufferUploader.Upload((D12Ressource*)mMaterialsBuffer[0].GetAddressOf(), &material, sizeof(MaterialCelShader),
				(D12CommandList*)mCommandList.GetAddressOf(), (D12CommandQueue*)mCommandQueue.GetAddressOf(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			mMaterialsBuffer.emplace_back(D12Ressource());

			mMaterialsBuffer[1].Initialize(mDevice.Get(), Utils::CalculateCOnstantBufferAlignment(sizeof(MaterialCelShader)), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
			mMaterialsBuffer[1]->SetName(L"Material CB 2");

			material.diffuseAlbedo = { .0f, 0.0f,0.65f, 1.0f };

			mBufferUploader.Upload((D12Ressource*)mMaterialsBuffer[1].GetAddressOf(), &material, sizeof(MaterialCelShader),
				(D12CommandList*)mCommandList.GetAddressOf(), (D12CommandQueue*)mCommandQueue.GetAddressOf(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			mMaterialsBuffer.emplace_back(D12Ressource());

			mMaterialsBuffer[2].Initialize(mDevice.Get(), Utils::CalculateCOnstantBufferAlignment(sizeof(MaterialCelShader)), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
			mMaterialsBuffer[2]->SetName(L"Material CB 3 (floor)");

			material.diffuseAlbedo = { .3f, 0.3f,0.3f, 1.0f };

			mBufferUploader.Upload((D12Ressource*)mMaterialsBuffer[2].GetAddressOf(), &material, sizeof(MaterialCelShader),
				(D12CommandList*)mCommandList.GetAddressOf(), (D12CommandQueue*)mCommandQueue.GetAddressOf(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		}


		mLights[0].position = { 0.0f, 0.0f, 0.0f };
		mLights[0].strength = 1.0f;
		mLights[0].direction = { 1.0f, -1.0f, -1.0f };


		// transform Allocations
		{
			mObjTransforms.emplace_back(D12Ressource());
			mObjTransforms[0].Initialize(mDevice.Get(),
				Utils::CalculateCOnstantBufferAlignment(sizeof(ObjectData)),
				D3D12_HEAP_TYPE_UPLOAD,
				D3D12_RESOURCE_STATE_GENERIC_READ);
			mObjTransforms[0]->SetName(L"Object Transform 1");

			ObjectData tempData;
			memcpy(mObjTransforms[0].GetCPUMemory(), &tempData, sizeof(ObjectData));

			mObjTransforms.emplace_back(D12Ressource());
			mObjTransforms[1].Initialize(mDevice.Get(),
				Utils::CalculateCOnstantBufferAlignment(sizeof(ObjectData)),
				D3D12_HEAP_TYPE_UPLOAD,
				D3D12_RESOURCE_STATE_GENERIC_READ);
			mObjTransforms[1]->SetName(L"Object Transform 2");

			tempData.transform.r[0] = { .3f, 0.0f, 1.0f, 0.0f };
			tempData.transform.r[3] = { -3.0f, 0.0f, -2.0f, 0.0f };
			memcpy(mObjTransforms[1].GetCPUMemory(), &tempData, sizeof(ObjectData));

			mObjTransforms.emplace_back(D12Ressource());
			mObjTransforms[2].Initialize(mDevice.Get(),
				Utils::CalculateCOnstantBufferAlignment(sizeof(ObjectData)),
				D3D12_HEAP_TYPE_UPLOAD,
				D3D12_RESOURCE_STATE_GENERIC_READ);
			mObjTransforms[2]->SetName(L"Object Transform 3 (floor)");

			tempData.transform = DirectX::XMMatrixIdentity();
			tempData.transform.r[0] = { 1000.0f, 0.0f, 0.0f, 0.0f };
			tempData.transform.r[1] = { 0.0f, 0.3f, 0.0f, 0.0f };
			tempData.transform.r[2] = { 0.0f, 0.0f, 1000.0f, 0.0f };
			tempData.transform.r[3] = { 0.0f, -4.0f, 0.0f, 1.0f };

			memcpy(mObjTransforms[2].GetCPUMemory(), &tempData, sizeof(ObjectData));
		}

	}

	void RenderAPI::UpdateDraw()
	{
		memcpy(mCBPassData.GetCPUMemory(), &mViewProjectionMatrix, sizeof(PassData::viewProjectionMatrix));
		memcpy((BYTE*)mCBPassData.GetCPUMemory() + sizeof(PassData::viewProjectionMatrix), &mLights[0], sizeof(Light));



		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mSwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mCommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
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

		mCommandList.GFXCmd()->IASetVertexBuffers(0, 1, &mVertexBufferView);
		mCommandList.GFXCmd()->IASetIndexBuffer(&mIndexBufferView);

		// draw call
		{
			//cube1
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(0, mCBPassData.Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(1, mObjTransforms[0].Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(2, mMaterialsBuffer[0].Get()->GetGPUVirtualAddress());

			mCommandList.GFXCmd()->DrawIndexedInstanced(G_INDICES, 1, 0, 0, 0);

			//cube2
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(0, mCBPassData.Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(1, mObjTransforms[1].Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(2, mMaterialsBuffer[1].Get()->GetGPUVirtualAddress());

			mCommandList.GFXCmd()->DrawIndexedInstanced(G_INDICES, 1, 0, 0, 0);

			//floor
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(0, mCBPassData.Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(1, mObjTransforms[2].Get()->GetGPUVirtualAddress());
			mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(2, mMaterialsBuffer[2].Get()->GetGPUVirtualAddress());

			mCommandList.GFXCmd()->DrawIndexedInstanced(G_INDICES, 1, 0, 0, 0);
		}


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

		mCommandQueue.FlushQueue();

		for (int i = 0; i < mMaterialsBuffer.size(); i++)
		{
			mMaterialsBuffer[i].Release();
		}

		for (int i = 0; i < mObjTransforms.size(); i++)
		{
			mObjTransforms[i].Release();
		}

		

		mVertexBuffer.Release();
		mIndexBuffer.Release();
		mCBPassData.Release();



		mBasePipeline.Release();
		mDepthDescHeap.Release();
		mDepthBuffer.Release();

		mBufferUploader.Release();

		mSwapChain.Release();

		mCommandList.Release();
		mCommandQueue.Release();

		if (mDevice.Get())
		{
			mDevice.Reset();
		}
	}
}