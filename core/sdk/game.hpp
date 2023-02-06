class SafeExecution {
private:
	static int fail(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
		if (code == EXCEPTION_ACCESS_VIOLATION) {
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else {
			return EXCEPTION_CONTINUE_SEARCH;
		};
	}
public:
	template<typename T = void*, typename R = void*, typename... Args>
	static T Execute(uint64_t ptr, R ret, Args... args) {
		__try {
			return reinterpret_cast<T(__stdcall*)(Args...)>(ptr)(args...);
		}
		__except (fail(GetExceptionCode(), GetExceptionInformation())) {
			return ret;
		}
	}
};

using namespace System;

enum class KeyCode : int {
	None = 0,
	Backspace = 8,
	Delete = 127,
	Tab = 9,
	Clear = 12,
	Return = 13,
	Pause = 19,
	Escapee = 27,
	Space = 32,
	Keypad0 = 256,
	Keypad1 = 257,
	Keypad2 = 258,
	Keypad3 = 259,
	Keypad4 = 260,
	Keypad5 = 261,
	Keypad6 = 262,
	Keypad7 = 263,
	Keypad8 = 264,
	Keypad9 = 265,
	KeypadPeriod = 266,
	KeypadDivide = 267,
	KeypadMultiply = 268,
	KeypadMinus = 269,
	KeypadPlus = 270,
	KeypadEnter = 271,
	KeypadEquals = 272,
	UpArrow = 273,
	DownArrow = 274,
	RightArrow = 275,
	LeftArrow = 276,
	Insert = 277,
	Home = 278,
	End = 279,
	PageUp = 280,
	PageDown = 281,
	F1 = 282,
	F2 = 283,
	F3 = 284,
	F4 = 285,
	F5 = 286,
	F6 = 287,
	F7 = 288,
	F8 = 289,
	F9 = 290,
	F10 = 291,
	F11 = 292,
	F12 = 293,
	F13 = 294,
	F14 = 295,
	F15 = 296,
	Alpha0 = 48,
	Alpha1 = 49,
	Alpha2 = 50,
	Alpha3 = 51,
	Alpha4 = 52,
	Alpha5 = 53,
	Alpha6 = 54,
	Alpha7 = 55,
	Alpha8 = 56,
	Alpha9 = 57,
	Exclaim = 33,
	DoubleQuote = 34,
	Hash = 35,
	Dollar = 36,
	Percent = 37,
	Ampersand = 38,
	Quote = 39,
	LeftParen = 40,
	RightParen = 41,
	Asterisk = 42,
	Plus = 43,
	Comma = 44,
	Minus = 45,
	Period = 46,
	Slash = 47,
	Colon = 58,
	Semicolon = 59,
	Less = 60,
	Equals = 61,
	Greater = 62,
	Question = 63,
	At = 64,
	LeftBracket = 91,
	Backslash = 92,
	RightBracket = 93,
	Caret = 94,
	Underscore = 95,
	BackQuote = 96,
	A = 97,
	B = 98,
	C = 99,
	D = 100,
	E = 101,
	F = 102,
	G = 103,
	H = 104,
	I = 105,
	J = 106,
	K = 107,
	L = 108,
	M = 109,
	N = 110,
	O = 111,
	P = 112,
	Q = 113,
	R = 114,
	S = 115,
	T = 116,
	U = 117,
	V = 118,
	W = 119,
	X = 120,
	Y = 121,
	Z = 122,
	LeftCurlyBracket = 123,
	Pipe = 124,
	RightCurlyBracket = 125,
	Tilde = 126,
	Numlock = 300,
	CapsLock = 301,
	ScrollLock = 302,
	RightShift = 303,
	LeftShift = 304,
	RightControl = 305,
	LeftControl = 306,
	RightAlt = 307,
	LeftAlt = 308,
	LeftCommand = 310,
	LeftApple = 310,
	LeftWindows = 311,
	RightCommand = 309,
	RightApple = 309,
	RightWindows = 312,
	AltGr = 313,
	Help = 315,
	Print = 316,
	SysReq = 317,
	Break = 318,
	Menu = 319,
	Mouse0 = 323,
	Mouse1 = 324,
	Mouse2 = 325,
	Mouse3 = 326,
	Mouse4 = 327,
	Mouse5 = 328,
	Mouse6 = 329
};
class Object {
public:
	Transform* transform() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::get_transform(): Transform");
		return SafeExecution::Execute<Transform*>(off, nullptr, this);
	}
};
class Type {
public:
	// pass as "Namespace.Classname, Assembly.Name"
	static Type* GetType(const char* qualified_name) {
		static auto off = METHOD("mscorlib::System::Type::GetType(String): Type");
		return reinterpret_cast<Type * (__cdecl*)(String*)>(off)(String::New(qualified_name));
	}
	static Type* SkinnedMeshRenderer() {
		Type* type = GetType(xorstr_("UnityEngine.SkinnedMeshRenderer, UnityEngine.CoreModule"));
		return type;
	}
	static Type* Renderer() {
		Type* type = GetType(xorstr_("UnityEngine.Renderer, UnityEngine.CoreModule"));
		return type;
	}
	static Type* Shader() {
		Type* type = GetType(xorstr_("UnityEngine.Shader, UnityEngine.CoreModule"));
		return type;
	}
	static Type* Projectile() {
		Type* type = GetType(xorstr_("Projectile, Assembly-CSharp"));
		return type;
	}
};

class GameObject;
class Component {
public:
	Transform* transform() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::get_transform(): Transform");
		return SafeExecution::Execute<Transform*>(off, nullptr, this);
	}
	template<typename T = Component>
	T* GetComponent(Type* type) {
		if (!this || !type) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::GetComponent(Type): Component");
		return SafeExecution::Execute<T*>(off, nullptr, this, type);
	}
	template<typename T = Component>
	Array<T*>* GetComponentsInChildren(Type* type) {
		if (!this || !type) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::GetComponentsInChildren(Type): Component[]");
		return SafeExecution::Execute<Array<T*>*>(off, nullptr, this, type);
	}
	GameObject* gameObject() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Component::get_gameObject(): GameObject");
		return SafeExecution::Execute<GameObject*>(off, nullptr, this);
	}
	const char* class_name() {
		if (!this) return "";
		auto oc = *reinterpret_cast<uint64_t*>(this);
		if (!oc) return "";
		return *reinterpret_cast<char**>(oc + 0x10);
	}
	uint32_t class_name_hash() {
		if (!this) return 0;
		auto oc = *reinterpret_cast<uint64_t*>(this);
		if (!oc) return 0;
		const char* name = *reinterpret_cast<char**>(oc + 0x10);
		return RUNTIME_CRC32(name);
	}
	bool IsPlayer() {
		if (!this) return false;

		return !strcmp(this->class_name(), xorstr_("BasePlayer")) ||
			!strcmp(this->class_name(), xorstr_("NPCPlayerApex")) ||
			!strcmp(this->class_name(), xorstr_("NPCMurderer")) ||
			!strcmp(this->class_name(), xorstr_("NPCPlayer")) ||
			!strcmp(this->class_name(), xorstr_("HumanNPC")) ||
			!strcmp(this->class_name(), xorstr_("Scientist")) ||
			!strcmp(this->class_name(), xorstr_("TunnelDweller")) ||
			!strcmp(this->class_name(), xorstr_("HTNPlayer")) ||
			!strcmp(this->class_name(), xorstr_("ScientistNPC")) ||
			!strcmp(this->class_name(), xorstr_("NPCShopKeeper"));
	}
};
class Renderer_;
class GameObject : public Component {
public:
	int layer() {
		if (!this) return 0;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::GameObject::get_layer(): Int32");
		return reinterpret_cast<int(__fastcall*)(GameObject*)>(off)(this);
	}
	const wchar_t* tag() {
		if (!this) return 0;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::GameObject::get_tag(): String");
		return reinterpret_cast<String * (__fastcall*)(GameObject*)>(off)(this)->buffer;
	}
	const wchar_t* name()
	{
		if (!this) return 0;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Object::get_name(): String");
		return reinterpret_cast<String * (__fastcall*)(GameObject*)>(off)(this)->buffer;
	}
	template<typename T = GameObject>
	T* GetComponent(Type* type) {
		if (!this || !type) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::GameObject::GetComponent(Type): Component");
		return SafeExecution::Execute<T*>(off, nullptr, this, type);
	}
};
class Transform : public Component {
public:
	Vector3 position() {
		if (!this)
			return Vector3::Zero();

		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::get_position(): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero(), this);
	}
	Vector3 localPosition() {
		if (!this)
			return Vector3::Zero();

		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::get_localPosition(): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero(), this);
	}
	Vector3 up() {
		if (!this)
			return Vector3::Zero();

		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::get_up(): Vector3");
		return SafeExecution::Execute<Vector3>(off, Vector3::Zero(), this);
	}
	void set_position(Vector3 value) {
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::set_position(Vector3): Void");
		reinterpret_cast<void(__fastcall*)(Transform*, Vector3)>(off)(this, value);
	}
	Vector3 InverseTransformPoint(Vector3 position) {
		if (!this) return Vector3::Zero();

		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::InverseTransformPoint(Vector3): Vector3");

		return reinterpret_cast<Vector3(__fastcall*)(Transform*, Vector3)>(off)(this, position);
	}

	Vector3 InverseTransformDirection(Vector3 position) {
		if (!this) return Vector3::Zero();

		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Transform::InverseTransformDirection(Vector3): Vector3");

		return reinterpret_cast<Vector3(__fastcall*)(Transform*, Vector3)>(off)(this, position);
	}
};
class BasePlayer;
class LocalPlayer {
public:
	static BasePlayer* Entity() {
		static auto clazz = CLASS("Assembly-CSharp::LocalPlayer");
		return *reinterpret_cast<BasePlayer**>(std::uint64_t(clazz->static_fields));
	}
};

class Networkable {
public:
	FIELD("Facepunch.Network::Network::Networkable::ID", ID, uint32_t);
};
class BaseEntity;
class BaseNetworkable : public Component {
public:
	class EntityRealm {
	public:
		template<typename T = BaseNetworkable*> T Find(uint32_t uid) {
			static auto off = METHOD("Assembly-CSharp::EntityRealm::Find(UInt32): BaseNetworkable");
			return reinterpret_cast<T(__fastcall*)(EntityRealm*, uint32_t)>(off)(this, uid);
		}
		template<typename T = BaseNetworkable*>
		T FindClosest(uint32_t hash, BaseNetworkable* targetEnt, float dist) {
			T ent = nullptr;

			auto entityList = this->entityList();
			if (entityList) {
				for (int i = 1; i < entityList->vals->size; i++) {
					auto baseNetworkable = *reinterpret_cast<BaseNetworkable**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
					if (!baseNetworkable) continue;

					if (baseNetworkable->class_name_hash() == hash && baseNetworkable->transform()->position().distance(targetEnt->transform()->position()) <= dist) {
						ent = reinterpret_cast<T>(baseNetworkable);
						break;
					}
				}
			}

			return ent;
		}
		FIELD("Assembly-CSharp::EntityRealm::entityList", entityList, ListDictionary*);
	};

