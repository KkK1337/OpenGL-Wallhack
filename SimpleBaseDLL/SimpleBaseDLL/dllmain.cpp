// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

//==============================================================================
void __stdcall CallFunction()
{
	__asm{push edx}
}
//==============================================================================

//==============================================================================
BOOL APIENTRY DllMain(HANDLE hInst, DWORD dwReason, LPVOID lpReserved)
{
	// Declare 2 strings title and caption
	wchar_t const *title = L"DLL Injected";
	wchar_t const *caption = L"Simple WallHack by KkK1337";
	

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		HookOpenGL();
		MessageBox(NULL, title, caption, MB_OK);
	}
	return TRUE;
}
//==============================================================================
