/**********************************************************************
	File name: dllmain.cpp

	Defines the entry point for the DLL application.
**********************************************************************/

#include "stdafx.h"

// Every DLL file must have a DllMain function, which is the entry point for the library. 
// Unless you must do a specific initialization of the library, the default DllMain is sufficient.
// APIENTRY is a shortcut to __stdcall
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	// Perform actions based on the reason for calling.
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH: {
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.      
			break;
		}
		case DLL_THREAD_ATTACH: {
			// Do thread-specific initialization.
			break;
		}
		case DLL_THREAD_DETACH: {
			// Do thread-specific cleanup.            
			break;
		}
		case DLL_PROCESS_DETACH: {
			// Perform any necessary cleanup.
			break;
		}
    }

    return TRUE;
}