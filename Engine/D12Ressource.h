#pragma once
#include <wrl.h>

namespace Engine 
{
	class D12Ressource : public Microsoft::WRL::ComPtr<ID3D12Resource>
	{

	public:
		D12Ressource() = default;
		~D12Ressource();

		D12Ressource(const D12Ressource& other) = delete;
		D12Ressource& operator=(const D12Ressource& other) = delete;

		D12Ressource(D12Ressource&& other) noexcept;
		D12Ressource& operator=(D12Ressource&& other) noexcept;

		void Initialize(ID3D12Device* pDevice, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);

		void InitializeAsDepthBuffer(ID3D12Device* pDevice, const unsigned int width, const unsigned int height);
		void InitializeAsRenderTarget(ID3D12Device* pDevice, const unsigned int width, const unsigned int height, DXGI_FORMAT format);

		void CreateRTV(ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);


		void Release();
		void* GetCPUMemory();

	private:
		void* mMemory = nullptr;

	};


}

