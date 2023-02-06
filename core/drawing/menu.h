#include <vector>
#include <algorithm>
bool applied = false;
#include "../../imgui/imgui_internal.h"
#include "core/main/config.h"
#include <core/drawing/Foundation Icons.h>
// Fonts
ImFont* TabsFont;
ImFont* SubTabsFont;
ImFont* NameFont;
ImFont* MenuFont;
ImFont* IconFont;
static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2 operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
namespace menu {
	struct TabInfo {
		ImRect bb;
		unsigned int index;
	};
	ImVec2 cursor_pos = { 0,0 };
	ImFont* title_font = nullptr, * tabs_font = nullptr, * menu_font = nullptr;
	static int tab_index = 0;
	static float border_bang = 1.0f;
	static int old_tab_index = 0;
	std::vector<TabInfo> tabs_info;

	static const char* targetbone[]{
	"None",
	"body",
	"head",
	"randomize (all)",
	"randomize (main)",
	};
	static const char* crosshairs[]{
	"None",
	"Simple",
	"Evilcheats",
	"Cirle",
	};
	static const char* fakelag_type[]{
	"None",
	"Custom",
	"Legit",
	"randomizer",
	};
	static const char* box_style[]{
		"Cornered",
		"Normal",
		"3D",
	};
	bool Hotkey(const char* label, int* k, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
		const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), window->DC.CursorPos + size);
		const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
			return false;

		const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);
		const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
		const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

		const bool hovered = ImGui::ItemHoverable(frame_bb, id);

		if (hovered) {
			ImGui::SetHoveredID(id);
			g.MouseCursor = ImGuiMouseCursor_TextInput;
		}

		const bool user_clicked = hovered && io.MouseClicked[0];

		if (focus_requested || user_clicked) {
			if (g.ActiveId != id) {
				// Start edition
				memset(io.MouseDown, 0, sizeof(io.MouseDown));
				memset(io.KeysDown, 0, sizeof(io.KeysDown));
				*k = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {
			// Release focus when we click outside
			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		bool value_changed = false;
		int key = *k;

		if (g.ActiveId == id) {
			for (auto i = 0; i <= 6; i++) {
				if (io.MouseDown[i] || GetAsyncKeyState(VK_XBUTTON1) || GetAsyncKeyState(VK_XBUTTON2)) {
					switch (i) {
					case 0:
						key = VK_LBUTTON;
						break;
					case 1:
						key = VK_RBUTTON;
						break;
					case 2:
						key = VK_MBUTTON;
						break;
					}
					if (GetAsyncKeyState(VK_XBUTTON2))
						key = VK_XBUTTON2;
					if (GetAsyncKeyState(VK_XBUTTON1))
						key = VK_XBUTTON1;

					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
			if (!value_changed) {
				for (auto i = VK_BACK; i <= VK_RMENU; i++) {
					if (io.KeysDown[i]) {
						key = i;
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}
			}

			if (IsKeyPressedMap(ImGuiKey_Escape, false)) {
				*k = 0;
				ImGui::ClearActiveID();
			}
			else {
				*k = key;
			}
		}

		// Render
		// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

		const char* buf_display = "none";

		ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 0)), true, style.FrameRounding);

		if (*k != 0 && g.ActiveId != id) {
			const char* key_name;
			get_keycode_name(*k, buf_display);
		}
		else if (g.ActiveId == id) {
			buf_display = "Press any key...";
		}

		const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
		ImVec2 render_pos = frame_bb.Min + style.FramePadding;
		ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
		//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
		//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

		return value_changed;
	}

}
bool SubTabEx(const char* label, const bool active, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x, label_size.y + style.FramePadding.y);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemEdited(id);

	static int alpha = 0;
	if (active)
	{
		alpha += 15;
		if (alpha > 255)
			alpha = 255;
	}
	if (pressed)
		alpha = 0;
	ImGui::RenderNavHighlight(bb, id);
	if (active)
	{
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 3), bb.Max, ImColor(0, 70, 255, alpha), 15.f);
	}
	if (active || hovered)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 255, 255));
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x, bb.Max.y - 3), bb.Max, ImColor(0, 70, 255, alpha), 15.f);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(100, 100, 100));
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}


	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
	return pressed;
}
bool SubTab(const char* label, const bool active, const ImVec2& size_arg)
{
	return SubTabEx(label, active, size_arg, 0);
}
std::string comp_name() {

	char buff[MAX_PATH];
	GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);

	return std::string(buff);
}

