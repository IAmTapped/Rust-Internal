#define NOMINMAX
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#define THEM
#define authh

#include <Windows.h>
#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <d3d11.h>
#include <string>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>
#include <emmintrin.h>
#include <comdef.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <lmcons.h>
#include <thread>
#include <map>
#include <shlobj.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Winmm.lib")
#pragma warning ( disable : 4172 )

#include "core/sdk/utils/string.hpp"
#include "core/sdk/utils/xorstr.hpp"
#include "core/sdk/utils/xorf.hpp"
#include "utils/WinReg.hpp"
#include "utils/Fingerprint.hpp"

#include "core/sdk/vector.hpp"
#include "settings.hpp"
#include "core/stdafx.hpp"
#include "core/drawing/renderer.hpp"

#define FGUI_IMPLEMENTATION
#define FGUI_USE_D2D
#include "imgui/imgui.h"

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_elements.h"
#include "core/drawing/menu.h"
#include "core/sdk/utils/hookengine.hpp"
#include "core/sdk/mem.hpp"
#include "core/sdk/utils/crc32.hpp"
#include "core/sdk/il2cpp/wrapper.hpp"
#include "core/sdk/il2cpp/dissector.hpp"
#include "core/sdk/structs.hpp"
#include "core/sdk/game.hpp"
#include "core/main/cache.hpp"
#include "core/main/log.h"
#include "core/main/other.hpp"

#include "core/sdk/utils/math.hpp"
#include "core/main/aimutils.hpp"
#include "core/main/entities.hpp"
#include "core/main/hooks.hpp"
#include "core/drawing/d3d.hpp"
#include "detours/detours.h"
#pragma comment(lib, "detours/detours.lib")
#pragma warning( disable : 4244 )
#include <future>
#include "Initilization.h"
void injection(HINSTANCE hModule) {

}


BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: // A process is loading the DLL.
		//AllocConsole();
		//std::cout << "Successfully injected into rust | issues -> none\n" << std::endl;
		DisableThreadLibraryCalls(hModule);
		//njection(hModule);
		CreateThread(NULL, 0, start, NULL, 0, NULL);
		break;

	case DLL_PROCESS_DETACH: // A process unloads the DLL.
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(LPVOID&)present_original, (PBYTE)present_hook);
		DetourTransactionCommit();
		break;
	}
	return TRUE;
}
