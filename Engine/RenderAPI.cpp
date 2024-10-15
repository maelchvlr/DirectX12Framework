#include "pch.h"
#include "RenderAPI.h"

#include "DXGIFactory.h"
#include "DXGIAdapter.h"

namespace Engine
{
	RenderAPI::~RenderAPI()
	{
	}


	void RenderAPI::Initialize(HWND hwnd)
	{

		DXGIFactory factory;

		DXGIAdapter adapter = factory.GetAdapter();

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		PRINT_W_N("Selected GPU: " << desc.Description << ".");

		mDevice.Init(adapter.Get());

		mDevice->SetName(L"Main virtual device");
	}
}