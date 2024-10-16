#pragma once
#include <wrl.h>


namespace Engine {
	class DXDebug : public Microsoft::WRL::ComPtr<IDXGIDebug> 
	{
	public:
		DXDebug() = default;


		void Enable();

		void GetLiveObjects();


	private:

		void GetInterface();

	public:
		static inline DXDebug& Get() { return mInstance; };


	private:
		static DXDebug mInstance;
	};



}

