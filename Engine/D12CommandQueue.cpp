#include "pch.h"
#include "D12CommandQueue.h"


namespace Engine
{
	D12CommandQueue::~D12CommandQueue()
	{

		Release();
	}


	void D12CommandQueue::Initialize(ID3D12Device* device)
	{

		//Create the ressources needed:
		//the queue
		//the fence


		//Direct commandqueue
		D3D12_COMMAND_QUEUE_DESC description = {};
		description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		description.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		description.NodeMask = 0;


		ENGINE_EVAL_HR(device->CreateCommandQueue(&description, IID_PPV_ARGS(GetAddressOf())), "Error creating the command queue.");

		ENGINE_EVAL_HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf())), "Error creating the fence.");

	}

	void D12CommandQueue::Release()
	{

		if (Get())
		{
			Reset();
		}

		if (mFence.Get())
		{
			mFence.Reset();
		}
	}
}