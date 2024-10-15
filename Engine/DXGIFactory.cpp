#include "pch.h"
#include "DXGIFactory.h"



namespace Engine
{
	using namespace Microsoft::WRL;

	DXGIFactory::DXGIFactory()
	{
		ENGINE_EVAL_HR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)), "Error creating the Factory2.");


	}

	DXGIAdapter DXGIFactory::GetAdapter()
	{

		ComPtr<IDXGIFactory6> fac6;
		ComPtr<IDXGIAdapter> adapter;

		if (Get()->QueryInterface(IID_PPV_ARGS(&fac6)) == S_OK)
		{
			ENGINE_EVAL_HR(fac6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)), "Error Finding the adapter.");

			// more elaborate error handling (for example if i want to get a message if i couldn't find any adapter)

		}
		else
		{
			ENGINE_ASSERT(false);
		}

		return DXGIAdapter(adapter.Get());
	}

}
