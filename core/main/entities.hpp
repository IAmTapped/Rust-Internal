#define min(a,b)            (((a) < (b)) ? (a) : (b))
//inline BasePlayer LocalPlayer;
//PlayerInventory Inventory;
void radar_bg() {
	float mid_x = MiscVisual::radar::x + MiscVisual::radar::size / 2;
	float mid_y = MiscVisual::radar::y + MiscVisual::radar::size / 2;
	POINT p;
	if (GetCursorPos(&p)) {
		if (p.x >= mid_x - MiscVisual::radar::size && p.x <= mid_x + MiscVisual::radar::size) {
			if (p.y >= mid_y - MiscVisual::radar::size && p.y <= mid_y + MiscVisual::radar::size) {
				if (GetAsyncKeyState(VK_LBUTTON) && show) {
					MiscVisual::radar::x = p.x - (MiscVisual::radar::size / 2);
					MiscVisual::radar::y = p.y - (MiscVisual::radar::size / 2);
				}
			}
		}
	}
	Renderer::filled_circle(Vector2(mid_x, mid_y), Color3(18, 18, 18, 179), MiscVisual::radar::size);
	Renderer::circle(Vector2(mid_x, mid_y), Color3(0, 0, 0), MiscVisual::radar::size, 1.f);

	Renderer::filled_circle(Vector2(mid_x, mid_y), Color3(255, 255, 255), 3.5f);
	//	Renderer::Triangle(Vector2(mid_x, mid_y), Color3(255, 0, 0), 3.5f);
}
float NormalizeAngle(float angle) {
	while (angle > 360.0f) {
		angle -= 360.0f;
	}
	while (angle < 0.0f) {
		angle += 360.0f;
	}
	return angle;
}
Vector3 NormalizeAngles(Vector3 angles) {
	angles.x = NormalizeAngle(angles.x);
	angles.y = NormalizeAngle(angles.y);
	angles.z = NormalizeAngle(angles.z);
	return angles;
}

Vector3 EulerAngles(Quaternion q1) {
	float num = q1.w * q1.w;
	float num2 = q1.x * q1.x;
	float num3 = q1.y * q1.y;
	float num4 = q1.z * q1.z;
	float num5 = num2 + num3 + num4 + num;
	float num6 = q1.x * q1.w - q1.y * q1.z;
	Vector3 vector;
	if (num6 > 0.4995f * num5) {
		vector.y = 2.0f * std::atan2f(q1.y, q1.x);
		vector.x = 1.57079637f;
		vector.z = 0.0f;
		return NormalizeAngles(vector * 57.2958f);
	}
	if (num6 < -0.4995f * num5) {
		vector.y = -2.0f * std::atan2f(q1.y, q1.x);
		vector.x = -1.57079637f;
		vector.z = 0.0f;
		return NormalizeAngles(vector * 57.2958f);
	}
	Quaternion quaternion = Quaternion(q1.w, q1.z, q1.x, q1.y);
	vector.y = std::atan2f(2.0f * quaternion.x * quaternion.w + 2.0f * quaternion.y * quaternion.z, 1.0f - 2.0f * (quaternion.z * quaternion.z + quaternion.w * quaternion.w));
	vector.x = std::asin(2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y));
	vector.z = std::atan2f(2.0f * quaternion.x * quaternion.y + 2.0f * quaternion.z * quaternion.w, 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));
	return NormalizeAngles(vector * 57.2958f);
}
void radar_logic(DWORD64 ObjectClass, DWORD64 Object, char* buff) {
	float mid_x = MiscVisual::radar::x + MiscVisual::radar::size / 2;
	float mid_y = MiscVisual::radar::y + MiscVisual::radar::size / 2;
	if (LocalPlayer::Entity()) {
		Vector3 local = LocalPlayer::Entity()->get_bone_pos(head);
		if (strstr(buff, ("player.prefab")) || strstr(buff, ("scientist")) && !strstr(buff, ("prop")) && !strstr(buff, ("corpse"))) {
			BasePlayer* Player = (BasePlayer*)Object;
			if (!Player->isCached()) return;

			Vector3 ply = cachedBones[Player->userID()]->head->position;
			float dist = Calc3D_Dist(local, ply);
			float y = local.x - ply.x;
			float x = local.z - ply.z;

			Vector3 eulerAngles = EulerAngles(LocalPlayer::Entity()->eyes()->rotation()/*pViewMatrix->GetRotation()*/);
			float num = atan2(y, x) * 57.29578f - 270.f - eulerAngles.y;
			float PointPos_X = dist * cos(num * 0.0174532924f);
			float PointPos_Y = dist * sin(num * 0.0174532924f);
			PointPos_X = PointPos_X * (MiscVisual::radar::size / MiscVisual::radar::range) / 2.f;
			PointPos_Y = PointPos_Y * (MiscVisual::radar::size / MiscVisual::radar::range) / 2.f;

			Vector2 point = Vector2(MiscVisual::radar::x + MiscVisual::radar::size / 2.f + PointPos_X, MiscVisual::radar::y + MiscVisual::radar::size / 2.f + PointPos_Y);

			if (Distance_2D(point, Vector2(mid_x, mid_y)) < MiscVisual::radar::size) {
				if (!Player->playerModel()->isNpc()) {
					if (!Player->HasPlayerFlag(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity()->is_teammate()) {
							Renderer::filled_circle(point, Color3(255, 255, 255), 3.5f);
						}
						else {
							if (Player->health() <= 0) {
								Renderer::filled_circle(point, Color3(255, 0, 0), 3.5f);
							}
							else {
								if (Player->is_visible()) {
									Renderer::filled_circle(point, Color3(255, 153, 51), 3.5f);
								}
								else {
									Renderer::filled_circle(point, Color3(96, 96, 96), 3.5f);
								}
							}
						}
					}
					else if (player::sleepers && Player->HasPlayerFlag(PlayerFlags::Sleeping)) {
						Renderer::filled_circle(point, Color3(255, 255, 51), 3.5f);
					}
				}
				else if (player::npc && Player->playerModel()->isNpc()) {
					Renderer::filled_circle(point, Color3(255, 128, 0), 3.5f);
				}
			}
		}
	}
}
namespace entities {
	namespace belt {
		Vector2 pos = Vector2(200, 200);
		bool should_drag = false;
		bool should_move = false;

