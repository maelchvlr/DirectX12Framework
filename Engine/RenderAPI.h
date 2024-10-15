#pragma once
#include "EngineMin.h"
#include <Windows.h>

namespace Engine
{
	class ENGINE_API RenderAPI
	{

	public:
		RenderAPI() = default;
		~RenderAPI();

		void Initialize(HWND hwnd);
	};
}

