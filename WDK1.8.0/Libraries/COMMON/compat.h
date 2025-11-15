#ifndef _COMPAT_H_
	#define _COMPAT_H_
	
	#if _WIN32	// Windows
		#include "win/winc.h"
	#else 		// Linux
		#include "linux/linuxc.h"
	#endif 		// Windows End
	
#endif