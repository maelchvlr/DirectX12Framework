#pragma once
#include <wrl.h>

#include <D3D12.h>
#include <dxgi1_6.h>


namespace Engine
{
	class ENGINE_API D12Device : public Microsoft::WRL::ComPtr<ID3D12Device>
	{
	public:
		D12Device() = default;

		D12Device(IDXGIAdapter* pAdapter);

		void Init(IDXGIAdapter* pAdapter);
	};

}