	bool isClient() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::BaseNetworkable::get_isClient(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(BaseNetworkable*)>(off)(this);
	}

	bool IsDestroyed() {
		if (!this) return true;
		static auto off = OFFSET("Assembly-CSharp::BaseNetworkable::<IsDestroyed>k__BackingField");
		return *reinterpret_cast<bool*>(this + off);
	}

	static EntityRealm* clientEntities() {
		static auto clazz = CLASS("Assembly-CSharp::BaseNetworkable");
		return *reinterpret_cast<EntityRealm**>(std::uint64_t(clazz->static_fields));
	}

	const wchar_t* ShortPrefabName() {
		if (!this) return L"";
		static auto off = METHOD("Assembly-CSharp::BaseNetworkable::get_ShortPrefabName(): String");
		return reinterpret_cast<String * (__fastcall*)(BaseNetworkable*)>(off)(this)->buffer;
	}

	std::uint32_t ShortPrefabName_hash() {
		if (!this) return 0;
		static auto off = METHOD("Assembly-CSharp::BaseNetworkable::get_ShortPrefabName(): String");
		return RUNTIME_CRC32_W(reinterpret_cast<String * (__fastcall*)(BaseNetworkable*)>(off)(this)->buffer);
	}
	FIELD("Assembly-CSharp::BaseNetworkable::<JustCreated>k__BackingField", JustCreated, bool);
	FIELD("Assembly-CSharp::BaseNetworkable::net", net, Networkable*);
	FIELD("Assembly-CSharp::BaseNetworkable::parentEntity", parentEntity, BaseEntity*);
};
class Material;
class Skinnable {
public:
	FIELD("Assembly-CSharp::Skinnable::_sourceMaterials", _sourceMaterials, Array<Material*>*);
};
class ItemSkin {
public:
	FIELD("Assembly-CSharp::ItemSkin::Skinnable", _Skinnable, Skinnable*);
	FIELD("Assembly-CSharp::ItemSkin::Materials", Materials, Array<Material*>*);
};
class Model;
class BaseEntity : public BaseNetworkable {
public:
	enum class Signal {
		Attack,
		Alt_Attack,
		DryFire,
		Reload,
		Deploy,
		Flinch_Head,
		Flinch_Chest,
		Flinch_Stomach,
		Flinch_RearHead,
		Flinch_RearTorso,
		Throw,
		Relax,
		Gesture,
		PhysImpact,
		Eat,
		Startled
	};
	enum class Flags
	{
		Placeholder = 1,
		On = 2,
		OnFire = 4,
		Open = 8,
		Locked = 16,
		Debugging = 32,
		Disabled = 64,
		Reserved1 = 128,
		Reserved2 = 256,
		Reserved3 = 512,
		Reserved4 = 1024,
		Reserved5 = 2048,
		Broken = 4096,
		Busy = 8192,
		Reserved6 = 16384,
		Reserved7 = 32768,
		Reserved8 = 65536,
		Reserved9 = 131072,
		Reserved10 = 262144
	};

	FIELD("Assembly-CSharp::BaseEntity::flags", flags, BaseEntity::Flags)
		bool IsValid() {
		if (!this) return false;
		return !this->IsDestroyed() && this->net() != nullptr;
	}

	bool HasFlag(BaseEntity::Flags f);

	void ServerRPC(const char* funcName) {
		if (!this) return;
		static auto off = METHOD("Assembly-CSharp::BaseEntity::ServerRPC(String): Void");
		reinterpret_cast<void(__stdcall*)(BaseEntity*, String*)>(off)(this, String::New(funcName));
	}
	Vector3 GetWorldVelocity() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::BaseEntity::GetWorldVelocity(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(BaseEntity*)>(off)(this);
	}
	Vector3 ClosestPoint(Vector3 p) {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::BaseEntity::ClosestPoint(Vector3): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(BaseEntity*, Vector3)>(off)(this, p);
	}
	void SendSignalBroadcast(Signal a, char* str = xorstr_("")) {
		if (!this) return;
		static auto off = METHOD("Assembly-CSharp::BaseEntity::SendSignalBroadcast(Signal,String): Void");
		return reinterpret_cast<void(__fastcall*)(BaseEntity*, Signal, String*)>(off)(this, a, String::New(str));
	}
	FIELD("Assembly-CSharp::BaseEntity::model", model, Model*);
	FIELD("Assembly-CSharp::BaseEntity::itemSkin", itemSkin, ItemSkin*);
};

BaseEntity::Flags operator &(BaseEntity::Flags lhs, BaseEntity::Flags rhs) {
	return static_cast<BaseEntity::Flags> (
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(lhs) &
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(rhs)
		);
}
BaseEntity::Flags operator ^(BaseEntity::Flags lhs, BaseEntity::Flags rhs) {
	return static_cast<BaseEntity::Flags> (
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(lhs) ^
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(rhs)
		);
}
BaseEntity::Flags operator ~(BaseEntity::Flags rhs) {
	return static_cast<BaseEntity::Flags> (
		~static_cast<std::underlying_type<BaseEntity::Flags>::type>(rhs)
		);
}
BaseEntity::Flags& operator |=(BaseEntity::Flags& lhs, BaseEntity::Flags rhs) {
	lhs = static_cast<BaseEntity::Flags> (
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(lhs) |
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(rhs)
		);

	return lhs;
}
BaseEntity::Flags& operator &=(BaseEntity::Flags& lhs, BaseEntity::Flags rhs) {
	lhs = static_cast<BaseEntity::Flags> (
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(lhs) &
		static_cast<std::underlying_type<BaseEntity::Flags>::type>(rhs)
		);

	return lhs;
}

bool BaseEntity::HasFlag(BaseEntity::Flags f)
{
	return (this->flags() & f) == f;
}

class GamePhysics {
public:
	enum QueryTriggerInteraction {
		UseGlobal = 0,
		Ignore = 1,
		Collide = 2,
	};
	STATIC_FUNCTION("Assembly-CSharp::GamePhysics::LineOfSight(Vector3,Vector3,Int32,Single): Boolean", LineOfSight, bool(Vector3, Vector3, int, float));
	STATIC_FUNCTION("Assembly-CSharp::GamePhysics::CheckCapsule(Vector3,Vector3,Single,Int32,QueryTriggerInteraction): Boolean", CheckCapsule, bool(Vector3, Vector3, float, int, QueryTriggerInteraction));
};
Vector3 ClosestPoint(BaseEntity* base, Vector3 p) {
	if (!base) return Vector3::Zero();
	static auto off = METHOD("Assembly-CSharp::BaseEntity::ClosestPoint(Vector3): Vector3");
	return reinterpret_cast<Vector3(__fastcall*)(BaseEntity*, Vector3)>(off)(base, p);
}
bool LineOfSight(Vector3 a, Vector3 b) {
	int mask = combot::pierce ? 10551296 : 1503731969; // projectile los, flyhack mask

	bool result = GamePhysics::LineOfSight(a, b, mask, 0.f) && GamePhysics::LineOfSight(b, a, mask, 0.f);
	return result;
}
class Time {
public:
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_time(): Single", time, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_deltaTime(): Single", deltaTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_fixedTime(): Single", fixedTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_unscaledTime(): Single", unscaledTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_unscaledDeltaTime(): Single", unscaledDeltaTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_fixedDeltaTime(): Single", fixedDeltaTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_maximumDeltaTime(): Single", maximumDeltaTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_smoothDeltaTime(): Single", smoothDeltaTime, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_timeScale(): Single", timeScale, float());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::set_timeScale(Single): Void", set_timeScale, void(float));
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_frameCount(): Int32", frameCount, int());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_renderedFrameCount(): Int32", renderedFrameCount, int());
	STATIC_FUNCTION("UnityEngine.CoreModule::UnityEngine::Time::get_realtimeSinceStartup(): Single", realtimeSinceStartup, float());
};
class DamageTypeList {
public:
	float Total() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::Rust::DamageTypeList::Total(): Single");
		return reinterpret_cast<float(__fastcall*)(DamageTypeList*)>(off)(this);
	}
};
typedef struct _UncStr {
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;

class HitInfo {
public:
	FIELD("Assembly-CSharp::HitInfo::Initiator", Initiator, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::WeaponPrefab", WeaponPrefab, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::DoHitEffects", DoHitEffects, bool);
	FIELD("Assembly-CSharp::HitInfo::DoDecals", DoDecals, bool);
	FIELD("Assembly-CSharp::HitInfo::IsPredicting", IsPredicting, bool);
	FIELD("Assembly-CSharp::HitInfo::UseProtection", UseProtection, bool);
	FIELD("Assembly-CSharp::HitInfo::DidHit", DidHit, bool);
	FIELD("Assembly-CSharp::HitInfo::HitEntity", HitEntity, BaseEntity*);
	FIELD("Assembly-CSharp::HitInfo::HitBone", HitBone, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitPart", HitPart, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitMaterial", HitMaterial, uint32_t);
	FIELD("Assembly-CSharp::HitInfo::HitPositionWorld", HitPositionWorld, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitPositionLocal", HitPositionLocal, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitNormalWorld", HitNormalWorld, Vector3);
	FIELD("Assembly-CSharp::HitInfo::HitNormalLocal", HitNormalLocal, Vector3);
	FIELD("Assembly-CSharp::HitInfo::PointStart", PointStart, Vector3);
	FIELD("Assembly-CSharp::HitInfo::PointEnd", PointEnd, Vector3);
	FIELD("Assembly-CSharp::HitInfo::ProjectileID", ProjectileID, int);
	FIELD("Assembly-CSharp::HitInfo::ProjectileDistance", ProjectileDistance, float);
	FIELD("Assembly-CSharp::HitInfo::ProjectileVelocity", ProjectileVelocity, Vector3);
	FIELD("Assembly-CSharp::HitInfo::damageTypes", damageTypes, DamageTypeList*);

	bool isHeadshot() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::HitInfo::get_isHeadshot(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(HitInfo*)>(off)(this);
	}
};
class Effect {
public:
};
float GLOBAL_TIME = 0.f;
class BaseCombatEntity : public BaseEntity {
public:
	enum Lifestate {
		Alive = 0,
		Dead = 1
	};
	FIELD("Assembly-CSharp::BaseCombatEntity::_health", health, float);
	FIELD("Assembly-CSharp::BaseCombatEntity::_maxHealth", maxHealth, float);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsHitNotification", sendsHitNotification, bool);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsMeleeHitNotification", sendsMeleeHitNotification, bool);
	FIELD("Assembly-CSharp::BaseCombatEntity::sendsMeleeHitNotification", lastNotifyFrame, int);
	FIELD("Assembly-CSharp::BaseCombatEntity::lifestate", lifestate, Lifestate);

	static inline void(*OnAttacked_)(BaseCombatEntity*, HitInfo*) = nullptr;
	void OnAttacked(HitInfo* info) {
		return OnAttacked_(this, info);
	}
};
class ConsoleSystem {
public:
	struct Option {
		static Option* Client() {
			static auto off = METHOD("Facepunch.Console::Option::get_Client(): Option");
			return reinterpret_cast<Option * (__fastcall*)()>(off)();
		}
		bool IsFromServer() {
			return *reinterpret_cast<bool*>(this + 0x6);
		}
	};

