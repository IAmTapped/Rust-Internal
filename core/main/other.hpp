#define max(a,b)            (((a) > (b)) ? (a) : (b))
inline BasePlayer LocalPlayer;
float Max(float a, float b)
{
	return (a <= b) ? b : a;
}
_NODISCARD _Check_return_ inline float Abs(_In_ float _Xx) noexcept /* strengthened */ {
	return _CSTD fabsf(_Xx);
}
float Magnitude2d()
{
	return sqrt(x * x + z * z);
	//return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

inline BasePlayer justaNigger;

namespace other {
	Vector3 m_manipulate = Vector3::Zero();


	void find_manipulate_angle() {
		auto loco = LocalPlayer::Entity();
		Vector3 re_p = loco->transform()->position() + loco->transform()->up() * (PlayerEyes::EyeOffset().y + loco->eyes()->viewOffset().y);
		// real eye pos

		Vector3 choice = Vector3::Zero();

		/*if (LineOfSight(re_p, target_ply->find_mpv_bone()->position) || !target_ply->isCached()) {
			m_manipulate = Vector3::Zero();
			return;
		}*/


		float desyncTime = (Time::realtimeSinceStartup() - loco->lastSentTickTime()) - 0.03125 * 1;
		float mm_max_eye = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * loco->MaxVelocity()) - 0.05f;
		auto right = loco->eyes()->MovementRight();
		auto forward = loco->eyes()->MovementForward();

		//LocalPlayer::Entity()->eyes()->viewOffset(), 1.5f;

		std::array<Vector3, 8> arr = {
			Vector3(right.x * (mm_max_eye / 2), 1.3, right.z * (mm_max_eye / 2)), // small right
			Vector3(right.x * mm_max_eye, 1.5, right.z * mm_max_eye), // big right

			Vector3(-(right.x * (mm_max_eye / 2)), 1.4, -(right.z * (mm_max_eye / 2))), // small left
			Vector3(-(right.x * mm_max_eye), 1.5, -(right.z * mm_max_eye)), // big left

			Vector3(1.4, (mm_max_eye / 2), 1.4), // small up
			//Vector3(1.5f, (mm_max_eye * 2), 1.4f), // big up

			//change
			Vector3(1.5, mm_max_eye, 0),

			                                      //1.4
			Vector3(forward.x * (mm_max_eye / 2), 1.4, forward.z * (mm_max_eye / 2)), // small forward
			Vector3(forward.x * mm_max_eye, 1.5, forward.z * mm_max_eye), // big forward
		};                                  //1.5
		for (auto s : arr) {
			Vector3 point = re_p + s;

			if (!LineOfSight(point, re_p))
				continue;

			if (!target_ply->bones()->head->visible_(point))
				continue;

			choice = s;
			break;
		}
		if (choice.empty()) {
			m_manipulate = Vector3::Zero();
			return;
		}

		m_manipulate = choice;
	}
	void test_bundle(AssetBundle* bundle) {
		if (!bundle) {
			std::cout << "bundle nfound\n";
			return;
		}

		auto arr = bundle->GetAllAssetNames();
		if (!arr) {
			std::cout << "arr nfound\n";
			return;
		}

		for (int j = 0; j < arr->size(); j++) {
			auto name = arr->get(j);

			printf("%ls\n", name->buffer);
		}

		std::cout << "bundletest - success\n";
	}
};
uintptr_t m_pMovement = 0;
bool stop = false;
bool go_back = false, go_forward = false, go_up = false;
bool nigger = false;

inline Vector3 quatmult(const Vector3* point, Vector4* quat)
{
	float num = quat->x * 2.f;
	float num2 = quat->y * 2.f;
	float num3 = quat->z * 2.f;
	float num4 = quat->x * num;
	float num5 = quat->y * num2;
	float num6 = quat->z * num3;
	float num7 = quat->x * num2;
	float num8 = quat->x * num3;
	float num9 = quat->y * num3;
	float num10 = quat->w * num;
	float num11 = quat->w * num2;
	float num12 = quat->w * num3;
	Vector3 result;
	result.x = (1.f - (num5 + num6)) * point->x + (num7 - num12) * point->y + (num8 + num11) * point->z;
	result.y = (num7 + num12) * point->x + (1.f - (num4 + num6)) * point->y + (num9 - num10) * point->z;
	result.z = (num8 - num11) * point->x + (num9 + num10) * point->y + (1.f - (num4 + num5)) * point->z;
	return result;
}
const Vector3 vecright = { 1.f, 0.f, 0.f };
const Vector3 vecforward = { 0.f, 0.f, 1.f };
const Vector3 vecup = { 0.f, 1.f, 0.f };

