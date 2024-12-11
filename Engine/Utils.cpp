#include "pch.h"
#include "Utils.h"


namespace Engine
{
	namespace Utils
	{
		UINT CalculateCOnstantBufferAlignment(const UINT allocation)
		{
			return (allocation + 255) & ~255;
		}
	}
}