	static inline String* (*Run_)(Option*, String*, Array<System::Object_*>*) = nullptr;
	static String* Run(Option* option, String* command, Array<System::Object_*>* args) {
		return Run_(option, command, args);
	}
};
class BaseMountable : public BaseCombatEntity {
public:
	FIELD("Assembly-CSharp::BaseMountable::canWieldItems", canWieldItems, bool);

	BaseMountable* GetVehicleParent() {
		if (!this) return {};
		static auto off = METHOD("Assembly-CSharp::BaseVehicleMountPoint::GetVehicleParent(): BaseVehicle");
		return reinterpret_cast<BaseMountable * (*)(BaseMountable*)>(off)(this);
	}

	static inline Vector3(*EyePositionForPlayer_)(BaseMountable*, BasePlayer*, Quaternion) = nullptr;
	Vector3 EyePositionForPlayer(BasePlayer* ply, Quaternion rot) {
		return EyePositionForPlayer_(this, ply, rot);
	}
};
class RigidBody {
public:
	Vector3 velocity() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("UnityEngine.PhysicsModule::UnityEngine::Rigidbody::get_velocity(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(RigidBody*)>(off)(this);
	}
	void set_velocity(Vector3 value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.PhysicsModule::UnityEngine::Rigidbody::set_velocity(Vector3): Void");
		return reinterpret_cast<void(__fastcall*)(RigidBody*, Vector3)>(off)(this, value);
	}
};
class BaseMovement {
public:
	FIELD("Assembly-CSharp::BaseMovement::adminCheat", adminCheat, bool);
	FIELD("Assembly-CSharp::BaseMovement::<TargetMovement>k__BackingField", TargetMovement, Vector3);
	FIELD("Assembly-CSharp::BaseMovement::<Running>k__BackingField", Running, float);
	FIELD("Assembly-CSharp::BaseMovement::<Grounded>k__BackingField", Grounded, float);
	FIELD("Assembly-CSharp::BaseMovement::<Ducking>k__BackingField", Ducking, float);
};
class ModelState;
class PlayerWalkMovement : public BaseMovement {
public:
	FIELD("Assembly-CSharp::PlayerWalkMovement::flying", flying, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::body", body, RigidBody*);
	FIELD("Assembly-CSharp::PlayerWalkMovement::maxAngleWalking", maxAngleWalking, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::maxVelocity", maxVelocity, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::groundAngle", groundAngle, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::groundAngleNew", groundAngleNew, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::groundNormal", groundNormal, Vector3);
	FIELD("Assembly-CSharp::PlayerWalkMovement::jumpTime", jumpTime, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::landTime", landTime, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::groundTime", groundTime, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::gravityMultiplier", gravityMultiplier, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::climbing", climbing, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::sliding", sliding, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::grounded", grounded, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::jumping", jumping, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::swimming", swimming, bool);
	FIELD("Assembly-CSharp::PlayerWalkMovement::ladder", ladder, void*);
	static inline void(*UpdateVelocity_)(PlayerWalkMovement*) = nullptr;
	void UpdateVelocity() {
		return UpdateVelocity_(this);
	}
	static inline void(*HandleJumping_)(PlayerWalkMovement*, ModelState*, bool, bool) = nullptr;
	void HandleJumping(ModelState* modelState, bool wantsJump, bool jumpInDirection = false) {
		return HandleJumping_(this, modelState, wantsJump, jumpInDirection);
	}
};
class Phrase {
public:
	const wchar_t* english() {
		if (!this) return L"";
		static auto off = OFFSET("Rust.Localization::Phrase::english");
		return (*reinterpret_cast<String**>(this + off))->buffer;
	}
};
class Renderer_;
class SkinnedMeshRenderer;
class Wearable : public Component {
public:
	FIELD("Assembly-CSharp::Wearable::renderers", renderers, List<Renderer_*>*);
	FIELD("Assembly-CSharp::Wearable::skinnedRenderers", skinnedRenderers, List<SkinnedMeshRenderer*>*);
};
class ItemModWearable {
public:
	Wearable* targetWearable() {
		if (!this) return nullptr;
		static auto off = METHOD("Assembly-CSharp::ItemModWearable::get_targetWearable(): Wearable");
		return reinterpret_cast<Wearable * (__fastcall*)(ItemModWearable*)>(off)(this);
	}
};
class ItemDefinition : public Component {
public:
	FIELD("Assembly-CSharp::ItemDefinition::displayName", displayName, Phrase*);
	FIELD("Assembly-CSharp::ItemDefinition::itemid", itemid, int);
	FIELD("Assembly-CSharp::ItemDefinition::<ItemModWearable>k__BackingField", itemModWearable, ItemModWearable*);
	const wchar_t* shortname() {
		if (!this) return L"";
		static auto off = OFFSET("Assembly-CSharp::ItemDefinition::shortname");
		return (*reinterpret_cast<String**>(this + off))->buffer;
	}
};
class Item {
public:

