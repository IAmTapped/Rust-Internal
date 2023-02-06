template <typename T>
T Read(DWORD64 address) {
	if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000) {
		return *(T*)(address);
	}
	else {
		return T{};
	}
}
template <typename T>
void Write(DWORD64 address, T data) {
	if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000) {
		*(T*)(address) = data;
	}
}
#define read(Addr, Type) Read<Type>((DWORD64)Addr)
#define write(Addr, Data, Type) Write<Type>((DWORD64)Addr, Data);
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

#define OFFSETFIELD(type, name, offset) struct {unsigned char _pad##__COUNTER__[offset]; type name;}