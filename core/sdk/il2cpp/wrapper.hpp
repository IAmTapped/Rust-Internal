#pragma once



auto gameAssembly = GetModuleHandleA(xorstr_("GameAssembly.dll"));

#define ProcAddr(func) GetProcAddress(gameAssembly, func)

template<typename T, typename... Args>
inline T call(const char* func, Args... args) {
	return reinterpret_cast<T(__fastcall*)(Args...)>(ProcAddr(func))(args...);
}

namespace System {
	class Object_ {
	public:

	};
	template<typename T = void*>
	class Array {
	public:
		uint32_t size( ) {
			if (!this) return 0;
			return *reinterpret_cast<uint32_t*>(this + 0x18);
		}
		T get(int idx) {
			if (!this) return T{};
			return *reinterpret_cast<T*>(this + (0x20 + (idx * 0x8)));
		}
		void add(int idx, T value) {
			if (!this) return;
			*reinterpret_cast<T*>(this + (0x20 + (idx * 0x8))) = value;
		}
	};
	class String : public Object_ {
	public:
		char pad_0000[ 0x10 ];
		int len;
		wchar_t buffer[ 0 ];

		static String* New(const char* str) {
			return call<String*, const char*>(xorstr_("il2cpp_string_new"), str);
		}
	};
	template<typename T = void*>
	struct List {
	public:
		char pad_0000[ 0x10 ];
		void* buffer;
		uint32_t size;

		T* get(uint32_t idx) {
			if (!this) return nullptr;

			if (idx > this->size) return nullptr;

			void* items = this->buffer;

			if (!items) return nullptr;

			return *reinterpret_cast<T**>((uint64_t)items + (0x20 + (idx * 0x8)));
		}
	};
}

class Il2CppType
{
public:
	char* name( ) {
		return call<char*, Il2CppType*>(xorstr_("il2cpp_type_get_name"), this);
	}
};

class Il2CppMethod {
public:
	uint32_t paramCount( ) {
		return call<uint32_t, Il2CppMethod*>(xorstr_("il2cpp_method_get_param_count"), this);
	}

	Il2CppType* retType( ) {
		return call<Il2CppType*, Il2CppMethod*>(xorstr_("il2cpp_method_get_return_type"), this);
	}

	Il2CppType* getParam(uint32_t idx) {
		return call<Il2CppType*, Il2CppMethod*, uint32_t>(xorstr_("il2cpp_method_get_param"), this, idx);
	}

	char* name( ) {
		return call<char*, Il2CppMethod*>(xorstr_("il2cpp_method_get_name"), this);
	}
};

uint64_t il2cpp_resolve_icall(const char* str) {
	return call<uint64_t, const char*>(xorstr_("il2cpp_resolve_icall"), str);
}
uint64_t il2cpp_object_new(uint64_t klass) {
	return call<uint64_t, uint64_t>(xorstr_("il2cpp_object_new"), klass);
}
class Il2CppField {
public:
	char* name( ) {
		return call<char*, Il2CppField*>(xorstr_("il2cpp_field_get_name"), this);
	}

	uint32_t offset( ) {
		return call<uint32_t, Il2CppField*>(xorstr_("il2cpp_field_get_offset"), this);
	}
};

class Il2CppClass
{
public:
	class Il2CppImage* image; //0x0000
	char pad_0008[ 8 ]; //0x0008
	char* name; //0x0010
	char* namespaze; //0x0018
	char pad_0020[ 152 ]; //0x0020
	void* static_fields; //0x00B8

	Il2CppMethod* methods(void* iter) {
		return call<Il2CppMethod*, Il2CppClass*, void*>(xorstr_("il2cpp_class_get_methods"), this, iter);
	}

	Il2CppField* fields(void* iter) {
		return call<Il2CppField*, Il2CppClass*, void*>(xorstr_("il2cpp_class_get_fields"), this, iter);
	}
}; //Size: 0x00C0

class Il2CppImage
{
public:
	char* assemblyName; //0x0000
	char* name; //0x0008

	uint64_t classcount( ) {
		return call<uint64_t, Il2CppImage*>(xorstr_("il2cpp_image_get_class_count"), this);
	}

	Il2CppClass* get_class(uint64_t idx) {
		return call<Il2CppClass*, Il2CppImage*, uint64_t>(xorstr_("il2cpp_image_get_class"), this, idx);
	}
}; //Size: 0x0010
template<typename T = System::Object_>
System::Array<T*>* il2cpp_array_new(Il2CppClass* klazz, uint64_t length) {
	auto ret = call<System::Array<T*>*, Il2CppClass*, uint64_t>(xorstr_("il2cpp_array_new"), klazz, length);
	return ret;
}

class Il2CppAssembly {
public:
	uint64_t buffer;
};

class Il2CppDomain {
public:
	size_t assemblyCount( ) {
		size_t size = 0;
		auto assemblies = call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr_("il2cpp_domain_get_assemblies"), this, &size);

		return size;

	}

	Il2CppAssembly** assemblies( ) {
		size_t size = 0;

		return call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr_("il2cpp_domain_get_assemblies"), this, &size);
	}
};

Il2CppDomain* il2cpp_domain_get( ) {
	return call<Il2CppDomain*>(xorstr_("il2cpp_domain_get"));
}

void* il2cpp_runtime_invoke(void* method_ptr, void* obj, void** param, void** exc) {
	return call<void*, void*, void*, void**, void**>("il2cpp_runtime_invoke", method_ptr, obj, param, exc);
}

void* il2cpp_object_get_virtual_method(void* obj, void* method) {
	return call<void*, void*, void*>("il2cpp_object_get_virtual_method", obj, method);
}