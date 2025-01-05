#pragma once
#include "D12Ressource.h"
#include "D12CommandList.h"
#include "D12CommandQueue.h"

namespace Engine
{
	class ENGINE_API BufferUploader
	{
	public:
		BufferUploader() = default;
		~BufferUploader();

		void Initialize(ID3D12Device* pDevice, const UINT memoryAllocation);

		void Upload(D12Ressource* pDestination, void* pSourceData, const UINT size, D12CommandList* pCommandList, D12CommandQueue* pCommandQueue, D3D12_RESOURCE_STATES customState = D3D12_RESOURCE_STATE_COMMON );

		void Release();


	private:
		D12Ressource mInternalBuffer;
		void* mInternalMemory = nullptr;

		UINT mMaxSize = 0;
	};
}

