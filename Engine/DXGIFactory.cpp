#include "pch.h"
#include "DXGIFactory.h"



namespace Engine
{
	using namespace Microsoft::WRL;

	DXGIFactory::DXGIFactory()
	{
		ENGINE_EVAL_HR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)), "Some error");


	}

	DXGIAdapter DXGIFactory::GetAdapter()
	{

		ComPtr<IDXGIFactory6> fac6;
		ComPtr<IDXGIAdapter> adapter;

		if (Get()->QueryInterface(IID_PPV_ARGS(&fac6)) == S_OK)
		{


		}



		return DXGIAdapter();
	}

}
