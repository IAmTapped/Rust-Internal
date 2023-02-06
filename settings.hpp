float indicator_x = 100;
float indicator_y = 100;
namespace settings {
	std::string data_dir = xorstr_("");
	bool cheat_init = false;

	bool panic = false;
	bool realoud = true;
	namespace tr {
		bool manipulated = false;
		bool FakeShotKey = false;
		bool Onlader = false;
		bool Fake_lag = false;
	}

	namespace Settings
	{
		inline bool PressedKeys[256] = {};
		inline BOOL g_bMenu = true;

		inline std::string currentConfig = "Default";
		inline std::vector<std::string> ConfigList = { "Default" };
	}
	char ConfigName[0x100] = "your name config";
	namespace auth {
		std::wstring username = wxorstr_(L"kors");
		std::wstring days_left = wxorstr_(L"Release");
	}
}
bool menu_init = false;
bool init, show = true;
uintptr_t closestHeli = NULL;
__declspec(selectany) uintptr_t game_assembly = NULL;
__declspec(selectany) uintptr_t unity_player = NULL;
namespace stuff {
	int ScreenHeight = 0;
	int ScreenWidth = 0;
	float flyhack = 0.f;
	float hor_flyhack = 0.f;
	float max_flyhack = 0.f;
	float max_hor_flyhack = 0.f;
	char ConfigName[0x100] = "cfg";
}
namespace stor {
	uintptr_t uBase = NULL;
	uintptr_t gBase = NULL;
	uintptr_t closestPlayer = NULL;
	uintptr_t closestHeli = NULL;
	uintptr_t meme_target = NULL;
}
std::string C4 = ("C4");
std::string Satchel = ("Satchel");
std::string IncenRocket = ("Incendiary Rocket");
std::string Rocket = ("Rocket");

std::wstring wC4 = (L"C4");
std::wstring wSatchel = (L"Satchel");
std::wstring wIncenRocket = (L"Incendiary Rocket");
std::wstring wRocket = (L"Rocket");
#include "imgui/imgui.h"
namespace colors {
	float colornameandweapon[] = { 255.f , 0.f ,255.f ,255.f };
	float colorbox[] = { 255.f , 0.f ,255.f ,255.f };
	float fov[] = { 255.f , 0.f ,255.f ,255.f };

	//playrs
	ImVec4 viseble_sleepers = { 255 , 255.f ,255.f ,255.f };
	ImVec4 inviseble_sleepers = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 viseble_teammate = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 inviseble_teammate = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 viseble_players = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 inviseble_players = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 viseble_npcs = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 inviseble_npcs = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 viseble_box = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 inviseble_box = { 255.f , 255.f ,255.f ,255.f };
	ImVec4 target_box = { 255.f , 255.f ,255.f ,255.f };
}
std::wstring time2 = wxorstr_(L"unknown");
namespace combot
{
	bool target = false;
	int targetline_pos = 0;

	bool keybinds = false;
	bool niggerUp = false;
	int niggerupKey = { 0 };

	
	bool logo = true;
	bool fake_lag = false;
	float fake_tick = 0.5f;
	bool Skeet_indicator = false;
	bool anti_aim_indicator = false;
	bool indicator_lua = false;

	bool bfly = false;
	int fly;
	bool bclimb = false;
	inline int FireRate = 100.f;
	inline float FastBulletSpeed = 1.0f;
	int assist;
	bool bindmanipulator = false;
	bool auto_farm_tree = false;
	bool auto_farm_ore = false;
	bool silent_melee = false;
	bool Suicide = false;
	bool AlwaysSuicide = false;
	bool SuicideRate = false;
	bool magic_fire = false;
	bool anti_aim = false;
	int anti_aim_speed = 10;
	int anti_aim_yaw = false;
	//bool fly = false;
	bool remove_attack_anim = false;
	bool hitslog = false;
	bool pierce = false;
	bool autoshoot = false;
	bool Perfect_Silent_player = false;
	bool Perfect_Silent_Heli = false;
	bool Manipulated = false;
	bool always_heli_weakspot = false;
	bool draw_targeting_fov = false;
	int targetbone = 0;
	int Falelag_type = 0;
	float fovaim = 50.f;
	bool aimbot = false;
	int aimkey = { 0 };
	bool smooth = false;
	bool testHiter = false;
	int smooth_factor = 1;
	bool hitsound = false;
	//misc
	bool spoof_hitdistance = false;
	bool spiderman = false;
	
	float hitdistance = 100.f;
	bool AlwaysSprint = false;
	bool running = false;
	float fov = 90.f; 
	int zoomkey = { 0x42 };
	bool bullet_tracers = false;
	bool Jesus = false;
	bool Nofalldamage = false;
	bool Noattacksprinting = false;
	bool godModetest = false;
	bool nojumpingrestriction = false;
	bool nosprintingrestriction = false;

	bool OnLadder = false;
	int OnLadderkey = { 0 };
	int ManipulatedKey = { 0 };

	bool fakeadmin = false;
	bool time_shot = false;
	//MiscWeapon
	float recoil_control = 100.f;
	float spead_control = 100.f;
	bool no_sway = false;
	bool big_bullets = false;
	bool fast_bullets = false;
	bool insta_eoka = false;
	bool automatic = false;
	bool fakeshots = false;
	int fakeshotkey = { 0 };
	bool FLY = { 0 };
}
namespace Hotkeys
{
	//inline int hotkey_MenuKey = VK_INSERT;
//	inline int hotkey_AimKey = VK_CAPITAL;
	inline int hotkey_SpamKey = 0;
	inline int hotkey_FlyhackKey = 0;
	inline int hotkey_SpiderKey = 0;
	inline int hotkey_SuicideKey = 0;
	inline int hotkey_SilentKey = 0;
	//inline int hotkey_AimbotKey = 0;
	//inline int hotkey_lag = VK_CAPITAL;
}
namespace resource
{
	bool ores = false;
	bool stash = false;
	bool vehicles = false;
	bool hemp = false;
	bool corpses = false;
	bool traps = false;
}
namespace warings
{
	bool Viseble = false;
}
namespace player
{
	//bool target_ply = false;
	bool Chams = false;
	bool Hpbar = false;
	bool fill = false;
	bool enable = false;
	bool distance = false;
	bool player = false;
	bool Wounded = false;
	bool Name = false;
	bool Weapon = false;
	bool npc = false;
	bool sleepers = false;
	bool box = false;
	int boxstyle = 0;
	bool skeleton = false;
	bool health_bar = false;
	bool debug_prefab = false;
}
namespace MiscVisual
{
	bool radar_ = false;
	namespace radar {
		float x = 300.f;
		float y = 300.f;
		float range = 160.f;
		float size = 140.f;
	}
	bool rayleigh_changer = false;
	float rayleigh = 1.f;
	bool patrol_heli = false;
	bool RaidEsp = false;
	bool flyhack_indicator = false;
	bool nighmode = false;
	bool target_info = false;
	bool activ_slot = false;
	bool reload_indicator = false;
	bool corsshairs = false;
	int corsshairsstyle = 0;
}
namespace vars
{
	namespace visuals {
		bool radar_ = false;
		namespace radar {
			float x = 300.f;
			float y = 300.f;
			float range = 160.f;
			float size = 120.f;
		
		}
	}


}
bool init2 = false;