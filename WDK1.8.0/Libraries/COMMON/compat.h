#ifndef _COMPAT_H_
	#define _COMPAT_H_
	
	#if _WIN32	// Windows
		#include "win/winc.h"
	#else 		// Linux
		#include "linux/linuxc.h"
	#endif 		// Windows End
	
	
	#ifndef _WIN32
		#define InitKeyboard() init_keyboard()
	#else
		#define InitKeyboard() /* nada */
	#endif

		
	#ifndef _WIN32
		#define ResetKeyboard() reset_keyboard()
	#else
		#define ResetKeyboard() /* nada */
	#endif
	
	#ifdef _WIN32
		#define STDCALL unsigned __stdcall
	#else
		#define STDCALL void*
	#endif

	
#endif