#include "pch.h"
#include "D12Debug.h"




namespace Engine {
	D12Debug D12Debug::mInstance;


	void D12Debug::Enable()
	{
		GetInterface();
		Get()->EnableDebugLayer();

	}

	void D12Debug::GetInterface()
	{
		if (!Get()) {
			ENGINE_EVAL_HR(D3D12GetDebugInterface(IID_PPV_ARGS(GetAddressOf())), "Error while getting the debug interface");

		}


	}

}