#pragma once

#include "CSX.h"
#include <assert.h>

//VMT UNDETECTED HOOKS

namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(void* base, size_t len, std::uint32_t flags)
		{
			_base = base;
			_length = len;
			if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
				throw std::runtime_error("Failed to protect region.");
		}
		~protect_guard()
		{
			VirtualProtect(_base, _length, _old, (PDWORD)&_old);
		}

	private:
		void*         _base;
		size_t        _length;
		std::uint32_t _old;
	};
}

class vfunc_hook
{
public:
	uintptr_t * search_free_data_page(const char * module_name, const std::size_t vmt_size);
	vfunc_hook();
	vfunc_hook(void* base);
	~vfunc_hook();

	bool setup(void * base, const char * moduleName = nullptr);
	template<typename T>
	void hook_index(int index, T fun)
	{
		assert(index >= 0 && index <= (int)vftbl_len);
		new_vftb1[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
	}
	void unhook_index(int index)
	{
		new_vftb1[index] = old_vftbl[index];
	}
	void unhook_all()
	{
		try {
			if (old_vftbl != nullptr) {
				auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)class_base = old_vftbl;
				old_vftbl = nullptr;
			}
		}
		catch (...) {
		}
	}

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

private:

	static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);

	void*           class_base;
	std::size_t     vftbl_len;
	std::uintptr_t* new_vftb1;
	std::uintptr_t* old_vftbl;
	LPCVOID         search_base = nullptr;
	bool wasAllocated = false;
};




// VMT DETECTED HOOKS

namespace CSX
{
	namespace Hook
	{
		PVOID WriteVTable(PVOID pTablePtr, PVOID pFuncAddress, DWORD dwIndex);

		class VTable
		{
		public:


			VTable();
			//[swap_lines]
			bool InitTable(PVOID pTablePtrPtr);

			void HookIndex(DWORD dwIndex, PVOID pNewAddress);

			PVOID GetFuncAddress(DWORD dwIndex);
			PVOID GetHookIndex(DWORD dwIndex, PVOID pNewAddress);
			bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx);
			void UnHook();
			void ReHook();
			//[/swap_lines]
		private:
			//[swap_lines]
			PVOID * pPtrPtrTable;
			PVOID* pPtrOldTable;
			PVOID* pPtrNewTable;

			DWORD dwCountFunc;
			DWORD dwSizeTable;
			//[/swap_lines]
		};
	}
}