	FIELD("Assembly-CSharp::Item::uid", uid, uint32_t);
	FIELD("Assembly-CSharp::Item::amount", amount, int);
	FIELD("Assembly-CSharp::Item::info", info, ItemDefinition*);
	int GetID() {
		DWORD64 Info = mem::read<DWORD64>((DWORD64)this + 0x20); // public ItemDefinition info;
		int ID = mem::read<int>(Info + 0x18); // public int itemid;
		return ID;
	}
	uintptr_t entity() {
		return mem::read<uintptr_t>(this + 0x98);
	}
	char* ClassName() {
		return (char*)mem::read<DWORD64>(mem::read<DWORD64>(this->entity()) + 0x10);
	}
	template<typename T = void*>
	T* heldEntity() {
		if (!this) return nullptr;
		static auto off = OFFSET("Assembly-CSharp::Item::heldEntity");
		return *reinterpret_cast<T**>(this + off);
	}
};
class ItemContainer {
public:
	FIELD("Assembly-CSharp::ItemContainer::itemList", itemList, List<Item*>*);
};
class PlayerInventory {
public:
	FIELD("Assembly-CSharp::PlayerInventory::containerBelt", containerBelt, ItemContainer*);
	FIELD("Assembly-CSharp::PlayerInventory::containerWear", containerWear, ItemContainer*);
	FIELD("Assembly-CSharp::PlayerInventory::containerMain", containerMain, ItemContainer*);
};
class PlayerEyes : public Component {
public:
	FIELD("Assembly-CSharp::PlayerEyes::viewOffset", viewOffset, Vector3);
	FIELD("Assembly-CSharp::PlayerEyes::<bodyRotation>k__BackingField", bodyRotation, Quaternion);
	static Vector3 EyeOffset() {
		static auto clazz = CLASS("Assembly-CSharp::PlayerEyes");
		return *reinterpret_cast<Vector3*>(std::uint64_t(clazz->static_fields));
	}
	Vector3 position() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::get_position(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Quaternion rotation() {
		if (!this) return Quaternion{};
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::get_rotation(): Quaternion");
		return reinterpret_cast<Quaternion(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Vector3 MovementForward() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::MovementForward(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Vector3 MovementRight() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::MovementRight(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Vector3 BodyForward() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::BodyForward(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(PlayerEyes*)>(off)(this);
	}
	Ray BodyRay() {
		if (!this) return Ray();
		static auto off = METHOD("Assembly-CSharp::PlayerEyes::BodyRay(): Ray");
		return reinterpret_cast<Ray(__fastcall*)(PlayerEyes*)>(off)(this);
	}

	static inline Vector3(*BodyLeanOffset_)(PlayerEyes*) = nullptr;
	Vector3 BodyLeanOffset() {
		return BodyLeanOffset_(this);
	}
	static inline void(*DoFirstPersonCamera_)(PlayerEyes*, Component*) = nullptr;
	void DoFirstPersonCamera(Component* cam) {
		return DoFirstPersonCamera_(this, cam);
	}
};
enum class PlayerFlags : int {
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
};
PlayerFlags operator &(PlayerFlags lhs, PlayerFlags rhs) {
	return static_cast<PlayerFlags> (
		static_cast<std::underlying_type<PlayerFlags>::type>(lhs) &
		static_cast<std::underlying_type<PlayerFlags>::type>(rhs)
		);
}
PlayerFlags operator ^(PlayerFlags lhs, PlayerFlags rhs) {
	return static_cast<PlayerFlags> (
		static_cast<std::underlying_type<PlayerFlags>::type>(lhs) ^
		static_cast<std::underlying_type<PlayerFlags>::type>(rhs)
		);
}
PlayerFlags operator ~(PlayerFlags rhs) {
	return static_cast<PlayerFlags> (
		~static_cast<std::underlying_type<PlayerFlags>::type>(rhs)
		);
}
PlayerFlags& operator |=(PlayerFlags& lhs, PlayerFlags rhs) {
	lhs = static_cast<PlayerFlags> (
		static_cast<std::underlying_type<PlayerFlags>::type>(lhs) |
		static_cast<std::underlying_type<PlayerFlags>::type>(rhs)
		);

	return lhs;
}
PlayerFlags& operator &=(PlayerFlags& lhs, PlayerFlags rhs) {
	lhs = static_cast<PlayerFlags> (
		static_cast<std::underlying_type<PlayerFlags>::type>(lhs) &
		static_cast<std::underlying_type<PlayerFlags>::type>(rhs)
		);

	return lhs;
}

class ModelState {
public:
	enum class Flags : uint32_t {
		Ducked = 1,
		Jumped = 2,
		OnGround = 4,
		Sleeping = 8,
		Sprinting = 16,
		OnLadder = 32,
		Flying = 64,
		Aiming = 128,
		Prone = 256,
		Mounted = 512,
		Relaxed = 1024,
		OnPhone = 2048,
	};
	FIELD("Rust.Data::ModelState::flags", flags, int);
	void set_jumped(bool value) {
		if (!this) return;
		static auto off = METHOD("Rust.Data::ModelState::set_jumped(Boolean): Void");
		return reinterpret_cast<void(__fastcall*)(ModelState*, bool)>(off)(this, value);
	}
	void set_ducked(bool value) {
		if (!this) return;
		static auto off = METHOD("Rust.Data::ModelState::set_ducked(Boolean): Void");
		return reinterpret_cast<void(__fastcall*)(ModelState*, bool)>(off)(this, value);
	}
	void set_mounted(bool value) {
		if (!this) return;
		static auto off = METHOD("Rust.Data::ModelState::set_mounted(Boolean): Void");
		return reinterpret_cast<void(__fastcall*)(ModelState*, bool)>(off)(this, value);
	}
	FIELD("Rust.Data::ModelState::poseType", poseType, int);

	bool mounted() {
		if (!this) return false;
		static auto ptr = METHOD("Rust.Data::ModelState::get_mounted(): Boolean");
		return reinterpret_cast<bool(*)(ModelState*)>(ptr)(this);
	}
	static inline void(*set_flying_)(ModelState*, bool) = nullptr;
	void set_flying(bool state) {
		set_flying_(this, state);
	}
};
class ViewmodelBob {
public:
	static inline void(*Apply_)(ViewmodelBob*, uintptr_t, float) = nullptr;
	void Apply(uintptr_t vm, float fov) {
		Apply_(this, vm, fov);
	}
};
class ViewmodelSway {
public:
	static inline void(*Apply_)(ViewmodelSway*, uintptr_t) = nullptr;
	void Apply(uintptr_t vm) {
		Apply_(this, vm);
	}
};
class ViewmodelLower {
public:
	static inline void(*Apply_)(ViewmodelLower*, uintptr_t) = nullptr;
	void Apply(uintptr_t vm) {
		Apply_(this, vm);
	}
};
class ViewmodelClothing {
public:
	FIELD("Assembly-CSharp::ViewmodelClothing::SkeletonSkins", SkeletonSkins, Array<uintptr_t>*);
	static inline void(*CopyToSkeleton_)(ViewmodelClothing*, uintptr_t, GameObject*, Item*) = nullptr;
	void CopyToSkeleton(uintptr_t skel, GameObject* parent, Item* item) {
		CopyToSkeleton_(this, skel, parent, item);
	}
};
class BaseViewModel : public Component {
public:
	static List<BaseViewModel*>* ActiveModels() {
		static auto clazz = CLASS("Assembly-CSharp::BaseViewModel");
		return *reinterpret_cast<List<BaseViewModel*>**>(std::uint64_t(clazz->static_fields) + 0x8);
	}
	FIELD("Assembly-CSharp::BaseViewModel::model", model, Model*);
};
class ViewModel : public Component {
public:
	FIELD("Assembly-CSharp::ViewModel::instance", instance, BaseViewModel*);
	FIELD("Assembly-CSharp::ViewModel::viewModelPrefab", viewModelPrefab, Component*);
	static inline void(*Play_)(ViewModel*, String, int) = nullptr;
	void Play(String name, int layer = 0) {
		Play_(this, name, layer);
	}
};
class HeldEntity : public BaseEntity {
public:
	FIELD("Assembly-CSharp::HeldEntity::viewModel", viewModel, ViewModel*);
	static inline void(*AddPunch_)(HeldEntity*, Vector3, float) = nullptr;
	void AddPunch(Vector3 amount, float duration) {
		return AddPunch_(this, amount, duration);
	}
	Item* GetItem() {
		if (!this) return nullptr;
		static auto off = METHOD("Assembly-CSharp::HeldEntity::GetItem(): Item");
		return reinterpret_cast<Item * (__fastcall*)(HeldEntity*)>(off)(this);
	}
};
class Shader {
public:
	static Shader* Find(char* name) {
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Shader::Find(String): Shader");
		return reinterpret_cast<Shader * (__fastcall*)(String*)>(off)(String::New(name));
	}
	static int PropertyToID(char* name) {
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Shader::PropertyToID(String): Int32");
		return reinterpret_cast<int(__fastcall*)(String*)>(off)(String::New(name));
	}
};
class Material {
public:
	void SetColor(int proper, Color value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::SetColor(Int32,Color): Void");
		return reinterpret_cast<void(__fastcall*)(Material*, int, Color)>(off)(this, proper, value);
	}
	void SetColor(char* proper, Color value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::SetColor(String,Color): Void");
		return reinterpret_cast<void(__fastcall*)(Material*, String*, Color)>(off)(this, String::New(proper), value);
	}
	void SetInt(char* name, int value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::SetInt(String,Int32): Void");
		return reinterpret_cast<void(__fastcall*)(Material*, String*, int)>(off)(this, String::New(name), value);
	}
	void SetFloat(char* name, float value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::SetFloat(String,Single): Void");
		return reinterpret_cast<void(__fastcall*)(Material*, String*, float)>(off)(this, String::New(name), value);
	}
	Shader* shader() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::get_shader(): Shader");
		return reinterpret_cast<Shader * (__fastcall*)(Material*)>(off)(this);
	}
	void set_shader(Shader* val) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Material::set_shader(Shader): Void");
		return reinterpret_cast<void(__fastcall*)(Material*, Shader*)>(off)(this, val);
	}
};
class Renderer_ {
public:
	Material* material() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Renderer::get_material(): Material");
		return reinterpret_cast<Material * (__fastcall*)(Renderer_*)>(off)(this);
	}
	void set_material(Material* value) {
		if (!this) return;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Renderer::set_material(Material): Void");
		return reinterpret_cast<void(__fastcall*)(Renderer_*, Material*)>(off)(this, value);
	}
	Array<Material*>* materials() {
		if (!this) return nullptr;
		static auto off = METHOD("UnityEngine.CoreModule::UnityEngine::Renderer::get_materials(): Material[]");
		return reinterpret_cast<Array<Material*>*(__fastcall*)(Renderer_*)>(off)(this);
	}
};
class GameManifest
{
public:
	static Object* GUIDToObject(String* guid) {
		static auto ptr = METHOD("Assembly-CSharp::GameManifest::GUIDToObject(String): Object");
		return reinterpret_cast<Object * (__fastcall*)(String*)>(ptr)(guid);
	}
};
template<typename T = Object>
class ResourceRef {
public:
	T* Get() {
		if (!this) return nullptr;
		String* guid = *reinterpret_cast<String**>(this + 0x10);
		T* _cachedObject = (T*)GameManifest::GUIDToObject(guid);

		return _cachedObject;
	}
};
class SkinnedMeshRenderer : public Renderer_ {
public:

};
class ItemModProjectile {
public:
	static inline float(*GetRandomVelocity_)(ItemModProjectile*) = nullptr;

	float GetRandomVelocity() {
		return GetRandomVelocity_(this);
	}
	FIELD("Assembly-CSharp::ItemModProjectile::numProjectiles", numProjectiles, int);
	FIELD("Assembly-CSharp::ItemModProjectile::projectileVelocity", projectileVelocity, float);
	FIELD("Assembly-CSharp::ItemModProjectile::projectileSpread", projectileSpread, float);
	FIELD("Assembly-CSharp::ItemModProjectile::ammoType", ammoType, int);
	FIELD("Assembly-CSharp::ItemModProjectile::projectileVelocitySpread", projectileVelocitySpread, float);
	FIELD("Assembly-CSharp::ItemModProjectile::projectileObject", projectileObject, ResourceRef<GameObject>*);
};
class StringPool {
public:
	static uint32_t Get(const char* str) {
		static auto off = METHOD("Assembly-CSharp::StringPool::Get(String): UInt32");
		return reinterpret_cast<uint32_t(__fastcall*)(String*)>(off)(String::New(str));
	}

	static String* Get(uint32_t i) {
		static auto off = METHOD("Assembly-CSharp::StringPool::Get(UInt32): String");
		return reinterpret_cast<String * (__fastcall*)(uint32_t)>(off)(i);
	}
};
class Attack;
class HitTest {
public:
	FIELD("Assembly-CSharp::HitTest::type", type, Type);
	FIELD("Assembly-CSharp::HitTest::Radius", Radius, float);
	FIELD("Assembly-CSharp::HitTest::Forgiveness", Forgiveness, float);
	FIELD("Assembly-CSharp::HitTest::MaxDistance", MaxDistance, float);
	FIELD("Assembly-CSharp::HitTest::MultiHit", MultiHit, bool);
	FIELD("Assembly-CSharp::HitTest::BestHit", BestHit, bool);
	FIELD("Assembly-CSharp::HitTest::AttackRay", AttackRay, Ray);
	FIELD("Assembly-CSharp::HitTest::DidHit", DidHit, bool);
	FIELD("Assembly-CSharp::HitTest::gameObject", gameObject, GameObject*);
	FIELD("Assembly-CSharp::HitTest::ignoreEntity", ignoreEntity, BaseEntity*);
	FIELD("Assembly-CSharp::HitTest::HitEntity", HitEntity, BaseNetworkable*);
	FIELD("Assembly-CSharp::HitTest::HitPoint", HitPoint, Vector3);
	FIELD("Assembly-CSharp::HitTest::HitNormal", HitNormal, Vector3);
	FIELD("Assembly-CSharp::HitTest::HitDistance", HitDistance, float);
	FIELD("Assembly-CSharp::HitTest::HitTransform", HitTransform, Transform*);
	FIELD("Assembly-CSharp::HitTest::HitPart", HitPart, uint32_t);
	FIELD("Assembly-CSharp::HitTest::HitMaterial", HitMaterial, String*);

	Vector3 HitPointWorld() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::HitTest::HitPointWorld(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(HitTest*)>(off)(this);
	}

	Vector3 HitNormalWorld() {
		if (!this) return Vector3::Zero();
		static auto off = METHOD("Assembly-CSharp::HitTest::HitNormalWorld(): Vector3");
		return reinterpret_cast<Vector3(__fastcall*)(HitTest*)>(off)(this);
	}
	static inline Attack* (*BuildAttackMessage_)(HitTest*) = nullptr;
	Attack* BuildAttackMessage() {
		return BuildAttackMessage_(this);
	}
};

/*class Mathf {
public:
	STATIC_FUNCTION("UnityEngine.CoreModule::Mathf::Abs(Int32): Int32", Abs, int(int));
	STATIC_FUNCTION("UnityEngine.CoreModule::Mathf::Max(Single,Single): Single", Max, float(float, float));
	STATIC_FUNCTION("Assembly-CSharp::Vector3Ex::Magnitude2D(Vector3): Single", Magnitude2D, float(Vector3));
	STATIC_FUNCTION("Assembly-CSharp::BasePlayer::GetHeight(): Single", GetHeight, float());
	STATIC_FUNCTION("Assembly-CSharp::BasePlayer::GetRadius(): Single", GetRadius, float());
};*/
class Physics {
public:
	static void IgnoreLayerCollision(int layer1, int layer2, bool ignore) {
		return reinterpret_cast<void(*)(int, int, bool)>(il2cpp_resolve_icall(xorstr_("UnityEngine.Physics::IgnoreLayerCollision")))(layer1, layer2, ignore);
	}
	STATIC_FUNCTION("Assembly-CSharp::GamePhysics::CheckCapsule(Vector3,Vector3,Single,Int32,QueryTriggerInteraction): Boolean", CheckCapsule, bool(Vector3, Vector3, float, int, QueryTriggerInteraction));
};
class unk {
public:

};
class Input {
public:
	STATIC_FUNCTION("UnityEngine.InputLegacyModule::UnityEngine::Input::GetKeyDown(KeyCode): Boolean", GetKeyDown, bool(KeyCode));
	STATIC_FUNCTION("UnityEngine.InputLegacyModule::UnityEngine::Input::GetKey(KeyCode): Boolean", GetKey, bool(KeyCode));
};
class Projectile : public Component {
public:
	FIELD("Assembly-CSharp::Projectile::swimRandom", swimRandom, float);
	FIELD("Assembly-CSharp::Projectile::drag", drag, float);
	FIELD("Assembly-CSharp::Projectile::thickness", thickness, float);
	FIELD("Assembly-CSharp::Projectile::projectileID", projectileID, int);
	FIELD("Assembly-CSharp::Projectile::mod", mod, ItemModProjectile*);
	FIELD("Assembly-CSharp::Projectile::traveledDistance", traveledDistance, float);
	FIELD("Assembly-CSharp::Projectile::initialDistance", initialDistance, float);
	FIELD("Assembly-CSharp::Projectile::ricochetChance", ricochetChance, float);
	FIELD("Assembly-CSharp::Projectile::currentPosition", currentPosition, Vector3);
	FIELD("Assembly-CSharp::Projectile::hitTest", hitTest, HitTest*);
	FIELD("Assembly-CSharp::Projectile::currentVelocity", currentVelocity, Vector3);
	FIELD("Assembly-CSharp::Projectile::gravityModifier", gravityModifier, float);
	static inline void(*Launch_)(Projectile*) = nullptr;
	void Launch() {
		return Launch_(this);
	}
	static inline void(*DoMovement_)(Projectile*, float) = nullptr;
	void DoMovement(float deltaTime) {
		return DoMovement_(this, deltaTime);
	}
	static inline void(*Update_)(Projectile*) = nullptr;
	void Update() {
		return Update_(this);
	}
	static inline void(*Retire_)(Projectile*) = nullptr;
	void Retire() {
		return Retire_(this);
	}
	static inline bool(*Refract_)(Projectile*, uint64_t&, Vector3, Vector3, float) = nullptr;
	bool Refract(uint64_t& seed, Vector3 point, Vector3 normal, float resistance) {
		return Refract_(this, seed, point, normal, resistance);
	}
	static inline void(*SetEffectScale_)(Projectile*, float) = nullptr;
	void SetEffectScale(float sca) {
		return SetEffectScale_(this, sca);
	}
	static inline bool(*DoHit_)(Projectile*, HitTest*, Vector3, Vector3) = nullptr;
	bool DoHit(HitTest* test, Vector3 point, Vector3 world) {
		return DoHit_(this, test, point, world);
	}

	bool isAuthoritative() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::Projectile::get_isAuthoritative(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(Projectile*)>(off)(this);
	}
	bool isAlive() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::Projectile::get_isAlive(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(Projectile*)>(off)(this);
	}
};
class AttackEntity : public BaseEntity {
public:
	FIELD("Assembly-CSharp::AttackEntity::lastTickTime", lastTickTime, float);
};
class BaseMelee : public AttackEntity {
public:
	static inline void (*ProcessAttack_)(BaseMelee*, HitTest*) = nullptr;
	void ProcessAttack(HitTest* test) {
		return ProcessAttack_(this, test);
	}
};
class BaseProjectile : public AttackEntity {
public:
	class Magazine {
	public:
		FIELD("Assembly-CSharp::Magazine::ammoType", ammoType, ItemDefinition*);
		FIELD("Assembly-CSharp::Magazine::contents", contents, int);
	};
	static inline Projectile* (*CreateProjectile_)(BaseProjectile*, String*, Vector3, Vector3, Vector3) = nullptr;
	Projectile* CreateProjectile(String* prefabPath, Vector3 pos, Vector3 forward, Vector3 velocity) {
		return CreateProjectile_(this, prefabPath, pos, forward, velocity);
	}
	FIELD("Assembly-CSharp::BaseProjectile::primaryMagazine", primaryMagazine, Magazine*);
	FIELD("Assembly-CSharp::BaseProjectile::projectileVelocityScale", projectileVelocityScale, float);
	FIELD("Assembly-CSharp::BaseProjectile::aimCone", aimCone, float);
	FIELD("Assembly-CSharp::BaseProjectile::hipAimCone", hipAimCone, float);
	FIELD("Assembly-CSharp::BaseProjectile::nextReloadTime", nextReloadTime, float);
	FIELD("Assembly-CSharp::BaseProjectile::reloadTime", reloadTime, float);
	FIELD("Assembly-CSharp::BaseProjectile::automatic", automatic, bool);
	FIELD("Assembly-CSharp::BaseProjectile::aimSway", aimSway, float);
	FIELD("Assembly-CSharp::BaseProjectile::aimSwaySpeed", aimSwaySpeed, float);
	FIELD("Assembly-CSharp::AttackEntity::repeatDelay", repeatDelay, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::capsuleHeightDucked", capsuleHeightDucked, float);
	FIELD("Assembly-CSharp::PlayerWalkMovement::capsuleCenterDucked", capsuleCenterDucked, float);
	FIELD("Assembly-CSharp::BowWeapon::attackReady", attackReady, float);
	FIELD("Assembly-CSharp::BowWeapon::arrowBack", arrowBack, float);

	void DoAttack() {
		if (!this) return;
		static auto off = METHOD("Assembly-CSharp::BaseProjectile::DoAttack(): Void");
		return reinterpret_cast<void(__fastcall*)(BaseProjectile*)>(off)(this);
	}
	bool Empty() {
		if (!this) return true;
		if (!this->primaryMagazine()) return true;

		return this->primaryMagazine()->contents() <= 0;
	}
	bool HasReloadCooldown() {
		return GLOBAL_TIME < this->nextReloadTime();
	}
	float CalculateCooldownTime(float nextTime, float cooldown) {
		float num = GLOBAL_TIME;
		float num2 = 0.f;

		float ret = nextTime;

		if (ret < 0.f) {
			ret = std::max(0.f, num + cooldown - num2);
		}
		else if (num - ret <= num2) {
			ret = std::min(ret + cooldown, num + cooldown);
		}
		else {
			ret = std::max(ret + cooldown, num + cooldown - num2);
		}
		return ret;
	}
};

namespace ConVar {
	class Graphics {
	public:
		static float& _fov() {
			static auto clazz = CLASS("Assembly-CSharp::ConVar::Graphics");
			return *reinterpret_cast<float*>(std::uint64_t(clazz->static_fields) + 0x18);
		}
	};
}
class CompoundBowWeapon {
public:
	float GetProjectileVelocityScale(bool getmax = false) {
		if (!this) return 0.f;
		static auto off = METHOD("Assembly-CSharp::CompoundBowWeapon::GetProjectileVelocityScale(Boolean): Single");
		return reinterpret_cast<float(__fastcall*)(CompoundBowWeapon*, bool)>(off)(this, getmax);
	}
};
class BowWeapon : public BaseProjectile {
public:
	FIELD("Assembly-CSharp::BowWeapon::attackReady", attackReady, bool);
	FIELD("Assembly-CSharp::BowWeapon::arrowBack", arrowBack, float);

	static inline void(*BowWeapon_)(BowWeapon*) = nullptr;
	void DoAttack2() {
		return BowWeapon_(this);
	}
};
class FlintStrikeWeapon : public BaseProjectile {
public:
	FIELD("Assembly-CSharp::FlintStrikeWeapon::successFraction", successFraction, float);
	FIELD("Assembly-CSharp::FlintStrikeWeapon::_didSparkThisFrame", _didSparkThisFrame, bool);

	static inline void(*DoAttack_)(FlintStrikeWeapon*) = nullptr;
	void DoAttack() {
		return DoAttack_(this);
	}
};
class PlayerModel;
class Renderer_;
class SkinnedMultiMesh {
public:
	List<Renderer_*>* Renderers() {
		if (!this) return nullptr;
		static auto off = OFFSET("Assembly-CSharp::SkinnedMultiMesh::<Renderers>k__BackingField");
		return *reinterpret_cast<List<Renderer_*>**>(this + off);
	}
	static inline void(*RebuildModel_)(SkinnedMultiMesh*, PlayerModel*, bool) = nullptr;
	void RebuildModel(PlayerModel* model, bool reset) {
		return RebuildModel_(this, model, reset);
	}
};
class SkinSet {
public:
	FIELD("Assembly-CSharp::SkinSet::BodyMaterial", BodyMaterial, Material*);
	FIELD("Assembly-CSharp::SkinSet::HeadMaterial", HeadMaterial, Material*);
	FIELD("Assembly-CSharp::SkinSet::EyeMaterial", EyeMaterial, Material*);
};
class SkinSetCollection {
public:
	FIELD("Assembly-CSharp::SkinSetCollection::Skins", Skins, Array<SkinSet*>*);
};
class PlayerModel {
public:
	Vector3 newVelocity() {
		if (!this) return Vector3::Zero();
		static auto off = OFFSET("Assembly-CSharp::PlayerModel::newVelocity");
		return *reinterpret_cast<Vector3*>(this + off);
	}
	bool isNpc() {
		if (!this) return false;
		static auto off = OFFSET("Assembly-CSharp::PlayerModel::<IsNpc>k__BackingField");
		return *reinterpret_cast<bool*>(this + off);
	}
	FIELD("Assembly-CSharp::PlayerModel::_multiMesh", _multiMesh, SkinnedMultiMesh*);
	FIELD("Assembly-CSharp::PlayerModel::MaleSkin", MaleSkin, SkinSetCollection*);
	FIELD("Assembly-CSharp::PlayerModel::FemaleSkin", FemaleSkin, SkinSetCollection*);
};
class TOD_AtmosphereParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_AtmosphereParameters::RayleighMultiplier", RayleighMultiplier, float);
	FIELD("Assembly-CSharp-firstpass::TOD_AtmosphereParameters::Fogginess", Fogginess, float);
};
class TOD_NightParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_NightParameters::AmbientMultiplier", AmbientMultiplier, float);
};
class TOD_SunParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_SunParameters::MeshSize", MeshSize, float);
	FIELD("Assembly-CSharp-firstpass::TOD_SunParameters::MeshBrightness", MeshBrightness, float);
	FIELD("Assembly-CSharp-firstpass::TOD_SunParameters::MeshContrast", MeshContrast, float);
};
class TOD_StarParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_StarParameters::Size", Size, float);
	FIELD("Assembly-CSharp-firstpass::TOD_StarParameters::Brightness", Brightness, float);
};
enum TOD_FogType {
	None = 0,
	Atmosphere = 1,
	Directional = 2,
	Gradient = 3
};
class TOD_FogParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_FogParameters::Mode", Mode, TOD_FogType);
	FIELD("Assembly-CSharp-firstpass::TOD_FogParameters::HeightBias", HeightBias, float);
};
class TOD_CloudParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_CloudParameters::Size", Size, float);
	FIELD("Assembly-CSharp-firstpass::TOD_CloudParameters::Opacity", Opacity, float);
	FIELD("Assembly-CSharp-firstpass::TOD_CloudParameters::Coverage", Coverage, float);
	FIELD("Assembly-CSharp-firstpass::TOD_CloudParameters::Brightness", Brightness, float);
};
class TOD_DayParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_DayParameters::AmbientMultiplier", AmbientMultiplier, float);
	FIELD("Assembly-CSharp-firstpass::TOD_DayParameters::ReflectionMultiplier", ReflectionMultiplier, float);
};
class TOD_CycleParameters {
public:
	FIELD("Assembly-CSharp-firstpass::TOD_CycleParameters::Hour", Hour, float);
};
class TOD_Sky {
public:
	static List<TOD_Sky*>* instances() {
		static auto clazz = CLASS("Assembly-CSharp-firstpass::TOD_Sky");
		return *reinterpret_cast<List<TOD_Sky*>**>(std::uint64_t(clazz->static_fields));
	}

	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Atmosphere", Atmosphere, TOD_AtmosphereParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Night", Night, TOD_NightParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Sun", Sun, TOD_SunParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Day", Day, TOD_DayParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Cycle", Cycle, TOD_CycleParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Stars", Stars, TOD_StarParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Clouds", Clouds, TOD_CloudParameters*);
	FIELD("Assembly-CSharp-firstpass::TOD_Sky::Fog", Fog, TOD_FogParameters*);
};

