#include "pch.h"
#include "Application.h"

#include <windowsx.h>

#include "DXDebug.h"

namespace Engine
{
	LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case  WM_NCCREATE: {
				LPCREATESTRUCT param = reinterpret_cast<LPCREATESTRUCT>(lparam);
				Application* pointer = reinterpret_cast<Application*>(param->lpCreateParams);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pointer));

				PRINT_N("Oh boy, here we go.");
				PRINT_N("Sent create message.");

				break;
			}

			case WM_CREATE: {
				Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				pointer->OnCreate(hwnd);

				break;
			}

			case WM_DESTROY: {
				Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				pointer->OnDestroy();

				PostQuitMessage(0);
				break;
			}

		}


		return DefWindowProc(hwnd, msg, wparam, lparam);
	}


	bool Application::Initialize()
	{
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = L"BaseWindowClass";
		wndClass.style = 0;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wndClass.lpszMenuName = 0;
		wndClass.hInstance = GetModuleHandle(NULL); 
		wndClass.lpfnWndProc = WindProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;

		RegisterClass(&wndClass);

		mWindowHandle = CreateWindow(L"BaseWindowClass", L"DirectX12Engine", WS_OVERLAPPEDWINDOW, 200, 200, mWidth, mHeight, 0, 0, wndClass.hInstance, this);


		if (!mWindowHandle)
		{
			return false;
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		bIsRunning = true;

		return bIsRunning;
	}

	void Application::OnCreate(HWND hwnd)
	{

		PRINT_N("Window created.");
		mRenderer.Initialize(hwnd, mWidth, mHeight);
	}

	void Application::Update()
	{
		MSG message;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (bIsRunning)
		{
			mRenderer.UpdateDraw();
		}
	}

	void Application::OnDestroy()
	{
		bIsRunning = false;
		PRINT_N("Going dark.");


		mRenderer.Release();
		DXDebug::Get().GetLiveObjects();



	}
}