void flyhack()
{
	while (true)
	{
		uintptr_t m_pLocalPlayer = (uintptr_t)LocalPlayer::Entity();
		if (!m_pLocalPlayer)
		{
			Sleep(400);
			continue;
		}

		uintptr_t m_pEyes = mem::read<uintptr_t>((uintptr_t)LocalPlayer::Entity() + 0x600);
		if (!m_pEyes)
		{
			Sleep(400);
			continue;
		}
		m_pMovement = mem::read<uintptr_t>((uintptr_t)LocalPlayer::Entity() + 0x4E8);
		if (!m_pMovement)
		{
			Sleep(400);
			continue;
		}

		if ((combot::bfly && GetAsyncKeyState(combot::fly)) || (combot::bclimb && GetAsyncKeyState(combot::assist)))
		{
			//mist::misc->write<uint8_t> ( m_pMovement + 0x13C, 1 ); // flying BOOL
			ModelState* model_state = (ModelState*)mem::read<uintptr_t>(reinterpret_cast<uintptr_t>(LocalPlayer::Entity()) + 0x5F8);
			mem::write<float>((uintptr_t)model_state + 0x14, 1.f);
			mem::write<float>(m_pMovement + 0x64, -999.f); // noclip ;)
			mem::write<float>(m_pMovement + 0x6C, -999.f);

			Vector3 targetmovement{};
			PlayerEyes* eyes = (PlayerEyes*)m_pEyes;

			Vector4 rotation = mem::read<Vector4>(m_pEyes + 0x44);

			LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::Sprinting);

			if (GetAsyncKeyState('W') || go_forward)
				targetmovement += quatmult(&vecforward, &rotation);
			if (GetAsyncKeyState('S') || go_back)
				targetmovement -= quatmult(&vecforward, &rotation);
			if (GetAsyncKeyState('A'))
				targetmovement -= quatmult(&vecright, &rotation);
			if (GetAsyncKeyState('D'))
				targetmovement += quatmult(&vecright, &rotation);

			if (GetAsyncKeyState(VK_SPACE) || go_up)
				targetmovement.y += 1.f;

			float d = 5.f;
			if (GetAsyncKeyState(VK_CONTROL))
				d = 2.5f;
			if (GetAsyncKeyState(VK_SHIFT))
				d = 10.f;

			if (!(targetmovement == Vector3{ 0.f, 0.f, 0.f }))
				mem::write<Vector3>(m_pMovement + 0x34, targetmovement * d);//private Vector3 <TargetMovement>k__BackingField; // 0x34

			LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::Sprinting);

			nigger = true;
		}
		else
		{
			if (nigger)
			{
				mem::write<float>(m_pMovement + 0x64, 1.f);
				mem::write<float>(m_pMovement + 0x6C, 0.55f);
				//mist::misc->write<float> ( m_pMovement + 0x74, 2.5f );
				nigger = false;
			}
		}
		mem::write<bool>(m_pMovement + 0x18, 0);
		Sleep(1);
	}
}
Vector3 GetEntityPosition(std::uint64_t entity) {
	if (!entity) return Vector3::Zero();

	uintptr_t plyVis = mem::read<uintptr_t>(entity + 0x8);
	if (!plyVis) return Vector3::Zero();

	uintptr_t visualState = mem::read<uintptr_t>(plyVis + 0x38);
	if (!visualState) return Vector3::Zero();

	Vector3 ret = mem::read<Vector3>(visualState + 0x90);
	return ret;
}

class f_object {
public:
	bool valid;
	DWORD64 entity;
	Vector3 position;
	float dist = 10000.f;
	bool visible = false;

	bool operator<(const f_object& b) { return this->dist < b.dist; }
	bool operator>(const f_object& b) { return this->dist > b.dist; }
	bool operator<=(const f_object& b) { return this->dist <= b.dist; }
	bool operator>=(const f_object& b) { return this->dist >= b.dist; }
	f_object() {
		this->valid = false;
	}
	f_object(Vector3 target) {
		this->valid = false;
		this->position = target;
	}

