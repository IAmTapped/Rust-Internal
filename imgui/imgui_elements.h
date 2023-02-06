
#define get_keycode_name(vk, name)\
switch (vk) {\
case VK_CONTROL: name = "control"; break;\
case VK_SHIFT: name = "shift"; break;\
case VK_LSHIFT: name = "shift"; break;\
case VK_MENU: name = "alt"; break;\
case VK_TAB: name = "tab"; break;\
case VK_LBUTTON: name = "mouse 1"; break;\
case VK_RBUTTON: name = "mouse 2"; break;\
case VK_MBUTTON: name = "mouse 3"; break;\
case VK_XBUTTON1: name = "mouse 4"; break;\
case VK_XBUTTON2: name = "mouse 5"; break;\
case VK_PRIOR: name = "page up"; break;\
case VK_NEXT: name = "page down"; break;\
case VK_END: name = "end"; break;\
case VK_HOME: name = "home"; break;\
case VK_LEFT: name = "left arrow"; break;\
case VK_UP: name = "up arrow"; break;\
case VK_RIGHT: name = "right arrow"; break;\
case VK_DOWN: name = "down arrow"; break;\
case VK_INSERT: name = "insert"; break;\
case VK_DELETE: name = "delete"; break;\
case 'A': name = "a"; break;\
case 'B': name = "b"; break;\
case 'C': name = "c"; break;\
case 'D': name = "d"; break;\
case 'E': name = "e"; break;\
case 'F': name = "f"; break;\
case 'G': name = "g"; break;\
case 'H': name = "h"; break;\
case 'I': name = "i"; break;\
case 'J': name = "j"; break;\
case 'K': name = "k"; break;\
case 'L': name = "l"; break;\
case 'M': name = "m"; break;\
case 'N': name = "n"; break;\
case 'O': name = "o"; break;\
case 'P': name = "p"; break;\
case 'Q': name = "q"; break;\
case 'R': name = "r"; break;\
case 'S': name = "s"; break;\
case 'T': name = "t"; break;\
case 'U': name = "u"; break;\
case 'V': name = "v"; break;\
case 'W': name = "w"; break;\
case 'X': name = "x"; break;\
case 'Y': name = "y"; break;\
case 'Z': name = "z"; break;\
case VK_NUMPAD0: name = "numpad 0"; break;\
case VK_NUMPAD1: name = "numpad 1"; break;\
case VK_NUMPAD2: name = "numpad 2"; break;\
case VK_NUMPAD3: name = "numpad 3"; break;\
case VK_NUMPAD4: name = "numpad 4"; break;\
case VK_NUMPAD5: name = "numpad 5"; break;\
case VK_NUMPAD6: name = "numpad 6"; break;\
case VK_NUMPAD7: name = "numpad 7"; break;\
case VK_NUMPAD8: name = "numpad 8"; break;\
case VK_NUMPAD9: name = "numpad 9"; break;\
case VK_F1: name = "F1"; break;\
case VK_F2: name = "F2"; break;\
case VK_F3: name = "F3"; break;\
case VK_F4: name = "F4"; break;\
case VK_F5: name = "F5"; break;\
case VK_F6: name = "F6"; break;\
case VK_F7: name = "F7"; break;\
case VK_F8: name = "F8"; break;\
case VK_F9: name = "F9"; break;\
case VK_F10: name = "F10"; break;\
case VK_F11: name = "F11"; break;\
case VK_F12: name = "F12"; break;\
default: name = "unknown";\
}



bool Tab(const char* icon, const char* label, const char* desc, const ImVec2& size_arg, const bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	static float sizeplus = 0.f;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (selected)
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(25, 30, 35));

	//	ImGui::PushFont(big);
	window->DrawList->AddText(ImVec2(bb.Min.x + 35, bb.Min.y + 13), ImColor(255 / 255.f, 255 / 255.f, 255 / 255.f, 255.f / 255.f), label);
	//	ImGui::PopFont();

	window->DrawList->AddText(ImVec2(bb.Min.x + 35, bb.Min.y + 26), ImColor(100 / 255.f, 100 / 255.f, 100 / 255.f, 255.f / 255.f), desc);

	//	ImGui::PushFont(icons);
	window->DrawList->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(icon).y / 2), ImColor(84 / 255.f, 182 / 255.f, 225 / 255.f, 255.f / 255.f), icon);
	//	ImGui::PopFont();

	return pressed;
}

void RenderCheckMar1k(ImVec2 pos, ImU32 col, float sz)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness * 0.5f;
	pos = ImVec2(pos.x + thickness * 0.25f, pos.y + thickness * 0.25f);

	float third = sz / 3.0f;
	float bx = pos.x + third;
	float by = pos.y + sz - third * 0.5f;
	window->DrawList->PathLineTo(ImVec2(bx - third, by - third));
	window->DrawList->PathLineTo(ImVec2(bx, by));
	window->DrawList->PathLineTo(ImVec2(bx + third * 2, by - third * 2));
	window->DrawList->PathStroke(col, false, thickness);
}

