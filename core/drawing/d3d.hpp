

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

void undo_hooks( );
HWND game_window;

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* D3D11ResizeBuffersHook) (IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
typedef void(__stdcall* D3D11DrawHook) (ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11DrawIndexedInstancedHook) (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
D3D11PresentHook phookD3D11Present = NULL;
D3D11ResizeBuffersHook phookD3D11ResizeBuffers = NULL;
D3D11PSSetShaderResourcesHook phookD3D11PSSetShaderResources = NULL;
D3D11DrawHook phookD3D11Draw = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11DrawIndexedInstancedHook phookD3D11DrawIndexedInstanced = NULL;
D3D11CreateQueryHook phookD3D11CreateQuery = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pContextVTable = NULL;
DWORD_PTR* pDeviceVTable = NULL;
HWND window = nullptr;
ID3D11RenderTargetView* mainRenderTargetView;
WNDPROC oWndProc;
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		show = !show;
	}
	return CallWindowProcA(original_windowproc, hWnd, uMsg, wParam, lParam);
}
void InitFonts() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();


	ImGui_ImplWin32_Init(game_window);
	ImGui_ImplDX11_Init(device, immediate_context);
}

HRESULT present_hook(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags) {
	if (!device) {
		swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
		device->GetImmediateContext(&immediate_context);
		ID3D11Texture2D* renderTarget = nullptr;
		swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
		device->CreateRenderTargetView(renderTarget, nullptr, &render_target_view);
		renderTarget->Release();
		Renderer::Init(swapChain);
		InitFonts();

	}
	

	immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);
	immediate_context->RSGetViewports(&vps, &viewport);
	screen_size = { viewport.Width, viewport.Height };
	screen_center = { viewport.Width / 2.0f, viewport.Height / 2.0f };
	if (!settings::panic) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (Renderer::new_frame(swapChain)) {
			if (combot::logo)
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				float sWidth = viewport.Width;
				Renderer::rectangle_filled(Vector2{ 4, 4 }, Vector2{ 190, 2 }, Color3(111, 127, 189)); // right
				Renderer::rectangle_filled(Vector2{ 4, 5 }, Vector2{ 190, 18 }, Color3(0, 0, 0, 253)); // right		
				Renderer::t(Vector2(5, 6), Color3(255, 255, 255), 12.f, false, true, wxorstr_(L"NiggerScript | ! ./. | %d:%d:%d"), (int)st.wHour, (int)st.wMinute, (int)st.wSecond);
			}
			if (combot::draw_targeting_fov)
			{
				Renderer::circle(screen_center, Color3(0, 0, 0), combot::fovaim, 1.4f);
				Renderer::circle(screen_center, Color3(255, 140, 0), combot::fovaim, 1.f);
			}
			entities::loop();
			Renderer::end_frame();
		}

		if (show) {

			build_menu();
		}
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return present_original(swapChain, SyncInterval, Flags);
}

HRESULT resize_hook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	Renderer::reset_canvas();

	if (render_target_view) render_target_view->Release();

	if (immediate_context) immediate_context->Release();
	if (device) device->Release();

	device = nullptr;

	return resize_original(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}
HMODULE lpParam;
HMODULE Kernel32Dll;
HINSTANCE hinstDLL;
DWORD __stdcall start(LPVOID) {

	game_assembly = (DWORD64)GetModuleHandleA(("GameAssembly.dll"));
	unity_player = (DWORD64)GetModuleHandleA(("UnityPlayer.dll"));
	

	while (!game_window) {
		EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
			DWORD pid = 0;
			GetWindowThreadProcessId(hWnd, &pid);
			if (pid == GetCurrentProcessId()) {
				*reinterpret_cast<HWND*>(lParam) = hWnd;
				return FALSE;
			}

			return TRUE;
			}, reinterpret_cast<LPARAM>(&game_window));
	}
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	ZeroMemory(&sd, sizeof sd);
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = game_window;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
		MessageBoxW(0, (L"Failed to create D3D11 device and swap chain"), (L"Failure"), MB_ICONERROR);
	}

	auto table = *reinterpret_cast<PVOID**>(swapChain);
	present_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT)>(table[8]);
	resize_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)>(table[13]);

	context->Release();
	device->Release();
	swapChain->Release();

	original_windowproc = (WNDPROC)SetWindowLongPtrW(game_window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)present_original, (PBYTE)present_hook);
	DetourTransactionCommit();
	initialize_cheat();
	do_hooks();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)resize_original, (PBYTE)resize_hook);
	DetourTransactionCommit();
	return NULL;
}