	static f_object get_closest_object(Vector3 from, const char* namee, Vector3 ignore = Vector3::Zero(), Vector3 ignore2 = Vector3::Zero(), Vector3 ignore3 = Vector3::Zero(), bool classname = false, const char* classnamee = xorstr_(""), float get_dist = 99999.f, bool vis = false, bool y = false) {
		f_object lowest = f_object();

		auto entityList = BaseNetworkable::clientEntities()->entityList();
		if (entityList) {
			for (int i = 1; i < entityList->vals->size; i++) {
				uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!Entity) continue;
				uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
				if (!Object) continue;
				uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
				if (!ObjectClass) continue;
				pUncStr name = mem::read<pUncStr>(ObjectClass + 0x60); if (!name) continue;
				char* buff = name->stub;
				f_object res = f_object();
				if (classname) {
					if (strstr((char*)mem::read<DWORD64>(mem::read<DWORD64>(mem::read<DWORD64>(Object + 0x28)) + 0x10), classnamee)) {
						uintptr_t a = mem::read<UINT64>(ObjectClass + 0x30);
						float dist = Calc3D_Dist(GetEntityPosition(a), from);
						if (GetEntityPosition(a) != ignore && GetEntityPosition(a) != ignore2 && GetEntityPosition(a) != ignore3) {
							res.valid = dist <= get_dist;
							res.dist = dist;
							res.entity = Entity;
							res.position = GetEntityPosition(a);
							if (res < lowest) lowest = res;
						}
					}
				}
				else {
					if (strstr(buff, namee)) {
						uintptr_t a = mem::read<UINT64>(ObjectClass + 0x30);
						float dist = Calc3D_Dist(GetEntityPosition(a), from);
						if (GetEntityPosition(a) != ignore && GetEntityPosition(a) != ignore2 && GetEntityPosition(a) != ignore3) {
							if (vis ? LineOfSight(GetEntityPosition(a), from) : true) {
								if (y) {
									if (GetEntityPosition(a).y > 0) {
										res.valid = dist <= get_dist;
										res.dist = dist;
										res.entity = Entity;
										res.position = GetEntityPosition(a);
										if (res < lowest) lowest = res;
									}
								}
								else {
									res.valid = dist <= get_dist;
									res.dist = dist;
									res.entity = Entity;
									res.position = GetEntityPosition(a);
									if (res < lowest) lowest = res;
								}
							}
						}
					}
				}
			}
			return lowest;
		}
	}

	static float MaxMeleeDist(DWORD64 melee, bool localplayer) {
		float pad = 0.1f;
		typedef float(__stdcall* RetF)();
		float time = Time::time();

		float desyncTime = max(time - LocalPlayer::Entity()->lastSentTickTime() - 0.0325f, 0.f);
		float res = pad + desyncTime * 5.5f;
		if (localplayer) {
			res += (1.5f * mem::read<float>(melee + 0x278)); //maxDistance
		}
		return max(res, 0.05f);
	}

	static f_object get_melee_target(BasePlayer* Player, DWORD64 melee) {
		f_object res = f_object();

		if (Player->health() <= 0) return res;
		Vector3 prepos = Player->get_bone_pos(head);
		Vector3 closest_entity = ClosestPoint(LocalPlayer::Entity(), prepos);
		Vector3 closest_local = ClosestPoint(Player, closest_entity);
		float disttoentity = MaxMeleeDist(melee, false);
		float distfromlocal = MaxMeleeDist(melee, true);

		float maxdist = distfromlocal + disttoentity;

		float realdist = (closest_local - closest_entity).length();

		if (realdist > maxdist) return res;

		Vector3 target;
		if (realdist <= disttoentity) {
			target = closest_entity;
		}
		else {
			Vector3 dir = (closest_entity - closest_local).normalized();
			target = closest_local + dir * disttoentity;
		}
		res.valid = LineOfSight(closest_local, target);
		if (!res.valid) return res;

		Vector3 lppos = LocalPlayer::Entity()->get_bone_pos(neck);
		res.valid = LineOfSight(lppos, target);
		if (!res.valid) return res;

		res.dist = realdist;
		res.entity = (DWORD64)Player;
		res.position = target;
		return res;
	}
};


namespace lol
{
	void do_attack(f_object target, uintptr_t Held, bool transform) {
		if (!target.valid || !Held) return;

		if (mem::read<float>(Held + 0x238) >= Time::time()) { return; }
		if (mem::read<float>(Held + 0x244) < mem::read<float>(Held + 0x1E0)) { return; }


		uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity()->get_bone_pos(neck), (target.position - LocalPlayer::Entity()->get_bone_pos(neck)).normalized());
		if (!target.entity) return;
		if (transform) {
			trans = reinterpret_cast<BasePlayer*>(target.entity)->mono_transform(head);
		}
		else {
			trans = (DWORD64)reinterpret_cast<Object*>(target.entity)->transform();
		} if (!trans) {

			return;
		}

		auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
	}
}
void auto_farm_loop(bool weaponmelee, uintptr_t active) {

	if (combot::auto_farm_ore) {
		if (weaponmelee) {
			f_object ore_hot_spot = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
				xorstr_(""),
				Vector3::Zero(),
				Vector3::Zero(),
				Vector3::Zero(),
				true,
				xorstr_("OreHotSpot"));
			if (ore_hot_spot.valid) {
				Vector3 local = ClosestPoint(LocalPlayer::Entity(), ore_hot_spot.position);
				if (Calc3D_Dist(local, ore_hot_spot.position) <= 2.f) {
					if (reinterpret_cast<BaseEntity*>(ore_hot_spot.entity)->IsValid()) {
						lol::do_attack(ore_hot_spot, active, false);
					}
				}
			}
		}
	}
	if (combot::auto_farm_tree) {
		if (weaponmelee) {
			f_object tree_entity = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
				xorstr_(""),
				Vector3::Zero(),
				Vector3::Zero(),
				Vector3::Zero(),
				true,
				xorstr_("TreeEntity"));
			if (tree_entity.valid) {
				tree_entity.position = Vector3::Zero();
				f_object tree_marker = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
					xorstr_(""),
					Vector3::Zero(),
					Vector3::Zero(),
					Vector3::Zero(),
					true,
					xorstr_("TreeMarker"));
				if (tree_marker.valid) {
					Vector3 locala = ClosestPoint(LocalPlayer::Entity(), tree_marker.position);
					if (Calc3D_Dist(locala, tree_marker.position) <= 2.f) {
						tree_entity.position = tree_marker.position;
						Vector3 local = ClosestPoint(LocalPlayer::Entity(), tree_entity.position);
						if (Calc3D_Dist(local, tree_entity.position) <= 2.f) {
							if (reinterpret_cast<BaseEntity*>(tree_entity.entity)->IsValid()) {
								lol::do_attack(tree_entity, active, false);
							}
						}
					}
				}
			}
		}
	}
}

float flyhackDistanceVertical = 0.f;
float flyhackDistanceHorizontal = 0.f;
float flyhackPauseTime;
void TestFlying() {

	flyhackPauseTime = Max(0.f, flyhackPauseTime - Time::deltaTime());
	bool inAir = false;
	//float radius = reinterpret_cast<float(*)(BasePlayer*)>(game_assembly + CO::GetRadius)(LocalPlayer::Entity());
	//float height = reinterpret_cast<float(*)(BasePlayer*, bool)>(game_assembly + CO::GetHeight)(LocalPlayer::Entity(), false);
	//float radius = LocalPlayer.GetRadius();
	//float height = LocalPlayer.GetHeight();
	Vector3 vector = (LocalPlayer::Entity()->lastSentTick()->position() + mem::read<Vector3>(mem::read<uintptr_t>(LocalPlayer::Entity() + 0x4C0) + 0x4B4)) * 0.5f;
	//Vector3 vector2 = vector + Vector3(0.f, radius - 2.f, 0.f);
	//Vector3 vector3 = vector + Vector3(0.f, height - radius, 0.f);
	//float radius2 = radius - 0.05f;
	//bool a = Physics::CheckCapsule(vector2, vector3, radius2, 1503731969, QueryTriggerInteraction::Ignore);
	//inAir = !a;
	if (inAir) {
		bool flag = false;

		Vector3 vector4 = mem::read<Vector3>(mem::read<uintptr_t>(LocalPlayer::Entity() + 0x4C0) + 0x4B4) - LocalPlayer::Entity()->lastSentTick()->position();
		float num3 = Abs(vector4.y);
		//float num4 = Magnitude2d(vector4);
		if (vector4.y >= 0.f) {
			flyhackDistanceVertical += vector4.y;
			flag = true;
		}
		/*	if (num3 < num4) {
				flyhackDistanceHorizontal += num4;
				flag = true;
			}*/
		if (flag) {
			float num5 = Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num6 = LocalPlayer::Entity()->GetJumpHeight() + num5;
			if (flyhackDistanceVertical > num6) {
				//return true;
			}
			float num7 = Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num8 = 5.f + num7;
			if (flyhackDistanceHorizontal > num8) {
				//return true;
			}
		}
	}
	else {
		flyhackDistanceHorizontal = 0.f;
		flyhackDistanceVertical = 0.f;
	}
}
void CheckFlyhack() {
	float num5 = Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num6 = LocalPlayer::Entity()->GetJumpHeight() + num5;
	stuff::max_flyhack = num6;
	if (flyhackDistanceVertical <= num6) {
		stuff::flyhack = flyhackDistanceVertical;
	}

	float num7 = Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num8 = 5.f + num7;
	stuff::max_hor_flyhack = num8;
	if (flyhackDistanceHorizontal <= num8) {
		stuff::hor_flyhack = flyhackDistanceHorizontal;
	}
}
