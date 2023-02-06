//#include <string>
//#include <vector>
//#include <Windows.h>
//template <typename T>
//class VarType
//{
//public:
//	VarType(std::string category_, std::string name_, T* value_)
//	{
//		category = category_;
//		name = name_;
//		value = value_;
//	}
//
//	std::string category, name;
//	T* value;
//};
//namespace config
//{
//	std::string szPath = "C:\\rust.pub\\";
//	std::vector<VarType<int>*> ints;
//	std::vector<VarType<bool>*> bools;
//	std::vector<VarType<float>*> floats;
//
//
//	void Save()
//	{
//		std::string file;
//		file = szPath + settings::ConfigName + ".cfg";
//
//		CreateDirectoryA(szPath.c_str(), NULL);
//
//		for (VarType<int>* pValue : ints)
//			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());
//
//		for (VarType<float>* pValue : floats)
//			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());
//
//		for (VarType<bool>* pValue : bools)
//			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
//	}
//
//	void Load()
//	{
//		std::string file;
//		file = szPath + settings::ConfigName + ".cfg";
//		CreateDirectoryA(szPath.c_str(), NULL);
//
//		char value_l[32] = { '\0' };
//
//		for (VarType<int>* pValue : ints)
//		{
//			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
//			*pValue->value = atoi(value_l);
//		}
//
//		for (VarType<float>* pValue : floats)
//		{
//			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
//			*pValue->value = (float)atof(value_l);
//		}
//
//		for (VarType<bool>* pValue : bools)
//		{
//			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
//			*pValue->value = !strcmp(value_l, "1");
//		}
//	}
//
//	void SetupValue(int& value, int def, std::string category, std::string name)
//	{
//		value = def;
//		ints.push_back(new VarType<int>(category, name, &value));
//	}
//
//	void SetupValue(bool& value, bool def, std::string category, std::string name)
//	{
//		value = def;
//		bools.push_back(new VarType<bool>(category, name, &value));
//	}
//
//	void SetupValue(float& value, float def, std::string category, std::string name)
//	{
//		value = def;
//		floats.push_back(new VarType<float>(category, name, &value));
//	}
//
//	void Setup()
//	{
//		//combat
//		SetupValue(combot::always_heli_weakspot, false, xorstr_("Combat"), xorstr_("always_heli_weakspot"));
//		SetupValue(combot::Perfect_Silent_player, false, xorstr_("Combat"), xorstr_("Perfect_Silent_player"));
//		SetupValue(combot::aimbot, false, xorstr_("Combat"), xorstr_("aimbot"));
//		SetupValue(combot::smooth, false, xorstr_("Combat"), xorstr_("smooth"));
//		SetupValue(combot::Manipulated, false, xorstr_("Combat"), xorstr_("Manipulated"));
//		SetupValue(combot::autoshoot, false, xorstr_("Combat"), xorstr_("autoshoot"));
//		SetupValue(combot::pierce, false, xorstr_("Combat"), xorstr_("pierce"));
//		SetupValue(combot::draw_targeting_fov, false, xorstr_("Combat"), xorstr_("draw_targeting_fov"));
//		SetupValue(combot::fovaim, 100.f, xorstr_("Combat"), xorstr_("fovaim"));
//		//weapon
//
//		SetupValue(combot::recoil_control, 100.f, xorstr_("Weapon"), xorstr_("recoil"));
//		SetupValue(combot::spead_control, 100.f, xorstr_("Weapon"), xorstr_("spead"));
//		SetupValue(combot::no_sway, false, xorstr_("Weapon"), xorstr_("no_sway"));
//		SetupValue(combot::remove_attack_anim, false, xorstr_("Weapon"), xorstr_("remove_attack_anim"));
//		SetupValue(combot::big_bullets, false, xorstr_("Weapon"), xorstr_("big_bullets"));
//		SetupValue(combot::fast_bullets, false, xorstr_("Weapon"), xorstr_("fast_bullets"));
//		SetupValue(combot::insta_eoka, false, xorstr_("Weapon"), xorstr_("insta_eoka"));
//		SetupValue(combot::automatic, false, xorstr_("Weapon"), xorstr_("automatic"));
//
//		//misc
//		SetupValue(combot::fakeadmin, false, xorstr_("Misc"), xorstr_("fakeadmin"));
//		SetupValue(combot::fakeshots, false, xorstr_("Misc"), xorstr_("fakeshots"));
//		SetupValue(combot::OnLadder, false, xorstr_("Misc"), xorstr_("OnLadder"));
//		SetupValue(combot::Noattacksprinting, false, xorstr_("Misc"), xorstr_("Noattacksprinting"));
//		SetupValue(combot::nojumpingrestriction, false, xorstr_("Misc"), xorstr_("nojumpingrestriction"));
//		SetupValue(combot::nosprintingrestriction, false, xorstr_("Misc"), xorstr_("nosprintingrestriction"));
//		SetupValue(combot::Nofalldamage, false, xorstr_("Misc"), xorstr_("Nofalldamage"));
//		SetupValue(combot::running, false, xorstr_("Misc"), xorstr_("Noclip running"));
//		SetupValue(combot::Jesus, false, xorstr_("Misc"), xorstr_("Jesus"));
//		SetupValue(MiscVisual::reload_indicator, false, xorstr_("Misc"), xorstr_("reload_indicator"));
//		SetupValue(MiscVisual::target_info, false, xorstr_("Misc"), xorstr_("target_info"));
//		SetupValue(MiscVisual::activ_slot, false, xorstr_("Misc"), xorstr_("activ_slot"));
//		SetupValue(combot::bullet_tracers, false, xorstr_("Misc"), xorstr_("bullet_tracers"));
//		SetupValue(combot::hitsound, false, xorstr_("Misc"), xorstr_("hitsound"));
//
//		//visuals
//		SetupValue(player::enable, false, xorstr_("Visulal"), xorstr_("enable"));
//		SetupValue(player::player, false, xorstr_("Visulal"), xorstr_("player"));
//		SetupValue(player::Name, false, xorstr_("Visulal"), xorstr_("Name"));
//		SetupValue(player::Weapon, false, xorstr_("Visulal"), xorstr_("Weapon"));
//		SetupValue(player::Wounded, false, xorstr_("Visulal"), xorstr_("Wounded"));
//		SetupValue(player::skeleton, false, xorstr_("Visulal"), xorstr_("skeleton"));
//		SetupValue(player::box, false, xorstr_("Visulal"), xorstr_("box"));
//		SetupValue(player::npc, false, xorstr_("Visulal"), xorstr_("npc"));
//		SetupValue(player::sleepers, false, xorstr_("Visulal"), xorstr_("sleepers"));
//		SetupValue(player::debug_prefab, false, xorstr_("Visulal"), xorstr_("debug_prefab"));
//		SetupValue(MiscVisual::nighmode, false, xorstr_("Visulal"), xorstr_("nighmode"));
//		SetupValue(MiscVisual::radar_, false, xorstr_("Visulal"), xorstr_("radar_"));
//		SetupValue(MiscVisual::patrol_heli, false, xorstr_("Visulal"), xorstr_("patrol_heli"));
//		SetupValue(resource::vehicles, false, xorstr_("Visulal"), xorstr_("vehicles"));
//		SetupValue(resource::corpses, false, xorstr_("Visulal"), xorstr_("corpses"));
//		SetupValue(resource::traps, false, xorstr_("Visulal"), xorstr_("traps"));
//		SetupValue(resource::ores, false, xorstr_("Visulal"), xorstr_("ores"));
//		SetupValue(resource::hemp, false, xorstr_("Visulal"), xorstr_("hemp"));
//		//keys and oher
//		SetupValue(combot::aimkey, 0x4A, xorstr_("Keys"), xorstr_("aimkey"));
//		SetupValue(combot::zoomkey, 0x4A, xorstr_("Keys"), xorstr_("zoomkey"));
//		SetupValue(combot::fakeshotkey, 0x4A, xorstr_("Keys"), xorstr_("fakeshotkey"));
//		SetupValue(combot::OnLadderkey, 0x4A, xorstr_("Keys"), xorstr_("OnLadderkey"));
//
//		SetupValue(MiscVisual::radar::x, 100.f, xorstr_("Radar"), xorstr_("x"));
//		SetupValue(MiscVisual::radar::y, 100.f, xorstr_("Radar"), xorstr_("y"));
//		SetupValue(MiscVisual::radar::range, 121.f, xorstr_("Radar"), xorstr_("range"));
//
//	}
//
//	void Initialize()
//	{
//		szPath = "C:\\rust.pub\\";
//		Setup();
//	}
//
//}