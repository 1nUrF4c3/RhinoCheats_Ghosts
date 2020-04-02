//=====================================================================================

#pragma once

//=====================================================================================

namespace NeoGenesys
{
	template<typename Return, typename... Parameters>
	inline Return VariadicCall(DWORD_PTR address, Parameters... params)
	{
		typedef Return(*tFunction)(Parameters...);
		tFunction Function = (tFunction)address;
		return Function(params...);
	}
	/*
	//=====================================================================================
	*/
	template<typename... Parameters>
	inline std::string VariadicText(LPCSTR format, Parameters... params)
	{
		char szBuffer[4096] = { NULL };
		sprintf_s(szBuffer, format, params...);
		return szBuffer;
	}
	/*
	//=====================================================================================
	*/
	template<typename Type>
	inline void WriteMemoryProtected(LPVOID address, Type value)
	{
		DWORD dwProtection = PAGE_EXECUTE_READWRITE;

		VirtualProtect(address, sizeof(Type), dwProtection, &dwProtection);
		*(Type*)address = value;
		VirtualProtect(address, sizeof(Type), dwProtection, &dwProtection);
	}
	/*
	//=====================================================================================
	*/
	static MODULEINFO GetModuleInfo(LPCSTR name)
	{
		MODULEINFO ModuleInfo = { NULL };
		HMODULE hModule = GetModuleHandle(name);

		if (!hModule)
			return ModuleInfo;

		GetModuleInformation(GetCurrentProcess(), hModule, &ModuleInfo, sizeof(MODULEINFO));
		return ModuleInfo;
	}
	/*
	//=====================================================================================
	*/
	static bool Match(const BYTE* data, const BYTE* signature, const char* mask)
	{
		for (; *mask; ++mask, ++data, ++signature)
			if (*mask == 'x' && *data != *signature)
				return false;

		return !(*mask);
	}
	/*
	//=====================================================================================
	*/
	static DWORD_PTR FindPattern(const char* signature, const char* mask)
	{
		DWORD_PTR dwAddress = (DWORD_PTR)GetModuleHandle(NULL);
		DWORD_PTR dwLen = GetModuleInfo(NULL).SizeOfImage;

		for (DWORD_PTR i = 0; i < dwLen; i++)
			if (Match((BYTE*)(dwAddress + i), (BYTE*)signature, mask))
				return (DWORD_PTR)(dwAddress + i);

		return NULL;
	}
	/*
	//=====================================================================================
	*/
	static DWORD_PTR FindPattern(DWORD_PTR address, DWORD_PTR length, const char* signature, const char* mask)
	{
		for (DWORD_PTR i = 0; i < length; i++)
			if (Match((BYTE*)(address + i), (BYTE*)signature, mask))
				return (DWORD_PTR)(address + i);

		return NULL;
	}
	/*
	//=====================================================================================
	*/
	static DWORD_PTR ReadPointer(DWORD_PTR address, DWORD_PTR offset)
	{
		if (!address)
			return 0;

		if (sizeof(DWORD_PTR) == 0x8)
			return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));

		return (DWORD_PTR)*(DWORD*)(address + offset);
	}
}