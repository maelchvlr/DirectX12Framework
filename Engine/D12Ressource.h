#pragma once
#include <wrl.h>

namespace Engine 
{
	class D12Ressource : public Microsoft::WRL::ComPtr<ID3D12Resource>
	{

	public:
		D12Ressource() = default;
		~D12Ressource();

		void Initialize(ID3D12Device* pDevice, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);

		void InitializeAsDepthBuffer(ID3D12Device* pDevice, const unsigned int width, const unsigned int height);

		void Release();
		void* GetCPUMemory();

	private:
		void* mMemory = nullptr;

	};


}

