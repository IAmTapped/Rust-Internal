#pragma once
class default_t
{
public:
	template<typename T>
	operator T( ) const { return T( ); }
};
default_t const defaultt = default_t( );
#define NP(type) type nonptr = defaultt; if(!this) return nonptr;

static std::map<uint32_t, uint64_t> offsets = std::map<uint32_t, uint64_t>( );

Il2CppClass* klass(uint32_t path) {
	if (map_contains_key(offsets, path))
		return reinterpret_cast<Il2CppClass*>(offsets[ path ]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			if (path == RUNTIME_CRC32(temp.c_str( ))) {
				uint64_t ptr = std::uint64_t(klass);

				offsets.insert(std::make_pair(path, ptr));
				return klass;
			}
		}
	}

	return nullptr;
}

uint64_t static_field(uint32_t path) {
	if (map_contains_key(offsets, path))
		return std::uint32_t(offsets[ path ]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				if (RUNTIME_CRC32(t.c_str( )) == path) {
					uint32_t off = field->offset( );

					uint64_t ptr = *reinterpret_cast<uint64_t*>(std::uint64_t(klass->static_fields) + off);
					offsets.insert(std::make_pair(path, ptr));

					return off;
				}
			}
		}
	}

	return 0;
}


uint64_t field(uint32_t path) {
	if (map_contains_key(offsets, path))
		return std::uint32_t(offsets[ path ]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				if (RUNTIME_CRC32(t.c_str( )) == path) {
					uint32_t off = field->offset( );
					offsets.insert(std::make_pair(path, off));

					return off;
				}
			}
		}
	}

	return 0;
}

uint64_t method(uint32_t path) {
	if (map_contains_key(offsets, path))
		return offsets[ path ];

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			if (!klass) continue;

			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppMethod* mthd;
			void* iter = NULL;
			while (mthd = klass->methods(&iter)) {
				if (!mthd) continue;

				std::string temp2(temp + xorstr_("::") + mthd->name( ));

				if (mthd->paramCount( ) > 0) {
					temp2 = temp2 + xorstr_("(");
					for (int p = 0; p < mthd->paramCount( ); p++) {
						std::string t(mthd->getParam(p)->name( ));
						t = t.substr(t.find(xorstr_(".")) + 1);
						temp2 = temp2 + t + xorstr_(",");
					}
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2.substr(0, temp2.size( ) - 1);
					temp2 = temp2 + xorstr_("): ") + t.substr(t.find(".") + 1);
				}
				else {
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2 + xorstr_("(): ") + t.substr(t.find(".") + 1);
				}

				if (RUNTIME_CRC32(temp2.c_str( )) == path) {
					offsets.insert(std::make_pair(path, std::uint64_t(mthd)));
					return std::uint64_t(mthd);
				}
			}
		}
	}

	return 0;
}
#if defined _M_IX86
using ptr_t = uint32_t;
#elif defined _M_X64
using ptr_t = uint64_t;
#endif

enum UnmanagedCallingConvention
{
	UnmanagedCdecl,
	UnmanagedStdcall,
	UnmanagedFastcall,
};

template<typename t_Function>
class UnmanagedPointer
{
public:

	template<typename... t_FunctionParameters>
	auto operator()(t_FunctionParameters... params) {
		using result_type = decltype(std::declval<t_Function>( )(std::declval<t_FunctionParameters>( )...));
		using function_cdecl_ptr_t = result_type(__cdecl*)(t_FunctionParameters...);
		using function_stdcall_ptr_t = result_type(__stdcall*)(t_FunctionParameters...);
		using function_fastcall_ptr_t = result_type(_fastcall*)(t_FunctionParameters...);

		switch (this->m_CallingConvention) {
		case UnmanagedCdecl:
			return reinterpret_cast<function_cdecl_ptr_t>(this->m_Address)(params...);
		case UnmanagedStdcall:
			return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
		case UnmanagedFastcall:
			return reinterpret_cast<function_fastcall_ptr_t>(this->m_Address)(params...);
		}

		return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
	}

	UnmanagedPointer(ptr_t dwAddress, UnmanagedCallingConvention unmCallingConvention) {
		this->m_Address = dwAddress;
		this->m_CallingConvention = unmCallingConvention;
	}
private:
	ptr_t m_Address;
	UnmanagedCallingConvention m_CallingConvention;
};

#define STATIC_FUNCTION(method_path,name,ta) static inline UnmanagedPointer<ta> name = { METHOD(method_path), UnmanagedStdcall }

#define OFFSET(path) field(STATIC_CRC32(path))

#define METHOD(path) *reinterpret_cast<uint64_t*>(method(STATIC_CRC32(path)))

#define METHOD_INF(path) reinterpret_cast<void*>(method(STATIC_CRC32(path)))

#define CLASS(path) klass(STATIC_CRC32(path))

#define STATIC_FIELD(path) static_field(STATIC_CRC32(path))

#define FIELD(field_path,name,type) type& name() { \
		NP(type) \
		static auto off = OFFSET(field_path); \
		return mem::read<type>(this + off); }

#define OFFSET_FIELD(offset,name,type) type& name() { \
		NP(type) \
		static auto off = offset; \
		return *reinterpret_cast<type*>(this + off); }

#define ASSIGN_HOOK(method_path,hook) hook = reinterpret_cast<decltype(hook)>(METHOD(method_path))

void init_classes( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			uint64_t ptr = std::uint64_t(klass);

			offsets.insert(std::make_pair(RUNTIME_CRC32(temp.c_str( )), ptr));
		}
	}
}
void init_methods( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			if (!klass) continue;

			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppMethod* mthd;
			void* iter = NULL;
			while (mthd = klass->methods(&iter)) {
				if (!mthd) continue;

				std::string temp2(temp + xorstr_("::") + mthd->name( ));

				if (mthd->paramCount( ) > 0) {
					temp2 = temp2 + xorstr_("(");
					for (int p = 0; p < mthd->paramCount( ); p++) {
						std::string t(mthd->getParam(p)->name( ));
						t = t.substr(t.find(xorstr_(".")) + 1);
						temp2 = temp2 + t + xorstr_(",");
					}
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2.substr(0, temp2.size( ) - 1);
					temp2 = temp2 + xorstr_("): ") + t.substr(t.find(".") + 1);
				}
				else {
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2 + xorstr_("(): ") + t.substr(t.find(".") + 1);
				}

				offsets.insert(std::make_pair(RUNTIME_CRC32(temp2.c_str( )), std::uint64_t(mthd)));
			}
		}
	}
}
void init_fields( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				uint32_t off = field->offset( );
				offsets.insert(std::make_pair(RUNTIME_CRC32(t.c_str( )), off));
			}
		}
	}
}