		POINT cursor;
		POINT cursor_corrected;
		void belt_tab_mov(Vector2 size) {
			GetCursorPos(&cursor);

			if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
				should_drag = true;
				if (!should_move) {
					cursor_corrected.x = cursor.x - pos.x;
					cursor_corrected.y = cursor.y - pos.y;
					should_move = true;
				}
			}

			if (should_drag) {
				pos.x = cursor.x - cursor_corrected.x;
				pos.y = cursor.y - cursor_corrected.y;
			}

			if (GetAsyncKeyState(VK_LBUTTON) == 0) {
				should_drag = false;
				should_move = false;
			}
		}
	}
	namespace keybinds_ {
		Vector2 pos = Vector2(300, 300);
		bool should_drag = false;
		bool should_move = false;

		POINT cursor;
		POINT cursor_corrected;
		void belt_tab_mov(Vector2 size) {
			GetCursorPos(&cursor);

			if (GetAsyncKeyState(VK_LBUTTON) && (cursor.x > pos.x && cursor.y > pos.y && cursor.x < pos.x + size.x && cursor.y < pos.y + size.y)) {
				should_drag = true;
				if (!should_move) {
					cursor_corrected.x = cursor.x - pos.x;
					cursor_corrected.y = cursor.y - pos.y;
					should_move = true;
				}
			}

			if (should_drag) {
				pos.x = cursor.x - cursor_corrected.x;
				pos.y = cursor.y - cursor_corrected.y;
			}

			if (GetAsyncKeyState(VK_LBUTTON) == 0) {
				should_drag = false;
				should_move = false;
			}
		}


	}
	float dfc(BasePlayer* player) {
		if (!player)
			return 1000.f;

		if (!player->isCached())
			return 1000.f;

		if (player->bones()->dfc.empty())
			return 1000.f;

		return screen_center.distance_2d(player->bones()->dfc);
	}
	float BOG_TO_GRD(float BOG) {
		return (180 / M_PI) * BOG;
	}

	float GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}

	__forceinline uint32_t RandomInteger(uint32_t Min, uint32_t Max)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		const std::uniform_int_distribution<uint32_t> distr(Min, Max);
		return distr(eng);
	}
	Color3 get_color(BasePlayer* player, bool boxes = false) {
		return Color3({ colors::target_box.x, colors::target_box.y, colors::target_box.z, 1 });
	}

	void loop() {

		LogSystem::Render();
		LogSystem::RenderExplosions();

		if (MiscVisual::radar_)
			radar_bg();
		if (combot::bfly) {
			Renderer::FillRectangle(Vector2(viewport.Width / 2 - 214, 200), Vector2(428 * (stuff::flyhack / stuff::max_flyhack), 10), D2D1::ColorF::Lime);
			Renderer::Rectangle(Vector2(viewport.Width / 2 - 214, 200), Vector2(428, 10), D2D1::ColorF::Black);

			Renderer::FillRectangle(Vector2(viewport.Width / 2 - 214, 230), Vector2(428 * (stuff::hor_flyhack / stuff::max_hor_flyhack), 10), D2D1::ColorF::Lime);
			Renderer::Rectangle(Vector2(viewport.Width / 2 - 214, 230), Vector2(428, 10), D2D1::ColorF::Black);
		}
		//config::Setup();

		static Color3 clr = Color3(RandomInteger(100, 255), RandomInteger(100, 255), RandomInteger(100, 255), 255);
		static float faken_rot = 0.0f;
		static int gamerjuice = 0;
		float a = screen_center.y / 30.0f;
		float gamma = atan(a / a);
		if (combot::keybinds)
		{
			float mid_x = keybinds_::pos.x;
			float mid_y = keybinds_::pos.y;
			keybinds_::belt_tab_mov(Vector2(85, 20 + 20.0f));
			Renderer::rectangle_filled(Vector2{ keybinds_::pos.x,keybinds_::pos.y - 2 }, { 85, 1 }, Color3(238, 32, 77));
			Renderer::rectangle_filled(Vector2{ keybinds_::pos.x,keybinds_::pos.y }, { 85, 20 }, Color3(0, 0, 0));
			Renderer::t({ keybinds_::pos.x + 42,keybinds_::pos.y + 9 }, Color3(255, 255, 255), 10.f, true, true, wxorstr_(L"Keybinds"));
			int y_ = 0;
			if (settings::tr::manipulated) {
				Renderer::t({ keybinds_::pos.x + 42,keybinds_::pos.y + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L"Manipulator %s"), target_ply->_displayName());
				y_ += 15;
			}
			if (settings::tr::FakeShotKey) {
				Renderer::t({ keybinds_::pos.x + 42,keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" FakeShot    on"));
				y_ += 15;
			}
			if (settings::tr::Onlader) {
				Renderer::t({ keybinds_::pos.x + 42,keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" OnLadder    on"));
				y_ += 15;
			}
			if (combot::fake_lag) {
				if (combot::Falelag_type == 1)
				{
					Renderer::t({ keybinds_::pos.x + 42, keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" FakeLag  %.3f"), combot::fake_tick);
					y_ += 15;
				}
				if (combot::Falelag_type == 2)
				{
					Renderer::t({ keybinds_::pos.x + 42, keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" FakeLag Legit"));
					y_ += 15;
				}
				if (combot::Falelag_type == 3)
				{
					Renderer::t({ keybinds_::pos.x + 42, keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" FakeLag randomizer"));
					y_ += 15;
				}
				/*				Renderer::t({ keybinds_::pos.x + 42, keybinds_::pos.y + (0, y_) + 27 }, Color3(255, 255, 255), 11.f, true, true, wxorstr_(L" FakeLag  %.3f"), combot::fake_tick);
								y_ += 15;*/
			}
		}
		if (combot::indicator_lua)
		{
			int y_ = 0;
			Renderer::t({ screen_center.x, screen_center.y + 18 }, Color3(238, 32, 77), 12.f, true, true, wxorstr_(L"Niggerscript"));
			Renderer::rectangle_filled({ screen_center.x - 49, screen_center.y + 28 }, { 100, 1.5 }, Color3(238, 32, 77));
			if (settings::tr::manipulated) {
				Renderer::t({ screen_center.x, screen_center.y + 38 }, Color3(238, 32, 77), 10.f, true, true, wxorstr_(L"Manipulator"));
				auto local = LocalPlayer::Entity();
				float desyncTime = (Time::realtimeSinceStartup() - local->lastSentTickTime()) - 0.03125 * 3;
				Renderer::rectangle_filled({ screen_center.x - 49, screen_center.y + 48 }, { 100, 5 }, Color3(0, 0, 0));
				float desync_max_value = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * local->MaxVelocity()) - 0.05f;//added
				Renderer::rectangle_filled({ screen_center.x - 49, screen_center.y + 48 }, { 100 * (desyncTime), 5 }, Color3(0, 255, 0));
				Renderer::t({ (6) + (70 * (desyncTime)), screen_center.y + 27 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"%d"), (int)desyncTime);
				y_ += 15;
			}
			if (settings::tr::FakeShotKey) {
				Renderer::t({ screen_center.x, screen_center.y + (0, y_) + 44 }, Color3(173, 0, 0), 10.f, true, true, wxorstr_(L"FakeShot"));
				y_ += 15;
			}
			if (settings::tr::Onlader) {
				Renderer::t({ screen_center.x, screen_center.y + (0, y_) + 44 }, Color3(173, 0, 0), 10.f, true, true, wxorstr_(L"OnLadder"));
				y_ += 15;
			}
		}
		if (combot::Skeet_indicator)
		{
			int y_ = 0;
			if (settings::tr::manipulated)
			{
				Renderer::FillRectangle(Vector2{ 4, screen_center.y + 2 }, Vector2{ 34, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right
				Renderer::t({ 8,  screen_center.y + 8 }, Color3(173, 0, 0), 19.f, false, false, wxorstr_(L"MP"));
				y_ += 32;
			}
			if (settings::tr::FakeShotKey)
			{
				Renderer::FillRectangle(Vector2{ 4, screen_center.y + (0, y_) + 2 }, Vector2{ 34, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right
				Renderer::t({ 8,  screen_center.y + (0, y_) + 8 }, Color3(52, 201, 36), 19.f, false, false, wxorstr_(L"FS"));
				y_ += 32;
			}
			if (settings::tr::Onlader)
			{
				Renderer::FillRectangle(Vector2{ 4, screen_center.y + (0, y_) + 2 }, Vector2{ 34, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right
				Renderer::t({ 8,  screen_center.y + (0, y_) + 8 }, Color3(52, 201, 36), 19.f, false, false, wxorstr_(L"OL"));
				y_ += 32;
			}
			if (combot::fake_lag)
			{
				if (combot::Falelag_type == 1)
				{
					Renderer::FillRectangle(Vector2{ 4, screen_center.y + (0, y_) + 2 }, Vector2{ 94, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right

					Renderer::t({ 8,  screen_center.y + (0, y_) + 8 }, Color3(237, 118, 14), 19.f, false, false, wxorstr_(L"FL %.3f"), combot::fake_tick);
					y_ += 32;
				}
				if (combot::Falelag_type == 2)
				{
					Renderer::FillRectangle(Vector2{ 4, screen_center.y + (0, y_) + 2 }, Vector2{ 94, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right
					Renderer::t({ 8,  screen_center.y + (0, y_) + 8 }, Color3(237, 118, 14), 19.f, false, false, wxorstr_(L"FL LEGIT"));
					y_ += 32;
				}
				if (combot::Falelag_type == 3)
				{
					Renderer::FillRectangle(Vector2{ 4, screen_center.y + (0, y_) + 2 }, Vector2{ 119, 30 }, D2D1::ColorF(0, 0, 0, 0.3)); // right
					Renderer::t({ 8,  screen_center.y + (0, y_) + 8 }, Color3(237, 118, 14), 19.f, false, false, wxorstr_(L"FL RANDOM"));
					y_ += 32;
				}
			}

		}
		switch (MiscVisual::corsshairsstyle) {
		case 1:

			Renderer::FillRectangle(Vector2{ screen_center.x + 5, screen_center.y }, Vector2{ 4, 2 }, D2D1::ColorF::White); // right
			Renderer::FillRectangle(Vector2{ screen_center.x - 5 + 2, screen_center.y }, Vector2{ -4 , 2 }, D2D1::ColorF::White); // left
			Renderer::FillRectangle(Vector2{ screen_center.x , screen_center.y - 5 + 2 }, Vector2{ 2 , -4 }, D2D1::ColorF::White); // up
			Renderer::FillRectangle(Vector2{ screen_center.x , screen_center.y + 5 }, Vector2{ 2 , 4 }, D2D1::ColorF::White); // down
			break;
		case 2:
			Renderer::line(screen_center, Vector2{ screen_center.x + 4, screen_center.y + 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x + 4, screen_center.y - 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x - 4, screen_center.y - 4 }, Color3(255, 255, 255), false, 0.7f);
			Renderer::line(screen_center, Vector2{ screen_center.x - 4, screen_center.y + 4 }, Color3(255, 255, 255), false, 0.7f);

			Renderer::line(Vector2{ screen_center.x + 4, screen_center.y + 4 }, Vector2{ screen_center.x + 4 + 4, screen_center.y + 4 + 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x + 4, screen_center.y - 4 }, Vector2{ screen_center.x + 4 + 4, screen_center.y - 4 - 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x - 4, screen_center.y - 4 }, Vector2{ screen_center.x - 4 - 4, screen_center.y - 4 - 4 }, Color3(0, 0, 0), false, 0.7f);
			Renderer::line(Vector2{ screen_center.x - 4, screen_center.y + 4 }, Vector2{ screen_center.x - 4 - 4, screen_center.y + 4 + 4 }, Color3(0, 0, 0), false, 0.7f);
			break;
		case 3:
			Renderer::circle(screen_center, Color3(0, 0, 0), 2.f, 1.f);
			Renderer::circle(screen_center, Color3(0, 0, 0), 4.f, 1.f);
			Renderer::circle(screen_center, Color3(255, 255, 255), 3.f, 1.f);
			break;

		}

		auto local = LocalPlayer::Entity();
		if (local != NULL) {
			target_ply = nullptr;
		}

		/*if (settings::tr::manipulated) {
			Renderer::t({ screen_center.x, screen_center.y + 18 }, Color3(173, 0, 0), 10.f, true, true, wxorstr_(L"[Manipulated]"));

			float desyncTime = (Time::realtimeSinceStartup() - local->lastSentTickTime()) - 0.03125 * 3;
			Renderer::rectangle_filled({ screen_center.x - 33, screen_center.y + 35 }, { 100, 5 }, Color3(0, 0, 0));

			Renderer::rectangle_filled({ screen_center.x - 33, screen_center.y + 36 }, { 100 * (desyncTime), 5 }, Color3(0, 255, 0));
			Renderer::t({ (screen_center.x - 32) + (70 * (desyncTime)), screen_center.y + 27 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"%d"), (int)desyncTime);
		}*/
		if (MiscVisual::reload_indicator) {
			auto held = local->GetHeldEntity<BaseProjectile>();

			if (held) {

				if (held->HasReloadCooldown() && held->class_name_hash() != STATIC_CRC32("BowWeapon") && held->class_name_hash() != STATIC_CRC32("CompoundBowWeapon")) { // im sorry for my sins
					float time_left = held->nextReloadTime() - GLOBAL_TIME;
					float time_full = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
					Renderer::rectangle_filled({ screen_center.x - 33, screen_center.y + 88 }, { 71, 6 }, Color3(0, 0, 0));
					Renderer::rectangle_filled({ screen_center.x - 33, screen_center.y + 88 }, { 70 * (time_left / time_full), 6 }, Color3(0, 255, 0));

					Renderer::t({ (screen_center.x - 32) + (70 * (time_left / time_full)), screen_center.y + 79 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"[%.2f]"), time_left);
				}
				if (held->class_name_hash() == STATIC_CRC32("BaseProjectile") ||
					held->class_name_hash() == STATIC_CRC32("BowWeapon") ||
					held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon") ||
					held->class_name_hash() == STATIC_CRC32("BaseLauncher") ||
					held->class_name_hash() == STATIC_CRC32("CrossbowWeapon")) {
					if (held->Empty()) {
						Renderer::t({ screen_center.x, screen_center.y + 98 }, Color3(89, 227, 255), 12.f, true, true, wxorstr_(L"[empty weapon]"));
					}
				}
			}
		}

		if (combot::aimbot && !LocalPlayer::Entity()->is_teammate()) {
			if (target_ply) {
				if (GetAsyncKeyState(combot::aimkey)) {
					do_aimbot(reinterpret_cast<BasePlayer*>(target_ply));
				}
			}
		}

		if (closestHeli != NULL) {
			if (reinterpret_cast<BaseEntity*>(closestHeli)->IsDestroyed()) {
				closestHeli = NULL;
			}
			if (!reinterpret_cast<Component*>(closestHeli)->gameObject()) {
				closestHeli = NULL;
			}
		}

		if (player::enable || resource::ores || resource::stash || resource::corpses || resource::traps || resource::hemp || resource::vehicles) {
			auto entityList = BaseNetworkable::clientEntities()->entityList();
			if (!entityList) {
				target_ply = nullptr;
				return;
			}

			if (entityList->vals->size <= 1) {
				target_ply = nullptr;
				return;
			}

			viewMatrix = Camera::getViewMatrix();
			if (target_ply != nullptr) {
				if (!target_ply->IsValid() || target_ply->health() <= 0 || target_ply->is_teammate() || target_ply->HasPlayerFlag(PlayerFlags::Sleeping) || entities::dfc(target_ply) > combot::fovaim || (target_ply->playerModel()->isNpc() && !player::npc)) {
					target_ply = nullptr;
				}
				else {
					if (target_ply->isCached()) {
						auto bounds = target_ply->bones()->bounds;
						if (combot::target && target_ply != NULL)
						{
							if (combot::targetline_pos == 1) {
								if (!bounds.empty())
									Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y / 2 - 550.f }, Color3(255, 255, 255), true);
							}
							if (combot::targetline_pos == 2) {
								Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x, screen_size.y }, Color3(255, 255, 255), true);
							}
							if (combot::targetline_pos == 3) {
								Renderer::line({ bounds.left + ((bounds.right - bounds.left) / 2), bounds.bottom }, { screen_center.x , screen_size.y / 2 }, Color3(255, 255, 255), true);
							}
						}


						auto mpv = target_ply->find_mpv_bone();
						Bone* target;
						if (mpv != nullptr)
							target = mpv;
						else
							target = target_ply->bones()->head;


						if (MiscVisual::target_info) {
							float xs = screen_center.x / 2, ys = screen_center.y / 2;
							auto* target2 = reinterpret_cast<BasePlayer*>(target_ply);
							wchar_t name[64];
							if (target_ply != NULL)
							{
								_swprintf(name, L"%s", target2->_displayName());
							}
							else
							{
								_swprintf(name, L"");
							}
							Vector2 ScreenPos;
							Renderer::t({ screen_center.x, screen_center.y - 40 }, Color3(255, 255, 255), 12.f, true, true, name);

							if (target->visible)
								Renderer::t({ screen_center.x, screen_center.y - 20 }, Color3(66, 135, 245), 12.f, true, true, wxorstr_(L"[Visible]"));
							else
								Renderer::t({ screen_center.x, screen_center.y - 20 }, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"[Not visible]"));
						}



						if (MiscVisual::activ_slot) {
							int w = 200, h = 102;

							belt::belt_tab_mov(Vector2(w, h + 20.0f));

							Renderer::rectangle_filled({ belt::pos.x, belt::pos.y }, Vector2(w, 20), Color3(0, 0, 0));
							Renderer::rectangle_filled(Vector2(belt::pos.x, belt::pos.y + 20.0f), Vector2(w, h), Color3(23, 23, 23));
							Renderer::rectangle_filled(Vector2(belt::pos.x + 5.0f, belt::pos.y + 5.0f), Vector2(w - 10, h - 10), Color3(25, 25, 25));

							Renderer::t({ belt::pos.x + 7.0f, belt::pos.y }, Color3(255, 255, 255), 12.f, false, false, target_ply->_displayName());

							auto list = target_ply->inventory()->containerBelt()->itemList();
							if (list) {
								if (list->size) {
									int y = 0;
									for (int i = 0; i < list->size; i++) {
										auto item = (Item*)list->get(i);
										if (!item)
											continue;

										Color3 col = item->uid() == target_ply->clActiveItem() ? Color3(255, 0, 0) : Color3(255, 255, 255);

										Renderer::t({ belt::pos.x + 7.0f, belt::pos.y + 20.0f + y }, col, 12.f, false, false, wxorstr_(L"%s [x%d]"), item->info()->displayName()->english(), item->amount());

										y += 15;
									}
								}
							}
						}
					}
				}
			}


			for (int i = 0; i < entityList->vals->size; i++) {

				auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!entity) continue;
				uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!Entity) continue;
				uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
				if (!Object) continue;
				uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
				if (!ObjectClass) continue;
				pUncStr name = mem::read<pUncStr>(ObjectClass + 0x60); if (!name) continue;
				char* buff = name->stub;

				if (!entity->IsValid()) continue;
				if (player::debug_prefab && entity->transform()->position().distance(LocalPlayer::Entity()->transform()->position()) <= 5.0f) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						Renderer::t(screen, Color3(0, 255, 0), 12.f, true, true, wxorstr_(L"%s"), StringConverter::ToUnicode(entity->class_name()).c_str());
						Renderer::t(screen + Vector2(0, 15), Color3(0, 255, 0), 12.f, true, true, wxorstr_(L"%s"), entity->ShortPrefabName());
					}
				}

				if (MiscVisual::radar_)
					radar_logic(ObjectClass, Entity, buff);

				if (entity->class_name_hash() == STATIC_CRC32("BasePlayer") ||
					entity->class_name_hash() == STATIC_CRC32("NPCPlayerApex") ||
					entity->class_name_hash() == STATIC_CRC32("NPCMurderer") ||
					entity->class_name_hash() == STATIC_CRC32("NPCPlayer") ||
					entity->class_name_hash() == STATIC_CRC32("HumanNPC") ||
					entity->class_name_hash() == STATIC_CRC32("Scientist") ||
					entity->class_name_hash() == STATIC_CRC32("HTNPlayer") ||
					entity->class_name_hash() == STATIC_CRC32("HumanNPCNew") ||
					entity->class_name_hash() == STATIC_CRC32("HumanNPC") ||
					entity->class_name_hash() == STATIC_CRC32("ScientistNPCNew") ||
					entity->class_name_hash() == STATIC_CRC32("ScientistNPC") ||
					entity->class_name_hash() == STATIC_CRC32("TunnelDweller") ||
					entity->class_name_hash() == STATIC_CRC32("BanditGuard")) {
					if (!player::enable && !player::sleepers) continue;
					auto p = reinterpret_cast<BasePlayer*>(entity);

					if (!p->isCached()) continue;
					if (p->health() <= 0.0f) continue;
					if (p->HasPlayerFlag(PlayerFlags::Sleeping) && !player::sleepers) continue;
					if (p->playerModel()->isNpc() && !player::npc) continue;
					if (p->userID() == LocalPlayer::Entity()->userID()) continue;

					auto bone = p->bones()->bounds;
					if (!bone.empty()) {
						int y_ = 0;

						float box_width = bone.right - bone.left;
						float box_height = bone.bottom - bone.top;
						Vector2 footPos = { bone.left + (box_width / 2), bone.bottom + 7.47f };
						Vector2 headPos = { bone.left + (box_width / 2), bone.top - 9.54f };

						Color3 col = Color3(255, 140, 0);
						Color3 box_col = Color3(255, 140, 0);

						if (player::skeleton) {
							float dist = p->bones()->head->position.distance(local->bones()->head->position);


							auto info = p->bones();

							auto head_b = info->head;
							auto spine4_b = info->spine4;
							auto l_upperarm_b = info->l_upperarm;
							auto l_forearm_b = info->l_forearm;
							auto l_hand_b = info->l_hand;
							auto r_upperarm_b = info->r_upperarm;
							auto r_forearm_b = info->r_forearm;
							auto r_hand_b = info->r_hand;
							auto pelvis_b = info->pelvis;
							auto l_hip_b = info->l_hip;
							auto l_knee_b = info->l_knee;
							auto l_foot_b = info->l_foot;
							auto r_hip_b = info->r_hip;
							auto r_knee_b = info->r_knee;
							auto r_foot_b = info->r_foot;
							auto r_toe_b = info->r_toe;
							auto l_toe_b = info->l_toe;

							Vector2 head, spine, l_upperarm, l_forearm, l_hand, r_upperarm, r_forearm, r_hand, pelvis, l_hip, l_knee, l_foot, r_hip, r_knee, r_foot, l_toe, r_toe;
							if (Camera::world_to_screen(head_b->position, head) &&
								Camera::world_to_screen(spine4_b->position, spine) &&
								Camera::world_to_screen(l_upperarm_b->position, l_upperarm) &&
								Camera::world_to_screen(l_forearm_b->position, l_forearm) &&
								Camera::world_to_screen(l_hand_b->position, l_hand) &&
								Camera::world_to_screen(r_upperarm_b->position, r_upperarm) &&
								Camera::world_to_screen(r_forearm_b->position, r_forearm) &&
								Camera::world_to_screen(r_hand_b->position, r_hand) &&
								Camera::world_to_screen(pelvis_b->position, pelvis) &&
								Camera::world_to_screen(l_hip_b->position, l_hip) &&
								Camera::world_to_screen(l_knee_b->position, l_knee) &&
								Camera::world_to_screen(l_foot_b->position, l_foot) &&
								Camera::world_to_screen(r_hip_b->position, r_hip) &&
								Camera::world_to_screen(r_knee_b->position, r_knee) &&
								Camera::world_to_screen(r_toe_b->position, r_toe) &&
								Camera::world_to_screen(l_toe_b->position, l_toe) &&
								Camera::world_to_screen(r_foot_b->position, r_foot)) {

								Color3 viscol = p->playerModel()->isNpc() ? Color3(38, 255, 0) : Color3(0, 250, 255);
								Color3 inviscol = p->playerModel()->isNpc() ? Color3(22, 145, 0) : Color3(0, 152, 156);

								if (p->HasPlayerFlag(PlayerFlags::Sleeping)) {
									viscol = Color3(255, 255, 255);
									inviscol = Color3(255, 0, 0);
								}
								else if (!p->playerModel()->isNpc()) {
									viscol = Color3(255, 255, 255);
									inviscol = Color3(255, 0, 0);
								}
								else {
									viscol = Color3(255, 255, 255);
									inviscol = Color3(255, 0, 0);
								}

								Renderer::line(head, spine, (head_b->visible || spine4_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(spine, l_upperarm, (spine4_b->visible || l_upperarm_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(l_upperarm, l_forearm, (l_upperarm_b->visible || l_forearm_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(l_forearm, l_hand, (l_forearm_b->visible || l_hand_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(spine, r_upperarm, (spine4_b->visible || r_upperarm_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(r_upperarm, r_forearm, (r_upperarm_b->visible || r_forearm_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(r_forearm, r_hand, (r_forearm_b->visible || r_hand_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(spine, pelvis, (spine4_b->visible || pelvis_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(pelvis, l_hip, (pelvis_b->visible || l_hip_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(l_hip, l_knee, (l_hip_b->visible || l_knee_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(l_knee, l_foot, (l_knee_b->visible || l_foot_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(pelvis, r_hip, (pelvis_b->visible || r_hip_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(r_hip, r_knee, (r_hip_b->visible || r_knee_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(r_knee, r_foot, (r_knee_b->visible || r_foot_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(r_foot, r_toe, (r_foot_b->visible || r_toe_b->visible) ? viscol : inviscol, 2.f);
								Renderer::line(l_foot, l_toe, (l_foot_b->visible || l_toe_b->visible) ? viscol : inviscol, 2.f);
							}

						}
						if (warings::Viseble)
						{
							auto mpv = target_ply->find_mpv_bone();
							Bone* target;
							if (mpv != nullptr)
								target = mpv;
							else
								target = target_ply->bones()->head;

							if (target->visible)
								LogSystem::Log(StringFormat::format(wxorstr_(L"%s Is Viseble"), p->_displayName()), 5.f);
						}
						if (player::Name)
						{
							Renderer::t(headPos, col, 11.f, true, true, wxorstr_(L"[%s]"), p->_displayName());
						}
						if (player::Weapon)
						{
							if (p->GetHeldItem() && !p->HasPlayerFlag(PlayerFlags::Sleeping)) {
								Renderer::t(footPos, Color3(48, 213, 200), 11.f, true, true, wxorstr_(L"[%s]"), p->GetHeldItem()->info()->displayName()->english());
								y_ += 13;
							}
						}
						if (player::distance)
						{

							Renderer::t(footPos + Vector2(0, y_), col, 11.f, true, true, wxorstr_(L"[%.2fM][%d%]"), p->bones()->head->position.Calc3D_Dist(local->bones()->head->position, p->bones()->head->position), (int)p->health());
							y_ += 13;
						}
						if (player::Wounded)
						{
							if (p->HasPlayerFlag(PlayerFlags::Wounded)) {
								Renderer::t(headPos - Vector2(0, 15.f), Color3(255, 0, 0, 255), 11.f, true, true, wxorstr_(L"[Wounded]"));
							}
						}
						if (player::fill)
						{
							Renderer::rectangle_filled({ bone.left, bone.top }, Vector2(box_width, box_height), Color3(0, 0, 0, 128));
						}
						if (player::Hpbar)
						{
							int health = p->health();
							float maxheal = (p->isNpc() ? p->maxHealth() : 100);
							const auto health_pc = min(health / maxheal, 1);
							const auto height = (bone.bottom - bone.top) * health_pc;
							Renderer::rectangle_filled(footPos + Vector2(0, y_) - Vector2(30, 0), { 60, 5 }, Color3(0, 0, 0, 2555));
							Renderer::rectangle_filled(footPos + Vector2(0, y_) - Vector2(30, 0), { 60 * (health / maxheal), 5 }, Color3(0, 255, 0));
							Renderer::rectangle(footPos + Vector2(0, y_) - Vector2(30, 0), { 60, 5 }, Color3(0, 0, 0, 255));
							Renderer::t(footPos + Vector2(60 * (health / maxheal), y_), Color3(255, 255, 255), 8.f, true, true, wxorstr_(L"%d"), (int)health);
							//Renderer::t({ (screen_center.x - 32) + (70 * (time_left / time_full)), screen_center.y + 79 }, Color3(255, 255, 255), 12.f, true, true, wxorstr_(L"[%.2f]"), time_left);
							y_ += 10;
						}
						if (player::box) {
							switch (player::boxstyle) {
							case 0: // cornered
								Renderer::line({ bone.left, bone.top }, { bone.left + (box_width / 3.5f), bone.top }, box_col, true, 1.5f);
								Renderer::line({ bone.right, bone.top }, { bone.right - (box_width / 3.5f), bone.top }, box_col, true, 1.5f);

								Renderer::line({ bone.left, bone.bottom }, { bone.left + (box_width / 3.5f), bone.bottom }, box_col, true, 1.5f);
								Renderer::line({ bone.right, bone.bottom }, { bone.right - (box_width / 3.5f), bone.bottom }, box_col, true, 1.5f);

								Renderer::line({ bone.left, bone.top }, { bone.left, bone.top + (box_width / 3.5f) }, box_col, true, 1.5f);
								Renderer::line({ bone.right, bone.top }, { bone.right, bone.top + (box_width / 3.5f) }, box_col, true, 1.5f);

								Renderer::line({ bone.left, bone.bottom }, { bone.left, bone.bottom - (box_width / 3.5f) }, box_col, true, 1.5f);
								Renderer::line({ bone.right, bone.bottom }, { bone.right, bone.bottom - (box_width / 3.5f) }, box_col, true, 1.5f);

								break;
							case 1: // 2d

								Renderer::line({ bone.left, bone.top }, { bone.right, bone.top }, box_col, true, 1.5f);
								Renderer::line({ bone.left, bone.top }, { bone.right, bone.top }, box_col, true, 1.5f);
								Renderer::line({ bone.left, bone.bottom }, { bone.right, bone.bottom }, box_col, true, 1.5f);

								Renderer::line({ bone.left, bone.top }, { bone.left, bone.bottom }, box_col, true, 1.5f);
								Renderer::line({ bone.right, bone.top }, { bone.right, bone.bottom }, box_col, true, 1.5f);
								break;
							case 2: // 3d (hippity hoppity your 3d box is now my property)
								CBounds bone = CBounds();

								if (p->IsDucked()) {
									bone.center = p->midPoint() + Vector3(0.0f, 0.55f, 0.0f);
									bone.extents = Vector3(0.4f, 0.65f, 0.4f);
								}
								else {
									if (p->HasPlayerFlag(PlayerFlags::Wounded) || p->HasPlayerFlag(PlayerFlags::Sleeping)) {
										bone.center = p->bones()->pelvis->position;
										bone.extents = Vector3(0.9f, 0.2f, 0.4f);
									}
									else {
										bone.center = p->midPoint() + Vector3(0.0f, 0.85f, 0.0f);
										bone.extents = Vector3(0.4f, 0.9f, 0.4f);
									}
								}

								float y = math::euler_angles(p->bones()->eye_rot).y;
								Vector3 center = bone.center;
								Vector3 extents = bone.extents;
								Vector3 frontTopLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z - extents.z), y);
								Vector3 frontTopRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z - extents.z), y);
								Vector3 frontBottomLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z - extents.z), y);
								Vector3 frontBottomRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z - extents.z), y);
								Vector3 backTopLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z + extents.z), y);
								Vector3 backTopRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z + extents.z), y);
								Vector3 backBottomLeft = math::rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z + extents.z), y);
								Vector3 backBottomRight = math::rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z + extents.z), y);

								Vector2 frontTopLeft_2d, frontTopRight_2d, frontBottomLeft_2d, frontBottomRight_2d, backTopLeft_2d, backTopRight_2d, backBottomLeft_2d, backBottomRight_2d;
								if (Camera::world_to_screen(frontTopLeft, frontTopLeft_2d) &&
									Camera::world_to_screen(frontTopRight, frontTopRight_2d) &&
									Camera::world_to_screen(frontBottomLeft, frontBottomLeft_2d) &&
									Camera::world_to_screen(frontBottomRight, frontBottomRight_2d) &&
									Camera::world_to_screen(backTopLeft, backTopLeft_2d) &&
									Camera::world_to_screen(backTopRight, backTopRight_2d) &&
									Camera::world_to_screen(backBottomLeft, backBottomLeft_2d) &&
									Camera::world_to_screen(backBottomRight, backBottomRight_2d)) {

									Renderer::line(frontTopLeft_2d, frontTopRight_2d, box_col, true, 1.5f);
									Renderer::line(frontTopRight_2d, frontBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomRight_2d, frontBottomLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomLeft_2d, frontTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(backTopLeft_2d, backTopRight_2d, box_col, true, 1.5f);
									Renderer::line(backTopRight_2d, backBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(backBottomRight_2d, backBottomLeft_2d, box_col, true, 1.5f);
									Renderer::line(backBottomLeft_2d, backTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontTopLeft_2d, backTopLeft_2d, box_col, true, 1.5f);
									Renderer::line(frontTopRight_2d, backTopRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomRight_2d, backBottomRight_2d, box_col, true, 1.5f);
									Renderer::line(frontBottomLeft_2d, backBottomLeft_2d, box_col, true, 1.5f);
								}
								break;
							}
						}

						if (!p->is_teammate() && !p->HasPlayerFlag(PlayerFlags::Sleeping)) {
							if (dfc(p) < combot::fovaim) {
								if (target_ply == nullptr)
									target_ply = p;
								else
									if (dfc(target_ply) > dfc(p))
										target_ply = p;
							}
						}
					}
				}

				if (entity->class_name_hash() == STATIC_CRC32("OreResourceEntity") && resource::ores) {
					if (entity->ShortPrefabName_hash() == STATIC_CRC32("stone-ore") ||
						entity->ShortPrefabName_hash() == STATIC_CRC32("sulfur-ore") ||
						entity->ShortPrefabName_hash() == STATIC_CRC32("metal-ore")) {
						Vector2 screen;
						if (Camera::world_to_screen(entity->transform()->position(), screen)) {
							switch (entity->ShortPrefabName_hash()) {
							case STATIC_CRC32("stone-ore"):
								Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Stone Ore"));
								break;
							case STATIC_CRC32("sulfur-ore"):
								Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Sulfur Ore"));
								break;
							case STATIC_CRC32("metal-ore"):
								Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Metal Ore"));
								break;
							}
						}
					}
				}
				if (entity->class_name_hash() == STATIC_CRC32("BaseHelicopter")) {
					BaseEntity* helicopter = (BaseEntity*)entity;
					if (helicopter) {
						float health = reinterpret_cast<BaseCombatEntity*>(entity)->health();
						float maxhealth = 10000.f;
						Vector3 pos = helicopter->transform()->position();
						Vector2 screenPos;
						auto screen = reinterpret_cast<Camera*>(entity);
						if (screen->world_to_screen(pos, screenPos)) {
							if (MiscVisual::patrol_heli) {
								Renderer::t(screenPos, D2D1::ColorF(0.5f, 0.54f, 1.f), true, true, (L"Patrol Helicopter [%.1fm]"), Calc3D_Dist(LocalPlayer::Entity()->get_bone_pos(head), pos));
								Renderer::t(screenPos + Vector2(0, 15), D2D1::ColorF(0.5f, 0.54f, 1.f), true, true, (L"[%dHP]"), (int)health);

							}
						}
					}
				}
				if (entity->ShortPrefabName_hash() == STATIC_CRC32("small_stash_deployed") && resource::stash) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						if (entity->HasFlag(BaseEntity::Flags::Reserved5)) {
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Stash (Hidden)"));
						}
						else {
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Stash"));
						}
					}

				}
				if ((entity->ShortPrefabName_hash() == STATIC_CRC32("rhib") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("rowboat") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("hotairballoon") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("minicopter.entity") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("scraptransporthelicopter") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("submarineduo.entity") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("submarinesolo.entity") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("patrolhelicopter.prefab") ||
					entity->ShortPrefabName_hash() == STATIC_CRC32("testridablehorse")) && resource::vehicles) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						switch (entity->ShortPrefabName_hash()) {
						case STATIC_CRC32("rhib"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"RHIB"));
							break;
						case STATIC_CRC32("rowboat"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Boat"));
							break;
						case STATIC_CRC32("hotairballoon"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Hot Air Balloon"));
							break;
						case STATIC_CRC32("minicopter.entity"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Minicopter"));
							break;
						case STATIC_CRC32("scraptransporthelicopter"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Scrap Heli"));
							break;
						case STATIC_CRC32("testridablehorse"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Horse"));
							break;
						case STATIC_CRC32("submarineduo.entity"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Submarine (Duo)"));
							break;
						case STATIC_CRC32("submarinesolo.entity"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Submarine (Solo)"));
							break;
						case STATIC_CRC32("patrolhelicopter"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"PatrolHeli"));
							break;
						}
					}
				}
				if (entity->ShortPrefabName_hash() == STATIC_CRC32("hemp-collectable") && resource::hemp) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Hemp"));
					}

				}
				if ((entity->class_name_hash() == STATIC_CRC32("PlayerCorpse") ||
					entity->class_name_hash() == STATIC_CRC32("NPCPlayerCorpse")) && resource::corpses) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Corpse"));
					}
				}
				if ((entity->class_name_hash() == STATIC_CRC32("AutoTurret") ||
					entity->class_name_hash() == STATIC_CRC32("Landmine") ||
					entity->class_name_hash() == STATIC_CRC32("BearTrap") ||
					entity->class_name_hash() == STATIC_CRC32("SamSite") ||
					entity->class_name_hash() == STATIC_CRC32("GunTrap")) && resource::traps) {
					Vector2 screen;
					if (Camera::world_to_screen(entity->transform()->position(), screen)) {
						switch (entity->class_name_hash()) {
						case STATIC_CRC32("AutoTurret"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Auto Turret"));
							break;
						case STATIC_CRC32("Landmine"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Landmine"));
							break;
						case STATIC_CRC32("BearTrap"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Bear Trap"));
							break;
						case STATIC_CRC32("SamSite"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"SAM Site"));
							break;
						case STATIC_CRC32("GunTrap"):
							Renderer::t(screen, Color3(255, 0, 0), 12.f, true, true, wxorstr_(L"Shotgun Trap"));
							break;
						}
					}
				}
			}
		}
		else {
			if (target_ply != nullptr)
				target_ply = nullptr;
		}
	}
}

	/*static bool alreadystartedReload = false;

	static bool alreadyReset = false;
	if (Settings::Combat::Rage::Desync)
	{
		alreadyReset = false;
		static int updownleft = 0;
		static bool waspressed = false;
		if (Globals::PressedKeys[VK_DELETE])
		{
			waspressed = true;
		}

		if (!Globals::PressedKeys[VK_DELETE] && waspressed)
		{
			waspressed = false;
			updownleft += 1;
			if (updownleft > 3)
			{
				updownleft = 0;
			}
		}

		if (Globals::PressedKeys[Settings::Combat::Rage::DesyncKey])
		{
			float desyncTime = (UnityEngine::get_realtimeSinceStartup() - LocalPlayer.lastSentTickTime()) - 0.03125 * 3;
			float mm_max_eye = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * 5.5f) - 0.05f;
			LocalPlayer.clientTickInterval() = 1.f;

			if (updownleft == 0)
			{
				LocalPlayer.Eyes.viewOffset().y = 1.5f;
			}
			else if (updownleft == 1)
			{
				LocalPlayer.Eyes.viewOffset().x = mm_max_eye;
			}
			else if (updownleft == 2)
			{
				LocalPlayer.Eyes.viewOffset().x = -mm_max_eye;
			}
			else if (updownleft == 3)
			{
				LocalPlayer.Eyes.viewOffset().z = -mm_max_eye;
			}

			auto desynctime = UnityEngine::get_realtimeSinceStartup() - LocalPlayer.lastSentTickTime();
			auto desyncpercentage = ((desynctime / 0.99f) * 100.0f) + 1.f;
			static int shotCount = 0;
			if (desyncpercentage < 100 && Settings::Combat::Rage::InstantShoot && LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).Base_ != 0)
			{
				if (Settings::Combat::Rage::PowerShot && desyncpercentage > 90)
				{
					if (!LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.HasReloadCooldown() && LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents() == 0)
					{
						LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.reloadPressTime() = 0.1f;
					}
					if (shotCount < (int)(desynctime / LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.repeatDelay()))
					{
						if (LightFindTarget(Settings::Combat::Silent::SilentFOV).Base_ != 0)
						{
							if (LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents() != 0)
							{
								shotCount++;
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.LaunchProjectile();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents()--;
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.UpdateAmmoDisplay();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.ShotFired();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.DidAttackClientside();
							}
						}
					}
				}
				else if (!Settings::Combat::Rage::PowerShot && desyncpercentage > 5)
				{
					if (!LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.HasReloadCooldown() && LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents() == 0)
					{
						LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.reloadPressTime() = 0.1f;
					}
					if (shotCount < (int)(desynctime / LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.repeatDelay()))
					{
						if (LightFindTarget(Settings::Combat::Silent::SilentFOV).Base_ != 0)
						{
							if (LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents() != 0)
							{
								shotCount++;
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.LaunchProjectile();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.primaryMagazine().contents()--;
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.UpdateAmmoDisplay();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.ShotFired();
								LocalPlayer.Inventory.ContainerBelt.FindActiveItem(LocalPlayer.clActiveItem()).baseProjectile.DidAttackClientside();
							}
						}
					}
				}
			}
			else
			{
				shotCount = 0;
			}
		}
		else
		{
			LocalPlayer.Eyes.viewOffset() = Vector3().Zero() + Vector3(0, LocalPlayer.Eyes.viewOffset().y, 0);
			LocalPlayer.clientTickInterval() = 0.05f;
		}
	}
	else if (!alreadyReset)
	{
		LocalPlayer.Eyes.viewOffset() = Vector3().Zero();
		alreadyReset = true;
	}
}*/