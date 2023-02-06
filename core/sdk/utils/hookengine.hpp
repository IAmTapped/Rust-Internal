#include "detours/detours.h"

namespace hookengine {
	template<typename Function>
	void hook(Function*& func, void* detour) {
		DetourTransactionBegin( );
		DetourUpdateThread(GetCurrentThread( ));
		DetourAttach(&(PVOID&)func, detour);
		DetourTransactionCommit( );
	}

	template<typename Function>
	void unhook(Function*& func, void* detour) {
		DetourTransactionBegin( );
		DetourUpdateThread(GetCurrentThread( ));
		DetourDetach(&(PVOID&)func, detour);
		DetourTransactionCommit( );
	}
}