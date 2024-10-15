#pragma once
#include "EngineMin.h"

#include <Windows.h>

#include "RenderAPI.h"


namespace Engine{
	class ENGINE_API Application
	{

	public:
		Application() = default;

		bool Initialize();

		void OnCreate(HWND hwnd);

		void Update();

		void OnDestroy();

		inline bool IsRunning() { return bIsRunning; }

	private: // Subsystems
		RenderAPI mRenderer;

	private: // Variables
		HWND mWindowHandle = nullptr;
		bool bIsRunning = false;
	};
}

