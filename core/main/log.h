#include <winnt.h>
#include <string>
#include <WinUser.h>

float timee = 120.f;
bool sdk_initialized = false;
bool timer_initialized = false;
float timeSinceStartup = 0;
float timeFrequency = 0;
float get_time_since_startup() {
	LARGE_INTEGER PerformanceCount;
	LARGE_INTEGER FrequencyCount;

	if (!timer_initialized) {
		timer_initialized = true;

		PerformanceCount.QuadPart = 0;
		QueryPerformanceCounter(&PerformanceCount);

		FrequencyCount.QuadPart = 0;
		QueryPerformanceFrequency(&FrequencyCount);

		timeFrequency = float(FrequencyCount.QuadPart);

		timeSinceStartup = float(PerformanceCount.QuadPart);
	}

	PerformanceCount.QuadPart = 0;
	QueryPerformanceCounter(&PerformanceCount);

	return float(PerformanceCount.QuadPart - timeSinceStartup) / timeFrequency;
}
class BulletTracer {
public:
	Vector3 start;
	Vector3 end;
	BulletTracer() {
		this->start = Vector3::Zero();
		this->end = Vector3::Zero();
	}
	BulletTracer(Vector3 st, Vector3 en) {
		this->start = st;
		this->end = en;
	}
};
struct Explosion {
public:
	std::string name;
	float timeSince;
	Vector3 position;
};
__forceinline float Calc3D_Dist(const Vector3& Src, const Vector3& Dst) {
	return sqrtf(pow((Src.x - Dst.x), 2) + pow((Src.y - Dst.y), 2) + pow((Src.z - Dst.z), 2));
}

float x, y, z;

class LogSystem {
public:
	static inline int max_entries = 10;

	static void draw_text(Vector2, std::wstring);
	static void draw_line(Vector2, Vector2);

	struct LogEntry {
	public:
		std::wstring message;
		float startedAt;
		float duration;

		LogEntry(std::wstring message, float duration) {
			this->message = message;
			this->duration = duration;
			this->startedAt = get_time_since_startup();
		}
	};
	static inline std::vector<BulletTracer> bulletTracers = std::vector<BulletTracer>();
	static inline std::vector<LogEntry> logs = std::vector<LogEntry>();
	static inline std::vector<Explosion> loggedExplosions = std::vector<Explosion>();
	static void Log(std::wstring message, float duration) {
		if (logs.size() >= max_entries)
			logs.erase(logs.begin());

		logs.push_back(LogEntry(message, duration));
	}
	
	static void RenderTracers() {
		for (int i = 0; i < bulletTracers.size(); i++) {
			BulletTracer tracer = bulletTracers[i];
			auto entityList = BaseNetworkable::clientEntities()->entityList();
			for (int i = 0; i < entityList->vals->size; i++) {
				auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				auto screen = reinterpret_cast<Camera*>(entity);

				Vector2 s_pos_start; Vector2 s_pos_end;
				if (screen->world_to_screen(tracer.start, s_pos_start) && screen->world_to_screen(tracer.end, s_pos_end)) {
					draw_line(s_pos_start, s_pos_end);
				}
			}
		}
	}
	static void Render() {
		float yPos = 25.0f;
		for (int i = 0; i < logs.size(); i++) {
			LogEntry entry = logs[i];
			if ((get_time_since_startup() - entry.startedAt) >= entry.duration) {
				logs.erase(logs.begin() + i);
				continue;
			}
			Renderer::rectangle_filled({ 20, yPos - 8.f }, Vector2(400, 16), Color3(0, 0, 0, 174));
			draw_text(Vector2(225, yPos), entry.message);
			yPos += 15.0f;
		}
	}
	static void LogExplosion(std::string type, Vector3 pos) {
		bool explosionCollision = false;
		std::vector<Explosion>::iterator it;
		for (it = loggedExplosions.begin(); it != loggedExplosions.end(); it++) {
			Vector2 explPos;
			if (it->position.distance(pos) <= 25.0f) {
				explosionCollision = true;
				break;
			}
		}
		if (!explosionCollision) {
			Explosion explosion = Explosion();
			explosion.name = StringFormat::format(xorstr_("%s Raid"), type.c_str());
			explosion.position = pos;
			explosion.timeSince = get_time_since_startup();
			loggedExplosions.push_back(explosion);
		}
	}
	static void RenderExplosions() {
		for (int i = 0; i < LogSystem::loggedExplosions.size(); i++) {
			if ((get_time_since_startup() - LogSystem::loggedExplosions[i].timeSince) >= timee) {
				LogSystem::loggedExplosions.erase(LogSystem::loggedExplosions.begin() + i);
				continue;
			}
			Explosion explosion = LogSystem::loggedExplosions.at(i);

			Vector2 explPos;
			auto entityList = BaseNetworkable::clientEntities()->entityList();
			for (int i = 0; i < entityList->vals->size; i++) {
				auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				auto screen = reinterpret_cast<Camera*>(entity);

				if (screen->world_to_screen(explosion.position, explPos)) {
					
					
					
					
					
					(
						explPos,
						D2D1::ColorF::Red,
						true,
						true,
						StringConverter::ToUnicode(StringFormat::format(xorstr_("%s [%.2fm] [%d]"),explosion.name.c_str(),Calc3D_Dist(explosion.position, LocalPlayer::Entity()->get_bone_pos(head)),(int)(timee - (get_time_since_startup() - LogSystem::loggedExplosions[i].timeSince)))).c_str()
					);
				}
			}
	
		}
	}
};
void LogSystem::draw_text(Vector2 pos, std::wstring str) {
	//Renderer::t(pos, D2D1::ColorF::White, false, true, str.c_str());
	Renderer::t(pos, Color3(255, 255, 255), 12.f, false, true, str.c_str());
}

void LogSystem::draw_line(Vector2 pos, Vector2 pos2) {
	Renderer::line(pos, pos2, Color3(255, 0, 0), 1.f, true);
}

