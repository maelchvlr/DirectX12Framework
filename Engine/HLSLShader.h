#pragma once

namespace Engine {

	class HLSLShader
	{
	public: 
		enum ShaderType
		{
			VERTEX,
			PIXEL,
		};

	public:

		HLSLShader() = default;
		~HLSLShader();

		void Initialize(const LPCWSTR filename, const ShaderType shadertype);

		void Release();


	private:

		ID3DBlob* mByteCode = nullptr;


	};
}

