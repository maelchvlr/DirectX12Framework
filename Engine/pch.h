
#ifndef PCH_H
#define PCH_H

// Windows headers:
#include <iostream>
#include <vector>
#include <string>

//DirectX headers:
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

//compiled lib files for D12

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3Dcompiler.lib")

// General headers:
#include "EngineMin.h"
#include "Helper.h"
#include "EngineSettings.h"

#endif //PCH_H