bool CheckboxD(const char* label, bool* v)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const float square_sz = ImGui::GetFrameHeight();
	const ImVec2 pos = window->DC.CursorPos;
	const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y + style.FramePadding.y * 2.0f));
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
	{
		*v = !(*v);
		ImGui::MarkItemEdited(id);
	}

	const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));

	float t = *v ? 1.0f : 0.0f;

	float ANIM_SPEED = 0.05f;
	if (g.LastActiveIdTimer == g.CurrentWindow->GetID(label))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg = ImGui::GetColorU32(ImVec4(120 / 255.f, 120 / 255.f, 120 / 255.f, 120 / 255.f));
	ImU32 col_bg2;
	ImU32 col_bg3;

	col_bg2 = ImGui::GetColorU32(ImLerp(ImVec4(190 / 255.f, 190 / 255.f, 190 / 255.f, 0 / 255.f), ImVec4(5 / 255.f, 193 / 255.f, 245 / 255.f, 255.f / 255.f), t));
	col_bg3 = ImGui::GetColorU32(ImLerp(ImVec4(190 / 255.f, 190 / 255.f, 190 / 255.f, 0 / 255.f), ImVec4(255, 255, 255, 255), t));

	window->DrawList->AddRect(check_bb.Min, check_bb.Max, col_bg, 4.f, 15, 2.f);
	window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, col_bg2, 4.f);

	if (*v)
	{
		RenderCheckMar1k(ImVec2{ check_bb.Min.x + 4,check_bb.Min.y + 4 }, col_bg3, square_sz - 8);
	}

	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

	return pressed;
}

bool SubTab(const char* label, const ImVec2& size_arg, const bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	static float sizeplus = 0.f;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (selected)
		window->DrawList->AddRectFilled({ bb.Min.x,bb.Min.y }, { bb.Max.x,bb.Max.y }, ImColor(30, 35, 40, 200));

	if (selected)
		window->DrawList->AddRectFilled({ bb.Max.x,bb.Max.y }, { bb.Max.x - 3,bb.Min.y }, ImColor(84 / 255.f, 182 / 255.f, 225 / 255.f, 255.f / 255.f));

	//	ImGui::PushFont(big);
	window->DrawList->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(255 / 255.f, 255 / 255.f, 255 / 255.f, 255.f / 255.f), label);
	//	ImGui::PopFont();


	return pressed;
}

static bool IsKeyPressedMap(ImGuiKey key, bool repeat)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}


void ImGui::StyleColorsLight(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void ImGui::StyleColorsDark(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(222 / 255.f, 222 / 255.f, 222 / 255.f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(27 / 255.f, 31 / 255.f, 37 / 255.f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(39 / 255.f, 43 / 255.f, 47 / 255.f, 0.f);
	colors[ImGuiCol_TabBar] = ImVec4(20 / 255.f, 22 / 255.f, 26 / 255.f, 1.f);
	colors[ImGuiCol_BackgroundTop] = ImVec4(41 / 255.f, 42 / 255.f, 47 / 255.f, 1);
	colors[ImGuiCol_BackgroundBot] = ImVec4(26 / 255.f, 28 / 255.f, 32 / 255.f, 1);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(50 / 255.f, 53 / 255.f, 58 / 255.f, 1);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_Combo] = colors[ImGuiCol_Checkbox];
	colors[ImGuiCol_PopupBg] = colors[ImGuiCol_Combo];
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
	colors[ImGuiCol_CheckMark] = ImVec4(214 / 255.f, 252 / 255.f, 255 / 255.f, 1);
	colors[ImGuiCol_Checkbox] = ImVec4(32 / 255.f, 38 / 255.f, 46 / 255.f, 1);
	colors[ImGuiCol_CheckboxActive] = ImVec4(90 / 255.f, 181 / 255.f, 218 / 255.f, 1);
	colors[ImGuiCol_CheckboxBorder] = ImVec4(42 / 255.f, 47 / 255.f, 57 / 255.f, 1);
	colors[ImGuiCol_CheckboxBorderHovered] = ImVec4(87 / 255.f, 123 / 255.f, 249 / 255.f, 1);
	colors[ImGuiCol_SliderGrab] = colors[ImGuiCol_CheckboxActive];
	colors[ImGuiCol_Slider] = ImVec4(21 / 255.f, 23 / 255.f, 28 / 255.f, 1);
	colors[ImGuiCol_SliderActive] = ImVec4(88 / 255.f, 163 / 255.f, 195 / 255.f, 1);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(50 / 255.f, 53 / 255.f, 58 / 255.f, 1);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImVec4(27 / 255.f, 31 / 255.f, 37 / 255.f, 1.00f);
	colors[ImGuiCol_TabTextActive] = ImVec4(174 / 255.f, 129 / 255.f, 19 / 255.f, 1.f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}