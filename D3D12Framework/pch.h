#pragma once
#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#undef min
#undef max

// STL Essentials
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <span>
#include <algorithm>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <utility>
#include <filesystem>
#include <typeindex>


// Direct3D related headers
#include <wrl.h>
#include <shellapi.h>	

// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgidebug.h>
#include "../d3dx/d3dx12.h"


// D3DCompiler
#include <d3dcompiler.h>

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// DirectXTex
#include "DirectXTex/DDSTextureLoader12.h"
#include "DirectXTex/WICTextureLoader12.h"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace Microsoft::WRL;

// Import libraries
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// Additional Helper Headers
#include "Defines.h"
#include "Concepts.h"
#include "Utility.h"
#include "ShaderResource.h"

// Game Headers
#include "WinCore.h"
#include "D3DCore.h"
#include "GameFramework.h"