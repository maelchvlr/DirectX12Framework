#pragma once
#include "EngineMin.h"
#include <Windows.h>

#include "D12Device.h"

namespace Engine
{
	class ENGINE_API RenderAPI
	{

	public:
		RenderAPI() = default;
		~RenderAPI();

		void Initialize(HWND hwnd);

	private:

		D12Device mDevice;
	};
}

