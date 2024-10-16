#pragma once
#include "EngineMin.h"
#include <Windows.h>

#include "D12Device.h"
#include "D12CommandQueue.h"
#include "D12CommandList.h"

namespace Engine
{
	class ENGINE_API RenderAPI
	{

	public:
		RenderAPI() = default;
		~RenderAPI();

		void Initialize(HWND hwnd);

		void Release();

	private:

		D12Device mDevice;
		D12CommandQueue	mCommandQueue;
		D12CommandList mCommandList;

	};
}