void AimbotTab()
{
	static int aim_tabs = 0;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 349, ImGui::GetWindowHeight() - 400));
	if (SubTab("General##aim", aim_tabs == 0, ImVec2(100, 30)))
		aim_tabs = 0;
	ImGui::SameLine();
	if (SubTab("Weapons##aim", aim_tabs == 1, ImVec2(100, 30)))
		aim_tabs = 1;
	if (aim_tabs == 0)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("Aim Assist"), &combot::aimbot);
			ImGui::Checkbox(xorstr_("Smooth"), &combot::smooth);
			if (combot::smooth) {
				ImGui::SliderInt(xorstr_("Smoothing"), &combot::smooth_factor, 1, 50);
			}
			menu::Hotkey(xorstr_("Aim Assist Key"), &combot::aimkey, ImVec2(200.f, 0));

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##two", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("Pilent"), &combot::Perfect_Silent_player);
			ImGui::Combo(xorstr_("Target"), &combot::targetbone, menu::targetbone, IM_ARRAYSIZE(menu::targetbone));
			ImGui::Checkbox(xorstr_("Manipulated"), &combot::Manipulated);
			ImGui::Checkbox(xorstr_("Always Heli Weakspot"), &combot::always_heli_weakspot);
			ImGui::Checkbox(xorstr_("Auto shoot"), &combot::autoshoot);
			ImGui::Checkbox(xorstr_("Pierce"), &combot::pierce);
			ImGui::Checkbox(xorstr_("Draw fov"), &combot::draw_targeting_fov);
			ImGui::SliderFloat(xorstr_("Fov"), &combot::fovaim, 10.00f, 2500.f);
		}
		ImGui::EndChild();
	}
	if (aim_tabs == 1)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::SliderFloat(xorstr_("Recoil"), &combot::recoil_control, 0.00f, 100.f);
			ImGui::SliderFloat(xorstr_("Spread"), &combot::spead_control, 0.00f, 100.f);
			ImGui::Checkbox(xorstr_("No Sway"), &combot::no_sway);
			ImGui::Checkbox(xorstr_("Big Bullets"), &combot::big_bullets);
			ImGui::Checkbox(xorstr_("Fast Bullets"), &combot::fast_bullets);
			ImGui::Checkbox(xorstr_("Insta Eoka"), &combot::insta_eoka);
			ImGui::Checkbox(xorstr_("Automatic"), &combot::automatic);
			ImGui::Checkbox(xorstr_("Remove attack animation"), &combot::remove_attack_anim);

			//	ImGui::Checkbox(xorstr_("Magic Fire"), &combot::magic_fire);
		}
		ImGui::EndChild();
	}
}
static const char* target_tracer_pos[]{
	"None",
	"top",
	"Down",
	"Crosshair",
};
void ESPTab()
{
	static int esp_tabs = 0;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 349, ImGui::GetWindowHeight() - 400));
	if (SubTab("General##Esp", esp_tabs == 0, ImVec2(100, 30)))
		esp_tabs = 0;
	ImGui::SameLine();
	if (SubTab("World##Esp", esp_tabs == 1, ImVec2(100, 30)))
		esp_tabs = 1;
	if (SubTab("World##Esp", esp_tabs == 2, ImVec2(100, 30)))
		esp_tabs = 2;
	if (esp_tabs == 0)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("Enable Visuals"), &player::enable);
			if (player::enable)
			{
				ImGui::Checkbox(xorstr_("Player"), &player::player);
				ImGui::Checkbox(xorstr_("Npc"), &player::npc);
				ImGui::Checkbox(xorstr_("Sleepers"), &player::sleepers);
			}

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##two", ImVec2(230, 340), true);
		{
			if (player::player)
			{
				ImGui::Checkbox(xorstr_("Name"), &player::Name);
				ImGui::Checkbox(xorstr_("Weapon"), &player::Weapon);
				ImGui::Checkbox(xorstr_("distance"), &player::distance);
				ImGui::Checkbox(xorstr_("Wounded"), &player::Wounded);
				ImGui::Checkbox(xorstr_("Skeleton"), &player::skeleton);
				ImGui::Checkbox(xorstr_("Fill Box"), &player::fill);
				ImGui::Checkbox(xorstr_("Box"), &player::box);
				ImGui::Checkbox(xorstr_("Helth Bar"), &player::Hpbar);
				if (player::box) {
					ImGui::PushItemWidth(100);
					ImGui::Combo(xorstr_("Box Style"), &player::boxstyle, menu::box_style, IM_ARRAYSIZE(menu::box_style));
					ImGui::PopItemWidth();
				}
				ImGui::Checkbox(xorstr_("target"), &combot::target);
				if (combot::target) {
					ImGui::PushItemWidth(100);
					ImGui::Combo(xorstr_("targetline_pos"), &combot::targetline_pos, target_tracer_pos, IM_ARRAYSIZE(target_tracer_pos));
					ImGui::PopItemWidth();
				}
			}
		}
		ImGui::EndChild();
	}
	if (esp_tabs == 1)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("NightMode"), &MiscVisual::nighmode);

			ImGui::Checkbox(xorstr_("Debug Prefab"), &player::debug_prefab);
			ImGui::Checkbox(xorstr_("Patrol Heli"), &MiscVisual::patrol_heli);
			ImGui::Checkbox(xorstr_("Vehicles"), &resource::vehicles);
			ImGui::Checkbox(xorstr_("Corpses"), &resource::corpses);
			ImGui::Checkbox(xorstr_("Traps"), &resource::traps);
			ImGui::Checkbox(xorstr_("Ores"), &resource::ores);
			ImGui::Checkbox(xorstr_("Hemp"), &resource::hemp);
		}
		ImGui::EndChild();
	}
	if (esp_tabs == 2)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("Viseble"), &warings::Viseble);
		}
		ImGui::EndChild();
	}
}
void OtherTab()
{
	static int misc_tabs = 0;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 349, ImGui::GetWindowHeight() - 400));

	if (SubTab("Other##Misc", misc_tabs == 0, ImVec2(100, 30)))
		misc_tabs = 0;
	ImGui::SameLine();
	//if (SubTab("Config##Misc", misc_tabs == 1, ImVec2(100, 30)))
	misc_tabs = 1;
	if (misc_tabs == 0)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{

			ImGui::SliderFloat(xorstr_("Fov"), &combot::fov, 30.00f, 130.f);
			menu::Hotkey(xorstr_("Zoom"), &combot::zoomkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("Fake admin"), &combot::fakeadmin);
			ImGui::Checkbox(xorstr_("Fakeshots"), &combot::fakeshots);
			menu::Hotkey(xorstr_("Fake shots"), &combot::fakeshotkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("On Ladder"), &combot::OnLadder);
			menu::Hotkey(xorstr_("On Ladder "), &combot::OnLadderkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("No attack sprinting"), &combot::Noattacksprinting);
			ImGui::Checkbox(xorstr_("Better Jump"), &combot::nojumpingrestriction);
			ImGui::Checkbox(xorstr_("No sprint ingrestriction"), &combot::nosprintingrestriction);
			ImGui::Checkbox(xorstr_("No fall damage"), &combot::Nofalldamage);

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##two", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("Fake lag"), &combot::fake_lag);
			if (combot::fake_lag) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("Fake lag tick"), &combot::Falelag_type, menu::fakelag_type, IM_ARRAYSIZE(menu::fakelag_type));
				ImGui::PopItemWidth();
				if (combot::Falelag_type == 1)
				{
					ImGui::SliderFloat(xorstr_("Fake tick"), &combot::fake_tick, 0.005f, 1.f);
				}
			}
			ImGui::Checkbox(xorstr_("Noclip Walking"), &combot::running);
			ImGui::Checkbox(xorstr_("Jesus"), &combot::Jesus);
			ImGui::Checkbox(xorstr_("Reload Indicator"), &MiscVisual::reload_indicator);
			ImGui::Checkbox(xorstr_("Target Info"), &MiscVisual::target_info);
			ImGui::Checkbox(xorstr_("Activ Slot"), &MiscVisual::activ_slot);
			ImGui::Checkbox(xorstr_("Crosshairs"), &MiscVisual::corsshairs);
			if (MiscVisual::corsshairs) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("crosshairs Style"), &MiscVisual::corsshairsstyle, menu::crosshairs, IM_ARRAYSIZE(menu::crosshairs));
				ImGui::PopItemWidth();
			}
			ImGui::Checkbox(xorstr_("Logo"), &combot::logo);
			ImGui::Checkbox(xorstr_("Skeet Indicator"), &combot::Skeet_indicator);
			ImGui::Checkbox(xorstr_("Simple Indicator"), &combot::indicator_lua);
			ImGui::Checkbox(xorstr_("keybinds"), &combot::keybinds);
		}
		ImGui::EndChild();
	}
	if (misc_tabs == 1)
	{
		ImGui::BeginChild("##one", ImVec2(230, 340), true);
		{
			ImGui::Checkbox(xorstr_("panic"), &settings::panic);
		}
		ImGui::EndChild();
	}
}

