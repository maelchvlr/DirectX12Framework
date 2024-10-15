#pragma once




#define ENGINE_EVAL_HR(hr, error) if((hr) != S_OK) { std::cout << "API Error : " << error << " At file : " << __FILE__ << std::endl; }