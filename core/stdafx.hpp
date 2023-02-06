Vector2 screen_size = { 0, 0 };
Vector2 screen_center = { 0, 0 };
D3D11_VIEWPORT viewport;

HRESULT(*present_original)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resize_original)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

typedef HRESULT(__fastcall* DX11PresentFn) (IDXGISwapChain* pSwapChain, UINT syncintreval, UINT flags);
DX11PresentFn oPresent = nullptr;
ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediate_context = nullptr;
ID3D11RenderTargetView* render_target_view = nullptr;

WNDPROC original_windowproc = nullptr;
UINT vps = 1;

template<typename T1, typename T2>
bool map_contains_key(T1 map, T2 key) {
	return map.count(key) > 0;
}
float get_lowest(std::vector<float> input) {
	float ret = FLT_MAX;

	for (float idx : input) {
		if (idx < ret)
			ret = idx;
	}

	return ret;
}
#define FOUNDW(str,str2) str.find(str2) != std::wstring::npos
#define FOUND(str,str2) str.find(str2) != std::string::npos
std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length( ), replace);
		pos += replace.length( );
	}
	return subject;
}
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
std::string DownloadString(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str( ), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[ 2000 ];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}
#pragma comment(lib, "ntdll.lib")

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
										   PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);
void BlueScreen( ) {
	BOOLEAN bl;
	ULONG Response;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
}
std::string GetActiveWindowTitle( ) {
	char wnd_title[ 256 ];
	HWND hwnd = GetForegroundWindow( );
	GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
	return wnd_title;
}
bool get_key(int key) {
	return GetAsyncKeyState(key) && GetActiveWindowTitle( ).find(xorstr_("Rust")) != std::string::npos;
}
std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length( );
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

enum Bone_List : int {
    // assets / prefabs / player / player_mod = 0,
    pelvis = 1,
    l_hip = 2,
    l_knee = 3,
    l_foot = 4,
    l_toe = 5,
    l_ankle_scale = 6,
    penis = 7,
    GenitalCensor = 8,
    GenitalCensor_LOD0 = 9,
    Inner_LOD0 = 10,
    GenitalCensor_LOD1 = 11,
    GenitalCensor_LOD2 = 12,
    r_hip = 13,
    r_knee = 14,
    r_foot = 15,
    r_toe = 16,
    r_ankle_scale = 17,
    spine1 = 18,
    spine1_scale = 19,
    spine2 = 20,
    spine3 = 21,
    spine4 = 22,
    l_clavicle = 23,
    l_upperarm = 24,
    l_forearm = 25,
    l_hand = 26,
    l_index1 = 27,
    l_index2 = 28,
    l_index3 = 29,
    l_little1 = 30,
    l_little2 = 31,
    l_little3 = 32,
    l_middle1 = 33,
    l_middle2 = 34,
    l_middle3 = 35,
    l_prop = 36,
    l_ring1 = 37,
    l_ring2 = 38,
    l_ring3 = 39,
    l_thumb1 = 40,
    l_thumb2 = 41,
    l_thumb3 = 42,
    IKtarget_righthand_min = 43,
    IKtarget_righthand_max = 44,
    l_ulna = 45,
    neck = 46,
    head = 47,
    jaw = 48,
    eyeTranform = 49,
    l_eye = 50,
    l_Eyelid = 51,
    r_eye = 52,
    r_Eyelid = 53,
    r_clavicle = 54,
    r_upperarm = 55,
    r_forearm = 56,
    r_hand = 57,
    r_index1 = 58,
    r_index2 = 59,
    r_index3 = 60,
    r_little1 = 61,
    r_little2 = 62,
    r_little3 = 63,
    r_middle1 = 64,
    r_middle2 = 65,
    r_middle3 = 66,
    r_prop = 67,
    r_ring1 = 68,
    r_ring2 = 69,
    r_ring3 = 70,
    r_thumb1 = 71,
    r_thumb2 = 72,
    r_thumb3 = 73,
    IKtarget_lefthand_min = 74,
    IKtarget_lefthand_max = 75,
    r_ulna = 76,
    l_breast = 77,
    r_breast = 78,
    BoobCensor = 79,
    BreastCensor_LOD0 = 80,
    BreastCensor_LOD1 = 81,
    BreastCensor_LOD2 = 82,
    collision = 83,
    displacement = 84
};
bool play_sound(const char* name) {
    return PlaySoundA(name, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
}