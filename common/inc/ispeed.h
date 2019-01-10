



/********PRE-PROCESSORS*******/
#include <stdio.h>
#include <stdlib.h>



#ifdef linux
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <malloc.h>
#include <stdbool.h>
#include <errno.h>
#endif