class MonoBehaviour {
public:
	static inline System::Object_* (*StartCoroutine_)(MonoBehaviour*, System::Object_*) = nullptr;
	System::Object_* StartCoroutine(System::Object_* routine) {
		return StartCoroutine_(this, routine);
	}
};
class BoneCache {
public:
	Bone* head;
	Bone* neck;
	Bone* spine4;
	Bone* spine1;
	Bone* l_upperarm;
	Bone* l_forearm;
	Bone* l_hand;
	Bone* r_upperarm;
	Bone* r_forearm;
	Bone* r_hand;
	Bone* pelvis;
	Bone* l_hip;
	Bone* l_knee;
	Bone* l_foot;
	Bone* r_hip;
	Bone* r_knee;
	Bone* r_foot;
	Bone* r_toe;
	Bone* l_toe;

	box_bounds bounds;
	Vector2 dfc;
	Vector2 forward;
	Quaternion eye_rot;

	BoneCache() {
		head = new Bone();
		neck = new Bone();
		spine4 = new Bone();
		spine1 = new Bone();
		l_upperarm = new Bone();
		l_forearm = new Bone();
		l_hand = new Bone();
		r_upperarm = new Bone();
		r_forearm = new Bone();
		r_hand = new Bone();
		pelvis = new Bone();
		l_hip = new Bone();
		l_knee = new Bone();
		l_foot = new Bone();
		r_hip = new Bone();
		r_knee = new Bone();
		r_foot = new Bone();

		bounds = { 0, 0, 0, 0 };
		dfc = Vector2();
		forward = { };
		eye_rot = { };
	}
};
class Attack {
public:
	FIELD("Rust.Data::ProtoBuf::Attack::hitID", hitID, uint32_t);
	FIELD("Rust.Data::ProtoBuf::Attack::hitBone", hitBone, uint32_t);
	FIELD("Rust.Data::ProtoBuf::Attack::hitMaterialID", hitMaterialID, uint32_t);
	FIELD("Rust.Data::ProtoBuf::Attack::hitPositionWorld", hitPositionWorld, Vector3);
	FIELD("Rust.Data::ProtoBuf::Attack::hitNormalWorld", hitNormalWorld, Vector3);
	FIELD("Rust.Data::ProtoBuf::Attack::pointStart", pointStart, Vector3);
	FIELD("Rust.Data::ProtoBuf::Attack::pointEnd", pointEnd, Vector3);
};
class PlayerAttack {
public:
	FIELD("Rust.Data::ProtoBuf::PlayerAttack::attack", attack, Attack*);
};
class PlayerProjectileAttack {
public:
	FIELD("Rust.Data::ProtoBuf::PlayerProjectileAttack::playerAttack", playerAttack, PlayerAttack*);
};
std::map<uint64_t, BoneCache*> cachedBones = std::map<uint64_t, BoneCache*>();
class InputMessage {
public:
	int& buttons() {
		return *reinterpret_cast<int*>(this + 0x14);
	}
};
enum BUTTON {
	FORWARD = 2,
	BACKWARD = 4,
	LEFT = 8,
	RIGHT = 16,
	JUMP = 32,
	DUCK = 64,
	SPRINT = 128,
	USE = 256,
	FIRE_PRIMARY = 1024,
	FIRE_SECONDARY = 2048,
	RELOAD = 8192,
	FIRE_THIRD = 134217728,
};
class InputState {
public:
	FIELD("Assembly-CSharp::InputState::current", current, InputMessage*);
	FIELD("Assembly-CSharp::InputState::previous", previous, InputMessage*);
	static inline bool(*IsDown_)(InputState*, BUTTON) = nullptr;
	bool IsDown(BUTTON btn) {
		return IsDown_(this, btn);
	}
};
class TeamMember {
public:
	bool online() {
		return *reinterpret_cast<bool*>(this + 0x38);
	}
	uint64_t& userID() {
		return *reinterpret_cast<uint64_t*>(this + 0x20);
	}
	Vector3& position() {
		return *reinterpret_cast<Vector3*>(this + 0x2C);
	}
	const wchar_t* displayName() {
		if (!this) return L"---";
		return (*reinterpret_cast<String**>(this + 0x18))->buffer;
	}
};
class PlayerTeam {
public:
	List<TeamMember*>* members() {
		return *reinterpret_cast<List<TeamMember*>**>(this + 0x30);
	}
};
class PlayerInput {
public:
	FIELD("Assembly-CSharp::PlayerInput::state", state, InputState*);
};

