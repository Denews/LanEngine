#pragma once
#define WIN32_LEAN_AND_MEAN
#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <set>
#include <map>
#include <memory>
#include <vector>
#include <utility>

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdlib>
#include <tchar.h>

#include "Context.h"

using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using float32 = float;
using float64 = double;

using tsize = size_t;

using char8 = char;
using char16 = char16_t;
using chat32 = char32_t;
using wchar = wchar_t;

using tsize = size_t;

using tstring = std::string;
using tstringstream = std::stringstream;
using tofstream = std::ofstream;
using tifstream = std::ifstream;
using tfstream = std::fstream;

using vector = DirectX::XMVECTOR;
using fvec2 = DirectX::XMFLOAT2;
using fvec3 = DirectX::XMFLOAT3;
using fvec4 = DirectX::XMFLOAT4;

using ivec2 = DirectX::XMINT2;
using ivec3 = DirectX::XMINT3;
using ivec4 = DirectX::XMINT4;

using matrix = DirectX::XMMATRIX;
using fmat44 = DirectX::XMFLOAT4X4;