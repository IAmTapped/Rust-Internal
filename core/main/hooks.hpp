#include <intrin.h>
#include <mem.h>
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)
typedef void(__fastcall* sendclienttick)(BasePlayer*);
inline sendclienttick original_sendclienttick{ };
int yeet = 0;
int flick = 0;
int yaw = 100;

void ClientUpdate_hk(BasePlayer* player) {
	auto local = LocalPlayer::Entity();
	if (local) {
		if (player::enable || player::sleepers) {
			bonecache::cachePlayer(player);
		}
	}
	return player->ClientUpdate();
}

void ClientUpdate_Sleeping_hk(BasePlayer* player)
{
	auto local = LocalPlayer::Entity();
	if (local) {
		if (player::enable || player::sleepers) {
			bonecache::cachePlayer(player);
		}
	}

	return player->ClientUpdate_Sleeping();
}

Vector3 GetModifiedAimConeDirection_hk(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
	if (combot::Perfect_Silent_player && target_ply != nullptr && target_ply->isCached()) {
		inputVec = (aimutils::get_prediction() - LocalPlayer::Entity()->eyes()->position()).normalized();
	}

	aimCone *= combot::spead_control / 100.0f;

	return AimConeUtil::GetModifiedAimConeDirection(aimCone, inputVec, anywhereInside);
}
float Distance_3D2(Vector3 a, Vector3 b) {
	return sqrtf(powf((a.x - b.x), 2) + powf((a.y - b.y), 2) + powf((a.z - b.z), 2));
}
Attack* BuildAttackMessage_hk(HitTest* self, PlayerProjectileAttack* a2) {
	auto ret = self->BuildAttackMessage();
	auto entity = BaseNetworkable::clientEntities()->Find<BasePlayer*>(ret->hitID());
	if (combot::always_heli_weakspot)
	{
		if (entity->class_name_hash() == STATIC_CRC32("BaseHelicopter"))
		{
			if (entity->health() <= 5000.0f)
				ret->hitBone() = StringPool::Get(xorstr_("tail_rotor_col"));
			else
				ret->hitBone() = StringPool::Get(xorstr_("engine_col"));
		}
	}
	if (combot::spoof_hitdistance) {
		//write(a2 + 0x2C, combot::hitdistance, float);
		Vector3 target;
		//Projectile::initialDistance() = localPlayer.distance(Bone_List::head) - 68.5f;
	}
	auto localPlayer = LocalPlayer::Entity();
	if (localPlayer) {
		if (reinterpret_cast<BasePlayer*>(self->ignoreEntity())->userID() == localPlayer->userID()) { // isAuthoritative
			if (combot::bullet_tracers) {

				DDraw::Line(localPlayer->eyes()->position(), ret->hitPositionWorld(), Color(1, 0, 0, 1), 2.f, false, true);
			}

			if (entity) {
				if (entity->IsPlayer()) {
					if (entity->isCached()) {
						if (localPlayer->isCached()) {
							// trajectory_end = 1 meter
							// player_distance = 0.2 meter
							// profit $$$$$$

							if (combot::big_bullets) {
								auto bone = entity->model()->find_bone(ret->hitPositionWorld());
								if (bone.second) {
									ret->hitPositionWorld() = bone.first->position();
								}
							}
							if (combot::targetbone != 0) {
								if (combot::targetbone == 1)
									ret->hitBone() = StringPool::Get(xorstr_("spine4"));
								else if (combot::targetbone == 2)
									ret->hitBone() = StringPool::Get(xorstr_("head"));
								else if (combot::targetbone == 3) {

									// yandere dev in this bitch
									int num = rand() % 100;
									if (num > 90)
										ret->hitBone() = StringPool::Get(xorstr_("head"));
									else if (num < 90 && num > 80)
										ret->hitBone() = StringPool::Get(xorstr_("neck"));
									else if (num < 80 && num > 70)
										ret->hitBone() = StringPool::Get(xorstr_("l_clavicle"));
									else if (num < 70 && num > 60)
										ret->hitBone() = StringPool::Get(xorstr_("pelvis"));
									else if (num < 60 && num > 50)
										ret->hitBone() = StringPool::Get(xorstr_("r_hip"));
									else if (num < 50 && num > 40)
										ret->hitBone() = StringPool::Get(xorstr_("r_foot"));
									else if (num < 40 && num > 30)
										ret->hitBone() = StringPool::Get(xorstr_("spine1"));
									else if (num < 30 && num > 20)
										ret->hitBone() = StringPool::Get(xorstr_("l_hand"));
									else if (num < 20 && num > 10)
										ret->hitBone() = StringPool::Get(xorstr_("r_upperarm"));
									else if (num < 10)
										ret->hitBone() = StringPool::Get(xorstr_("l_knee"));
									else
										ret->hitBone() = StringPool::Get(xorstr_("spine4"));
								}
								else if (combot::targetbone == 4) {
									int yeet = rand() % 100;
									if (yeet > 50)
										ret->hitBone() = StringPool::Get(xorstr_("head"));
									else
										ret->hitBone() = StringPool::Get(xorstr_("spine4"));
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

void DoAttack_hk(FlintStrikeWeapon* weapon) {
	if (combot::insta_eoka)
		weapon->_didSparkThisFrame() = true;

	return weapon->DoAttack();
}

Vector3 BodyLeanOffset_hk(PlayerEyes* a1) {
	if (combot::Manipulated && target_ply != nullptr) {
		if (target_ply != nullptr) {
			if (other::m_manipulate.empty() || !LocalPlayer::Entity()->GetHeldEntity())
				return a1->BodyLeanOffset();

			return other::m_manipulate;
		}
	}

	return a1->BodyLeanOffset();
}
void DoFirstPersonCamera_hk(PlayerEyes* a1, Component* cam) {
	a1->DoFirstPersonCamera(cam);
	if (combot::Manipulated) {
		Vector3 re_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
		cam->transform()->set_position(re_p);
	}
}
bool CanAttack_hk(BasePlayer* self) {
	if (combot::Noattacksprinting)
		return true;

	return self->CanAttack();
}
void UpdateVelocity_hk(PlayerWalkMovement* self) {
	if (!self->flying()) {
		Vector3 vel = self->TargetMovement();
		if (combot::nosprintingrestriction) {
			float max_speed = (self->swimming() || self->Ducking() > 0.5) ? 1.7f : 5.5f;
			if (vel.length() > 0.f) {
				Vector3 target_vel = Vector3(vel.x / vel.length() * max_speed, vel.y, vel.z / vel.length() * max_speed);
				self->TargetMovement() = target_vel;
			}
		}
	}
	return self->UpdateVelocity();
}
Vector3 EyePositionForPlayer_hk(BaseMountable* mount, BasePlayer* player, Quaternion lookRot) {
	if (player->userID() == LocalPlayer::Entity()->userID()) {
		if (combot::Manipulated && target_ply != nullptr) {
			return mount->EyePositionForPlayer(player, lookRot) + other::m_manipulate;
		}
	}

	return mount->EyePositionForPlayer(player, lookRot);
}
void HandleJumping_hk(PlayerWalkMovement* a1, ModelState* state, bool wantsJump, bool jumpInDirection = false) {
	if (combot::nojumpingrestriction) {
		if (!wantsJump)
			return;

		a1->grounded() = (a1->climbing() = (a1->sliding() = false));
		state->set_ducked(false);
		a1->jumping() = true;
		state->set_jumped(true);
		a1->jumpTime() = Time::time();
		a1->ladder() = nullptr;

		Vector3 curVel = a1->body()->velocity();
		a1->body()->set_velocity({ curVel.x, 10, curVel.z });
		return;
	}


	return a1->HandleJumping(state, wantsJump, jumpInDirection);
}
void OnLand_hk(BasePlayer* ply, float vel) {
	if (!combot::Nofalldamage)
		ply->OnLand(vel);
}

bool IsDown_hk(InputState* self, BUTTON btn) {
	if (combot::autoshoot) {
		if (btn == BUTTON::FIRE_PRIMARY) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			if (held && !held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
				if (target_ply != nullptr && target_ply->isCached()) {
					auto mpv = target_ply->find_mpv_bone();
					Vector3 target;
					if (mpv != nullptr)
						target = mpv->position;
					else
						target = target_ply->bones()->head->position;

					if (LineOfSight(target, LocalPlayer::Entity()->eyes()->position()))
						return true;
				}
			}
		}
	}

	return self->IsDown(btn);
}

void Play(ViewModel* viewmodel, pUncStr name, int layer = 0) {
	if (combot::remove_attack_anim) {
		static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::DoAttack(): Void");
		if (!CALLED_BY(ptr, 0x296) || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -75944661) {
			return original_viewmodelplay(viewmodel, name, layer);
		}
	}
	else {
		return original_viewmodelplay(viewmodel, name, layer);
	}
}
void __fastcall SetFlying(ModelState* a1, bool a2) { }
void SendClientTick(BasePlayer* baseplayer) {
	if (!baseplayer) return original_sendclienttick(baseplayer);
	if (combot::anti_aim) {
		auto input = read(baseplayer + 0x4E0, uintptr_t);
		auto state = read(input + 0x20, uintptr_t);
		auto current = read(state + 0x10, uintptr_t); if (!current) { return original_sendclienttick(baseplayer); }
		yeet += combot::anti_aim_speed;
		if (yeet >= 999) { // reset number
			yeet = 0;
		}
		if (combot::anti_aim_yaw == 0) {
			yaw = 100;
		}
		if (combot::anti_aim_yaw == 1) {
			yaw = -100;
		}
	    //LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::OnLadder);
		write(current + 0x18, Vector3(yaw, yeet, 0), Vector3);
	}
	return original_sendclienttick(baseplayer);
}


void OnAttacked_hk(BaseCombatEntity* self, HitInfo* info) {
	self->OnAttacked(info);

	if (self->class_name_hash() == STATIC_CRC32("BasePlayer")) {
		if (info->Initiator() == LocalPlayer::Entity()) {
			auto entity = reinterpret_cast<BasePlayer*>(self);
			if (std::filesystem::exists(settings::data_dir + xorstr_("C:\\hitsound\\hs.wav")))
				play_sound((settings::data_dir + xorstr_("C:\\hitsound\\hs.wav")).c_str());
		}
	}

}
void ClientInput_hk(BasePlayer* plly, uintptr_t state) {
	if (!plly)
		return plly->ClientInput(state);
	if (plly->userID() == LocalPlayer::Entity()->userID()) {
		//if (combot::Manipulated && target_ply != nullptr && target_ply->isCached() && target_ply != nullptr)
		//	plly->clientTickInterval() = 0.15f;
		//else
		//	plly->clientTickInterval() = 0.05f;
		settings::tr::Fake_lag = combot::fake_lag;
		if (combot::fake_lag)
		{
			if (combot::Falelag_type == 1) {
				plly->clientTickInterval() = combot::fake_tick; // half way 0.5f
			}
			else if (combot::Falelag_type == 2) {
				int yeet = rand() % 100;
				if (yeet > 50)
					plly->clientTickInterval() = 0.25f;//0.99
				else
					plly->clientTickInterval() = 0.05f;
			}
			else if (combot::Falelag_type == 3) {
				int yeet = rand() % 100;
				int num = rand() % 100;
				if (num > 90)
					plly->clientTickInterval() = 1.05f;
				else if (num < 90 && num > 80)
					plly->clientTickInterval() = 0.85f;
				else if (num < 80 && num > 70)
					plly->clientTickInterval() = 0.70f;
				else if (num < 70 && num > 60)
					plly->clientTickInterval() = 0.200f;
				else if (num < 60 && num > 50)
					plly->clientTickInterval() = 0.50f;
				else if (num < 50 && num > 40)
					plly->clientTickInterval() = 0.133f;
				else if (num < 40 && num > 30)
					plly->clientTickInterval() = 0.12f;
				else if (num < 30 && num > 20)
					plly->clientTickInterval() = 0.14f;
				else if (num < 20 && num > 10)
					plly->clientTickInterval() = 0.10f;
				else if (num < 10)
					plly->clientTickInterval() = 0.09f;
				else
					plly->clientTickInterval() = 0.05f;
			}
		}
		//if (combot::fake_lag)
		//	plly->clientTickInterval() = combot::fake_tick;
		//else 
		//	plly->clientTickInterval() = 0.05f;

		auto held = plly->GetHeldEntity<BaseProjectile>();
		if (held) {
			if (combot::no_sway & held->class_name_hash() != STATIC_CRC32("BaseMelee")) {
				held->aimSway() = 0.f;
				held->aimSwaySpeed() = 0.f;
			}
				if (combot::magic_fire & held->class_name_hash() == STATIC_CRC32("BaseProjectile"))
					held->repeatDelay() = 0.06f;//rapid bang //0.02
				else if (held->class_name_hash() == STATIC_CRC32("BaseProjectile"))
					held->repeatDelay() = 0.12f;

			if (combot::automatic)
				held->automatic() = true;

			if (GetAsyncKeyState(combot::FLY))
			{
				held->capsuleHeightDucked() - 3000.f;
				held->capsuleCenterDucked() - 3000.f;
			}
			else
			{
				held->capsuleHeightDucked() = 1.1f;
				held->capsuleCenterDucked() = 0.550f;

			}


			settings::tr::FakeShotKey = combot::fakeshots && GetAsyncKeyState(combot::fakeshotkey);
			if (combot::fakeshots && GetAsyncKeyState(combot::fakeshotkey))
				held->SendSignalBroadcast(BaseEntity::Signal::Attack, xorstr_(""));

			if (combot::autoshoot || (combot::Manipulated && target_ply != nullptr)) {
				if (!held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
					if (target_ply != nullptr && target_ply->isCached()) {
						auto mpv = target_ply->find_mpv_bone();
						Vector3 target;
						if (mpv != nullptr)
							target = mpv->position;
						else
							target = target_ply->bones()->head->position;

						if (LineOfSight(target, plly->eyes()->position()))
							held->DoAttack();
					}
				}
			}
		}

		settings::tr::manipulated = combot::Manipulated & target_ply != nullptr;

		GLOBAL_TIME = Time::time();

		if (combot::Manipulated && target_ply != nullptr && target_ply->isCached() && target_ply != nullptr)
			other::find_manipulate_angle();
		else
			if (!other::m_manipulate.empty())
				other::m_manipulate = Vector3::Zero();
		Physics::IgnoreLayerCollision(4, 12, !combot::Jesus);
		Physics::IgnoreLayerCollision(30, 12, combot::Jesus);
		Physics::IgnoreLayerCollision(11, 12, combot::Jesus);

		if (GetAsyncKeyState(combot::zoomkey))
			ConVar::Graphics::_fov() = 15.f;
		else
			ConVar::Graphics::_fov() = combot::fov;

		if (combot::fakeadmin)
			plly->playerFlags() |= PlayerFlags::IsAdmin;

		if (combot::bfly)

		if (combot::Noattacksprinting)
			if (plly->mounted())
				plly->mounted()->canWieldItems() = true;

		Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
		DWORD64 active = weapon->entity();
		char* classname = weapon->ClassName();
		bool weaponmelee = weapon && classname && (strcmp(classname, xorstr_("BaseMelee")) || strcmp(classname, xorstr_("Jackhammer")));
		auto_farm_loop(weaponmelee, active);
		auto entityList = BaseNetworkable::clientEntities()->entityList();
		for (int i = 0; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			BasePlayer* lol = (BasePlayer*)Entity;
			Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
			DWORD64 active = weapon->entity();
			char* classname = weapon->ClassName();

			bool weaponmelee = weapon && classname && (strcmp(classname, xorstr_("BaseMelee")) || strcmp(classname, xorstr_("Jackhammer")));
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
				if (combot::silent_melee && weaponmelee && Calc3D_Dist(lol->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= 2.5f) {
					f_object target = f_object::get_melee_target((BasePlayer*)Entity, active);
					lol::do_attack(target, active, true);
				}
			}
		}
		if (MiscVisual::nighmode != 0) {
			auto list = TOD_Sky::instances();
			if (list) {
				for (int j = 0; j < list->size; j++) {
					auto sky = (TOD_Sky*)list->get(j);
					if (!sky)
						continue;

					float amb = 5.f;

					sky->Day()->AmbientMultiplier() = amb == 4.f ? 0.2f : 1.f;
					sky->Night()->AmbientMultiplier() = amb;
				}
			}
		}
	}

	
	//if (target_ply != nullptr) {
	//	auto l1 = target_ply->playerModel()->_multiMesh()->Renderers();
	//	if (l1) {
	//		for (int i = 0; i < l1->size; i++) {
	//			auto rend = (Renderer_*)l1->get(i);
	//			if (!rend)
	//				continue;

	//			std::cout << *reinterpret_cast<uintptr_t*>(rend->material() + 0xB0) << std::endl;
	//			std::cout << reinterpret_cast<uintptr_t>(rend->material()->shader()) << std::endl << std::endl;

	//			if (combot::fovaim > 1000) {
	//				auto list = *reinterpret_cast<Array<Material*>**>(rend + 0x140);
	//				if (list) {
	//					for (int j = 0; j < list->size(); j++) {
	//						auto g = list->get(j);
	//						if (!g)
	//							continue;

	//						*reinterpret_cast<Material**>(g) = nullptr;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	plly->ClientInput(state);

	// before network 
	settings::tr::Onlader = combot::OnLadder && GetAsyncKeyState(combot::OnLadderkey);
	if (combot::OnLadder && GetAsyncKeyState(combot::OnLadderkey))
		LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::OnLadder);

	if (combot::running)
		LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::OnGround);
	if (combot::Noattacksprinting)
		LocalPlayer::Entity()->add_modelstate_flag(ModelState::Flags::Sprinting);
	
	if (combot::godModetest) {
		plly->clientTickInterval() = 6.f;
		//LocalPlayer::Entity()->transform()->set_position({ 0.f, 0.f, 0.f });
		LocalPlayer::Entity()->transform()->position() = { 0.f, 0.f, 0.f };
		plly->clientTickInterval() = 628925.f;
		//LocalPlayer::Entity()->transform()->set_position({ 24.f, 40.f, 110.f });
		LocalPlayer::Entity()->transform()->position() = { 24.f, 40.f, 110.f };
		plly->clientTickInterval() = -259.f;
		plly->clientTickInterval() = -3.141592f;

	}
}

void DoMovement_hk(Projectile* pr, float deltaTime) {
	if (pr->isAuthoritative())
		if (combot::big_bullets)
			pr->thickness() = 2.6f;//1.0
		else
			pr->thickness() = 0.1f;

	return pr->DoMovement(deltaTime);
}

float GetRandomVelocity_hk(ItemModProjectile* self) {
	float modifier = 1.1f;//1.0

	if (combot::fast_bullets)
		modifier += 0.4f;

	return self->GetRandomVelocity() * modifier;
}

void AddPunch_hk(HeldEntity* attackEntity, Vector3 amount, float duration) {
	amount *= combot::recoil_control / 100.0f;

	attackEntity->AddPunch(amount, duration);
}

Vector3 MoveTowards_hk(Vector3 current, Vector3 target, float maxDelta) {
	static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::SimulateAimcone(): Void");
	if (CALLED_BY(ptr, 0x800)) {
		target *= combot::recoil_control / 100.0f;
		maxDelta *= combot::recoil_control / 100.0f;
	}

	return Vector3_::MoveTowards(current, target, maxDelta);
}
bool DoHit_hk(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
	if (combot::pierce) {
		if (prj->isAuthoritative()) {
			auto lol = test->HitEntity();
			auto go = test->gameObject();

			if (go && !lol) {
				if (go->layer() == 0 || go->layer() == 24) {
					return false;
				}
			}
			if (lol) {
				if (lol->class_name_hash() == STATIC_CRC32("CargoShip") || lol->class_name_hash() == STATIC_CRC32("BaseOven")
					|| lol->class_name_hash() == STATIC_CRC32("TreeEntity") || lol->class_name_hash() == STATIC_CRC32("OreResourceEntity")
					|| lol->class_name_hash() == STATIC_CRC32("CH47HelicopterAIController") || lol->class_name_hash() == STATIC_CRC32("MiniCopter")
					|| lol->class_name_hash() == STATIC_CRC32("BoxStorage") || lol->class_name_hash() == STATIC_CRC32("Workbench")
					|| lol->class_name_hash() == STATIC_CRC32("VendingMachine") || lol->class_name_hash() == STATIC_CRC32("Barricade")
					|| lol->class_name_hash() == STATIC_CRC32("BuildingPrivlidge") || lol->class_name_hash() == STATIC_CRC32("LootContainer")
					|| lol->class_name_hash() == STATIC_CRC32("HackableLockedCrate") || lol->class_name_hash() == STATIC_CRC32("ResourceEntity")
					|| lol->class_name_hash() == STATIC_CRC32("RidableHorse") || lol->class_name_hash() == STATIC_CRC32("MotorRowboat")
					|| lol->class_name_hash() == STATIC_CRC32("ScrapTransportHelicopter") || lol->class_name_hash() == STATIC_CRC32("JunkPile")
					|| lol->class_name_hash() == STATIC_CRC32("MiningQuarry") || lol->class_name_hash() == STATIC_CRC32("WaterCatcher")
					|| lol->class_name_hash() == STATIC_CRC32("SupplyDrop") || lol->class_name_hash() == STATIC_CRC32("HelicopterDebris")
					|| lol->class_name_hash() == STATIC_CRC32("RHIB"))  {
					
					return false;
				}
			}
		}
	}
	return prj->DoHit(test, point, normal);
}
void SetEffectScale_hk(Projectile* self, float eScale) {
	return self->SetEffectScale((combot::Perfect_Silent_player && self->isAuthoritative()) ? 1.6f : eScale);
}
String* ConsoleRun_hk(ConsoleSystem::Option* optiom, String* str, Array<System::Object_*>* args) {
	if (optiom->IsFromServer()) {
		if (str->buffer) {
			auto string = std::wstring(str->buffer);
			if (string.find(wxorstr_(L"noclip")) != std::wstring::npos ||
				string.find(wxorstr_(L"debugcamera")) != std::wstring::npos ||
				string.find(wxorstr_(L"admintime")) != std::wstring::npos ||
				string.find(wxorstr_(L"camlerp")) != std::wstring::npos ||
				string.find(wxorstr_(L"camspeed")) != std::wstring::npos) {

				str = String::New(xorstr_(""));
			}
		}
	}

	return ConsoleSystem::Run(optiom, str, args);
}

void set_flying_hk(ModelState* modelState, bool state) {
	modelState->set_flying(false);
}

void do_hooks() {
	hookengine::hook(original_viewmodelplay, Play);
	hookengine::hook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::hook(BaseCombatEntity::OnAttacked_, OnAttacked_hk);
	hookengine::hook(BasePlayer::ClientUpdate_Sleeping_, ClientUpdate_Sleeping_hk);
	hookengine::hook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::hook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::hook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::hook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::hook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::hook(FlintStrikeWeapon::DoAttack_, DoAttack_hk);
	hookengine::hook(InputState::IsDown_, IsDown_hk);
	hookengine::hook(ConsoleSystem::Run_, ConsoleRun_hk);
	hookengine::hook(ModelState::set_flying_, set_flying_hk);
	hookengine::hook(HitTest::BuildAttackMessage_, BuildAttackMessage_hk);
	hookengine::hook(Projectile::DoHit_, DoHit_hk);
	hookengine::hook(BaseMountable::EyePositionForPlayer_, EyePositionForPlayer_hk);
	hookengine::hook(Projectile::SetEffectScale_, SetEffectScale_hk);
	hookengine::hook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::hook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::hook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::hook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::hook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
	hookengine::hook(Vector3_::MoveTowards_, MoveTowards_hk);
	hookengine::hook(HeldEntity::AddPunch_, AddPunch_hk);
}
void undo_hooks() {
	hookengine::unhook(original_viewmodelplay, Play);
	hookengine::unhook(BaseCombatEntity::OnAttacked_, OnAttacked_hk);
	hookengine::unhook(BasePlayer::ClientUpdate_, ClientUpdate_hk);
	hookengine::unhook(PlayerWalkMovement::UpdateVelocity_, UpdateVelocity_hk);
	hookengine::unhook(PlayerWalkMovement::HandleJumping_, HandleJumping_hk);
	hookengine::unhook(BasePlayer::CanAttack_, CanAttack_hk);
	hookengine::unhook(BasePlayer::OnLand_, OnLand_hk);
	hookengine::unhook(Projectile::DoMovement_, DoMovement_hk);
	hookengine::unhook(FlintStrikeWeapon::DoAttack_, DoAttack_hk);
	hookengine::unhook(InputState::IsDown_, IsDown_hk);
	hookengine::unhook(ConsoleSystem::Run_, ConsoleRun_hk);
	hookengine::unhook(ModelState::set_flying_, set_flying_hk);
	hookengine::unhook(HitTest::BuildAttackMessage_, BuildAttackMessage_hk);
	hookengine::unhook(Projectile::DoHit_, DoHit_hk);
	hookengine::unhook(BaseMountable::EyePositionForPlayer_, EyePositionForPlayer_hk);
	hookengine::unhook(Projectile::SetEffectScale_, SetEffectScale_hk);
	hookengine::unhook(BasePlayer::ClientInput_, ClientInput_hk);
	hookengine::unhook(ItemModProjectile::GetRandomVelocity_, GetRandomVelocity_hk);
	hookengine::unhook(PlayerEyes::BodyLeanOffset_, BodyLeanOffset_hk);
	hookengine::unhook(AimConeUtil::GetModifiedAimConeDirection_, GetModifiedAimConeDirection_hk);
	hookengine::unhook(PlayerEyes::DoFirstPersonCamera_, DoFirstPersonCamera_hk);
	hookengine::unhook(Vector3_::MoveTowards_, MoveTowards_hk);
	hookengine::unhook(HeldEntity::AddPunch_, AddPunch_hk);
}