class PlayerTick {
public:
	Vector3 position() {
		Vector3 th = mem::read<Vector3>(this + 0x20);
		if (!th.empty()) {
			return th;
		}
		else { return Vector3::Zero(); }
	}
};
class BasePlayer;
BasePlayer* target_ply = nullptr;
class BasePlayer : public BaseCombatEntity {
public:
	static inline bool(*CanAttack_)(BasePlayer*) = nullptr;
	static inline void(*OnLand_)(BasePlayer*, float) = nullptr;
	static inline void(*ClientInput_)(BasePlayer*, uintptr_t) = nullptr;
	void OnLand(float fVelocity) {
		return OnLand_(this, fVelocity);
	}
	void ClientInput(uintptr_t unk) {
		return ClientInput_(this, unk);
	}
	bool CanAttack() {
		return CanAttack_(this);
	}
	static inline void(*ClientUpdate_)(BasePlayer*) = nullptr;
	void ClientUpdate() {
		return ClientUpdate_(this);
	}
	static inline void(*ClientUpdate_Sleeping_)(BasePlayer*) = nullptr;
	void ClientUpdate_Sleeping() {
		return ClientUpdate_Sleeping_(this);
	}
	static inline void(*SendProjectileAttack_)(BasePlayer*, PlayerProjectileAttack*) = nullptr;
	void SendProjectileAttack(PlayerProjectileAttack* attack) {
		return SendProjectileAttack_(this, attack);
	}

	void set_viewangles(const Vector2& VA) {
		DWORD64 Input = mem::read<DWORD64>(this + 0x4E0);//	public PlayerInput input;
		write(Input + 0x3C, VA, Vector2);
	}
	Vector2 viewangles() {
		DWORD64 Input = mem::read<DWORD64>(this + 0x4E0);//	public PlayerInput input;
		return  mem::read<Vector2>(Input + 0x3C);
	}
	const wchar_t* _displayName() {
		if (!this) return L"";
		static auto off = OFFSET("Assembly-CSharp::BasePlayer::_displayName");
		return (*reinterpret_cast<String**>(this + off))->buffer;
	}
	//FIELD("Assembly-CSharp::Weather::set_atmosphere_rayleigh", set_atmosphere_rayleigh, float);

