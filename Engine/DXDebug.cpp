#include "pch.h"
#include "DXDebug.h"

namespace Engine
{
	DXDebug DXDebug::mInstance;

	void DXDebug::Enable()
	{
		GetInterface();

	}

	void DXDebug::GetLiveObjects()
	{
		GetInterface();
		Get()->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}

	void DXDebug::GetInterface()
	{

		if (!Get()) {

			ENGINE_EVAL_HR(DXGIGetDebugInterface1(0, IID_PPV_ARGS(GetAddressOf())), "Error creating the DXGI Debug interface");

		}
	}


}