bool TabEx(const char* label, const bool active, const ImVec2& size_arg, ImGuiButtonFlags flags, int index)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	//const ImVec2 icon_size = ImGui::CalcTextSize(icon, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x, label_size.y + style.FramePadding.y);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemEdited(id);

	static int alpha = 0;
	if (active)
	{
		alpha += 15;
		if (alpha > 255)
			alpha = 255;
	}
	if (pressed)
		alpha = 0;
	/*ImVec2 pos_icon;
	switch (index)
	{
	case 0: pos_icon = ImVec2(70, 0); break;
	case 1: pos_icon = ImVec2(60, 0); break;
	case 2: pos_icon = ImVec2(65, 0); break;
	}
	ImGui::RenderNavHighlight(bb, id);*/


	if (active)
	{
		ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, ImVec2(bb.Min.x + 4, bb.Max.y), ImColor(0, 70, 255, alpha), 15.f);
	}
	if (active || hovered)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 255, 255));
		ImGui::RenderTextClipped(bb.Min + style.FramePadding + ImVec2(20, 0), bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor(1);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(100, 100, 100));
		ImGui::RenderTextClipped(bb.Min + style.FramePadding + ImVec2(20, 0), bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor(1);
	}

	return pressed;
}
bool Tab(const char* label, const bool active, const ImVec2& size_arg, int index)
{
	return TabEx(label, active, size_arg, 0, index);
}
ImVec4 rgb_to_imvec4(float r, float g, float b, float a)
{
	return ImVec4(r / 255, g / 255, b / 255, a / 255);
}
ImFont* Elements;


