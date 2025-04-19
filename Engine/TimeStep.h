#pragma once
#include <chrono>

namespace Engine {

	namespace Timer {

		class ENGINE_API TimeStep
		{
		public:

			TimeStep();

			float Tick();


		private:

			std::chrono::steady_clock::time_point mLastTime;
		};
	}

}