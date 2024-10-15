#include "pch.h"
#include "RenderAPI.h"

#include "DXGIFactory.h"

namespace Engine
{
	RenderAPI::~RenderAPI()
	{
	}
	void RenderAPI::Initialize(HWND hwnd)
	{

		DXGIFactory factory;
		/*
		ACCESS THE HARDWARE -> CREATE A VIRTUAL DEVICE (basically the context for D12 API)

		COM SYSTEM 

		*/


	}
}