	FIELD("Assembly-CSharp::BasePlayer::clothingWaterSpeedBonus", clothingWaterSpeedBonus, float);
	FIELD("Assembly-CSharp::BasePlayer::userID", userID, uint64_t);
	FIELD("Assembly-CSharp::BasePlayer::mounted", mounted, BaseMountable*);
	FIELD("Assembly-CSharp::BasePlayer::movement", movement, PlayerWalkMovement*);
	FIELD("Assembly-CSharp::BasePlayer::modelState", modelState, ModelState*);
	FIELD("Assembly-CSharp::BasePlayer::playerModel", playerModel, PlayerModel*);
	FIELD("Assembly-CSharp::BasePlayer::input", input, PlayerInput*);
	FIELD("Assembly-CSharp::BasePlayer::clientTeam", clientTeam, PlayerTeam*);
	FIELD("Assembly-CSharp::BasePlayer::playerFlags", playerFlags, PlayerFlags);
	FIELD("Assembly-CSharp::BasePlayer::inventory", inventory, PlayerInventory*);
	FIELD("Assembly-CSharp::BasePlayer::clActiveItem", clActiveItem, uint32_t);
	FIELD("Assembly-CSharp::BasePlayer::maxProjectileID", maxProjectileID, int);
	FIELD("Assembly-CSharp::BasePlayer::eyes", eyes, PlayerEyes*);
	FIELD("Assembly-CSharp::BasePlayer::lastSentTick", lastSentTick, PlayerTick*);
	FIELD("Assembly-CSharp::BasePlayer::lastHeadshotSoundTime", lastHeadshotSoundTime, float);
	FIELD("Assembly-CSharp::BasePlayer::lastSentTickTime", lastSentTickTime, float);
	FIELD("Assembly-CSharp::BasePlayer::clientTickInterval", clientTickInterval, float);
	Item* GetWeaponInfo(int Id) {
		DWORD64 Inventory = mem::read<DWORD64>(this + 0x690);
		DWORD64 Belt = mem::read<DWORD64>(Inventory + 0x28); // containerBelt
		DWORD64 ItemList = mem::read<DWORD64>(Belt + 0x38);// public List<Item> itemList;
		DWORD64 Items = mem::read<DWORD64>(ItemList + 0x10); //	public List<InventoryItem.Amount> items;
		return (Item*)mem::read<DWORD64>(Items + 0x20 + (Id * 0x8));
	}
	Item* GetActiveWeapon() {
		int ActUID = mem::read<int>(this + 0x5B0);
		if (!ActUID) return nullptr;
		Item* ActiveWeapon;
		for (int i = 0; i < 6; i++)
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->uid())
				return ActiveWeapon;
		return nullptr;
	}
	bool isNpc() {
		if (!this) return false;
		static auto off = OFFSET("Assembly-CSharp::PlayerModel::<IsNpc>k__BackingField");
		return *reinterpret_cast<bool*>(this + off);
	}
	ULONG64 mono_transform(int bone) {

		DWORD64 entity_model = mem::read<DWORD64>(this + 0x120);
		DWORD64 bone_dict = mem::read<DWORD64>(entity_model + 0x48);
		DWORD64 val = mem::read<DWORD64>(bone_dict + 0x20 + bone * 0x8);
		return val;
	}
	bool IsDucked() { // lad don't fancy calling functions in a non-game thread, eh, thy lad shall recreate it.
		if (!this) return false;

		if (this->movement() != nullptr)
			return this->movement()->Ducking() > 0.5f;

		return this->modelState() != nullptr && this->modelState()->flags() & 1;
	}
	Vector3 get_bone_pos(Bone_List bone) {
		uintptr_t player_model = mem::read<uintptr_t>(this + 0x128);// public Model model
		uintptr_t boneTransforms = mem::read<uintptr_t>(player_model + 0x48);//public Transform[] boneTransforms;
		Transform* transform = mem::read<Transform*>(boneTransforms + 0x20);
		//const Transform* transform = read(BoneValue + 0x10, Transform*);

		if (!transform)
			return Vector3::Zero();
		return transform->position();
	}
	Bone* find_mpv_bone() {
		if (!this)
			return nullptr;

		if (!this->isCached())
			return nullptr;

		auto bones = this->bones();

		if (bones->head->visible)
			return bones->head;

		if (bones->neck->visible)
			return bones->neck;

		if (bones->spine1->visible)
			return bones->spine1;

		if (bones->spine4->visible)
			return bones->spine4;

		if (bones->l_hand->visible)
			return bones->l_hand;

		if (bones->r_hand->visible)
			return bones->r_hand;

		if (bones->l_forearm->visible)
			return bones->l_forearm;

		if (bones->r_forearm->visible)
			return bones->r_forearm;

		if (bones->pelvis->visible)
			return bones->pelvis;

		if (bones->l_knee->visible)
			return bones->l_knee;

		if (bones->r_knee->visible)
			return bones->r_knee;

		if (bones->l_foot->visible)
			return bones->l_foot;

		if (bones->r_foot->visible)
			return bones->r_foot;

		return bones->head;
	}

	// ret type is bone, found

	void add_modelstate_flag(ModelState::Flags flag) {
		int flags = this->modelState()->flags();

		this->modelState()->flags() = flags |= (int)flag;
	}
	bool HasPlayerFlag(PlayerFlags flag) {
		if (!this) return false;

		return (playerFlags() & flag) == flag;
	}
	bool OnLadder() {
		if (!this) return false;
		static auto off = METHOD("Assembly-CSharp::BasePlayer::OnLadder(): Boolean");
		return reinterpret_cast<bool(__fastcall*)(BasePlayer*)>(off)(this);
	}
	float GetRadius() {
		if (!this) return 0.f;
		static auto off = METHOD("Assembly-CSharp::BasePlayer::GetRadius(): Single");
		return reinterpret_cast<float(__fastcall*)(BasePlayer*)>(off)(this);
	}
	float GetJumpHeight() {
		if (!this) return 0.f;
		static auto off = METHOD("Assembly-CSharp::BasePlayer::GetJumpHeight(): Single");
		return reinterpret_cast<float(__fastcall*)(BasePlayer*)>(off)(this);
	}
	float GetMaxSpeed() {
		if (!this) return 0.f;
		static auto off = METHOD("Assembly-CSharp::BasePlayer::GetMaxSpeed(): Single");
		return reinterpret_cast<float(__fastcall*)(BasePlayer*)>(off)(this);
	}
	float MaxVelocity() {
		if (!this) return 0.f;

		if (this->mounted())
			return this->GetMaxSpeed() * 4;

		return this->GetMaxSpeed();
	}
	float GetHeight(bool ducked) {
		if (!this) return 0.f;
		static auto off = METHOD("Assembly-CSharp::BasePlayer::GetHeight(Boolean): Single");
		return reinterpret_cast<float(__fastcall*)(BasePlayer*, bool)>(off)(this, ducked);
	}
	static ListDictionary* visiblePlayerList() {
		static auto clazz = CLASS("Assembly-CSharp::BasePlayer");
		return *reinterpret_cast<ListDictionary**>(std::uint64_t(clazz->static_fields) + 0x8);
	}
	bool in_minicopter() {
		if (!this)
			return false;

		if (!this->mounted())
			return false;

		if (this->mounted()->GetVehicleParent()->class_name_hash() == STATIC_CRC32("MiniCopter")) {
			return true;
		}

		return false;
	}
	bool is_target() {
		if (!target_ply)
			return false;

		if (!this)
			return false;

		return this->userID() == target_ply->userID();
	}
	bool isCached() {
		if (!this)
			return false;

		return map_contains_key(cachedBones, this->userID());
	}

	bool is_teammate() {
		if (!this)
			return false;

		auto team = LocalPlayer::Entity()->clientTeam();
		if (team) {
			auto list = team->members();
			if (list) {
				for (int i = 0; i < list->size; i++) {
					auto member = reinterpret_cast<TeamMember*>(list->get(i));
					if (!member) continue;

					if (member->userID() == this->userID()) {
						return true;
					}
				}
			}
		}

		return false;
	}
	Vector3 midPoint() {
		if (!this->isCached())
			return Vector3::Zero();

		return this->bones()->r_foot->position.midPoint(this->bones()->l_foot->position) - Vector3(0.0f, 0.1f, 0.0f);
	}
	bool out_of_fov() {
		if (!this->isCached())
			return true;

		return this->bones()->dfc.distance(screen_center) > 1000.f;
	}
	bool is_visible() {
		if (!this->isCached())
			return false;

		if (cachedBones[this->userID()]->head->visible ||
			cachedBones[this->userID()]->neck->visible ||
			cachedBones[this->userID()]->spine4->visible ||
			cachedBones[this->userID()]->pelvis->visible ||
			cachedBones[this->userID()]->r_foot->visible ||
			cachedBones[this->userID()]->l_foot->visible ||
			cachedBones[this->userID()]->r_knee->visible ||
			cachedBones[this->userID()]->l_knee->visible) {

			return true;
		}

		return false;
	}


	BoneCache* bones() {
		return this->isCached() ? cachedBones[this->userID()] : new BoneCache();
	}
	template<typename T = HeldEntity>
	T* GetHeldEntity() {
		if (!this) return nullptr;

		auto inventory = this->inventory();
		if (!inventory) return nullptr;

		auto belt = inventory->containerBelt();
		if (!belt) return nullptr;

		auto item_list = belt->itemList();
		if (!item_list) return nullptr;

		for (int i = 0; i < item_list->size; i++) {
			auto item = reinterpret_cast<Item*>(item_list->get(i));
			if (!item) continue;

			if (item->uid() == this->clActiveItem())
				return item->heldEntity<T>();
		}

		return nullptr;
	}
	Item* GetHeldItem() {
		if (!this) return nullptr;

		auto inventory = this->inventory();
		if (!inventory) return nullptr;

		auto belt = inventory->containerBelt();
		if (!belt) return nullptr;

		auto item_list = belt->itemList();
		if (!item_list) return nullptr;

		for (int i = 0; i < item_list->size; i++) {
			auto item = reinterpret_cast<Item*>(item_list->get(i));
			if (!item) continue;

			if (item->uid() == this->clActiveItem())
				return item;
		}

		return nullptr;
	}
};
class Vector3_ {
public:
	static inline Vector3(*MoveTowards_)(Vector3, Vector3, float) = nullptr;

	static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {
		return MoveTowards_(current, target, maxDistanceDelta);
	}
};
class DDraw {
public:
	STATIC_FUNCTION("Assembly-CSharp::UnityEngine::DDraw::Line(Vector3,Vector3,Color,Single,Boolean,Boolean): Void", Line, void(Vector3, Vector3, Color, float, bool, bool));
	STATIC_FUNCTION("Assembly-CSharp::UnityEngine::DDraw::Sphere(Vector3,Single,Color,Single,Boolean): Void", Sphere, void(Vector3, float, Color, float, bool));
};
class AssetBundle {
public:
	Array<String*>* GetAllAssetNames() {
		if (!this) return {};
		static auto off = METHOD("UnityEngine.AssetBundleModule::UnityEngine::AssetBundle::GetAllAssetNames(): String[]");
		return reinterpret_cast<Array<String*>*(*)(AssetBundle*)>(off)(this);
	}
	template<typename T = Object>
	T* LoadAsset(char* name, Type* type) {
		if (!this) return {};
		static auto off = METHOD("UnityEngine.AssetBundleModule::UnityEngine::AssetBundle::LoadAsset(String,Type): Object");
		return reinterpret_cast<T * (*)(AssetBundle*, String*, Type*)>(off)(this, String::New(name), type);
	}
	static AssetBundle* LoadFromFile(char* path) {
		static auto off = METHOD("UnityEngine.AssetBundleModule::UnityEngine::AssetBundle::LoadFromFile(String): AssetBundle");
		return reinterpret_cast<AssetBundle * (*)(String*)>(off)(String::New(path));
	}
};
std::array<int, 20> valid_bones = {
		1, 2, 3, 5, 6, 14, 15, 17, 18, 21, 23, 24, 25, 26, 27, 48, 55, 56, 57, 76
};
class Model : public Component {
public:
	FIELD("Assembly-CSharp::Model::boneTransforms", boneTransforms, Array<Transform*>*);
	FIELD("Assembly-CSharp::Model::boneNames", boneNames, Array<String*>*);

