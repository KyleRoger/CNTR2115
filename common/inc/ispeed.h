



/********PRE-PROCESSORS*******/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>

#ifdef linux
	#include <pthread.h>
	#include <sys/wait.h>
#endif