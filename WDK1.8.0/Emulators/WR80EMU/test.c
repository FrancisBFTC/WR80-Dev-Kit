#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <pthread.h>

int main(){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return 0;
}