	Bone* resolve(uint32_t hash) {
		if (!this) return nullptr;

		if (!this->boneNames() || !this->boneTransforms()) return nullptr;

		auto bone_names = this->boneNames();
		auto bone_transforms = this->boneTransforms();

		for (int i = 0; i < bone_names->size(); i++) {
			auto bone_name = bone_names->get(i);
			auto bone_transform = bone_transforms->get(i);
			if (!bone_name || !bone_transform) continue;

			if (RUNTIME_CRC32_W(bone_name->buffer) == hash) {
				Vector3 ret = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
				return new Bone(bone_transform->position(), LineOfSight(bone_transform->position(), ret), bone_transform);
			}
		}

		return nullptr;
	}

	std::pair<Transform*, bool> find_bone(Vector3 from) {
		std::pair<Transform*, bool> ret = std::pair<Transform*, bool>(nullptr, false);

		if (!this)
			return ret;

		std::vector<std::pair<Transform*, float>> distances = std::vector<std::pair<Transform*, float>>();

		auto arr = this->boneTransforms();
		if (!arr)
			return ret;

		for (auto j : valid_bones) {
			auto bone = arr->get(j);
			if (!bone)
				continue;

			float dist = bone->position().distance(from);

			distances.push_back({ bone, dist });
		}


		// find smallest from float (second)
		std::pair<Transform*, float> temp = { nullptr, 99999.f };
		for (int i = 0; i < distances.size(); i++) {
			if (distances[i].second < temp.second) {
				temp.first = distances[i].first;
				temp.second = distances[i].second;
			}
		}

		ret.first = temp.first;
		ret.second = true;

		return ret;
	}
};
namespace Network {
	class Client {
	public:
		bool IsConnected() {
			if (!this) return false;
			static auto off = METHOD("Facepunch.Network::Network::Client::IsConnected(): Boolean");
			return reinterpret_cast<bool(__fastcall*)(Client*)>(off)(this);
		}
		String* ConnectedAddress() {
			return *reinterpret_cast<String**>(this + 0x40);
		}
	};
	class Net {
	public:
		static Client* cl() {
			static auto clazz = CLASS("Facepunch.Network::Network::Net");
			return *reinterpret_cast<Client**>(std::uint64_t(clazz->static_fields));
		}
	};
}
class AimConeUtil {
public:
	static inline Vector3(*GetModifiedAimConeDirection_)(float, Vector3, bool) = nullptr;
	static Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
		return GetModifiedAimConeDirection_(aimCone, inputVec, anywhereInside);
	}
};
class SilentVector {
public:
	static inline void(*SendProjectileAttack_)(PlayerProjectileAttack*) = nullptr;
	static void SendProjectileAttack(PlayerProjectileAttack* attack) {
		return SendProjectileAttack_(attack);
	}

};
Matrix viewMatrix = {};
class Camera {
public:
	static char* memstr(char* haystack, const char* needle, int size) {
		char* p;
		char needlesize = strlen(needle);

		for (p = haystack; p <= (haystack - needlesize + size); p++) {
			if (memcmp(p, needle, needlesize) == 0)
				return p; /* found */
		}

		return NULL;
	}
	static uint64_t GetCamera() {
		const auto base = (uint64_t)GetModuleHandleA(xorstr_("UnityPlayer.dll"));

		if (!base)
			return 0;

		const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
		const auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos_header->e_lfanew);

		uint64_t data_base;
		uint64_t data_size;

		for (int i = 0;;) {
			const auto section = reinterpret_cast<IMAGE_SECTION_HEADER*>(
				base + dos_header->e_lfanew + // nt_header base 
				sizeof(IMAGE_NT_HEADERS64) +  // start of section headers
				(i * sizeof(IMAGE_SECTION_HEADER))); // section header at our index

			if (strcmp((char*)section->Name, xorstr_(".data")) == 0) {
				data_base = section->VirtualAddress + base;
				data_size = section->SizeOfRawData;
				break;
			}

			i++;

			if (i >= nt_header->FileHeader.NumberOfSections) {
				return 0;
			}
		}

		uint64_t camera_table = 0;

		const auto camera_string = memstr((char*)data_base, xorstr_("AllCameras"), data_size);
		for (auto walker = (uint64_t*)camera_string; walker -= 0; walker -= 1) {
			if (*walker > 0x100000 && *walker < 0xF00000000000000) {
				// [[[[unityplayer.dll + ctable offset]]] + 0x30] = Camera
				camera_table = *walker;
				break;
			}
		}

		if (camera_table)
			return camera_table;

		return 0;
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

	static Matrix getViewMatrix() {
		static auto camera_list = GetCamera();
		if (!camera_list) return Matrix();

		auto camera_table = *reinterpret_cast<uint64_t*>(camera_list);
		auto cam = *reinterpret_cast<uint64_t*>(camera_table);

		return *reinterpret_cast<Matrix*>(cam + 0x2E4);
	}
};

typedef void(__fastcall* viewmodelPlay)(ViewModel*, pUncStr, int);
inline viewmodelPlay original_viewmodelplay{ };

void initialize_cheat() {//bool = Boolean , int = Int32 , float = Single , Vector3 = Vector3 , string = String
	//ASSIGN_HOOK("Assembly-CSharp::ViewModel::Play(String,Int32): Void", ViewModel::Play_);
	ASSIGN_HOOK("Assembly-CSharp::ViewModel::Play(String,Int32): Void", original_viewmodelplay);
	ASSIGN_HOOK("Assembly-CSharp::BasePlayer::ClientUpdate(): Void", BasePlayer::ClientUpdate_);
	ASSIGN_HOOK("Assembly-CSharp::BasePlayer::ClientUpdate_Sleeping(): Void", BasePlayer::ClientUpdate_Sleeping_);
	ASSIGN_HOOK("Assembly-CSharp::HitTest::BuildAttackMessage(): Attack", HitTest::BuildAttackMessage_);
	ASSIGN_HOOK("Assembly-CSharp::PlayerWalkMovement::HandleJumping(ModelState,Boolean,Boolean): Void", PlayerWalkMovement::HandleJumping_);
	ASSIGN_HOOK("Assembly-CSharp::PlayerEyes::DoFirstPersonCamera(Camera): Void", PlayerEyes::DoFirstPersonCamera_);
	ASSIGN_HOOK("Assembly-CSharp::BasePlayer::ClientInput(InputState): Void", BasePlayer::ClientInput_);
	ASSIGN_HOOK("Assembly-CSharp::Projectile::DoMovement(Single): Void", Projectile::DoMovement_);
	ASSIGN_HOOK("Assembly-CSharp::ViewmodelSway::Apply(CachedTransform<BaseViewModel>&): Void", ViewmodelSway::Apply_);
	ASSIGN_HOOK("Assembly-CSharp::ViewmodelBob::Apply(CachedTransform<BaseViewModel>&,Single): Void", ViewmodelBob::Apply_);
	ASSIGN_HOOK("Assembly-CSharp::ViewmodelLower::Apply(CachedTransform<BaseViewModel>&): Void", ViewmodelLower::Apply_);
	ASSIGN_HOOK("Assembly-CSharp::Projectile::DoHit(HitTest,Vector3,Vector3): Boolean", Projectile::DoHit_);
	ASSIGN_HOOK("Assembly-CSharp::Projectile::SetEffectScale(Single): Void", Projectile::SetEffectScale_);
	ASSIGN_HOOK("Facepunch.Console::ConsoleSystem::Run(Option,String,Object[]): String", ConsoleSystem::Run_);
	ASSIGN_HOOK("Rust.Data::ModelState::set_flying(Boolean): Void", ModelState::set_flying_);
	ASSIGN_HOOK("Assembly-CSharp::BasePlayer::OnLand(Single): Void", BasePlayer::OnLand_);
	ASSIGN_HOOK("Assembly-CSharp::BaseMountable::EyePositionForPlayer(BasePlayer,Quaternion): Vector3", BaseMountable::EyePositionForPlayer_);
	ASSIGN_HOOK("Assembly-CSharp::SkinnedMultiMesh::RebuildModel(PlayerModel,Boolean): Void", SkinnedMultiMesh::RebuildModel_);
	ASSIGN_HOOK("Assembly-CSharp::FlintStrikeWeapon::DoAttack(): Void", FlintStrikeWeapon::DoAttack_);
	ASSIGN_HOOK("Assembly-CSharp::BaseCombatEntity::OnAttacked(HitInfo): Void", BaseCombatEntity::OnAttacked_);
	ASSIGN_HOOK("Assembly-CSharp::InputState::IsDown(BUTTON): Boolean", InputState::IsDown_);
	ASSIGN_HOOK("Assembly-CSharp::PlayerEyes::get_BodyLeanOffset(): Vector3", PlayerEyes::BodyLeanOffset_);
	ASSIGN_HOOK("Assembly-CSharp::BaseProjectile::CreateProjectile(String,Vector3,Vector3,Vector3): Projectile", BaseProjectile::CreateProjectile_);
	ASSIGN_HOOK("Assembly-CSharp::PlayerWalkMovement::UpdateVelocity(): Void", PlayerWalkMovement::UpdateVelocity_);
	ASSIGN_HOOK("Assembly-CSharp::ItemModProjectile::GetRandomVelocity(): Single", ItemModProjectile::GetRandomVelocity_);
	ASSIGN_HOOK("Assembly-CSharp::BasePlayer::CanAttack(): Boolean", BasePlayer::CanAttack_);
	ASSIGN_HOOK("Assembly-CSharp::AimConeUtil::GetModifiedAimConeDirection(Single,Vector3,Boolean): Vector3", AimConeUtil::GetModifiedAimConeDirection_);
	ASSIGN_HOOK("Assembly-CSharp::HeldEntity::AddPunch(Vector3,Single): Void", HeldEntity::AddPunch_);
	ASSIGN_HOOK("UnityEngine.CoreModule::UnityEngine::Vector3::MoveTowards(Vector3,Vector3,Single): Vector3", Vector3_::MoveTowards_);
}