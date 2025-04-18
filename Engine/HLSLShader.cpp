#include "pch.h"
#include "HLSLShader.h"

namespace Engine
{
	HLSLShader::~HLSLShader()
	{
		Release();
	}

	void HLSLShader::Initialize(const LPCWSTR filename, const ShaderType shadertype)
	{

		UINT flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ALL_RESOURCES_BOUND;

		const char* target = "";

		switch (shadertype)
		{
			case ShaderType::VERTEX:
				target = "vs_5_1";
				break;

			case ShaderType::PIXEL:
				target = "ps_5_1";
				break;

			default:
				PRINT_N("Error, File '" << filename << "' : Shader type not supported.");
				return;
		}


		ID3DBlob* errorMsg = nullptr;

		HRESULT result = D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target, flags, 0, &mByteCode, &errorMsg);

		if (result != S_OK)
		{
			PRINT_N("Shader loading error: " << result);
		}
		if (errorMsg)
		{
			PRINT_N("Shader compilation error: " << (const char*) errorMsg->GetBufferPointer())
		}
		else {
			PRINT_W_N("Shader '"<< filename <<"' compiled successfully.");
		}


	}
	void HLSLShader::Release()
	{
		if (mByteCode)
		{
			mByteCode->Release();
			mByteCode = nullptr;
		}
	}
}
