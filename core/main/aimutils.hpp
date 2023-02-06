namespace aimutils {
	double get_bullet_drop(double height, double aaaa, float speed, float gravity) {
		double pitch = std::atan2(height, aaaa);
		double vel_double = speed * std::cos(pitch);
		double t = aaaa / vel_double;
		double y = (0.4905f * gravity * t * t);
		return y * 10;
	}
	Vector3 get_prediction() {
		auto mpv = target_ply->find_mpv_bone();
		Vector3 target;
		if (mpv != nullptr)
			target = mpv->position;
		else
			target = target_ply->bones()->head->position;

		Vector3 targetvel = target_ply->playerModel()->newVelocity();

		auto base_projectile = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
		if (base_projectile == nullptr)
			return target;

		static Type* type = Type::GetType(xorstr_("ItemModProjectile, Assembly-CSharp"));
		if (type == nullptr)
			return target;

		auto mag = base_projectile->primaryMagazine();
		if (mag == nullptr)
			return target;

		auto ammo = mag->ammoType();
		if (ammo == nullptr)
			return target;

		auto itemModProjectile = ammo->GetComponent<ItemModProjectile>(type); // 0x3189118 for getting Projectile* ref
		if (itemModProjectile == nullptr)
			return target;

		float bullet_speed = (itemModProjectile->GetRandomVelocity() * (combot::fast_bullets ? 1.4f : 1.f)) * base_projectile->projectileVelocityScale();

		if (base_projectile->class_name_hash() == STATIC_CRC32("CompoundBowWeapon"))
			bullet_speed = (itemModProjectile->GetRandomVelocity() * (combot::fast_bullets ? 1.4f : 1.f)) * reinterpret_cast<CompoundBowWeapon*>(base_projectile)->GetProjectileVelocityScale();

		if (bullet_speed == 0.f)
			return target;

		Projectile* projectile = itemModProjectile->projectileObject()->Get()->GetComponent<Projectile>(Type::Projectile());

		if (projectile == nullptr)
			return target;

		float distance = target.distance(LocalPlayer::Entity()->eyes()->position());
		float travel_time = distance / bullet_speed;
		Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.75f;
		Vector3 predicted_velocity = vel * travel_time;

		target.x += predicted_velocity.x;
		target.z += predicted_velocity.z;
		double height = target.y - LocalPlayer::Entity()->eyes()->position().y;
		Vector3 dir = target - LocalPlayer::Entity()->eyes()->position();
		float astronaut = sqrt((dir.x * dir.x) + (dir.z * dir.z));
		float drop = get_bullet_drop(height, astronaut, bullet_speed, projectile->gravityModifier());


		target.y += drop;

		

		if (combot::testHiter)
		{
			//projectile->initialDistance() = target.distance(LocalPlayer::Entity()->get_bone_pos(head)) - 68.5f;
			projectile->initialDistance() = 68.5f;
		}

		return target;
	}
}
int ScreenHeight = 0;
int ScreenWidth = 0;
	float Distance_2D(Vector2 a, Vector2 b) {
		return sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
	}
	static bool world_to_screen(Vector3 world, Vector2& screen) {
		const auto matrix = viewMatrix.transpose();

		const Vector3 translation = { matrix[3][0], matrix[3][1], matrix[3][2] };
		const Vector3 up = { matrix[1][0], matrix[1][1], matrix[1][2] };
		const Vector3 right = { matrix[0][0], matrix[0][1], matrix[0][2] };

		const auto w = translation.dot_product(world) + matrix[3][3];

		if (w < 0.1f)
			return false;

		const auto x = right.dot_product(world) + matrix[0][3];
		const auto y = up.dot_product(world) + matrix[1][3];

		screen =
		{
			screen_center.x * (1.f + x / w),
			screen_center.y * (1.f - y / w)
		};

		return true;
	}


float GetFovHeli(Vector3 pos) {
	Vector2 ScreenPos;
	if (!world_to_screen(pos, ScreenPos)) return 1000.f;
	return Distance_2D(Vector2(screen_center.x / 2, screen_center.y / 2), ScreenPos);
}
void Normalize(float& Yaw, float& Pitch) {
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}
void StepConstant(Vector2& angles) {
	bool smooth = combot::smooth;
	Vector2 angles_step = angles - LocalPlayer::Entity()->viewangles();
	Normalize(angles_step.x, angles_step.y);

	if (smooth) {
		float factor_pitch = (combot::smooth_factor / 10.f);
		if (angles_step.x < 0.f) {
			if (factor_pitch > std::abs(angles_step.x)) {
				factor_pitch = std::abs(angles_step.x);
			}
			angles.x = LocalPlayer::Entity()->viewangles().x - factor_pitch;
		}
		else {
			if (factor_pitch > angles_step.x) {
				factor_pitch = angles_step.x;
			}
			angles.x = LocalPlayer::Entity()->viewangles().x + factor_pitch;
		}
	}
	if (smooth) {
		float factor_yaw = (combot::smooth_factor / 10.f);
		if (angles_step.y < 0.f) {
			if (factor_yaw > std::abs(angles_step.y)) {
				factor_yaw = std::abs(angles_step.y);
			}
			angles.y = LocalPlayer::Entity()->viewangles().y - factor_yaw;
		}
		else {
			if (factor_yaw > angles_step.y) {
				factor_yaw = angles_step.y;
			}
			angles.y = LocalPlayer::Entity()->viewangles().y + factor_yaw;
		}
	}
}
#define M_PI 3.14159265358979323846f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
__forceinline Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst) {
	Vector3 dir = Src - Dst;
	return Vector2{ RAD2DEG(std::asin(dir.y / dir.length())), RAD2DEG(-std::atan2(dir.x, -dir.z)) };
}
void do_aimbot(BasePlayer* player) {
	if (!LocalPlayer::Entity() || !player) { return; }
	if (!LocalPlayer::Entity()->clActiveItem() || player->IsDestroyed()) { return; }
	Vector3 local =  LocalPlayer::Entity()->get_bone_pos(head);
	Vector3 target = player->get_bone_pos(head);
	(aimutils::get_prediction() - LocalPlayer::Entity()->eyes()->position()).normalized();
	Vector2 Offset = CalcAngle(local, target) - LocalPlayer::Entity()->viewangles();
	Normalize(Offset.y, Offset.x);
	Vector2 AngleToAim = LocalPlayer::Entity()->viewangles() + Offset;
	if (combot::smooth) {
		StepConstant(AngleToAim);
	}
	Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer::Entity()->set_viewangles(AngleToAim);
}
