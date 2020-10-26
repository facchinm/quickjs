#include "Arduino.h"
#include <fenv.h>
#include "netsocket/NetworkStack.h"

/*
 * The following symbol is simply the bitwise-inclusive OR of all floating-point
 * rounding direction constants defined above.
 */
#define	_ROUND_MASK		(FE_TONEAREST | FE_UPWARD | FE_DOWNWARD | FE_TOWARDZERO)

typedef enum {
    FP_RN=0,			/* round to nearest representable number */
    FP_RP=1,			/* round toward positive infinity */
    FP_RM=2,			/* round toward negative infinity */
    FP_RZ=3				/* round to zero (truncate) */
} fp_rnd;

extern	fp_rnd		_softfloat_float_rounding_mode;

extern "C" int fesetround(int round)
{
	unsigned int fpscr;

	/* Check whether requested rounding direction is supported */
	if (round & ~_ROUND_MASK)
		return -1;

	/* Set the rounding direction */
	/*
	_softfloat_float_rounding_mode &= ~_ROUND_MASK;
	_softfloat_float_rounding_mode |= round;
	*/

	__asm volatile("vmrs %0, fpscr" : "=r" (fpscr));
	fpscr &= ~(_ROUND_MASK << 22);
	fpscr |= round << 22;
	__asm volatile("vmsr fpscr, %0" :: "r" (fpscr));

	return 0;
}


extern "C" int pthread_mutex_lock(pthread_mutex_t* mut) {

}

extern "C" int pthread_mutex_unlock(pthread_mutex_t *) {

}

extern "C" int pthread_cond_signal(pthread_cond_t *) {

}

extern "C" int pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *) {

}

extern "C" int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *) {

}

extern "C" int pthread_cond_destroy(pthread_cond_t *) {

}

extern "C" int pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *) {

}

extern "C" int clock_gettime(clockid_t clk_id, struct timespec *tp) {
	return millis();
}

extern "C" int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout) {
	return ::select(FD_SETSIZE, readset, NULL, NULL, timeout);
}

/*
pipe
readlink
lstat
getcwd
dup2
dup
nanosleep
chdir
symlink
sysconf
chdir
setuid
setgid
waitpid
pclose
utimes
popen
realpath
select
*/