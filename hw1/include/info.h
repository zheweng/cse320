#ifndef INFO_H
#define INFO_H
#include <stdlib.h>
#include <stdio.h>

/* #ifdef DEBUG
	#define debug(fmt, ...) do{fprintf(stderr, "DEBUG: %s:%s:%d " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)}while(0)
#else
	#define debug(fmt, ...)
#endif

#endif */

#ifdef  INFO
	#define info(fmt, ...) do{fprintf(stderr, fmt, ##__VA_ARGS__ );}while(0);
#else
	#define info(fmt, ...)

#endif

#endif
