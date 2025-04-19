#pragma once
#include "EngineMin.h"

#include <Windows.h>

#include "RenderAPI.h"

#include <TimeStep.h>


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
		Timer::TimeStep mTimeStepSystem;

	private: // Variables
		HWND mWindowHandle = nullptr;
		bool bIsRunning = false;


	private:
		UINT mWidth = 1280;
		UINT mHeight = 720;
	};
}

