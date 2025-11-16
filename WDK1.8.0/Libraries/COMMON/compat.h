#ifndef _COMPAT_H_
	#define _COMPAT_H_
	
	#ifdef _WIN32
		#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
			#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
		#endif
	#endif
	
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
	
	#ifdef _WIN32
		#define CreateThread(thread, func) \
			unsigned tid; \
			thread = (HANDLE)_beginthreadex(NULL, 0, func, NULL, 0, &tid)
	#else
		#define CreateThread(thread, func) \
			pthread_create(&thread, NULL, func, NULL)
	#endif
	
	#ifdef _WIN32
		#define CloseThread(thread, timeout_ms) \
			WaitForSingleObject(thread, timeout_ms); \
			CloseHandle(thread)
	#else
		#define CloseThread(thread, timeout_ms)                   \
			do {                                                  \
				struct timespec ts;                               \
				clock_gettime(CLOCK_REALTIME, &ts);               \
				ts.tv_sec  += (timeout_ms) / 1000;                \
				ts.tv_nsec += ((timeout_ms) % 1000) * 1000000;    \
				if (ts.tv_nsec >= 1000000000) {                   \
					ts.tv_sec++;                                  \
					ts.tv_nsec -= 1000000000;                     \
				}                                                 \
				pthread_timedjoin_np(thread, NULL, &ts);          \
			} while(0)
	#endif
	
	#ifdef _WIN32
	    #define CriticalEnter(cs) EnterCriticalSection(&(cs))
	    #define CriticalLeave(cs) LeaveCriticalSection(&(cs))
	#else
	    #define CriticalEnter(cs) pthread_mutex_lock(&(cs))
	    #define CriticalLeave(cs) pthread_mutex_unlock(&(cs))
	#endif

	#ifdef _WIN32
		#define EnableANSI() enableVTMode()
	#else
		#define EnableANSI() /* nada */
	#endif
	
#endif
