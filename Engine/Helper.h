#pragma once
#include <exception>

#define KBs(x) (x) * 1024



#define ENGINE_EVAL_HR(hr, error) if((hr) != S_OK) { std::cout << "API Error : " << error << " At file : " << __FILE__ << std::endl; }
#define PRINT_W_N(message) std::wcout << message << std::endl;
#define PRINT_N(message) std::cout << message << std::endl;

#ifdef _DEBUG
#define ENGINE_ASSERT(exp) if (!(exp)) { std::cout << "Critical error in file: " << __FILE__ << " at line : " << __LINE__ << std::endl; throw std::exception("Critical Error"); }

#else
#define ENGINE_ASSERT(exp)
#endif