void Skyline_sky() {
	// Style
	auto& style = ImGui::GetStyle();
	style.FrameRounding = 1.f;
	style.PopupRounding = 10.f;
	style.FramePadding = ImVec2(2, 1);
	style.WindowRounding = 15.f;
	// Colors
	auto colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
	colors[ImGuiCol_FrameBgHovered] = ImColor(0, 70, 255, 200);
	colors[ImGuiCol_FrameBgActive] = ImColor(0, 70, 255);
	colors[ImGuiCol_Separator] = ImColor(0, 70, 255);
	colors[ImGuiCol_SliderGrab] = ImColor(0, 70, 255, 100);
	colors[ImGuiCol_SliderGrabActive] = ImColor(0, 70, 255);
	colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20);
	colors[ImGuiCol_ScrollbarBg] = ImColor(35, 35, 35);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(15, 15, 15);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(0, 70, 255, 100);
	colors[ImGuiCol_ScrollbarGrabActive] = ImColor(0, 70, 255);
	colors[ImGuiCol_Border] = ImColor();
	colors[ImGuiCol_ChildBg] = ImColor(35, 35, 35);
	colors[ImGuiCol_Header] = ImColor(15, 15, 15);
	colors[ImGuiCol_HeaderHovered] = ImColor(0, 70, 255, 200);
	colors[ImGuiCol_HeaderActive] = ImColor(0, 70, 255);
	colors[ImGuiCol_Button] = ImColor(20, 20, 20);
	colors[ImGuiCol_ButtonHovered] = ImColor(0, 70, 255, 200);
	colors[ImGuiCol_ButtonActive] = ImColor(0, 70, 255);
	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	ImGui::SetNextWindowSize(ImVec2(650, 400));
	ImGui::Begin("clux", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
	{
		char name[UNLEN + 1];
		DWORD username_len = UNLEN + 1;
		GetUserNameA(name, &username_len);
		std::string winname = name;
		const char* label = name;
		const char* label2 = xorstr_("clux");
		const char* footer = xorstr_("clux");
		auto labelsize = ImGui::CalcTextSize(label);
		auto label_iconsize = ImGui::CalcTextSize(label);
		auto footersize = ImGui::CalcTextSize(footer);
		static int tabs = 0;
		auto p = ImGui::GetWindowPos();
		auto d = ImGui::GetWindowDrawList();
		d->AddRectFilled(p, ImVec2(p.x + 650, p.y + 400), ImColor(30, 30, 30), 12.f);
		ImGui::BeginGroup();
		{
			d->AddText(NameFont, 22.f, ImVec2(p.x + (160 - labelsize.x - 37) * 0.5, p.y + (70 - labelsize.y) * 0.5), ImColor(255, 255, 255), label2);
			d->AddText(NameFont, 16.f, ImVec2(p.x + (160 - labelsize.x - 104) * 0.5, p.y + 363), ImColor(255, 255, 255), xorstr_("Username: "));
			d->AddText(NameFont, 16.f, ImVec2(p.x + (160 - labelsize.x + 32) * 0.5, p.y + 363), ImColor(255, 255, 255), label);
			//	d->AddText(IconFont, 24.f, ImVec2(p.x + (160 - label_iconsize.x - 10) * 0.5, p.y + (70 - label_iconsize.y) * 0.5), ImColor(0, 70, 255), label_icon);
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 646, ImGui::GetWindowHeight() - 320));
			ImGui::BeginGroup();
			{
				if (Tab("Combat", tabs == 0, ImVec2(160, 30), 0))
					tabs = 0;
				if (Tab("Visuals", tabs == 1, ImVec2(160, 30), 1))
					tabs = 1;
				if (Tab("Misc", tabs == 2, ImVec2(160, 30), 2))
					tabs = 2;
			}
			ImGui::EndGroup();
		}
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		{
			switch (tabs) {
			case 0: AimbotTab(); break;
			case 1: ESPTab(); break;
			case 2: OtherTab(); break;
			}
		}
		ImGui::EndGroup();
		d->AddRectFilled(ImVec2(p.x, p.y + 385), ImVec2(p.x + 650, p.y + 400), ImColor(0, 70, 255), 15.f);
		d->AddText(MenuFont, 16.f, ImVec2(p.x + (650 - footersize.x) * 0.5 + style.ItemInnerSpacing.x * 4, p.y + 383), ImColor(255, 255, 255), footer);
	} ImGui::End();
}
auto tab = [&](const char* name, int index, int& selected_tab, ImVec2 size) {
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.65f, 0.9f));
	if (selected_tab == index)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 0.95f));
	if (ImGui::Button(name, size)) {
		if (selected_tab == index)
			ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		selected_tab = index;
		return;
	}
	if (selected_tab == index)
		ImGui::PopStyleColor();
	ImGui::PopStyleColor();
};
void build_menu()
{
	auto Combo = [](const char* name, const char** items, int itemcnt, int& index) {
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, ImGui::GetStyle().ChildRounding);

		if (ImGui::BeginCombo(name, items[index], 0)) {
			bool value_changed = false;
			for (int i = 0; i < itemcnt; i++) {
				ImGui::PushID((void*)(intptr_t)i);
				const char* item_text = items[i];
				if (ImGui::Selectable(item_text, i == index)) {
					index = true;
					index = i;
				}
				if (i == index)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	};

	auto aimbot = [&]() {
		ImGui::Spacing();

		ImGui::BeginChild(xorstr_("AimbotChild"), ImVec2(-1.f, -1.f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::Checkbox(xorstr_("PSilent"), &combot::Perfect_Silent_player);
			ImGui::Combo(xorstr_("Target"), &combot::targetbone, menu::targetbone, IM_ARRAYSIZE(menu::targetbone));
			ImGui::Checkbox(xorstr_("MagicFire"), &combot::magic_fire);
			ImGui::Checkbox(xorstr_("Instant Hit Player"), &combot::testHiter);
			ImGui::Checkbox(xorstr_("godmode"), &combot::godModetest);
			ImGui::Checkbox(xorstr_("niggerHigh"), &combot::niggerUp);
			menu::Hotkey(xorstr_("nigger Key"), &combot::niggerupKey, ImVec2(200.f, 0));

		//	ImGui::Checkbox(xorstr_("Fly"), &combot::FLY);
			ImGui::Checkbox(xorstr_("Manipulated"), &combot::Manipulated);
			ImGui::Checkbox(xorstr_("Silent Melee"), &combot::silent_melee);
			ImGui::Checkbox(xorstr_("flyhack_indicator"), &combot::bfly);
			ImGui::Checkbox(xorstr_("Hitsound"), &combot::hitsound);
			ImGui::Checkbox(xorstr_("Always Heli Weakspot"), &combot::always_heli_weakspot);
			ImGui::Checkbox(xorstr_("Auto shoot"), &combot::autoshoot);
			ImGui::Checkbox(xorstr_("bullet tracers"), &combot::bullet_tracers);
			ImGui::Checkbox(xorstr_("Pierce"), &combot::pierce);
			ImGui::Checkbox(xorstr_("Draw fov"), &combot::draw_targeting_fov);
			ImGui::SliderFloat(xorstr_("Fov"), &combot::fovaim, 10.00f, 2500.f);
			ImGui::EndChild();
		}
	};

	auto visuals = [&]() {


		ImGui::Spacing();

		ImGui::BeginChild(xorstr_("VisualsChild"), ImVec2(250.f, -1.f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::Checkbox(xorstr_("Enable Visuals"), &player::enable);
			ImGui::Checkbox(xorstr_("Player"), &player::player);
			ImGui::Checkbox(xorstr_("Npc"), &player::npc);
			ImGui::Checkbox(xorstr_("Sleepers"), &player::sleepers);

			ImGui::Separator();
			ImGui::Checkbox(xorstr_("NightMode"), &MiscVisual::nighmode);
			ImGui::Checkbox(xorstr_("Debug Prefab"), &player::debug_prefab);
			ImGui::Checkbox(xorstr_("Patrol Heli"), &MiscVisual::patrol_heli);
			ImGui::Checkbox(xorstr_("Vehicles"), &resource::vehicles);
			ImGui::Checkbox(xorstr_("Corpses"), &resource::corpses);
			ImGui::Checkbox(xorstr_("Traps"), &resource::traps);
			ImGui::Checkbox(xorstr_("Ores"), &resource::ores);
			ImGui::Checkbox(xorstr_("Hemp"), &resource::hemp);
			ImGui::EndChild();
		}
		ImGui::SameLine();

		ImGui::BeginChild(xorstr_("ESPChild"), ImVec2(250.f, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::Checkbox(xorstr_("Name"), &player::Name);
			ImGui::Checkbox(xorstr_("Weapon"), &player::Weapon);
			ImGui::Checkbox(xorstr_("distance"), &player::distance);
			ImGui::Checkbox(xorstr_("Wounded"), &player::Wounded);
			ImGui::Checkbox(xorstr_("Skeleton"), &player::skeleton);
			ImGui::Checkbox(xorstr_("Fill Box"), &player::fill);
			ImGui::Checkbox(xorstr_("Box"), &player::box);
			ImGui::Checkbox(xorstr_("Helth Bar"), &player::Hpbar);
			if (player::box) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("Box Style"), &player::boxstyle, menu::box_style, IM_ARRAYSIZE(menu::box_style));
				ImGui::PopItemWidth();
			}
			ImGui::Checkbox(xorstr_("target"), &combot::target);
			if (combot::target) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("targetline_pos"), &combot::targetline_pos, target_tracer_pos, IM_ARRAYSIZE(target_tracer_pos));
				ImGui::PopItemWidth();
			}

			ImGui::EndChild();
		}
	};

	auto settings_tab = [&]() {
		ImGui::Spacing();
		ImGui::BeginChild(xorstr_("ColorEntries"), ImVec2(250.f, -1.f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::SliderFloat(xorstr_("Fov"), &combot::fov, 30.00f, 130.f);
			menu::Hotkey(xorstr_("Zoom"), &combot::zoomkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("Fake admin"), &combot::fakeadmin);
			ImGui::Checkbox(xorstr_("Fakeshots"), &combot::fakeshots);
			menu::Hotkey(xorstr_("Fake shots"), &combot::fakeshotkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("On Ladder"), &combot::OnLadder);
			menu::Hotkey(xorstr_("On Ladder "), &combot::OnLadderkey, ImVec2(200.f, 0));
			ImGui::Checkbox(xorstr_("No attack sprinting"), &combot::Noattacksprinting);
			ImGui::Checkbox(xorstr_("Better Jump"), &combot::nojumpingrestriction);
			ImGui::Checkbox(xorstr_("No sprint ingrestriction"), &combot::nosprintingrestriction);
			ImGui::Checkbox(xorstr_("No fall damage"), &combot::Nofalldamage);
			ImGui::Checkbox(xorstr_("Logo"), &combot::logo);
			ImGui::Checkbox(xorstr_("Skeet Indicator"), &combot::Skeet_indicator);
			ImGui::Checkbox(xorstr_("Simple Indicator"), &combot::indicator_lua);
			ImGui::Checkbox(xorstr_("keybinds"), &combot::keybinds);
			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::BeginChild(xorstr_("Settings"), ImVec2(250.f, 225.f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::Checkbox(xorstr_("Radar"), &MiscVisual::radar_);
			{
				ImGui::SliderFloat(xorstr_("Size"), &MiscVisual::radar::size, 5.f, 400.f);
				ImGui::SliderFloat(xorstr_("Range"), &MiscVisual::radar::range, 5.f, 400.f);
			}
		    ImGui::Checkbox(xorstr_("Anti-Aim"), &combot::anti_aim);
			ImGui::SliderInt(xorstr_("Spin Speed"), &combot::anti_aim_speed, 1, 50);
			ImGui::PushItemWidth(100);
			ImGui::SliderInt(xorstr_("Yaw"), &combot::anti_aim_yaw, 1, 50);
			ImGui::PopItemWidth();
			ImGui::Checkbox(xorstr_("Indicator(Test)"), &combot::anti_aim_indicator);
			if (ImGui::BeginCombo("AutoFarm", "Select"))
			{
				ImGui::Checkbox(xorstr_("Auto Farm Ores"), &combot::auto_farm_ore);
				ImGui::Checkbox(xorstr_("Auto Farm Trees"), &combot::auto_farm_tree);
				ImGui::EndCombo();
			}
		/*	ImGui::Checkbox(("Desync"), &Rage::Desync);
			ImGui::Checkbox(("Desync Bar"), &Rage::ShowDesync);
			menu::Hotkey("Desync Key ", &Rage::DesyncKey, ImVec2{ 200, 20 });
			if (Rage::Desync)
			{
				ImGui::Checkbox(("Instant Shoot"), &Rage::InstantShoot);
				ImGui::Checkbox(("Insta kill"), &Rage::PowerShot);
			}*/
			ImGui::EndChild();
		}
	};

	auto misc = [&]() {
		ImGui::Spacing();
		ImGui::BeginChild(xorstr_("MiscChild"), ImVec2(250.f, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::SliderFloat(xorstr_("Recoil"), &combot::recoil_control, 0.00f, 100.f);
			ImGui::SliderFloat(xorstr_("Spread"), &combot::spead_control, 0.00f, 100.f);
			ImGui::Checkbox(xorstr_("No Sway"), &combot::no_sway);
			ImGui::Checkbox(xorstr_("Big Bullets"), &combot::big_bullets);
			ImGui::Checkbox(xorstr_("Fast Bullets"), &combot::fast_bullets);
			ImGui::Checkbox(xorstr_("Insta Eoka"), &combot::insta_eoka);
			ImGui::Checkbox(xorstr_("Automatic"), &combot::automatic);
			ImGui::Checkbox(xorstr_("Remove attack animation"), &combot::remove_attack_anim);
			ImGui::Separator();
			ImGui::Checkbox(xorstr_("Fake lag"), &combot::fake_lag);
			if (combot::fake_lag) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("Fake lag tick"), &combot::Falelag_type, menu::fakelag_type, IM_ARRAYSIZE(menu::fakelag_type));
				ImGui::PopItemWidth();
				if (combot::Falelag_type == 1)
				{
					ImGui::SliderFloat(xorstr_("Fake tick"), &combot::fake_tick, 0.005f, 1.f);
				}
			}
			ImGui::Checkbox(xorstr_("Noclip Walking"), &combot::running);
			ImGui::Checkbox(xorstr_("Jesus"), &combot::Jesus);
			ImGui::Checkbox(xorstr_("Reload Indicator"), &MiscVisual::reload_indicator);
			ImGui::Checkbox(xorstr_("Target Info"), &MiscVisual::target_info);
			ImGui::Checkbox(xorstr_("Activ Slot"), &MiscVisual::activ_slot);
			ImGui::Checkbox(xorstr_("Crosshairs"), &MiscVisual::corsshairs);
			if (MiscVisual::corsshairs) {
				ImGui::PushItemWidth(100);
				ImGui::Combo(xorstr_("crosshairs Style"), &MiscVisual::corsshairsstyle, menu::crosshairs, IM_ARRAYSIZE(menu::crosshairs));
				ImGui::PopItemWidth();
			}

			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::BeginChild(xorstr_("Misc2Child"), ImVec2(250.f, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			ImGui::EndChild();
		}
	};
	auto config_tab = [&]() {
	};
	auto& st = ImGui::GetStyle();
	st.FrameBorderSize = 1.0f;
	st.FramePadding = ImVec2(4.0f, 2.0f);
	st.ItemSpacing = ImVec2(8.0f, 2.0f);
	st.WindowBorderSize = 1.0f;
	st.WindowRounding = 1.0f;
	st.ChildRounding = 1.0f;
	st.AntiAliasedFill = true;
	st.AntiAliasedLines = true;
	st.WindowBorderSize = 0.0f;

	// Setup style
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TabButton] = ImVec4(0.08f, 0.08f, 0.08f, 1.f);
	colors[ImGuiCol_TabButtonHovered] = ImVec4(0.08f, 0.08f, 0.08f, 1.f);
	colors[ImGuiCol_TabButtonActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	ImGui::SetNextWindowSize(ImVec2(500, 350));
	if (ImGui::Begin(xorstr_("CheatHWND"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::GetStyle().WindowPadding = { 5.f, 5.f };

		if (ImGui::BeginChild(xorstr_("CheatHWNDChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding)) {

			static int selected_tab = 0;
			tab(xorstr_("Aimbot"), 0, selected_tab, ImVec2(88.f, 28.f));
			ImGui::SameLine();
			tab(xorstr_("Visuals"), 2, selected_tab, ImVec2(88.f, 28.f));
			ImGui::SameLine();
			tab(xorstr_("Misc"), 1, selected_tab, ImVec2(88.f, 28.f));
			ImGui::SameLine();
			tab(xorstr_("Settings"), 3, selected_tab, ImVec2(89.f, 28.f));
			//ImGui::SameLine();
			//tab(xorstr_("Config"), 4, selected_tab, ImVec2(89.f, 28.f));

			switch (selected_tab) {
			case 0:
				aimbot();
				break;
			case 1:
				misc();
				break;
			case 2:
				visuals();
				break;
			case 3:
				settings_tab();
				break;
			case 4:
				//	config_tab();
				break;
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}

}
